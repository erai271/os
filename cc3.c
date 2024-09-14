main(argc: int, argv: **byte, envp: **byte) {
	var fd: int;
	var f: *file;
	var out: *file;
	var peg: *peg;
	var a: alloc;
	var len: int;
	var src: *byte;
	var node: *peg_node;
	setup_alloc(&a);

	fd = open(argv[1], 0, 0);
	if fd < 0 {
		die("open failed");
	}

	f = fopen(fd, &a);
	src = freadall(f, &len);
	fclose(f);

	peg = peg_new(src, len, &a);
	node = peg_parse(peg);
	peg_free(peg);

	out = fopen(1, &a);
	peg_show(out, node);
	fputc(out, '\n');

	fflush(out);
	fclose(out);
}
