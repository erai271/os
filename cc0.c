#include <stdlib.h> // malloc
#include <stdio.h> // getchar, putchar, EOF

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations                                                      //
///////////////////////////////////////////////////////////////////////////////

int lineno;
struct node;
struct type;
struct label;
struct fix;

struct node *expr(void);
struct label *mklabel(void);
void showt(struct type *t);
void show(struct node *n);

///////////////////////////////////////////////////////////////////////////////
// Helpers                                                                   //
///////////////////////////////////////////////////////////////////////////////

void
die(char *msg)
{
	fprintf(stderr, "die(%d): %s\n", lineno, msg);
	exit(1);
}

// strlen-ish
int
slen(unsigned char *s)
{
	int i = 0;

	while (s[i]) {
		i += 1;
	}

	return i;
}

// strcmp-ish
int
cmp(unsigned char *a, unsigned char *b)
{
	int i;

	i = 0;
	while (1) {
		if (a[i] > b[i]) {
			return 1;
		}

		if (a[i] < b[i]) {
			return -1;
		}

		if (a[i] == 0) {
			return 0;
		}

		i += 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Lexer                                                                     //
///////////////////////////////////////////////////////////////////////////////

enum {
	T_EOF,
	T_IDENT,
	T_NUM,
	T_STR,
	T_LPAR,
	T_RPAR,
	T_LBRA,
	T_RBRA,
	T_COMMA,
	T_SEMI,
	T_COLON,
	T_STAR,
	T_INVALID
};

// The token from the source
unsigned char token[4096];

// Token type
int tt;

// Token length
int tlen;

// Single character lookahead
int nc;

// Line number of the start of the current token
int lineno;

// Read in an identifier
void
feed_ident(void)
{
	tt = T_IDENT;
	while ((nc >= 'a' && nc <= 'z') || (nc >= 'A' && nc <= 'Z') ||
			(nc >= '0' && nc <= '9') || nc == '_') {
		if (tlen == sizeof(token) - 1) {
			die("identifier too long");
		}
		token[tlen] = nc;
		tlen += 1;
		nc = getchar();
	}
	token[tlen] = 0;
}

// Parse a hex digit
int
hexdig(int c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	}

	if (c >= 'A' && c <= 'F') {
		return c - 'F' + 10;
	}

	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}

	die("invalid hex digit");
	return 0;
}

// Parse an escape and replace nc with the value
void
feed_escape(void)
{
	int hex;

	nc = getchar();
	switch (nc) {
	case '\\': case '\'': case '"':
		return;
	case 't':
		nc = '\t';
		break;
	case 'r':
		nc = '\r';
		break;
	case 'n':
		nc = '\n';
		break;
	case 'x':
		nc = getchar();
		hex = hexdig(nc) << 4;

		nc = getchar();
		hex += hexdig(nc);

		nc = hex;
		break;
	default:
		die("invalid escape");
	}
}

// Read in a string
void
feed_str(void)
{
	tt = T_STR;
	nc = getchar();
	while (nc != '"') {
		if (nc == EOF) {
			die("eof in string literal");
		}

		if (nc == 0 || nc == '\n') {
			die("invalid char in string");
		}

		if (tlen == sizeof(token) - 1) {
			die("string too long");
		}

		if (nc == '\\') {
			feed_escape();
		}

		token[tlen] = nc;
		tlen += 1;
		nc = getchar();
	}
	token[tlen] = 0;
	nc = getchar();
}

// Read in a number
void
feed_num(void)
{
	tt = T_NUM;
	while (nc >= '0' && nc <= '9') {
		if (tlen == sizeof(token) - 1) {
			die("identifier too long");
		}
		token[tlen] = nc;
		tlen += 1;
		nc = getchar();
	}
	token[tlen] = 0;
}

