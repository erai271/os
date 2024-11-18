struct parser {
	a: *alloc;
	l: *lexer;
}

pdie(c: *parser, msg: *byte) {
	ldie(c.l, msg);
}

setup_parser(a: *alloc): *parser {
	var c: *parser;

	c = alloc(a, sizeof(*c)):*parser;

	c.a = a;

	c.l = setup_lexer(a);

	return c;
}

// Copy the current token
intern(c: *parser): *byte {
	var ret: *byte;
	var i: int;

	ret = alloc(c.a, c.l.tlen + 1);

	i = 0;
	loop {
		if (i == c.l.tlen) {
			ret[i] = 0:byte;
			return ret;
		}

		ret[i] = c.l.token[i];

		i = i + 1;
	}
}

// ident := IDENT
parse_ident(c: *parser): *node {
	var n: *node;

	if (c.l.tt != T_IDENT) {
		return 0:*node;
	}

	n = mknode0(c, N_IDENT);
	n.s = intern(c);
	feed(c.l);

	return n;
}

parse_hex(c: *parser): *node {
	var n: *node;
	var x: int;
	var d: int;
	var i: int;

	x = 0;
	i = 0;
	loop {
		if (i == c.l.tlen) {
			break;
		}

		d = c.l.token[i]:int;

		if (d >= '0' && d <= '9') {
			d = d - '0';
		} else if (d >= 'a' && d <= 'f') {
			d = (d - 'a') + 10;
		} else {
			d = (d - 'A') + 10;
		}

		x = x * 16;
		x = x + d;
		i = i + 1;

		if (x > 0x7fffffff) {
			pdie(c, "overflow");
		}
	}

	n = mknode0(c, N_NUM);
	n.n = x;
	feed(c.l);

	return n;
}

// num := NUM
parse_num(c: *parser): *node {
	var n: *node;
	var x: int;
	var d: int;
	var i: int;

	if (c.l.tt == T_HEX) {
		return parse_hex(c);
	}

	if (c.l.tt != T_NUM) {
		return 0:*node;
	}

	x = 0;
	i = 0;
	loop {
		if (i == c.l.tlen) {
			break;
		}

		d = (c.l.token[i]:int) - '0';

		x = x * 10;

		x = x + d;
		i = i + 1;

		if (x > 0x7fffffff) {
			pdie(c, "overflow");
		}
	}

	n = mknode0(c, N_NUM);
	n.n = x;
	feed(c.l);

	return n;
}

// chr := CHAR
parse_chr(c: *parser): *node {
	var n: *node;

	if (c.l.tt != T_CHAR) {
		return 0:*node;
	}

	n = mknode0(c, N_CHAR);
	n.n = c.l.token[0]:int;
	feed(c.l);

	return n;
}

// str := STR
parse_str(c: *parser): *node {
	var n: *node;

	if (c.l.tt != T_STR) {
		return 0:*node;
	}

	n = mknode0(c, N_STR);
	n.s = intern(c);
	feed(c.l);

	return n;
}

// primary := ident
//          | num
//          | str
//          | chr
//          | '(' expr ')'
parse_primary(c: *parser): *node {
	var n: *node;

	n = parse_ident(c);
	if (n) {
		return n;
	}

	n = parse_num(c);
	if (n) {
		return n;
	}

	n = parse_str(c);
	if (n) {
		return n;
	}

	n = parse_chr(c);
	if (n) {
		return n;
	}

	if (c.l.tt == T_LPAR) {
		feed(c.l);

		n = parse_expr(c);
		if (!n) {
			pdie(c, "expected expr");
		}

		if (c.l.tt != T_RPAR) {
			pdie(c, "expected )");
		}
		feed(c.l);

		return n;
	}

	return 0:*node;
}

// expr_list := expr
//            | expr ',' expr_list
parse_expr_list(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;

	a = parse_expr(c);
	if (!a) {
		return 0:*node;
	}

	e = mknode1(c, N_EXPRLIST, a);
	n = e;

	loop {
		if (c.l.tt != T_COMMA) {
			return n;
		}
		feed(c.l);

		a = parse_expr(c);
		if (!a) {
			pdie(c, "expected expr");
		}

		e.b = mknode1(c, N_EXPRLIST, a);
		e = e.b;
	}
}

