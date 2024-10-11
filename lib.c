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

memcmp(a: *byte, b: *byte, n: int): int {
	var i: int;

	i = 0;

	loop {
		if i == n {
			return 0;
		}

		if (a[i] > b[i]) {
			return 1;
		}

		if (a[i] < b[i]) {
			return -1;
		}

		i = i + 1;
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

fdgetc(fd: int): int {
	var b: byte;
	var ret: int;
	ret = read(fd, &b, 1);
	if (ret == 1) {
		return b:int;
	} else if (ret == 0) {
		return -1;
	} else {
		exit(3);
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
	var d: int;

	d = n & 15;

	n = n >> 4;

	if n {
		fdputh(fd, n);
	}

	fdputc(fd, "0123456789abcdef"[d]:int);
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

xxd_line(line: *byte, offset: int, data: *byte, len: int) {
	var i: int;
	var j: int;
	var d: *byte;
	d = "0123456789abcdef";

	i = 0;

	line[i] = d[(offset >> 28) & 15];
	line[i + 1] = d[(offset >> 24) & 15];
	line[i + 2] = d[(offset >> 20) & 15];
	line[i + 3] = d[(offset >> 16) & 15];
	line[i + 4] = d[(offset >> 12) & 15];
	line[i + 5] = d[(offset >> 8) & 15];
	line[i + 6] = d[(offset >> 4) & 15];
	line[i + 7] = d[offset & 15];
	line[i + 8] = ':':byte;
	line[i + 9] = ' ':byte;

	i = i + 10;

	j = 0;
	loop {
		if j == 16 {
			break;
		}

		if j < len {
			line[i] = d[(data[j]:int >> 4) & 15];
			line[i + 1] = d[data[j]:int & 15];
		} else {
			line[i] = ' ':byte;
			line[i + 1] = ' ':byte;
		}

		if j + 1 < len {
			line[i + 2] = d[(data[j + 1]:int >> 4) & 15];
			line[i + 3] = d[data[j + 1]:int & 15];
		} else {
			line[i + 2] = ' ':byte;
			line[i + 3] = ' ':byte;
		}

		line[i + 4] = ' ':byte;

		j = j + 2;
		i = i + 5;
	}

	line[i] = ' ':byte;
	i = i + 1;

	j = 0;
	loop {
		if j == 16 || j >= len {
			break;
		}

		if data[j]:int >= 0x20 && data[j]:int < 0x80 {
			line[i] = data[j];
		} else {
			line[i] = '.':byte;
		}

		j = j + 1;
		i = i + 1;
	}

	line[i] = '\n':byte;
	i = i + 1;

	line[i] = 0:byte;
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

readall(fd: int, len: *int, a: *alloc): *byte {
	var buf: *byte;
	var tmp: *byte;
	var cap: int;
	var newcap: int;
	var ret: int;
	var n: int;

	cap = 0;
	n = 0;

	loop {
		if n == cap {
			if cap == 0 {
				newcap = 4096;
			} else {
				newcap = cap * 2;
			}

			tmp = alloc(a, newcap);
			memcpy(tmp, buf, n);
			free(a, buf);
			buf = tmp;
			cap = newcap;
		}

		ret = read(fd, &buf[n], cap - n);

		if ret < 0 {
			die("readall failed");
		}

		if ret == 0 {
			break;
		}

		n = n + ret;
	}

	*len = n;

	return buf;
}

hex2int(s: *byte, len: int, ok: *int): int {
	var i: int;
	var x: int;
	var d: int;
	x = 0;
	i = 0;
	loop {
		if i == len {
			break;
		}

		d = s[i]:int;

		if d == '_' {
			i = i + 1;
			continue;
		} else if d >= '0' && d <= '9' {
			d = d - '0';
		} else if d >= 'a' && d <= 'f' {
			d = d - 'a' + 10;
		} else if d >= 'A' && d <= 'F' {
			d = d - 'A' + 10;
		} else {
			*ok = 0;
			return 0;
		}

		x = x * 16;
		x = x + d;
		i = i + 1;

		if x > 0x7fffffff {
			*ok = 0;
			return 0;
		}
	}

	*ok = 1;
	return x;
}

dec2int(s: *byte, len: int, ok: *int): int {
	var x: int;
	var d: int;
	var i: int;

	x = 0;
	i = 0;
	loop {
		if i == len {
			break;
		}

		d = s[i]:int;
		if d == '_' {
			i = i + 1;
			continue;
		} else if d >= '0' && d <= '9' {
			d = d - '0';
		} else {
			*ok = 0;
			return 0;
		}

		x = x * 10;

		x = x + d;
		i = i + 1;

		if x > 0x7fffffff {
			*ok = 0;
			return 0;
		}
	}

	*ok = 1;
	return x;
}

hexdig(ch: int, ok: *int): int {
	if ch >= '0' && ch <= '9' {
		*ok = 1;
		return ch - '0';
	} else if ch >= 'A' && ch <= 'F' {
		*ok = 1;
		return ch - 'F' + 10;
	} else if ch >= 'a' && ch <= 'f' {
		*ok = 1;
		return ch - 'a' + 10;
	} else {
		*ok = 0;
		return 0;
	}
}


unescape(s: *byte, i: *int, len: int, ok: *int): int {
	var ch: int;
	var hex: int;

	*ok = 1;

	if *i >= len {
		*ok = 0;
		return 0;
	}

	ch = s[*i]:int;
	*i = *i + 1;

	if ch != '\\' {
		return ch;
	}

	if *i >= len {
		*ok = 0;
		return 0;
	}

	ch = s[*i]:int;
	*i = *i + 1;

	if ch == 't' {
		return '\t';
	} else if ch == 'r' {
		return '\r';
	} else if ch == 'n' {
		return '\n';
	} else if ch == '\\' {
		return '\\';
	} else if ch == '\'' {
		return '\'';
	} else if ch == '\"' {
		return '\"';
	} else if ch == 'x' {
		if *i >= len {
			*ok = 0;
			return 0;
		}

		ch = s[*i]:int;
		*i = *i + 1;

		hex = hexdig(ch, ok) * 16;
		if !*ok {
			return 0;
		}

		if *i >= len {
			*ok = 0;
			return 0;
		}

		ch = s[*i]:int;
		*i = *i + 1;

		hex = hex | hexdig(ch, ok);
		if !*ok {
			return 0;
		}

		return hex;
	} else {
		*ok = 0;
		return 0;
	}
}
