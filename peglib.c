struct peg_frame {
	pos: int;
	depth: int;
	op: int;
	tag: int;
	line: int;
	col: int;
}

struct peg_op {
	tag: int;
	nargs: int;
	start: int;
	end: int;
}

struct peg {
	a: *alloc;

	filename: *byte;

	src: *byte;
	size: int;
	pos: int;
	line: int;
	col: int;
	tag: int;

	fail_depth: int;
	fail_tag: int;
	fail_line: int;
	fail_col: int;
	fail_literal: *byte;

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
	c.stack[c.sp].tag = c.tag;
	c.stack[c.sp].line = c.line;
	c.stack[c.sp].col = c.col;
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

	if c.depth > c.fail_depth {
		c.fail_depth = c.depth;
		c.fail_tag = c.tag;
		c.fail_line = c.line;
		c.fail_col = c.col;
		c.fail_literal = 0:*byte;
	}

	c.sp = c.sp - 1;
	c.pos = c.stack[c.sp].pos;
	c.depth = c.stack[c.sp].depth;
	c.op = c.stack[c.sp].op;
	c.tag = c.stack[c.sp].tag;
	c.line = c.stack[c.sp].line;
	c.col = c.stack[c.sp].col;
}

get(c: *peg): int {
	var ch: int;

	if c.pos == c.size {
		return -1;
	}

	ch = c.src[c.pos]:int;
	c.pos = c.pos + 1;
	c.col = c.col + 1;

	if ch == '\n' {
		c.col = 1;
		c.line = c.line + 1;
	}

	if ch == 0 {
		die("invalid nul in source");
	}

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
			c.fail_literal = s;
			return 0;
		}

		i = i + 1;
	}

	return 1;
}

enter(c: *peg, tag: int) {
	choice(c);
	c.tag = tag;
}

leave(c: *peg, tag: int) {
	var nargs: int;
	var start: int;
	var end: int;
	var tmp: *byte;

	commit(c);
	c.fail_depth = 0;

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

peg_new(filename: *byte, src: *byte, len: int, a: *alloc): *peg {
	var c: *peg;

	c = alloc(a, sizeof(*c)):*peg;

	c.a = a;

	c.filename = filename;

	c.src = src;
	c.size = len;
	c.pos = 0;
	c.tag = 0;
	c.line = 1;
	c.col = 1;
	c.tag = 0;

	c.fail_depth = 0;
	c.fail_tag = 0;
	c.fail_line = 0;
	c.fail_col = 0;
	c.fail_literal = 0:*byte;

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
		fdputs(2, "syntax error at ");
		fdputs(2, c.filename);
		fdputs(2, ":");
		fdputd(2, c.fail_line);
		fdputs(2, ":");
		fdputd(2, c.fail_col);
		fdputs(2, " expected ");
		fdputs(2, tag_to_str(c.fail_tag));
		if c.fail_literal {
			fdputs(2, " '");
			fdputs(2, c.fail_literal);
			fdputs(2, "'");
		}
		fdputs(2, "\n");
		exit(1);
	}
	commit(c);
	return construct(c);
}

peg_reset(c: *peg, src: *byte, len: int) {
	c.src = src;
	c.size = len;
	c.pos = 0;
	c.tag = 0;
	c.line = 1;
	c.col = 1;
	c.tag = 0;
	c.fail_depth = 0;
	c.fail_tag = 0;
	c.fail_line = 0;
	c.fail_col = 0;
	c.fail_literal = 0:*byte;
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
