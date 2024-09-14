struct file {
	a: *alloc;
	fd: int;
	buf: *byte;
	r: int;
	w: int;
	cap: int;
	eof: int;
}

fopen(fd: int, a: *alloc): *file {
	var f: *file;

	f = alloc(a, sizeof(*f)): *file;

	f.fd = fd;
	f.a = a;
	f.r = 0;
	f.w = 0;
	f.cap = 4096;
	f.eof = 0;

	f.buf = alloc(a, f.cap);

	return f;
}

fclose(f: *file): void {
	if (close(f.fd) != 0) {
		die("write failed");
	}

	free(f.a, f.buf);

	free(f.a, f: *byte);
}

fflush(f: *file): void {
	var ret: int;

	loop {
		if (f.r == f.w) {
			f.r = 0;
			f.w = 0;
			return;
		}

		ret = write(f.fd, &f.buf[f.r], f.w - f.r);

		if (ret < 0) {
			die("write failed");
		}

		f.r = f.r + ret;
	}
}

ffill(f: *file): void {
	var ret: int;

	if (f.eof) {
		return;
	}

	if (f.r == f.w) {
		f.r = 0;
		f.w = 0;
	}

	if (f.w == f.cap) {
		die("out of space");
	}

	ret = read(f.fd, &f.buf[f.w], f.cap - f.w);

	if (ret < 0) {
		die("read failed");
	}

	if (ret == 0) {
		f.eof = 1;
	}

	f.w = f.w + ret;
}

fputc(f: *file, ch: int): void {
	if (f.w == f.cap) {
		fflush(f);
	}

	f.buf[f.w] = ch: byte;
	f.w = f.w + 1;

	if (ch == '\n') {
		fflush(f);
	}
}

fgetc(f: *file): int {
	var ch: int;

	if (f.r == f.w) {
		ffill(f);
	}

	if (f.eof) {
		return -1;
	}

	ch = f.buf[f.r]: int;

	f.r = f.r + 1;

	return ch;
}

fgets(f: *file, buf: *byte, len: int): int {
	var i: int;
	var c: int;

	if len == 1 {
		buf[0] = 0:byte;
		return 0;
	}

	i = 0;
	loop {
		if i + 1 == len {
			buf[i] = 0:byte;
			return i;
		}

		c = fgetc(f);
		if c == -1 || c == '\n' {
			buf[i] = 0:byte;
			return i;
		}

		buf[i] = c:byte;
		i = i + 1;
	}
}

fputs(f: *file, s: *byte) {
	var i: int;
	i = 0;
	loop {
		if !s[i] {
			break;
		}
		fputc(f, s[i]:int);
		i = i + 1;
	}
}

fputb(f: *file, s: *byte, n: int) {
	var i: int;
	i = 0;
	loop {
		if i >= n {
			break;
		}
		fputc(f, s[i]:int);
		i = i + 1;
	}
}

fseek(f: *file, off: int) {
	f.r = 0;
	f.w = 0;
	f.eof = 0;
	if lseek(f.fd, off, 0) != off {
		die("invalid seek");
	}
}

freadall(f: *file, size: *int): *byte {
	var i: int;
	var cap: int;
	var ret: *byte;
	var tmp: *byte;
	var ch: int;

	i = 0;
	cap = 0;
	loop {
		ch = fgetc(f);
		if ch == -1 {
			*size = i;
			return ret;
		}

		if i == cap {
			if cap == 0 {
				cap = 4096;
				ret = alloc(f.a, cap);
			} else {
				cap = cap * 2;
				tmp = alloc(f.a, cap);
				memcpy(tmp, ret, i);
				free(f.a, ret);
				ret = tmp;
			}
		}

		ret[i] = ch:byte;
		i = i + 1;
	}
}
