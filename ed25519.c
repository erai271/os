// https://www.rfc-editor.org/rfc/rfc8032
// p = 2**255 - 19
// = 0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffed
// = 57896044618658097711785492504343953926634992332820282019728792003956564819949
// order
// L = 2**252 + 0x14def9dea2f79cd65812631a5cf5d3ed
// = 0x1000000000000000000000000000000014def9dea2f79cd65812631a5cf5d3ed
// = 7237005577332262213973186563042994240857116359379907606001950938285454250989
// cofactor = 8
//
// cv25519: v**2 = u**3 + a*u**2 + u mod p
// a = 486662
// = 0x76d06
// u = 9
// = 0x9
// v = 14781619447589544791020593568409986887264606134616475288964881837755586237401
// = 0x20ae19a1b8a086b4e01edd2c7748d14c923d4d7e6d7c61b229e9c5a27eced3d9
//
// ed25519: -x**2 + y**2 = 1 + d*x**2*y**2 mod p
// d = 37095705934669439343138083508754565189542113879843219016388785533085940283555
// = 0x52036cee2b6ffe738cc740797779e89800700a4d4141d8ab75eb4dca135978a3
// x = 15112221349535400772501151409588531511454012693041857206046113283949847762202
// = 0x216936d3cd6e53fec0a4e231fdd6dc5c692cc7609525a7b2c9562d608f25d51a
// y = 46316835694926478169428394003475163141307993866256225615783033603165251855960
// = 0x6666666666666666666666666666666666666666666666666666666666666658
//
// sqrt(-486664) = 6853475219497561581579357271197624642482790079785650197046958215289687604742
// = 0xf26edf460a006bbd27b08dc03fc4f7ec5a1d3d14b7d1a82cc6e04aaff457e06
//
// birational map
//
// ed25519 -> cv25519
// u = (1 + y) / (1 - y)
// v = sqrt(-486664) * u / x
//
// cv25519 -> ed25519
// x = sqrt(-486664) * u / v
// y = (u - 1) / (u + 1)

struct _ed25519_limb {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
	x4: int;
	x5: int;
	x6: int;
	x7: int;
}

struct _ed25519_point {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
	x4: int;
	x5: int;
	x6: int;
	x7: int;
	y0: int;
	y1: int;
	y2: int;
	y3: int;
	y4: int;
	y5: int;
	y6: int;
	y7: int;
}

struct _ed25519_mod {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
	x4: int;
	x5: int;
	x6: int;
	x7: int;
	x8: int;
	x9: int;
	x10: int;
	x11: int;
	x12: int;
	x13: int;
	x14: int;
	x15: int;
	x16: int;
}

struct _x25519_block {
	_x0: int;
	_x1: int;
	_x2: int;
	_x3: int;
	_x4: int;
}

struct _ed25519_pub {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
}

struct _ed25519_sig {
	x0: int;
	x1: int;
	x2: int;
	x3: int;
	x4: int;
	x5: int;
	x6: int;
	x7: int;
}

ed25519_reduce(r: *int) {
	var c: int;
	var k: int;

	c = (r[0] + 19) >> 32;
	c = (c + r[1]) >> 32;
	c = (c + r[2]) >> 32;
	c = (c + r[3]) >> 32;
	c = (c + r[4]) >> 32;
	c = (c + r[5]) >> 32;
	c = (c + r[6]) >> 32;
	c = (c + r[7] + (1 << 31)) >> 32;

	k = -c >> 32;

	c = c + r[0] + (18 & k); r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + r[1]; r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + r[2]; r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + r[3]; r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + r[4]; r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + r[5]; r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + r[6]; r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + r[7] + ((1 << 31) & k); r[7] = c & (-1 >> 32); c = c >> 32;
}

ed25519_add(r: *int, a: *int, b: *int) {
	var c: int;

	c = a[0] + b[0]; r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] + b[1]; r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] + b[2]; r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] + b[3]; r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] + b[4]; r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[5] + b[5]; r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[6] + b[6]; r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[7] + b[7]; r[7] = c & (-1 >> 32); c = c >> 32;

	ed25519_reduce(r);
}

ed25519_sub(r: *int, a: *int, b: *int) {
	var c: int;

	c = 1 + a[0] + (-19 & (-1 >> 32)) + (b[0] ^ (-1 >> 32)); r[0] = c & (-1 >> 32); c = c >> 32;
	c = c + a[1] + (-1 >> 32) + (b[1] ^ (-1 >> 32)); r[1] = c & (-1 >> 32); c = c >> 32;
	c = c + a[2] + (-1 >> 32) + (b[2] ^ (-1 >> 32)); r[2] = c & (-1 >> 32); c = c >> 32;
	c = c + a[3] + (-1 >> 32) + (b[3] ^ (-1 >> 32)); r[3] = c & (-1 >> 32); c = c >> 32;
	c = c + a[4] + (-1 >> 32) + (b[4] ^ (-1 >> 32)); r[4] = c & (-1 >> 32); c = c >> 32;
	c = c + a[5] + (-1 >> 32) + (b[5] ^ (-1 >> 32)); r[5] = c & (-1 >> 32); c = c >> 32;
	c = c + a[6] + (-1 >> 32) + (b[6] ^ (-1 >> 32)); r[6] = c & (-1 >> 32); c = c >> 32;
	c = c + a[7] + (-1 >> 33) + (b[7] ^ (-1 >> 32)); r[7] = c & (-1 >> 32); c = c >> 32;

	ed25519_reduce(r);
}

