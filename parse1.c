struct node {
	kind: int;
	a: *node;
	b: *node;
	filename: *byte;
	lineno: int;
	colno: int;
	n: int;
	s: *byte;
	t: *type;
}

enum {
	N_IDENT,
	N_NUM,
	N_CHAR,
	N_STR,
	N_STMTLIST,
	N_EXPRLIST,
	N_CALL,
	N_DOT,
	N_ARGLIST,
	N_FUNC,
	N_ARGDECL,
	N_FUNCDECL,
	N_PROGRAM,
	N_FUNCTYPE,
	N_PTRTYPE,
	N_STRUCT,
	N_MEMBERDECL,
	N_MEMBERLIST,
	N_CONDLIST,
	N_COND,
	N_ENUM,
	N_ENUMITEM,
	N_ENUMLIST,
	N_LOOP,
	N_BREAK,
	N_CONTINUE,
	N_RETURN,
	N_VARDECL,
	N_LABEL,
	N_GOTO,
	N_ASSIGN,
	N_SIZEOF,
	N_REF,
	N_DEREF,
	N_CAST,
	N_INDEX,
	N_LT,
	N_GT,
	N_LE,
	N_GE,
	N_EQ,
	N_NE,
	N_ADD,
	N_SUB,
	N_MUL,
	N_LSH,
	N_RSH,
	N_BNOT,
	N_BOR,
	N_BAND,
	N_AND,
	N_OR,
	N_XOR,
	N_NOT,
	N_POS,
	N_NEG,
	N_DIV,
	N_MOD,
}

mknode(c: *compiler, kind: int, a: *node, b: *node): *node {
	var ret: *node;
	ret = alloc(c.a, sizeof(*ret)):*node;
	ret.kind = kind;
	ret.a = a;
	ret.b = b;
	ret.filename = c.filename;
	ret.lineno = c.lineno;
	ret.colno = c.colno;
	ret.n = 0;
	ret.s = 0:*byte;
	ret.t = 0:*type;
	return ret;
}

mknode0(c: *compiler, kind: int): *node {
	return mknode(c, kind, 0:*node, 0:*node);
}

mknode1(c: *compiler, kind: int, a: *node): *node {
	return mknode(c, kind, a, 0:*node);
}

// Copy the current token
intern(c: *compiler): *byte {
	var ret: *byte;
	var i: int;

	ret = alloc(c.a, c.tlen + 1);

	i = 0;
	loop {
		if (i == c.tlen) {
			ret[i] = 0:byte;
			return ret;
		}

		ret[i] = c.token[i];

		i = i + 1;
	}
}

// ident := IDENT
parse_ident(c: *compiler): *node {
	var n: *node;

	if (c.tt != T_IDENT) {
		return 0:*node;
	}

	n = mknode0(c, N_IDENT);
	n.s = intern(c);
	feed(c);

	return n;
}

parse_hex(c: *compiler): *node {
	var n: *node;
	var x: int;
	var d: int;
	var i: int;

	x = 0;
	i = 0;
	loop {
		if (i == c.tlen) {
			break;
		}

		d = c.token[i]:int;

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
			cdie(c, "overflow");
		}
	}

	n = mknode0(c, N_NUM);
	n.n = x;
	feed(c);

	return n;
}

// num := NUM
parse_num(c: *compiler): *node {
	var n: *node;
	var x: int;
	var d: int;
	var i: int;

	if (c.tt == T_HEX) {
		return parse_hex(c);
	}

	if (c.tt != T_NUM) {
		return 0:*node;
	}

	x = 0;
	i = 0;
	loop {
		if (i == c.tlen) {
			break;
		}

		d = (c.token[i]:int) - '0';

		x = x * 10;

		x = x + d;
		i = i + 1;

		if (x > 0x7fffffff) {
			cdie(c, "overflow");
		}
	}

	n = mknode0(c, N_NUM);
	n.n = x;
	feed(c);

	return n;
}

// chr := CHAR
parse_chr(c: *compiler): *node {
	var n: *node;

	if (c.tt != T_CHAR) {
		return 0:*node;
	}

	n = mknode0(c, N_CHAR);
	n.n = c.token[0]:int;
	feed(c);

	return n;
}

// str := STR
parse_str(c: *compiler): *node {
	var n: *node;

	if (c.tt != T_STR) {
		return 0:*node;
	}

	n = mknode0(c, N_STR);
	n.s = intern(c);
	feed(c);

	return n;
}

