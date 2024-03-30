// in out iretq rdmsr wrmsr wrcr rdcr lgdt lldt lidt ltr hlt cli sti syscall sysret wbinvld invlpg

_start() {
}

ru32(p: *byte): int {
	return p[0]:int + (p[1]:int << 8) + (p[2]:int << 16) + (p[3]:int << 24);
}

_kstart(mb: *byte) {
	loop { }
}
