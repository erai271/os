struct decl {
	name: *byte;
	member_name: *byte;
	p: *decl;
	l: *decl;
	r: *decl;

	func_defined: int;
	func_type: *type;
	func_label: *label;
	func_def: *node;

	struct_defined: int;
	struct_size: int;
	struct_layout_done: int;
	struct_def: *node;

	member_defined: int;
	member_type: *type;
	member_offset: int;
	member_def: *node;

	enum_defined: int;
	enum_value: int;
	enum_def: *node;

	var_defined: int;
	var_type: *type;
	var_offset: int;
	var_def: *node;

	goto_defined: int;
	goto_label: *label;
}

find(c: *compiler, name: *byte, member_name: *byte, make: int): *decl {
	var p: *decl;
	var d: *decl;
	var link: **decl;
	var dir: int;

	p = 0: *decl;
	link = &c.decls;
	loop {
		d = *link;
		if (!d) {
			break;
		}

		dir = strcmp(name, d.name);

		if (dir == 0) {
			if (!member_name && !d.member_name) {
				dir = 0;
			} else if (!member_name) {
				dir = -1;
			} else if (!d.member_name) {
				dir = 1;
			} else {
				dir = strcmp(member_name, d.member_name);
			}
		}

		if (dir < 0) {
			p = d;
			link = &d.l;
		} else if (dir > 0) {
			p = d;
			link = &d.r;
		} else {
			return d;
		}
	}


	if (!make) {
		return 0:*decl;
	}

	d = alloc(c.a, sizeof(*d)): *decl;

	d.name = name;
	d.member_name = member_name;

	d.p = p;
	d.l = 0:*decl;
	d.r = 0:*decl;

	d.func_defined = 0;
	d.func_type = 0:*type;
	d.func_label = mklabel(c.as);
	d.func_def = 0:*node;

	d.struct_defined = 0;
	d.struct_size = 0;
	d.struct_layout_done = 0;
	d.struct_def = 0:*node;

	d.member_defined = 0;
	d.member_type = 0:*type;
	d.member_offset = 0;
	d.member_def = 0:*node;

	d.enum_defined = 0;
	d.enum_value = 0;
	d.enum_def = 0:*node;

	d.var_defined = 0;
	d.var_type = 0:*type;
	d.var_offset = 0;
	d.var_def = 0:*node;

	d.goto_defined = 0;
	d.goto_label = mklabel(c.as);

	*link = d;

	return d;
}

// Find the first declaration
first_decl(c: *compiler): *decl {
	var d: *decl;

	d = c.decls;
	if (!d) {
		return 0:*decl;
	}

	loop {
		if (!d.l) {
			return d;
		}

		d = d.l;
	}
}

next_decl(c: *compiler, d: *decl): *decl {
	if (!d) {
		return 0:*decl;
	}

	if (d.r) {
		d = d.r;

		loop {
			if (!d.l) {
				return d;
			}

			d = d.l;
		}
	}

	loop {
		if (!d.p) {
			return 0:*decl;
		}

		if (d.p.l == d) {
			return d.p;
		}

		d = d.p;
	}
}

