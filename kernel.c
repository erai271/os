ud2();

cpuid(a: *int, c: *int, d: *int, b: *int);

inb(a: int): int;
outb(a: int, x: int);

inw(a: int): int;
outw(a: int, x: int);

ind(a: int): int;
outd(a: int, x: int);

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

lgdt(base: *int, size: int);
lidt(base: *int, size: int);
lldt(s: int);
ltr(s: int);
lseg(cs: int, ds: int);

hlt();

cli();
sti();
rdflags(): int;
wrflags(x: int);

wbinvld(x: int);
invlpg(x: int);

_isr0();

_rgs(x: int): int;

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
	trap: int;	// 160
	err: int;	// 168
			// 176
}

taskswitch(save_regs: *regs, load_regs: *regs);

_start() {
	loop {
		ud2();
	}
}

_r32(p: *byte): int;
_w32(p: *byte, x: int): int;

_r16(p: *byte): int;
_w16(p: *byte, x: int): int;

// FIXME a very dumb and lossy but simple serial debug

enum {
	IO_COM0 = 0x3f8,
	IO_PIC1 = 0x20,
	IO_PIC2 = 0xa0,
	IO_PIT = 0x40,
}

setup_serial() {
	outb(IO_COM0 + 1, 0x00);
	outb(IO_COM0 + 3, 0x80);
	outb(IO_COM0 + 0, 0x03);
	outb(IO_COM0 + 1, 0x00);
	outb(IO_COM0 + 3, 0x03);
	outb(IO_COM0 + 2, 0xc7);
	outb(IO_COM0 + 4, 0x0f);

	outb(IO_COM0, 0x1b);
	outb(IO_COM0, '[');
	outb(IO_COM0, 'H');

	outb(IO_COM0, 0x1b);
	outb(IO_COM0, '[');
	outb(IO_COM0, 'J');
}

sputc(c: int) {
	outb(IO_COM0, c);
}

sputs(s: *byte) {
	var i: int;
	i = 0;
	loop {
		if !s[i] {
			break;
		}
		sputc(s[i]:int);
		i = i + 1;
	}
}

sputd(x: int) {
	var d: int;
	if x < 0 {
		d = -(x % -10);
		x = x / -10;
		sputc('-');
		if x {
			sputd(x);
		}
		sputc('0' + d);
	} else {
		d = x % 10;
		x = x / 10;
		if x {
			sputd(x);
		}
		sputc('0' + d);
	}
}

sputh(x: int) {
	var d: int;
	d = x & 15;
	x = x >> 4;
	if x {
		sputh(x);
	}
	sputc("0123456789abcdef"[d]:int);
}

gdt_tss(s: *int, base: int, size: int, access: int, flags: int) {
	s[0] = (((base & ~0xffff) << 32)
		+ ((flags & 0xf) << 52)
		+ (((size - 1) & 0xf0000) << 32)
		+ ((access & 0xff) << 40)
		+ ((base & 0xff0000) << 16)
		+ ((base & 0xffff) << 16)
		+ ((size - 1) & 0xffff));
	s[1] = base >> 32;
}

idt_gate(s: *int, offset: int, dpl: int, ist: int) {
	s[0] = (((offset & ~0xffff) << 32)
		+ (1 << 47)
		+ ((dpl & 3) << 45)
		+ (0x0e << 40)
		+ ((ist & 7) << 32)
		+ (8 << 16)
		+ (offset & 0xffff));
	s[1] = offset >> 32;
}

bzero(s: *byte, size: int) {
	var i: int;
	i = 0;
	loop {
		if i == size {
			break;
		}
		s[i] = 0:byte;
		i = i + 1;
	}
}

panic(r: *regs) {
	cli();

	kputs("\n");
	kputh(r.rax);
	kputs(" ");
	kputh(r.rcx);
	kputs(" ");
	kputh(r.rdx);
	kputs(" ");
	kputh(r.rbx);
	kputs("\n");

	kputh(r.rsi);
	kputs(" ");
	kputh(r.rdi);
	kputs(" ");
	kputh(r.r8);
	kputs(" ");
	kputh(r.r9);
	kputs("\n");

	kputh(r.r10);
	kputs(" ");
	kputh(r.r11);
	kputs(" ");
	kputh(r.r12);
	kputs(" ");
	kputh(r.r13);
	kputs("\n");

	kputh(r.r14);
	kputs(" ");
	kputh(r.r15);
	kputs(" ");
	kputh(r.rbp);
	kputs(" ");
	kputh(r.rsp);
	kputs(" ");
	kputh16(r.ss);
	kputs("\n");

	kputh(r.rip);
	kputs(" ");
	kputh(r.rflags);
	kputs(" ");
	kputh(rdcr2());
	kputs(" ");
	kputh(rdcr4());
	kputs(" ");
	kputh16(r.cs);
	kputs("\n");

	loop {
		hlt();
	}
}

_isr(r: *regs) {
	var global: *global;
	global = g();

	if (r.trap < 32) {
		kputs("EX ");
		kputd(r.trap);
		kputs(":");
		kputd(r.err);
		kputs("\n");
		panic(r);
		loop {
			hlt();
		}
	} else if (r.trap == 32) {
		tick();
		outb(IO_PIC1, 0x20);
	} else {
		if (r.trap == 33) {
			isr_realtek();
		} else if (r.trap == 34) {
			isr_ahci();
		}
		// apic end of interrupt
		_w32(&global.lapic[0xb0], 0);
	}
}

fill_idt(s: *int) {
	var i: int;
	var ist: int;
	loop {
		if i == 256 {
			break;
		}
		if i == 2 {
			ist = 2;
		} else {
			ist = 1;
		}
		idt_gate(&s[i * 2], _isr0:int + i * 16, 0, ist);
		i = i + 1;
	}
}

memcmp(a: *byte, b: *byte, n: int): int {
	var i: int;

	i = 0;

	loop {
		if i == n {
			return 0;
		}

		if (a[i] > b[i]) {
			return 1;
		}

		if (a[i] < b[i]) {
			return -1;
		}

		i = i + 1;
	}
}

bytesum(a: *byte, n: int): byte {
	var i: int;
	var x: byte;

	i = 0;
	x = 0:byte;
	loop {
		if i == n {
			return x;
		}

		x = x + a[i];

		i = i + 1;
	}
}

ptov(p: int): *byte {
	if p < (1 << 30) {
		return (p + (-0x8000 << 16)): *byte;
	} else {
		return (p + (-1 << 47)): *byte;
	}
}

vtop(v: *byte): int {
	var va: int;
	va = v:int;

	// direct map
	if va >= (-1 << 47) && va < (-1 << 46) {
		return va & ((1 << 46) - 1);
	}

	// -2G to -1G is mapped to 0 - 1G
	if va >= (-1 << 31) && va < (-1 << 30) {
		return va & ((1 << 30) - 1);
	}

	kdie("BAD VA");
}

acpi_len(p: int): int {
	if p == 0 {
		return 0;
	}
	return _r32((ptov(p):int + 4):*byte);
}

valid_table(p: int, sig: *byte): int {
	var len: int;
	var v: *byte;

	v = ptov(p);

	if memcmp(v, sig, 4) {
		return 0;
	}

	len = acpi_len(p);
	if len < 36 {
		return 0;
	}

	if bytesum(v, len):int != 0 {
		return 0;
	}

	return len;
}

kputh(x: int) {
	_kputh(x, 64);
}

kputh8(x: int) {
	_kputh(x, 8);
}

kputh16(x: int) {
	_kputh(x, 16);
}

kputh32(x: int) {
	_kputh(x, 32);
}

_kputh(x: int, d: int) {
	loop {
		if d == 0 {
			break;
		}
		d = d - 4;
		kputc("0123456789abcdef"[(x >> d) & 15]:int);
	}
}

kputd(x: int) {
	var a: int;

	if x < 0 {
		kputc('-');
		a = -(x % 10);
		x = x / -10;
	} else {
		a = x % 10;
		x = x / 10;
	}

	if x != 0 {
		kputd(x);
	}

	kputc('0' + a);
}

find_xsdt(): int {
	var p: int;
	var v: *byte;
	var len: int;

	// Find the RDSP
	p = 0xe0000;
	loop {
		v = ptov(p);

		if p + 20 >= 0xfffff {
                        return 0;
		}

		if !memcmp(v, "RSD PTR ", 8) {
			if !bytesum(v, 20) {
				break;
			}
		}

		p = p + 16;
	}

	// Revision
	if v[15]:int != 2 {
		return 0;
	}

	len = _r32(&v[20]);
	if len < 36 || len > 128 {
		return 0;
	}

	if bytesum(v, len) {
		return 0;
	}

	p = (&v[24]):*int[0];
	if valid_table(p, "XSDT") == 0 {
		return 0;
	}

	return p;
}

find_acpi(xsdt: int, sig: *byte): int {
	var i: int;
	var len: int;
	var p: int;

	if xsdt == 0 {
		return 0;
	}

	i = 36;
	len = acpi_len(xsdt);

	loop {
		if i >= len {
			return 0;
		}

		p = ptov(xsdt + i):*int[0];

		if valid_table(p, sig) != 0 {
			return p;
		}

		i = i + 8;
	}
}

map_pci(pa: int): *byte {
	var global: *global;
	var va: int;
	var pt4p: int;
	var pt4: *int;
	var pt3: *int;
	var v2: int;
	var flags: int;
	global = g();
	global.mmio = global.mmio - (1 << 31);
	va = global.mmio;
	pt4p = rdcr3();
	pt4 = ptov(pt4p):*int;
	v2 = ((va: int) >> 30) & 511;
	pt3 = ptov(pt4[511] & -4096):*int;
	flags = 0x93;
	pt3[v2] = (pa & -(1 << 30)) | flags;
	pt3[v2 + 1] = ((pa + (1 << 30)) & -(1 << 30)) | flags;
	wrcr3(pt4p);
	return (va + (pa & ((1 << 30) - 1))):*byte;
}

struct pcidev {
	addr: *byte;
	type: int;
	bus: int;
	vid: int;
	did: int;
	cls: int;
	subcls: int;
	bar0: int;
	bar1: int;
	bar2: int;
	bar3: int;
	bar4: int;
	bar5: int;
	trap: *byte;
	msi: *byte;
}

