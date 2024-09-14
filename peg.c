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
	f: *file;
	fout: *file;
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
	scratch: *byte;
}

enum {
	FAIL = 0,
	OK = 1,
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
			return FAIL;
		}

		i = i + 1;
	}

	return OK;
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

enum {
	P_grammar = 1,
	P_rule,
	P_pattern,
	P_alternative,
	P_lookop,
	P_lookahead,
	P_countop,
	P_suffix,
	P_primary,
	P_any,
	P_literal,
	P_class,
	P_call,
	P_identifier,
	P_sp,
}

tag_to_str(tag: int): *byte {
	if tag == P_grammar { return "P_grammar"; }
	if tag == P_rule { return "P_rule"; }
	if tag == P_pattern { return "P_pattern"; }
	if tag == P_alternative { return "P_alternative"; }
	if tag == P_lookop { return "P_lookop"; }
	if tag == P_lookahead { return "P_lookahead"; }
	if tag == P_countop { return "P_countop"; }
	if tag == P_suffix { return "P_suffix"; }
	if tag == P_primary { return "P_primary"; }
	if tag == P_any { return "P_any"; }
	if tag == P_literal { return "P_literal"; }
	if tag == P_class { return "P_class"; }
	if tag == P_call { return "P_call"; }
	if tag == P_identifier { return "P_identifier"; }
	if tag == P_sp { return "P_sp"; }
	return "(invalid)";
}

charclass(c: *peg, s: *byte): int {
	var i: int;
	var ch: int;

	ch = get(c);

	i = 0;
	loop {
		if !s[i] {
			fail(c);
			return FAIL;
		}

		if ch == (s[i]:int) {
			break;
		}

		i = i + 1;
	}

	return OK;
}

any(c: *peg): int {
	var ch: int;
	ch = get(c);
	if ch == -1 {
		fail(c);
		return FAIL;
	}
	return OK;
}

// grammar <- sp rule+ !.
p_grammar(c: *peg): int {
	enter(c);

	if !p_sp(c) {
		fail(c);
		return FAIL;
	}

	if !p_rule(c) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);
		if !p_rule(c) {
			break;
		}
		commit(c);
	}

	choice(c);
	if any(c) {
		fail(c);
		fail(c);
		return FAIL;
	}

	leave(c, P_grammar);
	return OK;
}

// rule <- identifier sp '<-' sp pattern
p_rule(c: *peg): int {
	enter(c);

	if !p_identifier(c) {
		fail(c);
		return FAIL;
	}

	if !p_sp(c) {
		fail(c);
		return FAIL;
	}

	if !literal(c, "<-") {
		fail(c);
		return FAIL;
	}

	if !p_sp(c) {
		fail(c);
		return FAIL;
	}

	if !p_pattern(c) {
		fail(c);
		return FAIL;
	}

	leave(c, P_rule);
	return OK;
}

// pattern <- alternative ( '/' !'/' sp alternative )*
p_pattern(c: *peg): int {
	enter(c);

	if !p_alternative(c) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		if !literal(c, "/") {
			break;
		}

		choice(c);
		if literal(c, "/") {
			fail(c);
			fail(c);
			return FAIL;
		}

		if !p_sp(c) {
			break;
		}

		if !p_alternative(c) {
			break;
		}

		commit(c);
	}

	leave(c, P_pattern);
	return OK;
}

// lookop <- [!&]
p_lookop(c: *peg): int {
	enter(c);

	if !charclass(c, "!&") {
		fail(c);
		return FAIL;
	}

	leave(c, P_lookop);
	return OK;
}

// alternative <- lookahead+
p_alternative(c: *peg): int {
	enter(c);

	if !p_lookahead(c) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		if !p_lookahead(c) {
			break;
		}

		commit(c);
	}

	leave(c, P_alternative);
	return OK;
}

