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

fdputhn(fd: int, x: int, d: int) {
	loop {
		if d == 0 {
			break;
		}
		d = d - 4;
		fdputc(fd, "0123456789abcdef"[(x >> d) & 15]:int);
	}
}

fdputh8(fd: int, x: int) {
	fdputhn(fd, x, 8);
}

fdputh16(fd: int, x: int) {
	fdputhn(fd, x, 16);
}

fdputh32(fd: int, x: int) {
	fdputhn(fd, x, 32);
}

fdputh64(fd: int, x: int) {
	fdputhn(fd, x, 64);
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

fdxxd(fd: int, data: *byte, len: int) {
	var i: int;
	var j: int;
	loop {
		if i >= len {
			break;
		}

		fdputh32(fd, i);

		fdputc(fd, ':');
		fdputc(fd, ' ');

		j = 0;

		loop {
			if j == 16 {
				break;
			}

			if i + j < len {
				fdputh8(fd, data[i + j]: int);
			} else {
				fdputc(fd, ' ');
				fdputc(fd, ' ');
			}

			if i + j + 1 < len {
				fdputh8(fd, data[i + j + 1]: int);
			} else {
				fdputc(fd, ' ');
				fdputc(fd, ' ');
			}

			fdputc(fd, ' ');

			j = j + 2;
		}

		fdputc(fd, ' ');

		j = 0;
		loop {
			if j == 16 || i + j >= len {
				break;
			}

			if data[i + j]:int >= 0x20 && data[i + j]:int < 0x80 {
				fdputc(fd, data[i + j]: int);
			} else {
				fdputc(fd, '.');
			}

			j = j + 1;
		}

		fdputc(fd, '\n');

		i = i + 16;
	}
}

bzero(s: *byte, size: int) {
	var i: int;
	i = 0;
	loop {
		if i == size {
			break;
		}
		s[i] = 0:byte;
		i = i + 1;
	}
}

memset(dest: *byte, c: int, size: int) {
	var i: int;

	if size < 0 {
		return;
	}

	i = 0;
	loop {
		if i == size {
			break;
		}
		dest[i] = c:byte;
		i = i + 1;
	}
}

memcpy(dest: *byte, src: *byte, size: int) {
	var i: int;

	if size < 0 {
		return;
	}

	if src > dest {
		i = 0;
		loop {
			if i == size {
				break;
			}
			dest[i] = src[i];
			i = i + 1;
		}
	} else if src < dest {
		i = size;
		loop {
			if i == 0 {
				break;
			}
			i = i - 1;
			dest[i] = src[i];
		}
	}
}

unhex(dest: *byte, src: *byte): int {
	var i: int;
	var dlen: int;
	var x: int;
	var y: int;

	i = 0;
	dlen = 0;
	loop {
		x = src[i]:int;

		if x == 0 {
			break;
		}

		if x == ' ' || x == '\t' || x == '\r' || x == '\n' {
			i = i + 1;
			continue;
		}

		if x >= '0' && x <= '9' {
			x = x - '0';
		} else if x >= 'a' && x <= 'f' {
			x = x - 'a' + 10;
		} else if x >= 'A' && x <= 'F' {
			x = x - 'A' + 10;
		} else {
			i = i + 1;
			continue;
		}

		y = src[i + 1]:int;

		if y == 0 {
			break;
		}

		if y >= '0' && y <= '9' {
			y = (y - '0');
		} else if y >= 'a' && y <= 'f' {
			y = (y - 'a' + 10);
		} else if y >= 'A' && y <= 'F' {
			y = (y - 'A' + 10);
		} else {
			i = i + 1;
			continue;
		}

		dest[dlen] = ((x << 4) | y):byte;
		dlen = dlen + 1;
		i = i + 2;
	}

	return dlen;
}

reverse(buf: *byte, len: int) {
	var i: int;
	var x: byte;
	i = 0;
	len = len - 1;
	loop {
		if i >= len {
			break;
		}

		x = buf[i];
		buf[i] = buf[len];
		buf[len] = x;

		i = i + 1;
		len = len - 1;
	}
}

assert(x: int, msg: *byte) {
	if !x {
		die(msg);
	}
}