ed25519_mul(r: *int, a: *int, b: *int) {
	var _x: _ed25519_point;
	var x: *int;
	var c: int;
	var i: int;

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
	x[10] = 0;
	x[11] = 0;
	x[12] = 0;
	x[13] = 0;
	x[14] = 0;
	x[15] = 0;

	i = 0;
	loop {
		if i == 8 {
			break;
		}

		c = x[i + 0] + a[i] * b[0]; x[i + 0] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 1] + a[i] * b[1]; x[i + 1] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 2] + a[i] * b[2]; x[i + 2] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 3] + a[i] * b[3]; x[i + 3] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 4] + a[i] * b[4]; x[i + 4] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 5] + a[i] * b[5]; x[i + 5] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 6] + a[i] * b[6]; x[i + 6] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 7] + a[i] * b[7]; x[i + 7] = c & (-1 >> 32); c = c >> 32;
		c = c + x[i + 8]; x[i + 8] = c & (-1 >> 32);

		i = i + 1;
	}

	c = x[0] + x[8] * 38; x[0] = c & (-1 >> 32); c = c >> 32;
	c = c + x[1] + x[9] * 38; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + x[2] + x[10] * 38; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + x[3] + x[11] * 38; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + x[4] + x[12] * 38; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + x[5] + x[13] * 38; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + x[6] + x[14] * 38; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + x[7] + x[15] * 38; x[7] = c & (-1 >> 32); c = c >> 32;

	ed25519_reduce(x);

	c = c * 38 + x[0]; x[0] = c & (-1 >> 32); c = c >> 32;
	c = c + x[1]; x[1] = c & (-1 >> 32); c = c >> 32;
	c = c + x[2]; x[2] = c & (-1 >> 32); c = c >> 32;
	c = c + x[3]; x[3] = c & (-1 >> 32); c = c >> 32;
	c = c + x[4]; x[4] = c & (-1 >> 32); c = c >> 32;
	c = c + x[5]; x[5] = c & (-1 >> 32); c = c >> 32;
	c = c + x[6]; x[6] = c & (-1 >> 32); c = c >> 32;
	c = c + x[7]; x[7] = c & (-1 >> 32);

	ed25519_reduce(x);

	r[0] = x[0];
	r[1] = x[1];
	r[2] = x[2];
	r[3] = x[3];
	r[4] = x[4];
	r[5] = x[5];
	r[6] = x[6];
	r[7] = x[7];
}

ed25519_inv(r: *int, a: *int)  {
	var _x: _ed25519_point;
	var x: *int;
	var i: int;

	x = (&_x):*int;

	i = 0;
	loop {
		if i == 8 {
			break;
		}
		x[i] = a[i]; r[i] = a[i];
		i = i + 1;
	}

	i = 0;
	loop {
		if i == 254 {
			break;
		}
		ed25519_mul(r, r, r);
		if (i != 249 && i != 251) {
			ed25519_mul(r, r, x);
		}
		i = i + 1;
	}
}

ed25519_selectl(r: *int, a: *int, b: *int, k: int) {
	k = -(k & 1);
	r[0] = (a[0] & ~k) | (b[0] & k);
	r[1] = (a[1] & ~k) | (b[1] & k);
	r[2] = (a[2] & ~k) | (b[2] & k);
	r[3] = (a[3] & ~k) | (b[3] & k);
	r[4] = (a[4] & ~k) | (b[4] & k);
	r[5] = (a[5] & ~k) | (b[5] & k);
	r[6] = (a[6] & ~k) | (b[6] & k);
	r[7] = (a[7] & ~k) | (b[7] & k);
}

ed25519_zero(r: *int) {
	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;
	r[4] = 0;
	r[5] = 0;
	r[6] = 0;
	r[7] = 0;
}

ed25519_one(r: *int) {
	r[0] = 1;
	r[1] = 0;
	r[2] = 0;
	r[3] = 0;
	r[4] = 0;
	r[5] = 0;
	r[6] = 0;
	r[7] = 0;
}

ed25519_d(d: *int) {
	d[7] = (0x5203 << 16) | 0x6cee;
	d[6] = (0x2b6f << 16) | 0xfe73;
	d[5] = (0x8cc7 << 16) | 0x4079;
	d[4] = (0x7779 << 16) | 0xe898;
	d[3] = (0x0070 << 16) | 0x0a4d;
	d[2] = (0x4141 << 16) | 0xd8ab;
	d[1] = (0x75eb << 16) | 0x4dca;
	d[0] = (0x1359 << 16) | 0x78a3;
}

ed25519_a(a: *int) {
	a[7] = 0;
	a[6] = 0;
	a[5] = 0;
	a[4] = 0;
	a[3] = 0;
	a[2] = 0;
	a[1] = 0;
	a[0] = 486662;
}

