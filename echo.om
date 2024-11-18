main(argc: int, argv: **byte, envp: **byte) {
	var i: int;
	i = 1;
	loop {
		if i >= argc {
			fdputc(1, '\n');
			return;
		}

		fdputs(1, argv[i]);

		i = i + 1;

		if i < argc {
			fdputc(1, ' ');
		}
	}
}