// post_expr := primary
//            | 'sizeof' '(' expr ')'
//            | post_expr '[' expr ']'
//            | post_expr '(' expr_list ')'
//            | post_expr '.' ident
//            | post_expr ':' type
parse_post_expr(c: *parser): *node {
	var n: *node;
	var b: *node;

	if (c.l.tt == T_IDENT && !strcmp(c.l.token, "sizeof")) {
		feed(c.l);

		if (c.l.tt != T_LPAR) {
			pdie(c, "expected (");
		}
		feed(c.l);

		n = parse_expr(c);
		if (!n) {
			pdie(c, "expected expr");
		}

		if (c.l.tt != T_RPAR) {
			pdie(c, "expected )");
		}
		feed(c.l);

		return mknode1(c, N_SIZEOF, n);
	}

	n = parse_primary(c);
	if (!n) {
		return 0:*node;
	}

	loop {
		if (c.l.tt == T_LSQ) {
			feed(c.l);

			b = parse_expr(c);

			if (c.l.tt != T_RSQ) {
				pdie(c, "expected ]");
			}
			feed(c.l);

			n = mknode(c, N_INDEX, n, b);
		} else if (c.l.tt == T_LPAR) {
			feed(c.l);

			b = parse_expr_list(c);

			if (c.l.tt != T_RPAR) {
				pdie(c, "expected )");
			}
			feed(c.l);

			n  = mknode(c, N_CALL, n, b);
		} else if (c.l.tt == T_DOT) {
			feed(c.l);

			b = parse_ident(c);
			if (!b) {
				pdie(c, "expected ident");
			}

			n = mknode(c, N_DOT, n, b);
		} else if (c.l.tt == T_COLON) {
			feed(c.l);

			b = parse_type(c);
			if (!b) {
				pdie(c, "expected type");
			}

			n = mknode(c, N_CAST, n, b);
		} else {
			return n;
		}
	}
}

// unary_expr := post_expr
//             | '&' unary_expr
//             | '*' unary_expr
//             | '+' unary_expr
//             | '-' unary_expr
//             | '~' unary_expr
//             | '!' unary_expr
parse_unary_expr(c: *parser): *node {
	var n: *node;

	if (c.l.tt == T_AMP) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_REF, n);
	}

	if (c.l.tt == T_STAR) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_DEREF, n);
	}

	if (c.l.tt == T_ADD) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_POS, n);
	}

	if (c.l.tt == T_SUB) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_NEG, n);
	}

	if (c.l.tt == T_NOT) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_NOT, n);
	}

	if (c.l.tt == T_BANG) {
		feed(c.l);

		n = parse_unary_expr(c);
		if (!n) {
			pdie(c, "expected unary_expr");
		}

		return mknode1(c, N_BNOT, n);
	}

	return parse_post_expr(c);
}


// shift_expr := unary_expr
//             | shift_expr '<<' unary_expr
//             | shift_expr '>>' unary_expr
parse_shift_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_unary_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.l.tt == T_LSH) {
			feed(c.l);

			b = parse_unary_expr(c);
			if (!b) {
				pdie(c, "expected unary_expr");
			}

			a = mknode(c, N_LSH, a, b);
		} else if (c.l.tt == T_RSH) {
			feed(c.l);

			b = parse_unary_expr(c);
			if (!b) {
				pdie(c, "expected unary_expr");
			}

			a = mknode(c, N_RSH, a, b);
		} else {
			return a;
		}
	}
}

// mul_expr := shift_expr
//           | mul_expr '*' shift_expr
//           | mul_expr '/' shift_expr
//           | mul_expr '%' shift_expr
//           | mul_expr '&' shift_expr
parse_mul_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_shift_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.l.tt == T_STAR) {
			feed(c.l);

			b = parse_shift_expr(c);
			if (!b) {
				pdie(c, "expected shift_expr");
			}

			a = mknode(c, N_MUL, a, b);
		} else if (c.l.tt == T_DIV) {
			feed(c.l);

			b = parse_shift_expr(c);
			if (!b) {
				pdie(c, "expected shift_expr");
			}

			a = mknode(c, N_DIV, a, b);
		} else if (c.l.tt == T_MOD) {
			feed(c.l);

			b = parse_shift_expr(c);
			if (!b) {
				pdie(c, "expected shift_expr");
			}

			a = mknode(c, N_MOD, a, b);
		} else if (c.l.tt == T_AMP) {
			feed(c.l);

			b = parse_shift_expr(c);
			if (!b) {
				pdie(c, "expected shift_expr");
			}

			a = mknode(c, N_AND, a, b);
		} else {
			return a;
		}
	}
}

