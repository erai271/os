struct compiler {
	a: *alloc;
	f: *file;
	pos: int;
	stack: *int;
	sp: int;
	limit: int;
}

enum {
	FAIL = 0,
	OK = 1,
}

choice(c: *compiler) {
	if c.sp == c.limit {
		die("backtrack overflow");
	}
	c.stack[c.sp] = c.pos;
	c.sp = c.sp + 1;
}

commit(c: *compiler) {
	if c.sp == 0 {
		die("backtrack underflow");
	}
	c.sp = c.sp - 1;
}

fail(c: *compiler) {
	if c.sp == 0 {
		die("backtrack underflow");
	}
	c.sp = c.sp - 1;
	c.pos = c.stack[c.sp];
	fseek(c.f, c.pos);
}

get(c: *compiler): int {
	var ch: int;

	ch = fgetc(c.f);
	if ch != -1 {
		c.pos = c.pos + 1;
	}

	return ch;
}

literal(c: *compiler, s: *byte): int {
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

charclass(c: *compiler, s: *byte): int {
	var i: int;
	var ch: int;

	ch = get(c);

	i = 0;
	loop {
		if !s[i] {
			break;
		}

		if ch == (s[i]:int) {
			return OK;
		}

		i = i + 1;
	}

	fail(c);
	return FAIL;
}

any(c: *compiler): int {
	var ch: int;
	ch = get(c);
	if ch == -1 {
		fail(c);
		return FAIL;
	}
	return OK;
}

// grammar <- sp rule+ !.
p_grammar(c: *compiler): int {
	if !p_sp(c) {
		return FAIL;
	}

	if !p_rule(c) {
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
	if !any(c) {
		return OK;
	}
	fail(c);
	fail(c);

	return FAIL;
}

// rule <- ident '<-' sp pattern
p_rule(c: *compiler): int {
	if !p_ident(c) {
		return FAIL;
	}

	if !literal(c, "<-") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	if !p_pattern(c) {
		return FAIL;
	}

	return OK;
}

// pattern <- alt ( '/' sp alt )*
p_pattern(c: *compiler): int {
	if !p_alt(c) {
		return FAIL;
	}

	loop {
		choice(c);

		if !literal(c, "/") {
			break;
		}

		if !p_sp(c) {
			break;
		}

		if !p_alt(c) {
			break;
		}

		commit(c);
	}

	return OK;
}

// preop <- [!&] sp
p_preop(c: *compiler): int {
	if !charclass(c, "!&") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// alt <- ( preop? suffix )+
p_alt(c: *compiler): int {
	choice(c);
	if p_preop(c) {
		commit(c);
	}

	if !p_suffix(c) {
		return FAIL;
	}

	loop {
		choice(c);

		choice(c);
		if p_preop(c) {
			commit(c);
		}


		if !p_suffix(c) {
			break;
		}

		commit(c);
	}

	return OK;
}

// postop <- [*+?] sp
p_postop(c: *compiler): int {
	if !charclass(c, "*+?") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// suffix <- primary postop*
p_suffix(c: *compiler): int {
	if !p_primary(c) {
		return FAIL;
	}

	loop {
		choice(c);

		if !p_postop(c) {
			break;
		}

		commit(c);
	}

	return OK;
}

// primary <- group / any / literal / charclass / nonterminal
p_primary(c: *compiler): int {
	choice(c);
	if p_group(c) {
		commit(c);
		return OK;
	}

	choice(c);
	if p_any(c) {
		commit(c);
		return OK;
	}

	choice(c);
	if p_literal(c) {
		commit(c);
		return OK;
	}

	choice(c);
	if p_charclass(c) {
		commit(c);
		return OK;
	}

	choice(c);
	if p_nonterminal(c) {
		commit(c);
		return OK;
	}

	fail(c);
	return FAIL;
}

// group <- '(' sp pattern ')' sp
p_group(c: *compiler): int {
	if !literal(c, "(") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	if !p_pattern(c) {
		return FAIL;
	}

	if !literal(c, ")") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// any <- '.' sp
p_any(c: *compiler): int {
	if !literal(c, ".") {
		return FAIL;
	}

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// literal <- ['] ( !['] . )* ['] sp
p_literal(c: *compiler): int {
	choice(c);
	if !literal(c, "'") {
		return FAIL;
	}
	commit(c);

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

	choice(c);
	if !literal(c, "'") {
		return FAIL;
	}
	commit(c);

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// charclass <- '[' ( !']' ( . '-' . / . ) )* ']' sp
p_charclass(c: *compiler): int {
	choice(c);
	if !literal(c, "[") {
		return FAIL;
	}
	commit(c);

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
		if literal(c, "-") {
			if any(c) {
				commit(c);
			}
		}

		commit(c);
	}

	choice(c);
	if !literal(c, "]") {
		return FAIL;
	}
	commit(c);

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// nonterminal <- ident !'<-'
p_nonterminal(c: *compiler): int {
	if !p_ident(c) {
		return FAIL;
	}

	choice(c);
	if !literal(c, "<-") {
		return OK;
	}
	fail(c);
	fail(c);
	return FAIL;
}

// ident <- [a-zA-Z]+ sp
p_ident(c: *compiler): int {
	var chars: *byte;

	chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if !charclass(c, chars) {
		return FAIL;
	}

	loop {
		choice(c);

		if !charclass(c, chars) {
			break;
		}

		commit(c);
	}

	if !p_sp(c) {
		return FAIL;
	}

	return OK;
}

// sp <- ( [ \t\r\n] / '#' ( ![\r\n] . )* )*
p_sp(c: *compiler): int {
	loop {
		choice(c);

		choice(c);
		if charclass(c, " \t\r\n") {
			commit(c);
			commit(c);
			continue;
		}

		choice(c);
		if literal(c, "#") {
			commit(c);

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
			continue;
		}

		break;
	}

	return OK;
}

main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var a: alloc;
	var c: compiler;
	setup_alloc(&a);

	c.a = &a;
	c.pos = 0;
	c.limit = 1024;
	c.stack = alloc(c.a, c.limit * sizeof(c.stack[0])):*int;

	if argc != 2 {
		die("usage: ./peg <grammar.peg>");
	}

	fd = open(argv[1], 0, 0);
	if fd < 0 {
		die("failed to open grammar");
	}

	c.f = fopen(fd, c.a);

	if !p_grammar(&c) {
		die("FAIL");
	}
	fdputs(1, "OK\n");
}
