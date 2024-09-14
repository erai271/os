struct peg_frame {
	pos: int;
	depth: int;
	op: int;
}

struct peg_op {
	tag: int;
	nargs: int;
	start: int;
	end: int;
}

struct peg {
	a: *alloc;

	src: *byte;
	size: int;
	pos: int;

	stack: *peg_frame;
	sp: int;
	limit: int;

	depth: int;
	op: int;
	out: *peg_op;
	cap: int;

	nstack: **peg_node;
	np: int;
	ncap: int;
}

struct peg_node {
	tag: int;
	next: *peg_node;
	child: *peg_node;
	str: *byte;
	len: int;
}

choice(c: *peg) {
	if c.sp == c.limit {
		die("choice overflow");
	}
	c.stack[c.sp].pos = c.pos;
	c.stack[c.sp].depth = c.depth;
	c.stack[c.sp].op = c.op;
	c.sp = c.sp + 1;
}

commit(c: *peg) {
	if c.sp == 0 {
		die("commit underflow");
	}
	c.sp = c.sp - 1;
}

fail(c: *peg) {
	if c.sp == 0 {
		die("fail underflow");
	}
	c.sp = c.sp - 1;
	c.pos = c.stack[c.sp].pos;
	c.depth = c.stack[c.sp].depth;
	c.op = c.stack[c.sp].op;
}

get(c: *peg): int {
	var ch: int;

	if c.pos == c.size {
		return -1;
	}

	ch = c.src[c.pos]:int;
	c.pos = c.pos + 1;

	return ch;
}

literal(c: *peg, s: *byte): int {
	var i: int;
	var ch: int;

	i = 0;
	loop {
		if !s[i] {
			break;
		}

		ch = get(c);
		if ch != (s[i]:int) {
			fail(c);
			return 0;
		}

		i = i + 1;
	}

	return 1;
}

enter(c: *peg) {
	choice(c);
}

leave(c: *peg, tag: int) {
	var nargs: int;
	var start: int;
	var end: int;
	var tmp: *byte;

	commit(c);

	nargs = c.depth - c.stack[c.sp].depth;
	start = c.stack[c.sp].pos;
	end = c.pos;

	if c.op == c.cap {
		if c.cap == 0 {
			c.cap = 1024;
			c.out = alloc(c.a, c.cap * sizeof(c.out[0])):*peg_op;
		} else {
			c.cap = c.cap * 2;
			tmp = alloc(c.a, c.cap * sizeof(c.out[0]));
			memcpy(tmp, c.out:*byte, c.op * sizeof(c.out[0]));
			free(c.a, c.out:*byte);
			c.out = tmp:*peg_op;
		}
	}

	c.out[c.op].tag = tag;
	c.out[c.op].nargs = nargs;
	c.out[c.op].start = start;
	c.out[c.op].end = end;

	c.op = c.op + 1;
	c.depth = c.depth - nargs + 1;
}

charset(c: *peg, s: *byte): int {
	var i: int;
	var ch: int;

	ch = get(c);

	i = 0;
	loop {
		if !s[i] {
			fail(c);
			return 0;
		}

		if ch == (s[i]:int) {
			break;
		}

		i = i + 1;
	}

	return 1;
}

any(c: *peg): int {
	var ch: int;
	ch = get(c);
	if ch == -1 {
		fail(c);
		return 0;
	}
	return 1;
}

construct(c: *peg): *peg_node {
	var i: int;
	var j: int;
	var nargs: int;
	var n: *peg_node;
	var link: **peg_node;

	c.nstack[0] = 0:*peg_node;

	i = 0;
	loop {
		if i == c.op {
			return c.nstack[0];
		}

		n = alloc(c.a, sizeof(*n)):*peg_node;

		n.tag = c.out[i].tag;
		n.next = 0:*peg_node;
		n.child = 0:*peg_node;
		n.str = &c.src[c.out[i].start];
		n.len = c.out[i].end - c.out[i].start;

		nargs = c.out[i].nargs;
		if nargs > c.np {
			die("node underflow");
		}

		link = &n.child;
		j = c.np - nargs;
		loop {
			if j == c.np {
				break;
			}

			*link = c.nstack[j];
			link = &c.nstack[j].next;

			j = j + 1;
		}

		c.np = c.np - nargs;
		if c.np == c.ncap {
			die("node overflow");
		}

		c.nstack[c.np] = n;
		c.np = c.np + 1;

		i = i + 1;
	}
}

peg_new(src: *byte, len: int, a: *alloc): *peg {
	var c: *peg;

	c = alloc(a, sizeof(*c)):*peg;

	c.a = a;

	c.src = src;
	c.size = len;
	c.pos = 0;

	c.limit = 1024;
	c.stack = alloc(a, c.limit * sizeof(*c.stack)):*peg_frame;
	c.sp = 0;

	c.depth = 0;
	c.op = 0;
	c.out = 0:*peg_op;
	c.cap = 0;

	c.ncap = 1024;
	c.nstack = alloc(a, c.ncap * sizeof(*c.nstack)):**peg_node;
	c.np = 0;

	return c;
}

peg_parse(c: *peg): *peg_node {
	choice(c);
	if !p_grammar(c) {
		die("syntax error");
	}
	commit(c);
	return construct(c);
}

peg_reset(c: *peg, src: *byte, len: int) {
	c.src = src;
	c.size = len;
	c.pos = 0;
	c.depth = 0;
	c.sp = 0;
	c.op = 0;
	c.np = 0;
}

peg_free(c: *peg) {
	free(c.a, c.stack: *byte);
	free(c.a, c.nstack: *byte);
	free(c.a, c.out: *byte);
	free(c.a, c: *byte);
}

peg_show(out: *file, n: *peg_node) {
	var i: int;
	var ch: int;
	var hex: *byte;
	hex = "0123456789abcdef";
	fputs(out, "(");
	fputs(out, tag_to_str(n.tag));
	if n.child {
		n = n.child;
		loop {
			if !n {
				break;
			}

			fputc(out, ' ');
			peg_show(out, n);

			n = n.next;
		}
	} else {
		fputc(out, ' ');
		fputc(out, '"');
		i = 0;
		loop {
			if i == n.len {
				break;
			}

			ch = n.str[i]:int;
			if ch < 32 || ch > 127 || ch == '\\' || ch == '"' {
				fputc(out, '\\');
				fputc(out, 'x');
				fputc(out, hex[ch >> 4]:int);
				fputc(out, hex[ch & 15]:int);
			} else {
				fputc(out, ch);
			}

			i = i + 1;
		}
		fputc(out, '"');
	}
	fputs(out, ")");
}
