// https://www.rfc-editor.org/rfc/rfc4634

struct sha512_ctx {
	a: int;
	b: int;
	c: int;
	d: int;
	e: int;
	f: int;
	g: int;
	h: int;
	len: int;
	block: _sha512_block;
}

struct _sha512_block {
	c0: int;
	c1: int;
	c2: int;
	c3: int;
	c4: int;
	c5: int;
	c6: int;
	c7: int;
	c8: int;
	c9: int;
	c10: int;
	c11: int;
	c12: int;
	c13: int;
	c14: int;
	c15: int;
}

struct _sha512_digest {
	c: sha512_ctx;
}

sha512_init(ctx: *sha512_ctx) {
	ctx.a = (0x6a09e667 << 32) | 0xf3bcc908;
	ctx.b = (0xbb67ae85 << 32) | 0x84caa73b;
	ctx.c = (0x3c6ef372 << 32) | 0xfe94f82b;
	ctx.d = (0xa54ff53a << 32) | 0x5f1d36f1;
	ctx.e = (0x510e527f << 32) | 0xade682d1;
	ctx.f = (0x9b05688c << 32) | 0x2b3e6c1f;
	ctx.g = (0x1f83d9ab << 32) | 0xfb41bd6b;
	ctx.h = (0x5be0cd19 << 32) | 0x137e2179;
	ctx.len = 0;
}

ror64(x: int, n: int): int {
	return (x >> n) | (x << (64 - n));
}

sha512_round(r: *sha512_ctx, w: *int, k: int) {
	var s0: int;
	var s1: int;
	var t1: int;
	var t2: int;
	var ch: int;
	var maj: int;
	var w16: int;
	var i: int;

	s1 = ror64(r.e, 14); s1 = s1 ^ ror64(r.e, 18); s1 = s1 ^ ror64(r.e, 41);
	ch = (r.e & r.f) ^ ((~r.e) & r.g);
	t1 = r.h + s1 + ch + k + w[0];
	s0 = ror64(r.a, 28); s0 = s0 ^ ror64(r.a, 34); s0 = s0 ^ ror64(r.a, 39);
	maj = (r.a & r.b) ^ (r.a & r.c) ^ (r.b & r.c);

	t2 = s0 + maj;
	r.h = r.g;
	r.g = r.f;
	r.f = r.e;
	r.e = r.d + t1;
	r.d = r.c;
	r.c = r.b;
	r.b = r.a;
	r.a = t1 + t2;

	s0 = ror64(w[1], 1); s0 = s0 ^ ror64(w[1], 8); s0 = s0 ^ (w[1] >> 7);
	s1 = ror64(w[14], 19); s1 = s1 ^ ror64(w[14], 61); s1 = s1 ^ (w[14] >> 6);
	w16 = w[0] + s0 + w[9] + s1;

	i = 0;
	loop {
		if i == 15 {
			break;
		}

		w[i] = w[i + 1];

		i = i + 1;
	}

	w[15] = w16;
}

