doxxd(fd: int, buf: *byte) {
	var n: int;
	var m: int;
	var ret: int;

	m = 0;
	loop {
		n = 0;
		loop {
			ret = read(fd, buf, 16 - n);
			if ret == 0 {
				break;
			}

			if ret < 0 {
				die("read failed");
			}

			n = n + ret;

			if n == 16 {
				break;
			}
		}

		xxd_line(&buf[16], m, buf, n);
		fdputs(1, &buf[16]);

		if n < 16 {
			break;
		}

		m = m + n;
	}
}

main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var i: int;
	var a: alloc;
	var buf: *byte;

	setup_alloc(&a);

	buf = alloc(&a, 4096);

	if argc == 1 {
		doxxd(0, buf);
	} else if argc == 2 {
		fd = open(argv[1], 0, 0);
		doxxd(fd, buf);
		close(fd);
	} else {
		die("usage: xxd [file]");
	}
}