// lookahead <- (lookop sp)? suffix
p_lookahead(c: *peg): int {
	enter(c);

	choice(c);
	if p_lookop(c) && p_sp(c) {
		commit(c);
	}

	if !p_suffix(c) {
		fail(c);
		return FAIL;
	}

	leave(c, P_lookahead);
	return OK;
}

// countop <- [*+?]
p_countop(c: *peg): int {
	enter(c);

	if !charclass(c, "*+?") {
		fail(c);
		return FAIL;
	}

	leave(c, P_countop);
	return OK;
}

// suffix <- primary (countop sp)*
p_suffix(c: *peg): int {
	enter(c);

	if !p_primary(c) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		if !p_countop(c) {
			break;
		}

		if !p_sp(c) {
			break;
		}

		commit(c);
	}

	leave(c, P_suffix);
	return OK;
}

// primary <- ( '(' sp pattern ')' / any / literal / class / call ) sp
p_primary(c: *peg): int {
	enter(c);

	loop {
		choice(c);
		if literal(c, "(") && p_sp(c) && p_pattern(c) && literal(c, ")") {
			commit(c);
			break;
		}

		choice(c);
		if p_any(c) {
			commit(c);
			break;
		}

		choice(c);
		if p_literal(c) {
			commit(c);
			break;
		}

		choice(c);
		if p_class(c) {
			commit(c);
			break;
		}

		choice(c);
		if p_call(c) {
			commit(c);
			break;
		}

		fail(c);
		fail(c);
		return FAIL;
	}

	if !p_sp(c) {
		fail(c);
		return FAIL;
	}

	leave(c, P_primary);
	return OK;
}

// any <- '.'
p_any(c: *peg): int {
	enter(c);

	if !literal(c, ".") {
		fail(c);
		return FAIL;
	}

	leave(c, P_any);
	return OK;
}

// literal <- ['] ( !['] . )* [']
p_literal(c: *peg): int {
	enter(c);

	if !literal(c, "'") {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		choice(c);
		if literal(c, "'") {
			fail(c);
			fail(c);
			break;
		}

		if !any(c) {
			break;
		}

		commit(c);
	}

	if !literal(c, "'") {
		fail(c);
		return FAIL;
	}

	leave(c, P_literal);
	return OK;
}

// charclass <- '[' ( !']' ( . '-' . / . ) )* ']'
p_class(c: *peg): int {
	enter(c);

	if !literal(c, "[") {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		choice(c);
		if literal(c, "]") {
			fail(c);
			fail(c);
			break;
		}

		if !any(c) {
			break;
		}

		choice(c);
		if literal(c, "-") && any(c) {
			commit(c);
		}

		commit(c);
	}

	if !literal(c, "]") {
		fail(c);
		return FAIL;
	}

	leave(c, P_class);
	return OK;
}

// call <- identifier !'<-'
p_call(c: *peg): int {
	enter(c);

	if !p_identifier(c) {
		fail(c);
		return FAIL;
	}

	choice(c);
	if p_sp(c) && literal(c, "<-") {
		fail(c);
		fail(c);
		fail(c);
		return FAIL;
	}

	leave(c, P_call);
	return OK;
}

// identifier <- [a-zA-Z0-9_]+
p_identifier(c: *peg): int {
	var chars: *byte;
	enter(c);

	chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

	if !charclass(c, chars) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		if !charclass(c, chars) {
			break;
		}

		commit(c);
	}

	leave(c, P_identifier);
	return OK;
}

// sp <- ( [ \t\r\n] / '//' ( ![\r\n] . )* )*
p_sp(c: *peg): int {
	enter(c);

	loop {
		choice(c);

		choice(c);
		if charclass(c, " \t\r\n") {
			commit(c);
			commit(c);
			continue;
		}

		choice(c);
		if literal(c, "//") {
			loop {
				choice(c);

				choice(c);
				if charclass(c, "\r\n") {
					fail(c);
					fail(c);
					break;
				}

				if !any(c) {
					break;
				}

				commit(c);
			}

			commit(c);
			commit(c);
			continue;
		}

		fail(c);
		break;
	}

	leave(c, P_sp);
	return OK;
}