// add_expr := mul_expr
//           | add_expr '+' mul_expr
//           | add_expr '-' mul_expr
//           | add_expr '|' mul_expr
//           | add_expr '^' mul_expr
parse_add_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_mul_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.l.tt == T_ADD) {
			feed(c.l);

			b = parse_mul_expr(c);
			if (!b) {
				pdie(c, "expected mul_expr");
			}

			a = mknode(c, N_ADD, a, b);
		} else if (c.l.tt == T_SUB) {
			feed(c.l);

			b = parse_mul_expr(c);
			if (!b) {
				pdie(c, "expected mul_expr");
			}

			a = mknode(c, N_SUB, a, b);
		} else if (c.l.tt == T_OR) {
			feed(c.l);

			b = parse_mul_expr(c);
			if (!b) {
				pdie(c, "expected mul_expr");
			}

			a = mknode(c, N_OR, a, b);
		} else if (c.l.tt == T_XOR) {
			feed(c.l);

			b = parse_mul_expr(c);
			if (!b) {
				pdie(c, "expected mul_expr");
			}

			a = mknode(c, N_XOR, a, b);
		} else {
			return a;
		}
	}
}

// comp_expr := add_expr
//            | add_expr '<' add_expr
//            | add_expr '>' add_expr
//            | add_expr '<=' add_expr
//            | add_expr '>=' add_expr
//            | add_expr '==' add_expr
//            | add_expr '!=' add_expr
parse_comp_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_add_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt == T_LT) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_LT, a, b);
	}

	if (c.l.tt == T_GT) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_GT, a, b);
	}

	if (c.l.tt == T_LE) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_LE, a, b);
	}

	if (c.l.tt == T_GE) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_GE, a, b);
	}

	if (c.l.tt == T_EQ) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_EQ, a, b);
	}

	if (c.l.tt == T_NE) {
		feed(c.l);

		b = parse_add_expr(c);
		if (!b) {
			pdie(c, "expected add_expr");
		}

		return mknode(c, N_NE, a, b);
	}

	return a;
}

// bool_expr := bool_expr
//            | bool_expr '&&' comp_expr
//            | bool_expr '||' comp_expr
parse_bool_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_comp_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt == T_BAND) {
		feed(c.l);

		b = parse_bool_expr(c);
		if (!b) {
			pdie(c, "expected bool_expr");
		}

		return mknode(c, N_BAND, a, b);
	}

	if (c.l.tt == T_BOR) {
		feed(c.l);

		b = parse_bool_expr(c);
		if (!b) {
			pdie(c, "expected bool_expr");
		}

		return mknode(c, N_BOR, a, b);
	}

	return a;
}

// expr := bool_expr
//       | bool_expr '=' expr
parse_expr(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_bool_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt == T_ASSIGN) {
		feed(c.l);

		b = parse_expr(c);
		if (!b) {
			pdie(c, "expected expr");
		}

		return mknode(c, N_ASSIGN, a, b);
	}

	return a;
}

