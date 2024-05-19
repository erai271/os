main(argc: int, argv: **byte, envp: **byte) {
	var _a: _ed25519_point;
	var a: *byte;
	var _b: _ed25519_point;
	var b: *byte;
	var _c: _ed25519_point;
	var c: *byte;
	var ret: int;

	a = (&_a):*byte;
	b = (&_b):*byte;
	c = (&_c):*byte;

	assert(unhex(a, "e6db6867583030db3594c1a424b15f7c726624ec26b3353b10a903a6d0ab1c4c") == 32, "unhex");
	assert(unhex(b, "a546e36bf0527c9d3b16154b82465edd62144c0ac1fc5a18506a2244ba449ac4") == 32, "unhex");
	assert(x25519(c, a, b), "decode");
	fdxxd(1, c, 32);

	x25519_base(a);
	assert(unhex(b, "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a") == 32, "unhex");
	assert(x25519(c, a, b), "decode");
	fdxxd(1, c, 32);

	x25519_base(a);
	assert(unhex(b, "5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb") == 32, "unhex");
	assert(x25519(c, a, b), "decode");
	fdxxd(1, c, 32);

	x25519_base(a);
	assert(unhex(b, "5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb") == 32, "unhex");
	assert(x25519(c, a, b), "decode");
	assert(unhex(b, "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a") == 32, "unhex");
	assert(x25519(c, c, b), "decode");
	fdxxd(1, c, 32);
}