scan_pci(base: *byte, visit: func(dev: *pcidev)) {
	var i: int;
	var cap: int;
	var capid: int;
	var dev: pcidev;
	i = 0;
	loop {
		if i == 256*64 {
			break;
		}

		dev.addr = &base[i * 4096];
		dev.bus = i;

		dev.type = base[i * 4096 + 14]:int;

		dev.vid = (base[i * 4096 + 0]:int)
			+ (base[i * 4096 + 1]:int << 8);
		dev.did = (base[i * 4096 + 2]:int)
			+ (base[i * 4096 + 3]:int << 8);
		dev.cls = base[i * 4096 + 11]: int;
		dev.subcls = base[i * 4096 + 10]: int;
		dev.bar0 = _r32(&base[i * 4096 + 16 + 4 * 0]);
		dev.bar1 = _r32(&base[i * 4096 + 16 + 4 * 1]);
		dev.bar2 = _r32(&base[i * 4096 + 16 + 4 * 2]);
		dev.bar3 = _r32(&base[i * 4096 + 16 + 4 * 3]);
		dev.bar4 = _r32(&base[i * 4096 + 16 + 4 * 4]);
		dev.bar5 = _r32(&base[i * 4096 + 16 + 4 * 5]);

		dev.trap = &base[i * 4096 + 60];

		if dev.vid == 0xffff {
			i = i + 1;
			continue;
		}

		dev.msi = 0:*byte;

		if dev.type == 0 {
			cap = base[i * 4096 + 0x34]: int;
			loop {
				if cap == 0 || cap > 4094 {
					break;
				}

				capid = base[i * 4096 + cap]:int;

				if capid == 5 {
					dev.msi = &base[i * 4096 + cap];
					break;
				}

				cap = base[i * 4096 + cap + 1]:int;
			}
		}

		visit(&dev);

		i = i + 1;
	}
}

show_pcidev(dev: *pcidev) {
	kputh16(dev.bus);
	kputc(':');
	kputc(' ');
	kputh16(dev.vid);
	kputc(':');
	kputh16(dev.did);
	kputc(' ');
	kputh8(dev.cls);
	kputc(':');
	kputh8(dev.subcls);
	kputc(' ');
	kputh32(dev.bar0);
	kputc(' ');
	kputh32(dev.bar1);
	kputc(' ');
	kputh32(dev.bar2);
	kputc(' ');
	kputh32(dev.bar3);
	kputc(' ');
	kputh32(dev.bar4);
	kputc(' ');
	kputh32(dev.bar5);
	kputc('\n');
}

onesum(h: *byte, n: int, s: int): int {
	var i: int;
	i = 0;
	s = (s & 0xffff) + (s >> 16);
	s = (s & 0xffff) + (s >> 16);
	s = (s & 0xffff) + (s >> 16);
	loop {
		if i >= n {
			break;
		}
		if i + 1 == n {
			s = s + (h[i]:int << 8);
			s = (s & 0xffff) + ((s >> 16) & 1);
			break;
		}
		s = s + (h[i]:int << 8) + (h[i + 1]:int);
		s = (s & 0xffff) + ((s >> 16) & 1);
		i = i + 2;
	}
	return s;
}

struct vga {
	base: *byte;
	cx: int;
	cy: int;
	x: int;
	y: int;
}

vinit(v: *vga, base: *byte) {
	v.base = base;
	v.cx = 80;
	v.cy = 25;
	v.x = 0;
	v.y = 0;
}

vcursor(v: *vga) {
	var i: int;
	i = v.y * v.cx + v.x;
	outb(0x3d4, 0x0f);
	outb(0x3d5, i);
	outb(0x3d4, 0x0e);
	outb(0x3d5, i >> 8);
}

vclear(v: *vga) {
	var i: int;
	v.x = 0;
	v.y = 0;
	vcursor(v);
	i = 0;
	loop {
		if i == v.cx * v.cy {
			return;
		}
		v.base[2 * i] = 0:byte;
		v.base[2 * i + 1] = 0x0f:byte;
		i = i + 1;
	}
}

vshift(v: *vga) {
	var i: int;
	memcpy(v.base, &v.base[v.cx * 2], (v.cy - 1) * v.cx * 2);
	i = (v.cy - 1) * v.cx;
	loop {
		if i == v.cx * v.cy {
			break;
		}
		v.base[2 * i] = 0:byte;
		v.base[2 * i + 1] = 0x0f:byte;
		i = i + 1;
	}
	if v.y > 0 {
		v.y = v.y - 1;
	}
}

vputc(v: *vga, c: int) {
	if c == '\r' {
		v.x = 0;
	} else if c == '\n' {
		v.x = 0;
		v.y = v.y + 1;
		if v.y == v.cy {
			vshift(v);
		}
	} else {
		v.base[(v.y * v.cx + v.x) * 2] = c:byte;
		v.base[(v.y * v.cx + v.x) * 2 + 1] = 0x0f:byte;
		v.x = v.x + 1;
		if v.x == v.cx {
			v.x = 0;
			v.y = v.y + 1;
			if v.y == v.cy {
				vshift(v);
			}
		}
	}
	vcursor(v);
}

kputc(c: int) {
	var global: *global;
	global = g();
	vputc(&global.vga, c);
}

kputs(s: *byte) {
	var i: int;
	var global: *global;
	global = g();
	i = 0;
	loop {
		if !s[i] {
			break;
		}
		vputc(&global.vga, s[i]:int);
		i = i + 1;
	}
}

struct arp_entry {
	mac: int;
	ip: int;
	last_seen: int;
	state: int;
}

struct tcp_state {
	state: int;

	event_func: func(tcb:*tcp_state);

	// Network tuple
	peer_ip: int;
	peer_port: int;
	sock_ip: int;
	sock_port: int;

	deadline: int;
	rto: int;

	recv_nxt: int;
	recv_len: int;
	recv_buf: *byte;

	send_fin: int;
	send_ack: int;
	send_una: int;
	send_nxt: int;
	send_wnd: int;
	send_len: int;
	send_buf: *byte;

	fin_seq: int;
}

struct global {
	ptr: *global;
	ms: int;
	vga: vga;
	fr: *free_range;
	fp: *free_page;
	lapicp: int;
	lapic: *byte;
	mmio: int;
	realtek_port: *realtek_port;
	ahci_port: *ahci_port;
	boot_time: int;
	ip: int;
	ip_gw: int;
	ip_mask: int;
	arp: *arp_entry;
	arp_count: int;
	tcp: **tcp_state;
	tcp_count: int;
	rng: int;
}

struct free_page {
	next: *free_page;
	pa: int;
}

struct free_range {
	next: *free_range;
	start: int;
	end: int;
}

g(): *global {
	return _rgs(0):**global[0];
}

rand(): int {
	var global: *global;
	var ret: int;
	global = g();
	ret = global.rng;
	global.rng = ret * 5 + 1;
	return ret;
}

memset(dest: *byte, c: int, size: int) {
	var i: int;

	if size < 0 {
		return;
	}

	i = 0;
	loop {
		if i == size {
			break;
		}
		dest[i] = c:byte;
		i = i + 1;
	}
}

memcpy(dest: *byte, src: *byte, size: int) {
	var i: int;

	if size < 0 {
		return;
	}

	if src > dest {
		i = 0;
		loop {
			if i == size {
				break;
			}
			dest[i] = src[i];
			i = i + 1;
		}
	} else if src < dest {
		i = size;
		loop {
			if i == 0 {
				break;
			}
			i = i - 1;
			dest[i] = src[i];
		}
	}
}

memswap(a: *byte, b: *byte, size: int) {
	var i: int;
	var t: byte;

	if size < 0 {
		return;
	}

	i = 0;
	loop {
		if i == size {
			break;
		}
		t = a[i];
		a[i] = b[i];
		b[i] = t;
		i = i + 1;
	}
}

insert_sort(arr: *void, nitems: int, size: int, cmp: (func(a: *void, b: *void): int)) {
	var i: int;
	var j: int;
	var a: *void;
	var b: *void;

	if nitems <= 1 || size <= 0 {
		return;
	}

	i = 1;
	loop {
		if i == nitems {
			break;
		}

		j = i - 1;

		b = (&arr:*byte[size * i]):*void;

		loop {
			a = (&arr:*byte[size * j]):*void;

			if cmp(a, b) <= 0 {
				break;
			}

			memswap(a: *byte, b: *byte, size);

			if j == 0 {
				break;
			}

			j = j - 1;
		}

		i = i + 1;
	}
}

mmap_cmp(a: *void, b: *void): int {
	var a_addr: int;
	var b_addr: int;
	a_addr = a:*int[0];
	b_addr = b:*int[0];
	if a_addr < b_addr {
		return -1;
	} else if a_addr > b_addr {
		return 1;
	}
	return 0;
}

free(p: *byte) {
	var global: *global;
	var fp: *free_page;
	var flags: int;

	global = g();

	if !p {
		return;
	}

	if p:int & 4095 != 0 {
		kdie("BAD FREE");
	}

	fp = p:*free_page;
	fp.pa = vtop(p);

	flags = rdflags();
	cli();
	fp.next = global.fp;
	global.fp = fp;
	wrflags(flags);
}

alloc_page(): int {
	var global: *global;
	var fr: *free_range;
	var fp: *free_page;
	var flags: int;
	var ret: int;
	global = g();

	flags = rdflags();
	cli();

	fp = global.fp;
	if fp {
		global.fp = fp.next;
		wrflags(flags);
		return fp.pa;
	}
	fr = global.fr;
	loop {
		if !fr {
			break;
		}

		if fr.start < fr.end {
			ret = fr.start;
			fr.start = fr.start + 4096;
			wrflags(flags);
			return ret;
		}

		fr = fr.next;
	}

	kdie("OOM");
}

direct_map(brk: *int) {
	var pt4p: int;
	var pt4: *int;
	var pt3p: int;
	var pt3: *int;
	var va: int;
	var pa: int;
	var page_size: int;
	var map_size: int;
	var i: int;
	var n: int;

	map_size = 1 << 46;
	page_size = 1 << 30;

	va = -1 << 47;
	pa = 0;

	pt4p = rdcr3();
	pt4 = ptov(pt4p):*int;

	brk[0] = (brk[0] + 4095) & -4096;

	loop {
		if pa == map_size {
			break;
		}

		n = 512;

		pt3 = brk[0]:*int;
		brk[0] = brk[0] + 4096;
		pt3p = (pt3:int) & ((1 << 31) - 1);

		i = (va >> 39) & 511;
		pt4[i] = pt3p | 0x003;

		loop {
			if pa == map_size || n == 0 {
				break;
			}

			i = (va >> 30) & 511;
			pt3[i] = pa | 0x083;

			va = va + page_size;
			pa = pa + page_size;
			n = n - 1;
		}
	}

	wrcr3(pt4p);
}

setup_ring(ring: int, own: int) {
	var v: *byte;
	var i: int;
	var p: int;

	v = ptov(ring);
	i = 0;

	loop {
		if i == 16 {
			break;
		}

		p = alloc_page();

		_w32(&v[i * 16], (own << 31) | ((i == 15) << 30) | 4096);
		_w32(&v[i * 16 + 4], 0);
		_w32(&v[i * 16 + 8], p);
		_w32(&v[i * 16 + 12], p >> 32);

		i = i + 1;
	}
}

struct realtek_desc {
	flags: int;
	framep: int;
}

