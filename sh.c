struct shell {
	a: *alloc;
	prog: *byte;
	argv: **byte;
	argc: int;
	scriptfd: int;
	script: *byte;
	c: int;
	tt: int;
	buf: *byte;
	len: int;
	cap: int;
	status: int;
}

enum {
	T_EOF,
	T_LF,
	T_IN,
	T_OUT,
	T_PIPE,
	T_OR,
	T_AND,
	T_BG,
	T_WORD,
	T_NOT,
	T_LPAR,
	T_RPAR,
}

feedc(s: *shell) {
	if s.script[0] {
		s.c = s.script[0]:int;
		s.script = &s.script[1];
		return;
	}

	if s.scriptfd >= 0 {
		s.c = fdgetc(s.scriptfd);
		return;
	}

	s.c = -1;
}

feedw(s: *shell) {
	if s.len == s.cap {
		die("too long");
	}
	if s.c == -1 {
		die("unexpected EOF");
	}
	s.buf[s.len] = s.c:byte;
	s.len = s.len + 1;
	feedc(s);
}

feed_quote(s: *shell) {
	feedc(s);
	loop {
		if s.c == '\"' {
			feedc(s);
			break;
		}

		if s.c == '\\' {
			feedc(s);
			feedw(s);
			continue;
		}

		if s.c == '$' {
			feed_var(s);
			continue;
		}

		feedw(s);
	}
}

feed_var(s: *shell) {
	feedw(s);

	if s.c != '{' {
		die("variable missing {");
	}
	feedw(s);

	if !(
		(s.c >= 'A' && s.c <= 'Z')
		|| (s.c >= 'a' && s.c <= 'z')
		|| (s.c == '_' && s.c <= 'z')
	) {
		die("invalid name");
	}

	loop {
		if s.c == '}' {
			feedw(s);
			break;
		}

		if !(
			(s.c >= 'A' && s.c <= 'Z')
			|| (s.c >= 'a' && s.c <= 'z')
			|| (s.c >= '0' && s.c <= '9')
			|| (s.c == '_' && s.c <= 'z')
		) {
			die("invalid name");
		}

		feedw(s);
	}
}

feed(s: *shell) {
	loop {
		if s.c < 0 {
			s.tt = T_EOF;
			return;
		}

		if s.c == '#' {
			loop {
				feedc(s);
				if s.c == '\n' || s.c < 0 {
					break;
				}
			}
			continue;
		}

		if s.c == '\n' {
			feedc(s);
			s.tt = T_LF;
			return;
		}

		if s.c == ' ' || s.c == '\t' || s.c == '\r' {
			feedc(s);
			continue;
		}

		break;
	}

	if s.c == '<' {
		feedc(s);
		s.tt = T_IN;
		return;
	}

	if s.c == '>' {
		feedc(s);
		s.tt = T_OUT;
		return;
	}

	if s.c == '&' {
		feedc(s);
		s.tt = T_BG;
		if s.c == '&' {
			feedc(s);
			s.tt = T_AND;
		}
		return;
	}

	if s.c == '|' {
		feedc(s);
		s.tt = T_PIPE;
		if s.c == '|' {
			feedc(s);
			s.tt = T_OR;
		}
		return;
	}

	if s.c == '!' {
		feedc(s);
		s.tt = T_NOT;
		return;
	}

	if s.c == ';' {
		feedc(s);
		s.tt = T_LF;
		return;
	}

	if s.c == '(' {
		feedc(s);
		s.tt = T_LPAR;
		return;
	}

	if s.c == ')' {
		feedc(s);
		s.tt = T_RPAR;
		return;
	}

	s.len = 0;

	s.tt = T_WORD;
	loop {
		if (
			s.c > ' ' && s.c < '~'
			&& s.c != '&' && s.c != '|' && s.c != ';'
			&& s.c != '<' && s.c != '>' && s.c != '"'
			&& s.c != '\'' && s.c != '\\' && s.c != '*'
			&& s.c != '?' && s.c != '[' && s.c != '#'
			&& s.c != ']' && s.c != '%' && s.c != '$'
			&& s.c != '(' && s.c != ')' && s.c != '$'
		) {
			feedw(s);
			continue;
		}

		if s.c == '\\' {
			feedc(s);
			feedw(s);
			continue;
		}

		if s.c == '"' {
			feed_quote(s);
			continue;
		}

		if s.c == '$' {
			feed_var(s);
			continue;
		}

		break;
	}

	if !s.len {
		die("invalid char");
	}
}

enum {
	C_NOP,
	C_IN,
	C_OUT,
	C_CMD,
	C_ARG,
	C_FOR,
	C_SUBSHELL,
	C_AND,
	C_OR,
	C_PIPE,
}

struct cmd {
	kind: int;
	word: *byte;
	next: *cmd;
	arg: *cmd;
	redir: *cmd;
	cond: *cmd;
	body: *cmd;
	other: *cmd;
}