// Read in the next token
void
feed(void)
{
	tlen = 0;

	// Loop invariant: possible whitespace or EOF
	while (1) {
		// Reached the end of input
		if (nc == EOF) {
			tt = T_EOF;
			return;
		}

		// Whitespace
		if (nc == ' ' || nc == '\t' || nc == '\r') {
			nc = getchar();
			continue;
		}

		// Line end
		if (nc == '\n') {
			nc = getchar();
			lineno += 1;
			continue;
		}

		// Comment
		if (nc == '/') {
			nc = getchar();
			if (nc != '/') {
				// For now just die if we get a single slash
				die("invalid token /");
			}

			// Read until the end of the comment
			while (nc != '\n' && nc != EOF) {
				nc = getchar();
			}

			continue;
		}

		// Start of a real token
		break;
	}

	// Identifier
	if ((nc >= 'a' && nc <= 'z') || (nc >= 'A' && nc <= 'Z') || nc == '_') {
		feed_ident();
		return;
	}

	// String
	if (nc == '"') {
		feed_str();
		return;
	}

	// Number
	if (nc >= '0' && nc <= '9') {
		feed_num();
		return;
	}

	// Single character tokens
	switch (nc) {
	case '(':
		tt = T_LPAR;
		break;
	case ')':
		tt = T_RPAR;
		break;
	case '{':
		tt = T_LBRA;
		break;
	case '}':
		tt = T_RBRA;
		break;
	case ',':
		tt = T_COMMA;
		break;
	case ';':
		tt = T_SEMI;
		break;
	case ':':
		tt = T_COLON;
		break;
	case '*':
		tt = T_STAR;
		break;
	default:
		die("invalid char");
	}

	nc = getchar();
}

///////////////////////////////////////////////////////////////////////////////
// Grammar                                                                   //
///////////////////////////////////////////////////////////////////////////////

struct node {
	int kind;
	struct node *a, *b;
	struct type *t;
	unsigned int n;
	unsigned char *s;
	int lineno;
};

enum {
	N_IDENT,
	N_NUM,
	N_STR,
	N_STMTLIST,
	N_EXPRLIST,
	N_CALL,
	N_ARGLIST,
	N_FUNC,
	N_ARGDECL,
	N_FUNCDECL,
	N_PROGRAM,
	N_FUNCTYPE,
	N_PTRTYPE,
};

// Construct a node
struct node *
mknode(int kind, struct node *a, struct node *b)
{
	struct node *n;

	n = malloc(sizeof(*n));
	if (!n) {
		die("out of memory");
	}

	n->kind = kind;
	n->a = a;
	n->b = b;
	n->t = 0;
	n->n = 0;
	n->s = 0;
	n->lineno = lineno;

	return n;
}

// Copy the current token
unsigned char *
intern(void)
{
	unsigned char *ret;
	int i;

	ret = malloc(tlen + 1);
	if (!ret) {
		return 0;
	}

	for (i = 0; i < tlen; i += 1) {
		ret[i] = token[i];
	}
	ret[i] = 0;

	return ret;
}

// ident := IDENT
struct node *
ident(void)
{
	struct node *n;

	if (tt != T_IDENT) {
		return 0;
	}

	n = mknode(N_IDENT, 0, 0);
	n->s = intern();
	feed();

	return n;
}

// num := NUM
struct node *
num(void)
{
	struct node *n;
	unsigned int x;
	int d;
	int i;

	if (tt != T_NUM) {
		return 0;
	}

	x = 0;
	for (i = 0; i < tlen; i += 1) {
		d = token[i] - '0';

		if (x > ((unsigned int)-1) / 10) {
			die("overflow");
		}

		x *= 10;

		if (x > ((unsigned int)-1) - d) {
			die("overflow");
		}

		x += d;
	}

	n = mknode(N_NUM, 0, 0);
	n->n = x;
	feed();

	return n;
}

// str := STR
struct node *
str(void)
{
	struct node *n;

	if (tt != T_STR) {
		return 0;
	}

	n = mknode(N_STR, 0, 0);
	n->s = intern();
	feed();

	return n;
}