struct peg_node {
	tag: int;
	next: *peg_node;
	child: *peg_node;
	str: *byte;
	len: int;
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

enum {
	LOOK_NORMAL,
	LOOK_NOT,
	LOOK_AND,
}

decode_look(n: *peg_node): int {
	var ret: int;

	ret = LOOK_NORMAL;
	if n.child.tag == P_lookop {
		if n.child.str[0] == '!':byte {
			ret = LOOK_NOT;
		} else if n.child.str[0] == '&':byte {
			ret = LOOK_AND;
		}
	}

	return ret;
}

enum {
	ZERO_OR_ONE,
	EXACTLY_ONE,
	ZERO_OR_MORE,
	ONE_OR_MORE,
}

decode_count(n: *peg_node): int {
	var ret: int;

	ret = EXACTLY_ONE;
	n = n.child;
	loop {
		if !n {
			return ret;
		}

		if n.tag == P_countop {
			if n.str[0] == '?':byte {
				if ret == EXACTLY_ONE {
					ret = ZERO_OR_ONE;
				} else if ret == ONE_OR_MORE {
					ret = ZERO_OR_MORE;
				}
			} else if n.str[0] == '*':byte {
				ret = ZERO_OR_MORE;
			} else if n.str[0] == '+':byte {
				if ret == ZERO_OR_ONE {
					ret = ZERO_OR_MORE;
				} else if ret == EXACTLY_ONE {
					ret = ONE_OR_MORE;
				} else if ret == ZERO_OR_MORE {
					ret = ZERO_OR_MORE;
				}
			} else {
				die("invalid countop");
			}
		}

		n = n.next;
	}
}

translate_literal(c: *peg, n: *peg_node) {
	var i: int;
	var len: int;
	var ch: int;
	var hex: *byte;

	hex = "0123456789abcdef";

	i = 1;
	len = n.len - 1;
	loop {
		if i == len {
			break;
		}

		ch = n.str[i]:int;

		if ch < 32 || ch > 127 || ch == '\\' || ch == '"' {
			fputc(c.fout, '\\');
			fputc(c.fout, 'x');
			fputc(c.fout, hex[ch >> 4]:int);
			fputc(c.fout, hex[ch & 15]:int);
		} else {
			fputc(c.fout, ch);
		}

		i = i + 1;
	}
}

hexdig(c: byte): int {
	var ch: int;

	ch = c:int;

	if ch >= '0' && ch <= '9' {
		return ch - '0';
	}

	if ch >= 'a' && ch <= 'f' {
		return ch - 'a' + 10;
	}

	if ch >= 'A' && ch <= 'F' {
		return ch - 'A' + 10;
	}

	die("invalid hex digit");
}

parse_escape(s: *byte, i: *int, n: int): int {
	var nc: int;

	if *i == n {
		die("invalid escape");
	}

	nc = s[*i]:int;
	*i = *i + 1;

	if nc == 't' {
		return '\t';
	} else if nc == 'r' {
		return '\r';
	} else if nc == 'n' {
		return '\n';
	} else if nc == '\\' {
		return '\\';
	} else if nc == '\'' {
		return '\'';
	} else if nc == '"' {
		return '"';
	} else if nc == '-' {
		return '-';
	} else if nc == '[' {
		return '[';
	} else if nc == ']' {
		return ']';
	} else if nc == 'x' {
		if n - *i < 2 {
			die("invalid escape");
		}
		nc = hexdig(s[*i]) * 16 + hexdig(s[*i + 1]);
		*i = *i + 2;
		return nc;
	} else {
		die("invalid escape");
	}
}

translate_charset(c: *peg, n: *peg_node) {
	var i: int;
	var len: int;
	var ch: int;
	var a: int;
	var b: int;
	var hex: *byte;

	hex = "0123456789abcdef";

	memset(c.scratch, 0, 256);

	i = 1;
	len = n.len - 1;
	loop {
		if i == len {
			break;
		}

		ch = n.str[i]:int;
		i = i + 1;

		if ch == '\\' {
			ch = parse_escape(n.str, &i, len);
		}

		if i < len && n.str[i] == '-':byte {
			i = i + 1;

			if i == len {
				die("invalid range");
			}

			a = ch;

			ch = n.str[i]:int;
			i = i + 1;

			if ch == '\\' {
				ch = parse_escape(n.str, &i, len);
			}

			b = ch;

			loop {
				if a > b {
					break;
				}

				c.scratch[a] = 1:byte;

				a = a + 1;
			}
		} else {
			c.scratch[ch] = 1:byte;
		}
	}

	i = 1;
	loop {
		if i == 256 {
			break;
		}

		if c.scratch[i] {
			if ch < 32 || ch > 127 || ch == '\\' || ch == '"' {
				fputc(c.fout, '\\');
				fputc(c.fout, 'x');
				fputc(c.fout, hex[i >> 4]:int);
				fputc(c.fout, hex[i & 15]:int);
			} else {
				fputc(c.fout, i);
			}
		}

		i = i + 1;
	}
}

translate_pattern(c: *peg, n: *peg_node) {
	var count: int;
	var look: int;
	var d: *peg_node;

	loop {
		if n.tag == P_pattern {
			d = n.child;
			if !d.next {
				translate_pattern(c, d);
			} else {
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, d);
				d = d.next;
				loop {
					if !d {
						break;
					}

					fputs(c.fout, "    if !ok { choice(c);\n");
					translate_pattern(c, d);
					fputs(c.fout, "    }\n");

					d = d.next;
				}
				fputs(c.fout, "    if ok { commit(c); } else { fail(c); }\n");
			}
		} else if n.tag == P_alternative {
			d = n.child;
			translate_pattern(c, d);
			d = d.next;
			loop {
				if !d {
					break;
				}

				fputs(c.fout, "    if ok {\n");
				translate_pattern(c, d);
				fputs(c.fout, "    }\n");

				d = d.next;
			}
		} else if n.tag == P_lookahead {
			look = decode_look(n);
			d = n.child;
			if d.tag == P_lookop {
				d = d.next;
			}

			if look == LOOK_AND {
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, d);
				fputs(c.fout, "    fail(c);\n");
			} else if look == LOOK_NOT {
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, d);
				fputs(c.fout, "    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }\n");
			} else if look == LOOK_NORMAL {
				translate_pattern(c, d);
			} else {
				die("invalid lookop");
			}
		} else if n.tag == P_suffix {
			count = decode_count(n);
			if count == ZERO_OR_ONE {
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.fout, "    if ok { commit(c); } else { ok = 1; }\n");
			} else if count == EXACTLY_ONE {
				translate_pattern(c, n.child);
			} else if count == ZERO_OR_MORE {
				fputs(c.fout, "    loop {\n");
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.fout, "    if !ok { ok = 1; break; }\n");
				fputs(c.fout, "    commit(c);\n");
				fputs(c.fout, "    }\n");
			} else if count == ONE_OR_MORE {
				translate_pattern(c, n.child);
				fputs(c.fout, "    if ok {\n");
				fputs(c.fout, "    loop {\n");
				fputs(c.fout, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.fout, "    if !ok { ok = 1; break; }\n");
				fputs(c.fout, "    commit(c);\n");
				fputs(c.fout, "    }\n");
				fputs(c.fout, "    }\n");
			} else {
				die("invalid countop");
			}
		} else if n.tag == P_primary {
			translate_pattern(c, n.child);
		} else if n.tag == P_any {
			fputs(c.fout, "    ok = any(c);\n");
		} else if n.tag == P_literal {
			fputs(c.fout, "    ok = literal(c, \"");
			translate_literal(c, n);
			fputs(c.fout, "\");\n");
		} else if n.tag == P_class {
			fputs(c.fout, "    ok = charset(c, \"");
			translate_charset(c, n);
			fputs(c.fout, "\");\n");
		} else if n.tag == P_call {
			fputs(c.fout, "    ok = p_");
			fputb(c.fout, n.child.str, n.child.len);
			fputs(c.fout, "(c);\n");
		} else if n.tag == P_sp {
			n = n.next;
			continue;
		} else {
			fdputs(2, tag_to_str(n.tag));
			die("invalid tag");
		}

		break;
	}
}

