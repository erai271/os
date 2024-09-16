struct parser {
	a: *alloc;
	p: *peg;
}

setup_parser(a: *alloc): *parser {
	var c: *parser;

	c = alloc(a, sizeof(*c)):*parser;

	c.a = a;

	c.p = peg_new("", "", 0, a);

	return c;
}

parse(c: *parser, filename: *byte): *node {
	var f: *file;
	var fd: int;
	var len: int;
	var src: *byte;
	var pn: *peg_node;

	fd = open(filename, 0, 0);
	if fd < 0 {
		fdputs(2, "failed to open ");
		fdputs(2, filename);
		fdputs(2, "\n");
		exit(1);
	}

	f = fopen(fd, c.a);
	src = freadall(f, &len);
	fclose(f);

	peg_reset(c.p, filename, src, len);
	pn = peg_parse(c.p, P_sp);

	return reconstruct(c, pn);
}

fillpos(c: *parser, n: *node) {
}

copypos(n: *node, pn: *peg_node) {
	n.filename = pn.filename;
	n.lineno = pn.line;
	n.colno = pn.col;
}

reconstruct(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var link: **node;
	var n: *node;
	var p: *node;
	assert(pn.tag == P_grammar, "grammar");

	ret = 0:*node;
	link = &ret;

	pn = pn.child;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_enum_decl {
			n = reconstruct_enum(c, pn);
		} else if pn.tag == P_struct_decl {
			n = reconstruct_struct(c, pn);
		} else if pn.tag == P_func_decl {
			n = reconstruct_func(c, pn);
		} else {
			die("invalid decl");
		}

		p = mknode1(c, N_PROGRAM, n);
		copypos(p, pn);

		*link = p;
		link = &p.b;

		pn = pn.next;
	}

	return ret;
}

reconstruct_intern(c: *parser, pn: *peg_node, s: *byte, len: int): *node {
	var n: *node;
	var i: int;
	n = mknode0(c, N_IDENT);
	copypos(n, pn);
	i = 0;
	n.s = alloc(c.a, len + 1);
	loop {
		if i == len {
			break;
		}
		n.s[i] = s[i];
		i = i + 1;
	}
	n.s[i] = 0:byte;
	return n;
}

reconstruct_ident(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_ident, "ident");
	return reconstruct_intern(c, pn, pn.str, pn.len);
}

reconstruct_enum_item(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_enum_item, "enum_item");

	a = reconstruct_ident(c, pn.child);
	b = 0:*node;

	if pn.child.next {
		b = reconstruct_expr(c, pn.child.next);
		assert(b.kind == N_NUM, "enum must be num");
	}

	n = mknode(c, N_ENUMITEM, a, b);
	copypos(n, pn);

	return n;
}

reconstruct_enum(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var link: **node;
	var n: *node;
	assert(pn.tag == P_enum_decl, "enum_decl");

	n = mknode(c, N_ENUM, 0:*node, 0:*node);
	copypos(n, pn);

	pn = pn.child.next;
	link = &n.b;
	loop {
		if !pn {
			break;
		}

		a = mknode1(c, N_ENUMLIST, reconstruct_enum_item(c, pn));
		copypos(a, pn);
		*link = a;
		link = &a.b;

		pn = pn.next;
	}

	return n;
}

reconstruct_arg_decl(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_arg_decl, "arg_decl");

	a = reconstruct_ident(c, pn.child);
	b = reconstruct_type(c, pn.child.next);

	n = mknode(c, N_ARGDECL, a, b);
	copypos(n, pn);

	return n;
}

reconstruct_func_type(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	var link: **node;
	assert(pn.tag == P_func_type, "func_type");

	n = mknode(c, N_FUNCTYPE, 0:*node, 0:*node);
	copypos(n, pn);

	pn = pn.child;
	link = &n.a;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_arg_decl {
			a = mknode1(c, N_ARGLIST, reconstruct_arg_decl(c, pn));
			copypos(a, pn);
			*link = a;
			link = &a.b;
		} else if pn.tag == P_type {
			n.b = reconstruct_type(c, pn);
		} else {
			die("invalid func_type");
		}

		pn = pn.next;
	}

	return n;
}