////           x1 * y2 + x2 * y1                y1 * y2 - a * x1 * x2
//// x3 = ---------------------------,  y3 = ---------------------------
////       1 + d * x1 * x2 * y1 * y2          1 - d * x1 * x2 * y1 * y2
ed25519_pa(r: *int, a: *int, b: * int) {
	var _y1y2: _ed25519_limb;
	var y1y2: *int;
	var _x1x2: _ed25519_limb;
	var x1x2: *int;
	var _x1y2: _ed25519_limb;
	var x1y2: *int;
	var _x2y1: _ed25519_limb;
	var x2y1: *int;
	var _dxy: _ed25519_limb;
	var dxy: *int;
	var _dxy1: _ed25519_limb;
	var dxy1: *int;
	var _dxy2: _ed25519_limb;
	var dxy2: *int;
	var _d: _ed25519_limb;
	var d: *int;

	y1y2 = &_y1y2.x0;
	x1x2 = &_x1x2.x0;
	x1y2 = &_x1y2.x0;
	x2y1 = &_x2y1.x0;
	dxy = &_dxy.x0;
	dxy1 = &_dxy1.x0;
	dxy2 = &_dxy2.x0;
	d = &_d.x0;

	ed25519_d(d);

	ed25519_mul(y1y2, &a[8], &b[8]);
	ed25519_mul(x1x2, a, b);

	ed25519_mul(x1y2, a, &b[8]);
	ed25519_mul(x2y1, b, &a[8]);

	ed25519_mul(dxy, x1y2, x2y1);
	ed25519_mul(dxy, d, dxy);

	ed25519_one(dxy1);
	ed25519_add(dxy1, dxy1, dxy);
	ed25519_inv(dxy1, dxy1);

	ed25519_add(r, x1y2, x2y1);
	ed25519_mul(r, r, dxy1);

	ed25519_one(dxy2);
	ed25519_sub(dxy2, dxy2, dxy);
	ed25519_inv(dxy2, dxy2);

	ed25519_add(&r[8], y1y2, x1x2);
	ed25519_mul(&r[8], &r[8], dxy2);
}

ed25519_pk(r: *int, a: *int, k: *int) {
	var _b: _ed25519_point;
	var b: *int;
	var _c: _ed25519_point;
	var c: *int;
	var e: int;
	var i: int;
	var j: int;

	b = &_b.x0;
	c = &_c.x0;

	b[0] = a[0] & (-1 >> 32); r[0] = 0;
	b[1] = a[1] & (-1 >> 32); r[1] = 0;
	b[2] = a[2] & (-1 >> 32); r[2] = 0;
	b[3] = a[3] & (-1 >> 32); r[3] = 0;
	b[4] = a[4] & (-1 >> 32); r[4] = 0;
	b[5] = a[5] & (-1 >> 32); r[5] = 0;
	b[6] = a[6] & (-1 >> 32); r[6] = 0;
	b[7] = a[7] & (-1 >> 32); r[7] = 0;
	b[8] = a[8] & (-1 >> 32); r[8] = 1;
	b[9] = a[9] & (-1 >> 32); r[9] = 0;
	b[10] = a[10] & (-1 >> 32); r[10] = 0;
	b[11] = a[11] & (-1 >> 32); r[11] = 0;
	b[12] = a[12] & (-1 >> 32); r[12] = 0;
	b[13] = a[13] & (-1 >> 32); r[13] = 0;
	b[14] = a[14] & (-1 >> 32); r[14] = 0;
	b[15] = a[15] & (-1 >> 32); r[15] = 0;

	i = 7;
	loop {
		e = k[i];

		j = 0;
		loop {
			if j == 32 {
				break;
			}
			ed25519_pa(r, r, r);
			ed25519_pa(c, r, b);
			ed25519_selectl(r, r, c, -((e >> 31) & 1));
			ed25519_selectl(&r[8], &r[8], &c[8], -((e >> 31) & 1));
			e = e << 1;
			j = j + 1;
		}

		if i == 0 {
			break;
		}

		i = i - 1;
	}
}

ed25519_base(p: *int) {
	p[7] = (0x2169 << 16) | 0x36d3;
	p[6] = (0xcd6e << 16) | 0x53fe;
	p[5] = (0xc0a4 << 16) | 0xe231;
	p[4] = (0xfdd6 << 16) | 0xdc5c;
	p[3] = (0x692c << 16) | 0xc760;
	p[2] = (0x9525 << 16) | 0xa7b2;
	p[1] = (0xc956 << 16) | 0x2d60;
	p[0] = (0x8f25 << 16) | 0xd51a;
	p[15] = (0x6666 << 16) | 0x6666;
	p[14] = (0x6666 << 16) | 0x6666;
	p[13] = (0x6666 << 16) | 0x6666;
	p[12] = (0x6666 << 16) | 0x6666;
	p[11] = (0x6666 << 16) | 0x6666;
	p[10] = (0x6666 << 16) | 0x6666;
	p[9] = (0x6666 << 16) | 0x6666;
	p[8] = (0x6666 << 16) | 0x6658;
}

// 2**((p-1)//4)
ed25519_sqrtz(z: *int) {
	z[7] = (0x2b83 << 16) | 0x2480;
	z[6] = (0x4fc1 << 16) | 0xdf0b;
	z[5] = (0x2b4d << 16) | 0x0099;
	z[4] = (0x3dfb << 16) | 0xd7a7;
	z[3] = (0x2f43 << 16) | 0x1806;
	z[2] = (0xad2f << 16) | 0xe478;
	z[1] = (0xc4ee << 16) | 0x1b27;
	z[0] = (0x4a0e << 16) | 0xa0b0;
}

// sqrt(x) = x**((p+3)/8) * [1 or 2**((p-1)/4)]
ed25519_sqrt(r: *int, x: *int): int {
	var _a: _ed25519_limb;
	var _z: _ed25519_limb;
	var a: *int;
	var z: *int;
	var i: int;

	a = &_a.x0;
	z = &_z.x0;

	ed25519_one(a);

	i = 0;
	loop {
		if i == 252 {
			break;
		}

		ed25519_mul(a, a, a);

		if i != 251 {
			ed25519_mul(a, a, x);
		}

		i = i + 1;
	}

	ed25519_mul(z, a, a);
	ed25519_sub(z, z, x);
	i = ed25519_zerop(z);

	ed25519_sqrtz(z);
	ed25519_mul(z, z, a);

	ed25519_selectl(a, z, a, i);

	ed25519_mul(z, a, a);
	ed25519_sub(z, z, x);

	i = ed25519_zerop(z);

	r[0] = a[0];
	r[1] = a[1];
	r[2] = a[2];
	r[3] = a[3];
	r[4] = a[4];
	r[5] = a[5];
	r[6] = a[6];
	r[7] = a[7];

	return i;
}

