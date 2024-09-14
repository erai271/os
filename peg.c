struct compiler {
	a: *alloc;
	p: *peg;
	out: *file;
	scratch: *byte;
}

enum {
	FAIL = 0,
	OK = 1,
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

	if !charset(c, "!&") {
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

	if !charset(c, "*+?") {
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

	if !charset(c, chars) {
		fail(c);
		return FAIL;
	}

	loop {
		choice(c);

		if !charset(c, chars) {
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
		if charset(c, " \t\r\n") {
			commit(c);
			commit(c);
			continue;
		}

		choice(c);
		if literal(c, "//") {
			loop {
				choice(c);

				choice(c);
				if charset(c, "\r\n") {
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

translate_literal(c: *compiler, n: *peg_node) {
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
			fputc(c.out, '\\');
			fputc(c.out, 'x');
			fputc(c.out, hex[ch >> 4]:int);
			fputc(c.out, hex[ch & 15]:int);
		} else {
			fputc(c.out, ch);
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

translate_charset(c: *compiler, n: *peg_node) {
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
				fputc(c.out, '\\');
				fputc(c.out, 'x');
				fputc(c.out, hex[i >> 4]:int);
				fputc(c.out, hex[i & 15]:int);
			} else {
				fputc(c.out, i);
			}
		}

		i = i + 1;
	}
}

translate_pattern(c: *compiler, n: *peg_node) {
	var count: int;
	var look: int;
	var d: *peg_node;

	loop {
		if n.tag == P_pattern {
			d = n.child;
			if !d.next {
				translate_pattern(c, d);
			} else {
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, d);
				d = d.next;
				loop {
					if !d {
						break;
					}

					fputs(c.out, "    if !ok { choice(c);\n");
					translate_pattern(c, d);
					fputs(c.out, "    }\n");

					d = d.next;
				}
				fputs(c.out, "    if ok { commit(c); } else { fail(c); }\n");
			}
		} else if n.tag == P_alternative {
			d = n.child;
			translate_pattern(c, d);
			d = d.next;
			loop {
				if !d {
					break;
				}

				fputs(c.out, "    if ok {\n");
				translate_pattern(c, d);
				fputs(c.out, "    }\n");

				d = d.next;
			}
		} else if n.tag == P_lookahead {
			look = decode_look(n);
			d = n.child;
			if d.tag == P_lookop {
				d = d.next;
			}

			if look == LOOK_AND {
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, d);
				fputs(c.out, "    fail(c);\n");
			} else if look == LOOK_NOT {
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, d);
				fputs(c.out, "    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }\n");
			} else if look == LOOK_NORMAL {
				translate_pattern(c, d);
			} else {
				die("invalid lookop");
			}
		} else if n.tag == P_suffix {
			count = decode_count(n);
			if count == ZERO_OR_ONE {
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.out, "    if ok { commit(c); } else { ok = 1; }\n");
			} else if count == EXACTLY_ONE {
				translate_pattern(c, n.child);
			} else if count == ZERO_OR_MORE {
				fputs(c.out, "    loop {\n");
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.out, "    if !ok { ok = 1; break; }\n");
				fputs(c.out, "    commit(c);\n");
				fputs(c.out, "    }\n");
			} else if count == ONE_OR_MORE {
				translate_pattern(c, n.child);
				fputs(c.out, "    if ok {\n");
				fputs(c.out, "    loop {\n");
				fputs(c.out, "    choice(c);\n");
				translate_pattern(c, n.child);
				fputs(c.out, "    if !ok { ok = 1; break; }\n");
				fputs(c.out, "    commit(c);\n");
				fputs(c.out, "    }\n");
				fputs(c.out, "    }\n");
			} else {
				die("invalid countop");
			}
		} else if n.tag == P_primary {
			translate_pattern(c, n.child);
		} else if n.tag == P_any {
			fputs(c.out, "    ok = any(c);\n");
		} else if n.tag == P_literal {
			fputs(c.out, "    ok = literal(c, \"");
			translate_literal(c, n);
			fputs(c.out, "\");\n");
		} else if n.tag == P_class {
			fputs(c.out, "    ok = charset(c, \"");
			translate_charset(c, n);
			fputs(c.out, "\");\n");
		} else if n.tag == P_call {
			fputs(c.out, "    ok = p_");
			fputb(c.out, n.child.str, n.child.len);
			fputs(c.out, "(c);\n");
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

translate(c: *compiler, n: *peg_node) {
	var v: *peg_node;

	// Generate tags for each rule
	fputs(c.out, "enum {\n");
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.out, "    P_");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, ",\n");
		}

		v = v.next;
	}
	fputs(c.out, "}\n");

	// Generate tag to string
	fputs(c.out, "\ntag_to_str(tag: int): *byte {\n");
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.out, "    if tag == P_");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, " { return \"");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, "\"; }\n");
		}

		v = v.next;
	}
	fputs(c.out, "    die(\"invalid tag\");\n");
	fputs(c.out, "}\n");

	// Generate parsing functions for each rule
	v = n.child;
	loop {
		if !v {
			break;
		}

		if v.tag == P_rule {
			fputs(c.out, "\np_");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, "(c: *peg): int {\n");
			fputs(c.out, "    var ok: int;\n");
			fputs(c.out, "    enter(c);\n");
			translate_pattern(c, v.child.next);
			fputs(c.out, "    if ok { leave(c, P_");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, "); } else { fail(c); }\n");
			fputs(c.out, "    return ok;\n");
			fputs(c.out, "}\n");
		}

		v = v.next;
	}
}

main(argc: int, argv: **byte, envp: **byte) {
	var ifd: int;
	var ofd: int;
	var f: *file;
	var out: *file;
	var a: alloc;
	var c: compiler;
	var i: int;
	var src: *byte;
	var len: int;
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
	c.scratch = alloc(c.a, 256);

	f = fopen(ifd, c.a);
	src = freadall(f, &len);
	fclose(f);

	out = fopen(ofd, c.a);
	c.out = out;

	c.p = peg_new(src, len, c.a);
	node = peg_parse(c.p);

	translate(&c, node);

	fflush(out);
	fclose(out);
}
