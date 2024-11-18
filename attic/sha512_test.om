main(argc: int, argv: **byte, envp: **byte) {
	var d: _sha512_digest;
	var digest: *byte;
	digest = (&d):*byte;
	if argc == 2 {
		sha512(digest, argv[1], strlen(argv[1]));
		fdxxd(1, digest, 64);
	} else if argc == 3 {
		sha512_hmac(digest, argv[1], strlen(argv[1]), argv[2], strlen(argv[2]));
		fdxxd(1, digest, 64);
	} else {
		exit(2);
	}
}