// primary := ident
//          | num
//          | str
//          | chr
//          | '(' expr ')'
parse_primary(c: *compiler): *node {
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

	if (c.tt == T_LPAR) {
		feed(c);

		n = parse_expr(c);
		if (!n) {
			cdie(c, "expected expr");
		}

		if (c.tt != T_RPAR) {
			cdie(c, "expected )");
		}
		feed(c);

		return n;
	}

	return 0:*node;
}

// expr_list := expr
//            | expr ',' expr_list
parse_expr_list(c: *compiler): *node {
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
		if (c.tt != T_COMMA) {
			return n;
		}
		feed(c);

		a = parse_expr(c);
		if (!a) {
			cdie(c, "expected expr");
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
parse_post_expr(c: *compiler): *node {
	var n: *node;
	var b: *node;

	if (c.tt == T_IDENT && !strcmp(c.token, "sizeof")) {
		feed(c);

		if (c.tt != T_LPAR) {
			cdie(c, "expected (");
		}
		feed(c);

		n = parse_expr(c);
		if (!n) {
			cdie(c, "expected expr");
		}

		if (c.tt != T_RPAR) {
			cdie(c, "expected )");
		}
		feed(c);

		return mknode1(c, N_SIZEOF, n);
	}

	n = parse_primary(c);
	if (!n) {
		return 0:*node;
	}

	loop {
		if (c.tt == T_LSQ) {
			feed(c);

			b = parse_expr(c);

			if (c.tt != T_RSQ) {
				cdie(c, "expected ]");
			}
			feed(c);

			n = mknode(c, N_INDEX, n, b);
		} else if (c.tt == T_LPAR) {
			feed(c);

			b = parse_expr_list(c);

			if (c.tt != T_RPAR) {
				cdie(c, "expected )");
			}
			feed(c);

			n  = mknode(c, N_CALL, n, b);
		} else if (c.tt == T_DOT) {
			feed(c);

			b = parse_ident(c);
			if (!b) {
				cdie(c, "expected ident");
			}

			n = mknode(c, N_DOT, n, b);
		} else if (c.tt == T_COLON) {
			feed(c);

			b = parse_type(c);
			if (!b) {
				cdie(c, "expected type");
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
parse_unary_expr(c: *compiler): *node {
	var n: *node;

	if (c.tt == T_AMP) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_REF, n);
	}

	if (c.tt == T_STAR) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_DEREF, n);
	}

	if (c.tt == T_ADD) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_POS, n);
	}

	if (c.tt == T_SUB) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_NEG, n);
	}

	if (c.tt == T_NOT) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_NOT, n);
	}

	if (c.tt == T_BANG) {
		feed(c);

		n = parse_unary_expr(c);
		if (!n) {
			cdie(c, "expected unary_expr");
		}

		return mknode1(c, N_BNOT, n);
	}

	return parse_post_expr(c);
}


// shift_expr := unary_expr
//             | shift_expr '<<' unary_expr
//             | shift_expr '>>' unary_expr
parse_shift_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_unary_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.tt == T_LSH) {
			feed(c);

			b = parse_unary_expr(c);
			if (!b) {
				cdie(c, "expected unary_expr");
			}

			a = mknode(c, N_LSH, a, b);
		} else if (c.tt == T_RSH) {
			feed(c);

			b = parse_unary_expr(c);
			if (!b) {
				cdie(c, "expected unary_expr");
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
parse_mul_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_shift_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.tt == T_STAR) {
			feed(c);

			b = parse_shift_expr(c);
			if (!b) {
				cdie(c, "expected shift_expr");
			}

			a = mknode(c, N_MUL, a, b);
		} else if (c.tt == T_DIV) {
			feed(c);

			b = parse_shift_expr(c);
			if (!b) {
				cdie(c, "expected shift_expr");
			}

			a = mknode(c, N_DIV, a, b);
		} else if (c.tt == T_MOD) {
			feed(c);

			b = parse_shift_expr(c);
			if (!b) {
				cdie(c, "expected shift_expr");
			}

			a = mknode(c, N_MOD, a, b);
		} else if (c.tt == T_AMP) {
			feed(c);

			b = parse_shift_expr(c);
			if (!b) {
				cdie(c, "expected shift_expr");
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
parse_add_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_mul_expr(c);
	if (!a) {
		return 0:*node;
	}

	loop {
		if (c.tt == T_ADD) {
			feed(c);

			b = parse_mul_expr(c);
			if (!b) {
				cdie(c, "expected mul_expr");
			}

			a = mknode(c, N_ADD, a, b);
		} else if (c.tt == T_SUB) {
			feed(c);

			b = parse_mul_expr(c);
			if (!b) {
				cdie(c, "expected mul_expr");
			}

			a = mknode(c, N_SUB, a, b);
		} else if (c.tt == T_OR) {
			feed(c);

			b = parse_mul_expr(c);
			if (!b) {
				cdie(c, "expected mul_expr");
			}

			a = mknode(c, N_OR, a, b);
		} else if (c.tt == T_XOR) {
			feed(c);

			b = parse_mul_expr(c);
			if (!b) {
				cdie(c, "expected mul_expr");
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
parse_comp_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_add_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt == T_LT) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_LT, a, b);
	}

	if (c.tt == T_GT) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_GT, a, b);
	}

	if (c.tt == T_LE) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_LE, a, b);
	}

	if (c.tt == T_GE) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_GE, a, b);
	}

	if (c.tt == T_EQ) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_EQ, a, b);
	}

	if (c.tt == T_NE) {
		feed(c);

		b = parse_add_expr(c);
		if (!b) {
			cdie(c, "expected add_expr");
		}

		return mknode(c, N_NE, a, b);
	}

	return a;
}

