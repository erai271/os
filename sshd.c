read_line(fd: int, buf: *byte, max: int): int {
	var len: int;
	var ret: int;
	var c: byte;
	len = 0;
	loop {
		if len == max {
			return 0;
		}

		ret = read(fd, &c, 1);

		if ret == -EINTR {
			continue;
		}

		if ret == 0 {
			return 0;
		}

		if c == '\n':byte {
			buf[len] = 0:byte;
			return len;
		}

		if c == '\r':byte {
			continue;
		}

		buf[len] = c;
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
		if ret == -EINTR {
			continue;
		}
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
		if ret == -EINTR {
			continue;
		}

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
	SSH_MSG_DISCONNECT = 0x01,
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
	SSH_MSG_CHANNEL_OPEN = 0x5a,
	SSH_MSG_CHANNEL_OPEN_CONFIRMATION = 0x5b,
	SSH_MSG_CHANNEL_OPEN_FAILURE = 0x5c,
	SSH_MSG_CHANNEL_WINDOW_ADJUST = 0x5d,
	SSH_MSG_CHANNEL_DATA = 0x5e,
	SSH_MSG_CHANNEL_EXTENDED_DATA = 0x5f,
	SSH_MSG_CHANNEL_EOF = 0x60,
	SSH_MSG_CHANNEL_CLOSE = 0x61,
	SSH_MSG_CHANNEL_REQUEST = 0x62,
	SSH_MSG_CHANNEL_SUCCESS = 0x63,
	SSH_MSG_CHANNEL_FAILURE = 0x64,
}

enum {
	SSH_CR_UNKNOWN,
	SSH_CR_PTY,
	SSH_CR_SHELL,
	SSH_CR_EXEC,
	SSH_CR_WINCH,
	SSH_CR_SIGNAL,
	SSH_CR_EXIT_STATUS,
	SSH_CR_EXIT_SIGNAL,
}

struct ssh_cr_pty {
	term: ssh_str;
	cx: int;
	cy: int;
	px: int;
	py: int;
	modes: ssh_str;
}

struct ssh_cr_shell {
}

struct ssh_cr_exec {
	command: ssh_str;
}

struct ssh_cr_winch {
	cx: int;
	cy: int;
	px: int;
	py: int;
}

struct ssh_cr_signal {
	name: ssh_str;
}

struct ssh_cr_exit {
	status: int;
}

struct ssh_cr_exit_signal {
	name: ssh_str;
	core_dumpped: int;
	message: ssh_str;
	lang: ssh_str;
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

struct ssh_disconnect {
	reason: int;
	description: ssh_str;
	lang: ssh_str;
}

decode_disconnect(p: *ssh_disconnect, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_DISCONNECT {
		die("not a ssh_disconnect");
	}
	decode_u32(&p.reason, ctx);
	decode_str(&p.description, ctx);
	decode_str(&p.lang, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_disconnect(p: *ssh_disconnect, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_DISCONNECT;
	encode_u32(&p.reason, ctx);
	encode_str(&p.description, ctx);
	encode_str(&p.lang, ctx);
	finish_frame(ctx);
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

struct ssh_channel_open {
	channel_type: ssh_str;
	channel: int;
	initial_window_size: int;
	maximum_packet_size: int;
}

decode_channel_open(p: *ssh_channel_open, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_OPEN {
		die("not a channel_open");
	}

	decode_str(&p.channel_type, ctx);
	decode_u32(&p.channel, ctx);
	decode_u32(&p.initial_window_size, ctx);
	decode_u32(&p.maximum_packet_size, ctx);

	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

struct ssh_channel_open_confirmation {
	channel: int;
	sender_channel: int;
	initial_window_size: int;
	maximum_packet_size: int;
}

encode_channel_open_confirmation(p: *ssh_channel_open_confirmation, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_OPEN_CONFIRMATION;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	encode_u32(&p.sender_channel, ctx);
	encode_u32(&p.initial_window_size, ctx);
	encode_u32(&p.maximum_packet_size, ctx);
	finish_frame(ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

struct ssh_channel_open_failure {
	channel: int;
	reason: int;
	description: ssh_str;
	lang: ssh_str;
}

encode_channel_open_failure(p: *ssh_channel_open_failure, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_OPEN_FAILURE;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	encode_u32(&p.reason, ctx);
	encode_str(&p.description, ctx);
	encode_str(&p.lang, ctx);
	finish_frame(ctx);
}

struct ssh_channel_window_adjust {
	channel: int;
	window: int;
}

decode_channel_window_adjust(p: *ssh_channel_window_adjust, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_WINDOW_ADJUST {
		die("not a channel_window_adjust");
	}
	decode_u32(&p.channel, ctx);
	decode_u32(&p.window, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_channel_window_adjust(p: *ssh_channel_window_adjust, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_WINDOW_ADJUST;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	encode_u32(&p.window, ctx);
	finish_frame(ctx);
}

struct ssh_channel_data {
	channel: int;
	data: ssh_str;
}

decode_channel_data(p: *ssh_channel_data, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_DATA {
		die("not a channel_data");
	}
	decode_u32(&p.channel, ctx);
	decode_str(&p.data, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_channel_data(p: *ssh_channel_data, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_DATA;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	encode_str(&p.data, ctx);
	finish_frame(ctx);
}

struct ssh_channel_extended_data {
	channel: int;
	code: int;
	data: ssh_str;
}

encode_channel_extended_data(p: *ssh_channel_extended_data, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_EXTENDED_DATA;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	encode_u32(&p.code, ctx);
	encode_str(&p.data, ctx);
	finish_frame(ctx);
}

struct ssh_channel_eof {
	channel: int;
}

decode_channel_eof(p: *ssh_channel_eof, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_EOF {
		die("not a channel_eof");
	}
	decode_u32(&p.channel, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_channel_eof(p: *ssh_channel_eof, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_EOF;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	finish_frame(ctx);
}

struct ssh_channel_close {
	channel: int;
}

decode_channel_close(p: *ssh_channel_close, ctx: *sshd_ctx) {
	var tag: int;
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_CLOSE {
		die("not a channel_close");
	}
	decode_u32(&p.channel, ctx);
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_channel_close(p: *ssh_channel_close, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_CLOSE;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	finish_frame(ctx);
}

struct ssh_channel_request {
	channel: int;
	request: ssh_str;
	want_reply: int;
	kind: int;
	unknown: ssh_str;
	pty: ssh_cr_pty;
	shell: ssh_cr_shell;
	exec: ssh_cr_exec;
	winch: ssh_cr_winch;
	signal: ssh_cr_signal;
	exit: ssh_cr_exit;
	exit_signal: ssh_cr_exit_signal;
}

decode_channel_request(p: *ssh_channel_request, ctx: *sshd_ctx) {
	var tag: int;
	bzero(p:*byte, sizeof(*p));
	decode_u8(&tag, ctx);
	if tag != SSH_MSG_CHANNEL_REQUEST {
		die("not a channel_request");
	}
	decode_u32(&p.channel, ctx);
	decode_str(&p.request, ctx);
	decode_bool(&p.want_reply, ctx);
	if ssh_streq(&p.request, "pty-req") {
		p.kind = SSH_CR_PTY;
		decode_str(&p.pty.term, ctx);
		decode_u32(&p.pty.cx, ctx);
		decode_u32(&p.pty.cy, ctx);
		decode_u32(&p.pty.px, ctx);
		decode_u32(&p.pty.py, ctx);
		decode_str(&p.pty.modes, ctx);
	} else if ssh_streq(&p.request, "shell") {
		p.kind = SSH_CR_SHELL;
	} else if ssh_streq(&p.request, "exec") {
		p.kind = SSH_CR_EXEC;
		decode_str(&p.exec.command, ctx);
	} else if ssh_streq(&p.request, "window-change") {
		p.kind = SSH_CR_WINCH;
		decode_u32(&p.winch.cx, ctx);
		decode_u32(&p.winch.cy, ctx);
		decode_u32(&p.winch.px, ctx);
		decode_u32(&p.winch.py, ctx);
	} else if ssh_streq(&p.request, "signal") {
		p.kind = SSH_CR_SIGNAL;
		decode_str(&p.signal.name, ctx);
	} else {
		p.kind = SSH_CR_UNKNOWN;
		p.unknown.s = &ctx.frame[ctx.index];
		p.unknown.len = ctx.framelen - ctx.index;
		ctx.index = ctx.framelen;
	}
	if ctx.index != ctx.framelen {
		die("trailing data");
	}
}

encode_channel_request(p: *ssh_channel_request, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_REQUEST;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	if p.kind == SSH_CR_EXIT_STATUS {
		set_str(&p.request, "exit-status");
		encode_str(&p.request, ctx);
		p.want_reply = 0;
		encode_bool(&p.want_reply, ctx);
		encode_u32(&p.exit.status, ctx);
	} else if p.kind == SSH_CR_EXIT_SIGNAL {
		set_str(&p.request, "exit-signal");
		encode_str(&p.request, ctx);
		p.want_reply = 0;
		encode_bool(&p.want_reply, ctx);
		encode_str(&p.exit_signal.name, ctx);
		encode_bool(&p.exit_signal.core_dumpped, ctx);
		encode_str(&p.exit_signal.message, ctx);
		encode_str(&p.exit_signal.lang, ctx);
	} else {
		encode_str(&p.request, ctx);
		encode_bool(&p.want_reply, ctx);
	}
	finish_frame(ctx);
}

struct ssh_channel_success {
	channel: int;
}

encode_channel_success(p: *ssh_channel_success, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_SUCCESS;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	finish_frame(ctx);
}

struct ssh_channel_failure {
	channel: int;
}

encode_channel_failure(p: *ssh_channel_failure, ctx: *sshd_ctx) {
	var tag: int;
	clear_frame(ctx);
	tag = SSH_MSG_CHANNEL_FAILURE;
	encode_u8(&tag, ctx);
	encode_u32(&p.channel, ctx);
	finish_frame(ctx);
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
		if !ssh_streq(&cua.service, "ssh-connection") || !ssh_streq(&cua.user, ctx.username) {
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
	set_str(&s, ctx.username);
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

dosession(ctx: *sshd_ctx) {
	var cco: ssh_channel_open;
	var scc: ssh_channel_open_confirmation;
	var ccr: ssh_channel_request;
	var scs: ssh_channel_success;
	var scf: ssh_channel_failure;

	decode_channel_open(&cco, ctx);
	if  !ssh_streq(&cco.channel_type, "session") || cco.channel != 0 {
		die("not session");
	}

	ctx.stdout_window = cco.initial_window_size;
	if cco.maximum_packet_size < 1024 {
		die("max packet size too small");
	}

	scc.channel = 0;
	scc.sender_channel = 0;
	scc.initial_window_size = ctx.stdin_size;
	scc.maximum_packet_size = ctx.bufsz - 64;

	encode_channel_open_confirmation(&scc, ctx);
	write_frame(ctx);
}

dodisconnect(ctx: *sshd_ctx) {
	var d: ssh_disconnect;
	decode_disconnect(&d, ctx);
	exit(0);
}

dowindow(ctx: *sshd_ctx) {
	var cwa: ssh_channel_window_adjust;
	decode_channel_window_adjust(&cwa, ctx);
	ctx.stdout_window = ctx.stdout_window + cwa.window;
}

dodata(ctx: *sshd_ctx) {
	var cd: ssh_channel_data;
	decode_channel_data(&cd, ctx);
	if ctx.stdin_eof {
		return;
	}
	if cd.data.len > ctx.stdin_size - ctx.stdin_fill {
		die("stdin overflow");
	}
	memcpy(&ctx.stdin_buf[ctx.stdin_fill], cd.data.s, cd.data.len);
	ctx.stdin_fill = ctx.stdin_fill + cd.data.len;
}

doeof(ctx: *sshd_ctx) {
	var ce: ssh_channel_eof;
	decode_channel_eof(&ce, ctx);
	ctx.stdin_eof = 1;
}

doclose(ctx: *sshd_ctx) {
	var cc: ssh_channel_close;
	decode_channel_close(&cc, ctx);
	exit(0);
}

struct _argv4 {
	arg0: *byte;
	arg1: *byte;
	arg2: *byte;
	arg3: *byte;
}

ssh_spawn(ctx: *sshd_ctx, argv: **byte) {
	var pid: int;
	var stdin_read: int;
	var stdin_write: int;
	var stdout_read: int;
	var stdout_write: int;
	var stderr_read: int;
	var stderr_write: int;

	if ctx.has_child {
		die("already spawned");
	}

	if pipe(&stdin_read, &stdin_write) != 0 {
		die("stdin pipe failed");
	}

	if pipe(&stdout_read, &stdout_write) != 0 {
		die("stdout pipe failed");
	}

	if pipe(&stderr_read, &stderr_write) != 0 {
		die("stderr pipe failed");
	}

	pid = fork();
	if pid < 0 {
		die("fork failed");
	}

	if pid == 0 {
		close(ctx.fd);

		close(stdin_write);
		close(stdout_read);
		close(stderr_read);

		dup2(stdin_read, 0);
		dup2(stdout_write, 1);
		dup2(stderr_write, 2);

		close(stdin_read);
		close(stdout_write);
		close(stderr_write);

		exec(argv[0], argv, 0:**byte);

		exit(255);
	}

	close(stdin_read);
	close(stdout_write);
	close(stderr_write);

	ctx.has_child = 1;
	ctx.child_pid = pid;
	ctx.child_stdin = stdin_write;
	ctx.child_stdout = stdout_read;
	ctx.child_stderr = stderr_read;
}

dopty(cr: *ssh_cr_pty, ctx: *sshd_ctx): int {
	// allocate pty
	return 0;
}

doshell(cr: *ssh_cr_shell, ctx: *sshd_ctx): int {
	var argv: _argv4;
	argv.arg0 = "/bin/sh";
	argv.arg1 = 0:*byte;
	ssh_spawn(ctx, &argv.arg0);
	return 1;
}

doexec(cr: *ssh_cr_exec, ctx: *sshd_ctx): int {
	var argv: _argv4;
	var cmd: *byte;
	cmd = alloc(ctx.a, cr.command.len + 1);
	memcpy(cmd, cr.command.s, cr.command.len);
	cmd[cr.command.len] = 0:byte;
	argv.arg0 = "/bin/sh";
	argv.arg1 = "-c";
	argv.arg2 = cmd;
	argv.arg3 = 0:*byte;
	ssh_spawn(ctx, &argv.arg0);
	free(ctx.a, cmd);
	return 1;
}

dowinch(cr: *ssh_cr_winch, ctx: *sshd_ctx): int {
	// window change
	return 0;
}

dosignal(cr: *ssh_cr_signal, ctx: *sshd_ctx): int {
	// signal
	return 0;
}

dorequest(ctx: *sshd_ctx) {
	var cr: ssh_channel_request;
	var ss: ssh_channel_success;
	var sf: ssh_channel_failure;
	var ok: int;
	decode_channel_request(&cr, ctx);

	if cr.kind == SSH_CR_PTY {
		ok = dopty(&cr.pty, ctx);
	} else if cr.kind == SSH_CR_SHELL {
		ok = doshell(&cr.shell, ctx);
	} else if cr.kind == SSH_CR_EXEC {
		ok = doexec(&cr.exec, ctx);
	} else if cr.kind == SSH_CR_WINCH {
		ok = dowinch(&cr.winch, ctx);
	} else if cr.kind == SSH_CR_SIGNAL {
		ok = dosignal(&cr.signal, ctx);
	} else {
		die("unknown request");
		if cr.want_reply {
			sf.channel = 0;
			encode_channel_failure(&sf, ctx);
			write_frame(ctx);
			return;
		}
	}

	if cr.want_reply {
		if ok {
			ss.channel = 0;
			encode_channel_success(&ss, ctx);
			write_frame(ctx);
		} else {
			sf.channel = 0;
			encode_channel_failure(&sf, ctx);
			write_frame(ctx);
		}
	}
}

struct pfd4 {
	p0: int;
	p1: int;
	p2: int;
	p3: int;
}

reset_pfd(pfd: *int, ctx: *sshd_ctx): int {
	var n: int;
	var events: int;

	n = 0;

	if (
		// There is a window update to send
		(ctx.stdin_window > 1024)
		// There is stdout to send
		|| (ctx.stdout_fill > 0 && ctx.stdout_window > 0)
		// There is stderr to send
		|| (ctx.stderr_fill > 0 && ctx.stdout_window > 0)
		// Child exited and there's no more stdout/stderr
		|| (ctx.has_child
			&& ctx.stdout_fill == 0
			&& ctx.stderr_fill == 0
			&& ctx.child_stdout == -1
			&& ctx.child_stderr == -1
			&& ctx.child_pid == -1)
	) {
		events = POLLIN | POLLOUT;
	} else {
		events = POLLIN;
	}
	pfd[n] = ctx.fd | (events << 32);
	n = n + 1;

	if ctx.child_stdin >= 0 && (ctx.stdin_fill > 0 || ctx.stdin_eof) {
		events = POLLOUT;
		pfd[n] = ctx.child_stdin | (events << 32);
		n = n + 1;
	}

	if ctx.child_stdout >= 0 && ctx.stdout_fill < ctx.stderr_size {
		events = POLLIN;
		pfd[n] = ctx.child_stdout | (events << 32);
		n = n + 1;
	}

	if ctx.child_stderr >= 0 && ctx.stderr_fill < ctx.stderr_size {
		events = POLLIN;
		pfd[n] = ctx.child_stderr | (events << 32);
		n = n + 1;
	}

	return n;
}

poll_client(revents: int, ctx: *sshd_ctx) {
	var tag: int;
	var len: int;
	var swa: ssh_channel_window_adjust;
	var sd: ssh_channel_data;
	var sed: ssh_channel_extended_data;
	var sr: ssh_channel_request;
	var sc: ssh_channel_close;
	var se: ssh_channel_eof;
	if revents & POLLOUT {
		if ctx.stdin_window > 1024 {
			swa.channel = 0;
			swa.window = ctx.stdin_window;
			encode_channel_window_adjust(&swa, ctx);
			write_frame(ctx);
			ctx.stdin_window = 0;
		} else if ctx.stdout_fill > 0 && ctx.stdout_window > 0 {
			len = ctx.stdout_fill;
			if len > 1024 {
				len = 1024;
			}

			if len > ctx.stdout_window {
				len = ctx.stdout_window;
			}

			sd.channel = 0;
			sd.data.s = ctx.stdout_buf;
			sd.data.len = len;

			memcpy(ctx.stdout_buf, &ctx.stdout_buf[len], ctx.stdout_fill - len);
			ctx.stdout_window = ctx.stdout_window - len;
			ctx.stdout_fill = ctx.stdout_fill - len;

			encode_channel_data(&sd, ctx);
			write_frame(ctx);
		} else if ctx.stderr_fill > 0 && ctx.stdout_window > 0 {
			len = ctx.stderr_fill;
			if len > 1024 {
				len = 1024;
			}

			if len > ctx.stdout_window {
				len = ctx.stdout_window;
			}

			sed.channel = 0;
			sed.code = 1;
			sed.data.s = ctx.stderr_buf;
			sed.data.len = len;

			memcpy(ctx.stderr_buf, &ctx.stderr_buf[len], ctx.stderr_fill - len);
			ctx.stdout_window = ctx.stdout_window - len;
			ctx.stderr_fill = ctx.stderr_fill - len;

			encode_channel_extended_data(&sed, ctx);
			write_frame(ctx);
		} else {
			se.channel = 0;
			encode_channel_eof(&se, ctx);
			write_frame(ctx);

			sr.channel = 0;
			sr.want_reply = 0;
			sr.kind = SSH_CR_EXIT_STATUS;
			sr.exit.status = ctx.exit_status;
			encode_channel_request(&sr, ctx);
			write_frame(ctx);

			sc.channel = 0;
			encode_channel_close(&sc, ctx);
			write_frame(ctx);

			loop {
				read_frame(ctx);
				if ctx.frame[0]:int == SSH_MSG_DISCONNECT {
					break;
				}
			}

			exit(0);
		}
	} else {
		read_frame(ctx);

		tag = ctx.frame[0]:int;
		if tag == SSH_MSG_DISCONNECT {
			dodisconnect(ctx);
		} else if tag == SSH_MSG_KEXINIT {
			dokex(ctx);
		} else if tag == SSH_MSG_CHANNEL_WINDOW_ADJUST {
			dowindow(ctx);
		} else if tag == SSH_MSG_CHANNEL_DATA {
			dodata(ctx);
		} else if tag == SSH_MSG_CHANNEL_EOF {
			doeof(ctx);
		} else if tag == SSH_MSG_CHANNEL_CLOSE {
			doclose(ctx);
		} else if tag == SSH_MSG_CHANNEL_REQUEST {
			dorequest(ctx);
		} else {
			die("invalid packet");
		}
	}
}

poll_stdin(revents: int, ctx: *sshd_ctx) {
	var ret: int;
	if ctx.stdin_fill == 0 && ctx.stdin_eof {
		close(ctx.child_stdin);
		ctx.child_stdin = -1;
		return;
	}
	ret = write(ctx.child_stdin, ctx.stdin_buf, ctx.stdin_fill);
	if ret == -EINTR {
		return;
	}
	if ret == -EPIPE {
		close(ctx.child_stdin);
		ctx.child_stdin = -1;
		return;
	}
	memcpy(ctx.stdin_buf, &ctx.stdin_buf[ret], ctx.stdin_fill - ret);
	ctx.stdin_fill = ctx.stdin_fill - ret;
	ctx.stdin_window = ctx.stdin_window + ret;
}

poll_stdout(revents: int, ctx: *sshd_ctx) {
	var ret: int;
	ret = read(ctx.child_stdout, &ctx.stdout_buf[ctx.stdout_fill], ctx.stdout_size - ctx.stdout_fill);
	if ret == -EINTR {
		return;
	}
	if ret == 0 {
		close(ctx.child_stdout);
		ctx.child_stdout = -1;
		return;
	}
	ctx.stdout_fill = ctx.stdout_fill + ret;
}

poll_stderr(revents: int, ctx: *sshd_ctx) {
	var ret: int;
	ret = read(ctx.child_stderr, &ctx.stderr_buf[ctx.stderr_fill], ctx.stderr_size - ctx.stderr_fill);
	if ret == -EINTR {
		return;
	}
	if ret == 0 {
		close(ctx.child_stderr);
		ctx.child_stderr = -1;
		return;
	}
	ctx.stderr_fill = ctx.stderr_fill + ret;
}

poll_exit(ctx: *sshd_ctx) {
	var ret: int;
	var status: int;
	loop {
		ret = wait(-1, &status, WNOHANG);
		if ret <= 0 {
			break;
		}
		if ret == ctx.child_pid {
			ctx.child_pid = -1;
			ctx.exit_status = status;
		}
	}
}

client_loop(ctx: *sshd_ctx) {
	var _p: pfd4;
	var p: *int;
	var n: int;
	var i: int;
	var fd: int;
	var revents: int;

	p = &_p.p0;

	loop {
		poll_exit(ctx);

		n = reset_pfd(p, ctx);
		if poll(p, n, -1) == -EINTR {
			continue;
		}

		i = 0;
		loop {
			if i == n {
				break;
			}

			fd = p[i] & (-1 >> 32);
			revents = p[i] >> 48;
			i = i + 1;

			if !revents {
				continue;
			}

			if fd == ctx.fd {
				poll_client(revents, ctx);
			}

			if fd == ctx.child_stdin {
				poll_stdin(revents, ctx);
			}

			if fd == ctx.child_stdout {
				poll_stdout(revents, ctx);
			}

			if fd == ctx.child_stderr {
				poll_stderr(revents, ctx);
			}
		}
	}
}

cmain(ctx: *sshd_ctx) {
	doversion(ctx);

	read_frame(ctx);
	dokex(ctx);

	read_frame(ctx);
	doauth(ctx);

	read_frame(ctx);
	dosession(ctx);

	client_loop(ctx);
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
	username: *byte;
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
	has_child: int;
	child_pid: int;
	child_stdin: int;
	child_stdout: int;
	child_stderr: int;
	stdin_window: int;
	stdout_window: int;
	stdin_buf: *byte;
	stdin_size: int;
	stdin_fill: int;
	stdin_eof: int;
	stdout_buf: *byte;
	stdout_fill: int;
	stdout_size: int;
	stderr_buf: *byte;
	stderr_fill: int;
	stderr_size: int;
	exit_status: int;
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

dosigchld() {
}

_restorer();

signal(sig: int, handler: func()) {
	var act: sigaction;
	act.handler = (&dosigchld):int;
	act.flags = 1 << 26;
	act.restorer = _restorer:int;
	act.mask = 0;
	sigaction(sig, &act, 0:*sigaction);
}

main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var cfd: int;
	var port: int;
	var sa: sockaddr;
	var ctx: sshd_ctx;
	var a: alloc;
	setup_alloc(&a);

	signal(SIGCHLD, dosigchld);
	signal(SIGPIPE, SIG_IGN:func());

	bzero((&ctx):*byte, sizeof(ctx));

	if unhex((&ctx.userpub):*byte, "5afb3032d86d60d700e1782bfe8083ff95504cecbda531bef5ba8cdd20f34c82") != 32 {
		die("invalid userpub");
	}

	ed25519_pub((&ctx.pub):*byte, (&ctx.priv):*byte);

	ctx.child_pid = -1;
	ctx.child_stdin = -1;
	ctx.child_stdout = -1;
	ctx.child_stderr = -1;

	ctx.a = &a;
	ctx.bufsz = 64 * 1024;
	ctx.buf = alloc(ctx.a, ctx.bufsz);
	ctx.skex = alloc(ctx.a, 4096);
	ctx.ckex = alloc(ctx.a, 4096);
	ctx.sver = "SSH-2.0-omiltem";

	ctx.stdin_size = 64 * 1024;
	ctx.stdin_buf = alloc(ctx.a, ctx.stdin_size);
	ctx.stdout_size = 64 * 1024;
	ctx.stdout_buf = alloc(ctx.a, ctx.stdout_size);
	ctx.stderr_size = 64 * 1024;
	ctx.stderr_buf = alloc(ctx.a, ctx.stderr_size);

	ctx.username = "erai";
	format_key(&ctx.hostkey, &ctx.hostkeylen, (&ctx.pub):*byte, &ctx);
	format_key(&ctx.userkey, &ctx.userkeylen, (&ctx.userpub):*byte, &ctx);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if fd < 0 {
		die("failed to open socket");
	}

	port = 22;
	sa.fpa = AF_INET | ((port & 0xff) << 24) | (((port >> 8) & 0xff) << 16);
	sa.pad = 0;
	if bind(fd, (&sa):*byte, sizeof(sa)) != 0 {
		die("failed to bind");
	}

	if listen(fd, 128) != 0 {
		die("failed to listen");
	}

	loop {
		loop {
			if wait(-1, 0:*int, WNOHANG) <= 0 {
				break;
			}
		}

		ctx.fd = accept(fd, 0:*byte, 0:*int);
		if ctx.fd == -EINTR {
			continue;
		}

		if ctx.fd < 0 {
			exit(1);
		}

		if fork() == 0 {
			close(fd);
			cmain(&ctx);
			die("cmain returned");
		}

		close(ctx.fd);
	}
}
