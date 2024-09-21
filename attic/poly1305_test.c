main(c: int, v: **byte, e: **byte) {
	var _key: _poly1305_ctx;
	var _text: _poly1305_ctx;
	var _mac: _poly1305_ctx;
	var key: *byte;
	var text: *byte;
	var mac: *byte;

	key = (&_key):*byte;
	text = (&_text):*byte;
	mac = (&_mac):*byte;

	bzero(key, 32);
	bzero(text, 64);
	bzero(mac, 32);

	poly1305(mac, key, text, 64);
	fdxxd(1, mac, 16);

	key[0] = 0x85:byte;
	key[1] = 0xd6:byte;
	key[2] = 0xbe:byte;
	key[3] = 0x78:byte;
	key[4] = 0x57:byte;
	key[5] = 0x55:byte;
	key[6] = 0x6d:byte;
	key[7] = 0x33:byte;
	key[8] = 0x7f:byte;
	key[9] = 0x44:byte;
	key[10] = 0x52:byte;
	key[11] = 0xfe:byte;
	key[12] = 0x42:byte;
	key[13] = 0xd5:byte;
	key[14] = 0x06:byte;
	key[15] = 0xa8:byte;
	key[16] = 0x01:byte;
	key[17] = 0x03:byte;
	key[18] = 0x80:byte;
	key[19] = 0x8a:byte;
	key[20] = 0xfb:byte;
	key[21] = 0x0d:byte;
	key[22] = 0xb2:byte;
	key[23] = 0xfd:byte;
	key[24] = 0x4a:byte;
	key[25] = 0xbf:byte;
	key[26] = 0xf6:byte;
	key[27] = 0xaf:byte;
	key[28] = 0x41:byte;
	key[29] = 0x49:byte;
	key[30] = 0xf5:byte;
	key[31] = 0x1b:byte;

	poly1305(mac, key, "Cryptographic Forum Research Group", 34);
	fdxxd(1, mac, 16);
}
