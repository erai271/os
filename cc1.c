struct compiler {
	// Allocator
	a: *alloc;

	// Parser
	p: *parser;

	// Context
	filename: *byte;
	lineno: int;
	colno: int;

	// Assembler
	as: *assembler;

	// Namespace
	decls: *decl;

	// C Output
	do_cout: int;
	cout: *file;
}

cshow_context(c: *compiler) {
	fdputs(2, "on ");
	if (c.filename) {
		fdputs(2, c.filename);
	}
	fdputs(2, ":");
	fdputd(2, c.lineno);
	fdputs(2, ":");
	fdputd(2, c.colno);
	fdputs(2, "\n");
}

cdie(c: *compiler, msg: *byte) {
	cshow_context(c);
	fdputs(2, "cdie: ");
	fdputs(2, msg);
	fdputs(2, "\n");
	exit(1);
}

comp_setup(a: *alloc): *compiler {
	var c: *compiler;

	c = alloc(a, sizeof(*c)): *compiler;

	c.a = a;

	c.p = setup_parser(a);

	c.filename = 0:*byte;
	c.lineno = 0;
	c.colno = 0;

	c.as = setup_assembler(a);

	c.decls = 0:*decl;

	c.do_cout = 0;
	c.cout = 0:*file;

	return c;
}

open_coutput(c: *compiler, filename: *byte) {
	var fd: int;

	if (c.cout) {
		die("multiple output files");
	}

	unlink(filename);

	fd = open(filename, O_CREAT | O_WRONLY, (6 << 6) + (6 << 3) + 6);
	if (fd < 0) {
		die("failed to open output");
	}

	c.cout = fopen(fd, c.a);
}

flush_coutput(c: *compiler) {
	fflush(c.cout);
}

compile(c: *compiler, p: *node) {
	var n: *node;
	var d: *decl;
	var kind: int;

	// Process enum and struct declarations
	n = p;
	loop {
		if (!n) {
			break;
		}

		kind = n.a.kind;
		if (kind == N_STRUCT) {
			defstruct(c, n.a);
		} else if (kind == N_ENUM) {
			defenum(c, n.a);
		} else if (kind != N_FUNC && kind != N_FUNCDECL) {
			cdie(c, "invalid decl");
		}

		n = n.b;
	}

	// Process function declarations
	n = p;
	loop {
		if (!n) {
			break;
		}

		kind = n.a.kind;
		if (kind == N_FUNCDECL) {
			defextern(c, n.a);
		} else if (kind == N_FUNC) {
			defun(c, n.a);
		}

		n = n.b;
	}

	// Allocate structs
	d = first_decl(c);
	loop {
		if (!d) {
			break;
		}

		if (d.struct_defined) {
			layout_struct(c, d);
		}

		d = next_decl(c, d);
	}

	// Compile functions
	d = first_decl(c);
	loop {
		if (!d) {
			break;
		}

		if (d.func_defined) {
			compile_func(c, d);
		}

		d = next_decl(c, d);
	}
}

defextern(c: *compiler, n: *node): *decl {
	var d: *decl;
	var name: *byte;
	var t: *type;

	name = n.a.s;

	t = prototype(c, n.b);

	d = find(c, name, 0:*byte, 1);

	if (d.func_defined) {
		cdie(c, "duplicate function");
	}

	d.func_defined = 1;
	d.func_type = t;
	d.func_decl = n;

	return d;
}

defun(c: *compiler, n: *node) {
	var d: *decl;

	d = defextern(c, n.a);

	d.func_def = n;
}

defstruct(c: *compiler, n: *node) {
	var name: *byte;
	var d: *decl;

	name = n.a.s;

	if (!strcmp(name, "int") || !strcmp(name, "byte") || !strcmp(name, "func")) {
		cdie(c, "reserved word");
	}

	d = find(c, name, 0:*byte, 1);

	if (d.struct_defined) {
		cdie(c, "duplicate struct");
	}

	d.struct_defined = 1;
	d.struct_def = n;
}

defenum(c: *compiler, n: *node) {
	var d: *decl;
	var i: int;
	var name: *byte;

	n = n.b;
	i = 0;
	loop {
		if (!n) {
			break;
		}

		name = n.a.a.s;
		d = find(c, name, 0:*byte, 1);

		if (d.enum_defined) {
			cdie(c, "duplicate enum");
		}

		if (n.a.b) {
			i = n.a.b.n;
		}

		d.enum_defined = 1;
		d.enum_value = i;
		d.enum_def = n;

		i = i + 1;
		n = n.b;
	}
}

layout_struct(c: *compiler, d: *decl) {
	var m: *node;
	var offset: int;
	var name: *byte;
	var md: *decl;
	var t: *type;

	if (d.struct_layout_done) {
		if (d.struct_layout_done == 2) {
			cdie(c, "circular struct definition");
		}

		return;
	}

	d.struct_layout_done = 2;

	m = d.struct_def.b;
	offset = 0;
	loop {
		if (!m) {
			break;
		}

		name = m.a.a.s;
		t = prototype(c, m.a.b);

		md = find(c, d.name, name, 1);

		if (d.member_defined) {
			cdie(c, "duplicate member");
		}

		md.member_defined = 1;
		md.member_type = t;
		md.member_offset = offset;
		md.member_def = m;

		offset = offset + type_sizeof(c, t);

		m = m.b;
	}

	d.struct_size = offset;
	d.struct_layout_done = 1;
}

