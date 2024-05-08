main(argc: int, argv: **byte, envp: **byte) {
	var d: _sha256_digest;
	var digest: *byte;
	digest = (&d):*byte;
	sha256(digest, argv[1], strlen(argv[1]));
	fdxxd(1, digest, 32);
}
