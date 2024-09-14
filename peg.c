struct compiler {
	a: *alloc;
	p: *peg;
	out: *file;
	scratch: *byte;
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

	fputs(c.out, "    ok = literal(c, \"");
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
	fputs(c.out, "\");\n");
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
	var count: int;

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

	count = 0;
	i = 1;
	loop {
		if i == 256 {
			break;
		}
		count = count + c.scratch[i]:int;
		i = i + 1;
	}

	if count == 0 {
		fputs(c.out, "    fail(c);\n");
		fputs(c.out, "    ok = 0;\n");
		return;
	} else if count >= 255 {
		fputs(c.out, "    ok = any(c);\n");
		return;
	} else if count == 1 {
		fputs(c.out, "    ok = literal(c, \"");
	} else {
		fputs(c.out, "    ok = charset(c, \"");
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
	fputs(c.out, "\");\n");
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
			translate_literal(c, n);
		} else if n.tag == P_class {
			translate_charset(c, n);
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
			fputs(c.out, "    enter(c, P_");
			fputb(c.out, v.child.str, v.child.len);
			fputs(c.out, ");\n");
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
	var filename: *byte;
	setup_alloc(&a);

	ifd = 0;
	ofd = 1;
	filename = "-";

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

		filename = argv[i];
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

	c.p = peg_new(filename, src, len, c.a);
	node = peg_parse(c.p, P_sp);

	translate(&c, node);

	fflush(out);
	fclose(out);
}