reconstruct_type(c: *parser, pn: *peg_node): *node {
	var n: *node;
	assert(pn.tag == P_type, "type");
	pn = pn.child;
	if pn.tag == P_ident {
		return reconstruct_ident(c, pn);
	} else if pn.tag == P_byte {
		return reconstruct_intern(c, pn, "byte", 4);
	} else if pn.tag == P_int {
		return reconstruct_intern(c, pn, "int", 3);
	} else if pn.tag == P_void {
		return reconstruct_intern(c, pn, "void", 4);
	} else if pn.tag == P_ptr_type {
		n = mknode1(c, N_PTRTYPE, reconstruct_type(c, pn.child));
		copypos(n, pn);
		return n;
	} else if pn.tag == P_func {
		return reconstruct_func_type(c, pn.next);
	} else if pn.tag == P_type {
		return reconstruct_type(c, pn);
	} else {
		die("reconstruct_type: invalid type");
	}
}

reconstruct_member_decl(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_member_decl, "member_decl");

	a = reconstruct_ident(c, pn.child);
	b = reconstruct_type(c, pn.child.next);

	n = mknode(c, N_MEMBERDECL, a, b);
	copypos(n, pn);

	return n;
}

reconstruct_member_list(c: *parser, pn: *peg_node): *node {
	var n: *node;
	var a: *node;
	var ret: *node;
	var link: **node;

	ret = 0:*node;
	link = &ret;
	loop {
		if !pn {
			break;
		}

		a = reconstruct_member_decl(c, pn);

		n = mknode1(c, N_MEMBERLIST, a);
		copypos(n, pn);

		*link = n;
		link = &n.b;

		pn = pn.next;
	}

	return ret;
}

reconstruct_struct(c: *parser, pn: *peg_node): *node {
	var n: *node;
	var a: *node;
	var b: *node;
	assert(pn.tag == P_struct_decl, "struct_def");
	a = reconstruct_ident(c, pn.child.next);
	b = reconstruct_member_list(c, pn.child.next.next);
	n = mknode(c, N_STRUCT, a, b);
	copypos(n, pn);
	return n;
}

reconstruct_expr(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_expr, "expr");
	return reconstruct_bool(c, pn.child);
}

reconstruct_bool(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var link: **node;
	var kind: int;
	assert(pn.tag == P_bool_expr, "bool_expr");

	ret = reconstruct_comp(c, pn.child);

	link = &ret;
	pn = pn.child.next;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_band_op {
			kind = N_BAND;
		} else if pn.tag == P_bor_op {
			kind = N_BOR;
		} else {
			die("invalid boolop");
		}

		n = reconstruct_comp(c, pn.next);
		*link = mknode(c, kind, *link, n);
		copypos(*link, pn);
		link = &(*link).b;

		pn = pn.next.next;
	}

	return ret;
}

reconstruct_comp(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	var kind: int;
	var tag: int;
	assert(pn.tag == P_comp_expr, "comp_expr");

	a = reconstruct_add(c, pn.child);
	if !pn.child.next {
		return a;
	}

	tag = pn.child.next.tag;
	if tag == P_le_op {
		kind = N_LE;
	} else if tag == P_ge_op {
		kind = N_GE;
	} else if tag == P_lt_op {
		kind = N_LT;
	} else if tag == P_gt_op {
		kind = N_GT;
	} else if tag == P_eq_op {
		kind = N_EQ;
	} else if tag == P_ne_op {
		kind = N_NE;
	} else {
		die("invalid compop");
	}

	b = reconstruct_add(c, pn.child.next.next);

	n = mknode(c, kind, a, b);
	copypos(n, pn);

	return n;
}

reconstruct_add(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var link: **node;
	var kind: int;
	assert(pn.tag == P_add_expr, "add_expr");

	ret = reconstruct_mul(c, pn.child);

	link = &ret;
	pn = pn.child.next;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_add_op {
			kind = N_ADD;
		} else if pn.tag == P_sub_op {
			kind = N_SUB;
		} else if pn.tag == P_or_op {
			kind = N_OR;
		} else if pn.tag == P_xor_op {
			kind = N_XOR;
		} else {
			die("invalid addop");
		}

		n = reconstruct_mul(c, pn.next);
		*link = mknode(c, kind, *link, n);
		copypos(*link, pn);

		pn = pn.next.next;
	}

	return ret;
}