// x**2 = (1 - y**2) / (d * y**2 - 1) mod p
ed25519_decode(p: *int, y: *byte): int {
	var _xy: _ed25519_point;
	var xy: *int;
	var _a: _ed25519_limb;
	var a: *int;
	var _b: _ed25519_limb;
	var b: *int;

	xy = &_xy.x0;
	a = &_a.x0;
	b = &_b.x0;

	xy[8] = y[0]:int | (y[1]:int << 8) | (y[2]:int << 16) | (y[3]:int << 24);
	xy[9] = y[4]:int | (y[5]:int << 8) | (y[6]:int << 16) | (y[7]:int << 24);
	xy[10] = y[8]:int | (y[9]:int << 8) | (y[10]:int << 16) | (y[11]:int << 24);
	xy[11] = y[12]:int | (y[13]:int << 8) | (y[14]:int << 16) | (y[15]:int << 24);
	xy[12] = y[16]:int | (y[17]:int << 8) | (y[18]:int << 16) | (y[19]:int << 24);
	xy[13] = y[20]:int | (y[21]:int << 8) | (y[22]:int << 16) | (y[23]:int << 24);
	xy[14] = y[24]:int | (y[25]:int << 8) | (y[26]:int << 16) | (y[27]:int << 24);
	xy[15] = (y[28]:int | (y[29]:int << 8) | (y[30]:int << 16) | (y[31]:int << 24)) & (-1 >> 33);

	ed25519_mul(a, &xy[8], &xy[8]);
	ed25519_d(b);
	ed25519_mul(b, b, a);
	ed25519_one(xy);
	ed25519_sub(b, b, xy);
	ed25519_sub(xy, xy, a);
	ed25519_inv(b, b);
	ed25519_mul(xy, xy, b);

	if !ed25519_sqrt(xy, xy) {
		return 0;
	}

	ed25519_set_lsb(xy, y[31]:int >> 7);

	p[0] = xy[0];
	p[1] = xy[1];
	p[2] = xy[2];
	p[3] = xy[3];
	p[4] = xy[4];
	p[5] = xy[5];
	p[6] = xy[6];
	p[7] = xy[7];
	p[8] = xy[8];
	p[9] = xy[9];
	p[10] = xy[10];
	p[11] = xy[11];
	p[12] = xy[12];
	p[13] = xy[13];
	p[14] = xy[14];
	p[15] = xy[15];

	return 1;
}

ed25519_encode(dest: *byte, p: *int) {
	dest[0] = p[8]:byte;
	dest[1] = (p[8] >> 8):byte;
	dest[2] = (p[8] >> 16):byte;
	dest[3] = (p[8] >> 24):byte;
	dest[4] = p[9]:byte;
	dest[5] = (p[9] >> 8):byte;
	dest[6] = (p[9] >> 16):byte;
	dest[7] = (p[9] >> 24):byte;
	dest[8] = p[10]:byte;
	dest[9] = (p[10] >> 8):byte;
	dest[10] = (p[10] >> 16):byte;
	dest[11] = (p[10] >> 24):byte;
	dest[12] = p[11]:byte;
	dest[13] = (p[11] >> 8):byte;
	dest[14] = (p[11] >> 16):byte;
	dest[15] = (p[11] >> 24):byte;
	dest[16] = p[12]:byte;
	dest[17] = (p[12] >> 8):byte;
	dest[18] = (p[12] >> 16):byte;
	dest[19] = (p[12] >> 24):byte;
	dest[20] = p[13]:byte;
	dest[21] = (p[13] >> 8):byte;
	dest[22] = (p[13] >> 16):byte;
	dest[23] = (p[13] >> 24):byte;
	dest[24] = p[14]:byte;
	dest[25] = (p[14] >> 8):byte;
	dest[26] = (p[14] >> 16):byte;
	dest[27] = (p[14] >> 24):byte;
	dest[28] = p[15]:byte;
	dest[29] = (p[15] >> 8):byte;
	dest[30] = (p[15] >> 16):byte;
	dest[31] = (p[15] >> 24):byte | ((p[0] & 1) << 7):byte;
}

ed25519_pub(pub: *byte, b: *byte) {
	var _h: _sha512_digest;
	var h: *byte;
	var _a: _ed25519_point;
	var a: *int;
	var _s: _ed25519_limb;
	var s: *int;

	h = (&_h):*byte;
	a = &_a.x0;
	s = &_s.x0;

	// h || prefix = SHA-512(b)
	sha512(h, b, 32);

	// s = clamp(h)
	ed25519_clamp(s, h);

	// A = [s]B
	ed25519_base(a);
	ed25519_pk(a, a, s);

	// public key = A
	ed25519_encode(pub, a);
}

