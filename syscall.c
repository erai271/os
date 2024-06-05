syscall(n: int, a1: int, a2: int, a3: int, a4: int, a5: int, a6: int): int;

enum {
	O_RDONLY = 0,
	O_WRONLY = 1,
	O_RDWR = 2,
	O_CREAT = 64,
	O_DIRECTORY = 0x1000,

	EINTR = 4,

	AF_INET = 2,
	SOCK_STREAM = 1,

	POLLIN = 1,
	POLLOUT = 4,

	WNOHANG = 1,

	SIG_DFL = 0,
	SIG_IGN = 1,

	SIGINT = 2,
	SIGALRM = 14,
	SIGCHLD = 17,
	SIGWINCH = 28,
}

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

fstat(fd: int, buf: *byte): int {
	return syscall(5, fd, buf:int, 0, 0, 0, 0);
}

poll(pfd: *int, nfd: int, timeout: int): int {
	return syscall(7, pfd:int, nfd, timeout, 0, 0, 0);
}

mmap(addr: int, len: int, prot: int, flags: int, fd: int, off: int): int {
	return syscall(9, addr, len, prot, flags, fd, off);
}

struct sigaction {
	handler: int;
	flags: int;
	restorer: int;
	mask: int;
}

sigaction(sig: int, act: *sigaction, oact: *sigaction): int {
	return syscall(13, sig, act:int, oact:int, 8, 0, 0);
}

pipe(rfd: *int, wfd: *int): int {
	var buf: int;
	var ret: int;
	ret = syscall(22, (&buf):int, 0, 0, 0, 0, 0);
	if ret == 0 {
		*rfd = buf & (-1 >> 32);
		*wfd = buf >> 32;
	}
	return ret;
}

dup2(old: int, new: int): int {
	return syscall(33, old, new, 0, 0, 0, 0);
}

socket(pf: int, ty: int, pc: int): int {
	return syscall(41, pf, ty, pc, 0, 0, 0);
}

accept(fd: int, addr: *byte, len: *int): int {
	return syscall(43, fd, addr:int, len:int, 0, 0, 0);
}

bind(fd: int, addr: *byte, len: int): int {
	return syscall(49, fd, addr:int, len:int, 0, 0, 0);
}

listen(fd: int, backlog: int): int {
	return syscall(50, fd, backlog, 0, 0, 0, 0);
}

fork(): int {
	return syscall(57, 0, 0, 0, 0, 0, 0);
}

exec(cmd: *byte, argv: **byte, envp: **byte): int {
	return syscall(59, cmd:int, argv:int, envp:int, 0, 0, 0);
}

exit(n: int) {
	syscall(60, n, 0, 0, 0, 0, 0);
}

wait(pid: int, status: *int, flags: int): int {
	var s: int;
	var ret: int;
	s = 0;
	ret = syscall(61, pid, s:int, flags, 0, 0, 0);
	if status {
		*status = s & (-1 >> 32);
	}
	return ret;
}

rename(oldname: *byte, newname: *byte): int {
	return syscall(82, oldname: int, newname: int, 0, 0, 0, 0);
}

mkdir(name: *byte): int {
	return syscall(83, name: int, 0, 0, 0, 0, 0);
}

unlink(name: *byte): int {
	return syscall(87, name: int, 0, 0, 0, 0, 0);
}

getdirents(fd: int, buf: *byte, len: int): int {
	return syscall(217, fd, buf:int, len, 0, 0, 0);
}
