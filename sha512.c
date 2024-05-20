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
	ctx.a = (0x6a09 << 48) | (0xe667 << 32) | (0xf3bc << 16) | 0xc908;
	ctx.b = (0xbb67 << 48) | (0xae85 << 32) | (0x84ca << 16) | 0xa73b;
	ctx.c = (0x3c6e << 48) | (0xf372 << 32) | (0xfe94 << 16) | 0xf82b;
	ctx.d = (0xa54f << 48) | (0xf53a << 32) | (0x5f1d << 16) | 0x36f1;
	ctx.e = (0x510e << 48) | (0x527f << 32) | (0xade6 << 16) | 0x82d1;
	ctx.f = (0x9b05 << 48) | (0x688c << 32) | (0x2b3e << 16) | 0x6c1f;
	ctx.g = (0x1f83 << 48) | (0xd9ab << 32) | (0xfb41 << 16) | 0xbd6b;
	ctx.h = (0x5be0 << 48) | (0xcd19 << 32) | (0x137e << 16) | 0x2179;
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

	s1 = ror64(r.e, 14) ^ ror64(r.e, 18) ^ ror64(r.e, 41);
	ch = (r.e & r.f) ^ ((~r.e) & r.g);
	t1 = r.h + s1 + ch + k + w[0];
	s0 = ror64(r.a, 28) ^ ror64(r.a, 34) ^ ror64(r.a, 39);
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

	s0 = ror64(w[1], 1) ^ ror64(w[1], 8) ^ (w[1] >> 7);
	s1 = ror64(w[14], 19) ^ ror64(w[14], 61) ^ (w[14] >> 6);
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

	sha512_round(&r, w, (0x428a << 48) | (0x2f98 << 32) | (0xd728 << 16) | 0xae22);
	sha512_round(&r, w, (0x7137 << 48) | (0x4491 << 32) | (0x23ef << 16) | 0x65cd);
	sha512_round(&r, w, (0xb5c0 << 48) | (0xfbcf << 32) | (0xec4d << 16) | 0x3b2f);
	sha512_round(&r, w, (0xe9b5 << 48) | (0xdba5 << 32) | (0x8189 << 16) | 0xdbbc);
	sha512_round(&r, w, (0x3956 << 48) | (0xc25b << 32) | (0xf348 << 16) | 0xb538);
	sha512_round(&r, w, (0x59f1 << 48) | (0x11f1 << 32) | (0xb605 << 16) | 0xd019);
	sha512_round(&r, w, (0x923f << 48) | (0x82a4 << 32) | (0xaf19 << 16) | 0x4f9b);
	sha512_round(&r, w, (0xab1c << 48) | (0x5ed5 << 32) | (0xda6d << 16) | 0x8118);
	sha512_round(&r, w, (0xd807 << 48) | (0xaa98 << 32) | (0xa303 << 16) | 0x0242);
	sha512_round(&r, w, (0x1283 << 48) | (0x5b01 << 32) | (0x4570 << 16) | 0x6fbe);
	sha512_round(&r, w, (0x2431 << 48) | (0x85be << 32) | (0x4ee4 << 16) | 0xb28c);
	sha512_round(&r, w, (0x550c << 48) | (0x7dc3 << 32) | (0xd5ff << 16) | 0xb4e2);
	sha512_round(&r, w, (0x72be << 48) | (0x5d74 << 32) | (0xf27b << 16) | 0x896f);
	sha512_round(&r, w, (0x80de << 48) | (0xb1fe << 32) | (0x3b16 << 16) | 0x96b1);
	sha512_round(&r, w, (0x9bdc << 48) | (0x06a7 << 32) | (0x25c7 << 16) | 0x1235);
	sha512_round(&r, w, (0xc19b << 48) | (0xf174 << 32) | (0xcf69 << 16) | 0x2694);
	sha512_round(&r, w, (0xe49b << 48) | (0x69c1 << 32) | (0x9ef1 << 16) | 0x4ad2);
	sha512_round(&r, w, (0xefbe << 48) | (0x4786 << 32) | (0x384f << 16) | 0x25e3);
	sha512_round(&r, w, (0x0fc1 << 48) | (0x9dc6 << 32) | (0x8b8c << 16) | 0xd5b5);
	sha512_round(&r, w, (0x240c << 48) | (0xa1cc << 32) | (0x77ac << 16) | 0x9c65);
	sha512_round(&r, w, (0x2de9 << 48) | (0x2c6f << 32) | (0x592b << 16) | 0x0275);
	sha512_round(&r, w, (0x4a74 << 48) | (0x84aa << 32) | (0x6ea6 << 16) | 0xe483);
	sha512_round(&r, w, (0x5cb0 << 48) | (0xa9dc << 32) | (0xbd41 << 16) | 0xfbd4);
	sha512_round(&r, w, (0x76f9 << 48) | (0x88da << 32) | (0x8311 << 16) | 0x53b5);
	sha512_round(&r, w, (0x983e << 48) | (0x5152 << 32) | (0xee66 << 16) | 0xdfab);
	sha512_round(&r, w, (0xa831 << 48) | (0xc66d << 32) | (0x2db4 << 16) | 0x3210);
	sha512_round(&r, w, (0xb003 << 48) | (0x27c8 << 32) | (0x98fb << 16) | 0x213f);
	sha512_round(&r, w, (0xbf59 << 48) | (0x7fc7 << 32) | (0xbeef << 16) | 0x0ee4);
	sha512_round(&r, w, (0xc6e0 << 48) | (0x0bf3 << 32) | (0x3da8 << 16) | 0x8fc2);
	sha512_round(&r, w, (0xd5a7 << 48) | (0x9147 << 32) | (0x930a << 16) | 0xa725);
	sha512_round(&r, w, (0x06ca << 48) | (0x6351 << 32) | (0xe003 << 16) | 0x826f);
	sha512_round(&r, w, (0x1429 << 48) | (0x2967 << 32) | (0x0a0e << 16) | 0x6e70);
	sha512_round(&r, w, (0x27b7 << 48) | (0x0a85 << 32) | (0x46d2 << 16) | 0x2ffc);
	sha512_round(&r, w, (0x2e1b << 48) | (0x2138 << 32) | (0x5c26 << 16) | 0xc926);
	sha512_round(&r, w, (0x4d2c << 48) | (0x6dfc << 32) | (0x5ac4 << 16) | 0x2aed);
	sha512_round(&r, w, (0x5338 << 48) | (0x0d13 << 32) | (0x9d95 << 16) | 0xb3df);
	sha512_round(&r, w, (0x650a << 48) | (0x7354 << 32) | (0x8baf << 16) | 0x63de);
	sha512_round(&r, w, (0x766a << 48) | (0x0abb << 32) | (0x3c77 << 16) | 0xb2a8);
	sha512_round(&r, w, (0x81c2 << 48) | (0xc92e << 32) | (0x47ed << 16) | 0xaee6);
	sha512_round(&r, w, (0x9272 << 48) | (0x2c85 << 32) | (0x1482 << 16) | 0x353b);
	sha512_round(&r, w, (0xa2bf << 48) | (0xe8a1 << 32) | (0x4cf1 << 16) | 0x0364);
	sha512_round(&r, w, (0xa81a << 48) | (0x664b << 32) | (0xbc42 << 16) | 0x3001);
	sha512_round(&r, w, (0xc24b << 48) | (0x8b70 << 32) | (0xd0f8 << 16) | 0x9791);
	sha512_round(&r, w, (0xc76c << 48) | (0x51a3 << 32) | (0x0654 << 16) | 0xbe30);
	sha512_round(&r, w, (0xd192 << 48) | (0xe819 << 32) | (0xd6ef << 16) | 0x5218);
	sha512_round(&r, w, (0xd699 << 48) | (0x0624 << 32) | (0x5565 << 16) | 0xa910);
	sha512_round(&r, w, (0xf40e << 48) | (0x3585 << 32) | (0x5771 << 16) | 0x202a);
	sha512_round(&r, w, (0x106a << 48) | (0xa070 << 32) | (0x32bb << 16) | 0xd1b8);
	sha512_round(&r, w, (0x19a4 << 48) | (0xc116 << 32) | (0xb8d2 << 16) | 0xd0c8);
	sha512_round(&r, w, (0x1e37 << 48) | (0x6c08 << 32) | (0x5141 << 16) | 0xab53);
	sha512_round(&r, w, (0x2748 << 48) | (0x774c << 32) | (0xdf8e << 16) | 0xeb99);
	sha512_round(&r, w, (0x34b0 << 48) | (0xbcb5 << 32) | (0xe19b << 16) | 0x48a8);
	sha512_round(&r, w, (0x391c << 48) | (0x0cb3 << 32) | (0xc5c9 << 16) | 0x5a63);
	sha512_round(&r, w, (0x4ed8 << 48) | (0xaa4a << 32) | (0xe341 << 16) | 0x8acb);
	sha512_round(&r, w, (0x5b9c << 48) | (0xca4f << 32) | (0x7763 << 16) | 0xe373);
	sha512_round(&r, w, (0x682e << 48) | (0x6ff3 << 32) | (0xd6b2 << 16) | 0xb8a3);
	sha512_round(&r, w, (0x748f << 48) | (0x82ee << 32) | (0x5def << 16) | 0xb2fc);
	sha512_round(&r, w, (0x78a5 << 48) | (0x636f << 32) | (0x4317 << 16) | 0x2f60);
	sha512_round(&r, w, (0x84c8 << 48) | (0x7814 << 32) | (0xa1f0 << 16) | 0xab72);
	sha512_round(&r, w, (0x8cc7 << 48) | (0x0208 << 32) | (0x1a64 << 16) | 0x39ec);
	sha512_round(&r, w, (0x90be << 48) | (0xfffa << 32) | (0x2363 << 16) | 0x1e28);
	sha512_round(&r, w, (0xa450 << 48) | (0x6ceb << 32) | (0xde82 << 16) | 0xbde9);
	sha512_round(&r, w, (0xbef9 << 48) | (0xa3f7 << 32) | (0xb2c6 << 16) | 0x7915);
	sha512_round(&r, w, (0xc671 << 48) | (0x78f2 << 32) | (0xe372 << 16) | 0x532b);
	sha512_round(&r, w, (0xca27 << 48) | (0x3ece << 32) | (0xea26 << 16) | 0x619c);
	sha512_round(&r, w, (0xd186 << 48) | (0xb8c7 << 32) | (0x21c0 << 16) | 0xc207);
	sha512_round(&r, w, (0xeada << 48) | (0x7dd6 << 32) | (0xcde0 << 16) | 0xeb1e);
	sha512_round(&r, w, (0xf57d << 48) | (0x4f7f << 32) | (0xee6e << 16) | 0xd178);
	sha512_round(&r, w, (0x06f0 << 48) | (0x67aa << 32) | (0x7217 << 16) | 0x6fba);
	sha512_round(&r, w, (0x0a63 << 48) | (0x7dc5 << 32) | (0xa2c8 << 16) | 0x98a6);
	sha512_round(&r, w, (0x113f << 48) | (0x9804 << 32) | (0xbef9 << 16) | 0x0dae);
	sha512_round(&r, w, (0x1b71 << 48) | (0x0b35 << 32) | (0x131c << 16) | 0x471b);
	sha512_round(&r, w, (0x28db << 48) | (0x77f5 << 32) | (0x2304 << 16) | 0x7d84);
	sha512_round(&r, w, (0x32ca << 48) | (0xab7b << 32) | (0x40c7 << 16) | 0x2493);
	sha512_round(&r, w, (0x3c9e << 48) | (0xbe0a << 32) | (0x15c9 << 16) | 0xbebc);
	sha512_round(&r, w, (0x431d << 48) | (0x67c4 << 32) | (0x9c10 << 16) | 0x0d4c);
	sha512_round(&r, w, (0x4cc5 << 48) | (0xd4be << 32) | (0xcb3e << 16) | 0x42b6);
	sha512_round(&r, w, (0x597f << 48) | (0x299c << 32) | (0xfc65 << 16) | 0x7e2a);
	sha512_round(&r, w, (0x5fcb << 48) | (0x6fab << 32) | (0x3ad6 << 16) | 0xfaec);
	sha512_round(&r, w, (0x6c44 << 48) | (0x198c << 32) | (0x4a47 << 16) | 0x5817);

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
