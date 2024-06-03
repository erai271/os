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

struct _ssh_nonce {
	x0: int;
	x1: int;
}

read_frame(ctx: *sshd_ctx) {
	var len: int;
	var padlen: int;
	var minlen: int;
	var align: int;
	var maclen: int;
	var tmp: int;
	var _nonce: _ssh_nonce;
	var nonce: *byte;
	var seq: int;
	var _mackey: _sha512_digest;
	var mackey: *byte;
	var _mac: _sha256_digest;
	var mac: *byte;

	nonce = (&_nonce):*byte;
	mackey = (&_mackey):*byte;
	mac  = (&_mac):*byte;

	if read_fill(ctx.fd, ctx.buf, 4) != 0 {
		die("truncated");
	}

	if ctx.en_client_to_server {
		nonce[0] = 0:byte;
		nonce[1] = 0:byte;
		nonce[2] = 0:byte;
		nonce[3] = 0:byte;
		nonce[4] = (ctx.seq_client_to_server >> 56):byte;
		nonce[5] = (ctx.seq_client_to_server >> 48):byte;
		nonce[6] = (ctx.seq_client_to_server >> 40):byte;
		nonce[7] = (ctx.seq_client_to_server >> 32):byte;
		nonce[8] = (ctx.seq_client_to_server >> 24):byte;
		nonce[9] = (ctx.seq_client_to_server >> 16):byte;
		nonce[10] = (ctx.seq_client_to_server >> 8):byte;
		nonce[11] = ctx.seq_client_to_server:byte;

		seq = 0;
		chacha20_stream((&tmp):*byte, ctx.buf, 4, &seq, &((&ctx.ek_client_to_server):*byte)[32], nonce);

		minlen = 16;
		align = 8;
		maclen = 16;

		len = ((&tmp):*byte)[3]:int
			| (((&tmp):*byte)[2]:int << 8)
			| (((&tmp):*byte)[1]:int << 16)
			| (((&tmp):*byte)[0]:int << 24);

		if len % align != 0 {
			die("not aligned");
		}
	} else {
		minlen = 16;
		align = 8;
		maclen = 0;

		len = ctx.buf[3]:int
			| (ctx.buf[2]:int << 8)
			| (ctx.buf[1]:int << 16)
			| (ctx.buf[0]:int << 24);

		if (len + 4) % align != 0 {
			die("not aligned");
		}
	}

	if len > ctx.bufsz - 4 - maclen {
		die("too long");
	}

	if len < minlen - 4 {
		die("too short");
	}

	if read_fill(ctx.fd, &ctx.buf[4], len + maclen) != 0 {
		die("truncated");
	}

	if ctx.en_client_to_server {
		bzero(mackey, 64);
		seq = 0;
		chacha20_stream(mackey, mackey, 64, &seq, (&ctx.ek_client_to_server):*byte, nonce);
		poly1305(mac, mackey, ctx.buf, len + 4);
		if memcmp(mac, &ctx.buf[len + 4], 16) {
			die("mac");
		}
		memcpy(ctx.buf, (&tmp):*byte, 4);
		seq = 64;
		chacha20_stream(&ctx.buf[4], &ctx.buf[4], len, &seq, (&ctx.ek_client_to_server):*byte, nonce);
	}

	padlen = ctx.buf[4]:int;

	if padlen < 4 || padlen > len - 1 {
		die("bad padding");
	}

	ctx.seq_client_to_server = (ctx.seq_client_to_server + 1) & (-1 >> 32);
	ctx.frame = &ctx.buf[5];
	ctx.framelen = len - padlen - 1;
	ctx.index = 0;
}

