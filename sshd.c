//->
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name in ISO-10646 UTF-8 encoding [RFC3629]
//string    service name in US-ASCII
//string    "publickey"
//boolean   FALSE
//string    public key algorithm name
//string    public key blob

//<-
//byte      SSH_MSG_USERAUTH_PK_OK
//string    public key algorithm name from the request
//string    public key blob from the request

//->
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name
//string    service name
//string    "publickey"
//boolean   TRUE
//string    public key algorithm name
//string    public key to be used for authentication
//string    signature

//signature of
//string    session identifier
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name
//string    service name
//string    "publickey"
//boolean   TRUE
//string    public key algorithm name
//string    public key to be used for authentication

//<-
//SSH_MSG_USERAUTH_PK_OK

read_line(fd: int, buf: *byte, max: int): int {
	var len: int;
	var c: int;
	len = 0;
	loop {
		if len == max {
			return 0;
		}

		c = fdgetc(fd);
		if c == '\n' {
			buf[len] = 0:byte;
			return len;
		}

		if c == -1 {
			return 0;
		}

		if c == '\r' {
			continue;
		}

		buf[len] = c:byte;
		len = len + 1;
	}
}

writeall(fd: int, buf: *byte, n: int): int {
	var ret: int;
	loop {
		if n == 0 {
			return 0;
		}

		ret = write(fd, buf, n);
		if ret < 0 {
			return ret;
		}

		buf = &buf[ret];
		n = n - ret;
	}
}

read_fill(fd: int, buf: *byte, n: int): int {
	var ret: int;
	loop {
		if n == 0 {
			return 0;
		}

		ret = read(fd, buf, n);
		if ret <= 0 {
			return -1;
		}

		buf = &buf[ret];
		n = n - ret;
	}
}

_rdrand(): int;
read_rand(buf: *byte, n: int) {
	var i: int;
	i = 0;
	loop {
		if i == n {
			break;
		}
		buf[i] = _rdrand():byte;
		i = i + 1;
	}
}

read_frame(ctx: *sshd_ctx) {
	var len: int;
	var padlen: int;
	var min_size: int;
	var align: int;

	min_size = ctx.blksize;
	align = ctx.blksize;

	if min_size < 16 {
		min_size = 16;
	}

	if align < 8 {
		align = 8;
	}

	if read_fill(ctx.fd, ctx.buf, 5) != 0 {
		exit(1);
	}

	// todo decrypt

	len = ctx.buf[3]:int
		| (ctx.buf[2]:int << 8)
		| (ctx.buf[1]:int << 16)
		| (ctx.buf[0]:int << 24);

	padlen = ctx.buf[4]:int;

	if len > ctx.bufsz - 4 {
		exit(1);
	}

	if len < 1 || padlen > len - 1 {
		exit(1);
	}

	if padlen < 4 {
		exit(1);
	}

	if len < min_size - 4 {
		exit(1);
	}

	if (len + 4) % align != 0 {
		exit(1);
	}

	if read_fill(ctx.fd, &ctx.buf[5], len - 1) != 0 {
		exit(1);
	}

	// todo decrypt + mac

	ctx.frame = &ctx.buf[5];
	ctx.framelen = len - padlen - 1;
	ctx.index = 0;
}

write_frame(ctx: *sshd_ctx) {
	var len: int;
	var padlen: int;
	var min_size: int;
	var align: int;

	align = ctx.blksize;
	min_size = ctx.blksize;

	if align < 8 {
		align = 8;
	}

	if min_size < 16 {
		min_size = 16;
	}

	padlen = align - ((ctx.framelen + 5) % align);
	if padlen < 4 {
		padlen = padlen + align;
	}

	if ctx.framelen + padlen + 5 < min_size {
		padlen = padlen + align;
	}

	if padlen > ctx.bufsz - 5 - ctx.framelen {
		exit(1);
	}

	if padlen > 255 {
		exit(1);
	}

	len = padlen + ctx.framelen + 1;

	ctx.frame = &ctx.frame[-5];
	ctx.frame[0] = (len >> 24):byte;
	ctx.frame[1] = (len >> 16):byte;
	ctx.frame[2] = (len >> 8):byte;
	ctx.frame[3] = len:byte;
	ctx.frame[4] = padlen:byte;
	read_rand(&ctx.frame[ctx.framelen + 5], padlen);
	ctx.framelen = ctx.framelen + 5 + padlen;

	// todo encrypt + mac

	if writeall(ctx.fd, ctx.frame, len + 4) != 0 {
		exit(1);
	}
}

