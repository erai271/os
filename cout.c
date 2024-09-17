ctranslate(c: *compiler) {
	var d: *decl;
	var seen: int;
	var has_enum: int;

	fputs(c.cout, "#ifndef my__start\n");
	fputs(c.cout, "#define my__start main\n");
	fputs(c.cout, "#endif\n");
	fputs(c.cout, "#ifndef my_syscall\n");
	fputs(c.cout, "#define my_syscall syscall\n");
	fputs(c.cout, "#endif\n");

	// Pass: forward declare all structs
	d = first_decl(c);
	loop {
		if !d {
			break;
		}

		if d.struct_defined {
			fputs(c.cout, "struct my_");
			fputs(c.cout, d.name);
			fputs(c.cout, ";\n");
		}

		d = next_decl(c, d);
	}

	// Pass: define structs
	d = first_decl(c);
	loop {
		if !d {
			break;
		}

		if d.struct_defined {
			ctranslate_struct(c, d);
		}

		d = next_decl(c, d);
	}

	// Pass: define enums
	d = first_decl(c);
	has_enum = 0;
	seen = 0;
	loop {
		if !d {
			break;
		}

		if d.enum_defined {
			if !has_enum {
				fputs(c.cout, "enum {\n");
				has_enum = 1;
			}
			if seen {
				fputs(c.cout, ",\n");
			}
			fputs(c.cout, "\tmy_");
			fputs(c.cout, d.name);
			fputs(c.cout, " = ");
			fputd(c.cout, d.enum_value);
			seen = 1;
		}

		d = next_decl(c, d);
	}
	if has_enum {
		fputs(c.cout, "\n};\n");
	}

	// Pass: forward declare all functions
	d = first_decl(c);
	loop {
		if !d {
			break;
		}

		if d.func_defined {
			ctranslate_type(c, d.func_type, d.name, 0);
			fputs(c.cout, ";\n");
		}

		d = next_decl(c, d);
	}

	// Pass: define functions
	d = first_decl(c);
	loop {
		if !d {
			break;
		}

		if d.func_defined {
			ctranslate_func(c, d);
		}

		d = next_decl(c, d);
	}

	flush_coutput(c);
}

ctranslate_type(c: *compiler, ty: *type, name: *byte, ptr: int) {
	var arg: *type;
	if ty.kind == TY_VOID {
		fputs(c.cout, "void ");
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
	} else if ty.kind == TY_INT {
		fputs(c.cout, "unsigned long ");
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
	} else if ty.kind == TY_BYTE {
		fputs(c.cout, "unsigned char ");
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
	} else if ty.kind == TY_PTR {
		ctranslate_type(c, ty.val, "", 1);
		fputs(c.cout, "*");
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
	} else if ty.kind == TY_FUNC {
		ctranslate_type(c, ty.val, "", 1);
		if ptr {
			die("fixme cdecl order is hard");
		}
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
		fputs(c.cout, "(");
		if ptr {
			fputs(c.cout, "*");
		}
		arg = ty.arg;
		if arg {
			loop {
				ctranslate_type(c, arg.val, "", 1);
				arg = arg.arg;
				if arg {
					fputs(c.cout, ", ");
				} else {
					break;
				}
			}
		} else {
			fputs(c.cout, "void");
		}
		fputs(c.cout, ")");
	} else if ty.kind == TY_STRUCT {
		fputs(c.cout, "struct my_");
		fputs(c.cout, ty.st.name);
		if name && name[0] {
			fputs(c.cout, " my_");
			fputs(c.cout, name);
		}
	} else {
		die("invalid type");
	}
}

ctranslate_zero(c: *compiler, ty: *type) {
	var n: *node;
	var v: *decl;
	var arg: *type;
	if ty.kind == TY_VOID {
		die("invalid zero void");
	} else if ty.kind == TY_INT {
		fputs(c.cout, "0");
	} else if ty.kind == TY_BYTE {
		fputs(c.cout, "0");
	} else if ty.kind == TY_PTR {
		fputs(c.cout, "0");
	} else if ty.kind == TY_FUNC {
		fputs(c.cout, "0");
	} else if ty.kind == TY_STRUCT {
		fputs(c.cout, "{");
		n = ty.st.struct_def.b;
		loop {
			if !n {
				break;
			}
			v = find(c, ty.st.name, n.a.a.s, 0);
			ctranslate_zero(c, v.member_type);
			n = n.b;
		}
		fputs(c.cout, "}");
	} else {
		die("invalid type");
	}
}

