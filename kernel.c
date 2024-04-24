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

_isr(r: *regs) {
	if (r.trap < 32) {
		//sputs("EX ");
		//sputd(r.trap);
		//sputs("\n");
	}

	if (r.trap == 2) {
		//sputs("NMI\n");
	}

	var vga: *byte;

	vga = ptov(0xB8000);

	vga[2] = ('A' + r.trap):byte;
	vga[3] = 0x0e:byte;

	// pic end of interrupt
	if (r.trap >= 32 && r.trap < 48) {
		if (r.trap == 32) {
		} else if (r.trap == 42) {
			//sputc('n');
			//sputc('\n');
		} else if (r.trap == 43) {
			// ethernet interrupt
			//sputc('e');
		}
		if (r.trap < 40) {
			outb(IO_PIC1, 0x0b);
			if inb(IO_PIC1) & (1 << (r.trap & 7)) {
				outb(IO_PIC1, 0x20);
			} else {
				//sputs("s\n");
			}
		} else {
			outb(IO_PIC2, 0x0b);
			if inb(IO_PIC2) & (1 << (r.trap & 7)) {
				outb(IO_PIC2, 0x20);
				outb(IO_PIC1, 0x20);
			} else {
				//sputs("s\n");
			}
		}
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
	return (p + (-0x8000 << 16)): *byte;
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

show_addr(x: int) {
	var vga: *byte;
	var i: int;
	var j: int;

	vga = ptov(0xB8000);

	i = 64;
	j = 160;
	loop {
		if i == 0 {
			break;
		}
		i = i - 4;
		vga[j] = "0123456789abcdef"[(x >> i) & 15];
		vga[j + 1] = 0x0e:byte;
		j = j + 2;
	}

}

find_xsdt(): int {
	var p: int;

	// Find the RDSP
	p = 0xe0000;
	loop {
		if p + 20 >= 0xfffff {
                        return 0;
		}
		if !memcmp(ptov(p), "RSD PTR ", 8) {
			if !bytesum(ptov(p), 20) {
				break;
			}
		}
		p = p + 16;
	}

	//p = _r32(ptov(p + 16));

	p = ptov(p + 24):*int[0];
	show_addr(p);
	//if valid_table(p, "XSDT") == 0 {
		//return 0;
	//}

	return p;
}

find_acpi(rsdt: int, sig: *byte): int {
	var i: int;
	var len: int;
	var p: int;

	if rsdt == 0 {
		return 0;
	}

	i = 36;
	len = acpi_len(rsdt);

	loop {
		if i >= len {
			return 0;
		}

		p = _r32(ptov(rsdt + i));

		if valid_table(p, sig) != 0 {
			return p;
		}

		i = i + 4;
	}
}

map_pci(va: *byte, pa: int): *byte {
	var pt4p: int;
	var pt4: *int;
	var pt3: *int;
	var v2: int;
	var flags: int;
	pt4p = rdcr3();
	pt4 = ptov(pt4p):*int;
	v2 = ((va: int) >> 30) & 511;
	pt3 = ptov(pt4[511] & -4096):*int;
	flags = 0x93;
	pt3[v2] = (pa & -(1 << 30)) | flags;
	pt3[v2 + 1] = ((pa + (1 << 30)) & -(1 << 30)) | flags;
	wrcr3(pt4p);
	return &va[pa & ((1 << 30) - 1)];
}

struct pcidev {
	user: *void;
	addr: *byte;
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
}

scan_pci(base: *byte, visit: func(dev: *pcidev), user: *void) {
	var i: int;
	var dev: pcidev;
	i = 0;
	loop {
		if i == 256 {
			break;
		}

		dev.user = user;
		dev.addr = &base[i * 4096];
		dev.bus = i;

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

		visit(&dev);

		i = i + 1;
	}
}

clear_int(dev: *pcidev) {
	*dev.trap = 0xff:byte;
	_w16(&dev.addr[4], 0);
}

show_pcidev(dev: *pcidev) {
	sputh(dev.bus);
	sputc(':');
	sputc('\t');
	sputh(dev.vid);
	sputc(':');
	sputh(dev.did);
	sputc('\t');
	sputh(dev.cls);
	sputc(':');
	sputh(dev.subcls);
	sputc('\t');
	sputh(dev.bar0);
	sputc('\t');
	sputh(dev.bar1);
	sputc('\t');
	sputh(dev.bar2);
	sputc('\t');
	sputh(dev.bar3);
	sputc('\t');
	sputh(dev.bar4);
	sputc('\t');
	sputh(dev.bar5);
	sputc('\n');
}

map_bar(va: *byte, dev: *pcidev): *byte {
	var type: int;
	var pa: int;

	type = (dev.bar0 >> 1) & 3;
	pa = dev.bar0 & -16;

	// io bar
	if (dev.bar0 & 1) != 0 || type == 3 {
		return 0:*byte;
	}

	// 64 bit bar
	if type == 1 {
		pa = pa | (dev.bar1 << 32);
	}

	return map_pci(va, pa);
}

enum {
	E1_TXDESCLO = 0x3800,
	E1_TXDESCHI = 0x3804,
	E1_TXDESCLEN = 0x3808,
	E1_TXDESCHEAD = 0x3810,
	E1_TXDESCTAIL = 0x3818,
	E1_TCTRL = 0x0400,
	E1_TIPG = 0x0410,

	E1_RXDESCLO = 0x2800,
	E1_RXDESCHI = 0x2804,
	E1_RXDESCLEN = 0x2808,
	E1_RXDESCHEAD = 0x2810,
	E1_RXDESCTAIL = 0x2818,
	E1_RCTRL = 0x0100,

	E1_IMASK = 0x00d0,
	E1_ICR = 0x00C0,
}

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

init_e1000(dev: *pcidev) {
	var flag: *int;
	var base: *byte;
	var mac: int;
	var i: int;
	var txring: *byte;

	flag = dev.user: *int;
	if *flag || dev.vid != 0x8086 || (
		dev.did != 0x100e
		&& dev.did != 0x1539
	) {
		return;
	}
	*flag = 1;
	*dev.trap = 10:byte;

	base = map_bar(ptov(-(1<<34)), dev);
	if !base {
		return;
	}

	// Enable dma
	_w16(&dev.addr[4], 6);

	// Disable interrupts
	_w32(&base[E1_IMASK], 0);

	// Clear MTA
	i = 0;
	loop {
		if i == 0x80 {
			break;
		}
		_w32(&base[0x5200 + 4 * i], 0);
		i = i + 1;
	}

	// Read mac
	mac = _r32(&base[0x5400]) + (_r32(&base[0x5404])) << 32;

	txring = map_pci((-(1<<35)):*byte, 1024*1024*3);

	i = 0;
	loop {
		if i == 16 {
			break;
		}
		bzero(&txring[16*32 + 16*i], 16);
		((&txring[16*32 + 16*i]):*int)[0] = 1024*1024*3 + 4096 * (i + 1);
		_w16(&txring[16*32 + 16*i + 8], 2048);
		i = i + 1;
	}

	// Create tx descriptors
	_w32(&base[E1_TXDESCLO], 1024*1024*3);
	_w32(&base[E1_TXDESCHI], 0);
	_w32(&base[E1_TXDESCLEN], 16*32);
	_w32(&base[E1_TXDESCHEAD], 0);
	_w32(&base[E1_TXDESCTAIL], 0);

	// Create rx descriptors
	_w32(&base[E1_RXDESCLO], 1024*1024*3+16*32);
	_w32(&base[E1_RXDESCHI], 0);
	_w32(&base[E1_RXDESCLEN], 16*32);
	_w32(&base[E1_RXDESCHEAD], 0);
	_w32(&base[E1_RXDESCTAIL], 31);

	// Enable interrupts
	_w32(&base[E1_IMASK], 0);

	// Enable rx and tx
	_w32(&base[E1_TCTRL], 0x3003f0fa);
	_w32(&base[E1_RCTRL],
		// Enable
		(1 << 1)
		// StoreBadPackets
		+ (1 << 2)
		// UnicastPromiscuous
		+ (1 << 3)
		// MulticastPromiscuous
		+ (1 << 4)
		// LoobackMode
		+ (0 << 6)
		// BroadcastAccept
		+ (1 << 15)
		// ReceiveBufferSize
		+ (0 << 16)
		// StripEthernetCRC
		+ (1 << 26));

	// Send a packet
	(txring:*int)[0] = 1024*1024*3 + 16*32*2;
	_w16(&txring[8], 60);
	txring[10] = 0:byte;
	// ReportStatus + InsertFrameCheckSequence + EndOfPacket
	txring[11] = ((0 << 3) + (1 << 1) + 1):byte;
	txring[12] = 0:byte;
	txring[13] = 0:byte;
	txring[14] = 0:byte;
	txring[15] = 0:byte;
	txring[16*32*2 + 0] = (mac >> 40):byte;
	txring[16*32*2 + 1] = (mac >> 32):byte;
	txring[16*32*2 + 2] = (mac >> 24):byte;
	txring[16*32*2 + 3] = (mac >> 16):byte;
	txring[16*32*2 + 4] = (mac >> 8):byte;
	txring[16*32*2 + 5] = mac:byte;
	txring[16*32*2 + 6] = 0x36:byte;
	txring[16*32*2 + 7] = 0xb9:byte;
	txring[16*32*2 + 8] = 0x55:byte;
	txring[16*32*2 + 9] = 0x54:byte;
	txring[16*32*2 + 10] = 0xba:byte;
	txring[16*32*2 + 11] = 0xcf:byte;
	txring[16*32*2 + 12] = 0x08:byte;
	txring[16*32*2 + 13] = 0x00:byte;

	txring[16*32*2 + 14] = 0x45:byte;
	txring[16*32*2 + 15] = 0:byte;
	txring[16*32*2 + 16] = 0:byte;
	txring[16*32*2 + 17] = 28:byte;
	txring[16*32*2 + 18] = 0:byte;
	txring[16*32*2 + 19] = 0:byte;
	txring[16*32*2 + 20] = 0:byte;
	txring[16*32*2 + 21] = 0:byte;
	txring[16*32*2 + 22] = 64:byte;
	txring[16*32*2 + 23] = 1:byte;
	txring[16*32*2 + 24] = 0:byte;
	txring[16*32*2 + 25] = 0:byte;
	txring[16*32*2 + 26] = 192:byte;
	txring[16*32*2 + 27] = 168:byte;
	txring[16*32*2 + 28] = 32:byte;
	txring[16*32*2 + 29] = 2:byte;
	txring[16*32*2 + 30] = 192:byte;
	txring[16*32*2 + 31] = 168:byte;
	txring[16*32*2 + 32] = 32:byte;
	txring[16*32*2 + 33] = 1:byte;

	ip_checksum(&txring[16*32*2 + 14]);

	txring[16*32*2 + 34] = 8:byte;
	txring[16*32*2 + 35] = 0:byte;
	txring[16*32*2 + 36] = 0:byte;
	txring[16*32*2 + 37] = 0:byte;
	txring[16*32*2 + 38] = 0:byte;
	txring[16*32*2 + 39] = 0:byte;
	txring[16*32*2 + 40] = 0:byte;

	icmp_checksum(&txring[16*32*2 + 34]);

	_w32(&base[E1_TXDESCTAIL], 1);
	_w32(&base[E1_IMASK], (1 << 0) + (1 << 4));
}

init_nvme(dev: *pcidev) {
	var flag: *int;
	var base: *byte;

	flag = dev.user: *int;
	if *flag || dev.cls != 1 || dev.subcls != 8 {
		return;
	}
	*flag = 1;
	*dev.trap = 11:byte;

	// Enable dma
	_w16(&dev.addr[4], 6);

	// Map bar
	base = map_bar(ptov(-(1<<36)), dev);
	if !base {
		return;
	}

	sputs("nvme\n");

	// setup admin ring
	// enable interrupts


}

_kstart(mb: int) {
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
	var vga: *byte;

	vga = ptov(0xB8000);

	vga[0] = 'a':byte;
	vga[1] = 0x0e:byte;

	//setup_serial();

	brk = ptov(1024 * 512 * 3):int;

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

	// 100 hz pit
	outb(IO_PIT + 3, 0x36);
	outb(IO_PIT + 0, 0x9b);
	outb(IO_PIT + 0, 0x2e);

	// unmask pit
	outb(IO_PIC1 + 1, 0xfa);
	outb(IO_PIC2 + 1, 0xf3);

	show_addr(ptov(mb + 44):*int[0]);

        // Find ACPI tables
	//xsdt = find_xsdt();

        //mcfg = find_acpi(xsdt, "MCFG");

	//pcip = (ptov(mcfg + 44):*int)[0];
	//pci = map_pci(ptov(-(1<<31)), pcip);

	//scan_pci(pci, show_pcidev, 0:*void);
	//scan_pci(pci, clear_int, 0:*void);
	//flag = 0;
	//scan_pci(pci, init_e1000, (&flag):*void);
	//flag = 0;
	//scan_pci(pci, init_nvme, (&flag):*void);

	// Wait for interrupts
	loop {
		sti();
		hlt();
	}
}
