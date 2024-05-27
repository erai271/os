docat(fd: int, buf: *byte) {
	var n: int;
	var m: int;
	var k: int;
	loop {
		n = read(fd, buf, 4096);
		if n == 0 {
			return;
		}

		if n < 0 {
			die("read failed");
		}

		m = 0;
		loop {
			if m == n {
				break;
			}

			k = write(1, &buf[m], n - m);
			if k < 0 {
				die("write failed");
			}

			m = m + k;
		}
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
		docat(0, buf);
	} else {
		i = 1;
		loop {
			if i >= argc {
				break;
			}

			fd = open(argv[i], 0, 0);

			docat(fd, buf);

			close(fd);

			i = i + 1;
		}
	}
}
