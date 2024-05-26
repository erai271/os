main(argc: int, argv: **byte, envp: **byte) {
	var i: int;
	i = 1;
	loop {
		if i >= argc {
			return;
		}

		unlink(argv[i]);

		i = i + 1;
	}
}