struct realtek_port {
	next: *realtek_port;
	io: int;
	mac: int;
	tx: realtek_ring;
	rx: realtek_ring;
}

struct realtek_ring {
	ringp: int;
	ring: *realtek_desc;
	count: int;
	index: int;
}

alloc(): *byte {
	return ptov(alloc_page());
}

realtek_mkring(ring: *realtek_ring, rx: int) {
	var i: int;
	ring.count = 4096 >> 4;
	ring.index = 0;
	ring.ringp = alloc_page();
	ring.ring = ptov(ring.ringp):*realtek_desc;
	i = 0;
	loop {
		if i == ring.count {
			break;
		}
		ring.ring[i].framep = alloc_page();
		ring.ring[i].flags = (rx << 31) + (((i + 1) == ring.count) << 30) + 4095;
		i = i + 1;
	}
}

init_realtek(dev: *pcidev) {
	var global: *global;
	var realtek_port: *realtek_port;
	var io: int;

	global = g();

	if dev.vid != 0x10ec || dev.did != 0x8168 {
		return;
	}

	if dev.bar0 & 1 != 1 {
		return;
	}

	io = dev.bar0 & -2;

	// Enable dma
	_w16(&dev.addr[4], 7);

	// Reset
	outb(io + 0x37, 0x10);
	loop {
		if (inb(io + 0x37) & 0x10) == 0 {
			break;
		}
	}

	// Disable interrupts
	outw(io + 0x3c, 0x0000);

	// Setup MSI
	_w16(&dev.msi[2], 0x0081);
	_w32(&dev.msi[4], global.lapicp);
	_w32(&dev.msi[8], global.lapicp >> 32);
	_w16(&dev.msi[12], 33);

	// Config write enable
	outb(io + 0x50, 0xc0);

	// rx config
	outd(io + 0x44, 0x0000e70f);

	// tx config
	outb(io + 0x37, 0x0c);
	outd(io + 0x40, 0x03000700);

	// rx packet size
	outd(io + 0xda, 0x00000800);

	// tx packet size
	outd(io + 0xec, 0x00000010);

	var mac: int;
	realtek_port = alloc():*realtek_port;
	realtek_port.io = io;
	mac = ind(io) + (ind(io + 4) << 32);
	realtek_port.mac = ((mac >> 40) & 0xff)
		+ (((mac >> 32) & 0xff) << 8)
		+ (((mac >> 24) & 0xff) << 16)
		+ (((mac >> 16) & 0xff) << 24)
		+ (((mac >> 8) & 0xff) << 32)
		+ ((mac & 0xff) << 40);
	realtek_mkring(&realtek_port.tx, 0);
	realtek_mkring(&realtek_port.rx, 1);

	// tx ring
	outd(io + 0x20, realtek_port.tx.ringp);
	outd(io + 0x24, realtek_port.tx.ringp >> 32);

	// tx max size
	outb(io + 0xec, 16);

	// rx ring
	outd(io + 0xe4, realtek_port.rx.ringp);
	outd(io + 0xe8, realtek_port.rx.ringp >> 32);

	// Enable rx/tx
	outd(io + 0x37, 0x0c);

	// Enable interrupts
	outw(io + 0x3c, 0x00ff);
	outw(io + 0x3e, 0xffff);

	// Config write disable
	outb(io + 0x50, 0x00);

	// TX Doorbell
	//fill_packet(txringp);
	//outb(io + 0x38, 0x40);

	realtek_port.next = global.realtek_port;
	global.realtek_port = realtek_port;
}

kputmac(a: int) {
	kputh8((a >> 40) & 0xff);
	kputc(':');
	kputh8((a >> 32) & 0xff);
	kputc(':');
	kputh8((a >> 24) & 0xff);
	kputc(':');
	kputh8((a >> 16) & 0xff);
	kputc(':');
	kputh8((a >> 8) & 0xff);
	kputc(':');
	kputh8(a & 0xff);
}

kputip(a: int) {
	kputd((a >> 24) & 0xff);
	kputc('.');
	kputd((a >> 16) & 0xff);
	kputc('.');
	kputd((a >> 8) & 0xff);
	kputc('.');
	kputd(a & 0xff);
}

enum {
	ET_IP = 0x0800,
	ET_ARP = 0x0806,
	ARP_ETHER = 1,
	IP_ICMP = 1,
	IP_TCP = 6,
	IP_UDP = 17,
	ICMP_PONG = 0,
	ICMP_PING = 8,
}

struct rxinfo {
	port: *realtek_port;

	ether: *byte;
	ether_len: int;
	ether_src: int;
	ether_dest: int;
	ether_type: int;

	arp: *byte;
	arp_len: int;
	arp_op: int;
	arp_ether_src: int;
	arp_ip_src: int;
	arp_ether_dest: int;
	arp_ip_dest: int;

	ip: *byte;
	ip_len: int;
	ip_src: int;
	ip_dest: int;
	ip_proto: int;
	ip_psum: int;

	icmp: *byte;
	icmp_len: int;
	icmp_type: int;
	icmp_code: int;
	icmp_id: int;
	icmp_seq: int;

	icmp_seg: *byte;
	icmp_seg_len: int;

	udp: *byte;
	udp_len: int;
	udp_src: int;
	udp_dest: int;

	udp_seg: *byte;
	udp_seg_len: int;

	tcp: *byte;
	tcp_len: int;
	tcp_src: int;
	tcp_dest: int;
	tcp_seq: int;
	tcp_ack: int;
	tcp_flags: int;
	tcp_win: int;
	tcp_opt: *byte;
	tcp_opt_len: int;

	tcp_seg: *byte;
	tcp_seg_len: int;
}

memo_arp(ether: int, ip: int, auth: int) {
	var global: *global;
	var mask: int;
	global = g();

	// Filter multicast ether sources
	if ether & (1 << 40) {
		return;
	}

	// Filter all zeros mac
	if ether == 0 {
		return;
	}

	// Filter all zeros ip or all ones ip
	if ip == 0 || (ip + 1) >> 32 {
		return;
	}

	// Filter different networks
	mask = -1 << (32 - global.ip_mask);
	if (global.ip & mask) != (ip & mask) {
		return;
	}

	var i: int;
	var j: int;
	i = 0;
	j = 0;

	// Check if ip is already in the table
	loop {
		if i == global.arp_count {
			break;
		}

		// Update the entry for this ip if we've already seen it
		if global.arp[i].state != ARP_EMPTY && global.arp[i].ip == ip {
			global.arp[i].mac = ether;
			global.arp[i].last_seen = global.ms;
			global.arp[i].state = ARP_LIVE;
			return;
		}

		i = i + 1;
	}

	if !auth {
		return;
	}

	// Find an empty slot
	i = 0;
	loop {
		if i == global.arp_count {
			i = j;
			break;
		}

		// Use the first free entry
		if global.arp[i].state == ARP_EMPTY {
			break;
		}

		// Or evict the oldest entry if none are free
		if global.arp[i].last_seen < global.arp[j].last_seen {
			j = i;
		}

		i = i + 1;
	}

	// Add it to the memo table
	global.arp[i].mac = ether;
	global.arp[i].ip = ip;
	global.arp[i].last_seen = global.ms;
	global.arp[i].state = ARP_LIVE;
}

enum {
	// Empty entry in the arp table
	ARP_EMPTY = 0,
	// Valid entry in the arp table
	ARP_LIVE = 1,
	// Time to consider an entry stale
	ARP_THRESH = 15000,
}

struct txinfo {
	buf: *byte;
	len: int;

	port: *realtek_port;

	ether_type: int;
	ether_src: int;
	ether_dest: int;

	ip_proto: int;
	ip_src: int;
	ip_dest: int;

	icmp_type: int;
	icmp_code: int;
	icmp_id: int;
	icmp_seq: int;

	udp_src: int;
	udp_dest: int;

	tcp_src: int;
	tcp_dest: int;
	tcp_seq: int;
	tcp_ack: int;
	tcp_flags: int;
	tcp_win: int;
	tcp_opt: *byte;
	tcp_opt_len: int;
}

alloc_tx(): *txinfo {
	var pkt: *txinfo;
	pkt = alloc():*txinfo;
	bzero(pkt: *byte, 4096);
	pkt.buf = &(pkt:*byte)[1024];
	return pkt;
}

free_tx(pkt: *txinfo) {
	free(pkt:*byte);
}

send_realtek(pkt: *txinfo) {
	var port: *realtek_port;
	var i: int;
	var flags: int;

	flags = rdflags();
	cli();

	port = pkt.port;
	if !port {
		wrflags(flags);
		return;
	}

	if port.tx.index >= port.tx.count {
		port.tx.index = 0;
	}
	i = port.tx.index;
	port.tx.index = i +1;

	if port.tx.ring[i].flags & (1 << 31) != 0 {
		wrflags(flags);
		return;
	}

	memcpy(ptov(port.tx.ring[i].framep), pkt.buf, pkt.len);
	port.tx.ring[i].flags = (port.tx.ring[i].flags & (1 << 30)) + (0xb << 28) + pkt.len;
	outd(port.io + 0x38, 0x40);
	wrflags(flags);
}

send_ether(pkt: *txinfo) {
	pkt.buf = &pkt.buf[-14];
	pkt.len = pkt.len + 14;

	pkt.buf[0] = (pkt.ether_dest >> 40):byte;
	pkt.buf[1] = (pkt.ether_dest >> 32):byte;
	pkt.buf[2] = (pkt.ether_dest >> 24):byte;
	pkt.buf[3] = (pkt.ether_dest >> 16):byte;
	pkt.buf[4] = (pkt.ether_dest >> 8):byte;
	pkt.buf[5] = pkt.ether_dest:byte;

	pkt.buf[6] = (pkt.ether_src >> 40):byte;
	pkt.buf[7] = (pkt.ether_src >> 32):byte;
	pkt.buf[8] = (pkt.ether_src >> 24):byte;
	pkt.buf[9] = (pkt.ether_src >> 16):byte;
	pkt.buf[10] = (pkt.ether_src >> 8):byte;
	pkt.buf[11] = pkt.ether_src:byte;

	pkt.buf[12] = (pkt.ether_type >> 8): byte;
	pkt.buf[13] = pkt.ether_type: byte;

	send_realtek(pkt);
}