// bool_expr := bool_expr
//            | bool_expr '&&' comp_expr
//            | bool_expr '||' comp_expr
parse_bool_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_comp_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt == T_BAND) {
		feed(c);

		b = parse_bool_expr(c);
		if (!b) {
			cdie(c, "expected bool_expr");
		}

		return mknode(c, N_BAND, a, b);
	}

	if (c.tt == T_BOR) {
		feed(c);

		b = parse_bool_expr(c);
		if (!b) {
			cdie(c, "expected bool_expr");
		}

		return mknode(c, N_BOR, a, b);
	}

	return a;
}

// expr := bool_expr
//       | bool_expr '=' expr
parse_expr(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_bool_expr(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt == T_ASSIGN) {
		feed(c);

		b = parse_expr(c);
		if (!b) {
			cdie(c, "expected expr");
		}

		return mknode(c, N_ASSIGN, a, b);
	}

	return a;
}

// if_stmt := 'if' expr '{' stmt_list '}'
//          | 'if' expr '{' stmt_list '}' 'else' if_stmt
//          | 'if' expr '{' stmt_list '}' 'else' '{' stmt_list '}'
parse_if_stmt(c: *compiler): *node {
	var n: *node;
	var e: *node;
	var a: *node;
	var b: *node;

	if (c.tt != T_IDENT || strcmp(c.token, "if")) {
		return 0:*node;
	}
	feed(c);

	n = mknode0(c, N_CONDLIST);
	e = n;

	loop {
		a = parse_expr(c);
		if !a {
			cdie(c, "expected expr");
		}

		if (c.tt != T_LBRA) {
			cdie(c, "expected {");
		}
		feed(c);

		b = parse_stmt_list(c);

		if (c.tt != T_RBRA) {
			cdie(c, "expected }");
		}
		feed(c);

		e.a = mknode(c, N_COND, a, b);

		if (c.tt != T_IDENT || strcmp(c.token, "else")) {
			return n;
		}
		feed(c);

		if (c.tt == T_LBRA) {
			feed(c);

			b = parse_stmt_list(c);

			if (c.tt != T_RBRA) {
				cdie(c, "expected }");
			}
			feed(c);

			e.b = mknode1(c, N_CONDLIST, mknode(c, N_COND, 0:*node, b));

			return n;
		}

		if (c.tt != T_IDENT || strcmp(c.token, "if")) {
			cdie(c, "expected if");
		}
		feed(c);

		e.b = mknode0(c, N_CONDLIST);
		e = e.b;
	}
}

// loop_stmt := 'loop' '{' stmt_list '}'
parse_loop_stmt(c: *compiler): *node {
	var a: *node;

	if (c.tt != T_IDENT || strcmp(c.token, "loop")) {
		return 0:*node;
	}
	feed(c);

	if (c.tt != T_LBRA) {
		cdie(c, "expected {");
	}
	feed(c);

	a = parse_stmt_list(c);

	if (c.tt != T_RBRA) {
		cdie(c, "expected }");
	}
	feed(c);

	return mknode1(c, N_LOOP, a);
}

// break_stmt := 'break'
parse_break_stmt(c: *compiler): *node {
	if (c.tt != T_IDENT || strcmp(c.token, "break")) {
		return 0:*node;
	}
	feed(c);

	return mknode0(c, N_BREAK);
}