// primary := ident
//          | num
//          | str
//          | '(' expr ')'
struct node *
primary(void)
{
	struct node *n;

	n = ident();
	if (n) {
		return n;
	}

	n = num();
	if (n) {
		return n;
	}

	n = str();
	if (n) {
		return n;
	}

	if (tt == T_LPAR) {
		feed();

		n = expr();
		if (!n) {
			die("expected expr");
		}

		if (tt != T_RPAR) {
			die("expected )");
		}
		feed();

		return n;
	}

	return 0;
}

// expr_list := expr
//            | expr ',' expr_list
struct node *
expr_list(void)
{
	struct node *n, *e, *a;

	a = expr();
	if (!a) {
		return 0;
	}

	e = mknode(N_EXPRLIST, a, 0);
	n = e;

	while (1) {
		if (tt != T_COMMA) {
			return n;
		}
		feed();

		a = expr();
		if (!a) {
			die("expected expr");
		}

		e->b = mknode(N_EXPRLIST, a, 0);
		e = e->b;
	}
}

// post_expr := primary
//            | post_expr '(' expr_list ')'
struct node *
post_expr(void)
{
	struct node *n, *b;

	n = primary();
	if (!n) {
		return 0;
	}

	while (1) {
		if (tt == T_LPAR) {
			feed();

			b = expr_list();

			if (tt != T_RPAR) {
				die("expected )");
			}
			feed();

			n  = mknode(N_CALL, n, b);
			continue;
		}

		return n;
	}
}

// expr := post_expr
struct node *
expr(void)
{
	return post_expr();
}

// stmt := expr ';'
struct node *
stmt(void)
{
	struct node *n;

	n = expr();
	if (!n) {
		return 0;
	}

	if (tt != T_SEMI) {
		die("expected ;");
	}
	feed();

	return n;
}

// stmt_list := stmt
//            | stmt stmt_list
struct node *
stmt_list(void)
{
	struct node *n, *e, *a;

	a = stmt();
	if (!a) {
		return 0;
	}

	e = mknode(N_STMTLIST, a, 0);
	n = e;

	while (1) {
		a = stmt();
		if (!a) {
			return n;
		}

		e->b = mknode(N_STMTLIST, a, 0);
		e = e->b;
	}
}

// type := ident
//       | '*' type
//       | '(' type ')'
struct node *
type(void)
{
	struct node *n;

	if (tt == T_STAR) {
		feed();

		n = type();

		return mknode(N_PTRTYPE, n, 0);
	}

	if (tt == T_LPAR) {
		feed();

		n = type();

		if (tt != T_RPAR) {
			die("expected )");
		}
		feed();

		return n;
	}

	return ident();
}

// arg_decl := ':' type
//             ident ':' type
struct node *
arg_decl(void)
{
	struct node *n, *a, *b;

	a = ident();

	if (tt != T_COLON) {
		if (a) {
			die("expected :");
		}

		return 0;
	}
	feed();

	b = type();
	if (!b) {
		die("expected type");
	}

	return mknode(N_ARGDECL, a, b);
}

// arg_list := arg_decl
//           | arg_decl ',' arg_list
struct node *
arg_list(void)
{
	struct node *n, *e, *a;

	a = arg_decl();
	if (!a) {
		return 0;
	}

	e = mknode(N_ARGLIST, a, 0);
	n = e;

	while (1) {
		if (tt != T_COMMA) {
			return n;
		}
		feed();

		a = arg_decl();
		if (!a) {
			die("expected identifier");
		}

		e->b = mknode(N_ARGLIST, a, 0);
		e = e->b;
	}
}

// func_decl := ident '(' ')' ':' type
//            | ident '(' arg_list ')' ':' type
struct node *
func_decl(void)
{
	struct node *a, *b, *c;

	a = ident();
	if (!a) {
		return 0;
	}

	if (tt != T_LPAR) {
		die("expected (");
	}
	feed();

	b = arg_list();

	if (tt != T_RPAR) {
		die("expected )");
	}
	feed();

	if (tt != T_COLON) {
		die("expected :");
	}
	feed();

	c = type();
	if (!c) {
		die("expected type");
	}

	return mknode(N_FUNCDECL, a, mknode(N_FUNCTYPE, b, c));
}