send_arp2(port: *realtek_port, tha: int, tpa: int) {
	var pkt: *txinfo;
	var global: *global;
	var sha: int;
	var spa: int;
	global = g();

	if tha & (1 << 40) {
		return;
	}

	if tha == 0 {
		return;
	}

	sha = port.mac;
	spa = global.ip;

	pkt = alloc_tx();
	pkt.port = port;

	// htype
	pkt.buf[0] = 0:byte;
	pkt.buf[1] = 1:byte;

	// ptype
	pkt.buf[2] = (ET_IP >> 8):byte;
	pkt.buf[3] = (ET_IP):byte;

	// hlen
	pkt.buf[4] = 6:byte;

	// plen
	pkt.buf[5] = 4:byte;

	// oper
	pkt.buf[6] = 0: byte;
	pkt.buf[7] = 2: byte;

	// sha
	pkt.buf[8] = (sha >> 40): byte;
	pkt.buf[9] = (sha >> 32): byte;
	pkt.buf[10] = (sha >> 24): byte;
	pkt.buf[11] = (sha >> 16): byte;
	pkt.buf[12] = (sha >> 8): byte;
	pkt.buf[13] = sha: byte;

	// spa
	pkt.buf[14] = (spa >> 24): byte;
	pkt.buf[15] = (spa >> 16): byte;
	pkt.buf[16] = (spa >> 8): byte;
	pkt.buf[17] = spa: byte;

	// tha
	pkt.buf[18] = (tha >> 40): byte;
	pkt.buf[19] = (tha >> 32): byte;
	pkt.buf[20] = (tha >> 24): byte;
	pkt.buf[21] = (tha >> 16): byte;
	pkt.buf[22] = (tha >> 8): byte;
	pkt.buf[23] = tha: byte;

	// tpa
	pkt.buf[24] = (tpa >> 24): byte;
	pkt.buf[25] = (tpa >> 16): byte;
	pkt.buf[26] = (tpa >> 8): byte;
	pkt.buf[27] = tpa: byte;

	pkt.len = 28;

	pkt.ether_type = ET_ARP;
	pkt.ether_src = sha;
	pkt.ether_dest = tha;

	send_ether(pkt);

	free_tx(pkt);
}

handle_arp(pkt: *rxinfo) {
	var global: *global;
	global = g();

	// Add arp_ether_src arp_ip_src to the arp table
	memo_arp(pkt.arp_ether_src, pkt.arp_ip_src, 1);

	if pkt.arp_ip_dest != global.ip {
		return;
	}

	if pkt.arp_op == 1 {
		// Send ARP response
		send_arp2(pkt.port, pkt.arp_ether_src, pkt.arp_ip_src);
	}
}

rx_arp(pkt: *rxinfo) {
	var x: int;

	if pkt.arp_len < 28 {
		return;
	}

	// Hardware type
	x = (pkt.arp[0]:int << 8) + pkt.arp[1]:int;
	if x != ARP_ETHER {
		return;
	}

	// Protocol type
	x = (pkt.arp[2]:int << 8) + pkt.arp[3]:int;
	if x != ET_IP {
		return;
	}

	// Hardware and protocol length
	x = (pkt.arp[4]:int << 8) + pkt.arp[5]:int;
	if x != 0x0604 {
		return;
	}

	// Operation
	pkt.arp_op = (pkt.arp[6]:int << 8) + pkt.arp[7]:int;

	if pkt.arp_op != 1 && pkt.arp_op != 2 {
		return;
	}

	pkt.arp_ether_src = (pkt.arp[8]:int << 40)
		+ (pkt.arp[9]:int << 32)
		+ (pkt.arp[10]:int << 24)
		+ (pkt.arp[11]:int << 16)
		+ (pkt.arp[12]:int << 8)
		+ pkt.arp[13]:int;

	pkt.arp_ip_src = (pkt.arp[14]:int << 24)
		+ (pkt.arp[15]:int << 16)
		+ (pkt.arp[16]:int << 8)
		+ pkt.arp[17]:int;

	pkt.arp_ether_dest = (pkt.arp[18]:int << 40)
		+ (pkt.arp[19]:int << 32)
		+ (pkt.arp[20]:int << 24)
		+ (pkt.arp[21]:int << 16)
		+ (pkt.arp[22]:int << 8)
		+ pkt.arp[23]:int;

	pkt.arp_ip_dest = (pkt.arp[24]:int << 24)
		+ (pkt.arp[25]:int << 16)
		+ (pkt.arp[26]:int << 8)
		+ pkt.arp[27]:int;

	handle_arp(pkt);
}

send_arp1(port: *realtek_port, tha: int, tpa: int) {
	var pkt: *txinfo;
	var global: *global;
	var sha: int;
	var spa: int;
	global = g();

	sha = port.mac;
	spa = global.ip;

	pkt = alloc_tx();
	pkt.port = port;

	// htype
	pkt.buf[0] = 0:byte;
	pkt.buf[1] = 1:byte;

	// ptype
	pkt.buf[2] = (ET_IP >> 8):byte;
	pkt.buf[3] = (ET_IP):byte;

	// hlen
	pkt.buf[4] = 6:byte;

	// plen
	pkt.buf[5] = 4:byte;

	// oper
	pkt.buf[6] = 0: byte;
	pkt.buf[7] = 1: byte;

	// sha
	pkt.buf[8] = (sha >> 40): byte;
	pkt.buf[9] = (sha >> 32): byte;
	pkt.buf[10] = (sha >> 24): byte;
	pkt.buf[11] = (sha >> 16): byte;
	pkt.buf[12] = (sha >> 8): byte;
	pkt.buf[13] = sha: byte;

	// spa
	pkt.buf[14] = (spa >> 24): byte;
	pkt.buf[15] = (spa >> 16): byte;
	pkt.buf[16] = (spa >> 8): byte;
	pkt.buf[17] = spa: byte;

	// tha
	pkt.buf[18] = (tha >> 40): byte;
	pkt.buf[19] = (tha >> 32): byte;
	pkt.buf[20] = (tha >> 24): byte;
	pkt.buf[21] = (tha >> 16): byte;
	pkt.buf[22] = (tha >> 8): byte;
	pkt.buf[23] = tha: byte;

	// tpa
	pkt.buf[24] = (tpa >> 24): byte;
	pkt.buf[25] = (tpa >> 16): byte;
	pkt.buf[26] = (tpa >> 8): byte;
	pkt.buf[27] = tpa: byte;

	pkt.len = 28;

	pkt.ether_type = ET_ARP;
	pkt.ether_src = sha;
	pkt.ether_dest = tha;

	send_ether(pkt);

	free_tx(pkt);
}

find_arp(pkt: *txinfo): int {
	var flags: int;
	var global: *global;
	var mask: int;
	var now: int;
	var ret: int;
	var i: int;
	var ip: int;
	global = g();

	if !pkt.port {
		return 0;
	}

	ip = pkt.ip_dest;

	// Forward to gateway if on a different network
	mask = -1 << (32 - global.ip_mask);
	if (ip & mask) != (global.ip & mask) {
		ip = global.ip_gw;
	}

	flags = rdflags();
	cli();

	i = 0;
	now = global.ms;
	loop {
		if i == global.arp_count {
			wrflags(flags);
			send_arp1(pkt.port, (1 << 48) - 1, ip);
			return 0;
		}

		// Found the entry that matches
		if global.arp[i].state != 0 && global.arp[i].ip == ip {
			ret = global.arp[i].mac;
			wrflags(flags);
			return ret;
		}

		i = i + 1;
	}
}

send_ip(pkt: *txinfo) {
	var id: int;
	var sum: int;
	var dest: int;
	var global: *global;
	var port: *realtek_port;
	global = g();

	port = global.realtek_port;
	if !port {
		return;
	}

	pkt.port = port;

	if pkt.ip_src != global.ip {
		return;
	}

	dest = find_arp(pkt);
	if dest == 0 {
		return;
	}

	pkt.ether_type = ET_IP;
	pkt.ether_src = port.mac;
	pkt.ether_dest = dest;

	pkt.buf = &pkt.buf[-20];
	pkt.len = pkt.len + 20;

	// version + ihl
	pkt.buf[0] = 0x45:byte;

	// dscp
	pkt.buf[1] = 0:byte;

	// length
	pkt.buf[2] = (pkt.len >> 8):byte;
	pkt.buf[3] = pkt.len:byte;

	// identifier
	id = rand();
	pkt.buf[4] = (id >> 8):byte;
	pkt.buf[5] = id:byte;

	// fragment offset
	pkt.buf[6] = 0:byte;
	pkt.buf[7] = 0:byte;

	// ttl
	pkt.buf[8] = 64:byte;
	// proto
	pkt.buf[9] = pkt.ip_proto:byte;

	// checksum
	pkt.buf[10] = 0:byte;
	pkt.buf[11] = 0:byte;

	// src
	pkt.buf[12] = (pkt.ip_src >> 24):byte;
	pkt.buf[13] = (pkt.ip_src >> 16):byte;
	pkt.buf[14] = (pkt.ip_src >> 8):byte;
	pkt.buf[15] = pkt.ip_src:byte;

	// dest
	pkt.buf[16] = (pkt.ip_dest >> 24):byte;
	pkt.buf[17] = (pkt.ip_dest >> 16):byte;
	pkt.buf[18] = (pkt.ip_dest >> 8):byte;
	pkt.buf[19] = pkt.ip_dest:byte;

	sum = ~onesum(pkt.buf, 20, 0);

	pkt.buf[10] = (sum >> 8):byte;
	pkt.buf[11] = sum:byte;

	send_ether(pkt);
}

send_icmp(pkt: *txinfo) {
	var sum: int;

	pkt.buf = &pkt.buf[-8];
	pkt.len = pkt.len + 8;

	pkt.buf[0] = pkt.icmp_type:byte;
	pkt.buf[1] = pkt.icmp_code:byte;
	pkt.buf[2] = 0:byte;
	pkt.buf[3] = 0:byte;
	pkt.buf[4] = (pkt.icmp_id >> 8):byte;
	pkt.buf[5] = pkt.icmp_id:byte;
	pkt.buf[6] = (pkt.icmp_seq >> 8):byte;
	pkt.buf[7] = pkt.icmp_seq:byte;

	sum = ~onesum(pkt.buf, pkt.len, 0);
	pkt.buf[2] = (sum >> 8):byte;
	pkt.buf[3] = sum:byte;

	pkt.ip_proto = IP_ICMP;

	send_ip(pkt);
}

send_pong(pkt: *rxinfo) {
	var global: *global;
	var tx: *txinfo;
	global = g();
	tx = alloc_tx();
	memcpy(tx.buf, pkt.icmp_seg, pkt.icmp_seg_len);
	tx.len = pkt.icmp_seg_len;
	tx.ip_src = global.ip;
	tx.ip_dest = pkt.ip_src;
	tx.icmp_type = 0;
	tx.icmp_code = 0;
	tx.icmp_id = pkt.icmp_id;
	tx.icmp_seq = pkt.icmp_seq;
	send_icmp(tx);
	free_tx(tx);
}

handle_icmp(pkt: *rxinfo) {
	if pkt.icmp_type == ICMP_PING {
		if pkt.icmp_code == 0 {
			send_pong(pkt);
		}
	}
}

