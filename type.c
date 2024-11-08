struct type {
	kind: int;
	st: *decl;
	val: *type;
	arg: *type;
}

enum {
	TY_VOID,
	TY_INT,
	TY_BYTE,
	TY_PTR,
	TY_ARG,
	TY_FUNC,
	TY_STRUCT,
	TY_UNION,
}

type_sizeof(c: *compiler, t: *type): int {
	var kind: int;

	kind = t.kind;
	if (kind == TY_INT) {
		return 8;
	} else if (kind == TY_BYTE) {
		return 8;
	} else if (kind == TY_PTR) {
		return 8;
	} else if (kind == TY_FUNC) {
		return 8;
	} else if (kind == TY_STRUCT) {
		layout_struct(c, t.st);
		return t.st.struct_size;
	} else if (kind == TY_UNION) {
		layout_union(c, t.st);
		return t.st.struct_size;
	} else {
		cdie(c, "sizeof: invalid type");
	}
}

// Unify two types
unify(c: *compiler, a: *type, b: *type) {
	var kind: int;

	if (a == b) {
		return;
	}

	if ((a && !b) || (b && !a) || a.kind != b.kind) {
		cdie(c, "type error");
	}

	kind = a.kind;
	if (kind == TY_PTR) {
		unify(c, a.val, b.val);
	} else if (kind == TY_FUNC) {
		unify(c, a.val, b.val);
		unify(c, a.arg, b.arg);
	} else if (kind == TY_ARG) {
		unify(c, a.val, b.val);
		unify(c, a.arg, b.arg);
	} else if (kind == TY_STRUCT) {
		if (a.st != b.st) {
			cdie(c, "type error");
		}
	} else if (kind == TY_UNION) {
		if (a.st != b.st) {
			cdie(c, "type error");
		}
	} else if (kind != TY_VOID && kind != TY_INT && kind != TY_BYTE) {
		cdie(c, "unify: invalid type");
	}
}

count_args(c: *compiler, t: *type): int {
	var nargs: int;

	nargs = 0;
	loop {
		if (!t) {
			break;
		}

		t = t.arg;
		nargs = nargs + 1;
	}

	return nargs;
}

mktype(c: *compiler, kind: int, a: *type, b: *type, st: *decl): *type {
	var t: *type;

	t = alloc(c.a, sizeof(*t)):*type;

	t.kind = kind;
	t.st = st;
	t.val = a;
	t.arg = b;

	return t;
}

mktype_struct(c: *compiler, st: *decl): *type {
	return mktype(c, TY_STRUCT, 0:*type, 0:*type, st);
}

mktype_union(c: *compiler, st: *decl): *type {
	return mktype(c, TY_UNION, 0:*type, 0:*type, st);
}

mktype0(c: *compiler, kind: int): *type {
	return mktype(c, kind, 0:*type, 0:*type, 0:*decl);
}

mktype1(c: *compiler, kind: int, a: *type): *type {
	return mktype(c, kind, a, 0:*type, 0:*decl);
}

mktype2(c: *compiler, kind: int, a: *type, b: *type): *type {
	return mktype(c, kind, a, b, 0:*decl);
}

type_isint(t: *type): int {
	return t.kind == TY_INT || t.kind == TY_BYTE;
}

type_isprim(t: *type): int {
	return t.kind != TY_VOID && t.kind != TY_STRUCT && t.kind != TY_UNION;
}

prototype(c: *compiler, n: *node): *type {
	var a: *type;
	var b: *type;
	var st: *decl;
	var kind: int;

	if (!n) {
		return 0:*type;
	}

	c.lineno = n.lineno;
	c.colno = 0;

	kind = n.kind;
	if (kind == N_IDENT) {
		if (!strcmp(n.s, "void")) {
			return mktype0(c, TY_VOID);
		}

		if (!strcmp(n.s, "int")) {
			return mktype0(c, TY_INT);
		}

		if (!strcmp(n.s, "byte")) {
			return mktype0(c, TY_BYTE);
		}

		st = find(c, n.s, 0:*byte, 0);
		if (!st || !st.struct_defined) {
			cdie(c, "unknown struct");
		}

		if st.struct_def.kind == N_STRUCT {
			return mktype_struct(c, st);
		} else {
			return mktype_union(c, st);
		}
	} else if (kind == N_ARGLIST) {
		a = prototype(c, n.a.b);
		b = prototype(c, n.b);

		kind = a.kind;
		if (kind != TY_INT && kind != TY_BYTE
				&& kind != TY_PTR && kind != TY_FUNC) {
			cdie(c, "not a ptr arg");
		}

		return mktype2(c, TY_ARG, a, b);
	} else if (kind == N_FUNCTYPE) {
		if (n.b) {
			a = prototype(c, n.b);
		} else{
			a = mktype0(c, TY_VOID);
		}

		b = prototype(c, n.a);

		kind = a.kind;
		if (kind != TY_VOID && kind != TY_INT && kind != TY_BYTE
				&& kind != TY_PTR && kind != TY_FUNC) {
			cdie(c, "not a ptr return");
		}

		return mktype2(c, TY_FUNC, a, b);
	} else if (kind == N_PTRTYPE) {
		return mktype1(c, TY_PTR, prototype(c, n.a));
	} else {
		cdie(c, "prototype: invalid type");
	}
}