translate(c: *peg, n: *peg_node) {
	var v: *peg_node;

	// Generate tags for each rule
	fputs(c.fout, "enum {\n");
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.fout, "    P_");
			fputb(c.fout, v.child.str, v.child.len);
			fputs(c.fout, ",\n");
		}

		v = v.next;
	}
	fputs(c.fout, "}\n");

	// Generate tag to string
	fputs(c.fout, "\ntag_to_str(tag: int): *byte {\n");
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.fout, "    if tag == P_");
			fputb(c.fout, v.child.str, v.child.len);
			fputs(c.fout, " { return \"");
			fputb(c.fout, v.child.str, v.child.len);
			fputs(c.fout, "\"; }\n");
		}

		v = v.next;
	}
	fputs(c.fout, "    die(\"invalid tag\");\n");
	fputs(c.fout, "}\n");

	// Generate parsing functions for each rule
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.fout, "\np_");
			fputb(c.fout, v.child.str, v.child.len);
			fputs(c.fout, "(c: *peg): int {\n");
			fputs(c.fout, "    var ok: int;\n");
			fputs(c.fout, "    enter(c);\n");
			translate_pattern(c, v.child.next);
			fputs(c.fout, "    if ok { leave(c, P_");
			fputb(c.fout, v.child.str, v.child.len);
			fputs(c.fout, "); } else { fail(c); }\n");
			fputs(c.fout, "    return ok;\n");
			fputs(c.fout, "}\n");
		}

		v = v.next;
	}
}