// if_stmt := 'if' expr '{' stmt_list '}'
//          | 'if' expr '{' stmt_list '}' 'else' if_stmt
//          | 'if' expr '{' stmt_list '}' 'else' '{' stmt_list '}'
parse_if_stmt(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;
	var b: *node;

	if (c.l.tt != T_IDENT || strcmp(c.l.token, "if")) {
		return 0:*node;
	}
	feed(c.l);

	n = mknode0(c, N_CONDLIST);
	e = n;

	loop {
		a = parse_expr(c);
		if !a {
			pdie(c, "expected expr");
		}

		if (c.l.tt != T_LBRA) {
			pdie(c, "expected {");
		}
		feed(c.l);

		b = parse_stmt_list(c);

		if (c.l.tt != T_RBRA) {
			pdie(c, "expected }");
		}
		feed(c.l);

		e.a = mknode(c, N_COND, a, b);

		if (c.l.tt != T_IDENT || strcmp(c.l.token, "else")) {
			return n;
		}
		feed(c.l);

		if (c.l.tt == T_LBRA) {
			feed(c.l);

			b = parse_stmt_list(c);

			if (c.l.tt != T_RBRA) {
				pdie(c, "expected }");
			}
			feed(c.l);

			e.b = mknode1(c, N_CONDLIST, mknode(c, N_COND, 0:*node, b));

			return n;
		}

		if (c.l.tt != T_IDENT || strcmp(c.l.token, "if")) {
			pdie(c, "expected if");
		}
		feed(c.l);

		e.b = mknode0(c, N_CONDLIST);
		e = e.b;
	}
}

// loop_stmt := 'loop' '{' stmt_list '}'
parse_loop_stmt(c: *parser): *node {
	var a: *node;

	if (c.l.tt != T_IDENT || strcmp(c.l.token, "loop")) {
		return 0:*node;
	}
	feed(c.l);

	if (c.l.tt != T_LBRA) {
		pdie(c, "expected {");
	}
	feed(c.l);

	a = parse_stmt_list(c);

	if (c.l.tt != T_RBRA) {
		pdie(c, "expected }");
	}
	feed(c.l);

	return mknode1(c, N_LOOP, a);
}

// break_stmt := 'break'
parse_break_stmt(c: *parser): *node {
	if (c.l.tt != T_IDENT || strcmp(c.l.token, "break")) {
		return 0:*node;
	}
	feed(c.l);

	return mknode0(c, N_BREAK);
}

// continue_stmt := 'continue'
parse_continue_stmt(c: *parser): *node {
	if (c.l.tt != T_IDENT || strcmp(c.l.token, "continue")) {
		return 0:*node;
	}
	feed(c.l);

	return mknode0(c, N_CONTINUE);
}

// return_stmt := 'return'
//              | 'return' expr
parse_return_stmt(c: *parser): *node {
	var a: *node;

	if (c.l.tt != T_IDENT || strcmp(c.l.token, "return")) {
		return 0:*node;
	}
	feed(c.l);

	a = parse_expr(c);

	return mknode1(c, N_RETURN, a);
}

// var_decl := ident ':' type
parse_var_decl(c: *parser): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt != T_COLON) {
		pdie(c, "expected :");
	}
	feed(c.l);

	b = parse_type(c);
	if (!b) {
		pdie(c, "expected type");
	}

	return mknode(c, N_VARDECL, a, b);
}

// var_stmt := 'var' var_decl
parse_var_stmt(c: *parser): *node {
	var a: *node;

	if (c.l.tt != T_IDENT || strcmp(c.l.token, "var")) {
		return 0:*node;
	}
	feed(c.l);

	a = parse_var_decl(c);
	if (!a) {
		pdie(c, "expected var_decl");
	}

	return a;
}

// label_stmt := ':' ident
parse_label_stmt(c: *parser): *node {
	var a: *node;

	if (c.l.tt != T_COLON) {
		return 0:*node;
	}
	feed(c.l);

	a = parse_ident(c);
	if (!a) {
		pdie(c, "expected ident");
	}

	return mknode1(c, N_LABEL, a);
}

// goto_stmt := 'goto' ident
parse_goto_stmt(c: *parser): *node {
	var a: *node;

	if (c.l.tt != T_IDENT || strcmp(c.l.token, "goto")) {
		return 0:*node;
	}
	feed(c.l);

	a = parse_ident(c);
	if (!a) {
		pdie(c, "expected ident");
	}

	return mknode1(c, N_GOTO, a);
}

