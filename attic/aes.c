// https://doi.org/10.6028/NIST.FIPS.197-upd1

aes_expand(w: *byte, key: *byte) {
	var i: int;
	var j: int;
	var aes_sb: *byte;
	var aes_rc: *byte;

	aes_sb = "c|w{\xf2ko\xc50\x01g+\xfe\xd7\xabv\xca\x82\xc9}\xfaYG\xf0\xad\xd4\xa2\xaf\x9c\xa4r\xc0\xb7\xfd\x93&6?\xf7\xcc4\xa5\xe5\xf1q\xd81\x15\x04\xc7#\xc3\x18\x96\x05\x9a\x07\x12\x80\xe2\xeb'\xb2u\t\x83,\x1a\x1bnZ\xa0R;\xd6\xb3)\xe3/\x84S\xd1\x00\xed \xfc\xb1[j\xcb\xbe9JLX\xcf\xd0\xef\xaa\xfbCM3\x85E\xf9\x02\x7fP<\x9f\xa8Q\xa3@\x8f\x92\x9d8\xf5\xbc\xb6\xda!\x10\xff\xf3\xd2\xcd\x0c\x13\xec_\x97D\x17\xc4\xa7~=d]\x19s`\x81O\xdc\"*\x90\x88F\xee\xb8\x14\xde^\x0b\xdb\xe02:\nI\x06$\\\xc2\xd3\xacb\x91\x95\xe4y\xe7\xc87m\x8d\xd5N\xa9lV\xf4\xeaez\xae\x08\xbax%.\x1c\xa6\xb4\xc6\xe8\xddt\x1fK\xbd\x8b\x8ap>\xb5fH\x03\xf6\x0ea5W\xb9\x86\xc1\x1d\x9e\xe1\xf8\x98\x11i\xd9\x8e\x94\x9b\x1e\x87\xe9\xceU(\xdf\x8c\xa1\x89\r\xbf\xe6BhA\x99-\x0f\xb0T\xbb\x16";
	aes_rc = "\x01\x02\x04\x08\x10 @\x80\x1b6";

	i = 0;
	loop {
		if i == 16 {
			break;
		}

		w[i] = key[i];

		i = i + 1;
	}

	j = 0;
	loop {
		if j == 10 {
			break;
		}

		w[i] = w[i - 16] ^ aes_sb[w[i - 3]] ^ aes_rc[j]; i = i + 1;
		w[i] = w[i - 16] ^ aes_sb[w[i - 3]]; i = i + 1;
		w[i] = w[i - 16] ^ aes_sb[w[i - 3]]; i = i + 1;
		w[i] = w[i - 16] ^ aes_sb[w[i - 7]]; i = i + 1;

		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;

		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;

		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;
		w[i] = w[i - 16] ^ w[i - 4]; i = i + 1;

		j = j + 1;
	}
}

aes_two(x: byte): byte {
	var a: int;
	a = x:int;
	a = (a << 1) ^ ((a >> 7) * 0x1b);
	return a:byte;
}

struct _aes {
	a: int;
	b: int;
}

aes_cipher(cipher: *byte, plain: *byte, w: *byte) {
	var tmp: _aes;
	var s: *byte;
	var t: byte;
	var abcd: byte;
	var i: int;
	var j: int;
	var aes_sb: *byte;

	s = (&tmp):*byte;

	aes_sb = "c|w{\xf2ko\xc50\x01g+\xfe\xd7\xabv\xca\x82\xc9}\xfaYG\xf0\xad\xd4\xa2\xaf\x9c\xa4r\xc0\xb7\xfd\x93&6?\xf7\xcc4\xa5\xe5\xf1q\xd81\x15\x04\xc7#\xc3\x18\x96\x05\x9a\x07\x12\x80\xe2\xeb'\xb2u\t\x83,\x1a\x1bnZ\xa0R;\xd6\xb3)\xe3/\x84S\xd1\x00\xed \xfc\xb1[j\xcb\xbe9JLX\xcf\xd0\xef\xaa\xfbCM3\x85E\xf9\x02\x7fP<\x9f\xa8Q\xa3@\x8f\x92\x9d8\xf5\xbc\xb6\xda!\x10\xff\xf3\xd2\xcd\x0c\x13\xec_\x97D\x17\xc4\xa7~=d]\x19s`\x81O\xdc\"*\x90\x88F\xee\xb8\x14\xde^\x0b\xdb\xe02:\nI\x06$\\\xc2\xd3\xacb\x91\x95\xe4y\xe7\xc87m\x8d\xd5N\xa9lV\xf4\xeaez\xae\x08\xbax%.\x1c\xa6\xb4\xc6\xe8\xddt\x1fK\xbd\x8b\x8ap>\xb5fH\x03\xf6\x0ea5W\xb9\x86\xc1\x1d\x9e\xe1\xf8\x98\x11i\xd9\x8e\x94\x9b\x1e\x87\xe9\xceU(\xdf\x8c\xa1\x89\r\xbf\xe6BhA\x99-\x0f\xb0T\xbb\x16";

	i = 0;
	loop {
		if i == 16 {
			break;
		}

		s[i] = plain[i] ^ w[i];

		i = i + 1;
	}

	j = 1;
	loop {
		if j == 10 {
			break;
		}

		i = 0;
		loop {
			if i == 16 {
				break;
			}

			s[i] = aes_sb[s[i]];

			i = i + 1;
		}

		t=s[1]; s[1]=s[5];  s[5]=s[9];        s[9]=s[13];s[13]=t;
		t=s[2]; s[2]=s[10]; s[10]=t;   t=s[6];s[6]=s[14];s[14]=t;
		t=s[15];s[15]=s[11];s[11]=s[7];       s[7]=s[3]; s[3]=t;

		i = 0;
		loop {
			if i == 16 {
				break;
			}

			t = s[i]; abcd = s[i] ^ s[i + 1] ^ s[i + 2] ^ s[i + 3];
			s[i] = s[i] ^ abcd ^ aes_two(s[i] ^ s[i + 1]);
			s[i + 1] = s[i + 1] ^ abcd ^ aes_two(s[i + 1] ^ s[i + 2]);
			s[i + 2] = s[i + 2] ^ abcd ^ aes_two(s[i + 2] ^ s[i + 3]);
			s[i + 3] = s[i + 3] ^ abcd ^ aes_two(s[i + 3] ^ t);

			i = i + 4;
		}

		i = 0;
		loop {
			if i == 16 {
				break;
			}

			s[i] = s[i] ^ w[j * 16 + i];

			i = i + 1;
		}

		j = j + 1;
	}

	loop {
		if i == 16 {
			break;
		}

		s[i] = aes_sb[s[i]];

		i = i + 1;
	}

	t=s[1]; s[1]=s[5];  s[5]=s[9];        s[9]=s[13];s[13]=t;
	t=s[2]; s[2]=s[10]; s[10]=t;   t=s[6];s[6]=s[14];s[14]=t;
	t=s[15];s[15]=s[11];s[11]=s[7];       s[7]=s[3]; s[3]=t;

	loop {
		if i == 16 {
			break;
		}

		cipher[i] = s[i] ^ w[i + 10 * 16];

		i = i + 1;
	}
}