main(argc: int, argv: **byte, envp: **byte) {
	var ifd: int;
	var ofd: int;
	var a: alloc;
	var c: peg;
	var i: int;
	var node: *peg_node;
	setup_alloc(&a);

	ifd = 0;
	ofd = 1;

	i = 1;
	loop {
		if i >= argc {
			break;
		}

		if strcmp(argv[i], "-o") == 0 {
			i = i + 1;
			if i >= argc {
				die("expected output file name");
			}

			unlink(argv[i]);

			ofd = open(argv[i], O_CREAT | O_WRONLY, (6 << 6) + (6 << 3) + 6);
			if ofd < 0 {
				die("failed to open output");
			}

			i = i + 1;
			continue;
		}

		if argv[i][0] == '-':byte {
			die("usage: ./peg [-o grammar.c] <grammar.peg>");
		}

		if ifd != 0 {
			die("too many inputs");
		}

		ifd = open(argv[i], 0, 0);
		if ifd < 0 {
			die("failed to open input");
		}

		i = i + 1;
	}

	c.a = &a;
	c.pos = 0;
	c.limit = 1024;
	c.stack = alloc(c.a, c.limit * sizeof(c.stack[0])):*peg_frame;

	c.ncap = 1024;
	c.np = 0;
	c.nstack = alloc(c.a, c.ncap * sizeof(c.nstack[0])):**peg_node;

	c.scratch = alloc(c.a, 256);

	c.f = fopen(ifd, c.a);
	c.src = freadall(c.f, &c.size);

	c.fout = fopen(ofd, c.a);

	choice(&c);
	if !p_grammar(&c) {
		die("Syntax error");
	}
	commit(&c);

	node = construct(&c);

	translate(&c, node);

	fflush(c.fout);
	fclose(c.fout);
}
