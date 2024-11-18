main(argc: int, argv: **byte, envp: **byte) {
	var _a: _ed25519_point;
	var a: *byte;
	var _b: _ed25519_point;
	var b: *byte;
	var _c: _ed25519_point;
	var c: *byte;

	a = (&_a):*byte;
	b = (&_b):*byte;
	c = (&_c):*byte;

	assert(unhex(a, "9d61b19deffd5a60ba844af492ec2cc44449c5697b326919703bac031cae7f60") == 32, "unhex");
	ed25519_sign(c, a, "", 0);
	fdxxd(1, c, 64);
	fdputc(1, '\n');

	assert(unhex(a, "4ccd089b28ff96da9db6c346ec114e0f5b8a319f35aba624da8cf6ed4fb8a6fb") == 32, "unhex");
	ed25519_sign(c, a, "r", 1);
	fdxxd(1, c, 64);
	fdputc(1, '\n');
}
