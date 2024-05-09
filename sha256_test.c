main(argc: int, argv: **byte, envp: **byte) {
	var d: _sha256_digest;
	var digest: *byte;
	digest = (&d):*byte;
	if argc == 2 {
		sha256(digest, argv[1], strlen(argv[1]));
		fdxxd(1, digest, 32);
	} else if argc == 3 {
		sha256_hmac(digest, argv[1], strlen(argv[1]), argv[2], strlen(argv[2]));
		fdxxd(1, digest, 32);
	} else {
		exit(2);
	}
}