// continue_stmt := 'continue'
parse_continue_stmt(c: *compiler): *node {
	if (c.tt != T_IDENT || strcmp(c.token, "continue")) {
		return 0:*node;
	}
	feed(c);

	return mknode0(c, N_CONTINUE);
}

// return_stmt := 'return'
//              | 'return' expr
parse_return_stmt(c: *compiler): *node {
	var a: *node;

	if (c.tt != T_IDENT || strcmp(c.token, "return")) {
		return 0:*node;
	}
	feed(c);

	a = parse_expr(c);

	return mknode1(c, N_RETURN, a);
}

// var_decl := ident ':' type
parse_var_decl(c: *compiler): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt != T_COLON) {
		cdie(c, "expected :");
	}
	feed(c);

	b = parse_type(c);
	if (!b) {
		cdie(c, "expected type");
	}

	return mknode(c, N_VARDECL, a, b);
}

// var_stmt := 'var' var_decl
parse_var_stmt(c: *compiler): *node {
	var a: *node;

	if (c.tt != T_IDENT || strcmp(c.token, "var")) {
		return 0:*node;
	}
	feed(c);

	a = parse_var_decl(c);
	if (!a) {
		cdie(c, "expected var_decl");
	}

	return a;
}

// label_stmt := ':' ident
parse_label_stmt(c: *compiler): *node {
	var a: *node;

	if (c.tt != T_COLON) {
		return 0:*node;
	}
	feed(c);

	a = parse_ident(c);
	if (!a) {
		cdie(c, "expected ident");
	}

	return mknode1(c, N_LABEL, a);
}

// goto_stmt := 'goto' ident
parse_goto_stmt(c: *compiler): *node {
	var a: *node;

	if (c.tt != T_IDENT || strcmp(c.token, "goto")) {
		return 0:*node;
	}
	feed(c);

	a = parse_ident(c);
	if (!a) {
		cdie(c, "expected ident");
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
parse_stmt(c: *compiler): *node {
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
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_return_stmt(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_continue_stmt(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_var_stmt(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_label_stmt(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_goto_stmt(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	n = parse_expr(c);
	if (n) {
		if (c.tt != T_SEMI) {
			cdie(c, "expected ;");
		}
		feed(c);

		return n;
	}

	return 0:*node;
}

// stmt_list := stmt
//            | stmt stmt_list
parse_stmt_list(c: *compiler): *node {
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
parse_enum_item(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt != T_ASSIGN) {
		return mknode1(c, N_ENUMITEM, a);
	}
	feed(c);

	b = parse_num(c);
	if (!b) {
		cdie(c, "expected num");
	}

	return mknode(c, N_ENUMITEM, a, b);
}

// enum_list := enum_item
//            | enum_list ',' enum_list
parse_enum_list(c: *compiler): *node {
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
		if (c.tt != T_COMMA) {
			return n;
		}
		feed(c);

		a = parse_enum_item(c);
		if (!a) {
			return n;
		}

		e.b = mknode1(c, N_ENUMLIST, a);
		e = e.b;
	}
}

// enum_decl := 'enum' ident '{' enum_list '}'
parse_enum_decl(c: *compiler): *node {
	var b: *node;

	if (c.tt != T_IDENT) {
		return 0:*node;
	}

	if (strcmp(c.token, "enum")) {
		return 0:*node;
	}
	feed(c);

	if (c.tt != T_LBRA) {
		cdie(c, "expected {");
	}
	feed(c);

	b = parse_enum_list(c);

	if (c.tt != T_RBRA) {
		cdie(c, "expected }");
	}
	feed(c);

	return mknode(c, N_ENUM, 0:*node, b);
}

// type := ident
//       | '*' type
//       | '(' type ')'
//       | 'func' func_type
parse_type(c: *compiler): *node {
	var n: *node;

	if (c.tt == T_STAR) {
		feed(c);

		n = parse_type(c);

		return mknode1(c, N_PTRTYPE, n);
	}

	if (c.tt == T_LPAR) {
		feed(c);

		n = parse_type(c);

		if (c.tt != T_RPAR) {
			cdie(c, "expected )");
		}
		feed(c);

		return n;
	}

	if (c.tt == T_IDENT && !strcmp(c.token, "func")) {
		feed(c);

		n = parse_func_type(c);
		if (!n) {
			cdie(c, "expected func_type");
		}

		return n;
	}

	return parse_ident(c);
}

// member_decl := ident ':' type
parse_member_decl(c: *compiler): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0: *node;
	}

	if (c.tt != T_COLON) {
		cdie(c, "expected :");
	}
	feed(c);

	b = parse_type(c);
	if (!b) {
		cdie(c, "expected type");
	}

	return mknode(c, N_MEMBERDECL, a, b);
}

// member_list := member_decl
//              | member_decl ';' member_list
parse_member_list(c: *compiler): *node {
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
		if (c.tt != T_SEMI) {
			return n;
		}
		feed(c);

		a = parse_member_decl(c);
		if (!a) {
			return n;
		}

		e.b = mknode1(c, N_MEMBERLIST, a);
		e = e.b;
	}
}

// struct_decl := 'struct' ident '{' member_list '}'
parse_struct_decl(c: *compiler): *node {
	var a: *node;
	var b: *node;

	if (c.tt != T_IDENT) {
		return 0:*node;
	}

	if (strcmp(c.token, "struct")) {
		return 0:*node;
	}
	feed(c);

	a = parse_ident(c);
	if (!a) {
		cdie(c, "expected ident");
	}

	if (c.tt != T_LBRA) {
		cdie(c, "expected {");
	}
	feed(c);

	b = parse_member_list(c);

	if (c.tt != T_RBRA) {
		cdie(c, "expected }");
	}
	feed(c);

	return mknode(c, N_STRUCT, a, b);
}

// arg_decl := ':' type
//             ident ':' type
parse_arg_decl(c: *compiler): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt != T_COLON) {
		cdie(c, "expected :");
	}
	feed(c);

	b = parse_type(c);
	if (!b) {
		cdie(c, "expected type");
	}

	return mknode(c, N_ARGDECL, a, b);
}

// arg_list := arg_decl
//           | arg_decl ',' arg_list
parse_arg_list(c: *compiler): *node {
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
		if (c.tt != T_COMMA) {
			return n;
		}
		feed(c);

		a = parse_arg_decl(c);
		if (!a) {
			cdie(c, "expected identifier");
		}

		e.b = mknode1(c, N_ARGLIST, a);
		e = e.b;
	}
}