// stmt := if_stmt
//       | loop_stmt
//       | break_stmt ';'
//       | continue_stmt ';'
//       | return_stmt ';'
//       | var_stmt ';'
//       | label_stmt ';'
//       | goto_stmt ';'
//       | expr ';'
parse_stmt(c: *parser): *node {
	var n: *node;

	n = parse_if_stmt(c);
	if (n) {
		return n;
	}

	n = parse_loop_stmt(c);
	if (n) {
		return n;
	}

	n = parse_break_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_return_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_continue_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_var_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_label_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_goto_stmt(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	n = parse_expr(c);
	if (n) {
		if (c.l.tt != T_SEMI) {
			pdie(c, "expected ;");
		}
		feed(c.l);

		return n;
	}

	return 0:*node;
}

// stmt_list := stmt
//            | stmt stmt_list
parse_stmt_list(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;

	a = parse_stmt(c);
	if (!a) {
		return 0:*node;
	}

	e = mknode1(c, N_STMTLIST, a);
	n = e;

	loop {
		a = parse_stmt(c);
		if (!a) {
			return n;
		}

		e.b = mknode1(c, N_STMTLIST, a);
		e = e.b;
	}
}

// enum_item := ident
//            | ident '=' num
parse_enum_item(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt != T_ASSIGN) {
		return mknode1(c, N_ENUMITEM, a);
	}
	feed(c.l);

	b = parse_num(c);
	if (!b) {
		pdie(c, "expected num");
	}

	return mknode(c, N_ENUMITEM, a, b);
}

// enum_list := enum_item
//            | enum_list ',' enum_list
parse_enum_list(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;

	a = parse_enum_item(c);
	if (!a) {
		return 0:*node;
	}

	e = mknode1(c, N_ENUMLIST, a);
	n = e;

	loop {
		if (c.l.tt != T_COMMA) {
			return n;
		}
		feed(c.l);

		a = parse_enum_item(c);
		if (!a) {
			return n;
		}

		e.b = mknode1(c, N_ENUMLIST, a);
		e = e.b;
	}
}

// enum_decl := 'enum' ident '{' enum_list '}'
parse_enum_decl(c: *parser): *node {
	var b: *node;

	if (c.l.tt != T_IDENT) {
		return 0:*node;
	}

	if (strcmp(c.l.token, "enum")) {
		return 0:*node;
	}
	feed(c.l);

	if (c.l.tt != T_LBRA) {
		pdie(c, "expected {");
	}
	feed(c.l);

	b = parse_enum_list(c);

	if (c.l.tt != T_RBRA) {
		pdie(c, "expected }");
	}
	feed(c.l);

	return mknode(c, N_ENUM, 0:*node, b);
}

// type := ident
//       | '*' type
//       | '(' type ')'
//       | 'func' func_type
parse_type(c: *parser): *node {
	var n: *node;

	if (c.l.tt == T_STAR) {
		feed(c.l);

		n = parse_type(c);

		return mknode1(c, N_PTRTYPE, n);
	}

	if (c.l.tt == T_LPAR) {
		feed(c.l);

		n = parse_type(c);

		if (c.l.tt != T_RPAR) {
			pdie(c, "expected )");
		}
		feed(c.l);

		return n;
	}

	if (c.l.tt == T_IDENT && !strcmp(c.l.token, "func")) {
		feed(c.l);

		n = parse_func_type(c);
		if (!n) {
			pdie(c, "expected func_type");
		}

		return n;
	}

	return parse_ident(c);
}

// member_decl := ident ':' type
parse_member_decl(c: *parser): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0: *node;
	}

	if (c.l.tt != T_COLON) {
		pdie(c, "expected :");
	}
	feed(c.l);

	b = parse_type(c);
	if (!b) {
		pdie(c, "expected type");
	}

	return mknode(c, N_MEMBERDECL, a, b);
}

// member_list := member_decl
//              | member_decl ';' member_list
parse_member_list(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;

	a = parse_member_decl(c);
	if (!a) {
		return 0:*node;
	}

	e = mknode1(c, N_MEMBERLIST, a);
	n = e;

	loop {
		if (c.l.tt != T_SEMI) {
			return n;
		}
		feed(c.l);

		a = parse_member_decl(c);
		if (!a) {
			return n;
		}

		e.b = mknode1(c, N_MEMBERLIST, a);
		e = e.b;
	}
}

