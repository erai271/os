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

	// pic end of interrupt
	//if (r.trap >= 32 && r.trap < 48) {
	//	if (r.trap < 40) {
	//		outb(IO_PIC1, 0x0b);
	//		if inb(IO_PIC1) & (1 << (r.trap & 7)) {
	//			outb(IO_PIC1, 0x20);
	//		} else {
	//			kputs("s\n");
	//		}
	//	} else {
	//		outb(IO_PIC2, 0x0b);
	//		if inb(IO_PIC2) & (1 << (r.trap & 7)) {
	//			outb(IO_PIC2, 0x20);
	//			outb(IO_PIC1, 0x20);
	//		} else {
	//			kputs("s\n");
	//		}
	//	}
	//}
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

//enum {
//	E1_TXDESCLO = 0x3800,
//	E1_TXDESCHI = 0x3804,
//	E1_TXDESCLEN = 0x3808,
//	E1_TXDESCHEAD = 0x3810,
//	E1_TXDESCTAIL = 0x3818,
//	E1_TCTRL = 0x0400,
//	E1_TIPG = 0x0410,
//
//	E1_RXDESCLO = 0x2800,
//	E1_RXDESCHI = 0x2804,
//	E1_RXDESCLEN = 0x2808,
//	E1_RXDESCHEAD = 0x2810,
//	E1_RXDESCTAIL = 0x2818,
//	E1_RCTRL = 0x0100,
//
//	E1_IMASK = 0x00d0,
//	E1_ICR = 0x00C0,
//}

onesum(h: *byte, n: int): int {
	var i: int;
	var s: int;
	i = 0;
	s = 0;
	loop {
		if i >= n {
			break;
		}
		s = s + (h[i]:int << 8) + (h[i + 1]:int);
		s = (s & 0xffff) + ((s >> 16) & 1);
		i = i + 2;
	}
	return s;
}

ip_checksum(h: *byte) {
	var s: int;
	h[10] = 0:byte;
	h[11] = 0:byte;
	s = ~onesum(h, 20);
	h[10] = (s >> 8):byte;
	h[11] = s:byte;
}

icmp_checksum(p: *byte) {
	var s: int;
	p[2] = 0: byte;
	p[3] = 0: byte;
	s = ~onesum(p, 8);
	p[2] = (s >> 8):byte;
	p[3] = s:byte;
}

