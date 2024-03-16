die(msg: *byte) {
	fdputs(2, msg);
	fdputs(2, "\n");
	exit(1);
}

strlen(s: *byte): int {
	var ret: int;
	ret = 0;
	loop {
		if (s[ret] == 0:byte) {
			return ret;
		}
		ret = ret + 1;
	}
}

strcmp(a: *byte, b: *byte): int {
	var i: int;

	i = 0;

	loop {
		if (a[i] > b[i]) {
			return 1;
		}

		if (a[i] < b[i]) {
			return -1;
		}

		if (a[i] == 0:byte) {
			return 0;
		}

		i = i + 1;
	}
}

fdputc(fd: int, ch: int) {
	var b: byte;
	var ret: int;
	b = ch: byte;
	ret = write(fd, &b, 1);
	if (ret != 1) {
		exit(3);
	}
}

fdputs(fd: int, msg: *byte) {
	var len: int;
	var ret: int;
	var off: int;
	len = strlen(msg);
	off = 0;
	loop {
		if (off == len) {
			break;
		}
		ret = write(fd, msg, len - off);
		if (ret < 0) {
			exit(3);
		}
		off = off + ret;
	}
}

fdputh(fd: int, n: int) {
	var c: int;
	var r: int;

	r = 0;
	loop {
		if (n == 0) {
			break;
		}

		r = (r << 4) + (n & 15);
		n = n >> 4;
	}

	n = r;

	loop {
		c = n & 15;
		n = n >> 4;

		if (c < 10) {
			fdputc(fd, c + '0');
		} else {
			fdputc(fd, c + ('a' - 10));
		}

		if (n == 0) {
			break;
		}
	}
}

fdputd(fd: int, n: int) {
	var a: int;

	if (n < 0) {
		fdputc(fd, '-');
		a = -(n % 10);
		n = n / -10;
	} else {
		a = n % 10;
		n = n / 10;
	}

	if (n != 0) {
		fdputd(fd, n);
	}

	fdputc(fd, '0' + a);
}
