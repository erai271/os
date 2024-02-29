syscall(n: int, a1: int, a2: int, a3: int, a4: int, a5: int, a6: int): int;

struct compiler {
	entry: int;
	text_size: int;
}

enum { A, B, C, }

exit(n: int): void {
	syscall(60, n, 0, 0, 0, 0, 0);
}

read(fd: int, buf: *byte, n: int): int {
	return syscall(0, fd, buf: int, n, 0, 0, 0);
}

write(fd: int, buf: *byte, n: int): int {
	return syscall(1, fd, buf: int, n, 0, 0, 0);
}

mmap(addr: int, len: int, prot: int, flags: int, fd: int, off: int): int {
	return syscall(9, addr, len, prot, flags, fd, off);
}

getchar(): int {
	var b: byte;
	var ret: int;
	ret = read(0, &b, 1);
	if (ret < 0) {
		exit(3);
	}
	if (ret == 0) {
		return -1;
	}
	return b: int;
}

putchar(ch: int): void {
	var b: byte;
	var ret: int;
	b = ch: byte;
	ret = write(1, &b, 1);
	if (ret != 1) {
		exit(3);
	}
}

eputchar(ch: int): void {
	var b: byte;
	var ret: int;
	b = ch: byte;
	ret = write(2, &b, 1);
	if (ret != 1) {
		exit(3);
	}
}

writeout(c: *compiler): void {
	// magic
	putchar(0x7f);
	putchar('E');
	putchar('L');
	putchar('F');

	// class
	putchar(2);

	// endian
	putchar(1);

	// version
	putchar(1);

	// abi
	putchar(0);

	// abi version
	putchar(0);

	// padding
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// type
	putchar(2);
	putchar(0);

	// machine
	putchar(62);
	putchar(0);

	// version
	putchar(1);
	putchar(0);
	putchar(0);
	putchar(0);

	// entry point
	putchar(c.entry);
	putchar(c.entry >> 8);
	putchar(c.entry >> 16);
	putchar(c.entry >> 24);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phoff
	putchar(64);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// shoff
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// flags
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// ehsize
	putchar(64);
	putchar(0);

	// phentsize
	putchar(56);
	putchar(0);

	// phnum
	putchar(1);
	putchar(0);

	// shentsize
	putchar(64);
	putchar(0);

	// shnum
	putchar(0);
	putchar(0);

	// shstrndx
	putchar(0);
	putchar(0);

	// phdr[0].type
	putchar(1);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].flags
	putchar(5);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].offset
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].vaddr
	putchar(0);
	putchar(0);
	putchar(0x10);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].paddr
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].filesize
	putchar(c.text_size);
	putchar(c.text_size >> 8);
	putchar(c.text_size >> 16);
	putchar(c.text_size >> 24);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].memsize
	putchar(c.text_size);
	putchar(c.text_size >> 8);
	putchar(c.text_size >> 16);
	putchar(c.text_size >> 24);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].align
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// nop sled
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
	putchar(0x90);
}

main(): void {
	var c: compiler;

	//setup();

	//program();

	//typecheck();

	//translate();

	//assemble();

	//writeout();

	c.entry = 0x10161d;
	c.text_size = 0x1662;

	writeout(&c);
}