compile_func(c: *compiler, d: *decl) {
	var name: *byte;
	var v: *decl;
	var t: *type;
	var offset: int;
	var n: *node;
	var pragma: int;

	if (!d.func_def) {
		return;
	}

	n = d.func_def.a.b.a;
	offset = 16;
	loop {
		if (!n) {
			break;
		}

		name = n.a.a.s;
		t = prototype(c, n.a.b);

		v = find(c, d.name, name, 1);
		if (v.var_defined) {
			cdie(c, "duplicate argument");
		}

		v.var_defined = 1;
		v.var_type = t;
		v.var_offset = offset;
		v.var_def = n.a;

		offset = offset + 8;
		n = n.b;
	}

	// Hoist locals
	offset = hoist_locals(c, d, d.func_def.b, 0);

	if (!strcmp(d.name, "_start")) {
		pragma = 1;
	} else if (!strcmp(d.name, "_kstart")) {
		pragma = 2;
	} else {
		pragma = 0;
	}

	// Compile the function body
	emit_str(c.as, d.name);
	fixup_label(c.as, d.func_label);
	emit_preamble(c.as, offset, pragma);
	compile_stmt(c, d, d.func_def.b, 0:*label, 0:*label);
	emit_num(c.as, 0);

	if (pragma) {
		emit_ud(c.as);
	}

	emit_ret(c.as);
}

hoist_locals(c: *compiler, d: *decl, n: *node, offset: int): int {
	var kind: int;
	var name: *byte;
	var t: *type;
	var v: *decl;

	if (!n) {
		return offset;
	}

	kind = n.kind;
	if (kind == N_CONDLIST) {
		loop {
			if (!n) {
				return offset;
			}

			hoist_locals(c, d, n.a.b, offset);

			n = n.b;
		}
	} else if (kind == N_STMTLIST) {
		loop {
			if (!n) {
				return offset;
			}

			offset = hoist_locals(c, d, n.a, offset);

			n = n.b;
		}
	} else if (kind == N_LOOP) {
		return hoist_locals(c, d, n.a, offset);
	} else if (kind == N_LABEL) {
		name = n.a.s;
		v = find(c, d.name, name, 1);

		if (v.goto_defined) {
			cdie(c, "duplicate goto");
		}
		v.goto_defined = 1;

		return offset;
	} else if (kind != N_VARDECL) {
		return offset;
	}

	name = n.a.s;
	t = prototype(c, n.b);

	n.t = t;

	v = find(c, d.name, name, 1);

	if (v.var_defined) {
		cdie(c, "duplicate variable");
	}

	v.var_type = t;
	v.var_defined = 1;

	offset = offset + type_sizeof(c, t);

	v.var_offset = -offset;

	return offset;
}

compile_include(c: *compiler, n: *node) {
	var filename: *byte;
	var fd: int;
	var blob: *byte;
	var len: int;

	if n.b.a.kind != N_STR {
		die("non literal include");
	}

	filename = n.b.a.s;

	fd = open(filename, O_RDONLY, 0);
	if fd < 0 {
		die("failed to open include");
	}

	blob = readall(fd, &len, c.a);

	close(fd);

	as_opr(c.as, OP_POPR, R_RAX);
	as_opr(c.as, OP_POPR, R_RDI);
	as_opri64(c.as, OP_MOVABS, R_RAX, len);
	as_modrm(c.as, OP_STORE, R_RAX, R_RDI, 0, 0, 0);
	emit_blob(c.as, blob, len);

	free(c.a, blob);
}