//init_e1000(dev: *pcidev) {
//	var flag: *int;
//	var base: *byte;
//	var mac: int;
//	var i: int;
//	var txring: *byte;
//
//	if *flag || dev.vid != 0x8086 || (
//		dev.did != 0x100e
//		&& dev.did != 0x1539
//	) {
//		return;
//	}
//	*flag = 1;
//	*dev.trap = 10:byte;
//
//	base = map_bar(ptov(-(1<<34)), dev);
//	if !base {
//		return;
//	}
//
//	// Enable dma
//	_w16(&dev.addr[4], 6);
//
//	// Disable interrupts
//	_w32(&base[E1_IMASK], 0);
//
//	// Clear MTA
//	i = 0;
//	loop {
//		if i == 0x80 {
//			break;
//		}
//		_w32(&base[0x5200 + 4 * i], 0);
//		i = i + 1;
//	}
//
//	// Read mac
//	mac = _r32(&base[0x5400]) + (_r32(&base[0x5404])) << 32;
//
//	txring = alloc_page();
//
//	i = 0;
//	loop {
//		if i == 16 {
//			break;
//		}
//		bzero(&txring[16*32 + 16*i], 16);
//		((&txring[16*32 + 16*i]):*int)[0] = 1024*1024*3 + 4096 * (i + 1);
//		_w16(&txring[16*32 + 16*i + 8], 2048);
//		i = i + 1;
//	}
//
//	// Create tx descriptors
//	_w32(&base[E1_TXDESCLO], 1024*1024*3);
//	_w32(&base[E1_TXDESCHI], 0);
//	_w32(&base[E1_TXDESCLEN], 16*32);
//	_w32(&base[E1_TXDESCHEAD], 0);
//	_w32(&base[E1_TXDESCTAIL], 0);
//
//	// Create rx descriptors
//	_w32(&base[E1_RXDESCLO], 1024*1024*3+16*32);
//	_w32(&base[E1_RXDESCHI], 0);
//	_w32(&base[E1_RXDESCLEN], 16*32);
//	_w32(&base[E1_RXDESCHEAD], 0);
//	_w32(&base[E1_RXDESCTAIL], 31);
//
//	// Enable interrupts
//	_w32(&base[E1_IMASK], 0);
//
//	// Enable rx and tx
//	_w32(&base[E1_TCTRL], 0x3003f0fa);
//	_w32(&base[E1_RCTRL],
//		// Enable
//		(1 << 1)
//		// StoreBadPackets
//		+ (1 << 2)
//		// UnicastPromiscuous
//		+ (1 << 3)
//		// MulticastPromiscuous
//		+ (1 << 4)
//		// LoobackMode
//		+ (0 << 6)
//		// BroadcastAccept
//		+ (1 << 15)
//		// ReceiveBufferSize
//		+ (0 << 16)
//		// StripEthernetCRC
//		+ (1 << 26));
//
//	// Send a packet
//	(txring:*int)[0] = 1024*1024*3 + 16*32*2;
//	_w16(&txring[8], 60);
//	txring[10] = 0:byte;
//	// ReportStatus + InsertFrameCheckSequence + EndOfPacket
//	txring[11] = ((0 << 3) + (1 << 1) + 1):byte;
//	txring[12] = 0:byte;
//	txring[13] = 0:byte;
//	txring[14] = 0:byte;
//	txring[15] = 0:byte;
//	txring[16*32*2 + 0] = (mac >> 40):byte;
//	txring[16*32*2 + 1] = (mac >> 32):byte;
//	txring[16*32*2 + 2] = (mac >> 24):byte;
//	txring[16*32*2 + 3] = (mac >> 16):byte;
//	txring[16*32*2 + 4] = (mac >> 8):byte;
//	txring[16*32*2 + 5] = mac:byte;
//	txring[16*32*2 + 6] = 0x36:byte;
//	txring[16*32*2 + 7] = 0xb9:byte;
//	txring[16*32*2 + 8] = 0x55:byte;
//	txring[16*32*2 + 9] = 0x54:byte;
//	txring[16*32*2 + 10] = 0xba:byte;
//	txring[16*32*2 + 11] = 0xcf:byte;
//	txring[16*32*2 + 12] = 0x08:byte;
//	txring[16*32*2 + 13] = 0x00:byte;
//
//	txring[16*32*2 + 14] = 0x45:byte;
//	txring[16*32*2 + 15] = 0:byte;
//	txring[16*32*2 + 16] = 0:byte;
//	txring[16*32*2 + 17] = 28:byte;
//	txring[16*32*2 + 18] = 0:byte;
//	txring[16*32*2 + 19] = 0:byte;
//	txring[16*32*2 + 20] = 0:byte;
//	txring[16*32*2 + 21] = 0:byte;
//	txring[16*32*2 + 22] = 64:byte;
//	txring[16*32*2 + 23] = 1:byte;
//	txring[16*32*2 + 24] = 0:byte;
//	txring[16*32*2 + 25] = 0:byte;
//	txring[16*32*2 + 26] = 192:byte;
//	txring[16*32*2 + 27] = 168:byte;
//	txring[16*32*2 + 28] = 32:byte;
//	txring[16*32*2 + 29] = 2:byte;
//	txring[16*32*2 + 30] = 192:byte;
//	txring[16*32*2 + 31] = 168:byte;
//	txring[16*32*2 + 32] = 32:byte;
//	txring[16*32*2 + 33] = 1:byte;
//
//	ip_checksum(&txring[16*32*2 + 14]);
//
//	txring[16*32*2 + 34] = 8:byte;
//	txring[16*32*2 + 35] = 0:byte;
//	txring[16*32*2 + 36] = 0:byte;
//	txring[16*32*2 + 37] = 0:byte;
//	txring[16*32*2 + 38] = 0:byte;
//	txring[16*32*2 + 39] = 0:byte;
//	txring[16*32*2 + 40] = 0:byte;
//
//	icmp_checksum(&txring[16*32*2 + 34]);
//
//	_w32(&base[E1_TXDESCTAIL], 1);
//	_w32(&base[E1_IMASK], (1 << 0) + (1 << 4));
//}