ed25519_sign(sig: *byte, b: *byte, msg: *byte, len: int) {
	var _h: _sha512_digest;
	var h: *byte;
	var _hr: _sha512_digest;
	var hr: *byte;
	var _hk: _sha512_digest;
	var hk: *byte;
	var ctx: sha512_ctx;
	var _a: _ed25519_point;
	var a: *int;
	var _pub: _ed25519_point;
	var pub: *byte;
	var _rb: _ed25519_point;
	var rb: *int;
	var _s: _ed25519_limb;
	var s: *int;
	var _r: _ed25519_limb;
	var r: *int;
	var _k: _ed25519_limb;
	var k: *int;

	h = (&_h):*byte;
	hr = (&_hr):*byte;
	hk = (&_hk):*byte;
	a = &_a.x0;
	pub = (&_pub):*byte;
	rb = &_rb.x0;
	s = &_s.x0;
	r = &_r.x0;
	k = &_k.x0;

	// h || prefix = SHA-512(b)
	sha512(h, b, 32);

	// s = clamp(h)
	ed25519_clamp(s, h);

	// A = [s]B
	ed25519_base(a);
	ed25519_pk(a, a, s);
	ed25519_encode(pub, a);

	// r = SHA-512(prefix || M)
	sha512_init(&ctx);
	sha512_update(&ctx, &h[32], 32);
	sha512_update(&ctx, msg, len);
	sha512_final(hr, &ctx);
	ed25519_reduce_l(r, hr);

	// R = [r]B
	ed25519_base(rb);
	ed25519_pk(rb, rb, r);
	ed25519_encode(sig, rb);

	// k = SHA-512(R || A || M)
	sha512_init(&ctx);
	sha512_update(&ctx, sig, 32);
	sha512_update(&ctx, pub, 32);
	sha512_update(&ctx, msg, len);
	sha512_final(hk, &ctx);
	ed25519_reduce_l(k, hk);

	// S = k * s + r mod L
	ed25519_ma_l(s, s, k, r);
	ed25519_encode_l(&sig[32], s);
}

ed25519_l(l: *int) {
	l[7] = (0x1000 << 16) | 0x0000;
	l[6] = (0x0000 << 16) | 0x0000;
	l[5] = (0x0000 << 16) | 0x0000;
	l[4] = (0x0000 << 16) | 0x0000;
	l[3] = (0x14de << 16) | 0xf9de;
	l[2] = (0xa2f7 << 16) | 0x9cd6;
	l[1] = (0x5812 << 16) | 0x631a;
	l[0] = (0x5cf5 << 16) | 0xd3ed;
}

ed25519_mod1(m: *int, l: *int, q: int) {
	var c: int;
	var r: int;

	c = q * l[0]; r = 1 + m[0] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[1]; r = r + m[1] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[2]; r = r + m[2] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[3]; r = r + m[3] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[4]; r = r + m[4] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[5]; r = r + m[5] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[6]; r = r + m[6] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	c = c + q * l[7]; r = r + m[7] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;
	r = r + m[8] + (~c & (-1 >> 32)); c = c >> 32; r = r >> 32;

	q = q - (r ^ 1);

	c = q * l[0]; r = 1 + m[0] + (~c & (-1 >> 32)); m[0] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[1]; r = r + m[1] + (~c & (-1 >> 32)); m[1] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[2]; r = r + m[2] + (~c & (-1 >> 32)); m[2] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[3]; r = r + m[3] + (~c & (-1 >> 32)); m[3] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[4]; r = r + m[4] + (~c & (-1 >> 32)); m[4] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[5]; r = r + m[5] + (~c & (-1 >> 32)); m[5] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[6]; r = r + m[6] + (~c & (-1 >> 32)); m[6] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	c = c + q * l[7]; r = r + m[7] + (~c & (-1 >> 32)); m[7] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
	r = r + m[8] + (~c & (-1 >> 32)); m[8] = r & (-1 >> 32); c = c >> 32; r = r >> 32;
}

// r = x mod L
ed25519_mod_l(r: *int, x: *int) {
	var _l: _ed25519_limb;
	var l: *int;
	var _m: _ed25519_mod;
	var m: *int;

	l = &_l.x0;
	m = &_m.x0;

	ed25519_l(l);

	m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = x[3];
	m[4] = x[4];
	m[5] = x[5];
	m[6] = x[6];
	m[7] = x[7];
	m[8] = x[8];
	m[9] = x[9];
	m[10] = x[10];
	m[11] = x[11];
	m[12] = x[12];
	m[13] = x[13];
	m[14] = x[14];
	m[15] = x[15];
	m[16] = 0;

	ed25519_mod1(&m[8], l, m[15] >> 28);
	ed25519_mod1(&m[7], l, (m[15] << 4) + (m[14] >> 28));
	ed25519_mod1(&m[6], l, (m[14] << 4) + (m[13] >> 28));
	ed25519_mod1(&m[5], l, (m[13] << 4) + (m[12] >> 28));
	ed25519_mod1(&m[4], l, (m[12] << 4) + (m[11] >> 28));
	ed25519_mod1(&m[3], l, (m[11] << 4) + (m[10] >> 28));
	ed25519_mod1(&m[2], l, (m[10] << 4) + (m[9] >> 28));
	ed25519_mod1(&m[1], l, (m[9] << 4) + (m[8] >> 28));
	ed25519_mod1(&m[0], l, (m[8] << 4) + (m[7] >> 28));

	r[0] = m[0];
	r[1] = m[1];
	r[2] = m[2];
	r[3] = m[3];
	r[4] = m[4];
	r[5] = m[5];
	r[6] = m[6];
	r[7] = m[7];
}

