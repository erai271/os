// https://www.rfc-editor.org/rfc/rfc4634

struct sha256_ctx {
	a: int;
	b: int;
	c: int;
	d: int;
	e: int;
	f: int;
	g: int;
	h: int;
	len: int;
	block: _sha256_block;
}

struct _sha256_block {
	w0: int;
	w1: int;
	w2: int;
	w3: int;
	w4: int;
	w5: int;
	w6: int;
	w7: int;
	w8: int;
	w9: int;
	w10: int;
	w11: int;
	w12: int;
	w13: int;
	w14: int;
	w15: int;
}

struct _sha256_digest {
	d0: int;
	d1: int;
	d2: int;
	d3: int;
	d4: int;
}

sha256_init(r: *sha256_ctx) {
	r.a = 0x6a09e667;
	r.b = 0xbb67ae85;
	r.c = 0x3c6ef372;
	r.d = 0xa54ff53a;
	r.e = 0x510e527f;
	r.f = 0x9b05688c;
	r.g = 0x1f83d9ab;
	r.h = 0x5be0cd19;
	r.len = 0;
}

ror32(x: int, n: int): int {
	return ((x >> n) | (x << (32 - n))) & (-1 >> 32);
}

sha256_round(ctx: *sha256_ctx, w: *int, k: int) {
	var t1: int;
	var t2: int;
	var s0: int;
	var s1: int;
	var ch: int;
	var maj: int;
	var w16: int;

	s1 = ror32(ctx.e, 6); s1 = s1 ^ ror32(ctx.e, 11); s1 = s1 ^ ror32(ctx.e, 25);
	ch = (ctx.e & ctx.f) ^ ((~ctx.e) & ctx.g);
	t1 = (ctx.h + s1 + ch + k + w[0]) & (-1 >> 32);
	s0 = ror32(ctx.a, 2); s0 = s0 ^ ror32(ctx.a, 13); s0 = s0 ^ ror32(ctx.a, 22);
	maj = (ctx.a & ctx.b) ^ (ctx.a & ctx.c) ^ (ctx.b & ctx.c);
	t2 = (s0 + maj) & (-1 >> 32);

	s0 = ror32(w[16 - 15], 7); s0 = s0 ^ ror32(w[16 - 15], 18); s0 = s0 ^ (w[16 - 15] >> 3);
	s1 = ror32(w[16 - 2], 17); s1 = s1 ^ ror32(w[16 - 2], 19); s1 = s1 ^ (w[16 - 2] >> 10);
	w16 = (w[16 - 16] + s0 + w[16 - 7] + s1) & (-1 >> 32);

	w[0] = w[1];
	w[1] = w[2];
	w[2] = w[3];
	w[3] = w[4];
	w[4] = w[5];
	w[5] = w[6];
	w[6] = w[7];
	w[7] = w[8];
	w[8] = w[9];
	w[9] = w[10];
	w[10] = w[11];
	w[11] = w[12];
	w[12] = w[13];
	w[13] = w[14];
	w[14] = w[15];
	w[15] = w16;

	ctx.h = ctx.g;
	ctx.g = ctx.f;
	ctx.f = ctx.e;
	ctx.e = (ctx.d + t1) & (-1 >> 32);
	ctx.d = ctx.c;
	ctx.c = ctx.b;
	ctx.b = ctx.a;
	ctx.a = (t1 + t2) & (-1 >> 32);
}

