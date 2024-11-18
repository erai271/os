enum {
	R_RAX,
	R_RCX,
	R_RDX,
	R_RBX,
	R_RSP,
	R_RBP,
	R_RSI,
	R_RDI,
	R_R8,
	R_R9,
	R_R10,
	R_R11,
	R_R12,
	R_R13,
	R_R14,
	R_R15,
	R_RIP,

	R_ES = 0,
	R_CS = 1,
	R_SS = 2,
	R_DS = 3,
	R_FS = 4,
	R_GS = 5,

	R_CR0 = 0,
	R_CR1 = 1,
	R_CR2 = 2,
	R_CR3 = 3,
	R_CR4 = 4,
	R_CR5 = 5,
	R_CR6 = 6,
	R_CR7 = 7,
}

enum {
	CC_O  = 0x00,
	CC_NO = 0x01,
	CC_B  = 0x02,
	CC_AE = 0x03,
	CC_E  = 0x04,
	CC_NE = 0x05,
	CC_BE = 0x06,
	CC_A  = 0x07,
	CC_S  = 0x08,
	CC_NS = 0x09,
	CC_P  = 0x0a,
	CC_NP = 0x0b,
	CC_L  = 0x0c,
	CC_GE = 0x0d,
	CC_LE = 0x0e,
	CC_G  = 0x0f,
}

enum {
	OP_GS = 0x65,
	OP_OS = 0x66,

	OP_CLD = 0xfc,
	OP_CLI = 0xfa,
	OP_STI = 0xfb,
	OP_CPUID = 0x0fa2,
	OP_IN = 0xec,
	OP_IND = 0xed,
	OP_OUT = 0xee,
	OP_OUTD = 0xef,
	OP_HLT = 0xf4,
	OP_NOP = 0x90,
	OP_WBINVD = 0x0f09,
	OP_UD2 = 0x0f0b,

	OP_INVLPGM = 0x070f01,
	OP_LLDM = 0x020f00,
	OP_LTRM = 0x030f00,

	OP_WRSR = 0x8e,
	OP_RDSR = 0x8c,

	OP_RET = 0xc3,
	OP_CALL = 0xe8,
	OP_JMP = 0xe9,
	OP_JCC = 0x0f80,
	OP_SETCC = 0x0f90,

	OP_PUSHF = 0x9c,
	OP_POPF = 0x9d,
	OP_IRET = 0xcf,
	OP_IRETQ = 0x48cf,
	OP_WRMSR = 0x0f30,
	OP_RDMSR = 0x0f32,
	OP_RDCRR = 0x0f20,
	OP_WRCRR = 0x0f22,
	OP_LGDTM = 0x020f01,
	OP_LIDTM = 0x030f01,
	OP_LLDTM = 0x020f00,

	OP_ICALLM = 0x0200ff,

	OP_NOTM = 0x0200f7,
	OP_NEGM = 0x0300f7,

	OP_ANDRM = 0x23,
	OP_ORRM = 0x0b,
	OP_CMPRM = 0x3b,
	OP_TESTRM = 0x85,
	OP_SUBRM = 0x2b,
	OP_ADDRM = 0x03,
	OP_ADCRM = 0x13,
	OP_XORRM = 0x33,

	OP_ANDI = 0x040081,
	OP_ADDI = 0x000081,
	OP_SUBI = 0x050081,
	OP_ORI = 0x010081,
	OP_CMPI = 0x070081,

	OP_RDRAND = 0x060fc7,

	OP_IMULM = 0x0400f7,
	OP_IDIVM = 0x0700f7,
	OP_SHLM = 0x0400d3,
	OP_SHRM = 0x0500d3,

	OP_PUSHR = 0x50,

	OP_POPR = 0x58,

	OP_MOVI = 0x00c7,
	OP_MOVABS = 0xb8,

	OP_SYSCALL = 0x0f05,
	OP_SYSRET = 0x0f07,

	OP_LEA = 0x8d,
	OP_LOAD = 0x8b,
	OP_LOADB = 0x8a,
	OP_LOAD16 = 0x668a,
	OP_STOREB = 0x88,
	OP_STORE = 0x89,
	OP_STORE16 = 0x6689,
	OP_MOVE = 0x8b,
}

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

struct section {
	next: *section;
	name: *byte;
	start: int;
	end: int;
	index: int;
	name_offset: int;
	type: int;
	link: int;
	entsize: int;
}