// Translate an expression
compile_expr(c: *compiler, d: *decl, n: *node, rhs: int) {
	var no: *label;
	var out: *label;
	var v: *decl;
	var kind: int;

	c.filename = n.filename;
	c.lineno = n.lineno;
	c.colno = n.colno;

	kind = n.kind;
	if (kind == N_STR) {
		if (!rhs) {
			cdie(c, "str is not an lexpr");
		}

		emit_str(c.as, n.s);

		n.t = mktype1(c, TY_PTR, mktype0(c, TY_BYTE));
	} else if (kind == N_NUM) {
		if (!rhs) {
			cdie(c, "num is not an lexpr");
		}

		emit_num(c.as, n.n);

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_CHAR) {
		if (!rhs) {
			cdie(c, "char is not an lexpr");
		}

		emit_num(c.as, n.n);

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_EXPRLIST) {
		if (!rhs) {
			cdie(c, "call is not an lexpr");
		}

		if (n.b) {
			compile_expr(c, d, n.b, 1);
		}

		compile_expr(c, d, n.a, 1);

		if (n.b) {
			n.t = mktype2(c, TY_ARG, n.a.t, n.b.t);
		} else {
			n.t = mktype1(c, TY_ARG, n.a.t);
		}
	} else if (kind == N_CALL) {
		if (!rhs) {
			cdie(c, "call is not an lexpr");
		}

		if (n.b) {
			compile_expr(c, d, n.b, 1);
		}

		if (n.a.kind == N_IDENT) {
			v = find(c, n.a.s, 0:*byte, 0);
			if (v && v.enum_defined) {
				cdie(c, "type error");
			}

			v = find(c, d.name, n.a.s, 0);
			if (v && v.var_defined) {
				emit_lea(c.as, v.var_offset);
				n.a.t = v.var_type;
				emit_load(c.as, n.a.t);
				emit_call(c.as, count_args(c, n.a.t.arg));
			} else if !strcmp(n.a.s, "_include") {
				v = find(c, n.a.s, 0:*byte, 0);
				if (!v || !v.func_defined) {
					cdie(c, "no such function");
				}
				n.a.t = v.func_type;
				compile_include(c, n);
			} else {
				v = find(c, n.a.s, 0:*byte, 0);
				if (!v || !v.func_defined) {
					cdie(c, "no such function");
				}
				n.a.t = v.func_type;
				emit_lcall(c.as, v.func_label, count_args(c, n.a.t.arg));
			}
		} else {
			compile_expr(c, d, n.a, 1);
			emit_call(c.as, count_args(c, n.a.t.arg));
		}

		if (n.a.t.kind != TY_FUNC) {
			cdie(c, "calling not a function");
		}

		if (n.b) {
			unify(c, n.a.t.arg, n.b.t);
		} else {
			unify(c, n.a.t.arg, 0: *type);
		}

		n.t = n.a.t.val;
	} else if (kind == N_DOT) {
		compile_expr(c, d, n.a, 0);

		if (n.a.t.kind == TY_PTR) {
			if (n.a.t.val.kind != TY_STRUCT) {
				cdie(c, "dot not a struct");
			}

			v = find(c, n.a.t.val.st.name, n.b.s, 0);

			emit_load(c.as, n.a.t);
		} else {
			if (n.a.t.kind != TY_STRUCT) {
				cdie(c, "dot not a struct");
			}

			v = find(c, n.a.t.st.name, n.b.s, 0);
		}

		if (!v || !v.member_defined) {
			cdie(c, "no such member");
		}

		emit_num(c.as, v.member_offset);
		emit_add(c.as);

		n.t = v.member_type;

		if (rhs) {
			emit_load(c.as, n.t);
		}
	} else if (kind == N_IDENT) {
		v = find(c, n.s, 0:*byte, 0);
		if (v && v.enum_defined) {
			emit_num(c.as, v.enum_value);
			n.t = mktype0(c, TY_INT);
			return;
		}

		v = find(c, d.name, n.s, 0);
		if (v && v.var_defined) {
			emit_lea(c.as, v.var_offset);
			n.t = v.var_type;
			if (rhs) {
				emit_load(c.as, n.t);
			}
			return;
		}

		v = find(c, n.s, 0:*byte, 0);
		if (v && v.func_defined) {
			emit_ptr(c.as, v.func_label);
			n.t = v.func_type;
			return;
		}

		cdie(c, "no such variable");
	} else if (kind == N_ASSIGN) {
		if (!rhs) {
			cdie(c, "assign is not an lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 0);

		unify(c, n.a.t, n.b.t);

		n.t = n.a.t;

		emit_store(c.as, n.t);
	} else if (kind == N_SIZEOF) {
		if (!rhs) {
			cdie(c, "sizeof is not an lexpr");
		}

		out = mklabel(c.as);

		emit_jmp(c.as, out);

		compile_expr(c, d, n.a, 0);

		fixup_label(c.as, out);

		if (n.a.t.kind == TY_BYTE) {
			emit_num(c.as, 1);
		} else {
			emit_num(c.as, type_sizeof(c, n.a.t));
		}

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_REF) {
		if (!rhs) {
			cdie(c, "ref is not an lexpr");
		}

		compile_expr(c, d, n.a, 0);

		n.t = mktype1(c, TY_PTR, n.a.t);
	} else if (kind == N_DEREF) {
		compile_expr(c, d, n.a, 1);

		if (n.a.t.kind != TY_PTR) {
			cdie(c, "deref not a pointer");
		}

		n.t = n.a.t.val;

		if (rhs) {
			emit_load(c.as, n.t);
		}
	} else if (kind == N_INDEX) {
		compile_expr(c, d, n.a, 1);
		compile_expr(c, d, n.b, 1);

		if (n.a.t.kind != TY_PTR) {
			cdie(c, "not a pointer");
		}

		if (!type_isint(n.b.t)) {
			cdie(c, "index: not an int");
		}

		n.t = n.a.t.val;

		if (n.t.kind == TY_BYTE) {
			emit_num(c.as, 1);
		} else {
			emit_num(c.as, type_sizeof(c, n.t));
		}

		emit_mul(c.as);
		emit_add(c.as);

		if (rhs) {
			emit_load(c.as, n.t);
		}
	} else if (kind == N_LT) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_lt(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "lt: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_GT) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_gt(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "gt: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_LE) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_le(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "le: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_GE) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_ge(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "ge: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_EQ) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_eq(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "eq: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_NE) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_ne(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isprim(n.a.t)) {
			cdie(c, "ne: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_BNOT) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		no = mklabel(c.as);
		out = mklabel(c.as);

		compile_expr(c, d, n.a, 1);

		emit_jz(c.as, no);
		emit_num(c.as, 0);
		emit_jmp(c.as, out);
		fixup_label(c.as, no);
		emit_num(c.as, 1);
		fixup_label(c.as, out);

		if (!type_isprim(n.a.t)) {
			cdie(c, "not an prim");
		}

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_BOR) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		no = mklabel(c.as);
		out = mklabel(c.as);

		compile_expr(c, d, n.a, 1);
		emit_jz(c.as, no);
		emit_num(c.as, 1);
		emit_jmp(c.as, out);

		fixup_label(c.as, no);
		no = mklabel(c.as);

		compile_expr(c, d, n.b, 1);
		emit_jz(c.as, no);
		emit_num(c.as, 1);
		emit_jmp(c.as, out);

		fixup_label(c.as, no);
		emit_num(c.as, 0);

		fixup_label(c.as, out);

		if (!type_isprim(n.a.t)) {
			cdie(c, "not an prim");
		}

		if (!type_isprim(n.b.t)) {
			cdie(c, "not an prim");
		}

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_BAND) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		no = mklabel(c.as);
		out = mklabel(c.as);

		compile_expr(c, d, n.a, 1);
		emit_jz(c.as, no);

		compile_expr(c, d, n.b, 1);
		emit_jz(c.as, no);

		emit_num(c.as, 1);
		emit_jmp(c.as, out);

		fixup_label(c.as, no);
		emit_num(c.as, 0);

		fixup_label(c.as, out);

		if (!type_isprim(n.a.t)) {
			cdie(c, "not an prim");
		}

		if (!type_isprim(n.b.t)) {
			cdie(c, "not an prim");
		}

		n.t = mktype0(c, TY_INT);
	} else if (kind == N_POS) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.a, 1);

		if (!type_isint(n.a.t)) {
			cdie(c, "pos: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_NEG) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.a, 1);
		emit_neg(c.as);

		if (!type_isint(n.a.t)) {
			cdie(c, "neg: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_NOT) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.a, 1);
		emit_not(c.as);

		if (!type_isint(n.a.t)) {
			cdie(c, "not: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_ADD) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_add(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "add: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_SUB) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_sub(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "sub: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_MUL) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_mul(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "mul: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_DIV) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_div(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "div: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_MOD) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_mod(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "mod: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_LSH) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_lsh(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "lsh: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_RSH) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_rsh(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "rsh: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_AND) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_and(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "and: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_OR) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_or(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "or: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_XOR) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.b, 1);
		compile_expr(c, d, n.a, 1);
		emit_xor(c.as);

		unify(c, n.a.t, n.b.t);

		if (!type_isint(n.a.t)) {
			cdie(c, "xor: not an int");
		}

		n.t = n.a.t;
	} else if (kind == N_CAST) {
		if (!rhs) {
			cdie(c, "not lexpr");
		}

		compile_expr(c, d, n.a, 1);
		if (!type_isprim(n.a.t)) {
			cdie(c, "not a primitive");
		}

		n.t = prototype(c, n.b);
	} else {
		cdie(c, "not an expression");
	}
}

// Compile a statement
compile_stmt(c: *compiler, d: *decl, n: *node, top: *label, out: *label) {
	var no: *label;
	var ifout: *label;
	var v: *decl;
	var kind: int;

	if (!n) {
		return;
	}

	c.filename = n.filename;
	c.lineno = n.lineno;
	c.colno = n.colno;

	kind = n.kind;
	if (kind == N_CONDLIST) {
		ifout = mklabel(c.as);
		no = 0: *label;
		loop {
			if (no) {
				fixup_label(c.as, no);
			}

			if (!n) {
				break;
			}

			no = mklabel(c.as);

			if (n.a.a) {
				compile_expr(c, d, n.a.a, 1);
				emit_jz(c.as, no);
			}

			compile_stmt(c, d, n.a.b, top, out);
			emit_jmp(c.as, ifout);

			n = n.b;
		}
		fixup_label(c.as, ifout);
	} else if (kind == N_STMTLIST) {
		loop {
			if (!n) {
				break;
			}
			compile_stmt(c, d, n.a, top, out);
			n = n.b;
		}
	} else if (kind == N_LOOP) {
		top = mklabel(c.as);
		out = mklabel(c.as);
		fixup_label(c.as, top);
		compile_stmt(c, d, n.a, top, out);
		emit_jmp(c.as, top);
		fixup_label(c.as, out);
	} else if (kind == N_BREAK) {
		if (!out) {
			cdie(c, "break outside loop");
		}
		emit_jmp(c.as, out);
	} else if (kind == N_CONTINUE) {
		if (!top) {
			cdie(c, "continue outside loop");
		}
		emit_jmp(c.as, top);
	} else if (kind == N_RETURN) {
		if (n.a) {
			if (d.func_type.val.kind == TY_VOID) {
				cdie(c, "returning a value in a void function");
			}
			compile_expr(c, d, n.a, 1);
			unify(c, n.a.t, d.func_type.val);
		} else {
			if (d.func_type.val.kind != TY_VOID) {
				cdie(c, "returning void in a non void function");
			}
			emit_num(c.as, 0);
		}
		emit_ret(c.as);
	} else if (kind == N_LABEL) {
		v = find(c, d.name, n.a.s, 0);
		fixup_label(c.as, v.goto_label);
	} else if (kind == N_GOTO) {
		v = find(c, d.name, n.a.s, 0);
		if (!v || !v.goto_defined) {
			cdie(c, "label not defined");
		}
		emit_jmp(c.as, v.goto_label);
	} else if (kind != N_VARDECL) {
		compile_expr(c, d, n, 1);
		emit_pop(c.as, 1);
	}
}

emit_ssr(c: *compiler) {
	var d: *decl;
	var v: *decl;

	v = find(c, "global", "_save", 0);
	if (!v || !v.member_defined) {
		cdie(c, "no _save");
	}

	as_emit(c.as, OP_GS);
	as_modra(c.as, OP_STORE, R_RSP, v.member_offset);

	v = find(c, "global", "curtask", 0);
	if (!v || !v.member_defined) {
		cdie(c, "no global.curtask");
	}

	as_emit(c.as, OP_GS);
	as_modra(c.as, OP_LOAD, R_RSP, v.member_offset);

	v = find(c, "task", "stack", 0);
	if (!v || !v.member_defined) {
		cdie(c, "no task.stack");
	}

	as_modrm(c.as, OP_LOAD, R_RSP, R_RSP, 0, 0, v.member_offset);
	as_modri(c.as, OP_ADDI, R_RSP, 4096 - 176);

	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 0);
	as_modrm(c.as, OP_STORE, R_RDX, R_RSP, 0, 0, 16);
	as_modrm(c.as, OP_STORE, R_RBX, R_RSP, 0, 0, 24);
	as_modrm(c.as, OP_STORE, R_RBP, R_RSP, 0, 0, 40);
	as_modrm(c.as, OP_STORE, R_RSI, R_RSP, 0, 0, 48);
	as_modrm(c.as, OP_STORE, R_RDI, R_RSP, 0, 0, 56);
	as_modrm(c.as, OP_STORE, R_R8, R_RSP, 0, 0, 64);
	as_modrm(c.as, OP_STORE, R_R9, R_RSP, 0, 0, 72);
	as_modrm(c.as, OP_STORE, R_R10, R_RSP, 0, 0, 80);
	as_modrm(c.as, OP_STORE, R_R12, R_RSP, 0, 0, 96);
	as_modrm(c.as, OP_STORE, R_R13, R_RSP, 0, 0, 104);
	as_modrm(c.as, OP_STORE, R_R14, R_RSP, 0, 0, 112);
	as_modrm(c.as, OP_STORE, R_R15, R_RSP, 0, 0, 120);
	as_modrm(c.as, OP_STORE, R_RCX, R_RSP, 0, 0, 128);
	as_modrm(c.as, OP_STORE, R_R11, R_RSP, 0, 0, 136);

	as_modrr(c.as, OP_XORRM, R_RAX, R_RAX);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 8);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 88);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 160);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 168);

	as_modri(c.as, OP_MOVI, R_RAX, 43);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 144);
	as_modri(c.as, OP_MOVI, R_RAX, 35);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 152);

	v = find(c, "global", "_save", 0);
	if (!v || !v.member_defined) {
		cdie(c, "no _save");
	}

	as_emit(c.as, OP_GS);
	as_modra(c.as, OP_LOAD, R_RAX, v.member_offset);
	as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 32);
	as_modrr(c.as, OP_MOVE, R_RAX, R_RSP);

	as_modrr(c.as, OP_XORRM, R_RBP, R_RBP);
	as_opr(c.as, OP_PUSHR, R_RBP);
	as_opr(c.as, OP_PUSHR, R_RBP);
	as_modrr(c.as, OP_MOVE, R_RBP, R_RSP);

	as_opr(c.as, OP_PUSHR, R_RAX);

	d = find(c, "_ssr", 0:*byte, 1);
	if (d.func_defined && d.func_label.fixed) {
		as_jmp(c.as, OP_CALL, d.func_label);
	}

	as_op(c.as, OP_CLI);

	as_modri(c.as, OP_ADDI, R_RSP, 3 * 8);

	as_modrm(c.as, OP_LOAD, R_RAX, R_RSP, 0, 0, 0);
	as_modrm(c.as, OP_LOAD, R_RDX, R_RSP, 0, 0, 16);
	as_modrm(c.as, OP_LOAD, R_RBX, R_RSP, 0, 0, 24);
	as_modrm(c.as, OP_LOAD, R_RBP, R_RSP, 0, 0, 40);
	as_modrm(c.as, OP_LOAD, R_RSI, R_RSP, 0, 0, 48);
	as_modrm(c.as, OP_LOAD, R_RDI, R_RSP, 0, 0, 56);
	as_modrm(c.as, OP_LOAD, R_R8, R_RSP, 0, 0, 64);
	as_modrm(c.as, OP_LOAD, R_R9, R_RSP, 0, 0, 72);
	as_modrm(c.as, OP_LOAD, R_R10, R_RSP, 0, 0, 80);
	as_modrm(c.as, OP_LOAD, R_R12, R_RSP, 0, 0, 96);
	as_modrm(c.as, OP_LOAD, R_R13, R_RSP, 0, 0, 104);
	as_modrm(c.as, OP_LOAD, R_R14, R_RSP, 0, 0, 112);
	as_modrm(c.as, OP_LOAD, R_R15, R_RSP, 0, 0, 120);

	as_modrm(c.as, OP_LOAD, R_RCX, R_RSP, 0, 0, 128);
	as_modrm(c.as, OP_LOAD, R_R11, R_RSP, 0, 0, 136);

	as_modrm(c.as, OP_LOAD, R_RSP, R_RSP, 0, 0, 32);

	as_rex(c.as, OP_SYSRET, 0, 0, 0);
	as_op(c.as, OP_SYSRET);
}