// struct_decl := 'struct' ident '{' member_list '}'
parse_struct_decl(c: *parser): *node {
	var a: *node;
	var b: *node;

	if (c.l.tt != T_IDENT) {
		return 0:*node;
	}

	if (strcmp(c.l.token, "struct")) {
		return 0:*node;
	}
	feed(c.l);

	a = parse_ident(c);
	if (!a) {
		pdie(c, "expected ident");
	}

	if (c.l.tt != T_LBRA) {
		pdie(c, "expected {");
	}
	feed(c.l);

	b = parse_member_list(c);

	if (c.l.tt != T_RBRA) {
		pdie(c, "expected }");
	}
	feed(c.l);

	return mknode(c, N_STRUCT, a, b);
}

// arg_decl := ':' type
//             ident ':' type
parse_arg_decl(c: *parser): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt != T_COLON) {
		pdie(c, "expected :");
	}
	feed(c.l);

	b = parse_type(c);
	if (!b) {
		pdie(c, "expected type");
	}

	return mknode(c, N_ARGDECL, a, b);
}

// arg_list := arg_decl
//           | arg_decl ',' arg_list
parse_arg_list(c: *parser): *node {
	var n: *node;
	var e: *node;
	var a: *node;

	a = parse_arg_decl(c);
	if (!a) {
		return 0:*node;
	}

	e = mknode1(c, N_ARGLIST, a);
	n = e;

	loop {
		if (c.l.tt != T_COMMA) {
			return n;
		}
		feed(c.l);

		a = parse_arg_decl(c);
		if (!a) {
			pdie(c, "expected identifier");
		}

		e.b = mknode1(c, N_ARGLIST, a);
		e = e.b;
	}
}

// func_type := '(' ')' ':' type
//            | '(' arg_list ')' ':' type
//            | '(' ')'
//            | '(' arg_list ')'
parse_func_type(c: *parser): *node {
	var a: *node;
	var b: *node;

	if (c.l.tt != T_LPAR) {
		return 0: *node;
	}
	feed(c.l);

	a = parse_arg_list(c);

	if (c.l.tt != T_RPAR) {
		pdie(c, "expected )");
	}
	feed(c.l);

	if (c.l.tt != T_COLON) {
		return mknode1(c, N_FUNCTYPE, a);
	}
	feed(c.l);

	b = parse_type(c);
	if (!b) {
		pdie(c, "expected type");
	}

	return mknode(c, N_FUNCTYPE, a, b);
}

// func_decl := ident func_type
parse_func_decl(c: *parser): *node {
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	b = parse_func_type(c);
	if (!b) {
		pdie(c, "expected func_type");
	}

	return mknode(c, N_FUNCDECL, a, b);
}

// func := func_decl '{' stmt_list '}'
//       | func_decl ';'
parse_func(c: *parser): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_func_decl(c);
	if (!a) {
		return 0:*node;
	}

	if (c.l.tt == T_SEMI) {
		feed(c.l);
		return a;
	}

	if (c.l.tt != T_LBRA) {
		pdie(c, "expected {");
	}
	feed(c.l);

	b = parse_stmt_list(c);

	if (c.l.tt != T_RBRA) {
		pdie(c, "expected }");
	}
	feed(c.l);

	return mknode(c, N_FUNC, a, b);
}

// decl := enum_decl
//       | struct_decl
//       | func
parse_decl(c: *parser): *node {
	var n: *node;

	n = parse_enum_decl(c);
	if (n) {
		return n;
	}

	n = parse_struct_decl(c);
	if (n) {
		return n;
	}

	return parse_func(c);
}

// program := decl
//          | decl program
parse_program(c: *parser): *node {
	var n: *node;
	var e: *node;
	var d: *node;

	d = parse_decl(c);
	if (!d) {
		if (c.l.tt != T_EOF) {
			pdie(c, "expected eof");
		}
		return 0:*node;
	}

	e = mknode1(c, N_PROGRAM, d);
	n = e;

	loop {
		d = parse_decl(c);
		if (!d) {
			if (c.l.tt != T_EOF) {
				pdie(c, "expected eof");
			}

			e.b = 0:*node;
			return n;
		}

		e.b = mknode1(c, N_PROGRAM, d);
		e = e.b;
	}
}

parse(c: *parser, filename: *byte): *node {
	var p: *node;
	open_source(c.l, filename);
	p = parse_program(c);
	close_source(c.l);
	return p;
}

fillpos(c: *parser, n: *node) {
	n.filename = c.l.filename;
	n.lineno = c.l.lineno;
	n.colno = c.l.colno;
}
