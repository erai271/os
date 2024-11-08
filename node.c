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
	N_UNION,
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

mknode(c: *parser, kind: int, a: *node, b: *node): *node {
	var ret: *node;
	ret = alloc(c.a, sizeof(*ret)):*node;
	ret.kind = kind;
	ret.a = a;
	ret.b = b;
	ret.filename = 0:*byte;
	ret.lineno = 0;
	ret.colno = 0;
	ret.n = 0;
	ret.s = 0:*byte;
	ret.t = 0:*type;
	fillpos(c, ret);
	return ret;
}

mknode0(c: *parser, kind: int): *node {
	return mknode(c, kind, 0:*node, 0:*node);
}

mknode1(c: *parser, kind: int, a: *node): *node {
	return mknode(c, kind, a, 0:*node);
}

concat_program(a: *node, b: *node): *node {
	var ret: *node;

	if !a {
		return b;
	}

	if !b {
		return a;
	}

	ret = a;
	loop {
		if !a.b {
			break;
		}

		a = a.b;
	}

	a.b = b;

	return ret;
}

node_to_str(kind: int): *byte {
	if kind == N_IDENT { return "N_IDENT"; }
	if kind == N_NUM { return "N_NUM"; }
	if kind == N_CHAR { return "N_CHAR"; }
	if kind == N_STR { return "N_STR"; }
	if kind == N_STMTLIST { return "N_STMTLIST"; }
	if kind == N_EXPRLIST { return "N_EXPRLIST"; }
	if kind == N_CALL { return "N_CALL"; }
	if kind == N_DOT { return "N_DOT"; }
	if kind == N_ARGLIST { return "N_ARGLIST"; }
	if kind == N_FUNC { return "N_FUNC"; }
	if kind == N_ARGDECL { return "N_ARGDECL"; }
	if kind == N_FUNCDECL { return "N_FUNCDECL"; }
	if kind == N_PROGRAM { return "N_PROGRAM"; }
	if kind == N_FUNCTYPE { return "N_FUNCTYPE"; }
	if kind == N_PTRTYPE { return "N_PTRTYPE"; }
	if kind == N_STRUCT { return "N_STRUCT"; }
	if kind == N_UNION { return "N_UNION"; }
	if kind == N_MEMBERDECL { return "N_MEMBERDECL"; }
	if kind == N_MEMBERLIST { return "N_MEMBERLIST"; }
	if kind == N_CONDLIST { return "N_CONDLIST"; }
	if kind == N_COND { return "N_COND"; }
	if kind == N_ENUM { return "N_ENUM"; }
	if kind == N_ENUMITEM { return "N_ENUMITEM"; }
	if kind == N_ENUMLIST { return "N_ENUMLIST"; }
	if kind == N_LOOP { return "N_LOOP"; }
	if kind == N_BREAK { return "N_BREAK"; }
	if kind == N_CONTINUE { return "N_CONTINUE"; }
	if kind == N_RETURN { return "N_RETURN"; }
	if kind == N_VARDECL { return "N_VARDECL"; }
	if kind == N_LABEL { return "N_LABEL"; }
	if kind == N_GOTO { return "N_GOTO"; }
	if kind == N_ASSIGN { return "N_ASSIGN"; }
	if kind == N_SIZEOF { return "N_SIZEOF"; }
	if kind == N_REF { return "N_REF"; }
	if kind == N_DEREF { return "N_DEREF"; }
	if kind == N_CAST { return "N_CAST"; }
	if kind == N_INDEX { return "N_INDEX"; }
	if kind == N_LT { return "N_LT"; }
	if kind == N_GT { return "N_GT"; }
	if kind == N_LE { return "N_LE"; }
	if kind == N_GE { return "N_GE"; }
	if kind == N_EQ { return "N_EQ"; }
	if kind == N_NE { return "N_NE"; }
	if kind == N_ADD { return "N_ADD"; }
	if kind == N_SUB { return "N_SUB"; }
	if kind == N_MUL { return "N_MUL"; }
	if kind == N_LSH { return "N_LSH"; }
	if kind == N_RSH { return "N_RSH"; }
	if kind == N_BNOT { return "N_BNOT"; }
	if kind == N_BOR { return "N_BOR"; }
	if kind == N_BAND { return "N_BAND"; }
	if kind == N_AND { return "N_AND"; }
	if kind == N_OR { return "N_OR"; }
	if kind == N_XOR { return "N_XOR"; }
	if kind == N_NOT { return "N_NOT"; }
	if kind == N_POS { return "N_POS"; }
	if kind == N_NEG { return "N_NEG"; }
	if kind == N_DIV { return "N_DIV"; }
	if kind == N_MOD { return "N_MOD"; }
	return "(invalid)";
}

show_node(out: *file, n: *node) {
	var i: int;
	var ch: int;
	var hex: *byte;
	hex = "0123456789abcdef";
	if !n {
		return;
	}
	fputc(out, '(');
	fputs(out, node_to_str(n.kind));
	if n.kind == N_NUM {
		fputc(out, ' ');
		fputd(out, n.n);
	}
	if n.s {
		fputc(out, ' ');
		fputc(out, '"');
		i = 0;
		loop {
			ch = n.s[i]:int;
			if !ch {
				break;
			}
			if ch < 32 || ch > 127 || ch == '\\' || ch == '"' {
				fputc(out, '\\');
				fputc(out, 'x');
				fputc(out, hex[ch >> 4]:int);
				fputc(out, hex[ch & 15]:int);
			} else {
				fputc(out, ch);
			}
			i = i + 1;
		}
		fputc(out, '"');
	}
	if n.a {
		fputc(out, ' ');
		show_node(out, n.a);
	}
	if n.b {
		fputc(out, ' ');
		show_node(out, n.b);
	}
	fputc(out, ')');
}