reconstruct_mul(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var link: **node;
	var kind: int;
	assert(pn.tag == P_mul_expr, "mul_expr");

	ret = reconstruct_shift(c, pn.child);

	link = &ret;
	pn = pn.child.next;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_mul_op {
			kind = N_MUL;
		} else if pn.tag == P_div_op {
			kind = N_DIV;
		} else if pn.tag == P_mod_op {
			kind = N_MOD;
		} else if pn.tag == P_and_op {
			kind = N_AND;
		} else {
			die("invalid mulop");
		}

		n = reconstruct_shift(c, pn.next);
		*link = mknode(c, kind, *link, n);
		copypos(*link, pn);

		pn = pn.next.next;
	}

	return ret;
}

reconstruct_shift(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var link: **node;
	var kind: int;
	assert(pn.tag == P_shift_expr, "shift_expr");

	ret = reconstruct_unary(c, pn.child);

	link = &ret;
	pn = pn.child.next;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_lsh_op {
			kind = N_LSH;
		} else if pn.tag == P_rsh_op {
			kind = N_RSH;
		} else {
			die("invalid shift");
		}

		n = reconstruct_unary(c, pn.next);
		*link = mknode(c, kind, *link, n);
		copypos(*link, pn);

		pn = pn.next.next;
	}

	return ret;
}

reconstruct_unary(c: *parser, pn: *peg_node): *node {
	var n: *node;
	var ret: *node;
	var link: **node;
	var tag: int;
	var kind: int;
	assert(pn.tag == P_unary_expr, "unary_expr");

	link = &ret;
	pn = pn.child;
	loop {
		tag = pn.tag;
		if tag == P_ref_op {
			kind = N_REF;
		} else if tag == P_deref_op {
			kind = N_DEREF;
		} else if tag == P_pos_op {
			kind = N_POS;
		} else if tag == P_neg_op {
			kind = N_NEG;
		} else if tag == P_not_op {
			kind = N_NOT;
		} else if tag == P_bnot_op {
			kind = N_BNOT;
		} else if tag == P_post_expr {
			*link = reconstruct_post(c, pn);
			return ret;
		} else {
			die("invalid unary_expr");
		}

		n = mknode0(c, kind);
		copypos(n, pn);
		*link = n;
		link = &n.a;

		pn = pn.next;
	}
}

reconstruct_args(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var link: **node;
	var a: *node;
	var n: *node;

	ret = 0:*node;
	link = &ret;
	loop {
		if !pn {
			break;
		}

		a = reconstruct_expr(c, pn);
		n = mknode1(c, N_EXPRLIST, a);
		copypos(n, pn);

		*link = n;
		link = &n.b;

		pn = pn.next;
	}

	return ret;
}

reconstruct_post(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var tag: int;
	assert(pn.tag == P_post_expr, "post_expr");

	ret = reconstruct_primary(c, pn.child);

	pn = pn.child.next;
	loop {
		if !pn {
			break;
		}

		tag = pn.tag;

		if tag == P_index_expr {
			n = reconstruct_expr(c, pn.child);
			ret = mknode(c, N_INDEX, ret, n);
			copypos(ret, pn);
		} else if tag == P_call_expr {
			n = reconstruct_args(c, pn.child);
			ret = mknode(c, N_CALL, ret, n);
			copypos(ret, pn);
		} else if tag == P_member_expr {
			n = reconstruct_ident(c, pn.child);
			ret = mknode(c, N_DOT, ret, n);
			copypos(ret, pn);
		} else if tag == P_cast_expr {
			n = reconstruct_type(c, pn.child);
			ret = mknode(c, N_CAST, ret, n);
			copypos(ret, pn);
		} else {
			die("invalid post_expr");
		}

		pn = pn.next;
	}

	return ret;
}

reconstruct_primary(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_primary, "primary");
	pn = pn.child;
	if pn.tag == P_ident {
		return reconstruct_ident(c, pn);
	} else if pn.tag == P_hex {
		return reconstruct_hex(c, pn);
	} else if pn.tag == P_dec {
		return reconstruct_dec(c, pn);
	} else if pn.tag == P_str {
		return reconstruct_str(c, pn);
	} else if pn.tag == P_char {
		return reconstruct_char(c, pn);
	} else if pn.tag == P_sizeof_expr {
		return reconstruct_sizeof(c, pn);
	} else if pn.tag == P_expr {
		return reconstruct_expr(c, pn);
	} else {
		die("invalid primary");
	}
}

