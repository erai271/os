struct fixup {
	next: *fixup;
	ptr: *byte;
	at: int;
}

struct label {
	fix: *fixup;
	at: int;
	fixed: int;
}

struct chunk {
	next: *chunk;
	buf: *byte;
	fill: int;
	cap: int;
}

struct assembler {
	a: *alloc;
	fdout: int;
	at: int;
	text: *chunk;
	text_end: *chunk;
}

setup_assembler(a: *alloc): *assembler {
	var c: *assembler;
	c = alloc(a, sizeof(*c)): *assembler;
	c.a = a;
	c.fdout = 1;
	c.at = 0;
	c.text = 0:*chunk;
	c.text_end = 0:*chunk;
	return c;
}

putchar(c: *assembler, ch: int) {
	fdputc(c.fdout, ch);
}

open_output(c: *assembler, filename: *byte) {
	var fd: int;

	if (c.fdout != 1) {
		die("multiple output files");
	}

	unlink(filename);

	fd = open(filename, 64 + 1, (7 << 6) + (7 << 3) +7);
	if (fd < 0) {
		die("failed to open output");
	}

	c.fdout = fd;
}

// Create a new label
mklabel(c: *assembler): *label {
	var l: *label;

	l = alloc(c.a, sizeof(*l)):*label;

	l.fix = 0:*fixup;
	l.at = 0;
	l.fixed = 0;

	return l;
}

// Reserve size in the output buffer
reserve(c: *assembler, n: int) {
	var m: *byte;
	var b: *chunk;

	if (c.text_end && c.text_end.cap - c.text_end.fill >= n) {
		return;
	}

	if (n < 4096) {
		n = 4096;
	}

	m = alloc(c.a, n);
	b = alloc(c.a, sizeof(*b)):*chunk;

	b.buf = m;
	b.fill = 0;
	b.cap = n;
	b.next = 0:*chunk;

	if (c.text_end) {
		c.text_end.next = b;
		c.text_end = b;
	} else {
		c.text = b;
		c.text_end = b;
	}
}

// Add a single byte to the output
emit(c: *assembler, x: int) {
	reserve(c, 1);
	c.text_end.buf[c.text_end.fill] = x:byte;
	c.text_end.fill = c.text_end.fill + 1;
	c.at = c.at + 1;
}

// Fix a single reference
fixup(c: *assembler, here: *byte, delta: int) {
	here[0] = delta: byte;
	here[1] = (delta >> 8): byte;
	here[2] = (delta >> 16): byte;
	here[3] = (delta >> 24): byte;
}

// Add an new fixup for the current position
addfixup(c: *assembler, l: *label) {
	var f: *fixup;
	var here: *byte;

	reserve(c, 4);

	here = &c.text_end.buf[c.text_end.fill];

	emit(c, 0);
	emit(c, 0);
	emit(c, 0);
	emit(c, 0);

	if (l.fixed) {
		fixup(c, here, l.at - c.at);
	} else {
		f = alloc(c.a, sizeof(*f)): *fixup;

		f.next = l.fix;
		f.ptr = here;
		f.at = c.at;

		l.fix = f;
	}
}

// Fix references to a label to the current position
fixup_label(c: *assembler, l: *label) {
	var f: *fixup;

	if (l.fixed) {
		die("already fixed");
	}

	l.at = c.at;
	l.fixed = 1;

	f = l.fix;
	loop {
		if (!f) {
			break;
		}
		fixup(c, f.ptr, l.at - f.at);
		f = f.next;
	}
}

emit_ptr(c: *assembler, l: *label) {
	// lea %rax, [l]
	emit(c, 0x48);
	emit(c, 0x8d);
	emit(c, 0x05);
	addfixup(c, l);
	// push %rax
	emit(c, 0x50);
}

emit_jmp(c: *assembler, l: *label) {
	// jmp l
	emit(c, 0xe9);
	addfixup(c, l);
}

emit_num(c: *assembler, x: int) {
	// push x
	emit(c, 0x68);
	emit(c, x);
	emit(c, x >> 8);
	emit(c, x >> 16);
	emit(c, x >> 24);
}