mkcmd(s: *shell, kind: int): *cmd {
	var c: *cmd;
	c = alloc(s.a, sizeof(*c)):*cmd;
	bzero(c:*byte, sizeof(*c));
	c.kind = kind;
	return c;
}

// command = LF
//         | and_or LF
parse_command(s: *shell): *cmd {
	var c: *cmd;

	if s.tt == T_LF {
		c = mkcmd(s, C_NOP);
		feed(s);
		return c;
	}

	c = parse_and_or(s);
	if !c {
		return 0:*cmd;
	}

	if s.tt != T_LF {
		die("command has no end");
	}
	feed(s);

	return c;
}

// and_or = pipeline
//        | pipeline '||' and_or
//        | pipeine '&&' and_or
parse_and_or(s: *shell): *cmd {
	var c: *cmd;
	var link: **cmd;
	var p: *cmd;
	var t: *cmd;

	c = 0:*cmd;
	link = &c;

	c = parse_pipeline(s);
	if !c {
		return 0:*cmd;
	}

	loop {
		if s.tt == T_AND {
			feed(s);
			p = parse_pipeline(s);
			if ! p {
				die("expected pipeline");
			}
			t = mkcmd(s, C_AND);
			t.cond = *link;
			t.other = p;
			*link = t;
			link = &t.other;
		}

		if s.tt == T_OR {
			feed(s);
			p = parse_pipeline(s);
			if ! p {
				die("expected pipeline");
			}
			t = mkcmd(s, C_OR);
			t.cond = *link;
			t.other = p;
			*link = t;
			link = &t.other;
		}

		return c;
	}
}

// pipeline = '!' pipeline
//          | compound
//          | compound '|' pipeline
parse_pipeline(s: *shell): *cmd {
	var neg: int;
	var c: *cmd;
	var link: **cmd;
	var p: *cmd;
	var t: *cmd;

	c = 0:*cmd;
	link = &c;

	if s.tt == T_NOT {
		loop {
			if s.tt != T_NOT {
				break;
			}
			feed(s);
			neg = neg + 1;
		}

		c = parse_compound(s);
		if !c {
			die("expected compound");
		}
	} else {
		c = parse_compound(s);
		if !c {
			return 0:*cmd;
		}
	}

	loop {
		if s.tt != T_PIPE {
			return c;
		}
		feed(s);

		p = parse_compound(s);
		if !p {
			die("expected compound");
		}

		t = mkcmd(s, C_PIPE);
		t.cond = *link;
		t.body = p;
		*link = t;
		link = &t.body;
	}
}

// compound = subshell | if | for | simple
parse_compound(s: *shell): *cmd {
	var c: *cmd;

	c = parse_subshell(s);
	if c {
		return c;
	}

	c = parse_for(s);
	if c {
		return c;
	}

	c = parse_simple(s);
	if c {
		return c;
	}

	return 0:*cmd;
}

// subshell = '(' command_list ')'
//          | subshell redir_list
parse_subshell(s: *shell): *cmd {
	var body: *cmd;
	var link: **cmd;
	var t: *cmd;

	body = 0:*cmd;
	link = &body;

	if s.tt != T_LPAR {
		return 0:*cmd;
	}
	feed(s);

	loop {
		t = parse_command(s);
		if !t {
			break;
		}
		*link = t;
		link = &t.next;
	}

	if s.tt != T_RPAR {
		die("expected )");
	}
	feed(s);

	t = mkcmd(s, C_SUBSHELL);
	t.redir = parse_redir_list(s);
	return t;
}

parse_keyword(s: *shell, key: *byte): int {
	if s.tt != T_WORD {
		return 0;
	}

	if s.len != strlen(key) {
		return 0;
	}

	if memcmp(s.buf, key, s.len) {
		return 0;
	}

	feed(s);

	return 1;
}

// for = 'for' NAME 'in' word_list LF 'do' command_list 'done'
//     | for redir_list
parse_for(s: *shell): *cmd {
	var arg: *cmd;
	var arg_link: **cmd;
	var body: *cmd;
	var body_link: **cmd;
	var t: *cmd;
	var w: *byte;

	arg = 0:*cmd;
	arg_link = &arg;
	body = 0:*cmd;
	body_link = &body;

	if !parse_keyword(s, "for") {
		return 0:*cmd;
	}

	w = take(s);
	if !w {
		die("expceted name");
	}

	if !parse_keyword(s, "in") {
		die("expected in");
	}

	loop {
		if s.tt == T_LF {
			feed(s);
			break;
		}

		w = take(s);
		if !w {
			die("expected word");
		}
		t = mkcmd(s, C_ARG);
		t.word = w;
		*arg_link = t;
		arg_link = &t.next;
	}

	if !parse_keyword(s, "do") {
		die("expected do");
	}

	loop {
		if parse_keyword(s, "done") {
			break;
		}

		t = parse_command(s);
		if !t {
			die("expected command");
		}
		*body_link = t;
		body_link = &t.next;
	}

	t = mkcmd(s, C_FOR);
	t.arg = arg;
	t.body = body;
	t.redir = parse_redir_list(s);
	return t;
}