reconstruct_hex(c: *parser, pn: *peg_node): *node {
	var x: int;
	var n: *node;
	var ok: int;
	assert(pn.tag == P_hex, "hex");
	x = hex2int(&pn.str[2], pn.len - 2, &ok);
	if !ok {
		die("invalid hex");
	}
	n = mknode0(c, N_NUM);
	copypos(n, pn);
	n.n = x;
	return n;
}

reconstruct_dec(c: *parser, pn: *peg_node): *node {
	var x: int;
	var n: *node;
	var ok: int;
	assert(pn.tag == P_dec, "dec");
	x = dec2int(pn.str, pn.len, &ok);
	if !ok {
		die("invalid dec");
	}
	n = mknode0(c, N_NUM);
	copypos(n, pn);
	n.n = x;
	return n;
}

reconstruct_str(c: *parser, pn: *peg_node): *node {
	var i: int;
	var j: int;
	var len: int;
	var ch: int;
	var ok: int;
	var s: *byte;
	var n: *node;
	assert(pn.tag == P_str, "str");
	s = alloc(c.a, pn.len);
	i = 1;
	j = 0;
	len = pn.len - 1;
	loop {
		if i == len {
			break;
		}
		ch = unescape(pn.str, &i, len, &ok);
		if !ok {
			die("invalid escape");
		}
		s[j] = ch:byte;
		j = j + 1;
	}
	s[j] = 0:byte;
	n = mknode0(c, N_STR);
	copypos(n, pn);
	n.s = s;
	return n;
}

reconstruct_char(c: *parser, pn: *peg_node): *node {
	var i: int;
	var x: int;
	var ok: int;
	var n: *node;
	assert(pn.tag == P_char, "char");
	i = 1;
	x = unescape(pn.str, &i, pn.len - 1, &ok);
	if !ok || i != pn.len - 1 {
		die("invalid character");
	}
	n = mknode0(c, N_CHAR);
	copypos(n, pn);
	n.n = x;
	return n;
}
reconstruct_sizeof(c: *parser, pn: *peg_node): *node {
	var n: *node;
	var a: *node;
	assert(pn.tag == P_sizeof_expr, "sizeof_expr");
	a = reconstruct_expr(c, pn.child.next);
	n = mknode1(c, N_SIZEOF, a);
	copypos(n, pn);
	return n;
}

reconstruct_stmt(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_stmt, "stmt");
	pn = pn.child;
	if pn.tag == P_if_stmt {
		return reconstruct_if(c, pn);
	} else if pn.tag == P_loop_stmt {
		return reconstruct_loop(c, pn);
	} else if pn.tag == P_break_stmt {
		return reconstruct_break(c, pn);
	} else if pn.tag == P_continue_stmt {
		return reconstruct_continue(c, pn);
	} else if pn.tag == P_return_stmt {
		return reconstruct_return(c, pn);
	} else if pn.tag == P_var_stmt {
		return reconstruct_var(c, pn);
	} else if pn.tag == P_label_stmt {
		return reconstruct_label(c, pn);
	} else if pn.tag == P_goto_stmt {
		return reconstruct_goto(c, pn);
	} else if pn.tag == P_assign_stmt {
		return reconstruct_assign(c, pn);
	} else if pn.tag == P_expr_stmt {
		return reconstruct_expr_stmt(c, pn);
	} else if pn.tag == P_empty_stmt {
		return reconstruct_empty(c, pn);
	} else if pn.tag == P_compound_stmt {
		return reconstruct_compound(c, pn);
	} else {
		die("invalid stmt");
	}
}

reconstruct_if(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var link: **node;
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_if_stmt, "if_stmt");

	a = reconstruct_expr(c, pn.child.next);
	b = reconstruct_compound(c, pn.child.next.next);
	ret = mknode1(c, N_CONDLIST, mknode(c, N_COND, a, b));
	copypos(ret, pn);
	copypos(ret.a, pn);

	link = &ret.b;
	pn = pn.child.next.next.next;
	loop {
		if !pn {
			break;
		}

		if pn.tag == P_elif_stmt {
			a = reconstruct_expr(c, pn.child.next.next);
			b = reconstruct_compound(c, pn.child.next.next.next);
			n = mknode1(c, N_CONDLIST, mknode(c, N_COND, a, b));
			copypos(n, pn);
			copypos(n.a, pn);
			*link = n;
			link = &n.b;
		} else if pn.tag == P_else_stmt {
			a = 0:*node;
			b = reconstruct_compound(c, pn.child.next);
			n = mknode1(c, N_CONDLIST, mknode(c, N_COND, a, b));
			copypos(n, pn);
			copypos(n.a, pn);
			*link = n;
			link = &n.b;
			break;
		} else {
			die("invalid if_stmt");
		}

		pn = pn.next;
	}

	return ret;
}

