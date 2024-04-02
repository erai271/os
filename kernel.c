ud2();

cpuid(a: *int, c: *int, d: *int, b: *int);

inb(a: int): int;
outb(a: int, x: int);

rdmsr(r: int): int;
wrmsr(r: int, x: int);

rdcr0(): int;
wrcr0(x: int);
rdcr2(): int;
wrcr2(x: int);
rdcr3(): int;
wrcr3(x: int);
rdcr4(): int;
wrcr4(x: int);

lgdt(gdtp: int);
lidt(idtp: int);
lldt(s: int);
ltr(s: int);

hlt();

rdflags(): int;
wrflags(x: int);

wbinvld(x: int);
invlpg(x: int);

struct regs {
	rax: int;	// 0
	rcx: int;	// 8
	rdx: int;	// 16
	rbx: int;	// 24
	rsp: int;	// 32
	rbp: int;	// 40
	rsi: int;	// 48
	rdi: int;	// 56
	r8: int;	// 64
	r9: int;	// 72
	r10: int;	// 80
	r11: int;	// 88
	r12: int;	// 96
	r13: int;	// 104
	r14: int;	// 112
	r15: int;	// 120
	rip: int;	// 128
	rflags: int;	// 136
	cs: int;	// 144
	ss: int;	// 152
}

taskswitch(save_regs: *regs, load_regs: *regs);

_start() {
	loop {
		ud2();
	}
}

ru32(p: *byte): int {
	return p[0]:int + (p[1]:int << 8) + (p[2]:int << 16) + (p[3]:int << 24);
}

bootstrap(a: *regs, b: *regs) {
	a.r15 = 15;
	taskswitch(b, a);
	ud2();
}

_kstart(mb: *byte) {
	var a: regs;
	var b: regs;

	b.rax = 1;
	b.rcx = 2;
	b.rdx = 3;
	b.rbx = 4;
	b.rsp = (-0x8000 << 16) + 1024 * 1024 * 2 - 1024 * 32 - 24;
	b.rbp = 0;
	b.rsi = 7;
	b.rdi = 8;
	b.r8 = 9;
	b.r9 = 10;
	b.r10 = 11;
	b.r11 = 12;
	b.r12 = 13;
	b.r13 = 14;
	b.r14 = 15;
	b.r15 = 16;
	b.rip = bootstrap:int;
	b.rflags = 0;
	b.cs = 8;
	b.ss = 16;

	(b.rsp:**regs)[1] = &a;
	(b.rsp:**regs)[2] = &b;

	taskswitch(&a, &b);
	loop {
		hlt();
	}
}