rx_icmp(pkt: *rxinfo) {
	if pkt.icmp_len < 8 {
		return;
	}

	if onesum(pkt.icmp, pkt.icmp_len, 0) != 0xffff {
		return;
	}

	pkt.icmp_type = pkt.icmp[0]:int;
	pkt.icmp_code = pkt.icmp[1]:int;

	pkt.icmp_id = (pkt.icmp[4]:int << 8) + pkt.icmp[5]:int;
	pkt.icmp_seq = (pkt.icmp[6]:int << 8) + pkt.icmp[7]:int;

	pkt.icmp_seg = &pkt.icmp[8];
	pkt.icmp_seg_len = pkt.icmp_len - 8;

	handle_icmp(pkt);
}

send_udp(pkt: *txinfo) {
	var sum: int;

	pkt.ip_proto = IP_UDP;

	pkt.buf = &pkt.buf[-8];
	pkt.len = pkt.len + 8;

	pkt.buf[0] = (pkt.udp_src >> 8): byte;
	pkt.buf[1] = pkt.udp_src: byte;
	pkt.buf[2] = (pkt.udp_dest >> 8): byte;
	pkt.buf[3] = pkt.udp_dest: byte;
	pkt.buf[4] = (pkt.len >> 8): byte;
	pkt.buf[5] = pkt.len: byte;
	pkt.buf[6] = 0: byte;
	pkt.buf[7] = 0: byte;

	sum = (pkt.ip_src & 0xffff)
		+ ((pkt.ip_src >> 16) & 0xffff)
		+ (pkt.ip_dest & 0xffff)
		+ ((pkt.ip_dest >> 16) & 0xffff)
		+ IP_UDP
		+ pkt.len;

	sum = ~onesum(pkt.buf, pkt.len, sum);
	pkt.buf[6] = (sum >> 8):byte;
	pkt.buf[7] = sum:byte;

	send_ip(pkt);
}

udp_echo(pkt: *rxinfo) {
	var tx: *txinfo;
	tx = alloc_tx();
	tx.ip_src = pkt.ip_dest;
	tx.ip_dest = pkt.ip_src;
	tx.udp_src = pkt.udp_dest;
	tx.udp_dest = pkt.udp_src;
	memcpy(tx.buf, pkt.udp_seg, pkt.udp_seg_len);
	tx.len = pkt.udp_seg_len;
	send_udp(tx);
	free_tx(tx);
}

handle_udp(pkt: *rxinfo) {
	if pkt.udp_dest == 8000 {
		udp_echo(pkt);
	}
}

rx_udp(pkt: *rxinfo) {
	var sum: int;
	var len: int;

	if pkt.udp_len < 8 {
		return;
	}

	len = (pkt.udp[4]:int << 8)
		+ pkt.udp[5]:int;
	if len < 8 || len > pkt.udp_len {
		return;
	}

	pkt.udp_len = len;

	if _r16(&pkt.udp[6]) != 0 {
		sum = onesum(pkt.udp, pkt.udp_len, pkt.ip_psum);
		if sum != 0xffff {
			return;
		}
	}

	pkt.udp_src = (pkt.udp[0]:int << 8)
		+ pkt.udp[1]:int;

	pkt.udp_dest = (pkt.udp[2]:int << 8)
		+ pkt.udp[3]:int;

	pkt.udp_seg = &pkt.udp[8];
	pkt.udp_seg_len = pkt.udp_len - 8;

	handle_udp(pkt);
}

send_tcp(pkt: *txinfo) {
	var len: int;
	var sum: int;

	len = 20 + ((pkt.tcp_opt_len + 3) & -4);

	pkt.buf = &pkt.buf[-len];
	pkt.len = pkt.len + len;

	bzero(pkt.buf, len);

	pkt.buf[0] = (pkt.tcp_src >> 8):byte;
	pkt.buf[1] = pkt.tcp_src:byte;
	pkt.buf[2] = (pkt.tcp_dest >> 8):byte;
	pkt.buf[3] = pkt.tcp_dest:byte;

	pkt.buf[4] = (pkt.tcp_seq >> 24):byte;
	pkt.buf[5] = (pkt.tcp_seq >> 16):byte;
	pkt.buf[6] = (pkt.tcp_seq >> 8):byte;
	pkt.buf[7] = pkt.tcp_seq:byte;

	pkt.buf[8] = (pkt.tcp_ack >> 24):byte;
	pkt.buf[9] = (pkt.tcp_ack >> 16):byte;
	pkt.buf[10] = (pkt.tcp_ack >> 8):byte;
	pkt.buf[11] = pkt.tcp_ack:byte;

	pkt.buf[12] = (len << 2):byte;
	pkt.buf[13] = pkt.tcp_flags:byte;
	pkt.buf[14] = (pkt.tcp_win >> 8):byte;
	pkt.buf[15] = pkt.tcp_win:byte;

	memcpy(&pkt.buf[20], pkt.tcp_opt, pkt.tcp_opt_len);

	sum = (pkt.ip_src & 0xffff)
		+ ((pkt.ip_src >> 16) & 0xffff)
		+ (pkt.ip_dest & 0xffff)
		+ ((pkt.ip_dest >> 16) & 0xffff)
		+ IP_TCP
		+ pkt.len;

	sum = ~onesum(pkt.buf, pkt.len, sum);
	pkt.buf[16] = (sum >> 8):byte;
	pkt.buf[17] = sum:byte;

	pkt.ip_proto = IP_TCP;

	send_ip(pkt);
}

alloc_tcp(): *tcp_state {
	var global: *global;
	var flags: int;
	var i: int;
	var tcb: *tcp_state;

	global = g();

	flags = rdflags();
	cli();

	i = 0;
	loop {
		if i == global.tcp_count {
			wrflags(flags);
			return 0:*tcp_state;
		}

		if !global.tcp[i] {
			tcb = alloc():*tcp_state;
			bzero(tcb:*byte, sizeof(*tcb));
			tcb.recv_buf = alloc();
			tcb.send_buf = alloc();
			global.tcp[i] = tcb;
			wrflags(flags);
			return tcb;
		}

		i = i + 1;
	}
}

tcp_free(tcb: *tcp_state) {
	var global: *global;
	var flags: int;
	var i: int;
	global = g();
	flags = rdflags();
	cli();
	i = 0;
	loop {
		if i == global.tcp_count {
			break;
		}
		if global.tcp[i] == tcb {
			global.tcp[i] = 0:*tcp_state;
			break;
		}
		i = i + 1;
	}
	free(tcb.send_buf);
	free(tcb.recv_buf);
	free(tcb:*byte);
	wrflags(flags);
}

tcp_listen(port: int, event_func: func(tcb: *tcp_state)) {
	var global: *global;
	var tcb: *tcp_state;
	global = g();

	tcb = alloc_tcp();

	tcb.event_func = event_func;
	tcb.sock_ip = global.ip;
	tcb.sock_port = port;

	tcb.state = TCP_LISTEN;
}

tcp_echo(tcb: *tcp_state) {
	var buf: *byte;
	var len: int;

	if tcb.state == TCP_CLOSED {
		tcp_free(tcb);
		return;
	}

	if tcb.state == TCP_CLOSE_WAIT {
		tcp_close(tcb);
		return;
	}

	buf = alloc();

	len = 4096 - tcb.send_len;
	len = tcp_recv(tcb, buf, len);
	tcp_send(tcb, buf, len);

	free(buf);
}

send_rst(pkt: *rxinfo) {
	var tx: *txinfo;
	tx = alloc_tx();

	tx.ip_src = pkt.ip_dest;
	tx.ip_dest = pkt.ip_src;
	tx.tcp_src = pkt.tcp_dest;
	tx.tcp_dest = pkt.tcp_src;
	tx.tcp_win = 0;
	tx.tcp_opt = 0:*byte;
	tx.tcp_opt_len = 0;
	tx.len = 0;

	if pkt.tcp_flags & TCP_ACK {
		tx.tcp_seq = pkt.tcp_ack;
		tx.tcp_ack = 0;
		tx.tcp_flags = TCP_RST;
	} else {
		tx.tcp_seq = 0;
		tx.tcp_ack = pkt.tcp_seq + pkt.tcp_seg_len;
		tx.tcp_flags = TCP_RST | TCP_ACK;
		if pkt.tcp_flags & TCP_SYN {
			tx.tcp_ack = tx.tcp_ack + 1;
		}
	}

	send_tcp(tx);

	free_tx(tx);
}

send_ack(tcb: *tcp_state) {
	var tx: *txinfo;
	tx = alloc_tx();

	tx.ip_src = tcb.sock_ip;
	tx.ip_dest = tcb.peer_ip;
	tx.tcp_src = tcb.sock_port;
	tx.tcp_dest = tcb.peer_port;
	tx.tcp_win = 4096 - tcb.recv_len;
	tx.tcp_opt = 0:*byte;
	tx.tcp_opt_len = 0;
	tx.len = 0;

	tx.tcp_seq = tcb.send_nxt;
	tx.tcp_ack = tcb.recv_nxt;
	tx.tcp_flags = TCP_ACK;

	if tcb.send_len == 0 && tcb.send_fin {
		tcb.send_nxt = tcb.fin_seq;
	}

	send_tcp(tx);

	free_tx(tx);
}

send_psh(tcb: *tcp_state) {
	var tx: *txinfo;
	var len: int;
	tx = alloc_tx();

	tx.ip_src = tcb.sock_ip;
	tx.ip_dest = tcb.peer_ip;
	tx.tcp_src = tcb.sock_port;
	tx.tcp_dest = tcb.peer_port;
	tx.tcp_win = 4096 - tcb.recv_len;
	tx.tcp_opt = 0:*byte;
	tx.tcp_opt_len = 0;

	len = tcb.send_len;
	if len > tcb.send_wnd {
		len = tcb.send_wnd;
	}
	if len > 1460 {
		len = 1460;
	}

	memcpy(tx.buf, tcb.send_buf, len);
	tx.len = len;

	tx.tcp_seq = tcb.send_una;
	tx.tcp_ack = tcb.recv_nxt;
	tx.tcp_flags = TCP_ACK;

	if tcb.state == TCP_SYN_RECV {
		tx.tcp_flags = tx.tcp_flags | TCP_SYN;
	}

	if len > 0 {
		tx.tcp_flags = tx.tcp_flags | TCP_PSH;
	}

	if len == tcb.send_len && tcb.send_fin {
		tx.tcp_flags = tx.tcp_flags | TCP_FIN;

		if tcb.state == TCP_ESTAB {
			tcb.state = TCP_FIN_WAIT_1;
		} else if tcb.state == TCP_CLOSE_WAIT {
			tcb.state = TCP_LAST_ACK;
		}

		tcb.send_nxt = tcb.fin_seq;
	} else {
		tcb.send_nxt = (tcb.send_una + len) & ((1 << 32) - 1);
	}

	send_tcp(tx);

	free_tx(tx);
}

