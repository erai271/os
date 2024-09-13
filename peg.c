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

main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var a: alloc;
	var c: peg;
	var i: int;
	setup_alloc(&a);

	c.a = &a;
	c.pos = 0;
	c.limit = 1024;
	c.stack = alloc(c.a, c.limit * sizeof(c.stack[0])):*peg_frame;

	if argc != 2 {
		die("usage: ./peg <grammar.peg>");
	}

	fd = open(argv[1], 0, 0);
	if fd < 0 {
		die("failed to open grammar");
	}

	c.f = fopen(fd, c.a);

	c.src = freadall(c.f, &c.size);

	choice(&c);
	if !p_grammar(&c) {
		die("Syntax error");
	}
	commit(&c);

	i = 0;
	loop {
		if i == c.op {
			break;
		}

		if c.out[i].nargs == 0 {
			fdputs(1, tag_to_str(c.out[i].tag));
			fdputc(1, ' ');
			fdputc(1, '"');
			write(1, &c.src[c.out[i].start], c.out[i].end - c.out[i].start);
			fdputc(1, '"');
			fdputc(1, '\n');
		}

		if c.out[i].nargs != 0 {
			fdputs(1, tag_to_str(c.out[i].tag));
			fdputc(1, ' ');
			fdputd(1, c.out[i].nargs);
			fdputc(1, '\n');
		}

		i = i + 1;
	}
}