struct assembler {
	a: *alloc;
	out: *file;
	at: int;
	text: *chunk;
	text_end: *chunk;
	bits32: int;
	symbols: *symbol;
	sections: *section;
	num_sections: int;
}

struct symbol {
	next: *symbol;
	name: *byte;
	label: *label;
	name_offset: int;
}

setup_assembler(a: *alloc): *assembler {
	var c: *assembler;
	c = alloc(a, sizeof(*c)): *assembler;
	c.a = a;
	c.out = 0: *file;
	c.at = 160;  // elf header + program header + multiboot header
	c.text = 0:*chunk;
	c.text_end = 0:*chunk;
	c.bits32 = 0;
	c.symbols = 0:*symbol;
	c.num_sections = 0;
	add_section(c, "", SHT_NULL);
	add_section(c, ".text", SHT_PROGBITS);
	return c;
}

add_section(c: *assembler, name: *byte, type: int) {
	var s: *section;
	var n: *section;
	var end: int;

	end = c.at;

	emit_align(c, 16, OP_NOP);

	s = alloc(c.a, sizeof(*s)):*section;

	s.next = 0:*section;
	s.name = name;
	s.start = c.at;
	s.end = c.at;
	s.index = c.num_sections;
	s.type = type;
	s.link = 0;
	s.entsize = 0;

	n = c.sections;
	if n {
		loop {
			if !n.next {
				break;
			}
			n = n.next;
		}
		n.end = end;
		n.next = s;
	} else {
		c.sections = s;
	}

	c.num_sections = c.num_sections + 1;
}

find_section(c: *assembler, name: *byte): *section {
	var n: *section;
	n = c.sections;
	loop {
		if !n {
			break;
		}

		if strcmp(n.name, name) == 0 {
			break;
		}

		n = n.next;
	}
	return n;
}

putchar(c: *assembler, ch: int) {
	fputc(c.out, ch);
}

open_output(c: *assembler, filename: *byte) {
	var fd: int;

	if (c.out) {
		die("multiple output files");
	}

	unlink(filename);

	fd = open(filename, O_CREAT | O_WRONLY, (7 << 6) + (7 << 3) +7);
	if (fd < 0) {
		die("failed to open output");
	}

	c.out = fopen(fd, c.a);
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

	if (c.text_end.fill < 4) {
		die("invalid fixup");
	}

	here = &c.text_end.buf[c.text_end.fill - 4];

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

add_symbol(c: *assembler, name: *byte, l: *label) {
	var s: *symbol;

	s = alloc(c.a, sizeof(*s)):*symbol;

	s.next = c.symbols;
	s.name = name;
	s.label = l;

	c.symbols = s;
}

emit_restorer(c: *assembler) {
	as_modri(c, OP_MOVI, R_RAX, 15);
	as_op(c, OP_SYSCALL);
}

emit_ptr(c: *assembler, l: *label) {
	reserve(c, 16);
	as_modrm(c, OP_LEA, R_RAX, R_RIP, 0, 0, 128);
	addfixup(c, l);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_jmp(c: *assembler, l: *label) {
	as_jmp(c, OP_JMP, l);
}

emit_num(c: *assembler, x: int) {
	as_opri64(c, OP_MOVABS, R_RDX, x);
	as_opr(c, OP_PUSHR, R_RDX);
}

emit_blob(c: *assembler, s: *byte, n: int) {
	var a: *label;
	var b: *label;
	var i: int;

	a = mklabel(c);
	b = mklabel(c);

	as_jmp(c, OP_JMP, b);

	fixup_label(c, a);

	i = 0;
	loop {
		if i == n {
			break;
		}
		as_emit(c, s[i]:int);
		i = i + 1;
	}
	as_emit(c, 0);

	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);

	fixup_label(c, b);
	emit_ptr(c, a);
}

emit_str(c: *assembler, s: *byte) {
	var a: *label;
	var b: *label;
	var i: int;

	a = mklabel(c);
	b = mklabel(c);

	as_jmp(c, OP_JMP, b);

	fixup_label(c, a);

	i = 0;
	loop {
		if (!s[i]) {
			break;
		}
		as_emit(c, s[i]:int);
		i = i + 1;
	}
	as_emit(c, 0);

	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);
	as_op(c, OP_NOP);

	fixup_label(c, b);
	emit_ptr(c, a);
}