enum {
	TCP_CLOSED = 0,
	TCP_LISTEN = 1,
	TCP_SYN_SENT = 2,
	TCP_SYN_RECV = 3,
	TCP_ESTAB = 4,
	TCP_FIN_WAIT_1 = 5,
	TCP_FIN_WAIT_2 = 6,
	TCP_CLOSE_WAIT = 7,
	TCP_LAST_ACK = 8,
	TCP_CLOSING = 9,
	TCP_TIME_WAIT = 10,
}

find_tcb(pkt: *rxinfo): *tcp_state {
	var global: *global;
	var tcb: *tcp_state;
	var i: int;
	global = g();

	// Find a connection
	i = 0;
	loop {
		if i == global.tcp_count {
			break;
		}

		tcb = global.tcp[i];

		if tcb
			&& tcb.state >= TCP_SYN_SENT
			&& tcb.peer_ip == pkt.ip_src
			&& tcb.peer_port == pkt.tcp_src
			&& tcb.sock_ip == pkt.ip_dest
			&& tcb.sock_port == pkt.tcp_dest {
			return tcb;
		}

		i = i + 1;
	}

	// Find a listener
	i = 0;
	loop {
		if i == global.tcp_count {
			break;
		}

		tcb = global.tcp[i];

		if tcb
			&& tcb.state == TCP_LISTEN
			&& tcb.sock_ip == pkt.ip_dest
			&& tcb.sock_port == pkt.tcp_dest {
			return tcb;
		}

		i = i + 1;
	}

	return 0:*tcp_state;
}

handle_syn(tcb: *tcp_state, pkt: *rxinfo) {
	var c: *tcp_state;
	var tx: *txinfo;

	c = alloc_tcp();
	if !c {
		send_rst(pkt);
		return;
	}

	c.event_func = tcb.event_func;

	c.peer_ip = pkt.ip_src;
	c.peer_port = pkt.tcp_src;
	c.sock_ip = pkt.ip_dest;
	c.sock_port = pkt.tcp_dest;

	c.rto = 10;

	c.send_nxt = rand();
	c.send_ack = c.send_nxt;
	c.send_una = c.send_nxt;
	c.send_wnd = pkt.tcp_win;

	c.recv_nxt = (pkt.tcp_seq + 1) & ((1 << 32) - 1);

	c.state = TCP_SYN_RECV;

	tx = alloc_tx();

	tx.ip_src = c.sock_ip;
	tx.ip_dest = c.peer_ip;
	tx.tcp_src = c.sock_port;
	tx.tcp_dest = c.peer_port;
	tx.tcp_win = 4096 - c.recv_len;
	tx.tcp_opt = 0:*byte;
	tx.tcp_opt_len = 0;
	tx.len = 0;

	tx.tcp_flags = TCP_SYN | TCP_ACK;
	tx.tcp_seq = c.send_nxt;
	tx.tcp_ack = c.recv_nxt;

	c.send_nxt = (c.send_nxt + 1) & ((1 << 32) - 1);

	send_tcp(tx);

	free_tx(tx);
}

handle_seg(tcb: *tcp_state, pkt: *rxinfo) {
	var offset: int;
	var wnd: int;
	var len: int;

	// 1. Check sequence
	if pkt.tcp_seq != tcb.recv_nxt {
		if pkt.tcp_flags & TCP_RST {
			return;
		}
		send_ack(tcb);
		return;
	}

	// 2. Check RST
	if pkt.tcp_flags & TCP_RST {
		tcb.state = TCP_CLOSED;
		return;
	}

	// 4. Check SYN
	if pkt.tcp_flags & TCP_SYN {
		send_ack(tcb);
		return;
	}

	// 5. Check ACK
	if pkt.tcp_flags & TCP_ACK == 0 {
		return;
	}

	// Validate ACK
	offset = (pkt.tcp_ack - tcb.send_una) & ((1 << 31) - 1);
	wnd = (tcb.send_nxt - tcb.send_una) & ((1 << 31) - 1);
	if offset > wnd {
		send_ack(tcb);
		return;
	}

	if tcb.state == TCP_SYN_RECV {
		tcb.state = TCP_ESTAB;
		tcb.send_una = (tcb.send_una + 1) & ((1 << 32) - 1);
	}

	if tcb.state == TCP_FIN_WAIT_1 && tcb.send_una == tcb.send_nxt {
		tcb.state = TCP_FIN_WAIT_2;
	}

	if tcb.state == TCP_CLOSING && tcb.send_una == tcb.send_nxt {
		tcb.state = TCP_TIME_WAIT;
	}

	if tcb.state == TCP_LAST_ACK {
		tcb.state = TCP_CLOSED;
		return;
	}

	len = (pkt.tcp_ack - tcb.send_una) & ((1 << 31) - 1);
	if len > tcb.send_len {
		len = tcb.send_len;
	}
	tcb.send_len = tcb.send_len - len;
	memcpy(tcb.send_buf, &tcb.send_buf[len], tcb.send_len);

	tcb.send_una = pkt.tcp_ack;

	// 7. Process data
	if pkt.tcp_seg_len > 0 {
		len = 4096 - tcb.recv_len;
		if len > pkt.tcp_seg_len {
			len = pkt.tcp_seg_len;
		}

		memcpy(&tcb.recv_buf[tcb.recv_len], pkt.tcp_seg, len);
		tcb.recv_nxt = (tcb.recv_nxt + len) & ((1 << 32) - 1);
		tcb.recv_len = tcb.recv_len + len;
		send_ack(tcb);
	}

	// 8. Check FIN
	if pkt.tcp_flags & TCP_FIN {
		tcb.recv_nxt = (tcb.recv_nxt + len + 1) & ((1 << 32) - 1);

		if tcb.state == TCP_SYN_RECV || tcb.state == TCP_ESTAB {
			tcb.state = TCP_CLOSE_WAIT;
		}

		if tcb.state == TCP_FIN_WAIT_1 {
			if tcb.send_una == tcb.send_nxt {
				tcb.state = TCP_TIME_WAIT;
			} else {
				tcb.state = TCP_CLOSING;
			}
		}

		if tcb.state == TCP_FIN_WAIT_2 {
			tcb.state = TCP_TIME_WAIT;
		}
	}

	if tcb.state == TCP_TIME_WAIT {
		tcb.deadline = 300000;
	}
}

tcp_tick(tcb: *tcp_state) {
	var global: *global;
	var len: int;
	global = g();

	if tcb.state == TCP_TIME_WAIT {
		kputc('.');
		return;
	}

	if tcb.state <= TCP_LISTEN || (!tcb.send_fin && !tcb.send_len) {
		return;
	}

	// We got an ack send a new packet
	if tcb.send_ack != tcb.send_una {
		tcb.send_ack = tcb.send_una;
		if tcb.send_len || tcb.send_fin {
			send_psh(tcb);
		}
		tcb.rto = 10;
		tcb.deadline = tcb.rto;
		return;
	}

	// Consider retransmission
	if tcb.deadline > 0 {
		tcb.deadline = tcb.deadline - 1;
		return;
	}

	tcb.rto = tcb.rto * 2;
	if tcb.rto > 10000 {
		tcb.rto = 10000;
	}

	send_psh(tcb);
	tcb.deadline = tcb.rto;
}

tcp_send(tcb: *tcp_state, b: *byte, n: int): int {
	var cap: int;
	var flags: int;
	flags = rdflags();
	cli();

	if tcb.state != TCP_ESTAB || n < 0 {
		wrflags(flags);
		return 0;
	}

	cap = 4096 - tcb.send_len;
	if n > cap {
		n = cap;
	}

	memcpy(&tcb.send_buf[tcb.send_len], b, n);
	tcb.send_len = tcb.send_len + n;

	wrflags(flags);
	return n;
}

tcp_recv(tcb: *tcp_state, b: *byte, n: int): int {
	var cap: int;
	var flags: int;
	flags = rdflags();
	cli();

	if (tcb.state != TCP_ESTAB && tcb.recv_len == 0) || n < 0 {
		wrflags(flags);
		return 0;
	}

	if n > tcb.recv_len {
		n = tcb.recv_len;
	}

	memcpy(b, tcb.recv_buf, n);
	tcb.recv_len = tcb.recv_len - n;
	memcpy(tcb.recv_buf, &tcb.recv_buf[n], tcb.recv_len);

	wrflags(flags);
	return n;
}

tcp_close(tcb: *tcp_state) {
	var flags: int;
	flags = rdflags();
	cli();

	tcb.send_fin = 1;
	tcb.fin_seq = (tcb.send_una + tcb.send_len + 1) & ((1 << 32) - 1);

	wrflags(flags);
}

handle_tcp(pkt: *rxinfo) {
	var tcb: *tcp_state;

	if pkt.tcp_src == 0 || pkt.tcp_dest == 0 {
		return;
	}

	// Find state for the incoming packet
	tcb = find_tcb(pkt);
	if !tcb || tcb.state == TCP_CLOSED {
		send_rst(pkt);
	} else if tcb.state == TCP_LISTEN {
		// Handle incoming connection
		if pkt.tcp_flags & TCP_RST {
			return;
		}

		if pkt.tcp_flags & TCP_ACK {
			send_rst(pkt);
			return;
		}

		if pkt.tcp_flags & (TCP_FIN|TCP_SYN|TCP_PSH) != TCP_SYN {
			return;
		}

		handle_syn(tcb, pkt);
	} else if tcb.state == TCP_SYN_SENT {
		// out going connections not implemented yet
		send_rst(pkt);
	} else {
		handle_seg(tcb, pkt);
		tcb.event_func(tcb);
	}
}

enum {
	TCP_ACK = 16,
	TCP_PSH = 8,
	TCP_RST = 4,
	TCP_SYN = 2,
	TCP_FIN = 1,
}

rx_tcp(pkt: *rxinfo) {
	var sum: int;
	var off: int;

	if pkt.tcp_len < 20 {
		return;
	}

	sum = onesum(pkt.tcp, pkt.tcp_len, pkt.ip_psum);
	if sum != 0xffff {
		return;
	}

	pkt.tcp_src = (pkt.tcp[0]:int << 8)
		+ pkt.tcp[1]:int;
	pkt.tcp_dest = (pkt.tcp[2]:int << 8)
		+ pkt.tcp[3]:int;
	pkt.tcp_seq = (pkt.tcp[4]:int << 24)
		+ (pkt.tcp[5]:int << 16)
		+ (pkt.tcp[6]:int << 8)
		+ pkt.tcp[7]:int;
	pkt.tcp_ack = (pkt.tcp[8]:int << 24)
		+ (pkt.tcp[9]:int << 16)
		+ (pkt.tcp[10]:int << 8)
		+ pkt.tcp[11]:int;
	pkt.tcp_flags = pkt.tcp[13]:int;
	pkt.tcp_win = (pkt.tcp[14]:int << 8)
		+ pkt.tcp[15]:int;

	off = (pkt.tcp[12]:int >> 4) << 2;
	if off < 20 || off > pkt.tcp_len {
		return;
	}

	pkt.tcp_opt = &pkt.tcp[20];
	pkt.tcp_opt_len = off - 20;

	pkt.tcp_seg = &pkt.tcp[off];
	pkt.tcp_seg_len = pkt.tcp_len - off;

	handle_tcp(pkt);
}