emit_str(c: *assembler, s: *byte) {
	var a: *label;
	var b: *label;
	var i: int;

	a = mklabel(c);
	b = mklabel(c);

	// jmp b
	emit_jmp(c, b);

	// a:
	fixup_label(c, a);

	i = 0;
	// .string s
	loop {
		if (!s[i]) {
			break;
		}
		emit(c, s[i]:int);
		i = i + 1;
	}
	emit(c, 0);

	// nop sled
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);
	emit(c, 0x90);

	// b:
	fixup_label(c, b);
	// push a
	emit_ptr(c, a);
}

emit_pop(c: *assembler, n: int) {
	n = n * 8;
	// add rsp, 8*n
	emit(c, 0x48);
	emit(c, 0x81);
	emit(c, 0xc4);
	emit(c, n);
	emit(c, n >> 8);
	emit(c, n >> 16);
	emit(c, n >> 24);
}

emit_preamble(c: *assembler, n: int, start: int) {
	var i: int;
	if (start) {
		// xor rbp, rbp
		emit(c, 0x48);
		emit(c, 0x31);
		emit(c, 0xed);
		// mov rdi, [rsp]
		emit(c, 0x48);
		emit(c, 0x8b);
		emit(c, 0x3c);
		emit(c, 0x24);
		// lea rsi, [rsp + 8]
		emit(c, 0x48);
		emit(c, 0x8d);
		emit(c, 0x74);
		emit(c, 0x24);
		emit(c, 0x08);
		// lea rdx, [rsi + rdi * 8 + 8]
		emit(c, 0x48);
		emit(c, 0x8d);
		emit(c, 0x54);
		emit(c, 0xfe);
		emit(c, 0x08);
		// push rdx
		emit(c, 0x52);
		// push rsi
		emit(c, 0x56);
		// push rdi
		emit(c, 0x57);
		// push rbp
		emit(c, 0x55);
	}
	// push rbp
	emit(c, 0x55);
	// mov rbp, rsp
	emit(c, 0x48);
	emit(c, 0x89);
	emit(c, 0xe5);
	i = 0;
	loop {
		if (i >= n) {
			break;
		}
		emit_num(c, 0);
		i = i + 8;
	}
}

emit_store(c: *assembler, t: *type) {
	// pop rdi
	emit(c, 0x5f);
	// pop rax
	emit(c, 0x58);
	if (t.kind == TY_BYTE) {
		// mov [rdi], al
		emit(c, 0x88);
		emit(c, 0x07);
	} else if (type_isprim(t)) {
		// mov [rdi], rax
		emit(c, 0x48);
		emit(c, 0x89);
		emit(c, 0x07);
	} else {
		die("invalid store");
	}
	// push rax
	emit(c, 0x50);
}

emit_load(c: *assembler, t: *type) {
	// pop rdi
	emit(c, 0x5f);
	if (t.kind == TY_BYTE) {
		// xor rax, rax
		emit(c, 0x48);
		emit(c, 0x31);
		emit(c, 0xc0);
		// mov al, [rdi]
		emit(c, 0x8a);
		emit(c, 0x07);
	} else if (type_isprim(t)) {
		// mov rax, [rdi]
		emit(c, 0x48);
		emit(c, 0x8b);
		emit(c, 0x07);
	} else {
		die("invalid load");
	}
	// push rax
	emit(c, 0x50);
}

emit_jz(c: *assembler, l: *label) {
	// pop rax
	emit(c, 0x58);
	// test rax, rax
	emit(c, 0x48);
	emit(c, 0x85);
	emit(c, 0xc0);
	// jz no
	emit(c, 0x0f);
	emit(c, 0x84);
	addfixup(c, l);
}

emit_lea(c: *assembler, offset: int) {
	// lea rax, [rbp + offset]
	emit(c, 0x48);
	emit(c, 0x8d);
	emit(c, 0x85);
	emit(c, offset);
	emit(c, offset >> 8);
	emit(c, offset >> 16);
	emit(c, offset >> 24);
	// push rax
	emit(c, 0x50);
}