enum {
	SSH_MSG_KEXINIT = 0x14,
	SSH_MSG_NEWKEYS = 0x15,
	SSH_MSG_ECDH_INIT = 0x1e,
	SSH_MSG_ECDH_REPLY = 0x1f,
}

doversion(ctx: *sshd_ctx) {
	var ident: *byte;
	var n: int;

	//SSH-2.0-VERSION<CR><LF>
	n = read_line(ctx.fd, ctx.buf, ctx.bufsz);
	if n < 8 || memcmp(ctx.buf, "SSH-2.0-", 8) != 0 {
		exit(1);
	}

	ctx.cver = alloc(ctx.a, n + 1);
	memcpy(ctx.cver, ctx.buf, n);
	ctx.cver[n] = 0:byte;

	if writeall(ctx.fd, ctx.sver, strlen(ctx.sver)) != 0 {
		exit(1);
	}

	if writeall(ctx.fd, "\r\n", 2) != 0 {
		exit(1);
	}
}

struct ssh_str {
	len: int;
	s: *byte;
}

struct ssh_kexinit {
	cookie: ssh_str;
	kex_algorithms: ssh_str;
	server_host_key_algorithms: ssh_str;
	encryption_algorithms_client_to_server: ssh_str;
	encryption_algorithms_server_to_client: ssh_str;
	mac_algorithms_client_to_server: ssh_str;
	mac_algorithms_server_to_client: ssh_str;
	compression_algorithms_client_to_server: ssh_str;
	compression_algorithms_server_to_client: ssh_str;
	languages_client_to_server: ssh_str;
	languages_server_to_client: ssh_str;
	first_kex_packet_follows: int;
	reserved: int;
}

decode_u8(s: *int, ctx: *sshd_ctx) {
	if ctx.framelen < 1 || ctx.index > ctx.framelen - 1 {
		exit(1);
	}
	*s = ctx.frame[ctx.index]:int;
	ctx.index = ctx.index + 1;
}

decode_u32(s: *int, ctx: *sshd_ctx) {
	if ctx.framelen < 4 || ctx.index > ctx.framelen - 4 {
		exit(1);
	}
	*s = (ctx.frame[ctx.index]:int << 24)
		| (ctx.frame[ctx.index + 1]: int << 16)
		| (ctx.frame[ctx.index + 2]: int << 8)
		| ctx.frame[ctx.index + 3]: int;
	ctx.index = ctx.index + 4;
}

decode_str(s: *ssh_str, ctx: *sshd_ctx) {
	decode_u32(&s.len, ctx);
	if s.len < 0 || ctx.framelen < s.len || ctx.index > ctx.framelen - s.len {
		exit(1);
	}
	s.s = &ctx.frame[ctx.index];
	ctx.index = ctx.index + s.len;
}

decode_cookie(s: *ssh_str, ctx: *sshd_ctx) {
	if ctx.framelen < 16 || ctx.index > ctx.framelen - 16 {
		exit(1);
	}
	s.s = &ctx.frame[ctx.index];
	s.len = 16;
	ctx.index = ctx.index + 16;
}

decode_bool(s: *int, ctx: *sshd_ctx) {
	decode_u8(s, ctx);
	*s = (*s != 0);
}

decode_kexinit(kex: *ssh_kexinit, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_KEXINIT {
		exit(1);
	}
	decode_cookie(&kex.cookie, ctx);
	decode_str(&kex.kex_algorithms, ctx);
	decode_str(&kex.server_host_key_algorithms, ctx);
	decode_str(&kex.encryption_algorithms_client_to_server, ctx);
	decode_str(&kex.encryption_algorithms_server_to_client, ctx);
	decode_str(&kex.mac_algorithms_client_to_server, ctx);
	decode_str(&kex.mac_algorithms_server_to_client, ctx);
	decode_str(&kex.compression_algorithms_client_to_server, ctx);
	decode_str(&kex.compression_algorithms_server_to_client, ctx);
	decode_str(&kex.languages_client_to_server, ctx);
	decode_str(&kex.languages_server_to_client, ctx);
	decode_bool(&kex.first_kex_packet_follows, ctx);
	decode_u32(&kex.reserved, ctx);
	if ctx.index != ctx.framelen {
		exit(1);
	}
}

decode_newkeys(kex: *ssh_newkeys, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_NEWKEYS {
		exit(1);
	}
}

struct ssh_ecdh_init {
	qc: ssh_str;
}

decode_ecdh_init(dh: *ssh_ecdh_init, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_ECDH_INIT {
		exit(1);
	}
	decode_str(&dh.qc, ctx);
}