emit_isr(c: *compiler) {
	var d: *decl;
	var out: *label;
	var i: int;

	out = mklabel(c.as);

	i = 0;
	loop {
		if i == 256 {
			break;
		}
		reserve(c.as, 16);
		if i == 8 || i == 10 || i == 11 || i == 12
				|| i == 13 || i == 14 || i == 17 || i == 21
				|| i == 29 || i == 30 {
			as_emit(c.as, 0x90);
			as_emit(c.as, 0x90);
		} else {
			as_emit(c.as, 0x6a);
			as_emit(c.as, 0x00);
		}
		as_emit(c.as, 0x68);
		as_emit(c.as, i);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0xe9);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0x00);
		as_emit(c.as, 0x00);
		addfixup(c.as, out);
		as_emit(c.as, 0x90);
		as_emit(c.as, 0x90);
		as_emit(c.as, 0x90);
		as_emit(c.as, 0x90);
		i = i + 1;
	}

	fixup_label(c.as, out);

	// save regs
	as_modri(c.as, OP_SUBI, R_RSP, 176);
	as_modrm(c.as, OP_STORE, R_RBP, R_RSP, 0, 0, 40);
	as_modrr(c.as, OP_MOVE, R_RBP, R_RSP);
	as_opr(c.as, OP_PUSHR, R_RBP);

	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 0);
	as_modrm(c.as, OP_STORE, R_RCX, R_RBP, 0, 0, 8);
	as_modrm(c.as, OP_STORE, R_RDX, R_RBP, 0, 0, 16);
	as_modrm(c.as, OP_STORE, R_RBX, R_RBP, 0, 0, 24);
	as_modrm(c.as, OP_STORE, R_RSI, R_RBP, 0, 0, 48);
	as_modrm(c.as, OP_STORE, R_RDI, R_RBP, 0, 0, 56);
	as_modrm(c.as, OP_STORE, R_R8, R_RBP, 0, 0, 64);
	as_modrm(c.as, OP_STORE, R_R9, R_RBP, 0, 0, 72);
	as_modrm(c.as, OP_STORE, R_R10, R_RBP, 0, 0, 80);
	as_modrm(c.as, OP_STORE, R_R11, R_RBP, 0, 0, 88);
	as_modrm(c.as, OP_STORE, R_R12, R_RBP, 0, 0, 96);
	as_modrm(c.as, OP_STORE, R_R13, R_RBP, 0, 0, 104);
	as_modrm(c.as, OP_STORE, R_R14, R_RBP, 0, 0, 112);
	as_modrm(c.as, OP_STORE, R_R15, R_RBP, 0, 0, 120);

	// trap
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 0);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 160);
	// err
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 8);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 168);
	// rip
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 16);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 128);
	// cs
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 24);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 144);
	// flags
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 32);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 136);
	// rsp
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 40);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 32);
	// ss
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 176 + 48);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 152);

	d = find(c, "_isr", 0:*byte, 1);
	if (d.func_defined && d.func_label.fixed) {
		as_jmp(c.as, OP_CALL, d.func_label);
	}

	// rip
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 128);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 176 + 16);
	// cs
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 144);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 176 + 24);
	// flags
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 136);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 176 + 32);
	// rsp
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 32);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 176 + 40);
	// ss
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 152);
	as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 176 + 48);

	// restore regs
	as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 0);
	as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 8);
	as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
	as_modrm(c.as, OP_LOAD, R_RBX, R_RBP, 0, 0, 24);
	as_modrm(c.as, OP_LOAD, R_RSI, R_RBP, 0, 0, 48);
	as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 56);
	as_modrm(c.as, OP_LOAD, R_R8, R_RBP, 0, 0, 64);
	as_modrm(c.as, OP_LOAD, R_R9, R_RBP, 0, 0, 72);
	as_modrm(c.as, OP_LOAD, R_R10, R_RBP, 0, 0, 80);
	as_modrm(c.as, OP_LOAD, R_R11, R_RBP, 0, 0, 88);
	as_modrm(c.as, OP_LOAD, R_R12, R_RBP, 0, 0, 96);
	as_modrm(c.as, OP_LOAD, R_R13, R_RBP, 0, 0, 104);
	as_modrm(c.as, OP_LOAD, R_R14, R_RBP, 0, 0, 112);
	as_modrm(c.as, OP_LOAD, R_R15, R_RBP, 0, 0, 120);

	as_modrm(c.as, OP_LOAD, R_RBP, R_RBP, 0, 0, 40);

	as_modri(c.as, OP_ADDI, R_RSP, 176 + 3 * 8);

	as_op(c.as, OP_IRETQ);
}