emit_and(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rdx
	emit(c, 0x5a);
	// and rdx, rax
	emit(c, 0x48);
	emit(c, 0x21);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_or(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rdx
	emit(c, 0x5a);
	// or rdx, rax
	emit(c, 0x48);
	emit(c, 0x09);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_xor(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rdx
	emit(c, 0x5a);
	// xor rdx, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_add(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rdx
	emit(c, 0x5a);
	// add rdx, rax
	emit(c, 0x48);
	emit(c, 0x01);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_ret(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// mov rsp, rbp
	emit(c, 0x48);
	emit(c, 0x89);
	emit(c, 0xec);
	// pop rbp
	emit(c, 0x5d);
	// ret
	emit(c, 0xc3);
}

emit_call(c: *assembler, n: int) {
	// pop rax
	emit(c, 0x58);
	// call rax
	emit(c, 0xff);
	emit(c, 0xd0);
	// add rsp, 8*(n+1)
	emit_pop(c, n);
	// push rax
	emit(c, 0x50);
}

emit_gt(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// setg al
	emit(c, 0x0f);
	emit(c, 0x9f);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_lt(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// setl al
	emit(c, 0x0f);
	emit(c, 0x9c);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_ge(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// setge al
	emit(c, 0x0f);
	emit(c, 0x9d);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_le(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// setle al
	emit(c, 0x0f);
	emit(c, 0x9e);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_eq(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// sete al
	emit(c, 0x0f);
	emit(c, 0x94);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_ne(c: *assembler) {
	// pop rdx
	emit(c, 0x5a);
	// pop rcx
	emit(c, 0x59);
	// xor rax, rax
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xc0);
	// cmp rdx, rcx
	emit(c, 0x48);
	emit(c, 0x39);
	emit(c, 0xca);
	// setne al
	emit(c, 0x0f);
	emit(c, 0x95);
	emit(c, 0xc0);
	// mov rax
	emit(c, 0x50);
}

emit_sub(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rdx
	emit(c, 0x5a);
	// add rax, rdx
	emit(c, 0x48);
	emit(c, 0x29);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_mul(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rcx
	emit(c, 0x59);
	// mul rcx
	emit(c, 0x48);
	emit(c, 0xf7);
	emit(c, 0xe1);
	// push rax
	emit(c, 0x50);
}

emit_div(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rcx
	emit(c, 0x59);
	// xor rdx, rdx
	emit(c, 0x48);
	emit(c, 0x31);
	emit(c, 0xd2);
	// test rax, rax
	emit(c, 0x48);
	emit(c, 0x85);
	emit(c, 0xc0);
	// sets dl
	emit(c, 0x0f);
	emit(c, 0x98);
	emit(c, 0xc2);
	// neg rdx
	emit(c, 0x48);
	emit(c, 0xf7);
	emit(c, 0xda);
	// idiv rcx
	emit(c, 0x48);
	emit(c, 0xf7);
	emit(c, 0xf9);
	// push rax
	emit(c, 0x50);
}

emit_mod(c: *assembler) {
        // pop rax
        emit(c, 0x58);
        // pop rcx
        emit(c, 0x59);
        // xor rdx, rdx
        emit(c, 0x48);
        emit(c, 0x31);
        emit(c, 0xd2);
        // test rax, rax
        emit(c, 0x48);
        emit(c, 0x85);
        emit(c, 0xc0);
        // sets dl
        emit(c, 0x0f);
        emit(c, 0x98);
        emit(c, 0xc2);
        // neg rdx
        emit(c, 0x48);
        emit(c, 0xf7);
        emit(c, 0xda);
        // idiv rcx
        emit(c, 0x48);
        emit(c, 0xf7);
        emit(c, 0xf9);
        // push rdx
        emit(c, 0x52);
}

emit_lsh(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rcx
	emit(c, 0x59);
	// shl rax, cl
	emit(c, 0x48);
	emit(c, 0xd3);
	emit(c, 0xe0);
	// push rax
	emit(c, 0x50);
}

emit_rsh(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// pop rcx
	emit(c, 0x59);
	// shr rax, cl
	emit(c, 0x48);
	emit(c, 0xd3);
	emit(c, 0xe8);
	// push rax
	emit(c, 0x50);
}

emit_not(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// neg rax
	emit(c, 0x48);
	emit(c, 0xf7);
	emit(c, 0xd0);
	// push rax
	emit(c, 0x50);
}

emit_neg(c: *assembler) {
	// pop rax
	emit(c, 0x58);
	// neg rax
	emit(c, 0x48);
	emit(c, 0xf7);
	emit(c, 0xd8);
	// push rax
	emit(c, 0x50);
}

emit_syscall(c: *assembler) {
	// mov rax, [rbp + 16]
	emit(c, 0x48);
	emit(c, 0x8b);
	emit(c, 0x45);
	emit(c, 0x10);
	// mov rdi, [rbp + 24]
	emit(c, 0x48);
	emit(c, 0x8b);
	emit(c, 0x7d);
	emit(c, 0x18);
	// mov rsi, [rbp + 32]
	emit(c, 0x48);
	emit(c, 0x8b);
	emit(c, 0x75);
	emit(c, 0x20);
	// mov rdx, [rbp + 40]
	emit(c, 0x48);
	emit(c, 0x8b);
	emit(c, 0x55);
	emit(c, 0x28);
	// mov r10, [rbp + 48]
	emit(c, 0x4c);
	emit(c, 0x8b);
	emit(c, 0x55);
	emit(c, 0x30);
	// mov r8, [rbp + 56]
	emit(c, 0x4c);
	emit(c, 0x8b);
	emit(c, 0x45);
	emit(c, 0x38);
	// mov r9, [rbp + 64]
	emit(c, 0x4c);
	emit(c, 0x8b);
	emit(c, 0x4d);
	emit(c, 0x40);
	// syscall
	emit(c, 0x0f);
	emit(c, 0x05);
	// push rax
	emit(c, 0x50);
}

writeout(c: *assembler, start: *label) {
	var b: *chunk;
	var i: int;
	var text_size: int;
	var load_addr: int;
	var entry: int;

	load_addr = 0x100000;
	text_size = c.at;

	if (!start || !start.fixed) {
		die("_start is not defined");
	}

	entry = load_addr + start.at + 128;
	text_size = text_size + 128;

	// magic
	putchar(c, 0x7f);
	putchar(c, 'E');
	putchar(c, 'L');
	putchar(c, 'F');

	// class
	putchar(c, 2);

	// endian
	putchar(c, 1);

	// version
	putchar(c, 1);

	// abi
	putchar(c, 0);

	// abi version
	putchar(c, 0);

	// padding
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// type
	putchar(c, 2);
	putchar(c, 0);

	// machine
	putchar(c, 62);
	putchar(c, 0);

	// version
	putchar(c, 1);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// entry point
	putchar(c, entry);
	putchar(c, entry >> 8);
	putchar(c, entry >> 16);
	putchar(c, entry >> 24);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phoff
	putchar(c, 64);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// shoff
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// flags
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// ehsize
	putchar(c, 64);
	putchar(c, 0);

	// phentsize
	putchar(c, 56);
	putchar(c, 0);

	// phnum
	putchar(c, 1);
	putchar(c, 0);

	// shentsize
	putchar(c, 64);
	putchar(c, 0);

	// shnum
	putchar(c, 0);
	putchar(c, 0);

	// shstrndx
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].type
	putchar(c, 1);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].flags
	putchar(c, 5);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].offset
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].vaddr
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0x10);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].paddr
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].filesize
	putchar(c, text_size);
	putchar(c, text_size >> 8);
	putchar(c, text_size >> 16);
	putchar(c, text_size >> 24);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].memsize
	putchar(c, text_size);
	putchar(c, text_size >> 8);
	putchar(c, text_size >> 16);
	putchar(c, text_size >> 24);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// phdr[0].align
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// nop sled
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);
	putchar(c, 0x90);

	b = c.text;
	loop {
		if (!b) {
			break;
		}
		i = 0;
		loop {
			if (i >= b.fill) {
				break;
			}
			putchar(c, b.buf[i]: int);
			i = i + 1;
		}
		b = b.next;
	}
}