ctranslate_struct(c: *compiler, d: *decl) {
	var v: *decl;
	var n: *node;
	fputs(c.cout, "struct my_");
	fputs(c.cout, d.name);
	fputs(c.cout, " {\n");
	n = d.struct_def.b;
	loop {
		if !n {
			break;
		}

		v = find(c, d.name, n.a.a.s, 0);

		fputs(c.cout, "\t");
		ctranslate_type(c, v.member_type, n.a.a.s, 1);
		fputs(c.cout, ";\n");

		n = n.b;
	}
	fputs(c.cout, "};\n");
}

ctranslate_vars(c: *compiler, n: *node) {
	var kind: int;
	var child: *node;

	if !n {
		return;
	}

	kind = n.kind;
	if kind == N_CONDLIST {
		loop {
			if !n {
				break;
			}
			ctranslate_vars(c, n.a.b);
			n = n.b;
		}
	} else if kind == N_STMTLIST {
		loop {
			if !n {
				break;
			}
			ctranslate_vars(c, n.a);
			n = n.b;
		}
	} else if kind == N_LOOP {
		ctranslate_vars(c, n.a);
	} else if kind == N_VARDECL {
		fputs(c.cout, "\t");
		ctranslate_type(c, n.t, n.a.s, 1);
		fputs(c.cout, " = ");
		ctranslate_zero(c, n.t);
		fputs(c.cout, ";\n");
	}
}

ctranslate_str(c: *compiler, s: *byte) {
	var i: int;
	var ch: int;
	i = 0;
	fputs(c.cout, "(unsigned char *)\"");
	loop {
		if !s[i] {
			break;
		}

		ch = s[i]:int;

		if ch < 32 || ch > 127 || ch == '\\' || ch == '"' {
			fputc(c.cout, '\\');
			fputc(c.cout, '0' + (ch >> 6) & 7);
			fputc(c.cout, '0' + (ch >> 3) & 7);
			fputc(c.cout, '0' + (ch & 7));
		} else {
			fputc(c.cout, ch);
		}

		i = i + 1;
	}
	fputs(c.cout, "\"");
}