sha512_rounds(ctx: *sha512_ctx, block: *byte) {
	var r: sha512_ctx;
	var _w: _sha512_block;
	var w: *int;
	var i: int;
	w = (&_w):*int;

	r.a = ctx.a;
	r.b = ctx.b;
	r.c = ctx.c;
	r.d = ctx.d;
	r.e = ctx.e;
	r.f = ctx.f;
	r.g = ctx.g;
	r.h = ctx.h;

	i = 0;
	loop {
		if i == 16 {
			break;
		}

		w[i] = (block[0]:int << 56)
			| (block[1]:int << 48)
			| (block[2]:int << 40)
			| (block[3]:int << 32)
			| (block[4]:int << 24)
			| (block[5]:int << 16)
			| (block[6]:int << 8)
			| block[7]:int;

		block = &block[8];
		i = i + 1;
	}

	sha512_round(&r, w, (0x428a2f98 << 32) | 0xd728ae22);
	sha512_round(&r, w, (0x71374491 << 32) | 0x23ef65cd);
	sha512_round(&r, w, (0xb5c0fbcf << 32) | 0xec4d3b2f);
	sha512_round(&r, w, (0xe9b5dba5 << 32) | 0x8189dbbc);
	sha512_round(&r, w, (0x3956c25b << 32) | 0xf348b538);
	sha512_round(&r, w, (0x59f111f1 << 32) | 0xb605d019);
	sha512_round(&r, w, (0x923f82a4 << 32) | 0xaf194f9b);
	sha512_round(&r, w, (0xab1c5ed5 << 32) | 0xda6d8118);
	sha512_round(&r, w, (0xd807aa98 << 32) | 0xa3030242);
	sha512_round(&r, w, (0x12835b01 << 32) | 0x45706fbe);
	sha512_round(&r, w, (0x243185be << 32) | 0x4ee4b28c);
	sha512_round(&r, w, (0x550c7dc3 << 32) | 0xd5ffb4e2);
	sha512_round(&r, w, (0x72be5d74 << 32) | 0xf27b896f);
	sha512_round(&r, w, (0x80deb1fe << 32) | 0x3b1696b1);
	sha512_round(&r, w, (0x9bdc06a7 << 32) | 0x25c71235);
	sha512_round(&r, w, (0xc19bf174 << 32) | 0xcf692694);
	sha512_round(&r, w, (0xe49b69c1 << 32) | 0x9ef14ad2);
	sha512_round(&r, w, (0xefbe4786 << 32) | 0x384f25e3);
	sha512_round(&r, w, (0x0fc19dc6 << 32) | 0x8b8cd5b5);
	sha512_round(&r, w, (0x240ca1cc << 32) | 0x77ac9c65);
	sha512_round(&r, w, (0x2de92c6f << 32) | 0x592b0275);
	sha512_round(&r, w, (0x4a7484aa << 32) | 0x6ea6e483);
	sha512_round(&r, w, (0x5cb0a9dc << 32) | 0xbd41fbd4);
	sha512_round(&r, w, (0x76f988da << 32) | 0x831153b5);
	sha512_round(&r, w, (0x983e5152 << 32) | 0xee66dfab);
	sha512_round(&r, w, (0xa831c66d << 32) | 0x2db43210);
	sha512_round(&r, w, (0xb00327c8 << 32) | 0x98fb213f);
	sha512_round(&r, w, (0xbf597fc7 << 32) | 0xbeef0ee4);
	sha512_round(&r, w, (0xc6e00bf3 << 32) | 0x3da88fc2);
	sha512_round(&r, w, (0xd5a79147 << 32) | 0x930aa725);
	sha512_round(&r, w, (0x06ca6351 << 32) | 0xe003826f);
	sha512_round(&r, w, (0x14292967 << 32) | 0x0a0e6e70);
	sha512_round(&r, w, (0x27b70a85 << 32) | 0x46d22ffc);
	sha512_round(&r, w, (0x2e1b2138 << 32) | 0x5c26c926);
	sha512_round(&r, w, (0x4d2c6dfc << 32) | 0x5ac42aed);
	sha512_round(&r, w, (0x53380d13 << 32) | 0x9d95b3df);
	sha512_round(&r, w, (0x650a7354 << 32) | 0x8baf63de);
	sha512_round(&r, w, (0x766a0abb << 32) | 0x3c77b2a8);
	sha512_round(&r, w, (0x81c2c92e << 32) | 0x47edaee6);
	sha512_round(&r, w, (0x92722c85 << 32) | 0x1482353b);
	sha512_round(&r, w, (0xa2bfe8a1 << 32) | 0x4cf10364);
	sha512_round(&r, w, (0xa81a664b << 32) | 0xbc423001);
	sha512_round(&r, w, (0xc24b8b70 << 32) | 0xd0f89791);
	sha512_round(&r, w, (0xc76c51a3 << 32) | 0x0654be30);
	sha512_round(&r, w, (0xd192e819 << 32) | 0xd6ef5218);
	sha512_round(&r, w, (0xd6990624 << 32) | 0x5565a910);
	sha512_round(&r, w, (0xf40e3585 << 32) | 0x5771202a);
	sha512_round(&r, w, (0x106aa070 << 32) | 0x32bbd1b8);
	sha512_round(&r, w, (0x19a4c116 << 32) | 0xb8d2d0c8);
	sha512_round(&r, w, (0x1e376c08 << 32) | 0x5141ab53);
	sha512_round(&r, w, (0x2748774c << 32) | 0xdf8eeb99);
	sha512_round(&r, w, (0x34b0bcb5 << 32) | 0xe19b48a8);
	sha512_round(&r, w, (0x391c0cb3 << 32) | 0xc5c95a63);
	sha512_round(&r, w, (0x4ed8aa4a << 32) | 0xe3418acb);
	sha512_round(&r, w, (0x5b9cca4f << 32) | 0x7763e373);
	sha512_round(&r, w, (0x682e6ff3 << 32) | 0xd6b2b8a3);
	sha512_round(&r, w, (0x748f82ee << 32) | 0x5defb2fc);
	sha512_round(&r, w, (0x78a5636f << 32) | 0x43172f60);
	sha512_round(&r, w, (0x84c87814 << 32) | 0xa1f0ab72);
	sha512_round(&r, w, (0x8cc70208 << 32) | 0x1a6439ec);
	sha512_round(&r, w, (0x90befffa << 32) | 0x23631e28);
	sha512_round(&r, w, (0xa4506ceb << 32) | 0xde82bde9);
	sha512_round(&r, w, (0xbef9a3f7 << 32) | 0xb2c67915);
	sha512_round(&r, w, (0xc67178f2 << 32) | 0xe372532b);
	sha512_round(&r, w, (0xca273ece << 32) | 0xea26619c);
	sha512_round(&r, w, (0xd186b8c7 << 32) | 0x21c0c207);
	sha512_round(&r, w, (0xeada7dd6 << 32) | 0xcde0eb1e);
	sha512_round(&r, w, (0xf57d4f7f << 32) | 0xee6ed178);
	sha512_round(&r, w, (0x06f067aa << 32) | 0x72176fba);
	sha512_round(&r, w, (0x0a637dc5 << 32) | 0xa2c898a6);
	sha512_round(&r, w, (0x113f9804 << 32) | 0xbef90dae);
	sha512_round(&r, w, (0x1b710b35 << 32) | 0x131c471b);
	sha512_round(&r, w, (0x28db77f5 << 32) | 0x23047d84);
	sha512_round(&r, w, (0x32caab7b << 32) | 0x40c72493);
	sha512_round(&r, w, (0x3c9ebe0a << 32) | 0x15c9bebc);
	sha512_round(&r, w, (0x431d67c4 << 32) | 0x9c100d4c);
	sha512_round(&r, w, (0x4cc5d4be << 32) | 0xcb3e42b6);
	sha512_round(&r, w, (0x597f299c << 32) | 0xfc657e2a);
	sha512_round(&r, w, (0x5fcb6fab << 32) | 0x3ad6faec);
	sha512_round(&r, w, (0x6c44198c << 32) | 0x4a475817);

	ctx.a = ctx.a + r.a;
	ctx.b = ctx.b + r.b;
	ctx.c = ctx.c + r.c;
	ctx.d = ctx.d + r.d;
	ctx.e = ctx.e + r.e;
	ctx.f = ctx.f + r.f;
	ctx.g = ctx.g + r.g;
	ctx.h = ctx.h + r.h;
}