//init_nvme(dev: *pcidev) {
//	var flag: *int;
//	var base: *byte;
//
//	if *flag || dev.cls != 1 || dev.subcls != 8 {
//		return;
//	}
//	*flag = 1;
//	*dev.trap = 11:byte;
//
//	// Enable dma
//	_w16(&dev.addr[4], 6);
//
//	// Map bar
//	base = map_bar(ptov(-(1<<36)), dev);
//	if !base {
//		return;
//	}
//
//	kputs("nvme\n");
//
//	// setup admin ring
//	// enable interrupts
//}

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

struct global {
	ptr: *global;
	ms: int;
	vga: vga;
	fr: *free_range;
	fpage: *free_page;
	lapicp: int;
	lapic: *byte;
	mmio: int;
	realtek_port: *realtek_port;
	ahci_port: *ahci_port;
}

struct free_page {
	next: *free_page;
}

struct free_range {
	next: *free_range;
	start: int;
	end: int;
}

g(): *global {
	return _rgs(0):**global[0];
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

alloc_page(): int {
	var global: *global;
	var fr: *free_range;
	var ret: int;
	global = g();
	fr = global.fr;
	loop {
		if !fr {
			break;
		}

		if fr.start < fr.end {
			ret = fr.start;
			fr.start = fr.start + 4096;
			return ret;
		}

		fr = fr.next;
	}

	kputs("OOM\n");

	loop {
		cli();
		hlt();
	}
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

fill_packet(ringp: int) {
	var ring: *byte;
	var packet: *byte;

	ring = ptov(ringp);
	packet = ptov(ring:*int[1]);

	// Send a packet
	packet[0] = 0xff:byte;
	packet[1] = 0xff:byte;
	packet[2] = 0xff:byte;
	packet[3] = 0xff:byte;
	packet[4] = 0xff:byte;
	packet[5] = 0xff:byte;
	packet[6] = 0xff:byte;
	packet[7] = 0xff:byte;
	packet[8] = 0xff:byte;
	packet[9] = 0xff:byte;
	packet[10] = 0xff:byte;
	packet[11] = 0xff:byte;
	packet[12] = 0x08:byte;
	packet[13] = 0x00:byte;

	packet[14] = 0x45:byte;
	packet[15] = 0:byte;
	packet[16] = 0:byte;
	packet[17] = 28:byte;
	packet[18] = 0:byte;
	packet[19] = 0:byte;
	packet[20] = 0:byte;
	packet[21] = 0:byte;
	packet[22] = 64:byte;
	packet[23] = 1:byte;
	packet[24] = 0:byte;
	packet[25] = 0:byte;
	packet[26] = 192:byte;
	packet[27] = 168:byte;
	packet[28] = 2:byte;
	packet[29] = 100:byte;
	packet[30] = 192:byte;
	packet[31] = 168:byte;
	packet[32] = 1:byte;
	packet[33] = 178:byte;

	ip_checksum(&packet[14]);

	packet[34] = 8:byte;
	packet[35] = 0:byte;
	packet[36] = 0:byte;
	packet[37] = 0:byte;
	packet[38] = 0:byte;
	packet[39] = 0:byte;
	packet[40] = 0:byte;

	icmp_checksum(&packet[34]);

	ring:*int[0] = (0xb000 << 16) + (64);
}

struct realtek_port {
	next: *realtek_port;
	io: int;
	rxring: *byte;
	txring: *byte;
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

	var txringp: int;
	txringp = alloc_page();
	setup_ring(txringp, 0);
	fill_packet(txringp);

	// tx ring
	outd(io + 0x20, txringp);
	outd(io + 0x24, txringp >> 32);

	// tx max size
	outb(io + 0xec, 16);

	var rxringp: int;
	rxringp = alloc_page();
	setup_ring(rxringp, 1);

	// rx ring
	outd(io + 0xe4, rxringp);
	outd(io + 0xe8, rxringp >> 32);

	// Enable rx/tx
	outd(io + 0x37, 0x0c);

	// Enable interrupts
	outw(io + 0x3c, 0x00ff);
	outw(io + 0x3e, 0xffff);

	// Config write disable
	outb(io + 0x50, 0x00);

	// TX Doorbell
	outb(io + 0x38, 0x40);

	realtek_port = ptov(alloc_page()):*realtek_port;
	realtek_port.next = global.realtek_port;
	realtek_port.io = io;
	realtek_port.rxring = ptov(rxringp);
	realtek_port.txring = ptov(txringp);
	global.realtek_port = realtek_port;
}

isr_realtek() {
	var global: *global;
	var realtek_port: *realtek_port;
	var i: int;

	global = g();

	realtek_port = global.realtek_port;
	loop {
		if !realtek_port {
			break;
		}

		i = 0;
		loop {
			if i == 16 {
				break;
			}

			_w32(&realtek_port.rxring[i * 16], (1 << 31) | ((i == 15) << 30) | 4096);
			_w32(&realtek_port.rxring[i * 16 + 4], 0);

			i = i + 1;
		}

		// clear interrupt flags
		outw(realtek_port.io + 0x3e, 0xffff);

		realtek_port = realtek_port.next;
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

	// Set command list header
	bzero(ahci_port.cmd, 4096);
	_w32(&ahci_port.cmd[0x00], (1 << 16) + (1 << 10) + 5);
	_w32(&ahci_port.cmd[0x04], 0);
	_w32(&ahci_port.cmd[0x08], ahci_port.ctabp);
	_w32(&ahci_port.cmd[0x0c], ahci_port.ctabp >> 32);

	bzero(ahci_port.ctab, 4096);

	// Fill command FIS
	ahci_port.ctab[0x00] = 0x27:byte;
	ahci_port.ctab[0x01] = 0x80:byte;
	ahci_port.ctab[0x02] = 0xec:byte;

	// Fill PRDT
	_w32(&ahci_port.ctab[0x80], ahci_port.bufp);
	_w32(&ahci_port.ctab[0x84], ahci_port.bufp >> 32);
	_w32(&ahci_port.ctab[0x88], 0);
	_w32(&ahci_port.ctab[0x8c], (1 << 31) + 511);

	// Set AHCI Doorbell
	_w32(&ahci_port.port[0x38], 1);

	//loop {
	//	_w32(&port[0x30], -1);
	//	if _r32(&ahci_port.port[0x38]) & 1 == 0 {
	//		break;
	//	}
	//}

	//if _r32(&port[0x30]) != 0 {
	//	kputs("got error");
	//	return;
	//}

	//kputs("Found ");
	//kputd((512 * (&ahci_port.buf[200]):*int[0]) / 1000000000);
	//kputs("GB\n");

	ahci_port.next = global.ahci_port;
	global.ahci_port = ahci_port;
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
	global = g();
	global.ms = global.ms + 1;
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
	wrmsr((0xc000 << 16) + 0x0101, global.ptr:int);

	global.mmio = -(1 << 31);

	vinit(&global.vga, ptov(0xB8000));
	vclear(&global.vga);
	kputs("Starting up\n");

	global.fr = 0:*free_range;

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

	sti();

        // Find ACPI tables
	xsdt = find_xsdt();
        mcfg = find_acpi(xsdt, "MCFG");
	if !mcfg {
		kputs("No mcfg?\n");
	}

	pcip = (ptov(mcfg + 44):*int)[0];
	if !pcip {
		kputs("No pci?\n");
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
	scan_pci(pci, show_pcidev);
	scan_pci(pci, init_realtek);
	scan_pci(pci, init_ahci);

	// Wait for interrupts
	kputs("Sleeping...\n");
	loop {
		hlt();
	}
}