// x = a * b + y mod L
ed25519_ma_l(x: *int, a: *int, b: *int, y: *int) {
	var _z: _ed25519_mod;
	var z: *int;
	var c: int;
	var i: int;

	z = (&_z):*int;

	z[0] = 0;
	z[1] = 0;
	z[2] = 0;
	z[3] = 0;
	z[4] = 0;
	z[5] = 0;
	z[6] = 0;
	z[7] = 0;
	z[8] = 0;
	z[9] = 0;
	z[10] = 0;
	z[11] = 0;
	z[12] = 0;
	z[13] = 0;
	z[14] = 0;
	z[15] = 0;

	i = 0;
	loop {
		if i == 8 {
			break;
		}

		c = z[i + 0] + a[i] * b[0]; z[i + 0] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 1] + a[i] * b[1]; z[i + 1] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 2] + a[i] * b[2]; z[i + 2] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 3] + a[i] * b[3]; z[i + 3] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 4] + a[i] * b[4]; z[i + 4] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 5] + a[i] * b[5]; z[i + 5] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 6] + a[i] * b[6]; z[i + 6] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 7] + a[i] * b[7]; z[i + 7] = c & (-1 >> 32); c = c >> 32;
		c = c + z[i + 8]; z[i + 8] = c & (-1 >> 32);

		i = i + 1;
	}

	ed25519_mod_l(z, z);

	c = z[0] + y[0]; z[0] = c; c = c >> 32;
	c = z[1] + y[1]; z[1] = c; c = c >> 32;
	c = z[2] + y[2]; z[2] = c; c = c >> 32;
	c = z[3] + y[3]; z[3] = c; c = c >> 32;
	c = z[4] + y[4]; z[4] = c; c = c >> 32;
	c = z[5] + y[5]; z[5] = c; c = c >> 32;
	c = z[6] + y[6]; z[6] = c; c = c >> 32;
	c = z[7] + y[7]; z[7] = c; c = c >> 32;
	z[8] = 0;
	z[9] = 0;
	z[10] = 0;
	z[11] = 0;
	z[12] = 0;
	z[13] = 0;
	z[14] = 0;
	z[15] = 0;

	ed25519_mod_l(x, z);
}

// x = a_512 mod L
ed25519_reduce_l(x: *int, a: *byte) {
	var _z: _ed25519_point;
	var z: *int;

	z = (&_z):*int;

	z[0] = a[0]:int | (a[1]:int << 8) | (a[2]:int << 16) | (a[3]:int << 24);
	z[1] = a[4]:int | (a[5]:int << 8) | (a[6]:int << 16) | (a[7]:int << 24);
	z[2] = a[8]:int | (a[9]:int << 8) | (a[10]:int << 16) | (a[11]:int << 24);
	z[3] = a[12]:int | (a[13]:int << 8) | (a[14]:int << 16) | (a[15]:int << 24);
	z[4] = a[16]:int | (a[17]:int << 8) | (a[18]:int << 16) | (a[19]:int << 24);
	z[5] = a[20]:int | (a[21]:int << 8) | (a[22]:int << 16) | (a[23]:int << 24);
	z[6] = a[24]:int | (a[25]:int << 8) | (a[26]:int << 16) | (a[27]:int << 24);
	z[7] = a[28]:int | (a[29]:int << 8) | (a[30]:int << 16) | (a[31]:int << 24);
	z[8] = a[32]:int | (a[33]:int << 8) | (a[34]:int << 16) | (a[35]:int << 24);
	z[9] = a[36]:int | (a[37]:int << 8) | (a[38]:int << 16) | (a[39]:int << 24);
	z[10] = a[40]:int | (a[41]:int << 8) | (a[42]:int << 16) | (a[43]:int << 24);
	z[11] = a[44]:int | (a[45]:int << 8) | (a[46]:int << 16) | (a[47]:int << 24);
	z[12] = a[48]:int | (a[49]:int << 8) | (a[50]:int << 16) | (a[51]:int << 24);
	z[13] = a[52]:int | (a[53]:int << 8) | (a[54]:int << 16) | (a[55]:int << 24);
	z[14] = a[56]:int | (a[57]:int << 8) | (a[58]:int << 16) | (a[59]:int << 24);
	z[15] = a[60]:int | (a[61]:int << 8) | (a[62]:int << 16) | (a[63]:int << 24);

	ed25519_mod_l(x, z);
}

ed25519_encode_l(dest: *byte, p: *int) {
	dest[0] = p[0]:byte;
	dest[1] = (p[0] >> 8):byte;
	dest[2] = (p[0] >> 16):byte;
	dest[3] = (p[0] >> 24):byte;
	dest[4] = p[1]:byte;
	dest[5] = (p[1] >> 8):byte;
	dest[6] = (p[1] >> 16):byte;
	dest[7] = (p[1] >> 24):byte;
	dest[8] = p[2]:byte;
	dest[9] = (p[2] >> 8):byte;
	dest[10] = (p[2] >> 16):byte;
	dest[11] = (p[2] >> 24):byte;
	dest[12] = p[3]:byte;
	dest[13] = (p[3] >> 8):byte;
	dest[14] = (p[3] >> 16):byte;
	dest[15] = (p[3] >> 24):byte;
	dest[16] = p[4]:byte;
	dest[17] = (p[4] >> 8):byte;
	dest[18] = (p[4] >> 16):byte;
	dest[19] = (p[4] >> 24):byte;
	dest[20] = p[5]:byte;
	dest[21] = (p[5] >> 8):byte;
	dest[22] = (p[5] >> 16):byte;
	dest[23] = (p[5] >> 24):byte;
	dest[24] = p[6]:byte;
	dest[25] = (p[6] >> 8):byte;
	dest[26] = (p[6] >> 16):byte;
	dest[27] = (p[6] >> 24):byte;
	dest[28] = p[7]:byte;
	dest[29] = (p[7] >> 8):byte;
	dest[30] = (p[7] >> 16):byte;
	dest[31] = (p[7] >> 24):byte;
}