rx_ip(pkt: *rxinfo) {
	var global: *global;
	var len: int;
	var frag: int;

	global = g();

	if pkt.ip_len < 20 {
		return;
	}

	if onesum(pkt.ip, 20, 0) != 0xffff {
		return;
	}

	if pkt.ip[0] != 0x45:byte {
		return;
	}

	len = (pkt.ip[2]:int << 8)
		+ pkt.ip[3]:int;
	if len < 20 || len > pkt.ip_len {
		return;
	}
	pkt.ip_len = len;

	frag = (pkt.ip[6]:int << 8)
		+ pkt.ip[7]:int;

	if frag & (1 << 15) != 0 || frag & 8191 != 0 {
		return;
	}

	pkt.ip_proto = pkt.ip[9]:int;

	pkt.ip_src = (pkt.ip[12]:int << 24)
		+ (pkt.ip[13]:int << 16)
		+ (pkt.ip[14]:int << 8)
		+ pkt.ip[15]:int;
	pkt.ip_dest = (pkt.ip[16]:int << 24)
		+ (pkt.ip[17]:int << 16)
		+ (pkt.ip[18]:int << 8)
		+ pkt.ip[19]:int;

	// Pesudo header sum
	pkt.ip_psum = (pkt.ip_src & 0xffff)
		+ ((pkt.ip_src >> 16) & 0xffff)
		+ (pkt.ip_dest & 0xffff)
		+ ((pkt.ip_dest >> 16) & 0xffff)
		+ pkt.ip_proto
		+ (pkt.ip_len - 20);

	memo_arp(pkt.ether_src, pkt.ip_src, 1);

	if pkt.ip_dest != global.ip {
		return;
	}

	if pkt.ip_proto == IP_ICMP {
		pkt.icmp = &pkt.ip[20];
		pkt.icmp_len = pkt.ip_len - 20;
		rx_icmp(pkt);
	} else if pkt.ip_proto == IP_TCP {
		pkt.tcp = &pkt.ip[20];
		pkt.tcp_len = pkt.ip_len - 20;
		rx_tcp(pkt);
	} else if pkt.ip_proto == IP_UDP {
		pkt.udp = &pkt.ip[20];
		pkt.udp_len = pkt.ip_len - 20;
		rx_udp(pkt);
	}
}

rx_ether(pkt: *rxinfo) {
	if pkt.ether_len < 14 {
		return;
	}

	pkt.ether_dest = (pkt.ether[0]:int << 40)
		+ (pkt.ether[1]:int << 32)
		+ (pkt.ether[2]:int << 24)
		+ (pkt.ether[3]:int << 16)
		+ (pkt.ether[4]:int << 8)
		+ pkt.ether[5]:int;
	pkt.ether_src = (pkt.ether[6]:int << 40)
		+ (pkt.ether[7]:int << 32)
		+ (pkt.ether[8]:int << 24)
		+ (pkt.ether[9]:int << 16)
		+ (pkt.ether[10]:int << 8)
		+ pkt.ether[11]:int;
	pkt.ether_type = (pkt.ether[12]:int << 8)
		+ (pkt.ether[13]:int);

	if pkt.ether_type == ET_ARP {
		pkt.arp = &pkt.ether[14];
		pkt.arp_len = pkt.ether_len - 14;
		rx_arp(pkt);
	} else if pkt.ether_type == ET_IP {
		pkt.ip = &pkt.ether[14];
		pkt.ip_len = pkt.ether_len - 14;
		rx_ip(pkt);
	}
}

isr_realtek() {
	var pkt: rxinfo;
	var global: *global;
	var port: *realtek_port;
	var i: int;
	var packet: *byte;
	var len: int;

	global = g();

	port = global.realtek_port;
	loop {
		if !port {
			break;
		}

		// clear interrupt flags
		outw(port.io + 0x3e, 0xffff);

		i = 0;
		loop {
			if i == port.rx.count {
				break;
			}

			if port.rx.ring[i].flags & (1 << 31) == 0 {
				pkt.port = port;
				pkt.ether = ptov(port.rx.ring[i].framep);
				pkt.ether_len = port.rx.ring[i].flags & 4095;

				if pkt.ether_len >= 4 {
					pkt.ether_len = pkt.ether_len - 4;
				}

				rx_ether(&pkt);

				port.rx.ring[i].flags = (1 << 31) + (((i + 1) == port.rx.count) << 30) + 4095;
			}

			i = i + 1;
		}

		port = port.next;
	}
}

init_ahci(dev: *pcidev) {
	var global: *global;
	var ahci: *byte;

	global = g();

	if dev.cls != 1 || dev.subcls != 6 {
		return;
	}

	ahci = map_pci(dev.bar5);

	// Enable dma
	_w16(&dev.addr[4], 6);

	// Enable message signaled interrupts
	_w16(&dev.msi[8], 34);
	_w32(&dev.msi[4], global.lapicp);
	_w16(&dev.msi[2], 0x0001);

	// Find populated ports
	if _r32(&ahci[0x00]) & (1 << 31) == 0 {
		kputs("AHCI does not support 64 bit dma\n");
		return;
	}

	// Enable ahci and interrupts
	_w32(&ahci[0x4], (1 << 31) + (1 << 1));
	_w32(&ahci[0x08], -1);

	var pi: int;
	var i: int;

	pi = _r32(&ahci[0xc]);
	i = 0;
	loop {
		if pi == 0 {
			break;
		}

		if pi & 1 {
			init_ahci_port(ahci, i);
		}

		pi = pi >> 1;
		i = i + 1;
	}
}

struct ahci_port {
	next: *ahci_port;
	ahci: *byte;
	port: *byte;
	cmd: *byte;
	ctabp: int;
	ctab: *byte;
	fis: *byte;
	bufp: int;
	buf: *byte;
}

init_ahci_port(ahci: *byte, i: int) {
	var global: *global;
	var port: *byte;

	global = g();

	port = &ahci[0x100 + 0x80 * i];

	// Set ST=0
	_w32(&port[0x18], _r32(&port[0x18]) & -2 & ~(15 << 28));

	// Wait for DMA finish
	loop {
		if _r32(&port[0x18]) & (1 << 15) == 0 {
			break;
		}
	}

	// Clear errors
	_w32(&port[0x30], -1);

	var ahci_port: *ahci_port;
	ahci_port = ptov(alloc_page()): *ahci_port;
	ahci_port.next = 0:*ahci_port;
	ahci_port.ahci = ahci;
	ahci_port.port = port;

	// Allocate queues
	var cmdp: int;
	var fisp: int;
	var ctabp: int;

	// each command list header is 32 bytes, there are 32 of them
	cmdp = alloc_page();
	ahci_port.cmd = ptov(cmdp);
	bzero(ahci_port.cmd, 4096);

	//// each table must be aligned to 128 bytes
	//// allocate one 128 block to each command list from this page
	ctabp = alloc_page();
	ahci_port.ctabp = ctabp;
	ahci_port.ctab = ptov(ctabp);
	bzero(ahci_port.ctab, 4096);

	//// fis is 256 bytes
	fisp = alloc_page();
	ahci_port.fis = ptov(fisp);
	bzero(ahci_port.fis, 4096);

	// Set command list and fis pointers
	_w32(&port[0x00], cmdp);
	_w32(&port[0x04], cmdp >> 32);
	_w32(&port[0x08], fisp);
	_w32(&port[0x0c], fisp >> 32);

	// Spin up device
	_w32(&port[0x18], 0x11);

	// Wait for busy and drq to clear
	loop {
		if _r32(&port[0x20]) & 0x88 == 0 {
			break;
		}
	}

	// Enable interrupts
	_w32(&port[0x14], -1);
	_w32(&port[0x10], -1);

	// Send ATA IDENTIFY
	ahci_port.bufp = alloc_page();
	ahci_port.buf = ptov(ahci_port.bufp);

	// Set AHCI Doorbell
	//bzero(ahci_port.buf, 4096);
	//fill_fis_h2d(ahci_port, ATA_READ, 0);
	//_w32(&ahci_port.port[0x38], 1);

	ahci_port.next = global.ahci_port;
	global.ahci_port = ahci_port;
}

enum {
	ATA_IDENTIFY = 0xec,
	ATA_READ = 0x25,
	ATA_WRITE = 0x35,
}

fill_fis_h2d(ahci_port: *ahci_port, cmd: int, lba: int) {
	var w: int;
	var size: int;

	size = 4096;
	if cmd == ATA_IDENTIFY {
		size = 512;
	}

	bzero(ahci_port.cmd, 4096);

	w = (cmd == ATA_WRITE);

	// Set command list header: prd_count + cmd + write + cmd_words
	_w32(&ahci_port.cmd[0x00], (1 << 16) + (1 << 10) + (w << 6) + 5);
	_w32(&ahci_port.cmd[0x04], 0);
	_w32(&ahci_port.cmd[0x08], ahci_port.ctabp);
	_w32(&ahci_port.cmd[0x0c], ahci_port.ctabp >> 32);

	bzero(ahci_port.ctab, 4096);

	// Fill command FIS
	ahci_port.ctab[0] = 0x27:byte;
	ahci_port.ctab[1] = 0x80:byte;
	ahci_port.ctab[2] = cmd:byte;
	ahci_port.ctab[3] = 0:byte;

	ahci_port.ctab[4] = lba:byte;
	ahci_port.ctab[5] = (lba >> 8):byte;
	ahci_port.ctab[6] = (lba >> 16):byte;
	ahci_port.ctab[7] = (1 << 6):byte;

	ahci_port.ctab[8] = (lba >> 24):byte;
	ahci_port.ctab[9] = (lba >> 32):byte;
	ahci_port.ctab[10] = (lba >> 40):byte;
	ahci_port.ctab[11] = 0:byte;

	// Count of sectors
	ahci_port.ctab[12] = (size >> 9):byte;
	ahci_port.ctab[13] = (size >> 17):byte;
	ahci_port.ctab[14] = 0:byte;
	ahci_port.ctab[15] = 0:byte;

	ahci_port.ctab[16] = 0:byte;
	ahci_port.ctab[17] = 0:byte;
	ahci_port.ctab[18] = 0:byte;
	ahci_port.ctab[19] = 0:byte;

	// Fill PRDT
	_w32(&ahci_port.ctab[0x80], ahci_port.bufp);
	_w32(&ahci_port.ctab[0x84], ahci_port.bufp >> 32);
	_w32(&ahci_port.ctab[0x88], 0);
	_w32(&ahci_port.ctab[0x8c], (1 << 31) + (size - 1));
}