set_str(s: *ssh_str, v: *byte) {
	s.s = v;
	s.len = strlen(v);
}

struct _ssh_cookie {
	x0: int;
	x1: int;
}

clear_frame(ctx: *sshd_ctx) {
	ctx.frame = &ctx.buf[5];
	ctx.framelen = ctx.bufsz - 5;
	ctx.index = 0;
}

finish_frame(ctx: *sshd_ctx) {
	ctx.framelen = ctx.index;
}

encode_u8(x: *int, ctx: *sshd_ctx) {
	if ctx.framelen < 1 || ctx.index > ctx.framelen - 1 {
		exit(1);
	}
	ctx.frame[ctx.index] = (*x):byte;
	ctx.index = ctx.index + 1;
}

encode_cookie(x: *ssh_str, ctx: *sshd_ctx) {
	if x.len != 16 || ctx.framelen < 16 || ctx.index > ctx.framelen - 16 {
		exit(1);
	}
	memcpy(&ctx.frame[ctx.index], x.s, 16);
	ctx.index = ctx.index + 16;
}

encode_bool(x: *int, ctx: *sshd_ctx) {
	var y: int;
	y = *x != 0;
	encode_u8(&y, ctx);
}

encode_u32(x: *int, ctx: *sshd_ctx) {
	if ctx.framelen < 4 || ctx.index > ctx.framelen - 4 {
		exit(1);
	}
	ctx.frame[ctx.index] = (*x >> 24): byte;
	ctx.frame[ctx.index + 1] = (*x >> 16): byte;
	ctx.frame[ctx.index + 2] = (*x >> 8): byte;
	ctx.frame[ctx.index + 3] = (*x): byte;
	ctx.index = ctx.index + 4;
}

encode_blob(x: *ssh_blob, ctx: *sshd_ctx) {
	var len: int;
	len = x.alg.len + x.blob.len + 8;
	encode_u32(&len, ctx);
	encode_str(&x.alg, ctx);
	encode_str(&x.blob, ctx);
}

encode_str(x: *ssh_str, ctx: *sshd_ctx) {
	encode_u32(&x.len, ctx);
	if ctx.framelen < x.len || ctx.index > ctx.framelen - x.len {
		exit(1);
	}
	memcpy(&ctx.frame[ctx.index], x.s, x.len);
	ctx.index = ctx.index + x.len;
}

encode_kexinit(kex: *ssh_kexinit, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_KEXINIT;
	encode_u8(&tag, ctx);
	encode_cookie(&kex.cookie, ctx);
	encode_str(&kex.kex_algorithms, ctx);
	encode_str(&kex.server_host_key_algorithms, ctx);
	encode_str(&kex.encryption_algorithms_client_to_server, ctx);
	encode_str(&kex.encryption_algorithms_server_to_client, ctx);
	encode_str(&kex.mac_algorithms_client_to_server, ctx);
	encode_str(&kex.mac_algorithms_server_to_client, ctx);
	encode_str(&kex.compression_algorithms_client_to_server, ctx);
	encode_str(&kex.compression_algorithms_server_to_client, ctx);
	encode_str(&kex.languages_client_to_server, ctx);
	encode_str(&kex.languages_server_to_client, ctx);
	encode_bool(&kex.first_kex_packet_follows, ctx);
	encode_u32(&kex.reserved, ctx);
	finish_frame(ctx);
}

struct ssh_newkeys {
}

encode_newkeys(kex: *ssh_newkeys, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_NEWKEYS;
	encode_u8(&tag, ctx);
	finish_frame(ctx);
}

struct ssh_blob {
	alg: ssh_str;
	blob: ssh_str;
}

struct ssh_ecdh_reply {
	ks: ssh_blob;
	qs: ssh_str;
	sig: ssh_blob;
}

set_blob(d: *ssh_blob, alg: *byte, b: *byte, n: int) {
	set_str(&d.alg, alg);
	d.blob.s = b;
	d.blob.len = n;
}

encode_ecdh_reply(dh: *ssh_ecdh_reply, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_ECDH_REPLY;
	encode_u8(&tag, ctx);
	encode_blob(&dh.ks, ctx);
	encode_str(&dh.qs, ctx);
	encode_blob(&dh.sig, ctx);
	finish_frame(ctx);
}