sha256_rounds(ctx: *sha256_ctx, block: *byte) {
	var _w: _sha256_block;
	var w: *int;
	var r: sha256_ctx;
	w = &_w.w0;

	r.a = ctx.a;
	r.b = ctx.b;
	r.c = ctx.c;
	r.d = ctx.d;
	r.e = ctx.e;
	r.f = ctx.f;
	r.g = ctx.g;
	r.h = ctx.h;

	w[0] = (block[0]:int << 24)
		| (block[1]:int << 16)
		| (block[2]:int << 8)
		| block[3]:int;
	w[1] = (block[4]:int << 24)
		| (block[5]:int << 16)
		| (block[6]:int << 8)
		| block[7]:int;
	w[2] = (block[8]:int << 24)
		| (block[9]:int << 16)
		| (block[10]:int << 8)
		| block[11]:int;
	w[3] = (block[12]:int << 24)
		| (block[13]:int << 16)
		| (block[14]:int << 8)
		| block[15]:int;
	w[4] = (block[16]:int << 24)
		| (block[17]:int << 16)
		| (block[18]:int << 8)
		| block[19]:int;
	w[5] = (block[20]:int << 24)
		| (block[21]:int << 16)
		| (block[22]:int << 8)
		| block[23]:int;
	w[6] = (block[24]:int << 24)
		| (block[25]:int << 16)
		| (block[26]:int << 8)
		| block[27]:int;
	w[7] = (block[28]:int << 24)
		| (block[29]:int << 16)
		| (block[30]:int << 8)
		| block[31]:int;
	w[8] = (block[32]:int << 24)
		| (block[33]:int << 16)
		| (block[34]:int << 8)
		| block[35]:int;
	w[9] = (block[36]:int << 24)
		| (block[37]:int << 16)
		| (block[38]:int << 8)
		| block[39]:int;
	w[10] = (block[40]:int << 24)
		| (block[41]:int << 16)
		| (block[42]:int << 8)
		| block[43]:int;
	w[11] = (block[44]:int << 24)
		| (block[45]:int << 16)
		| (block[46]:int << 8)
		| block[47]:int;
	w[12] = (block[48]:int << 24)
		| (block[49]:int << 16)
		| (block[50]:int << 8)
		| block[51]:int;
	w[13] = (block[52]:int << 24)
		| (block[53]:int << 16)
		| (block[54]:int << 8)
		| block[55]:int;
	w[14] = (block[56]:int << 24)
		| (block[57]:int << 16)
		| (block[58]:int << 8)
		| block[59]:int;
	w[15] = (block[60]:int << 24)
		| (block[61]:int << 16)
		| (block[62]:int << 8)
		| block[63]:int;

	sha256_round(&r, w, 0x428a2f98);
	sha256_round(&r, w, 0x71374491);
	sha256_round(&r, w, 0xb5c0fbcf);
	sha256_round(&r, w, 0xe9b5dba5);
	sha256_round(&r, w, 0x3956c25b);
	sha256_round(&r, w, 0x59f111f1);
	sha256_round(&r, w, 0x923f82a4);
	sha256_round(&r, w, 0xab1c5ed5);
	sha256_round(&r, w, 0xd807aa98);
	sha256_round(&r, w, 0x12835b01);
	sha256_round(&r, w, 0x243185be);
	sha256_round(&r, w, 0x550c7dc3);
	sha256_round(&r, w, 0x72be5d74);
	sha256_round(&r, w, 0x80deb1fe);
	sha256_round(&r, w, 0x9bdc06a7);
	sha256_round(&r, w, 0xc19bf174);
	sha256_round(&r, w, 0xe49b69c1);
	sha256_round(&r, w, 0xefbe4786);
	sha256_round(&r, w, 0x0fc19dc6);
	sha256_round(&r, w, 0x240ca1cc);
	sha256_round(&r, w, 0x2de92c6f);
	sha256_round(&r, w, 0x4a7484aa);
	sha256_round(&r, w, 0x5cb0a9dc);
	sha256_round(&r, w, 0x76f988da);
	sha256_round(&r, w, 0x983e5152);
	sha256_round(&r, w, 0xa831c66d);
	sha256_round(&r, w, 0xb00327c8);
	sha256_round(&r, w, 0xbf597fc7);
	sha256_round(&r, w, 0xc6e00bf3);
	sha256_round(&r, w, 0xd5a79147);
	sha256_round(&r, w, 0x06ca6351);
	sha256_round(&r, w, 0x14292967);
	sha256_round(&r, w, 0x27b70a85);
	sha256_round(&r, w, 0x2e1b2138);
	sha256_round(&r, w, 0x4d2c6dfc);
	sha256_round(&r, w, 0x53380d13);
	sha256_round(&r, w, 0x650a7354);
	sha256_round(&r, w, 0x766a0abb);
	sha256_round(&r, w, 0x81c2c92e);
	sha256_round(&r, w, 0x92722c85);
	sha256_round(&r, w, 0xa2bfe8a1);
	sha256_round(&r, w, 0xa81a664b);
	sha256_round(&r, w, 0xc24b8b70);
	sha256_round(&r, w, 0xc76c51a3);
	sha256_round(&r, w, 0xd192e819);
	sha256_round(&r, w, 0xd6990624);
	sha256_round(&r, w, 0xf40e3585);
	sha256_round(&r, w, 0x106aa070);
	sha256_round(&r, w, 0x19a4c116);
	sha256_round(&r, w, 0x1e376c08);
	sha256_round(&r, w, 0x2748774c);
	sha256_round(&r, w, 0x34b0bcb5);
	sha256_round(&r, w, 0x391c0cb3);
	sha256_round(&r, w, 0x4ed8aa4a);
	sha256_round(&r, w, 0x5b9cca4f);
	sha256_round(&r, w, 0x682e6ff3);
	sha256_round(&r, w, 0x748f82ee);
	sha256_round(&r, w, 0x78a5636f);
	sha256_round(&r, w, 0x84c87814);
	sha256_round(&r, w, 0x8cc70208);
	sha256_round(&r, w, 0x90befffa);
	sha256_round(&r, w, 0xa4506ceb);
	sha256_round(&r, w, 0xbef9a3f7);
	sha256_round(&r, w, 0xc67178f2);

	ctx.a = (ctx.a + r.a) & (-1 >> 32);
	ctx.b = (ctx.b + r.b) & (-1 >> 32);
	ctx.c = (ctx.c + r.c) & (-1 >> 32);
	ctx.d = (ctx.d + r.d) & (-1 >> 32);
	ctx.e = (ctx.e + r.e) & (-1 >> 32);
	ctx.f = (ctx.f + r.f) & (-1 >> 32);
	ctx.g = (ctx.g + r.g) & (-1 >> 32);
	ctx.h = (ctx.h + r.h) & (-1 >> 32);
}