emit_pop(c: *assembler, n: int) {
	as_modri(c, OP_ADDI, R_RSP, n << 3);
}

emit_kstart(c: *assembler) {
	var hang: *label;
	var do_iret: *label;
	var do_ret: *label;
	var done: *label;

	c.bits32 = 1;

	hang = mklabel(c);
	do_iret = mklabel(c);
	do_ret = mklabel(c);
	done = mklabel(c);

	// Check for valid multiboot magic
	as_modri(c, OP_MOVI, R_RDX, 0x2badb002);
	as_modrr(c, OP_CMPRM, R_RAX, R_RDX);
	as_jmp(c, OP_JCC + CC_NE, hang);

	// Setup an early stack
	as_modri(c, OP_MOVI, R_RSP, 0x00300000);  // FIXME bss

	// Align stack to page
	as_modri(c, OP_ANDI, R_RSP, -0x1000);

	// pt3 -> 1g
	as_modri(c, OP_SUBI, R_RSP, 0x1000);
	as_modri(c, OP_MOVI, R_RAX, 0x83);
	as_modri(c, OP_MOVI, R_RDX, 0);
	as_modrm(c, OP_STORE, R_RAX, R_RSP, 0, 0, 0);
	as_modrm(c, OP_STORE, R_RDX, R_RSP, 0, 0, 4);
	as_modrm(c, OP_STORE, R_RAX, R_RSP, 0, 0, 510 * 8 + 0);
	as_modrm(c, OP_STORE, R_RDX, R_RSP, 0, 0, 510 * 8 + 4);

	// pt4 -> pt3
	as_modrr(c, OP_MOVE, R_RAX, R_RSP);
	as_modri(c, OP_SUBI, R_RSP, 0x1000);
	as_modri(c, OP_ORI, R_RAX, 3);
	as_modri(c, OP_MOVI, R_RDX, 0);
	as_modrm(c, OP_STORE, R_RAX, R_RSP, 0, 0, 0);
	as_modrm(c, OP_STORE, R_RDX, R_RSP, 0, 0, 4);
	as_modrm(c, OP_STORE, R_RAX, R_RSP, 0, 0, 511 * 8 + 0);
	as_modrm(c, OP_STORE, R_RDX, R_RSP, 0, 0, 511 * 8 + 4);

	// Load page table pt4
	as_modrr(c, OP_WRCRR, R_CR3, R_RSP);

	// Allocate space for the gdt
	as_modri(c, OP_SUBI, R_RSP, 64);
	as_modrr(c, OP_MOVE, R_RBP, R_RSP);

	// Null Segment
	as_modri(c, OP_MOVI, R_RAX, 0x00000000);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 8);
	as_modri(c, OP_MOVI, R_RAX, 0x00000000);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 12);

	// Kernel code segment
	as_modri(c, OP_MOVI, R_RAX, 0x00000000);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 16);
	as_modri(c, OP_MOVI, R_RAX, 0x00209800);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 20);

	// Kernel data segment
	as_modri(c, OP_MOVI, R_RAX, 0x00000000);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 24);
	as_modri(c, OP_MOVI, R_RAX, 0x00009200);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 28);

	// Load gdt
	as_modri(c, OP_MOVI, R_RAX, 23);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 0);
	as_modrm(c, OP_LEA, R_RAX, R_RBP, 0, 0, 8);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 2);
	as_modm(c, OP_LGDTM, R_RBP, 0, 0, 0);

	// Load null lldt
	as_modri(c, OP_MOVI, R_RAX, 0);
	as_modr(c, OP_LLDTM, R_RAX);

	// Load null idt
	as_modm(c, OP_LIDTM, R_RBP, 0, 0, 8);

	// Enable pae
	as_modri(c, OP_MOVI, R_RAX, 0xa0);
	as_modrr(c, OP_WRCRR, R_CR4, R_RAX);

	// Enable long mode
	as_modri(c, OP_MOVI, R_RCX, (-1 << 32) + (0xc0 << 24) + 0x000080);
	as_op(c, OP_RDMSR);
	as_modri(c, OP_ORI, R_RAX, 0x100);
	as_op(c, OP_WRMSR);

	// Enable paging
	as_modrr(c, OP_RDCRR, R_CR0, R_RAX);
	as_modri(c, OP_ORI, R_RAX, (-0x80000000) | 0x0001);
	as_modrr(c, OP_WRCRR, R_CR0, R_RAX);

	// flags
	as_modri(c, OP_MOVI, R_RAX, 0);
	as_opr(c, OP_PUSHR, R_RAX);
	// cs
	as_modri(c, OP_MOVI, R_RAX, 8);
	as_opr(c, OP_PUSHR, R_RAX);
	// pointer
	as_jmp(c, OP_CALL, do_iret);

	c.bits32 = 0;

	// Jump to top half
	as_jmp(c, OP_CALL, do_ret);

	// Reload the gdt in the top half
	as_modri(c, OP_ORI, R_RBP, -0x80000000);
	as_modri(c, OP_MOVI, R_RAX, 23);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 0);
	as_modrm(c, OP_LEA, R_RAX, R_RBP, 0, 0, 8);
	as_modrm(c, OP_STORE, R_RAX, R_RBP, 0, 0, 2);
	as_modm(c, OP_LGDTM, R_RBP, 0, 0, 0);

	// Reload segments
	as_modri(c, OP_MOVI, R_RAX, 16);
	as_modrr(c, OP_WRSR, R_ES, R_RAX);
	as_modrr(c, OP_WRSR, R_DS, R_RAX);
	as_modrr(c, OP_WRSR, R_FS, R_RAX);
	as_modrr(c, OP_WRSR, R_GS, R_RAX);
	as_modrr(c, OP_WRSR, R_SS, R_RAX);
	as_modrr(c, OP_MOVE, R_RSP, R_RSP);

	// Reload stack in the top half
	as_modri(c, OP_ORI, R_RSP, -0x80000000);

	// Kill the lower mapping
	as_modri(c, OP_MOVI, R_RAX, 0);
	as_modrm(c, OP_LEA, R_RDI, R_RBP, 0, 0, 64);
	as_modrm(c, OP_STORE, R_RAX, R_RDI, 0, 0, 4096);
	as_modrm(c, OP_STORE, R_RAX, R_RDI, 0, 0, 0);
	as_modri(c, OP_ANDI, R_RDI, 0x7fffffff);
	as_modrr(c, OP_WRCRR, R_CR3, R_RDI);

	// Setup a call frame for _kstart
	as_jmp(c, OP_JMP, done);

	// hlt forever
	fixup_label(c, hang);
	as_op(c, OP_CLI);
	as_op(c, OP_HLT);
	as_jmp(c, OP_JMP, hang);

	// iret to long mode
	fixup_label(c, do_iret);
	as_op(c, OP_IRET);

	// ret to top half
	fixup_label(c, do_ret);
	as_opr(c, OP_POPR, R_RAX);
	as_modri(c, OP_ORI, R_RAX, -0x80000000);
	as_opr(c, OP_PUSHR, R_RAX);
	as_op(c, OP_RET);

	// Setup a call frame for _kstart
	fixup_label(c, done);
	as_modrr(c, OP_XORRM, R_RBP, R_RBP);
	as_opr(c, OP_PUSHR, R_RBX);
	as_opr(c, OP_PUSHR, R_RBP);
}