reconstruct_loop(c: *parser, pn: *peg_node): *node {
	var n: *node;
	var a: *node;
	assert(pn.tag == P_loop_stmt, "loop_stmt");
	a = reconstruct_compound(c, pn.child.next);
	n = mknode1(c, N_LOOP, a);
	copypos(n, pn);
	return n;
}

reconstruct_break(c: *parser, pn: *peg_node): *node {
	var n: *node;
	assert(pn.tag == P_break_stmt, "break_stmt");
	n = mknode0(c, N_BREAK);
	copypos(n, pn);
	return n;
}

reconstruct_continue(c: *parser, pn: *peg_node): *node {
	var n: *node;
	assert(pn.tag == P_continue_stmt, "continue_stmt");
	n = mknode0(c, N_CONTINUE);
	copypos(n, pn);
	return n;
}

reconstruct_return(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var n: *node;
	assert(pn.tag == P_return_stmt, "return_stmt");
	a = 0:*node;
	if pn.child.next {
		a = reconstruct_expr(c, pn.child.next);
	}
	n = mknode1(c, N_RETURN, a);
	copypos(n, pn);
	return n;
}

reconstruct_var(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_var_stmt, "var_stmt");
	a = reconstruct_ident(c, pn.child.next);
	b = reconstruct_type(c, pn.child.next.next);
	n = mknode(c, N_VARDECL, a, b);
	copypos(n, pn);
	return n;
}

reconstruct_label(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var n: *node;
	assert(pn.tag == P_label_stmt, "label_stmt");
	a = reconstruct_ident(c, pn.child);
	n = mknode1(c, N_LABEL, a);
	copypos(n, pn);
	return n;
}

reconstruct_goto(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var n: *node;
	assert(pn.tag == P_goto_stmt, "goto_stmt");
	a = reconstruct_ident(c, pn.child.next);
	n = mknode1(c, N_GOTO, a);
	copypos(n, pn);
	return n;
}

reconstruct_assign(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var n: *node;
	assert(pn.tag == P_assign_stmt, "assign_stmt");
	a = reconstruct_unary(c, pn.child);
	b = reconstruct_expr(c, pn.child.next);
	n = mknode(c, N_ASSIGN, a, b);
	copypos(n, pn);
	return n;
}

reconstruct_expr_stmt(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_expr_stmt, "expr_stmt");
	return reconstruct_expr(c, pn.child);
}

reconstruct_empty(c: *parser, pn: *peg_node): *node {
	assert(pn.tag == P_empty_stmt, "empty_stmt");
	return 0:*node;
}

reconstruct_compound(c: *parser, pn: *peg_node): *node {
	var ret: *node;
	var n: *node;
	var a: *node;
	var link: **node;
	assert(pn.tag == P_compound_stmt, "compound_stmt");
	pn = pn.child;
	ret = 0:*node;
	link = &ret;
	loop {
		if ! pn {
			break;
		}

		a = reconstruct_stmt(c, pn);
		if a {
			n = mknode1(c, N_STMTLIST, a);
			copypos(n, pn);
			*link = n;
			link = &n.b;
		}

		pn = pn.next;
	}
	return ret;
}

reconstruct_func(c: *parser, pn: *peg_node): *node {
	var a: *node;
	var b: *node;
	var d: *node;
	var n: *node;
	assert(pn.tag == P_func_decl, "func_def");

	a = reconstruct_ident(c, pn.child);
	b = reconstruct_func_type(c, pn.child.next);

	d = mknode(c, N_FUNCDECL, a, b);
	copypos(d, pn);

	if !pn.child.next.next {
		return d;
	}

	b = reconstruct_compound(c, pn.child.next.next);

	n = mknode(c, N_FUNC, d, b);
	copypos(n, pn);

	return n;
}