// simple = word
//        | redir
//        | word simple
//        | redir simple
parse_simple(s: *shell): *cmd {
	var arg: *cmd;
	var arg_link: **cmd;
	var redir: *cmd;
	var redir_link: **cmd;
	var c: *cmd;
	var t: *cmd;
	var w: *byte;

	arg = 0:*cmd;
	arg_link = &arg;
	redir = 0:*cmd;
	redir_link = &redir;

	w = take(s);
	if w {
		t = mkcmd(s, C_ARG);
		t.word = w;
		*arg_link = t;
		arg_link = &t.next;
	} else {
		t = parse_redir(s);
		if t {
			*redir_link = t;
			redir_link = &t.next;
		} else {
			return 0: *cmd;
		}
	}

	loop {
		w = take(s);
		if w {
			t = mkcmd(s, C_ARG);
			t.word = w;
			*arg_link = t;
			arg_link = &t.next;
			continue;
		}

		t = parse_redir(s);
		if t {
			*redir_link = t;
			redir_link = &t.next;
			continue;
		}

		t = mkcmd(s, C_CMD);
		t.arg = arg;
		t.redir = redir;
		return t;
	}
}

// redir_list =
//            | redir redir_list
parse_redir_list(s: *shell): *cmd {
	var c: *cmd;
	var t: *cmd;
	var link: **cmd;

	c = 0:*cmd;
	link = &c;

	loop {
		t = parse_redir(s);
		if !t {
			return c;
		}
		*link = t;
		link = &t.next;
	}
}

take(s: *shell): *byte {
	var w: *byte;

	if s.tt != T_WORD {
		return 0: *byte;
	}

	w = alloc(s.a, s.len + 1);
	memcpy(w, s.buf, s.len);
	w[s.len] = 0:byte;
	feed(s);

	return w;
}

// redir = '>' word
//       | '<' word
parse_redir(s: *shell): *cmd {
	var c: *cmd;
	var w: *byte;

	if s.tt == T_IN {
		feed(s);
		c = mkcmd(s, C_IN);
		w = take(s);
		if !w {
			die("expected word");
		}
		c.word = w;
		return c;
	}

	if s.tt == T_OUT {
		feed(s);
		c = mkcmd(s, C_OUT);
		w = take(s);
		if !w {
			die("expected word");
		}
		c.word = w;
		return c;
	}

	return 0:*cmd;
}

execute_command(s: *shell, c: *cmd) {
	if c.kind == C_NOP {
		return;
	} else if c.kind == C_CMD {
	} else if c.kind == C_FOR {
		c = c.body;
		loop {
			if !c {
				break;
			}
			execute_command(s, c);
			c = c.next;
		}
	} else if c.kind == C_SUBSHELL {
		c = c.body;
		loop {
			if !c {
				break;
			}
			execute_command(s, c);
			c = c.next;
		}
	} else if c.kind == C_AND {
		execute_command(s, c.cond);
		if s.status == 0 {
			execute_command(s, c.other);
		}
	} else if c.kind == C_OR {
		execute_command(s, c.cond);
		if s.status != 0 {
			execute_command(s, c.other);
		}
	} else if c.kind == C_PIPE {
		execute_command(s, c.cond);
		c = c.body;
		loop {
			if !c {
				break;
			}
			execute_command(s, c);
			c = c.body;
		}
	} else {
		die("invalid");
	}
}

main(argc: int, argv: **byte, envp: **byte) {
	var a: alloc;
	var s: shell;
	var c: *cmd;
	var i: int;
	var fd: int;
	setup_alloc(&a);

	bzero((&s):*byte, sizeof(s));
	s.a = &a;

	if argc <= 1 {
		s.scriptfd = 0;
		s.script = "";
		s.prog = "";
		s.argc = 0;
		s.argv = 0:**byte;
	} else if strcmp(argv[1], "--") == 0 {
		s.scriptfd = 0;
		s.script = "";
		s.prog = "";
		s.argc = argc - 2;
		s.argv = &argv[2];
	} else if argc > 2 && strcmp(argv[1], "-c") == 0 {
		s.scriptfd = -1;
		s.script = argv[2];
		s.prog = "";
		s.argc = argc - 3;
		s.argv = &argv[3];
	} else {
		fd = open(argv[1], O_RDONLY, 0);
		if fd < 0 {
			die("could not open script");
		}
		s.scriptfd = fd;
		s.script = "";
		s.prog = argv[1];
		s.argc = argc - 2;
		s.argv = &argv[2];
	}

	s.buf = alloc(&a, 4096);
	s.len = 0;
	s.cap = 4096;

	feedc(&s);
	feed(&s);

	loop {
		if s.tt == T_EOF {
			exit(s.status);
		}

		c = parse_command(&s);
		if !c {
			exit(127);
		}

		execute_command(&s, c);
	}
}