write_frame(ctx: *sshd_ctx) {
	var len: int;
	var padlen: int;
	var minlen: int;
	var align: int;
	var maclen: int;
	var _nonce: _ssh_nonce;
	var nonce: *byte;
	var seq: int;
	var _mackey: _sha512_digest;
	var mackey: *byte;

	nonce = (&_nonce):*byte;
	mackey = (&_mackey):*byte;

	if ctx.en_server_to_client {
		minlen = 16;
		align = 8;
		maclen = 16;
		padlen = align - ((ctx.framelen + 1) % align);

		nonce[0] = 0:byte;
		nonce[1] = 0:byte;
		nonce[2] = 0:byte;
		nonce[3] = 0:byte;
		nonce[4] = (ctx.seq_server_to_client >> 56):byte;
		nonce[5] = (ctx.seq_server_to_client >> 48):byte;
		nonce[6] = (ctx.seq_server_to_client >> 40):byte;
		nonce[7] = (ctx.seq_server_to_client >> 32):byte;
		nonce[8] = (ctx.seq_server_to_client >> 24):byte;
		nonce[9] = (ctx.seq_server_to_client >> 16):byte;
		nonce[10] = (ctx.seq_server_to_client >> 8):byte;
		nonce[11] = ctx.seq_server_to_client:byte;
	} else {
		minlen = 16;
		align = 8;
		maclen = 0;
		padlen = align - ((ctx.framelen + 5) % align);
	}

	if padlen < 4 {
		padlen = padlen + align;
	}

	if ctx.framelen + padlen + 5 < minlen {
		padlen = padlen + align;
	}

	if padlen + ctx.framelen + 5 + maclen > ctx.bufsz {
		die("write too large");
	}

	if padlen > 255 {
		die("padding too large");
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

	if ctx.en_server_to_client {
		seq = 0;
		chacha20_stream(ctx.buf, ctx.buf, 4, &seq, &((&ctx.ek_server_to_client):*byte)[32], nonce);
		seq = 0;
		chacha20_stream(mackey, mackey, 64, &seq, (&ctx.ek_server_to_client):*byte, nonce);
		seq = 64;
		chacha20_stream(&ctx.buf[4], &ctx.buf[4], len, &seq, (&ctx.ek_server_to_client):*byte, nonce);
		poly1305(&ctx.buf[len + 4], mackey, ctx.buf, len + 4);
	}

	ctx.seq_server_to_client = (ctx.seq_server_to_client + 1) & (-1 >> 32);

	if writeall(ctx.fd, ctx.frame, len + 4 + maclen) != 0 {
		die("write truncated");
	}
}

enum {
	SSH_MSG_SERVICE_REQUEST = 0x05,
	SSH_MSG_SERVICE_ACCEPT = 0x06,
	SSH_MSG_KEXINIT = 0x14,
	SSH_MSG_NEWKEYS = 0x15,
	SSH_MSG_ECDH_INIT = 0x1e,
	SSH_MSG_ECDH_REPLY = 0x1f,
	SSH_MSG_USERAUTH_REQUEST = 0x32,
	SSH_MSG_USERAUTH_FAILURE = 0x33,
	SSH_MSG_USERAUTH_SUCCESS = 0x34,
	SSH_MSG_USERAUTH_PK_OK = 0x3c,
}

doversion(ctx: *sshd_ctx) {
	var ident: *byte;
	var n: int;

	//SSH-2.0-VERSION<CR><LF>
	n = read_line(ctx.fd, ctx.buf, ctx.bufsz);
	if n < 8 || memcmp(ctx.buf, "SSH-2.0-", 8) != 0 {
		die("bad version");
	}

	ctx.cver = alloc(ctx.a, n + 1);
	memcpy(ctx.cver, ctx.buf, n);
	ctx.cver[n] = 0:byte;

	if writeall(ctx.fd, ctx.sver, strlen(ctx.sver)) != 0 {
		die("write truncated");
	}

	if writeall(ctx.fd, "\r\n", 2) != 0 {
		die("write truncated");
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
		die("u8 truncated");
	}
	*s = ctx.frame[ctx.index]:int;
	ctx.index = ctx.index + 1;
}

decode_u32(s: *int, ctx: *sshd_ctx) {
	if ctx.framelen < 4 || ctx.index > ctx.framelen - 4 {
		die("u32 truncated");
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
		die("str truncated");
	}
	s.s = &ctx.frame[ctx.index];
	ctx.index = ctx.index + s.len;
}

decode_cookie(s: *ssh_str, ctx: *sshd_ctx) {
	if ctx.framelen < 16 || ctx.index > ctx.framelen - 16 {
		die("cookie truncated");
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
		die("not a kexinit");
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
		die("trailing data");
	}
}

decode_newkeys(kex: *ssh_newkeys, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_NEWKEYS {
		die("not a newkeys");
	}
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

struct ssh_ecdh_init {
	qc: ssh_str;
}

decode_ecdh_init(dh: *ssh_ecdh_init, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_ECDH_INIT {
		die("not a ecdh_init");
	}
	decode_str(&dh.qc, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

struct ssh_service_request {
	name: ssh_str;
}

decode_service_request(sr: *ssh_service_request, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_SERVICE_REQUEST {
		die("not a ecdh_init");
	}
	decode_str(&sr.name, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

struct ssh_userauth_request {
	user: ssh_str;
	service: ssh_str;
	method: ssh_str;
	has_sig: int;
	alg: ssh_str;
	pub: ssh_str;
	sig: ssh_str;
}

decode_userauth_request(ar: *ssh_userauth_request, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_USERAUTH_REQUEST {
		die("not a userauth_request");
	}

	decode_str(&ar.user, ctx);
	decode_str(&ar.service, ctx);
	decode_str(&ar.method, ctx);

	if ssh_streq(&ar.method, "none") {
		ar.has_sig = 0;
		set_str(&ar.alg, "");
		set_str(&ar.pub, "");
		set_str(&ar.sig, "");
	} else if ssh_streq(&ar.method, "publickey") {
		decode_bool(&ar.has_sig, ctx);
		decode_str(&ar.alg, ctx);
		decode_str(&ar.pub, ctx);
		if ar.has_sig {
			decode_str(&ar.sig, ctx);
		} else {
			set_str(&ar.sig, "");
		}
	} else {
		die("unknown method");
	}

	if ctx.index != ctx.framelen {
		die("trailing data");
	}
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
		die("u8 truncated");
	}
	ctx.frame[ctx.index] = (*x):byte;
	ctx.index = ctx.index + 1;
}

encode_cookie(x: *ssh_str, ctx: *sshd_ctx) {
	if x.len != 16 || ctx.framelen < 16 || ctx.index > ctx.framelen - 16 {
		die("cookie truncated");
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
		die("u32 truncated");
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
		die("str truncated");
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

struct ssh_service_accept {
	name: ssh_str;
}

encode_service_accept(sa: *ssh_service_accept, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_SERVICE_ACCEPT;
	encode_u8(&tag, ctx);
	encode_str(&sa.name, ctx);
	finish_frame(ctx);
}

struct ssh_userauth_failure {
	methods: ssh_str;
	partial_success: int;
}

encode_userauth_failure(uf: *ssh_userauth_failure, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_USERAUTH_FAILURE;
	encode_u8(&tag, ctx);
	encode_str(&uf.methods, ctx);
	encode_bool(&uf.partial_success, ctx);
	finish_frame(ctx);
}

struct ssh_userauth_success {
}

encode_userauth_success(us: *ssh_userauth_success, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_USERAUTH_SUCCESS;
	encode_u8(&tag, ctx);
	finish_frame(ctx);
}

struct ssh_userauth_pk_ok {
	alg: ssh_str;
	pub: ssh_str;
}

encode_userauth_pk_ok(ok: *ssh_userauth_pk_ok, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_USERAUTH_PK_OK;
	encode_u8(&tag, ctx);
	encode_str(&ok.alg, ctx);
	encode_str(&ok.pub, ctx);
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
	memcpy(ctx.ckex, ctx.frame, ctx.framelen);
	ctx.ckexlen = ctx.framelen;

	if ckex.first_kex_packet_follows {
		die("first_kex_packet_follows not implemented");
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
		die("bad ecdh_init");
	}

	read_rand((&rs):*byte, 32);

	x25519_base((&qs):*byte);

	if !x25519((&qs):*byte, (&qs):*byte, (&rs):*byte) {
		die("bad x25519 server");
	}

	if !x25519((&k):*byte, cdh.qc.s, (&rs):*byte) {
		die("bad x25519 client");
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

	if !ctx.has_session_id {
		memcpy((&ctx.session_id):*byte, (&eh):*byte, 32);
		ctx.has_session_id = 1;
	}

	memcpy((&ctx.eh):*byte, (&eh):*byte, 32);
	memcpy((&ctx.k):*byte, (&k):*byte, 32);

	kex_key((&ctx.iv_client_to_server):*byte, "A", ctx);
	kex_key((&ctx.iv_server_to_client):*byte, "B", ctx);
	kex_key((&ctx.ek_client_to_server):*byte, "C", ctx);
	kex_key((&ctx.ek_server_to_client):*byte, "D", ctx);
	kex_key((&ctx.ik_client_to_server):*byte, "E", ctx);
	kex_key((&ctx.ik_server_to_client):*byte, "F", ctx);

	// -> SSH_MSG_NEWKEYS
	encode_newkeys(&newkeys, ctx);
	write_frame(ctx);
	ctx.en_server_to_client = 1;

	// <- SSH_MSG_NEWKEYS
	read_frame(ctx);
	decode_newkeys(&newkeys, ctx);
	ctx.en_client_to_server = 1;
}

kex_key(key: *byte, a: *byte, ctx: *sshd_ctx) {
	var h: sha256_ctx;

	sha256_init(&h);
	ssh_mpint_sha256_update(&h, (&ctx.k):*byte, 32);
	sha256_update(&h, (&ctx.eh):*byte, 32);
	sha256_update(&h, a, 1);
	sha256_update(&h, (&ctx.session_id):*byte, 32);
	sha256_final(key, &h);

	sha256_init(&h);
	ssh_mpint_sha256_update(&h, (&ctx.k):*byte, 32);
	sha256_update(&h, (&ctx.eh):*byte, 32);
	sha256_update(&h, key, 32);
	sha256_final(&key[32], &h);
}

ssh_streq(a: *ssh_str, b: *byte): int {
	var len: int;
	len = strlen(b);
	return a.len == len && memcmp(a.s, b, len) == 0;
}

doauth(ctx: *sshd_ctx) {
	var csr: ssh_service_request;
	var cua: ssh_userauth_request;
	var ssa: ssh_service_accept;
	var suf: ssh_userauth_failure;
	var sus: ssh_userauth_success;
	var sok: ssh_userauth_pk_ok;
	var sig: _ed25519_sig;
	var b: int;
	var s: ssh_str;

	// <- SSH_MSG_SERVICE_REQUEST
	read_frame(ctx);
	decode_service_request(&csr, ctx);
	if !ssh_streq(&csr.name, "ssh-userauth") {
		die("not ssh-userauth");
	}

	// -> SSH_MSG_SERVICE_ACCEPT
	set_str(&ssa.name, "ssh-userauth");
	encode_service_accept(&ssa, ctx);
	write_frame(ctx);

	loop {

		// <- SSH_MSG_USERAUTH_REQUEST
		read_frame(ctx);
		decode_userauth_request(&cua, ctx);
		if !ssh_streq(&cua.service, "ssh-connection") || !ssh_streq(&cua.user, "erai") {
			die("bad auth");
		}

		if !ssh_streq(&cua.method, "publickey") || !ssh_streq(&cua.alg, "ssh-ed25519") {
			// -> SSH_MSG_USERAUTH_FAILURE("publickey")
			set_str(&suf.methods, "publickey");
			suf.partial_success = 0;
			encode_userauth_failure(&suf, ctx);
			write_frame(ctx);
			continue;
		}

		if cua.pub.len != ctx.userkeylen || memcmp(cua.pub.s, ctx.userkey, ctx.userkeylen) {
			// -> SSH_MSG_USERAUTH_FAILURE("publickey")
			set_str(&suf.methods, "publickey");
			suf.partial_success = 0;
			encode_userauth_failure(&suf, ctx);
			write_frame(ctx);
			continue;
		}

		if !cua.has_sig {
			// -> SSH_MSG_USERAUTH_PK_OK
			set_str(&sok.alg, "ssh-ed25519");
			sok.pub.s = ctx.userkey;
			sok.pub.len = ctx.userkeylen;
			encode_userauth_pk_ok(&sok, ctx);
			write_frame(ctx);
			continue;
		}

		break;
	}

	if cua.sig.len != 83 {
		die("sig wrong length");
	}

	memcpy((&sig):*byte, &cua.sig.s[19], 64);

	clear_frame(ctx);
	s.s = (&ctx.session_id):*byte;
	s.len = 32;
	encode_str(&s, ctx);
	b = SSH_MSG_USERAUTH_REQUEST;
	encode_u8(&b, ctx);
	set_str(&s, "erai");
	encode_str(&s, ctx);
	set_str(&s, "ssh-connection");
	encode_str(&s, ctx);
	set_str(&s, "publickey");
	encode_str(&s, ctx);
	b = 1;
	encode_bool(&b, ctx);
	set_str(&s, "ssh-ed25519");
	encode_str(&s, ctx);
	s.s = ctx.userkey;
	s.len = ctx.userkeylen;
	encode_str(&s, ctx);
	finish_frame(ctx);

	if !ed25519_verify((&sig):*byte, (&ctx.userpub):*byte, ctx.frame, ctx.framelen) {
		die("bad signature");
	}

	encode_userauth_success(&sus, ctx);
	write_frame(ctx);
}

cmain(ctx: *sshd_ctx) {
	doversion(ctx);
	dokex(ctx);
	doauth(ctx);
	die("shell not implemented");
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
	userpub: _ed25519_pub;
	hostkey: *byte;
	hostkeylen: int;
	userkey: *byte;
	userkeylen: int;
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
	macsize: int;
	has_session_id: int;
	session_id: _sha256_digest;
	eh: _sha256_digest;
	k: _ed25519_point;
	iv_client_to_server: _sha512_digest;
	iv_server_to_client: _sha512_digest;
	ek_client_to_server: _sha512_digest;
	ek_server_to_client: _sha512_digest;
	ik_client_to_server: _sha512_digest;
	ik_server_to_client: _sha512_digest;
	en_client_to_server: int;
	en_server_to_client: int;
	seq_client_to_server: int;
	seq_server_to_client: int;
}

format_key(d: **byte, dlen: *int, k: *byte, ctx: *sshd_ctx) {
	var s: ssh_str;
	clear_frame(ctx);
	set_str(&s, "ssh-ed25519");
	encode_str(&s, ctx);
	s.s = k;
	s.len = 32;
	encode_str(&s, ctx);
	finish_frame(ctx);
	*d = alloc(ctx.a, ctx.framelen);
	memcpy(*d, ctx.frame, ctx.framelen);
	*dlen = ctx.framelen;
	clear_frame(ctx);
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

	if unhex((&ctx.userpub):*byte, "5afb3032d86d60d700e1782bfe8083ff95504cecbda531bef5ba8cdd20f34c82") != 32 {
		die("invalid userpub");
	}

	ed25519_pub((&ctx.pub):*byte, (&ctx.priv):*byte);

	ctx.a = &a;
	ctx.bufsz = 64 * 1024;
	ctx.buf = alloc(ctx.a, ctx.bufsz);
	ctx.skex = alloc(ctx.a, 4096);
	ctx.ckex = alloc(ctx.a, 4096);
	ctx.sver = "SSH-2.0-omiltem";

	format_key(&ctx.hostkey, &ctx.hostkeylen, (&ctx.pub):*byte, &ctx);
	format_key(&ctx.userkey, &ctx.userkeylen, (&ctx.userpub):*byte, &ctx);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if fd < 0 {
		die("failed to open socket");
	}

	port = 4321;
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
			die("cmain returned");
		}

		close(ctx.fd);
	}
}