emit_builtin(c: *compiler) {
	var d: *decl;

	d = find(c, "syscall", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		emit_syscall(c.as);
		emit_ret(c.as);
	}

	d = find(c, "_restorer", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_restorer(c.as);
	}

	d = find(c, "_include", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_UD2);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "ud2", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_UD2);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "cpuid", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 24);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 32);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 40);
		as_modrm(c.as, OP_LOAD, R_RBX, R_RDI, 0, 0, 0);
		as_op(c.as, OP_CPUID);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_STORE, R_RAX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 24);
		as_modrm(c.as, OP_STORE, R_RCX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 32);
		as_modrm(c.as, OP_STORE, R_RDX, R_RDI, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 40);
		as_modrm(c.as, OP_STORE, R_RBX, R_RDI, 0, 0, 0);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "inb", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_op(c.as, OP_IN);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "outb", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_op(c.as, OP_OUT);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "inw", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_emit(c.as, OP_OS);
		as_op(c.as, OP_IND);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "outw", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_emit(c.as, OP_OS);
		as_op(c.as, OP_OUTD);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "ind", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_op(c.as, OP_IND);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "outd", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_op(c.as, OP_OUTD);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdmsr", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_op(c.as, OP_RDMSR);
		as_modri(c.as, OP_MOVI, R_RCX, 32);
		as_modr(c.as, OP_SHLM, R_RDX);
		as_modrr(c.as, OP_ORRM, R_RAX, R_RDX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "wrmsr", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_modrr(c.as, OP_MOVE, R_RDX, R_RAX);
		as_modri(c.as, OP_MOVI, R_RCX, 32);
		as_modr(c.as, OP_SHRM, R_RDX);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_op(c.as, OP_WRMSR);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdcr0", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrr(c.as, OP_RDCRR, R_CR0, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "wrcr0", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_modrr(c.as, OP_WRCRR, R_CR0, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdcr2", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrr(c.as, OP_RDCRR, R_CR2, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "wrcr2", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_modrr(c.as, OP_WRCRR, R_CR2, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdcr3", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrr(c.as, OP_RDCRR, R_CR3, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "wrcr3", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_modrr(c.as, OP_WRCRR, R_CR3, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdcr4", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrr(c.as, OP_RDCRR, R_CR4, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "wrcr4", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 16);
		as_modrr(c.as, OP_WRCRR, R_CR4, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "lgdt", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modri(c.as, OP_SUBI, R_RSP, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_modri(c.as, OP_SUBI, R_RAX, 1);
		as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 2);
		as_modm(c.as, OP_LGDTM, R_RSP, 0, 0, 0);
		as_modri(c.as, OP_ADDI, R_RSP, 16);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "lidt", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modri(c.as, OP_SUBI, R_RSP, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		as_modri(c.as, OP_SUBI, R_RAX, 1);
		as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_STORE, R_RAX, R_RSP, 0, 0, 2);
		as_modm(c.as, OP_LIDTM, R_RSP, 0, 0, 0);
		as_modri(c.as, OP_ADDI, R_RSP, 16);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "lldt", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modr(c.as, OP_LLDTM, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "ltr", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modr(c.as, OP_LTRM, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "lseg", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
                // es ds fs gs
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
                as_modrr(c.as, OP_WRSR, R_ES, R_RAX);
                as_modrr(c.as, OP_WRSR, R_DS, R_RAX);
                as_modrr(c.as, OP_WRSR, R_FS, R_RAX);
                as_modrr(c.as, OP_WRSR, R_GS, R_RAX);
                // ss
                as_opr(c.as, OP_PUSHR, R_RAX);
		// rsp
		as_opr(c.as, OP_PUSHR, R_RBP);
                // flags
		as_op(c.as, OP_PUSHF);
		// cs
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
                as_opr(c.as, OP_PUSHR, R_RAX);
		// rip
                as_op(c.as, OP_CALL);
                as_emit(c.as, 5);
                as_emit(c.as, 0);
                as_emit(c.as, 0);
                as_emit(c.as, 0);
                as_op(c.as, OP_JMP);
                as_emit(c.as, 2);
                as_emit(c.as, 0);
                as_emit(c.as, 0);
                as_emit(c.as, 0);
                as_op(c.as, OP_IRETQ);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "hlt", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_HLT);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "cli", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_CLI);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "sti", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_STI);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "rdflags", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_PUSHF);
		emit_ret(c.as);
	}

	d = find(c, "wrflags", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_op(c.as, OP_PUSHF);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_opr(c.as, OP_PUSHR, R_RAX);
		as_op(c.as, OP_POPF);
		emit_ret(c.as);
	}

	d = find(c, "wbinvld", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modm(c.as, OP_WBINVD, R_RAX, 0, 0, 0);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "invlpg", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
		as_modm(c.as, OP_INVLPGM, R_RAX, 0, 0, 0);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_ssr0", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_ssr(c);
	}

	d = find(c, "_isr0", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
                emit_isr(c);
	}

	d = find(c, "_rgs", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RSI, R_RBP, 0, 0, 16);
		as_emit(c.as, OP_GS);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RSI, 0, 0, 0);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_r32", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RSI, R_RBP, 0, 0, 16);
		c.as.bits32 = 1;
		as_modrm(c.as, OP_LOAD, R_RAX, R_RSI, 0, 0, 0);
		c.as.bits32 = 0;
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_w32", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		c.as.bits32 = 1;
		as_modrm(c.as, OP_STORE, R_RAX, R_RDI, 0, 0, 0);
		c.as.bits32 = 0;
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_r16", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RSI, R_RBP, 0, 0, 16);
		as_modrr(c.as, OP_XORRM, R_RAX, R_RAX);
		c.as.bits32 = 1;
		as_modrm(c.as, OP_LOAD16, R_RAX, R_RDI, 0, 0, 0);
		c.as.bits32 = 0;
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_w16", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 24);
		c.as.bits32 = 1;
		as_modrm(c.as, OP_STORE16, R_RAX, R_RDI, 0, 0, 0);
		c.as.bits32 = 0;
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "_rdrand", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);
		emit_preamble(c.as, 0, 0);
		as_modr(c.as, OP_RDRAND, R_RAX);
		as_opr(c.as, OP_PUSHR, R_RAX);
		emit_ret(c.as);
	}

	d = find(c, "taskswitch", 0:*byte, 1);
	if (d.func_defined && !d.func_label.fixed) {
		fixup_label(c.as, d.func_label);

		as_opr(c.as, OP_PUSHR, R_RBP);
		as_op(c.as, OP_PUSHF);

		as_modrm(c.as, OP_LOAD, R_RBP, R_RSP, 0, 0, 24);

		// save
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 0);
		as_modrm(c.as, OP_STORE, R_RCX, R_RBP, 0, 0, 8);
		as_modrm(c.as, OP_STORE, R_RDX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_STORE, R_RBX, R_RBP, 0, 0, 24);

		// rsp
		as_modrm(c.as, OP_LEA, R_RAX, R_RSP, 0, 0, 24);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 32);

		// rbp
		as_modrm(c.as, OP_LOAD, R_RAX, R_RSP, 0, 0, 8);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 40);

		as_modrm(c.as, OP_STORE, R_RSI, R_RBP, 0, 0, 48);
		as_modrm(c.as, OP_STORE, R_RDI, R_RBP, 0, 0, 56);
		as_modrm(c.as, OP_STORE, R_R8, R_RBP, 0, 0, 64);
		as_modrm(c.as, OP_STORE, R_R9, R_RBP, 0, 0, 72);
		as_modrm(c.as, OP_STORE, R_R10, R_RBP, 0, 0, 80);
		as_modrm(c.as, OP_STORE, R_R11, R_RBP, 0, 0, 88);
		as_modrm(c.as, OP_STORE, R_R12, R_RBP, 0, 0, 96);
		as_modrm(c.as, OP_STORE, R_R13, R_RBP, 0, 0, 104);
		as_modrm(c.as, OP_STORE, R_R14, R_RBP, 0, 0, 112);
		as_modrm(c.as, OP_STORE, R_R15, R_RBP, 0, 0, 120);

		// rip
		as_modrm(c.as, OP_LOAD, R_RAX, R_RSP, 0, 0, 16);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 128);

		// flags
		as_modrm(c.as, OP_LOAD, R_RAX, R_RSP, 0, 0, 0);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 136);

		// cs
		as_modrr(c.as, OP_RDSR, R_CS, R_RAX);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 144);

		// ss
		as_modrr(c.as, OP_RDSR, R_SS, R_RAX);
		as_modrm(c.as, OP_STORE, R_RAX, R_RBP, 0, 0, 152);

		as_modrm(c.as, OP_LOAD, R_RBP, R_RSP, 0, 0, 32);

		// restore
		as_modrm(c.as, OP_LOAD, R_RCX, R_RBP, 0, 0, 8);
		as_modrm(c.as, OP_LOAD, R_RDX, R_RBP, 0, 0, 16);
		as_modrm(c.as, OP_LOAD, R_RBX, R_RBP, 0, 0, 24);
		as_modrm(c.as, OP_LOAD, R_RSI, R_RBP, 0, 0, 48);
		as_modrm(c.as, OP_LOAD, R_RDI, R_RBP, 0, 0, 56);
		as_modrm(c.as, OP_LOAD, R_R8, R_RBP, 0, 0, 64);
		as_modrm(c.as, OP_LOAD, R_R9, R_RBP, 0, 0, 72);
		as_modrm(c.as, OP_LOAD, R_R10, R_RBP, 0, 0, 80);
		as_modrm(c.as, OP_LOAD, R_R11, R_RBP, 0, 0, 88);
		as_modrm(c.as, OP_LOAD, R_R12, R_RBP, 0, 0, 96);
		as_modrm(c.as, OP_LOAD, R_R13, R_RBP, 0, 0, 104);
		as_modrm(c.as, OP_LOAD, R_R14, R_RBP, 0, 0, 112);
		as_modrm(c.as, OP_LOAD, R_R15, R_RBP, 0, 0, 120);

		// ss
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 152);
		as_opr(c.as, OP_PUSHR, R_RAX);
		// rsp
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 32);
		as_opr(c.as, OP_PUSHR, R_RAX);
		// flags
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 136);
		as_opr(c.as, OP_PUSHR, R_RAX);
		// cs
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 144);
		as_opr(c.as, OP_PUSHR, R_RAX);
		// rip
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 128);
		as_opr(c.as, OP_PUSHR, R_RAX);

		// finish up restore
		as_modrm(c.as, OP_LOAD, R_RAX, R_RBP, 0, 0, 0);
		as_modrm(c.as, OP_LOAD, R_RBP, R_RBP, 0, 0, 40);

		// iretq
		as_op(c.as, OP_IRETQ);
	}
}