ssh_u32_sha256_update(ctx: *sha256_ctx, n: int) {
	var a: byte;
	a = (n >> 24):byte;
	sha256_update(ctx, &a, 1);
	a = (n >> 16):byte;
	sha256_update(ctx, &a, 1);
	a = (n >> 8):byte;
	sha256_update(ctx, &a, 1);
	a = n:byte;
	sha256_update(ctx, &a, 1);
}

ssh_blob_sha256_update(ctx: *sha256_ctx, b: *ssh_blob) {
	ssh_u32_sha256_update(ctx, b.alg.len + b.blob.len + 8);
	ssh_str_sha256_update(ctx, b.alg.s, b.alg.len);
	ssh_str_sha256_update(ctx, b.blob.s, b.blob.len);
}

ssh_str_sha256_update(ctx: *sha256_ctx, b: *byte, n: int) {
	ssh_u32_sha256_update(ctx, n);
	sha256_update(ctx, b, n);
}

ssh_mpint_sha256_update(ctx: *sha256_ctx, b: *byte, n: int) {
	var a: byte;

	loop {
		if n == 1 {
			break;
		}

		if b[0]:int != 0 {
			break;
		}

		b = &b[1];
		n = n - 1;
	}

	if b[0]:int & 128 {
		ssh_u32_sha256_update(ctx, n + 1);
		a = 0:byte;
		sha256_update(ctx, &a, 1);
		sha256_update(ctx, b, n);
	} else {
		ssh_u32_sha256_update(ctx, n);
		sha256_update(ctx, b, n);
	}
}

dokex(ctx: *sshd_ctx) {
	var cookie: _ssh_cookie;
	var ckex: ssh_kexinit;
	var skex: ssh_kexinit;
	var newkeys: ssh_newkeys;
	var cdh: ssh_ecdh_init;
	var sdh: ssh_ecdh_reply;
	var rs: _ed25519_point;
	var qs: _ed25519_point;
	var k: _ed25519_point;
	var ehctx: sha256_ctx;
	var eh: _sha256_digest;
	var sig: _ed25519_sig;

	// <- SSH_MSG_KEXINIT
	read_frame(ctx);
	decode_kexinit(&ckex, ctx);
	memcpy(ctx.ckex, ctx.buf, ctx.framelen);
	ctx.ckexlen = ctx.framelen;

	if ckex.first_kex_packet_follows {
		exit(1);
	}

	// -> SSH_MSG_KEXINIT
	read_rand((&cookie): *byte, 16);
	skex.cookie.len = 16;
	skex.cookie.s = (&cookie): *byte;
	set_str(&skex.kex_algorithms, "curve25519-sha256");
	set_str(&skex.server_host_key_algorithms, "ssh-ed25519");
	set_str(&skex.encryption_algorithms_client_to_server, "chacha20-poly1305@openssh.com");
	set_str(&skex.encryption_algorithms_server_to_client, "chacha20-poly1305@openssh.com");
	set_str(&skex.mac_algorithms_client_to_server, "none");
	set_str(&skex.mac_algorithms_server_to_client, "none");
	set_str(&skex.compression_algorithms_client_to_server, "none");
	set_str(&skex.compression_algorithms_server_to_client, "none");
	set_str(&skex.languages_client_to_server, "");
	set_str(&skex.languages_server_to_client, "");
	skex.first_kex_packet_follows = 0;
	skex.reserved = 0;

	encode_kexinit(&skex, ctx);
	memcpy(ctx.skex, ctx.frame, ctx.framelen);
	ctx.skexlen = ctx.index;

	write_frame(ctx);

	// <- SSH_MSG_ECDH_INIT
	read_frame(ctx);
	decode_ecdh_init(&cdh, ctx);

	if cdh.qc.len != 32 {
		die("wtf");
		exit(1);
	}

	read_rand((&rs):*byte, 32);

	x25519_base((&qs):*byte);

	if !x25519((&qs):*byte, (&qs):*byte, (&rs):*byte) {
		die("wtf");
		exit(1);
	}

	if !x25519((&k):*byte, cdh.qc.s, (&rs):*byte) {
		die("wtf");
		exit(1);
	}

	set_blob(&sdh.ks, "ssh-ed25519", (&ctx.pub):*byte, 32);
	sdh.qs.s = (&qs):*byte;
	sdh.qs.len = 32;
	set_blob(&sdh.sig, "ssh-ed25519", (&sig):*byte, 64);

	sha256_init(&ehctx);
	ssh_str_sha256_update(&ehctx, ctx.cver, strlen(ctx.cver));
	ssh_str_sha256_update(&ehctx, ctx.sver, strlen(ctx.sver));
	ssh_str_sha256_update(&ehctx, ctx.ckex, ctx.ckexlen);
	ssh_str_sha256_update(&ehctx, ctx.skex, ctx.skexlen);
	ssh_blob_sha256_update(&ehctx, &sdh.ks);
	ssh_str_sha256_update(&ehctx, cdh.qc.s, 32);
	ssh_str_sha256_update(&ehctx, (&qs):*byte, 32);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_final((&eh):*byte, &ehctx);

	ed25519_sign((&sig):*byte, (&ctx.priv):*byte, (&eh):*byte, 32);

	// -> SSH_MSG_ECDH_REPLY
	encode_ecdh_reply(&sdh, ctx);
	write_frame(ctx);

	// <- SSH_MSG_NEWKEYS
	read_frame(ctx);
	decode_newkeys(&newkeys, ctx);

	// -> SSH_MSG_NEWKEYS
	encode_newkeys(&newkeys, ctx);
	write_frame(ctx);

	if !ctx.has_session_id {
		memcpy((&ctx.session_id):*byte, (&eh):*byte, 32);
		ctx.has_session_id = 1;
	}

	memcpy((&ctx.eh):*byte, (&eh):*byte, 32);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "A", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.iv_client_to_server):*byte, &ehctx);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "B", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.iv_server_to_client):*byte, &ehctx);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "C", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.ek_client_to_server):*byte, &ehctx);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "D", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.ek_server_to_client):*byte, &ehctx);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "E", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.ik_client_to_server):*byte, &ehctx);

	sha256_init(&ehctx);
	ssh_mpint_sha256_update(&ehctx, (&k):*byte, 32);
	sha256_update(&ehctx, (&ctx.eh):*byte, 32);
	sha256_update(&ehctx, "F", 1);
	sha256_update(&ehctx, (&ctx.session_id):*byte, 32);
	sha256_final((&ctx.ik_server_to_client):*byte, &ehctx);
}