// func := func_decl '{' stmt_list '}'
//       | func_decl ';'
struct node *
func(void)
{
	struct node *n, *a, *b;

	a = func_decl();
	if (!a) {
		return 0;
	}

	if (tt == T_SEMI) {
		feed();
		return a;
	}

	if (tt != T_LBRA) {
		die("expected {");
	}
	feed();

	b = stmt_list();

	if (tt != T_RBRA) {
		die("expected }");
	}
	feed();

	return mknode(N_FUNC, a, b);
}

// program := func
//          | func program
struct node *
program(void)
{
	struct node *n, *e, *f;

	f = func();
	if (!f) {
		return 0;
	}

	e = mknode(N_PROGRAM, f, 0);
	n = e;

	while (1) {
		f = func();
		if (!f) {
			if (tt != T_EOF) {
				die("expected eof");
			}

			return n;
		}

		e->b = mknode(N_PROGRAM, f, 0);
		e = e->b;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Setup                                                                     //
///////////////////////////////////////////////////////////////////////////////

// Prime the parser by reading in the lookahead
void
setup(void)
{
	lineno = 1;
	nc = getchar();
	feed();
}

///////////////////////////////////////////////////////////////////////////////
// Type System                                                               //
///////////////////////////////////////////////////////////////////////////////

enum {
	T_VOID,
	T_INT,
	T_BYTE,
	T_PTR,
	T_ARG,
	T_FUNC,
};

struct type {
	int kind;
	struct type *val, *arg;
};

// Unify two types
void
unify(struct type *a, struct type *b)
{
	if (!a || !b || a == b) {
		return;
	}

	if (a->kind != b->kind) {
		die("type error");
	}

	switch (a->kind) {
	case T_VOID: case T_INT: case T_BYTE:
		break;
	case T_PTR:
		unify(a->val, b->val);
		break;
	case T_FUNC:
		unify(a->val, b->val);
		if ((!!a->arg) != (!!b->arg)) {
			die("arg mismatch");
		}
		unify(a->arg, b->arg);
		break;
	default:
		die("invalid type");
	}
}

struct type *
mktype(int kind, struct type *val, struct type *arg)
{
	struct type *t;

	t = malloc(sizeof(*t));
	if (!t) {
		die("out of memory");
	}

	t->kind = kind;
	t->val = val;
	t->arg = arg;

	return t;
}

// Convert a type node into a type
struct type *
prototype(struct node *n)
{
	if (!n) {
		return 0;
	}

	switch (n->kind) {
	case N_IDENT:
		if (!cmp(n->s, (unsigned char *)"void")) {
			return mktype(T_VOID, 0, 0);
		}
		if (!cmp(n->s, (unsigned char *)"int")) {
			return mktype(T_INT, 0, 0);
		}
		if (!cmp(n->s, (unsigned char *)"byte")) {
			return mktype(T_BYTE, 0, 0);
		}
		die("invalid primitive");
		return 0;
	case N_ARGLIST:
		return mktype(T_ARG, prototype(n->a->b), prototype(n->b));
	case N_FUNCTYPE:
		return mktype(T_FUNC, prototype(n->b), prototype(n->a));
	case N_PTRTYPE:
		return mktype(T_PTR, prototype(n->a), 0);
	default:
		die("invalid type");
		return 0;
	}
}

struct decl {
	unsigned char *name;
	struct type *ftype;
	struct node *body;
	struct decl *p, *l, *r;
	struct decl *next;
	struct label *label;
	int defined;
	int lineno;
};

struct decl *decls;

// Find a function declaration by name
struct decl *
find(unsigned char *name)
{
	struct decl **link, *d, *p;
	int c;

	p = 0;
	link = &decls;
	while (1) {
		d = *link;
		if (!d) {
			break;
		}

		c = cmp(name, d->name);

		if (c < 0) {
			p = d;
			link = &d->l;
			continue;
		}

		if (c > 0) {
			p = d;
			link = &d->r;
			continue;
		}

		return d;
	}

	d = malloc(sizeof(*d));
	if (!d) {
		die("out of memory");
	}

	d->name = name;
	d->ftype = 0;
	d->body = 0;
	d->p = p;
	d->l = 0;
	d->r = 0;
	d->label = mklabel();
	d->defined = 0;

	*link = d;

	return d;
}

// Evaluate function declaration
struct decl *
declare(struct node *n)
{
	struct decl *d;
	struct type *t;
	d = find(n->a->s);
	t = prototype(n->b);
	if (d->ftype) {
		unify(d->ftype, t);
	} else {
		d->ftype = t;
		d->lineno = n->lineno;
	}
	return d;
}

// Function definition
void
define(struct node *n)
{
	struct decl *d;

	d = declare(n->a);

	if (d->defined) {
		die("duplicate declaration");
	}

	d->body = n->b;
	d->defined = 1;
	d->lineno = n->lineno;
}

void
typeexpr(struct node *n)
{
	struct node *a;
	struct type *t;
	struct decl *d;

	switch (n->kind) {
	case N_STR:
		n->t = mktype(T_PTR, mktype(T_BYTE, 0, 0), 0);
		break;
	case N_NUM:
		n->t = mktype(T_INT, 0, 0);
		break;
	case N_CALL:
		typeexpr(n->a);
		t = n->a->t;
		if (t->kind != T_FUNC) {
			die("calling not a function");
		}
		for (a = n->b, t = t->arg; a && t; a = a->b, t = t->arg) {
			typeexpr(a->a);
			unify(a->a->t, t->val);
		}
		if (a || t) {
			die("wrong number of arguments");
		}
		n->t = n->a->t->val;
		break;
	case N_IDENT:
		d = find(n->s);
		if (!d->ftype) {
			die("no such function");
		}
		n->t = d->ftype;
		break;
	default:
		die("not an expression");
	}
}

// Type a statement
void
typestmt(struct node *n)
{
	switch (n->kind) {
	case N_STR: case N_NUM: case N_CALL:
		typeexpr(n);
		break;
	default:
		die("not a statement");
	}
}

// Type a function body
void
typebody(struct decl *d)
{
	struct node *n;

	for (n = d->body; n && n->a; n = n->b) {
		typestmt(n->a);
	}
}

// Find the first decl in a subtree
struct decl *
mindecl(struct decl *d)
{
	while (d && d->l) {
		d = d->l;
	}
	return d;
}

// Find the next declaration
struct decl *
nextdecl(struct decl *d)
{
	if (!d) {
		return 0;
	}

	if (d->r) {
		return mindecl(d->r);
	}

	for (; d->p; d = d->p) {
		if (d->p->l == d) {
			return d->p;
		}
	}

	return 0;
}

// Find the first declaration
struct decl *
firstdecl(void)
{
	return mindecl(decls);
}

// Evaluate declarations
void
typecheck(struct node *p)
{
	struct decl *d;
	struct node *n;

	// Process declarations
	for (n = p; n; n = n->b) {
		switch (n->a->kind) {
		case N_FUNCDECL:
			declare(n->a);
			break;
		case N_FUNC:
			define(n->a);
			break;
		default:
			die("invalid decl");
		}
	}

	// Check types
	for (d = firstdecl(); d; d = nextdecl(d)) {
		typebody(d);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Assembler                                                                 //
///////////////////////////////////////////////////////////////////////////////

struct label {
	struct label *next;
	struct fix *fix;
	int at;
	int fixed;
};

struct fix {
	struct fix *next;
	unsigned char *ptr;
	int at;
};

struct buf {
	unsigned char *buf;
	int fill, cap;
	struct buf *next;
};

int at;
struct label *labels;
struct buf *text, *text_end;

// Create a new label
struct label *
mklabel(void)
{
	struct label *l;

	l = malloc(sizeof(*l));
	if (!l) {
		die("out of memory");
	}

	l->next = labels;
	l->fix = 0;
	l->at = 0;
	l->fixed = 0;

	labels = l;

	return l;
}

// Reserve size in the output buffer
void
reserve(int n)
{
	unsigned char *m;
	struct buf *b;

	if (text_end && text_end->cap - text_end->fill >= n) {
		return;
	}

	if (n < 4096) {
		n = 4096;
	}

	m = malloc(n);
	if (!m) {
		die("out of memory");
	}

	b = malloc(sizeof(*b));
	if (!b) {
		die("out of memory");
	}

	b->buf = m;
	b->fill = 0;
	b->cap = n;
	b->next = 0;

	if (text_end) {
		text_end->next = b;
		text_end = b;
	} else {
		text = b;
		text_end = b;
	}
}

// Add a single byte to the output
void
emit(int c)
{
	reserve(1);
	text->buf[text->fill] = c;
	text->fill += 1;
	at += 1;
}

// Fix a single reference
void
fixup(unsigned char *here, int delta)
{
	here[0] = delta;
	here[1] = delta >> 8;
	here[2] = delta >> 16;
	here[3] = delta >> 24;
}

// Add an new fixup for the current position
void
addfixup(struct label *l)
{
	struct fix *f;
	unsigned char *here;

	reserve(4);
	here = &text_end->buf[text_end->fill];

	emit(0);
	emit(0);
	emit(0);
	emit(0);

	if (l->fixed) {
		fixup(here, l->at - at);
	} else {
		f = malloc(sizeof(*f));
		if (!f) {
			die("out of memory");
		}

		f->next = l->fix;
		f->ptr = here;
		f->at = at;

		l->fix = f;
	}
}

// Fix references to a label to the current position
void
fixup_label(struct label *l)
{
	struct fix *f;

	if (l->fixed) {
		die("already fixed");
	}

	l->at = at;
	l->fixed = 1;

	for (f = l->fix; f; f = f->next) {
		fixup(f->ptr, l->at - f->at);
	}
}

void
emit_ptr(struct label *l)
{
	// lea %rax, [l]
	emit(0x48);
	emit(0x8d);
	emit(0x05);
	addfixup(l);
	// push %rax
	emit(0x50);
}

void
emit_jmp(struct label *l)
{
	// jmp l
	emit(0xe9);
	addfixup(l);
}

void
emit_num(unsigned int x)
{
	// push x
	emit(0x68);
	emit(x);
	emit(x >> 8);
	emit(x >> 16);
	emit(x >> 24);
}

void
emit_str(unsigned char *s)
{
	struct label *a, *b;
	int i;

	a = mklabel();
	b = mklabel();

	// jmp b
	emit_jmp(b);

	// a:
	fixup_label(a);
	// .string s
	for (; *s; s += 1) {
		emit(*s);
	}
	emit(0);

	// nop sled
	for (i = 0; i < 8; i++) {
		emit(0x90);
	}

	// b:
	fixup_label(b);
	// push a
	emit_ptr(a);
}

void
emit_pop(int n)
{
	n *= 8;
	// add rsp, 8*n
	emit(0x48);
	emit(0x81);
	emit(0xc4);
	emit(n);
	emit(n >> 8);
	emit(n >> 16);
	emit(n >> 24);
}

void
emit_ret(void)
{
	// pop rax
	emit(0x58);
	// ret
	emit(0xc3);
}

void
emit_call(int n)
{
	// pop rax
	emit(0x58);
	// call rax
	emit(0xff);
	emit(0xd0);
	// add rsp, 8*(n+1)
	emit_pop(n);
	// push rax
	emit(0x50);
}

// Write the output
void
writeout(void)
{
	int i;
	int load_addr;
	int entry;
	struct decl *d;
	struct buf *b;

	load_addr = 0x100000;

	d = find((unsigned char *)"_start");
	if (!d->defined || !d->label->fixed) {
		die("no _start function");
	}

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

	entry = load_addr + d->label->at + 128;

	// entry point
	putchar(entry);
	putchar(entry >> 8);
	putchar(entry >> 16);
	putchar(entry >> 24);
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
	putchar(load_addr);
	putchar(load_addr >> 8);
	putchar(load_addr >> 16);
	putchar(load_addr >> 24);
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

	at += 128;

	// phdr[0].filesize
	putchar(at);
	putchar(at >> 8);
	putchar(at >> 16);
	putchar(at >> 24);
	putchar(0);
	putchar(0);
	putchar(0);
	putchar(0);

	// phdr[0].memsize
	putchar(at);
	putchar(at >> 8);
	putchar(at >> 16);
	putchar(at >> 24);
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
	for (i = 0; i < 8; i += 1) {
		putchar(0x90);
	}

	// text
	for(b = text; b; b = b->next) {
		for (i = 0; i < b->fill; i += 1) {
			putchar(b->buf[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// stdlib                                                                    //
///////////////////////////////////////////////////////////////////////////////

// Define "stdlib" functions
void
add_stdlib(void)
{
	struct label *a, *b;
	struct decl *d;

	d = find((unsigned char *)"_start");
	if (!d->defined) {
		d->defined = 1;
		fixup_label(d->label);
		d = find((unsigned char *)"main");
		emit_ptr(d->label);
		emit_call(0);
		// mov rax, 60
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0x3c);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// xor rdi, rdi
		emit(0x48);
		emit(0x31);
		emit(0xff);
		// syscall
		emit(0x0f);
		emit(0x05);
		// halt
		emit(0xeb);
		emit(0xfe);
	}

	d = find((unsigned char *)"getchar");
	if (!d->defined) {
		d->defined = 1;
		fixup_label(d->label);

		a = mklabel();
		b = mklabel();

		// push $0
		emit(0x6a);
		emit(0x00);
		// mov rax, 0
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rdi, 0
		emit(0x48);
		emit(0xc7);
		emit(0xc7);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rsi, rsp
		emit(0x48);
		emit(0x89);
		emit(0xe6);
		// mov rdx, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc2);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// syscall
		emit(0x0f);
		emit(0x05);
		// cmp rax, 1
		emit(0x48);
		emit(0x83);
		emit(0xf8);
		emit(0x01);
		// jne .fail
		emit(0x0f);
		emit(0x85);
		addfixup(a);
		// pop rax
		emit(0x58);
		// ret
		emit(0xc3);
		// .fail:
		fixup_label(a);
		// cmp rax, 0
		emit(0x48);
		emit(0x83);
		emit(0xf8);
		emit(0x00);
		// jne .die
		emit(0x0f);
		emit(0x85);
		addfixup(b);
		// pop rax
		emit(0x58);
		// mov rax, -1
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0xff);
		emit(0xff);
		emit(0xff);
		emit(0xff);
		// ret
		emit(0xc3);
		// .die:
		fixup_label(b);
		// mov rax, 60
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0x3c);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rdi, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc7);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// syscall
		emit(0x0f);
		emit(0x05);
		// jmp .die
		emit(0xe9);
		addfixup(b);

	}

	d = find((unsigned char *)"putchar");
	if (!d->defined) {
		d->defined = 1;
		fixup_label(d->label);

		a = mklabel();

		// mov rax, [rsp + 8]
		emit(0x48);
		emit(0x8b);
		emit(0x44);
		emit(0x24);
		emit(0x08);
		// push rax
		emit(0x50);
		// mov rax, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rdi, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc7);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rsi, rsp
		emit(0x48);
		emit(0x89);
		emit(0xe6);
		// mov rdx, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc2);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// syscall
		emit(0x0f);
		emit(0x05);
		// cmp rax, 1
		emit(0x48);
		emit(0x83);
		emit(0xf8);
		emit(0x01);
		// jne .fail
		emit(0x0f);
		emit(0x85);
		addfixup(a);
		// pop rax
		emit(0x58);
		// ret
		emit(0xc3);
		// .fail:
		fixup_label(a);
		// mov rax, 60
		emit(0x48);
		emit(0xc7);
		emit(0xc0);
		emit(0x3c);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// mov rdi, 1
		emit(0x48);
		emit(0xc7);
		emit(0xc7);
		emit(0x01);
		emit(0x00);
		emit(0x00);
		emit(0x00);
		// syscall
		emit(0x0f);
		emit(0x05);
		// jmp .fail
		emit(0xe9);
		addfixup(a);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Code Generation                                                           //
///////////////////////////////////////////////////////////////////////////////

// Translate an expression
void
texpr(struct node *n)
{
	struct decl *d;
	struct node *a;
	int nargs;

	switch (n->kind) {
	case N_IDENT:
		d = find(n->s);
		emit_ptr(d->label);
		break;
	case N_NUM:
		emit_num(n->n);
		break;
	case N_STR:
		emit_str(n->s);
		break;
	case N_EXPRLIST:
		if (n->b) {
			texpr(n->b);
		}
		texpr(n->a);
		break;
	case N_CALL:
		nargs = 0;
		for (a = n->b; a; a = a->b) {
			nargs += 1;
		}
		if (n->b) {
			texpr(n->b);
		}
		texpr(n->a);
		emit_call(nargs);
		break;
	default:
		die("invalid expr");
	}
}

// Translate a statement
void
tstmt(struct node *n)
{
	switch (n->kind) {
	case N_IDENT: case N_NUM: case N_STR: case N_CALL:
		texpr(n);
		emit_pop(1);
		break;
	default:
		die("invalid stmt");
	}
}

// Translate a function
void
tfunc(struct decl *d)
{
	struct node *n;

	if (!d->body) {
		return;
	}

	fixup_label(d->label);

	for (n = d->body; n && n->a; n = n->b) {
		tstmt(n->a);
	}

	emit_num(0);
	emit_ret();
}

// Translate all functions
void
translate(struct node *p)
{
	struct decl *d;

	for (d = firstdecl(); d; d = nextdecl(d)) {
		tfunc(d);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Compiler Debug Helpers                                                    //
///////////////////////////////////////////////////////////////////////////////

// Dump a type tree
void
showt(struct type *t)
{
	switch (t->kind) {
	case T_VOID:
		fprintf(stderr, "void");
		break;
	case T_INT:
		fprintf(stderr, "int");
		break;
	case T_BYTE:
		fprintf(stderr, "byte");
		break;
	case T_PTR:
		fprintf(stderr, "(ptr ");
		showt(t->val);
		fprintf(stderr, ")");
		break;
	case T_FUNC:
		fprintf(stderr, "(func ");
		showt(t->val);
		for (t = t->arg; t; t = t->arg) {
			fprintf(stderr, " ");
			showt(t->val);
		}
		fprintf(stderr, ")");
		break;
	default:
		die("invalid type");
	}
}

// Dump a parse tree
void
show(struct node *n)
{
	if (!n) {
		fprintf(stderr, "nil");
		return;
	}

	switch (n->kind) {
	case N_IDENT:
		fprintf(stderr, "(ident %s)", n->s);
		break;
	case N_NUM:
		fprintf(stderr, "(num %u)", n->n);
		break;
	case N_STR:
		fprintf(stderr, "(str \"%s\")", n->s);
		break;
	case N_STMTLIST:
		fprintf(stderr, "(stmtlist ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_EXPRLIST:
		fprintf(stderr, "(exprlist ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_CALL:
		fprintf(stderr, "(call ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_ARGDECL:
		fprintf(stderr, "(argdecl ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_ARGLIST:
		fprintf(stderr, "(arglist ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_FUNC:
		fprintf(stderr, "(func ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_FUNCDECL:
		fprintf(stderr, "(funcdecl ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_FUNCTYPE:
		fprintf(stderr, "(functype ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	case N_PROGRAM:
		fprintf(stderr, "(program ");
		show(n->a);
		fprintf(stderr, " ");
		show(n->b);
		fprintf(stderr, ")");
		break;
	default:
		die("invalid node");
	}

	if (n->t) {
		fprintf(stderr, ":");
		showt(n->t);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Main                                                                      //
///////////////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
	struct node *p;

	// Setup the compiler
	setup();

	// Parse the program
	p = program();

	// Verify types
	typecheck(p);

	// Generate code
	translate(p);

	// Define _start, getchar, putchar, malloc
	add_stdlib();

	// Write output
	writeout();

	return 0;
}
