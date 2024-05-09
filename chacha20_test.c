main(c:int,v:**byte,e:**byte) {
	var _key: _chacha20_state;
	var key: *byte;
	var _nonce: _chacha20_state;
	var nonce: *byte;
	var _block: _chacha20_state;
	var block: *byte;

	key = (&_key):*byte;
	nonce = (&_nonce):*byte;
	block = (&_block):*byte;

	bzero(key, sizeof(_key));
	bzero(nonce, sizeof(_nonce));

	chacha20_block(block, key, 0, nonce);
	fdputd(1, 1);
	fdputs(1, ":\n");
	fdxxd(1, block, 64);
	fdputc(1, '\n');

	chacha20_block(block, key, 1, nonce);
	fdputd(1, 2);
	fdputs(1, ":\n");
	fdxxd(1, block, 64);
	fdputc(1, '\n');

	key[31] = 1:byte;
	chacha20_block(block, key, 1, nonce);
	fdputd(1, 3);
	fdputs(1, ":\n");
	fdxxd(1, block, 64);
	fdputc(1, '\n');

	key[1] = 0xff:byte;
	key[31] = 0:byte;
	chacha20_block(block, key, 2, nonce);
	fdputd(1, 4);
	fdputs(1, ":\n");
	fdxxd(1, block, 64);
	fdputc(1, '\n');

	key[1] = 0:byte;
	nonce[11] = 2:byte;
	chacha20_block(block, key, 0, nonce);
	fdputd(1, 5);
	fdputs(1, ":\n");
	fdxxd(1, block, 64);
}