ctranslate_expr(c: *compiler, n: *node) {
	if n.kind == N_STR {
		ctranslate_str(c, n.s);
	} else if n.kind == N_NUM {
		fputd(c.cout, n.n);
		fputs(c.cout, "UL");
	} else if n.kind == N_CHAR {
		fputd(c.cout, n.n);
	} else if n.kind == N_CALL {
		fputs(c.cout, "(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")");
		fputs(c.cout, "(");
		n = n.b;
		loop {
			if !n {
				break;
			}
			fputs(c.cout, "(");
			ctranslate_expr(c, n.a);
			fputs(c.cout, ")");
			n = n.b;
			if n {
				fputs(c.cout, ",");
			}
		}
		fputs(c.cout, ")");
	} else if n.kind == N_DOT {
		fputs(c.cout, "(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")");
		if n.a.t.kind == TY_PTR {
			fputs(c.cout, "->");
		} else {
			fputs(c.cout, ".");
		}
		fputs(c.cout, "my_");
		fputs(c.cout, n.b.s);
	} else if n.kind == N_IDENT {
		fputs(c.cout, "my_");
		fputs(c.cout, n.s);
	} else if n.kind == N_ASSIGN {
		fputs(c.cout, "(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")=(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")");
	} else if n.kind == N_SIZEOF {
		fputd(c.cout, type_sizeof(c, n.a.t));
		fputs(c.cout, "UL");
	} else if n.kind == N_REF {
		fputs(c.cout, "&(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")");
	} else if n.kind == N_DEREF {
		fputs(c.cout, "*(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")");
	} else if n.kind == N_INDEX {
		fputs(c.cout, "(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")[");
		ctranslate_expr(c, n.b);
		fputs(c.cout, "]");
	} else if n.kind == N_LT {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))<((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_LE {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))<=((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_GT {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))>((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_GE {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))>=((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_EQ {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))==((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_NE {
		fputs(c.cout, "(unsigned long)(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))!=((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_BNOT {
		fputs(c.cout, "(unsigned long)(!(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))");
	} else if n.kind == N_BOR {
		fputs(c.cout, "(unsigned long)((");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")||(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, "))");
	} else if n.kind == N_BAND {
		fputs(c.cout, "(unsigned long)((");
		ctranslate_expr(c, n.a);
		fputs(c.cout, ")&&(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, "))");
	} else if n.kind == N_POS {
		ctranslate_expr(c, n.a);
	} else if n.kind == N_NEG {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(-(unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))");
	} else if n.kind == N_NOT {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(~(unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))");
	} else if n.kind == N_ADD {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))+((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_SUB {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))-((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_MUL {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))*((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_DIV {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))/((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_MOD {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))%((long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_LSH {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))<<((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_RSH {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))>>((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_AND {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))&((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_OR {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))|((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_XOR {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")(((unsigned long)(");
		ctranslate_expr(c, n.a);
		fputs(c.cout, "))^((unsigned long)(");
		ctranslate_expr(c, n.b);
		fputs(c.cout, ")))");
	} else if n.kind == N_CAST {
		fputs(c.cout, "(");
		ctranslate_type(c, n.t, "", 1);
		fputs(c.cout, ")");
		ctranslate_expr(c, n.a);
	} else {
		fdputd(2, n.kind);
		die("invalid expr");
	}
}

ctranslate_stmt(c: *compiler, n: *node) {
	var kind: int;
	var child: *node;

	if !n {
		return;
	}

	kind = n.kind;
	if kind == N_CONDLIST {
		fputs(c.cout, "\t");
		loop {
			fputs(c.cout, "if (");

			ctranslate_expr(c, n.a.a);

			fputs(c.cout, ") {\n");

			ctranslate_stmt(c, n.a.b);

			n = n.b;

			fputs(c.cout, "\t}");

			if !n {
				fputs(c.cout, "\n");
				break;
			}

			fputs(c.cout, " else ");

			if !n.a.a {
				fputs(c.cout, "{\n");
				ctranslate_stmt(c, n.a.b);
				fputs(c.cout, "\t}\n");
				break;
			}
		}
	} else if kind == N_STMTLIST {
		loop {
			if !n {
				break;
			}

			ctranslate_stmt(c, n.a);

			n = n.b;
		}
	} else if kind == N_LOOP {
		fputs(c.cout, "\twhile (1) {\n");
		ctranslate_stmt(c, n.a);
		fputs(c.cout, "\t}\n");
	} else if kind == N_BREAK {
		fputs(c.cout, "\tbreak;\n");
	} else if kind == N_CONTINUE {
		fputs(c.cout, "\tcontinue;\n");
	} else if kind == N_RETURN {
		fputs(c.cout, "\treturn");
		if n.a {
			fputs(c.cout, " ");
			ctranslate_expr(c, n.a);
		}
		fputs(c.cout, ";\n");
	} else if kind == N_LABEL {
		fputs(c.cout, "my_");
		fputs(c.cout, n.a.s);
		fputs(c.cout, ":\n");
	} else if kind == N_GOTO {
		fputs(c.cout, "\tgoto ");
		fputs(c.cout, "my_");
		fputs(c.cout, n.a.s);
		fputs(c.cout, ";\n");
	} else if kind != N_VARDECL {
		fputs(c.cout, "\t");
		ctranslate_expr(c, n);
		fputs(c.cout, ";\n");
	}
}

ctranslate_func(c: *compiler, d: *decl) {
	var n: *node;
	var ty: *type;
	if d.func_def {
		ctranslate_type(c, d.func_type.val, "", 1);
		fputs(c.cout, "my_");
		fputs(c.cout, d.name);
		fputs(c.cout, "(");
		n = d.func_def.a.b.a;
		ty = d.func_type.arg;
		loop {
			if !n {
				break;
			}

			ctranslate_type(c, ty.val, n.a.a.s, 1);

			n = n.b;
			ty = ty.arg;

			if n {
				fputs(c.cout, ", ");
			}
		}
		fputs(c.cout, ")\n");
		fputs(c.cout, "{\n");
		ctranslate_vars(c, d.func_def.b);
		ctranslate_stmt(c, d.func_def.b);
		fputs(c.cout, "}\n");
	}
}