sha512_finish(ctx: *sha512_ctx, nblocks: int, data: *byte, len: int) {
	var _final: _sha512_block;
	var final: *byte;
	var pad: int;
	final = (&_final):*byte;

	pad = nblocks * 1024;
	loop {
		if len < 128 {
			break;
		}

		sha512_rounds(ctx, data);

		data = &data[128];
		len = len - 128;
		pad = pad + 1024;
	}
	pad = pad + len * 8;

	bzero(final, 128);
	memcpy(final, data, len);
	final[len] = 0x80:byte;

	if len + 17 > 128 {
		sha512_rounds(ctx, final);
		bzero(final, 128);
	}

	final[120] = (pad >> 56):byte;
	final[121] = (pad >> 48):byte;
	final[122] = (pad >> 40):byte;
	final[123] = (pad >> 32):byte;
	final[124] = (pad >> 24):byte;
	final[125] = (pad >> 16):byte;
	final[126] = (pad >> 8):byte;
	final[127] = pad:byte;

	sha512_rounds(ctx, final);
}

sha512_digest(digest: *byte, ctx: *sha512_ctx) {
	var r: *int;
	var i: int;
	r = &ctx.a;
	i = 0;
	loop {
		if i == 8 {
			break;
		}
		digest[0] = (r[i] >> 56):byte; digest = &digest[1];
		digest[0] = (r[i] >> 48):byte; digest = &digest[1];
		digest[0] = (r[i] >> 40):byte; digest = &digest[1];
		digest[0] = (r[i] >> 32):byte; digest = &digest[1];
		digest[0] = (r[i] >> 24):byte; digest = &digest[1];
		digest[0] = (r[i] >> 16):byte; digest = &digest[1];
		digest[0] = (r[i] >> 8):byte; digest = &digest[1];
		digest[0] = r[i]:byte; digest = &digest[1];
		i = i + 1;
	}
}