main(argc: int, argv: **byte, envp: **byte) {
	var a: alloc;
	var c: *compiler;
	var p: *node;
	var d: *decl;
	var start: *label;
	var kstart: *label;
	var i: int;
	var show: int;
	var filename: *byte;
	var err: *file;

	setup_alloc(&a);

	c = comp_setup(&a);

	show = 0;

	filename = "a.out";

	i = 1;
	loop {
		if (i >= argc) {
			break;
		}

		if (!strcmp(argv[i], "-o")) {
			i = i + 1;
			if (i >= argc) {
				die("invalid -o at end of argument list");
			}
			filename = argv[i];
			i = i + 1;
			continue;
		}

		if (!strcmp(argv[i], "-fdump")) {
			i = i + 1;
			show = 1;
			continue;
		}

		if (!strcmp(argv[i], "-C")) {
			c.do_cout = 1;
			i = i + 1;
			continue;
		}

		if (argv[i][0] == '-':byte) {
			die("invalid argument");
		}

		p = concat_program(p, parse(c.p, argv[i]));

		i = i + 1;
	}

	if show {
		err = fopen(2, &a);
		show_node(err, p);
		fflush(err);
		return;
	}

	compile(c, p);

	if c.do_cout {
		open_coutput(c, filename);

		ctranslate(c);

		return;
	}

	emit_builtin(c);

	start = 0: *label;
	d = find(c, "_start", 0:*byte, 0);
	if (d && d.func_defined) {
		start = d.func_label;
	}

	kstart = 0: *label;
	d = find(c, "_kstart", 0:*byte, 0);
	if (d && d.func_defined) {
		kstart = d.func_label;
	}

	open_output(c.as, filename);

	writeout(c.as, start, kstart);
}