doauth(ctx: *sshd_ctx) {
	var k: ed25519_pub;
	unhex((&k):*byte, "5afb3032d86d60d700e1782bfe8083ff95504cecbda531bef5ba8cdd20f34c82");
	// todo auth
	exit(1);
}

cmain(ctx: *sshd_ctx) {
	doversion(ctx);
	dokex(ctx);
	doauth(ctx);
	// todo main loop
	exit(0);
}

struct sockaddr {
	fpa: int;
	pad: int;
}

struct sshd_ctx {
	fd: int;
	a: *alloc;
	priv: _ed25519_priv;
	pub: _ed25519_pub;
	buf: *byte;
	bufsz: int;
	frame: *byte;
	framelen: int;
	index: int;
	sver: *byte;
	cver: *byte;
	skex: *byte;
	skexlen: int;
	ckex: *byte;
	ckexlen: int;
	blksize: int;
	has_session_id: int;
	session_id: _sha256_digest;
	eh: _sha256_digest;
	iv_client_to_server: _sha256_digest;
	iv_server_to_client: _sha256_digest;
	ek_client_to_server: _sha256_digest;
	ek_server_to_client: _sha256_digest;
	ik_client_to_server: _sha256_digest;
	ik_server_to_client: _sha256_digest;
}

main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var cfd: int;
	var port: int;
	var sa: sockaddr;
	var ctx: sshd_ctx;
	var a: alloc;
	setup_alloc(&a);

	bzero((&ctx):*byte, sizeof(ctx));

	ed25519_pub((&ctx.pub):*byte, (&ctx.priv):*byte);

	ctx.a = &a;
	ctx.bufsz = 64 * 1024;
	ctx.buf = alloc(ctx.a, ctx.bufsz);
	ctx.skex = alloc(ctx.a, 4096);
	ctx.ckex = alloc(ctx.a, 4096);
	ctx.sver = "SSH-2.0-omiltem";
	ctx.blksize = 8;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if fd < 0 {
		die("failed to open socket");
	}

	port = 1234;
	sa.fpa = AF_INET | ((port & 0xff) << 24) | (((port >> 8) & 0xff) << 16);
	sa.pad = 0;
	if bind(fd, (&sa):*byte, sizeof(sa)) != 0 {
		die("failed to bind");
	}

	if listen(fd, 128) != 0 {
		die("failed to listen");
	}

	loop {
		ctx.fd = accept(fd, 0:*byte, 0:*int);
		if ctx.fd < 0 {
			continue;
		}

		if fork() == 0 {
			close(fd);
			cmain(&ctx);
			exit(1);
		}

		close(ctx.fd);
	}
}
