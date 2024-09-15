struct lexer {
	a: *alloc;
	in: *file;
	nc: int;
	filename: *byte;
	lineno: int;
	colno: int;
	tt: int;
	token: *byte;
	tlen: int;
	tmax: int;
}

enum {
	T_EOF,
	T_IDENT,
	T_NUM,
	T_HEX,
	T_STR,
	T_CHAR,
	T_LPAR,
	T_RPAR,
	T_LBRA,
	T_RBRA,
	T_COMMA,
	T_SEMI,
	T_COLON,
	T_STAR,
	T_DOT,
	T_NOT,
	T_ASSIGN,
	T_AMP,
	T_OR,
	T_XOR,
	T_LT,
	T_GT,
	T_LE,
	T_GE,
	T_EQ,
	T_NE,
	T_ADD,
	T_SUB,
	T_LSH,
	T_RSH,
	T_BANG,
	T_BOR,
	T_BAND,
	T_LSQ,
	T_RSQ,
	T_DIV,
	T_MOD,
}

setup_lexer(a: *alloc): *lexer {
	var c: *lexer;

	c = alloc(a, sizeof(*c)):*lexer;

	c.a = a;

	c.in = 0: *file;
	c.nc = 0;
	c.filename = 0:*byte;
	c.lineno = 1;
	c.colno = 1;
	c.tlen = 0;
	c.tmax = 4096;
	c.token = alloc(c.a, c.tmax);
	c.tt = 0;

	return c;
}

lshow_context(c: *lexer) {
	fdputs(2, "on ");
	if (c.filename) {
		fdputs(2, c.filename);
	}
	fdputs(2, ":");
	fdputd(2, c.lineno);
	fdputs(2, ":");
	fdputd(2, c.colno);
	fdputs(2, "\n");
}

ldie(c: *lexer, msg: *byte) {
	lshow_context(c);
	fdputs(2, "cdie: ");
	fdputs(2, msg);
	fdputs(2, "\n");
	exit(1);
}

open_source(c: *lexer, filename: *byte) {
	var fd: int;

	c.filename = filename;
	c.nc = 0;
	c.lineno = 1;
	c.colno = 1;
	c.tlen = 0;
	c.tt = 0;

	fd = open(filename, 0, 0);
	if (fd < 0) {
		ldie(c, "failed to open file");
	}

	c.in = fopen(fd, c.a);
	c.nc = fgetc(c.in);

	feed(c);
}

close_source(c: *lexer) {
	if (c.in) {
		fclose(c.in);
	}
	c.in = 0: *file;
}

feedc(c: *lexer) {
	c.nc = fgetc(c.in);
	if (c.nc == '\n') {
		c.lineno = c.lineno + 1;
		c.colno = 0;
	}
	c.colno = c.colno + 1;
}

tappend(c: *lexer) {
	c.token[c.tlen] = c.nc:byte;
	c.tlen = c.tlen + 1;
	if (c.tlen == c.tmax) {
		ldie(c, "identifier too long");
	}
	c.token[c.tlen] = 0:byte;
	feedc(c);
}

