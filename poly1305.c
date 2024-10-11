// https://www.rfc-editor.org/rfc/rfc7539

struct _poly1305_ctx {
	a0: int;
	a1: int;
	a2: int;
	a3: int;
	a4: int;
	a5: int;
	a6: int;
	a7: int;
	a8: int;
	a9: int;
}

poly1305_reduce(a: *int) {
	var c: int;
	var k: int;

	c = (a[0] + 5) >> 32;
	c = (c + a[1]) >> 32;
	c = (c + a[2]) >> 32;
	c = (c + a[3]) >> 32;
	c = (c + a[4] + (-4 & (-1 >> 32))) >> 32;

	k = -c >> 32;

	c = c + a[0] + (4 & k); a[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1]; a[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2]; a[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3]; a[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] + (-4 & (-1 >> 32) & k); a[4] = c & (-1 >> 32); c = c >> 32;
}

poly1305_load(dest: *int, src: *byte, n: int) {
	var i: int;
	i = 0;
	loop {
		if i == n {
			break;
		}
		dest[i] = src[i * 4]:int
			| (src[i * 4 + 1]:int << 8)
			| (src[i * 4 + 2]:int << 16)
			| (src[i * 4 + 3]:int << 24);
		i = i + 1;
	}
}

poly1305_digest(dest: *byte, src: *int) {
	dest[0] = src[0]:byte;
	dest[1] = (src[0] >> 8):byte;
	dest[2] = (src[0] >> 16):byte;
	dest[3] = (src[0] >> 24):byte;
	dest[4] = src[1]:byte;
	dest[5] = (src[1] >> 8):byte;
	dest[6] = (src[1] >> 16):byte;
	dest[7] = (src[1] >> 24):byte;
	dest[8] = src[2]:byte;
	dest[9] = (src[2] >> 8):byte;
	dest[10] = (src[2] >> 16):byte;
	dest[11] = (src[2] >> 24):byte;
	dest[12] = src[3]:byte;
	dest[13] = (src[3] >> 8):byte;
	dest[14] = (src[3] >> 16):byte;
	dest[15] = (src[3] >> 24):byte;
}

poly1305_add(a: *int, x: *int) {
	var c: int;
	c = x[0] + a[0]; a[0] = c & (-1 >> 32); c = c >> 32;
	c = c + x[1] + a[1]; a[1] = c & (-1 >> 32); c = c >> 32;
	c = c + x[2] + a[2]; a[2] = c & (-1 >> 32); c = c >> 32;
	c = c + x[3] + a[3]; a[3] = c & (-1 >> 32); c = c >> 32;
	c = c + x[4] + a[4]; a[4] = c & (-1 >> 32);
}

poly1305_mul(a: *int, r: *int) {
	var _x: _poly1305_ctx;
	var x: *int;
	var i: int;
	var c: int;

	x = (&_x):*int;

	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	x[3] = 0;
	x[4] = 0;
	x[5] = 0;
	x[6] = 0;
	x[7] = 0;
	x[8] = 0;
	x[9] = 0;

	// Schoolbook Multiplication
	c = a[0] * r[0] + x[0]; x[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[0] * r[1] + x[1]; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[0] * r[2] + x[2]; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[0] * r[3] + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[0] * r[4] + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	x[5] = (c + x[5]) & (-1 >> 32);

	c = a[1] * r[0] + x[1]; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] * r[1] + x[2]; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] * r[2] + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] * r[3] + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] * r[4] + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	x[6] = (c + x[6]) & (-1 >> 32);

	c = a[2] * r[0] + x[2]; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] * r[1] + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] * r[2] + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] * r[3] + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] * r[4] + x[6]; x[6] = c & (-1 >> 32); c = c >> 32;
	x[7] = (c + x[7]) & (-1 >> 32);

	c = a[3] * r[0] + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] * r[1] + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] * r[2] + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] * r[3] + x[6]; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] * r[4] + x[7]; x[7] = c & (-1 >> 32); c = c >> 32;
	x[8] = (c + x[8]) & (-1 >> 32);

	c = a[4] * r[0] + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] * r[1] + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] * r[2] + x[6]; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] * r[3] + x[7]; x[7] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] * r[4] + x[8]; x[8] = c & (-1 >> 32); c = c >> 32;
	x[9] = (c + x[9]) & (-1 >> 32);

	a[0] = x[0];
	a[1] = x[1];
	a[2] = x[2];
	a[3] = x[3];
	a[4] = x[4] & 3;

	// Modular reduction
	c = (x[4] >> 2) * 5 + ((x[5] & 3) << 30) * 5 + a[0]; a[0] = c & (-1 >> 32); c = c >> 32;
	c = c + (x[5] >> 2) * 5 + ((x[6] & 3) << 30) * 5 + a[1]; a[1] = c & (-1 >> 32); c = c >> 32;
	c = c + (x[6] >> 2) * 5 + ((x[7] & 3) << 30) * 5 + a[2]; a[2] = c & (-1 >> 32); c = c >> 32;
	c = c + (x[7] >> 2) * 5 + ((x[8] & 3) << 30) * 5 + a[3]; a[3] = c & (-1 >> 32); c = c >> 32;
	c = c + (x[8] >> 2) * 5 + ((x[9] & 3) << 30) * 5 + a[4]; a[4] = c & (-1 >> 32); c = c >> 32;
}

poly1305_truncate(dest: *int, key: *byte) {
	poly1305_load(dest, key, 4);

	dest[0] = dest[0] & 0x0fffffff;
	dest[1] = dest[1] & 0x0ffffffc;
	dest[2] = dest[2] & 0x0ffffffc;
	dest[3] = dest[3] & 0x0ffffffc;
	dest[4] = 0;
}

poly1305_pad(dest: *int, msg: *byte, n: int) {
	var _pad: _poly1305_ctx;
	var pad: *byte;
	var i: int;

	pad = (&_pad):*byte;

	i = 0;
	loop {
		if i == n {
			break;
		}

		pad[i] = msg[i];

		i = i + 1;
	}

	pad[i] = 1:byte;
	i = i + 1;

	loop {
		if i == 20 {
			break;
		}

		pad[i] = 0:byte;

		i = i + 1;
	}

	poly1305_load(dest, pad, 5);
}

poly1305(mac: *byte, key: *byte, msg: *byte, len: int) {
	var _a: _poly1305_ctx;
	var a: *int;
	var _r: _poly1305_ctx;
	var r: *int;
	var _s: _poly1305_ctx;
	var s: *int;
	var i: int;

	a = (&_a):*int;
	r = (&_r):*int;
	s = (&_s):*int;

	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	a[3] = 0;
	a[4] = 0;

	poly1305_truncate(r, key);

	i = 0;
	loop {
		if i >= len {
			break;
		}

		if (len - i >= 16) {
			poly1305_pad(s, &msg[i], 16);
		} else {
			poly1305_pad(s, &msg[i], len - i);
		}

		poly1305_add(a, s);
		poly1305_reduce(a);

		poly1305_mul(a, r);
		poly1305_reduce(a);

		i = i + 16;
	}

	poly1305_load(s, &key[16], 4);
	s[4] = 0;

	poly1305_add(a, s);

	poly1305_digest(mac, a);
}