ed25519_eq(a: *int, b: *int): int {
	var x: int;
	x = a[0] ^ b[0];
	x = x | (a[1] ^ b[1]);
	x = x | (a[2] ^ b[2]);
	x = x | (a[3] ^ b[3]);
	x = x | (a[4] ^ b[4]);
	x = x | (a[5] ^ b[5]);
	x = x | (a[6] ^ b[6]);
	x = x | (a[7] ^ b[7]);
	x = x | (a[8] ^ b[8]);
	x = x | (a[9] ^ b[9]);
	x = x | (a[10] ^ b[10]);
	x = x | (a[11] ^ b[11]);
	x = x | (a[12] ^ b[12]);
	x = x | (a[13] ^ b[13]);
	x = x | (a[14] ^ b[14]);
	x = x | (a[15] ^ b[15]);
	x = (x >> 32) | x;
	x = (x >> 16) | x;
	x = (x >> 8) | x;
	x = (x >> 4) | x;
	x = (x >> 2) | x;
	x = (x >> 1) | x;
	return (x & 1) ^ 1;
}

ed25519_verify(sig: *byte, pub: *byte, msg: *byte, len: int): int {
	var ctx: sha512_ctx;
	var a: *int;
	var b: *int;
	var r: *int;
	var s: *int;
	var k: *int;
	var hk: *byte;

	// A = public key
	ed25519_decode(a, pub);

	// sig = R || S
	ed25519_decode(r, sig);
	ed25519_reduce_l(s, &sig[32]);

	// k' = SHA-512(R || A || M)
	sha512_init(&ctx);
	sha512_update(&ctx, sig, 32);
	sha512_update(&ctx, pub, 32);
	sha512_update(&ctx, msg, len);
	sha512_final(hk, &ctx);
	ed25519_reduce_l(k, hk);

	// RHS = R + [k']A
	ed25519_pk(a, a, k);
	ed25519_pa(a, a, r);

	// LHS = [S]B
	ed25519_base(b);
	ed25519_pk(b, b, s);

	return ed25519_eq(a, b);
}

ed25519_bi(d: *int) {
	d[7] = (0x0f26 << 16) | 0xedf4;
	d[6] = (0x60a0 << 16) | 0x06bb;
	d[5] = (0xd27b << 16) | 0x08dc;
	d[4] = (0x03fc << 16) | 0x4f7e;
	d[3] = (0xc5a1 << 16) | 0xd3d1;
	d[2] = (0x4b7d << 16) | 0x1a82;
	d[1] = (0xcc6e << 16) | 0x04aa;
	d[0] = (0xff45 << 16) | 0x7e06;
}

// u = (1 + y) / (1 - y)
// v = sqrt(-486664) * u / x
cv25519_of_ed25519(uv: *int, xy: *int) {
	var _a: _ed25519_limb;
	var _b: _ed25519_limb;
	var _c: _ed25519_limb;
	var _d: _ed25519_limb;
	var a: *int;
	var b: *int;
	var c: *int;
	var d: *int;

	a = &_a.x0;
	b = &_b.x0;
	c = &_c.x0;
	d = &_d.x0;

	ed25519_one(a);
	ed25519_add(a, a, &xy[8]);
	ed25519_one(b);
	ed25519_sub(b, b, &xy[8]);
	ed25519_inv(b, b);

	ed25519_inv(d, xy);

	ed25519_mul(uv, a, b);

	ed25519_bi(c);
	ed25519_mul(c, c, uv);
	ed25519_mul(&uv[8], c, d);
}

// x = sqrt(-486664) * u / v
// y = (u - 1) / (u + 1)
ed25519_of_cv25519(xy: *int, uv: *int) {
	var _a: _ed25519_limb;
	var _b: _ed25519_limb;
	var _c: _ed25519_limb;
	var _d: _ed25519_limb;
	var a: *int;
	var b: *int;
	var c: *int;
	var d: *int;

	a = &_a.x0;
	b = &_b.x0;
	c = &_c.x0;
	d = &_d.x0;

	ed25519_bi(a);
	ed25519_mul(a, a, uv);
	ed25519_inv(b, &uv[8]);

	ed25519_one(c);
	ed25519_sub(c, uv, c);
	ed25519_one(d);
	ed25519_add(d, uv, d);
	ed25519_inv(d, d);

	ed25519_mul(xy, a, b);
	ed25519_mul(&xy[8], c, d);
}

// cv25519: v**2 = u**3 + a*u**2 + u mod p
x25519_decode(uv: *int, u: *byte): int {
	var _v: _ed25519_limb;
	var v: *int;

	v = &_v.x0;

	uv[0] = u[0]:int | (u[1]:int << 8) | (u[2]:int << 16) | (u[3]:int << 24);
	uv[1] = u[4]:int | (u[5]:int << 8) | (u[6]:int << 16) | (u[7]:int << 24);
	uv[2] = u[8]:int | (u[9]:int << 8) | (u[10]:int << 16) | (u[11]:int << 24);
	uv[3] = u[12]:int | (u[13]:int << 8) | (u[14]:int << 16) | (u[15]:int << 24);
	uv[4] = u[16]:int | (u[17]:int << 8) | (u[18]:int << 16) | (u[19]:int << 24);
	uv[5] = u[20]:int | (u[21]:int << 8) | (u[22]:int << 16) | (u[23]:int << 24);
	uv[6] = u[24]:int | (u[25]:int << 8) | (u[26]:int << 16) | (u[27]:int << 24);
	uv[7] = (u[28]:int | (u[29]:int << 8) | (u[30]:int << 16) | (u[31]:int << 24)) & (-1 >> 33);

	ed25519_reduce(uv);

	ed25519_a(v);
	ed25519_add(v, v, uv);
	ed25519_mul(v, v, uv);
	ed25519_mul(v, v, uv);
	ed25519_add(v, v, uv);

	if !ed25519_sqrt(&uv[8], v) {
		return 0;
	}

	return !ed25519_zerop(&uv[8]);
}

