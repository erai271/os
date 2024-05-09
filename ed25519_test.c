main(c: int, v: **byte, e: **byte) {
	var _r: _ed25519_point;
	var _p: _ed25519_point;
	var _k: _ed25519_point;
	var r: *int;
	var p: *int;
	var k: *int;

	r = (&_r):*int;
	p = (&_p):*int;
	k = (&_k):*int;

	bzero(r: *byte, sizeof(_r));
	bzero(p: *byte, sizeof(_p));
	bzero(k: *byte, sizeof(_k));

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

	k[7] = (0x1000 << 16) | 0x0000;
	k[6] = 0;
	k[5] = 0;
	k[4] = 0;
	k[3] = (0x14de << 16) | 0xf9de;
	k[2] = (0xa2f7 << 16) | 0x9cd6;
	k[1] = (0x5812 << 16) | 0x631a;
	k[0] = (0x5cf5 << 16) | 0xd3ed;

	ed25519_pk(r, p, k);

	fdputh32(1, r[7]);
	fdputh32(1, r[6]);
	fdputh32(1, r[5]);
	fdputh32(1, r[4]);
	fdputh32(1, r[3]);
	fdputh32(1, r[2]);
	fdputh32(1, r[1]);
	fdputh32(1, r[0]);
	fdputc(1, '\n');

	fdputh32(1, r[15]);
	fdputh32(1, r[14]);
	fdputh32(1, r[13]);
	fdputh32(1, r[12]);
	fdputh32(1, r[11]);
	fdputh32(1, r[10]);
	fdputh32(1, r[9]);
	fdputh32(1, r[8]);
	fdputc(1, '\n');
}
