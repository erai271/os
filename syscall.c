syscall(n: int, a1: int, a2: int, a3: int, a4: int, a5: int, a6: int): int;

_start(argc: int, argv: **byte, envp: **byte) {
	main(argc, argv, envp);
	exit(0);
}

read(fd: int, buf: *byte, n: int): int {
	return syscall(0, fd, buf: int, n, 0, 0, 0);
}

write(fd: int, buf: *byte, n: int): int {
	return syscall(1, fd, buf: int, n, 0, 0, 0);
}

open(name: *byte, flags: int, mode: int): int {
	return syscall(2, name: int, flags, mode, 0, 0, 0);
}

close(fd: int): int {
	return syscall(3, fd, 0, 0, 0, 0, 0);
}

mmap(addr: int, len: int, prot: int, flags: int, fd: int, off: int): int {
	return syscall(9, addr, len, prot, flags, fd, off);
}

exit(n: int) {
	syscall(60, n, 0, 0, 0, 0, 0);
}

unlink(name: *byte): int {
	return syscall(87, name: int, 0, 0, 0, 0, 0);
}