isr_ahci() {
	var global: *global;
	var ahci_port: *ahci_port;
	var i: int;

	global = g();

	ahci_port = global.ahci_port;
	loop {
		if !ahci_port {
			break;
		}

		_w32(&ahci_port.port[0x10], -1);

		ahci_port = ahci_port.next;
	}
}

tick() {
	var global: *global;
	var tcb: *tcp_state;
	var i: int;
	global = g();

	global.ms = global.ms + 1;

	i = 0;
	loop {
		if i == global.tcp_count {
			break;
		}

		tcb = global.tcp[i];
		if tcb {
			tcp_tick(tcb);
			tcb.event_func(tcb);
		}

		i = i + 1;
	}
}

kdie(msg: *byte) {
	kputs("die: ");
	kputs(msg);
	kputs("\n");
	cli();
	loop {
		hlt();
	}
}

sleep(ms: int) {
	var global: *global;
	var deadline: int;

	global = g();

	if ms <= 0 {
		return;
	}

	if rdflags() & 0x200 == 0 {
		kdie("attempt to sleep with interrupts disabled");
	}

	deadline = global.ms + ms;
	loop {
		if global.ms > deadline {
			break;
		}
		hlt();
	}
}

xxd(data: *byte, len: int) {
	var i: int;
	var j: int;
	loop {
		if i >= len {
			break;
		}

		kputh32(i);

		kputc(':');
		kputc(' ');

		j = 0;

		loop {
			if j == 16 {
				break;
			}

			if i + j < len {
				kputh8(data[i + j]: int);
			} else {
				kputc(' ');
				kputc(' ');
			}

			if i + j + 1 < len {
				kputh8(data[i + j + 1]: int);
			} else {
				kputc(' ');
				kputc(' ');
			}

			kputc(' ');

			j = j + 2;
		}

		kputc(' ');

		j = 0;
		loop {
			if j == 16 || i + j >= len {
				break;
			}

			if data[i + j]:int >= 0x20 && data[i + j]:int < 0x80 {
				kputc(data[i + j]: int);
			} else {
				kputc('.');
			}

			j = j + 1;
		}

		kputc('\n');

		i = i + 16;
	}
}

read_cmos(a: int): int {
	outb(0x70, (a & 0x7f) | 0x80);
	return inb(0x71);
}

read_rtc() {
	var global: *global;
	var epoch_time: int;
	var days_since_epoch: int;
	var sec: int;
	var min: int;
	var hour: int;
	var day: int;
	var mon: int;
	var year: int;
	var flags: int;
	var days_in_month: *byte;
	global = g();

	sec = 0;
	min = 0;
	hour = 0;
	day = 0;
	mon = 0;
	year = 0;
	flags = 0;

	loop {
		if read_cmos(0xa) & 0x80 {
			continue;
		}

		if read_cmos(0x00) != sec {
			sec = read_cmos(0x00);
			continue;
		}

		if read_cmos(0x02) != min {
			min = read_cmos(0x02);
			continue;
		}

		if read_cmos(0x04) != hour {
			hour = read_cmos(0x04);
			continue;
		}

		if read_cmos(0x07) != day {
			day = read_cmos(0x07);
			continue;
		}

		if read_cmos(0x08) != mon {
			mon = read_cmos(0x08);
			continue;
		}

		if read_cmos(0x09) != year {
			year = read_cmos(0x09);
			continue;
		}

		if read_cmos(0x0b) != flags {
			flags = read_cmos(0x0b);
			continue;
		}

		break;
	}

	// bcd -> binary
	if flags & 0x04 == 0 {
		sec = (sec & 15) + (((sec >> 4) & 15) * 10);
		min = (min & 15) + (((min >> 4) & 15) * 10);
		hour = (hour & 15) + (((hour >> 4) & 15) * 10) + (hour & 0x80);
		day = (day & 15) + (((day >> 4) & 15) * 10);
		mon = (mon & 15) + (((mon >> 4) & 15) * 10);
		year = (year & 15) + (((year >> 4) & 15) * 10);
	}

	// 12 hour clock
	if flags & 0x02 == 0 && hour & 0x80 {
		hour = ((hour & 0x7f) + 12) % 24;
	}

	// Current century
	year = year + 2000;

	days_since_epoch = year * 365
		+ (year / 4)
		- (year / 100)
		+ (year / 400)
		+ (day - 1)
		- 719527;

	if (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0) {
		days_in_month = " 313232332323";
	} else {
		days_in_month = " 303232332323";
	}

	var i: int;
	i = 1;
	loop {
		if i >= mon {
			break;
		}

		days_since_epoch = days_since_epoch + (days_in_month[i]:int - '0' + 28);

		i = i + 1;
	}

	epoch_time = days_since_epoch * 86400 + hour * 3600 + min * 60 + sec;

	global.boot_time = epoch_time;
}

_kstart(mb: int) {
	var global: global;
	var brk: int;
	var tss: *int;
	var tss_size: int;
	var idt: *int;
	var idt_size: int;
	var gdt: *int;
	var gdt_size: int;
        var xsdt: int;
        var mcfg: int;
	var pcip: int;
	var pci: *byte;
	var flag: int;
	var mbinfo: *byte;
	var mmap: *int;
	var mmap_len: int;
	var mmap_count: int;
	var fr: *free_range;

	bzero((&global):*byte, sizeof(global));
	global.ptr = &global;
	global.ip = (192 << 24) + (168 << 16) + (1 << 8) + 148;
	global.ip_gw = (192 << 24) + (168 << 16) + (1 << 8) + 1;
	global.ip_mask = 20;
	wrmsr((0xc000 << 16) + 0x0101, global.ptr:int);

	global.mmio = -(1 << 31);

	vinit(&global.vga, ptov(0xB8000));
	vclear(&global.vga);
	//kputs("Starting up\n");

	global.fr = 0:*free_range;
	global.fp = 0:*free_page;

	brk = ptov(1024 * 1024 * 3):int;

	mbinfo = ptov(mb);
	mmap = ptov(_r32(&mbinfo[48])): *int;
	mmap_len = _r32(&mbinfo[44]);
	mmap_count = mmap_len / 24;

	insert_sort(mmap:*void, mmap_count, 24, mmap_cmp);

	var i: int;
	var mmap_start: int;
	var mmap_end: int;
	i = 0;
	loop {
		if i == mmap_count {
			break;
		}

		mmap_start = mmap[i * 3];
		mmap_end = mmap_start + mmap[i * 3 + 1];

		if i + 1 < mmap_count {
			if mmap_end > mmap[i * 3 + 3] {
				kdie("OVERLAP");
				loop {
					cli();
					hlt();
				}
			}
		}

		mmap_start = (mmap_start + 4095) & -4096;
		mmap_end = mmap_end & -4096;

		if mmap_start < 0x400000 {
			mmap_start = 0x400000;
		}

		if mmap_start < mmap_end && _r32((&mmap[i * 3 + 2]):*byte) == 1 {
			fr = brk: *free_range;
			brk = brk + sizeof(*fr);
			fr.next = global.fr;
			fr.start = mmap_start;
			fr.end = mmap_end;
			global.fr = fr;
		}

		i = i + 1;
	}

	// Directly map all memory to just above the architectural hole
	direct_map(&brk);

	// Zero tss and add interrupt stacks
	tss = brk: *int;
	tss_size = 104;
	brk = brk + tss_size;
	bzero(tss: *byte, tss_size);

	// Fill idt with interrupt gates
	idt = brk: *int;
	idt_size = 4096;
	brk = brk + idt_size;
	fill_idt(idt);

	gdt = brk: *int;
	gdt_size = 8 * 7;

	// Null segment
	gdt[0] = 0;
	// Kernel code segment
	gdt[1] = 0x00209800 << 32;
	// Kernel data segment
	gdt[2] = 0x00009200 << 32;
	// User code segment
	gdt[3] = 0x0020f800 << 32;
	// User data segment
	gdt[4] = 0x0000f200 << 32;
	// Task segment
	gdt_tss(&gdt[5], tss: int, tss_size, 0x89, 0);

	// Load gdt idt tss and segments
	lgdt(gdt, gdt_size);
	lseg(8, 16);
	wrmsr((0xc000 << 16) + 0x0101, global.ptr:int);
	lldt(0);
	ltr(5 * 8);
	lidt(idt, idt_size);

	// interrupt stack
	brk = (brk + 4095) & -4096;
	brk = brk + 64 * 1024;
	((tss:int + 0x24):*int)[0] = brk;
	((tss:int + 0x04):*int)[0] = brk;

	// nmi stack
	brk = (brk + 4095) & -4096;
	brk = brk + 64 * 1024;
	((tss:int + 0x2c):*int)[0] = brk;

	// mask pic
	outb(IO_PIC1 + 1, 0xff);
	outb(IO_PIC2 + 1, 0xff);

	// initialize pic
	outb(IO_PIC1, 0x11);
	outb(IO_PIC2, 0x11);

	// pic offset
	outb(IO_PIC1 + 1, 32);
	outb(IO_PIC2 + 1, 40);

	// pic cascade
	outb(IO_PIC1 + 1, 4);
	outb(IO_PIC2 + 1, 2);

	// pic 8086 mode
	outb(IO_PIC1 + 1, 32);
	outb(IO_PIC2 + 1, 40);

	// 1000 hz pit
	outb(IO_PIT + 3, 0x36);
	outb(IO_PIT + 0, 0xa9);
	outb(IO_PIT + 0, 0x04);

	// unmask pit
	outb(IO_PIC1 + 1, 0xfe);
	outb(IO_PIC2 + 1, 0xff);

	// Allocate network state tables
	global.arp = alloc():*arp_entry;
	global.arp_count = 4096 / sizeof(*global.arp);
	global.tcp = alloc():**tcp_state;
	global.tcp_count = 4096 / sizeof(*global.tcp);
	bzero(global.arp: *byte, 4096);
	bzero(global.tcp: *byte, 4096);

	read_rtc();
	global.rng = global.boot_time;
	sti();

        // Find ACPI tables
	xsdt = find_xsdt();
        mcfg = find_acpi(xsdt, "MCFG");
	if !mcfg {
		kdie("No mcfg?\n");
	}

	pcip = (ptov(mcfg + 44):*int)[0];
	if !pcip {
		kdie("No pci?\n");
	}

	// Enable the local apic
	var lapicp: int;
	lapicp = rdmsr(0x1b) & -4096;
	var lapic: *byte;
	lapic = map_pci(lapicp);
	_w32(&lapic[0xf0], _r32(&lapic[0xf0]) | 0x1ff);
	global.lapicp = lapicp;
	global.lapic = lapic;

	pci = map_pci(pcip);
	//scan_pci(pci, show_pcidev);
	scan_pci(pci, init_realtek);
	scan_pci(pci, init_ahci);

	tcp_listen(22, tcp_echo);

	// Wait for interrupts
	kputs("zzz\n");
	loop {
		hlt();
	}
}