sha256_finish(ctx: *sha256_ctx, nblocks: int, data: *byte, len: int) {
	var _final: sha256_ctx;
	var final: *byte;
	var pad: int;
	var i: int;

	final = (&_final):*byte;

	pad = nblocks * 512;

	loop {
		if len < 64 {
			break;
		}

		sha256_rounds(ctx, data);

		data = &data[64];
		len = len - 64;
		pad = pad + 512;
	}

	pad = pad + len * 8;

	i = 0;
	loop {
		if i == len {
			break;
		}

		final[i] = data[i];

		i = i + 1;
	}

	final[i] = 0x80:byte;
	i = i + 1;

	loop {
		if i == 64 {
			break;
		}

		final[i] = 0:byte;

		i = i + 1;
	}

	if len + 9 > 64 {
		sha256_rounds(ctx, final);

		i = 0;
		loop {
			if i == 64 {
				break;
			}

			final[i] = 0:byte;

			i = i + 1;
		}
	}

	final[56] = (pad >> 56):byte;
	final[57] = (pad >> 48):byte;
	final[58] = (pad >> 40):byte;
	final[59] = (pad >> 32):byte;
	final[60] = (pad >> 24):byte;
	final[61] = (pad >> 16):byte;
	final[62] = (pad >> 8):byte;
	final[63] = pad:byte;

	sha256_rounds(ctx, final);
}