x25519_encode(u: *byte, uv: *int) {
	u[0] = uv[0]:byte;
	u[1] = (uv[0] >> 8):byte;
	u[2] = (uv[0] >> 16):byte;
	u[3] = (uv[0] >> 24):byte;
	u[4] = uv[1]:byte;
	u[5] = (uv[1] >> 8):byte;
	u[6] = (uv[1] >> 16):byte;
	u[7] = (uv[1] >> 24):byte;
	u[8] = uv[2]:byte;
	u[9] = (uv[2] >> 8):byte;
	u[10] = (uv[2] >> 16):byte;
	u[11] = (uv[2] >> 24):byte;
	u[12] = uv[3]:byte;
	u[13] = (uv[3] >> 8):byte;
	u[14] = (uv[3] >> 16):byte;
	u[15] = (uv[3] >> 24):byte;
	u[16] = uv[4]:byte;
	u[17] = (uv[4] >> 8):byte;
	u[18] = (uv[4] >> 16):byte;
	u[19] = (uv[4] >> 24):byte;
	u[20] = uv[5]:byte;
	u[21] = (uv[5] >> 8):byte;
	u[22] = (uv[5] >> 16):byte;
	u[23] = (uv[5] >> 24):byte;
	u[24] = uv[6]:byte;
	u[25] = (uv[6] >> 8):byte;
	u[26] = (uv[6] >> 16):byte;
	u[27] = (uv[6] >> 24):byte;
	u[28] = uv[7]:byte;
	u[29] = (uv[7] >> 8):byte;
	u[30] = (uv[7] >> 16):byte;
	u[31] = (uv[7] >> 24):byte;
}

x25519_base(u: *byte) {
	bzero(u, 32);
	u[0] = 9:byte;
}

x25519(uk: *byte, u: *byte, k: *byte): int {
	var _uv: _ed25519_point;
	var _xy: _ed25519_point;
	var _kc: _ed25519_limb;
	var uv: *int;
	var xy: *int;
	var kc: *int;

	uv = &_uv.x0;
	xy = &_xy.x0;
	kc = &_kc.x0;

	if !x25519_decode(uv, u) {
		return 0;
	}

	ed25519_of_cv25519(xy, uv);
	ed25519_clamp(kc, k);
	ed25519_pk(xy, xy, kc);
	cv25519_of_ed25519(uv, xy);
	x25519_encode(uk, uv);
	return 1;
}

ed25519_zerop(x: *int): int {
	var a: int;
	a = x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7];
	a = (a >> 32) | a;
	a = (a >> 16) | a;
	a = (a >> 8) | a;
	a = (a >> 4) | a;
	a = (a >> 2) | a;
	a = (a >> 1) | a;
	return (a & 1) ^ 1;
}

// cv25519: v**2 = u**3 + a*u**2 + u mod p
x25519_check(uv: *int): int {
	var _a: _ed25519_limb;
	var _b: _ed25519_limb;
	var a: *int;
	var b: *int;

	a = &_a.x0;
	b = &_b.x0;

	ed25519_a(a);
	ed25519_mul(b, uv, uv);
	ed25519_mul(a, a, b);
	ed25519_mul(b, b, uv);
	ed25519_add(a, a, b);
	ed25519_add(a, a, uv);

	ed25519_mul(b, &uv[8], &uv[8]);
	ed25519_sub(a, a, b);

	return ed25519_zerop(a);
}

// ed25519: -x**2 + y**2 = 1 + d*x**2*y**2 mod p
ed25519_check(xy: *int): int {
	var _a: _ed25519_limb;
	var _b: _ed25519_limb;
	var _c: _ed25519_limb;
	var a: *int;
	var b: *int;
	var c: *int;

	a = &_a.x0;
	b = &_b.x0;
	c = &_c.x0;

	ed25519_mul(b, xy, xy);
	ed25519_mul(c, &xy[8], &xy[8]);

	ed25519_one(a);
	ed25519_add(a, a, b);
	ed25519_sub(a, a, c);

	ed25519_mul(b, b, c);
	ed25519_d(c);
	ed25519_mul(b, b, c);

	ed25519_add(a, a, b);

	return ed25519_zerop(a);
}

ed25519_set_lsb(xy: *int, lsb: int) {
	var _a: _ed25519_limb;
	var a: *int;

	a = &_a.x0;

	ed25519_zero(a);
	ed25519_sub(a, a, &xy[8]);

	ed25519_selectl(&xy[8], &xy[8], a, (xy[0] ^ lsb) & 1);
}

ed25519_clamp(k: *int, b: *byte) {
	k[0] = (b[0]:int | (b[1]:int << 8) | (b[2]:int << 16) | (b[3]:int << 24)) & -8;
	k[1] = b[4]:int | (b[5]:int << 8) | (b[6]:int << 16) | (b[7]:int << 24);
	k[2] = b[8]:int | (b[9]:int << 8) | (b[10]:int << 16) | (b[11]:int << 24);
	k[3] = b[12]:int | (b[13]:int << 8) | (b[14]:int << 16) | (b[15]:int << 24);
	k[4] = b[16]:int | (b[17]:int << 8) | (b[18]:int << 16) | (b[19]:int << 24);
	k[5] = b[20]:int | (b[21]:int << 8) | (b[22]:int << 16) | (b[23]:int << 24);
	k[6] = b[24]:int | (b[25]:int << 8) | (b[26]:int << 16) | (b[27]:int << 24);
	k[7] = ((b[28]:int | (b[29]:int << 8) | (b[30]:int << 16) | (b[31]:int << 24)) & (-1 >> 33)) | (1 << 30);
}