// func_type := '(' ')' ':' type
//            | '(' arg_list ')' ':' type
//            | '(' ')'
//            | '(' arg_list ')'
parse_func_type(c: *compiler): *node {
	var a: *node;
	var b: *node;

	if (c.tt != T_LPAR) {
		return 0: *node;
	}
	feed(c);

	a = parse_arg_list(c);

	if (c.tt != T_RPAR) {
		cdie(c, "expected )");
	}
	feed(c);

	if (c.tt != T_COLON) {
		return mknode1(c, N_FUNCTYPE, a);
	}
	feed(c);

	b = parse_type(c);
	if (!b) {
		cdie(c, "expected type");
	}

	return mknode(c, N_FUNCTYPE, a, b);
}

// func_decl := ident func_type
parse_func_decl(c: *compiler): *node {
	var a: *node;
	var b: *node;

	a = parse_ident(c);
	if (!a) {
		return 0:*node;
	}

	b = parse_func_type(c);
	if (!b) {
		cdie(c, "expected func_type");
	}

	return mknode(c, N_FUNCDECL, a, b);
}

// func := func_decl '{' stmt_list '}'
//       | func_decl ';'
parse_func(c: *compiler): *node {
	var n: *node;
	var a: *node;
	var b: *node;

	a = parse_func_decl(c);
	if (!a) {
		return 0:*node;
	}

	if (c.tt == T_SEMI) {
		feed(c);
		return a;
	}

	if (c.tt != T_LBRA) {
		cdie(c, "expected {");
	}
	feed(c);

	b = parse_stmt_list(c);

	if (c.tt != T_RBRA) {
		cdie(c, "expected }");
	}
	feed(c);

	return mknode(c, N_FUNC, a, b);
}

// decl := enum_decl
//       | struct_decl
//       | func
parse_decl(c: *compiler): *node {
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
parse_program(c: *compiler, p: *node): *node {
	var n: *node;
	var e: *node;
	var d: *node;

	d = parse_decl(c);
	if (!d) {
		if (c.tt != T_EOF) {
			cdie(c, "expected eof");
		}
		return p;
	}

	e = mknode1(c, N_PROGRAM, d);
	n = e;

	loop {
		d = parse_decl(c);
		if (!d) {
			if (c.tt != T_EOF) {
				cdie(c, "expected eof");
			}

			e.b = p;
			return n;
		}

		e.b = mknode1(c, N_PROGRAM, d);
		e = e.b;
	}
}