sha256_digest(digest: *byte, ctx: *sha256_ctx) {
	digest[0] = (ctx.a >> 24):byte;
	digest[1] = (ctx.a >> 16):byte;
	digest[2] = (ctx.a >> 8):byte;
	digest[3] = ctx.a:byte;
	digest[4] = (ctx.b >> 24):byte;
	digest[5] = (ctx.b >> 16):byte;
	digest[6] = (ctx.b >> 8):byte;
	digest[7] = ctx.b:byte;
	digest[8] = (ctx.c >> 24):byte;
	digest[9] = (ctx.c >> 16):byte;
	digest[10] = (ctx.c >> 8):byte;
	digest[11] = ctx.c:byte;
	digest[12] = (ctx.d >> 24):byte;
	digest[13] = (ctx.d >> 16):byte;
	digest[14] = (ctx.d >> 8):byte;
	digest[15] = ctx.d:byte;
	digest[16] = (ctx.e >> 24):byte;
	digest[17] = (ctx.e >> 16):byte;
	digest[18] = (ctx.e >> 8):byte;
	digest[19] = ctx.e:byte;
	digest[20] = (ctx.f >> 24):byte;
	digest[21] = (ctx.f >> 16):byte;
	digest[22] = (ctx.f >> 8):byte;
	digest[23] = ctx.f:byte;
	digest[24] = (ctx.g >> 24):byte;
	digest[25] = (ctx.g >> 16):byte;
	digest[26] = (ctx.g >> 8):byte;
	digest[27] = ctx.g:byte;
	digest[28] = (ctx.h >> 24):byte;
	digest[29] = (ctx.h >> 16):byte;
	digest[30] = (ctx.h >> 8):byte;
	digest[31] = ctx.h:byte;
}

sha256(digest: *byte, data: *byte, dlen: int) {
	var ctx: sha256_ctx;
	sha256_init(&ctx);
	sha256_finish(&ctx, 0, data, dlen);
	sha256_digest(digest, &ctx);
}

sha256_hmac(mac: *byte, key: *byte, klen: int, data: *byte, dlen: int) {
	var _digest: _sha256_block;
	var digest: *byte;
	var _ipad: _sha256_block;
	var ipad: *byte;
	var _opad: _sha256_block;
	var opad: *byte;
	var ctx: sha256_ctx;
	var i: int;

	digest = (&_digest):*byte;
	ipad = (&_ipad):*byte;
	opad = (&_opad):*byte;

	i = 0;
	loop {
		if i == 64 {
			break;
		}

		digest[i] = 0: byte;

		i = i + 1;
	}

	if (klen <= 64) {
		i = 0;
		loop {
			if i == klen {
				break;
			}

			digest[i] = key[i];

			i = i + 1;
		}
	} else {
		sha256(digest, key, klen);
	}

	i = 0;
	loop {
		if i == 64 {
			break;
		}

		ipad[i] = digest[i] ^ (0x36:byte);
		opad[i] = digest[i] ^ (0x5c:byte);

		i = i + 1;
	}

	sha256_init(&ctx);
	sha256_rounds(&ctx, ipad);
	sha256_finish(&ctx, 1, data, dlen);
	sha256_digest(digest, &ctx);

	sha256_init(&ctx);
	sha256_rounds(&ctx, opad);
	sha256_finish(&ctx, 1, digest, 32);
	sha256_digest(mac, &ctx);
}

sha256_update(ctx: *sha256_ctx, msg: *byte, len: int) {
	var o: int;
	var r: int;

	o = ctx.len & 63;
	if o != 0 {
		r = 64 - o;
		if len < r {
			memcpy(&(&ctx.block):*byte[o], msg, len);
			ctx.len = ctx.len + len;
			return;
		} else {
			memcpy(&(&ctx.block):*byte[o], msg, r);
			sha256_rounds(ctx, (&ctx.block):*byte);
			ctx.len = ctx.len + r;
			len = len - r;
			msg = &msg[r];
		}
	}

	loop {
		if len < 64 {
			memcpy((&ctx.block):*byte, msg, len);
			ctx.len = ctx.len + len;
			return;
		}

		sha256_rounds(ctx, msg);

		ctx.len = ctx.len + 64;
		len = len - 64;
		msg = &msg[64];
	}
}

sha256_final(digest: *byte, ctx: *sha256_ctx) {
	sha256_finish(ctx, ctx.len >> 6, (&ctx.block):*byte, ctx.len & 63);
	sha256_digest(digest, ctx);
}