feed(c: *lexer) {
	c.tlen = 0;
	c.token[0] = 0:byte;

	loop {
		if (c.nc == -1) {
			// Reached the end of input
			c.tt = T_EOF;
			return;
		} else if (c.nc == ' ' || c.nc == '\t' || c.nc == '\r') {
			// Whitespace
			feedc(c);
		} else if (c.nc == '\n') {
			// Line end
			feedc(c);
		} else if (c.nc == '/') {
			// Comment
			feedc(c);
			if (c.nc == '/') {
				// Read until the end of the comment
				loop {
					if (c.nc == '\n' || c.nc == -1) {
						break;
					}
					feedc(c);
				}
			} else {
				c.tt = T_DIV;
				return;
			}

		} else {
			// Start of a real token
			break;
		}
	}

	// Identifier
	if ((c.nc >= 'a' && c.nc <= 'z') || (c.nc >= 'A' && c.nc <= 'Z') || c.nc == '_') {
		feed_ident(c);
		return;
	}

	// String
	if (c.nc == '"') {
		feed_str(c);
		return;
	}

	// Character
	if (c.nc == '\'') {
		feed_char(c);
		return;
	}

	// Number
	if (c.nc >= '0' && c.nc <= '9') {
		feed_num(c);
		return;
	}

	// Single character tokens
	if (c.nc == '(') {
		c.tt = T_LPAR;
		feedc(c);
	} else if (c.nc == ')') {
		c.tt = T_RPAR;
		feedc(c);
	} else if (c.nc == '{') {
		c.tt = T_LBRA;
		feedc(c);
	} else if (c.nc == '}') {
		c.tt = T_RBRA;
		feedc(c);
	} else if (c.nc == ',') {
		c.tt = T_COMMA;
		feedc(c);
	} else if (c.nc == ';') {
		c.tt = T_SEMI;
		feedc(c);
	} else if (c.nc == ':') {
		c.tt = T_COLON;
		feedc(c);
	} else if (c.nc == '*') {
		c.tt = T_STAR;
		feedc(c);
	} else if (c.nc == '.') {
		c.tt = T_DOT;
		feedc(c);
	} else if (c.nc == '=') {
		c.tt = T_ASSIGN;
		feedc(c);
		if (c.nc == '=') {
			c.tt = T_EQ;
			feedc(c);
		}
	} else if (c.nc == '&') {
		c.tt = T_AMP;
		feedc(c);
		if (c.nc == '&') {
			c.tt = T_BAND;
			feedc(c);
		}
	} else if (c.nc == '~') {
		c.tt = T_NOT;
		feedc(c);
	} else if (c.nc == '|') {
		c.tt = T_OR;
		feedc(c);
		if (c.nc == '|') {
			c.tt = T_BOR;
			feedc(c);
		}
	} else if (c.nc == '^') {
		c.tt = T_XOR;
		feedc(c);
	} else if (c.nc == '!') {
		c.tt = T_BANG;
		feedc(c);
		if (c.nc == '=') {
			c.tt = T_NE;
			feedc(c);
		}
	} else if (c.nc == '<') {
		c.tt = T_LT;
		feedc(c);
		if (c.nc == '<') {
			c.tt = T_LSH;
			feedc(c);
		} else if (c.nc == '=') {
			c.tt = T_LE;
			feedc(c);
		}
	} else if (c.nc == '>') {
		c.tt = T_GT;
		feedc(c);
		if (c.nc == '>') {
			c.tt = T_RSH;
			feedc(c);
		} else if (c.nc == '=') {
			c.tt = T_GE;
			feedc(c);
		}
	} else if (c.nc == '[') {
		c.tt = T_LSQ;
		feedc(c);
	} else if (c.nc == ']') {
		c.tt = T_RSQ;
		feedc(c);
	} else if (c.nc == '+') {
		c.tt = T_ADD;
		feedc(c);
	} else if (c.nc == '-') {
		c.tt = T_SUB;
		feedc(c);
	} else if (c.nc == '%') {
		c.tt = T_MOD;
		feedc(c);
	} else {
		ldie(c, "invalid char");
	}
}

feed_ident(c: *lexer) {
	c.tt = T_IDENT;
	loop {
		if (!((c.nc >= 'a' && c.nc <= 'z') ||
				(c.nc >= 'A' && c.nc <= 'Z') ||
				(c.nc >= '0' && c.nc <= '9') ||
				c.nc == '_')) {
			break;
		}
		tappend(c);
	}
}

feed_escape(c: *lexer) {
	var hex: int;
	var ok: int;

	// backslash
	feedc(c);

	if (c.nc == 't') {
		c.nc = '\t';
	} else if (c.nc == 'r') {
		c.nc = '\r';
	} else if (c.nc == 'n') {
		c.nc = '\n';
	} else if (c.nc == 'x') {
		c.nc = fgetc(c.in);
		hex = hexdig(c.nc, &ok) * 16;
		if !ok {
			ldie(c, "invalid escape");
		}

		c.nc = fgetc(c.in);
		hex = hex + hexdig(c.nc, &ok);
		if !ok {
			ldie(c, "invalid escape");
		}

		c.nc = hex;
	} else if (c.nc != '\\' && c.nc != '\'' && c.nc != '"') {
		ldie(c, "invalid escape");
	}
}

feed_str(c: *lexer) {
	c.tt = T_STR;

	// quote
	feedc(c);

	loop {
		if (c.nc == '"') {
			feedc(c);
			return;
		}

		if (c.nc == -1 || c.nc == 0 || c.nc == '\n') {
			ldie(c, "invalid char in string");
		}

		if (c.tlen == c.tmax) {
			ldie(c, "string too long");
		}

		if (c.nc == '\\') {
			feed_escape(c);
		}

		tappend(c);
	}
}

feed_char(c: *lexer) {
	c.tt = T_CHAR;

	// quote
	feedc(c);

	if (c.nc == 0 || c.nc == -1 || c.nc == '\'' || c.nc == '\n') {
		ldie(c, "invalid char");
	}

	if (c.nc == '\\') {
		feed_escape(c);
	}

	tappend(c);

	if (c.nc != '\'') {
		ldie(c, "expected '");
	}

	feedc(c);
}

feed_hex(c: *lexer) {
	c.tt = T_HEX;

	loop {
		if (!((c.nc >= '0' && c.nc <= '9')
				|| (c.nc >= 'a' && c.nc <= 'f')
				|| (c.nc >= 'A' && c.nc <= 'F'))) {
			break;
		}

		tappend(c);
	}

	if (c.tlen == 0) {
		ldie(c, "expected hex");
	}
}

feed_num(c: *lexer) {
	c.tt = T_NUM;

	if (c.nc == '0') {
		tappend(c);

		if (c.nc == 'x') {
			feedc(c);
			feed_hex(c);
			return;
		}
	}

	loop {
		if (!(c.nc >= '0' && c.nc <= '9')) {
			break;
		}

		tappend(c);
	}
}