sha512(digest: *byte, data: *byte, dlen: int) {
	var ctx: sha512_ctx;
	sha512_init(&ctx);
	sha512_finish(&ctx, 0, data, dlen);
	sha512_digest(digest, &ctx);
}

sha512_hmac(mac: *byte, key: *byte, klen: int, data: *byte, dlen: int) {
	var _digest: _sha512_block;
	var digest: *byte;
	var _ipad: _sha512_block;
	var ipad: *byte;
	var _opad: _sha512_block;
	var opad: *byte;
	var ctx: sha512_ctx;
	var i: int;

	digest = (&_digest):*byte;
	ipad = (&_ipad):*byte;
	opad = (&_opad):*byte;

	if (klen <= 128) {
		bzero(digest, 128);
		memcpy(digest, key, klen);
	} else {
		sha512(digest, key, klen);
	}

	i = 0;
	loop {
		if i == 128 {
			break;
		}

		ipad[i] = digest[i] ^ (0x36:byte);
		opad[i] = digest[i] ^ (0x5c:byte);

		i = i + 1;
	}

	sha512_init(&ctx);
	sha512_rounds(&ctx, ipad);
	sha512_finish(&ctx, 1, data, dlen);
	sha512_digest(digest, &ctx);

	sha512_init(&ctx);
	sha512_rounds(&ctx, opad);
	sha512_finish(&ctx, 1, digest, 64);
	sha512_digest(mac, &ctx);
}

sha512_update(ctx: *sha512_ctx, msg: *byte, len: int) {
	var o: int;
	var r: int;

	o = ctx.len & 127;
	if o != 0 {
		r = 128 - o;
		if len < r {
			memcpy(&(&ctx.block):*byte[o], msg, len);
			ctx.len = ctx.len + len;
			return;
		} else {
			memcpy(&(&ctx.block):*byte[o], msg, r);
			sha512_rounds(ctx, (&ctx.block):*byte);
			ctx.len = ctx.len + r;
			len = len - r;
			msg = &msg[r];
		}
	}

	loop {
		if len < 128 {
			memcpy((&ctx.block):*byte, msg, len);
			ctx.len = ctx.len + len;
			return;
		}

		sha512_rounds(ctx, msg);

		ctx.len = ctx.len + 128;
		len = len - 128;
		msg = &msg[128];
	}
}

sha512_final(digest: *byte, ctx: *sha512_ctx) {
	sha512_finish(ctx, ctx.len >> 7, (&ctx.block):*byte, ctx.len & 127);
	sha512_digest(digest, ctx);
}
