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
	quoted: int;
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
	T_LBRA,
	T_RBRA,
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

	if s.c == '{' {
		feedc(s);
		s.tt = T_LBRA;
		return;
	}

	if s.c == ')' {
		feedc(s);
		s.tt = T_RPAR;
		return;
	}

	if s.c == '}' {
		feedc(s);
		s.tt = T_RBRA;
		return;
	}

	s.len = 0;
	s.quoted = 0;

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
			s.quoted = 1;
			feedc(s);
			feedw(s);
			continue;
		}

		if s.c == '"' {
			s.quoted = 1;
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

// command = LF
//         | and_or LF
parse_command(s: *shell): int {
	if s.tt == T_LF {
		feed(s);
		return 1;
	}

	if !parse_and_or(s) {
		return 0;
	}

	if s.tt != T_LF {
		die("command has no end");
	}
	feed(s);

	return 1;
}

// and_or = pipeline
//        | pipeline '||' and_or
//        | pipeine '&&' and_or
parse_and_or(s: *shell): int {
	if !parse_pipeline(s) {
		return 0;
	}

	loop {
		if s.tt != T_AND && s.tt != T_OR {
			return 1;
		}
		feed(s);

		if !parse_pipeline(s) {
			die("expected pipeline");
		}
	}
}

// pipeline = '!' pipeline
//          | compound
//          | compound '|' pipeline
parse_pipeline(s: *shell): int {
	if s.tt == T_NOT {
		loop {
			if s.tt != T_NOT {
				break;
			}
			feed(s);
		}

		if !parse_compound(s) {
			die("expected compound");
		}
	} else {
		if !parse_compound(s) {
			return 0;
		}
	}

	loop {
		if s.tt != T_PIPE {
			return 1;
		}
		feed(s);

		if !parse_compound(s) {
			die("expected compound");
		}
	}
}

// compound = subshell | brace | if | for | while | simple
parse_compound(s: *shell): int {
	if parse_subshell(s) {
		return 1;
	} else if parse_brace(s) {
		return 1;
	} else if parse_if(s) {
		return 1;
	} else if parse_for(s) {
		return 1;
	} else if parse_while(s) {
		return 1;
	} else if parse_simple(s) {
		return 1;
	} else {
		return 0;
	}
}

// subshell = '(' command_list ')'
//          | subshell redir_list
parse_subshell(s: *shell): int {
	if s.tt != T_LPAR {
		return 0;
	}
	feed(s);

	loop {
		if !parse_command(s) {
			break;
		}
	}

	if s.tt != T_RPAR {
		die("expected )");
	}
	feed(s);

	parse_redir_list(s);

	return 1;
}

// brace = '{' command_list '}'
//          | brace redir_list
parse_brace(s: *shell): int {
	if s.tt != T_LBRA {
		return 0;
	}
	feed(s);

	loop {
		if !parse_command(s) {
			break;
		}
	}

	if s.tt != T_RBRA {
		die("expected }");
	}
	feed(s);

	parse_redir_list(s);

	return 1;
}

parse_keyword(s: *shell, key: *byte): int {
	if s.tt != T_WORD {
		return 0;
	}

	if s.quoted {
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

// if = 'if' command 'then' command_list else_fi
// else_fi = 'elif' command 'then' command_list else_fi
//         | 'else' command_list 'fi'
//         | 'fi'
parse_if(s: *shell): int {
	if !parse_keyword(s, "if") {
		return 0;
	}

	if !parse_command(s) {
		die("expected command");
	}

	if !parse_keyword(s, "then") {
		die("expected then");
	}

	loop {
		if parse_keyword(s, "fi") {
			break;
		}

		if parse_keyword(s, "elif") {
			if !parse_command(s) {
				die("expected command");
			}

			if !parse_keyword(s, "then") {
				die("expected then");
			}
			continue;
		}

		if parse_keyword(s, "else") {
			loop {
				if parse_keyword(s, "fi") {
					break;
				}

				if !parse_command(s) {
					die("expected command");
				}
			}
			break;
		}

		if !parse_command(s) {
			die("expected command");
		}
	}

	parse_redir_list(s);

	return 1;
}

// for = 'for' NAME 'in' word_list LF 'do' command_list 'done'
//     | for redir_list
parse_for(s: *shell): int {
	if !parse_keyword(s, "for") {
		return 0;
	}

	if s.tt != T_WORD {
		die("expceted name");
	}
	feed(s);

	if !parse_keyword(s, "in") {
		die("expected in");
	}

	loop {
		if s.tt == T_LF {
			feed(s);
			break;
		}

		if s.tt != T_WORD {
			die("expected word");
		}

		feed(s);
	}

	if !parse_keyword(s, "do") {
		die("expected do");
	}

	loop {
		if parse_keyword(s, "done") {
			break;
		}

		if !parse_command(s) {
			die("expected command");
		}
	}

	parse_redir_list(s);

	return 1;
}

// while = 'while' command 'do' command_list 'done'
//       | while redir_list
parse_while(s: *shell): int {
	if !parse_keyword(s, "while") {
		return 0;
	}

	if !parse_command(s) {
		die("expected command");
	}

	if !parse_keyword(s, "do") {
		die("expceted do");
	}

	loop {
		if !parse_keyword(s, "done") {
			break;
		}

		if !parse_command(s) {
			die("expected command");
		}
	}

	parse_redir_list(s);

	return 1;
}

// simple = word
//        | redir
//        | word simple
//        | redir simple
parse_simple(s: *shell): int {
	if s.tt == T_WORD {
		feed(s);
	} else if !parse_redir(s) {
		return 0;
	}

	loop {
		if s.tt == T_WORD {
			feed(s);
		} else if !parse_redir(s) {
			return 1;
		}
	}
}

// redir_list =
//            | redir redir_list
parse_redir_list(s: *shell): int {
	loop {
		if !parse_redir(s) {
				return 1;
		}
	}
}

// redir = '>' word
//       | '<' word
parse_redir(s: *shell): int {
	if s.tt != T_IN && s.tt != T_OUT {
		return 0;
	}
	feed(s);

	if s.tt != T_WORD {
		die("expected word");
	}
	feed(s);

	return 1;
}

execute_command(s: *shell) {
	die("noimpl");
}

main(argc: int, argv: **byte, envp: **byte) {
	var a: alloc;
	var s: shell;
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

		if !parse_command(&s) {
			exit(127);
		}

		execute_command(&s);
	}
}