emit_preamble(c: *assembler, n: int, pragma: int) {
	var i: int;
	if (pragma == 1) {
		as_modrr(c, OP_XORRM, R_RBP, R_RBP);
		as_modrm(c, OP_LOAD, R_RDI, R_RSP, 0, 0, 0);
		as_modrm(c, OP_LEA, R_RSI, R_RSP, 0, 0, 8);
		as_modrm(c, OP_LEA, R_RDX, R_RSI, R_RDI, 8, 8);
		as_opr(c, OP_PUSHR, R_RDX);
		as_opr(c, OP_PUSHR, R_RSI);
		as_opr(c, OP_PUSHR, R_RDI);
		as_opr(c, OP_PUSHR, R_RBP);
	} else if (pragma > 1) {
		emit_kstart(c);
	}
	as_opr(c, OP_PUSHR, R_RBP);
	as_modrr(c, OP_MOVE, R_RBP, R_RSP);
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
	as_opr(c, OP_POPR, R_RDI);
	as_opr(c, OP_POPR, R_RAX);
	if (t.kind == TY_BYTE) {
		as_modrm(c, OP_STOREB, R_RAX, R_RDI, 0, 0, 0);
	} else if (type_isprim(t)) {
		as_modrm(c, OP_STORE, R_RAX, R_RDI, 0, 0, 0);
	} else {
		die("invalid store");
	}
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_load(c: *assembler, t: *type) {
	as_opr(c, OP_POPR, R_RDI);
	if (t.kind == TY_BYTE) {
		as_modrr(c, OP_XORRM, R_RAX, R_RAX);
		as_modrm(c, OP_LOADB, R_RAX, R_RDI, 0, 0, 0);
	} else if (type_isprim(t)) {
		as_modrm(c, OP_LOAD, R_RAX, R_RDI, 0, 0, 0);
	} else {
		die("invalid load");
	}
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_jz(c: *assembler, l: *label) {
	as_opr(c, OP_POPR, R_RAX);
	as_modrr(c, OP_TESTRM, R_RAX, R_RAX);
	as_jmp(c, OP_JCC + CC_E, l);
}

emit_lea(c: *assembler, offset: int) {
	as_modrm(c, OP_LEA, R_RAX, R_RBP, 0, 0, offset);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_and(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RDX);
	as_modrr(c, OP_ANDRM, R_RAX, R_RDX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_or(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RDX);
	as_modrr(c, OP_ORRM, R_RAX, R_RDX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_xor(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RDX);
	as_modrr(c, OP_XORRM, R_RAX, R_RDX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_add(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RDX);
	as_modrr(c, OP_ADDRM, R_RAX, R_RDX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_ud(c: *assembler) {
	as_op(c, OP_UD2);
}

emit_ret(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_modrr(c, OP_MOVE, R_RSP, R_RBP);
	as_opr(c, OP_POPR, R_RBP);
	as_op(c, OP_RET);
}

emit_call(c: *assembler, n: int) {
	as_opr(c, OP_POPR, R_RAX);
	as_modr(c, OP_ICALLM, R_RAX);
	emit_pop(c, n);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_lcall(c: *assembler, l: *label, n: int) {
	as_jmp(c, OP_CALL, l);
	emit_pop(c, n);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_gt(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_G, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_lt(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_L, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_ge(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_GE, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_le(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_LE, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_eq(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_E, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_ne(c: *assembler) {
	as_opr(c, OP_POPR, R_RDX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RAX, R_RAX);
	as_modrr(c, OP_CMPRM, R_RDX, R_RCX);
	as_modrr(c, OP_SETCC + CC_NE, 0, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_sub(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RDX);
	as_modrr(c, OP_SUBRM, R_RAX, R_RDX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_mul(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RCX);
	as_modr(c, OP_IMULM, R_RCX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_div(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RDX, R_RDX);
	as_modrr(c, OP_TESTRM, R_RAX, R_RAX);
	as_modrr(c, OP_SETCC + CC_S, 0, R_RDX);
	as_modr(c, OP_NEGM, R_RDX);
	as_modr(c, OP_IDIVM, R_RCX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_mod(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RCX);
	as_modrr(c, OP_XORRM, R_RDX, R_RDX);
	as_modrr(c, OP_TESTRM, R_RAX, R_RAX);
	as_modrr(c, OP_SETCC + CC_S, 0, R_RDX);
	as_modr(c, OP_NEGM, R_RDX);
	as_modr(c, OP_IDIVM, R_RCX);
	as_opr(c, OP_PUSHR, R_RDX);
}

emit_lsh(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RCX);
	as_modr(c, OP_SHLM, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_rsh(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_opr(c, OP_POPR, R_RCX);
	as_modr(c, OP_SHRM, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_not(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_modr(c, OP_NOTM, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_neg(c: *assembler) {
	as_opr(c, OP_POPR, R_RAX);
	as_modr(c, OP_NEGM, R_RAX);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_syscall(c: *assembler) {
	as_modrm(c, OP_LOAD, R_RAX, R_RBP, 0, 0, 16);
	as_modrm(c, OP_LOAD, R_RDI, R_RBP, 0, 0, 24);
	as_modrm(c, OP_LOAD, R_RSI, R_RBP, 0, 0, 32);
	as_modrm(c, OP_LOAD, R_RDX, R_RBP, 0, 0, 40);
	as_modrm(c, OP_LOAD, R_R10, R_RBP, 0, 0, 48);
	as_modrm(c, OP_LOAD, R_R8, R_RBP, 0, 0, 56);
	as_modrm(c, OP_LOAD, R_R9, R_RBP, 0, 0, 64);
	as_op(c, OP_SYSCALL);
	as_opr(c, OP_PUSHR, R_RAX);
}

emit_align(c: *assembler, n: int, b: int) {
	var pad: int;

	pad = c.at & (n - 1);

	if pad == 0 {
		return;
	}

	loop {
		if pad == n {
			break;
		}

		as_emit(c, b);

		pad = pad + 1;
	}
}

emit_strtab_str(c: *assembler, s: *byte): int {
	var i: int;

	i = 0;
	loop {
		as_emit(c, s[i]:int);

		if !s[i] {
			break;
		}

		i = i + 1;
	}

	return i + 1;
}

enum {
	SHT_NULL = 0,
	SHT_PROGBITS = 1,
	SHT_SYMTAB = 2,
	SHT_STRTAB = 3,
}

emit_sections(c: *assembler): int {
	var at: int;
	var s: *section;
	var y: *symbol;
	var n: int;

	add_symbol(c, "", 0:*label);

	s = find_section(c, "");
	s.start = 0;
	s.end = 0;

	add_section(c, ".strtab", SHT_STRTAB);

	y = c.symbols;
	loop {
		if !y {
			break;
		}

		y.name_offset = n;
		n = n + emit_strtab_str(c, y.name);

		y = y.next;
	}

	add_section(c, ".symtab", SHT_SYMTAB);

	y = c.symbols;
	loop {
		if !y {
			break;
		}

		// name
		n = y.name_offset;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);

		// info
		if y.label {
			n = 0x12;
		} else {
			n = 0;
		}
		as_emit(c, n);

		// other
		as_emit(c, 0);

		// section
		if y.label {
			n = 1;
		} else {
			n = 0;
		}
		as_emit(c, n);
		as_emit(c, n >> 8);

		// addr
		if y.label {
			n = y.label.at + 0x100000;
		} else {
			n = 0;
		}
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);
		as_emit(c, n >> 32);
		as_emit(c, n >> 40);
		as_emit(c, n >> 48);
		as_emit(c, n >> 56);

		// size
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);

		y = y.next;
	}

	s = find_section(c, ".strtab");
	n = s.index;

	s = find_section(c, ".symtab");
	s.link = n;
	s.entsize = 0x18;

	add_section(c, ".shstrtab", SHT_STRTAB);

	s = c.sections;
	n = 0;
	loop {
		if !s {
			break;
		}

		s.name_offset = n;
		n = n + emit_strtab_str(c, s.name);

		s = s.next;
	}

	s = find_section(c, ".shstrtab");
	s.end = c.at;

	emit_align(c, 16, OP_NOP);

	at = c.at;

	s = c.sections;
	loop {
		if !s {
			break;
		}

		// Name
		n = s.name_offset;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);

		// Type
		n = s.type;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);

		// Flags
		as_emit(c, 6);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);

		// Addr
		if s.start == s.end {
			 n = 0;
		} else {
			n = s.start + 0x100000;
		}
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);
		as_emit(c, n >> 32);
		as_emit(c, n >> 40);
		as_emit(c, n >> 48);
		as_emit(c, n >> 56);

		// Offset
		n = s.start;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);
		as_emit(c, n >> 32);
		as_emit(c, n >> 40);
		as_emit(c, n >> 48);
		as_emit(c, n >> 56);

		// Size
		n = s.end - s.start;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);
		as_emit(c, n >> 32);
		as_emit(c, n >> 40);
		as_emit(c, n >> 48);
		as_emit(c, n >> 56);

		// Link
		n = s.link;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);

		// Info
		if s.type == SHT_SYMTAB {
			n = 1;
		} else {
			n = 0;
		}
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);

		// align
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);
		as_emit(c, 0);

		// entsize
		n = s.entsize;
		as_emit(c, n);
		as_emit(c, n >> 8);
		as_emit(c, n >> 16);
		as_emit(c, n >> 24);
		as_emit(c, n >> 32);
		as_emit(c, n >> 40);
		as_emit(c, n >> 48);
		as_emit(c, n >> 56);

		s = s.next;
	}

	return at;
}

writeout(c: *assembler, start: *label, kstart: *label) {
	var b: *chunk;
	var i: int;
	var text_size: int;
	var text_end: int;
	var load_addr: int;
	var entry: int;
	var kentry: int;
	var mb_magic: int;
	var mb_flags: int;
	var mb_checksum: int;
	var mb_addr: int;
	var s: *section;
	var shoff: int;

	if (!c.out) {
		die("output not opened");
	}

	load_addr = 0x100000;
	text_size = c.at;

	if (!start || !start.fixed) {
		if !kstart {
			die("_start is not defined");
		}
	} else {
		entry = load_addr + start.at;
	}

	text_size = text_size;
	text_end = load_addr + text_size;

	mb_magic = 0x1badb002;
	mb_flags = 0x00010003;
	mb_checksum = -(mb_magic + mb_flags);
	mb_addr = load_addr + 120;

	if (kstart && kstart.fixed) {
		kentry = load_addr + kstart.at;
	} else {
		mb_magic = 0;
		kentry = 0;
	}

	shoff = emit_sections(c: *assembler);

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
	putchar(c, shoff);
	putchar(c, shoff >> 8);
	putchar(c, shoff >> 16);
	putchar(c, shoff >> 24);
	putchar(c, shoff >> 32);
	putchar(c, shoff >> 40);
	putchar(c, shoff >> 48);
	putchar(c, shoff >> 56);

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
	putchar(c, c.num_sections);
	putchar(c, c.num_sections >> 8);

	s = find_section(c, ".shstrtab");

	// shstrndx
	putchar(c, s.index);
	putchar(c, s.index >> 8);

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

	// multiboot magic
	putchar(c, mb_magic);
	putchar(c, mb_magic >> 8);
	putchar(c, mb_magic >> 16);
	putchar(c, mb_magic >> 24);

	// multiboot flags
	putchar(c, mb_flags);
	putchar(c, mb_flags >> 8);
	putchar(c, mb_flags >> 16);
	putchar(c, mb_flags >> 24);

	// multboot checksum
	putchar(c, mb_checksum);
	putchar(c, mb_checksum >> 8);
	putchar(c, mb_checksum >> 16);
	putchar(c, mb_checksum >> 24);

	// multiboot header_addr
	putchar(c, mb_addr);
	putchar(c, mb_addr >> 8);
	putchar(c, mb_addr >> 16);
	putchar(c, mb_addr >> 24);

	// multiboot load_addr
	putchar(c, load_addr);
	putchar(c, load_addr >> 8);
	putchar(c, load_addr >> 16);
	putchar(c, load_addr >> 24);

	// multiboot load_end_addr
	putchar(c, text_end);
	putchar(c, text_end >> 8);
	putchar(c, text_end >> 16);
	putchar(c, text_end >> 24);

	// multiboot bss_end_addr
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);
	putchar(c, 0);

	// entry_addr
	putchar(c, kentry);
	putchar(c, kentry >> 8);
	putchar(c, kentry >> 16);
	putchar(c, kentry >> 24);

	// nop sled
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);
	putchar(c, OP_NOP);

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

	fflush(c.out);
}

as_emit(a: *assembler, b: int) {
	emit(a, b);
}

as_rex(a: *assembler, op: int, r: int, i: int, b: int) {
	var w: int;
	if a.bits32 {
		return;
	}
	w = 0x08;
	if op == OP_LOADB || op == OP_STOREB {
		w = 0;
	}
	as_emit(a, 0x40 + w + ((r >> 1) & 4) + ((i >> 2) & 2) + ((b >> 3) & 1));
}

as_op(a: *assembler, op: int) {
	if op > 0xff {
		as_emit(a, op >> 8);
		as_emit(a, op);
	} else {
		as_emit(a, op);
	}
}

// op + r
as_opr(a: *assembler, op: int, r: int) {
	if r < 0 || r > 15 {
		die("invalid reg");
	}
	if op != OP_PUSHR && op != OP_POPR || r > 7 {
		as_rex(a, op, r, 0, 0);
	}
	as_op(a, op + (r & 7));
}

as_opri64(a: *assembler, op: int, r: int, x: int) {
	if op != OP_MOVABS {
		die("only movabs");
	}
	as_opr(a, op, r);
	as_emit(a, x);
	as_emit(a, x >> 8);
	as_emit(a, x >> 16);
	as_emit(a, x >> 24);
	as_emit(a, x >> 32);
	as_emit(a, x >> 40);
	as_emit(a, x >> 48);
	as_emit(a, x >> 56);
}

// modrm
as_modrr(a: *assembler, op: int, r: int, b: int) {
	if r < 0 || r > 15 || b < 0 || b > 15 {
		die("invalid reg");
	}
	as_rex(a, op, r, 0, b);
	as_op(a, op);
	as_emit(a, 0xc0 + ((r << 3) & 0x38) + (b & 0x07));
}

// modrm /op
as_modr(a: *assembler, op: int, b: int) {
	as_modrr(a, op & 0xffff, op >> 16, b);
}

// modrm + disp
as_modra(a: *assembler, op: int, r: int, d: int) {
	as_rex(a, op, r, 0, 0);
	as_op(a, op);
	as_emit(a, ((r << 3) & 0x38) + R_RSP);
	as_emit(a, (R_RSP << 3) + R_RBP);
	as_emit(a, d);
	as_emit(a, d >> 8);
	as_emit(a, d >> 16);
	as_emit(a, d >> 24);
}

// modrm + sib + disp
as_modrm(a: *assembler, op: int, r: int, b: int, i: int, s: int, d: int) {
	var sib: int;
	var mod: int;
	var rm: int;
	var dw: int;

	if r < 0 || r > 15 {
		die("invalid reg");
	}

	rm = (r << 3) & 0x38;

	if d != 0 {
		if d >= -128 && d <= 127 {
			mod = 1;
			dw = 1;
		} else {
			mod = 2;
			dw = 4;
		}
	} else {
		mod = 0;
		dw = 0;
	}

	if mod == 0 {
		if b < 0 || b > 16 {
			die("invalid reg");
		}

		if s {
			if b == R_RIP {
				die("invalid base");
			}

			if i == R_RSP {
				die("invalid index");
			}

			rm = rm + R_RSP;
		} else {
			if i != 0 {
				die("invalid index");
			}

			if b == R_RIP {
				mod = 0;
				dw = 4;
				rm = rm + R_RBP;
			} else if b == R_RSP || b == R_R12 {
				s = 1;
				i = R_RSP;
				rm = rm + R_RSP;
			} else if b == R_RBP || b == R_R13 {
				mod = 1;
				dw = 1;
				rm = rm + R_RBP;
			} else {
				rm = rm + (b & 7);
			}
		}
	} else {
		if b < 0 || b > 16 || i < 0 || i > 15 {
			die("invalid reg");
		}

		if s {
			if b == R_RIP {
				die("invalid base");
			}

			if i == R_RSP {
				die("invalid index");
			}

			rm = rm + R_RSP;
		} else {
			if i != 0 {
				die("invalid index");
			}

			if b == R_RIP {
				mod = 0;
				dw = 4;
				rm = rm + R_RBP;
			} else if b == R_RSP || b == R_R12 {
				s = 1;
				i = R_RSP;
				rm = rm + R_RSP;
			} else {
				rm = rm + (b & 7);
			}
		}
	}

	as_rex(a, op, r, i, b);
	as_op(a, op);
	as_emit(a, (mod << 6) + rm);

	if s {
		sib = ((i << 3) & 0x38) + (b & 0x07);
		if s == 2 {
			sib = sib + 0x40;
		} else if s == 4 {
			sib = sib + 0x80;
		} else if s == 8 {
			sib = sib + 0xc0;
		} else if s != 1 {
			die("invalid scale");
		}
		as_emit(a, sib);
	}

	if dw == 1 {
		as_emit(a, d);
	} else if dw == 4 {
		as_emit(a, d);
		as_emit(a, d >> 8);
		as_emit(a, d >> 16);
		as_emit(a, d >> 24);
	}
}

// modrm /op
as_modm(a: *assembler, op: int, b: int, i: int, s: int, d: int) {
	as_modrm(a, op & 0xffff, op >> 16, b, i, s, d);
}

as_modri(a: *assembler, op: int, r: int, x: int) {
	if x < -(1 << 31) || x >= (1 << 31) {
		die("immediate too large");
	}
	as_modrr(a, op & 0xffff, op >> 16, r);
	as_emit(a, x);
	as_emit(a, x >> 8);
	as_emit(a, x >> 16);
	as_emit(a, x >> 24);
}

as_jmp(a: *assembler, op: int, l: *label) {
	reserve(a, 16);
	as_op(a, op);
	as_emit(a, 0);
	as_emit(a, 0);
	as_emit(a, 0);
	as_emit(a, 0);
	addfixup(a, l);
}
