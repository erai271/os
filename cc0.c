#ifndef my__start
#define my__start main
#endif
#ifndef my_syscall
#define my_syscall syscall
#endif
struct my_alloc;
struct my_assembler;
struct my_chunk;
struct my_compiler;
struct my_decl;
struct my_file;
struct my_fixup;
struct my_label;
struct my_name_node;
struct my_node;
struct my_page;
struct my_parser;
struct my_peg;
struct my_peg_compiler;
struct my_peg_frame;
struct my_peg_node;
struct my_peg_op;
struct my_section;
struct my_sigaction;
struct my_symbol;
struct my_type;
struct my_alloc {
	struct my_page* my_page;
};
struct my_assembler {
	struct my_alloc* my_a;
	struct my_file* my_out;
	unsigned long my_at;
	struct my_chunk* my_text;
	struct my_chunk* my_text_end;
	unsigned long my_bits32;
	struct my_symbol* my_symbols;
	struct my_section* my_sections;
	unsigned long my_num_sections;
};
struct my_chunk {
	struct my_chunk* my_next;
	unsigned char* my_buf;
	unsigned long my_fill;
	unsigned long my_cap;
};
struct my_compiler {
	struct my_alloc* my_a;
	struct my_parser* my_p;
	unsigned char* my_filename;
	unsigned long my_lineno;
	unsigned long my_colno;
	struct my_assembler* my_s;
	struct my_decl* my_decls;
	unsigned long my_do_cout;
	struct my_file* my_cout;
	struct my_label* my_start;
	struct my_label* my_kstart;
	struct my_decl* my_used_top;
};
struct my_decl {
	unsigned char* my_name;
	unsigned char* my_member_name;
	struct my_decl* my_p;
	struct my_decl* my_l;
	struct my_decl* my_r;
	struct my_decl* my_used_next;
	unsigned long my_func_defined;
	struct my_type* my_func_type;
	struct my_label* my_func_label;
	struct my_node* my_func_def;
	struct my_node* my_func_decl;
	unsigned long my_func_used;
	unsigned long my_func_preamble;
	unsigned long my_struct_defined;
	unsigned long my_struct_size;
	unsigned long my_struct_layout_done;
	struct my_node* my_struct_def;
	unsigned long my_member_defined;
	struct my_type* my_member_type;
	unsigned long my_member_offset;
	struct my_node* my_member_def;
	unsigned long my_enum_defined;
	unsigned long my_enum_value;
	struct my_node* my_enum_def;
	unsigned long my_var_defined;
	struct my_type* my_var_type;
	unsigned long my_var_offset;
	struct my_node* my_var_def;
	unsigned long my_goto_defined;
	struct my_label* my_goto_label;
};
struct my_file {
	struct my_alloc* my_a;
	unsigned long my_fd;
	unsigned char* my_buf;
	unsigned long my_r;
	unsigned long my_w;
	unsigned long my_cap;
	unsigned long my_eof;
};
struct my_fixup {
	struct my_fixup* my_next;
	unsigned char* my_ptr;
	unsigned long my_at;
};
struct my_label {
	struct my_fixup* my_fix;
	unsigned long my_at;
	unsigned long my_fixed;
};
struct my_name_node {
	struct my_name_node* my_next;
	unsigned char* my_name;
};
struct my_node {
	unsigned long my_kind;
	struct my_node* my_a;
	struct my_node* my_b;
	unsigned char* my_filename;
	unsigned long my_lineno;
	unsigned long my_colno;
	unsigned long my_n;
	unsigned char* my_s;
	struct my_type* my_t;
};
struct my_page {
	unsigned char* my_ptr;
	unsigned long my_fill;
	unsigned long my_size;
};
struct my_parser {
	struct my_alloc* my_a;
	struct my_peg* my_p;
};
struct my_peg {
	struct my_alloc* my_a;
	unsigned long(* my_grammar)(struct my_peg*);
	unsigned char*(* my_tag_to_str)(unsigned long);
	unsigned char* my_filename;
	unsigned char* my_src;
	unsigned long my_size;
	unsigned long my_pos;
	unsigned long my_line;
	unsigned long my_col;
	unsigned long my_tag;
	unsigned long my_fail_depth;
	unsigned long my_fail_tag;
	unsigned long my_fail_line;
	unsigned long my_fail_col;
	unsigned char* my_fail_literal;
	struct my_peg_frame* my_stack;
	unsigned long my_sp;
	unsigned long my_limit;
	unsigned long my_depth;
	unsigned long my_op;
	struct my_peg_op* my_out;
	unsigned long my_cap;
	struct my_peg_node** my_nstack;
	unsigned long my_np;
	unsigned long my_ncap;
};
struct my_peg_compiler {
	struct my_alloc* my_a;
	struct my_peg* my_p;
	struct my_file* my_out;
	unsigned char* my_scratch;
	unsigned char* my_prefix;
};
struct my_peg_frame {
	unsigned long my_pos;
	unsigned long my_depth;
	unsigned long my_op;
	unsigned long my_tag;
	unsigned long my_line;
	unsigned long my_col;
};
struct my_peg_node {
	unsigned long my_tag;
	struct my_peg_node* my_next;
	struct my_peg_node* my_child;
	unsigned char* my_str;
	unsigned long my_len;
	unsigned char* my_filename;
	unsigned long my_line;
	unsigned long my_col;
};
struct my_peg_op {
	unsigned long my_tag;
	unsigned long my_nargs;
	unsigned long my_start;
	unsigned long my_end;
	unsigned long my_line;
	unsigned long my_col;
};
struct my_section {
	struct my_section* my_next;
	unsigned char* my_name;
	unsigned long my_start;
	unsigned long my_end;
	unsigned long my_index;
	unsigned long my_name_offset;
	unsigned long my_type;
	unsigned long my_link;
	unsigned long my_entsize;
};
struct my_sigaction {
	unsigned long my_handler;
	unsigned long my_flags;
	unsigned long my_restorer;
	unsigned long my_mask;
};
struct my_symbol {
	struct my_symbol* my_next;
	unsigned char* my_name;
	struct my_label* my_label;
	unsigned long my_name_offset;
};
struct my_type {
	unsigned long my_kind;
	struct my_decl* my_st;
	struct my_type* my_val;
	struct my_type* my_arg;
};
enum {
	my_AF_INET = 2,
	my_CC_A = 7,
	my_CC_AE = 3,
	my_CC_B = 2,
	my_CC_BE = 6,
	my_CC_E = 4,
	my_CC_G = 15,
	my_CC_GE = 13,
	my_CC_L = 12,
	my_CC_LE = 14,
	my_CC_NE = 5,
	my_CC_NO = 1,
	my_CC_NP = 11,
	my_CC_NS = 9,
	my_CC_O = 0,
	my_CC_P = 10,
	my_CC_S = 8,
	my_EINTR = 4,
	my_EPIPE = 32,
	my_EXACTLY_ONE = 1,
	my_LOOK_AND = 2,
	my_LOOK_NORMAL = 0,
	my_LOOK_NOT = 1,
	my_N_ADD = 44,
	my_N_AND = 52,
	my_N_ARGDECL = 11,
	my_N_ARGLIST = 9,
	my_N_ASSIGN = 32,
	my_N_BAND = 51,
	my_N_BNOT = 49,
	my_N_BOR = 50,
	my_N_BREAK = 26,
	my_N_CALL = 7,
	my_N_CAST = 36,
	my_N_CHAR = 2,
	my_N_COND = 21,
	my_N_CONDLIST = 20,
	my_N_CONTINUE = 27,
	my_N_DEREF = 35,
	my_N_DIV = 58,
	my_N_DOT = 8,
	my_N_ENUM = 22,
	my_N_ENUMITEM = 23,
	my_N_ENUMLIST = 24,
	my_N_EQ = 42,
	my_N_EXPRLIST = 6,
	my_N_FUNC = 10,
	my_N_FUNCDECL = 12,
	my_N_FUNCTYPE = 14,
	my_N_GE = 41,
	my_N_GOTO = 31,
	my_N_GT = 39,
	my_N_IDENT = 0,
	my_N_INDEX = 37,
	my_N_LABEL = 30,
	my_N_LE = 40,
	my_N_LOOP = 25,
	my_N_LSH = 47,
	my_N_LT = 38,
	my_N_MEMBERDECL = 18,
	my_N_MEMBERLIST = 19,
	my_N_MOD = 59,
	my_N_MUL = 46,
	my_N_NE = 43,
	my_N_NEG = 57,
	my_N_NIL = 4,
	my_N_NOT = 55,
	my_N_NUM = 1,
	my_N_OR = 53,
	my_N_POS = 56,
	my_N_PROGRAM = 13,
	my_N_PTRTYPE = 15,
	my_N_REF = 34,
	my_N_RETURN = 28,
	my_N_RSH = 48,
	my_N_SIZEOF = 33,
	my_N_STMTLIST = 5,
	my_N_STR = 3,
	my_N_STRUCT = 16,
	my_N_SUB = 45,
	my_N_UNION = 17,
	my_N_VARDECL = 29,
	my_N_XOR = 54,
	my_ONE_OR_MORE = 3,
	my_OP_ADCRM = 19,
	my_OP_ADDI = 129,
	my_OP_ADDRM = 3,
	my_OP_ANDI = 262273,
	my_OP_ANDRM = 35,
	my_OP_CALL = 232,
	my_OP_CLD = 252,
	my_OP_CLI = 250,
	my_OP_CMPI = 458881,
	my_OP_CMPRM = 59,
	my_OP_CPUID = 4002,
	my_OP_GS = 101,
	my_OP_HLT = 244,
	my_OP_ICALLM = 131327,
	my_OP_IDIVM = 458999,
	my_OP_IMULM = 262391,
	my_OP_IN = 236,
	my_OP_IND = 237,
	my_OP_INVLPGM = 462593,
	my_OP_IRET = 207,
	my_OP_IRETQ = 18639,
	my_OP_JCC = 3968,
	my_OP_JMP = 233,
	my_OP_LEA = 141,
	my_OP_LGDTM = 134913,
	my_OP_LIDTM = 200449,
	my_OP_LLDM = 134912,
	my_OP_LLDTM = 134912,
	my_OP_LOAD = 139,
	my_OP_LOAD16 = 26250,
	my_OP_LOADB = 138,
	my_OP_LTRM = 200448,
	my_OP_MOVABS = 184,
	my_OP_MOVE = 139,
	my_OP_MOVI = 199,
	my_OP_NEGM = 196855,
	my_OP_NOP = 144,
	my_OP_NOTM = 131319,
	my_OP_ORI = 65665,
	my_OP_ORRM = 11,
	my_OP_OS = 102,
	my_OP_OUT = 238,
	my_OP_OUTD = 239,
	my_OP_POPF = 157,
	my_OP_POPR = 88,
	my_OP_PUSHF = 156,
	my_OP_PUSHR = 80,
	my_OP_RDCRR = 3872,
	my_OP_RDMSR = 3890,
	my_OP_RDRAND = 397255,
	my_OP_RDSR = 140,
	my_OP_RET = 195,
	my_OP_SETCC = 3984,
	my_OP_SHLM = 262355,
	my_OP_SHRM = 327891,
	my_OP_STI = 251,
	my_OP_STORE = 137,
	my_OP_STORE16 = 26249,
	my_OP_STOREB = 136,
	my_OP_SUBI = 327809,
	my_OP_SUBRM = 43,
	my_OP_SYSCALL = 3845,
	my_OP_SYSRET = 3847,
	my_OP_TESTRM = 133,
	my_OP_UD2 = 3851,
	my_OP_WBINVD = 3849,
	my_OP_WRCRR = 3874,
	my_OP_WRMSR = 3888,
	my_OP_WRSR = 142,
	my_OP_XORRM = 51,
	my_O_CREAT = 64,
	my_O_DIRECTORY = 4096,
	my_O_RDONLY = 0,
	my_O_RDWR = 2,
	my_O_WRONLY = 1,
	my_PEG_alternative = 3,
	my_PEG_any = 9,
	my_PEG_call = 12,
	my_PEG_class = 11,
	my_PEG_countop = 6,
	my_PEG_grammar = 0,
	my_PEG_identifier = 13,
	my_PEG_literal = 10,
	my_PEG_lookahead = 5,
	my_PEG_lookop = 4,
	my_PEG_pattern = 2,
	my_PEG_primary = 8,
	my_PEG_rule = 1,
	my_PEG_sp = 14,
	my_PEG_suffix = 7,
	my_POLLERR = 8,
	my_POLLHUP = 16,
	my_POLLIN = 1,
	my_POLLNVAL = 32,
	my_POLLOUT = 4,
	my_POLLPRI = 2,
	my_P_add_expr = 41,
	my_P_add_op = 37,
	my_P_and_op = 45,
	my_P_arg_decl = 9,
	my_P_as = 85,
	my_P_assign_stmt = 22,
	my_P_band_op = 27,
	my_P_bnot_op = 55,
	my_P_bool_expr = 29,
	my_P_bor_op = 28,
	my_P_break = 70,
	my_P_break_stmt = 16,
	my_P_byte = 81,
	my_P_call_expr = 58,
	my_P_cast_expr = 60,
	my_P_char = 67,
	my_P_comp_expr = 36,
	my_P_compound_stmt = 25,
	my_P_continue = 75,
	my_P_continue_stmt = 17,
	my_P_dec = 65,
	my_P_deref_op = 51,
	my_P_div_op = 43,
	my_P_elif_stmt = 12,
	my_P_else = 73,
	my_P_else_stmt = 13,
	my_P_empty_stmt = 24,
	my_P_enum = 78,
	my_P_enum_decl = 2,
	my_P_enum_item = 1,
	my_P_eq_op = 34,
	my_P_expr = 26,
	my_P_expr_stmt = 23,
	my_P_func = 84,
	my_P_func_decl = 6,
	my_P_func_type = 10,
	my_P_ge_op = 31,
	my_P_goto = 76,
	my_P_goto_stmt = 21,
	my_P_grammar = 0,
	my_P_gt_op = 33,
	my_P_hex = 64,
	my_P_ident = 87,
	my_P_if = 72,
	my_P_if_stmt = 14,
	my_P_index_expr = 57,
	my_P_int = 82,
	my_P_label_stmt = 20,
	my_P_le_op = 30,
	my_P_loop = 74,
	my_P_loop_stmt = 15,
	my_P_lsh_op = 47,
	my_P_lt_op = 32,
	my_P_member_decl = 3,
	my_P_member_expr = 59,
	my_P_mod_op = 44,
	my_P_mul_expr = 46,
	my_P_mul_op = 42,
	my_P_ne_op = 35,
	my_P_neg_op = 53,
	my_P_nil = 86,
	my_P_not_op = 54,
	my_P_or_op = 39,
	my_P_pos_op = 52,
	my_P_post_expr = 61,
	my_P_primary = 62,
	my_P_ptr_type = 8,
	my_P_ref_op = 50,
	my_P_reserved = 68,
	my_P_return = 69,
	my_P_return_stmt = 18,
	my_P_rsh_op = 48,
	my_P_shift_expr = 49,
	my_P_sizeof = 71,
	my_P_sizeof_expr = 63,
	my_P_sp = 88,
	my_P_stmt = 11,
	my_P_str = 66,
	my_P_struct = 79,
	my_P_struct_decl = 4,
	my_P_sub_op = 38,
	my_P_type = 7,
	my_P_unary_expr = 56,
	my_P_union = 80,
	my_P_union_decl = 5,
	my_P_var = 77,
	my_P_var_stmt = 19,
	my_P_void = 83,
	my_P_xor_op = 40,
	my_R_CR0 = 0,
	my_R_CR1 = 1,
	my_R_CR2 = 2,
	my_R_CR3 = 3,
	my_R_CR4 = 4,
	my_R_CR5 = 5,
	my_R_CR6 = 6,
	my_R_CR7 = 7,
	my_R_CS = 1,
	my_R_DS = 3,
	my_R_ES = 0,
	my_R_FS = 4,
	my_R_GS = 5,
	my_R_R10 = 10,
	my_R_R11 = 11,
	my_R_R12 = 12,
	my_R_R13 = 13,
	my_R_R14 = 14,
	my_R_R15 = 15,
	my_R_R8 = 8,
	my_R_R9 = 9,
	my_R_RAX = 0,
	my_R_RBP = 5,
	my_R_RBX = 3,
	my_R_RCX = 1,
	my_R_RDI = 7,
	my_R_RDX = 2,
	my_R_RIP = 16,
	my_R_RSI = 6,
	my_R_RSP = 4,
	my_R_SS = 2,
	my_SHT_NULL = 0,
	my_SHT_PROGBITS = 1,
	my_SHT_STRTAB = 3,
	my_SHT_SYMTAB = 2,
	my_SIGALRM = 14,
	my_SIGCHLD = 17,
	my_SIGINT = 2,
	my_SIGPIPE = 13,
	my_SIGWINCH = 28,
	my_SIG_DFL = 0,
	my_SIG_IGN = 1,
	my_SOCK_STREAM = 1,
	my_TY_ARG = 5,
	my_TY_BYTE = 3,
	my_TY_FUNC = 6,
	my_TY_INT = 2,
	my_TY_NIL = 0,
	my_TY_PTR = 4,
	my_TY_STRUCT = 7,
	my_TY_UNION = 8,
	my_TY_VOID = 1,
	my_WNOHANG = 1,
	my_ZERO_OR_MORE = 2,
	my_ZERO_OR_ONE = 0
};
unsigned char*( my_PEG_tag_to_str)(unsigned long my_tag);
unsigned char*( my_P_tag_to_str)(unsigned long my_tag);
void( my__start)(unsigned long my_argc,unsigned char** my_argv,unsigned char** my_envp);
void( my_add_section)(struct my_assembler* my_c,unsigned char* my_name,unsigned long my_type);
void( my_add_symbol)(struct my_assembler* my_c,unsigned char* my_name,struct my_label* my_l);
void( my_addfixup)(struct my_assembler* my_c,struct my_label* my_l);
unsigned char*( my_alloc)(struct my_alloc* my_c,unsigned long my_size);
unsigned long( my_any)(struct my_peg* my_c);
void( my_as_emit)(struct my_assembler* my_a,unsigned long my_b);
void( my_as_jmp)(struct my_assembler* my_a,unsigned long my_op,struct my_label* my_l);
void( my_as_modm)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_b,unsigned long my_i,unsigned long my_s,unsigned long my_d);
void( my_as_modr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_b);
void( my_as_modra)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_d);
void( my_as_modri)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_x);
void( my_as_modrm)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_b,unsigned long my_i,unsigned long my_s,unsigned long my_d);
void( my_as_modrr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_b);
void( my_as_op)(struct my_assembler* my_a,unsigned long my_op);
void( my_as_opr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r);
void( my_as_opri64)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_x);
void( my_as_rex)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_i,unsigned long my_b);
void( my_assert)(unsigned long my_x,unsigned char* my_msg);
unsigned long( my_call_check)(struct my_compiler* my_c,struct my_node* my_n);
void( my_cdie)(struct my_compiler* my_c,unsigned char* my_msg);
unsigned long( my_charset)(struct my_peg* my_c,unsigned char* my_s);
void( my_check_usage)(struct my_compiler* my_c);
void( my_choice)(struct my_peg* my_c);
unsigned long( my_close)(unsigned long my_fd);
void( my_commit)(struct my_peg* my_c);
struct my_compiler*( my_comp_setup)(struct my_alloc* my_a);
void( my_compile)(struct my_compiler* my_c,struct my_node* my_p);
void( my_compile_expr)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_rhs);
void( my_compile_func)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_compile_include)(struct my_compiler* my_c,struct my_node* my_n);
void( my_compile_stmt)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,struct my_label* my_top,struct my_label* my_out);
struct my_node*( my_concat_program)(struct my_node* my_a,struct my_node* my_b);
struct my_peg_node*( my_construct)(struct my_peg* my_c,unsigned long my_sp);
void( my_copypos)(struct my_node* my_n,struct my_peg_node* my_pn);
unsigned long( my_count_args)(struct my_compiler* my_c,struct my_type* my_t);
void( my_cshow_context)(struct my_compiler* my_c);
void( my_ctranslate)(struct my_compiler* my_c);
void( my_ctranslate_expr)(struct my_compiler* my_c,struct my_node* my_n);
void( my_ctranslate_func)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_ctranslate_stmt)(struct my_compiler* my_c,struct my_node* my_n);
void( my_ctranslate_str)(struct my_compiler* my_c,unsigned char* my_s);
void( my_ctranslate_struct)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_ctranslate_type)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,unsigned long my_decl,struct my_node* my_args);
void( my_ctranslate_type1)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,unsigned long my_decl);
void( my_ctranslate_type2)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,struct my_node* my_args);
void( my_ctranslate_vars)(struct my_compiler* my_c,struct my_node* my_n);
void( my_ctranslate_zero)(struct my_compiler* my_c,struct my_type* my_ty);
unsigned long( my_dec2int)(unsigned char* my_s,unsigned long my_len,unsigned long* my_ok);
unsigned long( my_decode_count)(struct my_peg_node* my_n);
unsigned long( my_decode_look)(struct my_peg_node* my_n);
void( my_defenum)(struct my_compiler* my_c,struct my_node* my_n);
struct my_decl*( my_defextern)(struct my_compiler* my_c,struct my_node* my_n);
void( my_defstruct)(struct my_compiler* my_c,struct my_node* my_n);
void( my_defun)(struct my_compiler* my_c,struct my_node* my_n);
void( my_defunion)(struct my_compiler* my_c,struct my_node* my_n);
void( my_die)(unsigned char* my_msg);
void( my_emit)(struct my_assembler* my_c,unsigned long my_x);
void( my_emit_add)(struct my_assembler* my_c);
void( my_emit_align)(struct my_assembler* my_c,unsigned long my_n,unsigned long my_b);
void( my_emit_and)(struct my_assembler* my_c);
void( my_emit_blob)(struct my_assembler* my_c,unsigned char* my_s,unsigned long my_n);
void( my_emit_builtin)(struct my_compiler* my_c);
void( my_emit_call)(struct my_assembler* my_c,unsigned long my_n);
void( my_emit_div)(struct my_assembler* my_c);
void( my_emit_eq)(struct my_assembler* my_c);
void( my_emit_ge)(struct my_assembler* my_c);
void( my_emit_gt)(struct my_assembler* my_c);
void( my_emit_isr)(struct my_compiler* my_c);
void( my_emit_jmp)(struct my_assembler* my_c,struct my_label* my_l);
void( my_emit_jz)(struct my_assembler* my_c,struct my_label* my_l);
void( my_emit_kstart)(struct my_assembler* my_c);
void( my_emit_lcall)(struct my_assembler* my_c,struct my_label* my_l,unsigned long my_n);
void( my_emit_le)(struct my_assembler* my_c);
void( my_emit_lea)(struct my_assembler* my_c,unsigned long my_offset);
void( my_emit_load)(struct my_assembler* my_c,struct my_type* my_t);
void( my_emit_lsh)(struct my_assembler* my_c);
void( my_emit_lt)(struct my_assembler* my_c);
void( my_emit_mod)(struct my_assembler* my_c);
void( my_emit_mul)(struct my_assembler* my_c);
void( my_emit_ne)(struct my_assembler* my_c);
void( my_emit_neg)(struct my_assembler* my_c);
void( my_emit_not)(struct my_assembler* my_c);
void( my_emit_num)(struct my_assembler* my_c,unsigned long my_x);
void( my_emit_or)(struct my_assembler* my_c);
void( my_emit_pop)(struct my_assembler* my_c,unsigned long my_n);
void( my_emit_preamble)(struct my_assembler* my_c,unsigned long my_n,unsigned long my_pragma);
void( my_emit_ptr)(struct my_assembler* my_c,struct my_label* my_l);
void( my_emit_restorer)(struct my_assembler* my_c);
void( my_emit_ret)(struct my_assembler* my_c);
void( my_emit_rsh)(struct my_assembler* my_c);
unsigned long( my_emit_sections)(struct my_assembler* my_c);
void( my_emit_ssr)(struct my_compiler* my_c);
void( my_emit_store)(struct my_assembler* my_c,struct my_type* my_t);
void( my_emit_str)(struct my_assembler* my_c,unsigned char* my_s);
unsigned long( my_emit_strtab_str)(struct my_assembler* my_c,unsigned char* my_s);
void( my_emit_sub)(struct my_assembler* my_c);
void( my_emit_syscall)(struct my_assembler* my_c);
void( my_emit_ud)(struct my_assembler* my_c);
void( my_emit_xor)(struct my_assembler* my_c);
void( my_enter)(struct my_peg* my_c,unsigned long my_tag);
void( my_exit)(unsigned long my_n);
void( my_fail)(struct my_peg* my_c);
void( my_fclose)(struct my_file* my_f);
void( my_fdputc)(unsigned long my_fd,unsigned long my_ch);
void( my_fdputd)(unsigned long my_fd,unsigned long my_n);
void( my_fdputs)(unsigned long my_fd,unsigned char* my_msg);
void( my_ffill)(struct my_file* my_f);
void( my_fflush)(struct my_file* my_f);
unsigned long( my_fgetc)(struct my_file* my_f);
void( my_fillpos)(struct my_parser* my_c,struct my_node* my_n);
struct my_decl*( my_find)(struct my_compiler* my_c,unsigned char* my_name,unsigned char* my_member_name,unsigned long my_make);
struct my_section*( my_find_section)(struct my_assembler* my_c,unsigned char* my_name);
struct my_decl*( my_first_decl)(struct my_compiler* my_c);
void( my_fixup)(struct my_assembler* my_c,unsigned char* my_here,unsigned long my_delta);
void( my_fixup_label)(struct my_assembler* my_c,struct my_label* my_l);
void( my_flush_coutput)(struct my_compiler* my_c);
struct my_file*( my_fopen)(unsigned long my_fd,struct my_alloc* my_a);
void( my_fputb)(struct my_file* my_f,unsigned char* my_s,unsigned long my_n);
void( my_fputc)(struct my_file* my_f,unsigned long my_ch);
void( my_fputd)(struct my_file* my_out,unsigned long my_n);
void( my_fputs)(struct my_file* my_f,unsigned char* my_s);
unsigned char*( my_freadall)(struct my_file* my_f,unsigned long* my_size);
void( my_free)(struct my_alloc* my_a,unsigned char* my_p);
unsigned long( my_get)(struct my_peg* my_c);
unsigned long( my_hex2int)(unsigned char* my_s,unsigned long my_len,unsigned long* my_ok);
unsigned long( my_hexdig)(unsigned long my_ch,unsigned long* my_ok);
unsigned long( my_hoist_locals)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_offset);
void( my_layout_struct)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_layout_union)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_leave)(struct my_peg* my_c,unsigned long my_tag);
unsigned long( my_literal)(struct my_peg* my_c,unsigned char* my_s);
void( my_main)(unsigned long my_argc,unsigned char** my_argv,unsigned char** my_envp);
void( my_mark_expr_used)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n);
void( my_mark_func_used)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_mark_stmt_used)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n);
void( my_memcpy)(unsigned char* my_dest,unsigned char* my_src,unsigned long my_size);
void( my_memset)(unsigned char* my_dest,unsigned long my_c,unsigned long my_size);
struct my_label*( my_mklabel)(struct my_assembler* my_c);
struct my_node*( my_mknode)(struct my_parser* my_c,unsigned long my_kind,struct my_node* my_a,struct my_node* my_b);
struct my_node*( my_mknode0)(struct my_parser* my_c,unsigned long my_kind);
struct my_node*( my_mknode1)(struct my_parser* my_c,unsigned long my_kind,struct my_node* my_a);
struct my_type*( my_mktype)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a,struct my_type* my_b,struct my_decl* my_st);
struct my_type*( my_mktype0)(struct my_compiler* my_c,unsigned long my_kind);
struct my_type*( my_mktype1)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a);
struct my_type*( my_mktype2)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a,struct my_type* my_b);
struct my_type*( my_mktype_struct)(struct my_compiler* my_c,struct my_decl* my_st);
struct my_type*( my_mktype_union)(struct my_compiler* my_c,struct my_decl* my_st);
unsigned long( my_mmap)(unsigned long my_addr,unsigned long my_len,unsigned long my_prot,unsigned long my_flags,unsigned long my_fd,unsigned long my_off);
struct my_decl*( my_next_decl)(struct my_compiler* my_c,struct my_decl* my_d);
unsigned char*( my_node_to_str)(unsigned long my_kind);
unsigned long( my_open)(unsigned char* my_name,unsigned long my_flags,unsigned long my_mode);
void( my_open_coutput)(struct my_compiler* my_c,unsigned char* my_filename);
void( my_open_output)(struct my_assembler* my_c,unsigned char* my_filename);
struct my_node*( my_parse)(struct my_parser* my_c,unsigned char* my_filename);
unsigned long( my_parse_escape)(unsigned char* my_s,unsigned long* my_i,unsigned long my_n);
unsigned long( my_peg_PEG_alternative)(struct my_peg* my_c);
unsigned long( my_peg_PEG_any)(struct my_peg* my_c);
unsigned long( my_peg_PEG_call)(struct my_peg* my_c);
unsigned long( my_peg_PEG_class)(struct my_peg* my_c);
unsigned long( my_peg_PEG_countop)(struct my_peg* my_c);
unsigned long( my_peg_PEG_grammar)(struct my_peg* my_c);
unsigned long( my_peg_PEG_identifier)(struct my_peg* my_c);
unsigned long( my_peg_PEG_literal)(struct my_peg* my_c);
unsigned long( my_peg_PEG_lookahead)(struct my_peg* my_c);
unsigned long( my_peg_PEG_lookop)(struct my_peg* my_c);
unsigned long( my_peg_PEG_pattern)(struct my_peg* my_c);
unsigned long( my_peg_PEG_primary)(struct my_peg* my_c);
unsigned long( my_peg_PEG_rule)(struct my_peg* my_c);
unsigned long( my_peg_PEG_sp)(struct my_peg* my_c);
unsigned long( my_peg_PEG_suffix)(struct my_peg* my_c);
unsigned long( my_peg_P_add_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_add_op)(struct my_peg* my_c);
unsigned long( my_peg_P_and_op)(struct my_peg* my_c);
unsigned long( my_peg_P_arg_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_as)(struct my_peg* my_c);
unsigned long( my_peg_P_assign_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_band_op)(struct my_peg* my_c);
unsigned long( my_peg_P_bnot_op)(struct my_peg* my_c);
unsigned long( my_peg_P_bool_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_bor_op)(struct my_peg* my_c);
unsigned long( my_peg_P_break)(struct my_peg* my_c);
unsigned long( my_peg_P_break_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_byte)(struct my_peg* my_c);
unsigned long( my_peg_P_call_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_cast_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_char)(struct my_peg* my_c);
unsigned long( my_peg_P_comp_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_compound_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_continue)(struct my_peg* my_c);
unsigned long( my_peg_P_continue_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_dec)(struct my_peg* my_c);
unsigned long( my_peg_P_deref_op)(struct my_peg* my_c);
unsigned long( my_peg_P_div_op)(struct my_peg* my_c);
unsigned long( my_peg_P_elif_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_else)(struct my_peg* my_c);
unsigned long( my_peg_P_else_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_empty_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_enum)(struct my_peg* my_c);
unsigned long( my_peg_P_enum_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_enum_item)(struct my_peg* my_c);
unsigned long( my_peg_P_eq_op)(struct my_peg* my_c);
unsigned long( my_peg_P_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_expr_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_func)(struct my_peg* my_c);
unsigned long( my_peg_P_func_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_func_type)(struct my_peg* my_c);
unsigned long( my_peg_P_ge_op)(struct my_peg* my_c);
unsigned long( my_peg_P_goto)(struct my_peg* my_c);
unsigned long( my_peg_P_goto_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_grammar)(struct my_peg* my_c);
unsigned long( my_peg_P_gt_op)(struct my_peg* my_c);
unsigned long( my_peg_P_hex)(struct my_peg* my_c);
unsigned long( my_peg_P_ident)(struct my_peg* my_c);
unsigned long( my_peg_P_if)(struct my_peg* my_c);
unsigned long( my_peg_P_if_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_index_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_int)(struct my_peg* my_c);
unsigned long( my_peg_P_label_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_le_op)(struct my_peg* my_c);
unsigned long( my_peg_P_loop)(struct my_peg* my_c);
unsigned long( my_peg_P_loop_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_lsh_op)(struct my_peg* my_c);
unsigned long( my_peg_P_lt_op)(struct my_peg* my_c);
unsigned long( my_peg_P_member_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_member_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_mod_op)(struct my_peg* my_c);
unsigned long( my_peg_P_mul_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_mul_op)(struct my_peg* my_c);
unsigned long( my_peg_P_ne_op)(struct my_peg* my_c);
unsigned long( my_peg_P_neg_op)(struct my_peg* my_c);
unsigned long( my_peg_P_nil)(struct my_peg* my_c);
unsigned long( my_peg_P_not_op)(struct my_peg* my_c);
unsigned long( my_peg_P_or_op)(struct my_peg* my_c);
unsigned long( my_peg_P_pos_op)(struct my_peg* my_c);
unsigned long( my_peg_P_post_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_primary)(struct my_peg* my_c);
unsigned long( my_peg_P_ptr_type)(struct my_peg* my_c);
unsigned long( my_peg_P_ref_op)(struct my_peg* my_c);
unsigned long( my_peg_P_reserved)(struct my_peg* my_c);
unsigned long( my_peg_P_return)(struct my_peg* my_c);
unsigned long( my_peg_P_return_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_rsh_op)(struct my_peg* my_c);
unsigned long( my_peg_P_shift_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_sizeof)(struct my_peg* my_c);
unsigned long( my_peg_P_sizeof_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_sp)(struct my_peg* my_c);
unsigned long( my_peg_P_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_str)(struct my_peg* my_c);
unsigned long( my_peg_P_struct)(struct my_peg* my_c);
unsigned long( my_peg_P_struct_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_sub_op)(struct my_peg* my_c);
unsigned long( my_peg_P_type)(struct my_peg* my_c);
unsigned long( my_peg_P_unary_expr)(struct my_peg* my_c);
unsigned long( my_peg_P_union)(struct my_peg* my_c);
unsigned long( my_peg_P_union_decl)(struct my_peg* my_c);
unsigned long( my_peg_P_var)(struct my_peg* my_c);
unsigned long( my_peg_P_var_stmt)(struct my_peg* my_c);
unsigned long( my_peg_P_void)(struct my_peg* my_c);
unsigned long( my_peg_P_xor_op)(struct my_peg* my_c);
void( my_peg_compile)(struct my_peg_compiler* my_c,unsigned char* my_filename);
struct my_peg*( my_peg_new)(unsigned char* my_filename,unsigned char* my_src,unsigned long my_len,struct my_alloc* my_a,unsigned long(* my_grammar)(struct my_peg*),unsigned char*(* my_tag_to_str)(unsigned long));
void( my_peg_open_output)(struct my_peg_compiler* my_c,unsigned char* my_filename);
struct my_peg_node*( my_peg_parse)(struct my_peg* my_c,unsigned long my_sp);
void( my_peg_reset)(struct my_peg* my_c,unsigned char* my_filename,unsigned char* my_src,unsigned long my_len);
struct my_type*( my_prototype)(struct my_compiler* my_c,struct my_node* my_n);
void( my_putchar)(struct my_assembler* my_c,unsigned long my_ch);
unsigned long( my_read)(unsigned long my_fd,unsigned char* my_buf,unsigned long my_n);
unsigned char*( my_readall)(unsigned long my_fd,unsigned long* my_len,struct my_alloc* my_a);
struct my_node*( my_reconstruct)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_add)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_arg_decl)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_args)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_assign)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_bool)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_break)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_char)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_comp)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_compound)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_continue)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_dec)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_empty)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_enum)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_enum_item)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_expr)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_expr_stmt)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_func)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_func_type)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_goto)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_hex)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_ident)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_if)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_intern)(struct my_parser* my_c,struct my_peg_node* my_pn,unsigned char* my_s,unsigned long my_len);
struct my_node*( my_reconstruct_label)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_loop)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_member_decl)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_member_list)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_mul)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_nil)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_post)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_primary)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_return)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_shift)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_sizeof)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_stmt)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_str)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_struct)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_type)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_unary)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_union)(struct my_parser* my_c,struct my_peg_node* my_pn);
struct my_node*( my_reconstruct_var)(struct my_parser* my_c,struct my_peg_node* my_pn);
void( my_reserve)(struct my_assembler* my_c,unsigned long my_n);
void( my_setup_alloc)(struct my_alloc* my_c);
struct my_assembler*( my_setup_assembler)(struct my_alloc* my_a);
struct my_parser*( my_setup_parser)(struct my_alloc* my_a);
struct my_peg_compiler*( my_setup_peg)(struct my_alloc* my_a,unsigned char* my_prefix);
void( my_show_node)(struct my_file* my_out,struct my_node* my_n);
unsigned long( my_strcmp)(unsigned char* my_a,unsigned char* my_b);
unsigned long( my_strlen)(unsigned char* my_s);
unsigned long( my_syscall)(unsigned long my_n,unsigned long my_a1,unsigned long my_a2,unsigned long my_a3,unsigned long my_a4,unsigned long my_a5,unsigned long my_a6);
void( my_translate)(struct my_peg_compiler* my_c,struct my_peg_node* my_n);
void( my_translate_charset)(struct my_peg_compiler* my_c,struct my_peg_node* my_n);
void( my_translate_literal)(struct my_peg_compiler* my_c,struct my_peg_node* my_n);
void( my_translate_pattern)(struct my_peg_compiler* my_c,struct my_peg_node* my_n);
unsigned long( my_type_isint)(struct my_type* my_t);
unsigned long( my_type_isprim)(struct my_type* my_t);
unsigned long( my_type_sizeof)(struct my_compiler* my_c,struct my_type* my_t);
void( my_typecheck_expr)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_rhs);
void( my_typecheck_func)(struct my_compiler* my_c,struct my_decl* my_d);
void( my_typecheck_stmt)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n);
unsigned long( my_unescape)(unsigned char* my_s,unsigned long* my_i,unsigned long my_len,unsigned long* my_ok);
void( my_unify)(struct my_compiler* my_c,struct my_type* my_a,struct my_type* my_b);
unsigned long( my_unlink)(unsigned char* my_name);
unsigned long( my_write)(unsigned long my_fd,unsigned char* my_buf,unsigned long my_n);
void( my_writeout)(struct my_assembler* my_c,struct my_label* my_start,struct my_label* my_kstart);
unsigned char*( my_PEG_tag_to_str)(unsigned long my_tag){
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_grammar)))) {
	return (unsigned char *)"grammar";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_rule)))) {
	return (unsigned char *)"rule";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_pattern)))) {
	return (unsigned char *)"pattern";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_alternative)))) {
	return (unsigned char *)"alternative";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_lookop)))) {
	return (unsigned char *)"lookop";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_lookahead)))) {
	return (unsigned char *)"lookahead";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_countop)))) {
	return (unsigned char *)"countop";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_suffix)))) {
	return (unsigned char *)"suffix";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_primary)))) {
	return (unsigned char *)"primary";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_any)))) {
	return (unsigned char *)"any";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_literal)))) {
	return (unsigned char *)"literal";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_class)))) {
	return (unsigned char *)"class";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_call)))) {
	return (unsigned char *)"call";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_identifier)))) {
	return (unsigned char *)"identifier";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_PEG_sp)))) {
	return (unsigned char *)"sp";
	}
	(my_die)(((unsigned char *)"invalid tag"));
}
unsigned char*( my_P_tag_to_str)(unsigned long my_tag){
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_grammar)))) {
	return (unsigned char *)"grammar";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_enum_item)))) {
	return (unsigned char *)"enum_item";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_enum_decl)))) {
	return (unsigned char *)"enum_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_member_decl)))) {
	return (unsigned char *)"member_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_struct_decl)))) {
	return (unsigned char *)"struct_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_union_decl)))) {
	return (unsigned char *)"union_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_func_decl)))) {
	return (unsigned char *)"func_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_type)))) {
	return (unsigned char *)"type";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ptr_type)))) {
	return (unsigned char *)"ptr_type";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_arg_decl)))) {
	return (unsigned char *)"arg_decl";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_func_type)))) {
	return (unsigned char *)"func_type";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_stmt)))) {
	return (unsigned char *)"stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_elif_stmt)))) {
	return (unsigned char *)"elif_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_else_stmt)))) {
	return (unsigned char *)"else_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_if_stmt)))) {
	return (unsigned char *)"if_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_loop_stmt)))) {
	return (unsigned char *)"loop_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_break_stmt)))) {
	return (unsigned char *)"break_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_continue_stmt)))) {
	return (unsigned char *)"continue_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_return_stmt)))) {
	return (unsigned char *)"return_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_var_stmt)))) {
	return (unsigned char *)"var_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_label_stmt)))) {
	return (unsigned char *)"label_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_goto_stmt)))) {
	return (unsigned char *)"goto_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_assign_stmt)))) {
	return (unsigned char *)"assign_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_expr_stmt)))) {
	return (unsigned char *)"expr_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_empty_stmt)))) {
	return (unsigned char *)"empty_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_compound_stmt)))) {
	return (unsigned char *)"compound_stmt";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_expr)))) {
	return (unsigned char *)"expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_band_op)))) {
	return (unsigned char *)"band_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_bor_op)))) {
	return (unsigned char *)"bor_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_bool_expr)))) {
	return (unsigned char *)"bool_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_le_op)))) {
	return (unsigned char *)"le_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ge_op)))) {
	return (unsigned char *)"ge_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_lt_op)))) {
	return (unsigned char *)"lt_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_gt_op)))) {
	return (unsigned char *)"gt_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_eq_op)))) {
	return (unsigned char *)"eq_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ne_op)))) {
	return (unsigned char *)"ne_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_comp_expr)))) {
	return (unsigned char *)"comp_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_add_op)))) {
	return (unsigned char *)"add_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_sub_op)))) {
	return (unsigned char *)"sub_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_or_op)))) {
	return (unsigned char *)"or_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_xor_op)))) {
	return (unsigned char *)"xor_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_add_expr)))) {
	return (unsigned char *)"add_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_mul_op)))) {
	return (unsigned char *)"mul_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_div_op)))) {
	return (unsigned char *)"div_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_mod_op)))) {
	return (unsigned char *)"mod_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_and_op)))) {
	return (unsigned char *)"and_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_mul_expr)))) {
	return (unsigned char *)"mul_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_lsh_op)))) {
	return (unsigned char *)"lsh_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_rsh_op)))) {
	return (unsigned char *)"rsh_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_shift_expr)))) {
	return (unsigned char *)"shift_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ref_op)))) {
	return (unsigned char *)"ref_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_deref_op)))) {
	return (unsigned char *)"deref_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_pos_op)))) {
	return (unsigned char *)"pos_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_neg_op)))) {
	return (unsigned char *)"neg_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_not_op)))) {
	return (unsigned char *)"not_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_bnot_op)))) {
	return (unsigned char *)"bnot_op";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_unary_expr)))) {
	return (unsigned char *)"unary_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_index_expr)))) {
	return (unsigned char *)"index_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_call_expr)))) {
	return (unsigned char *)"call_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_member_expr)))) {
	return (unsigned char *)"member_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_cast_expr)))) {
	return (unsigned char *)"cast_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_post_expr)))) {
	return (unsigned char *)"post_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_primary)))) {
	return (unsigned char *)"primary";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_sizeof_expr)))) {
	return (unsigned char *)"sizeof_expr";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_hex)))) {
	return (unsigned char *)"hex";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_dec)))) {
	return (unsigned char *)"dec";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_str)))) {
	return (unsigned char *)"str";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_char)))) {
	return (unsigned char *)"char";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_reserved)))) {
	return (unsigned char *)"reserved";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_return)))) {
	return (unsigned char *)"return";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_break)))) {
	return (unsigned char *)"break";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_sizeof)))) {
	return (unsigned char *)"sizeof";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_if)))) {
	return (unsigned char *)"if";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_else)))) {
	return (unsigned char *)"else";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_loop)))) {
	return (unsigned char *)"loop";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_continue)))) {
	return (unsigned char *)"continue";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_goto)))) {
	return (unsigned char *)"goto";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_var)))) {
	return (unsigned char *)"var";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_enum)))) {
	return (unsigned char *)"enum";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_struct)))) {
	return (unsigned char *)"struct";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_union)))) {
	return (unsigned char *)"union";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_byte)))) {
	return (unsigned char *)"byte";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_int)))) {
	return (unsigned char *)"int";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_void)))) {
	return (unsigned char *)"void";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_func)))) {
	return (unsigned char *)"func";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_as)))) {
	return (unsigned char *)"as";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_nil)))) {
	return (unsigned char *)"nil";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ident)))) {
	return (unsigned char *)"ident";
	}
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_sp)))) {
	return (unsigned char *)"sp";
	}
	(my_die)(((unsigned char *)"invalid tag"));
}
void( my__start)(unsigned long my_argc,unsigned char** my_argv,unsigned char** my_envp){
	(my_main)((my_argc),(my_argv),(my_envp));
	(my_exit)((0UL));
}
void( my_add_section)(struct my_assembler* my_c,unsigned char* my_name,unsigned long my_type){
	struct my_section* my_s = 0;
	struct my_section* my_n = 0;
	unsigned long my_end = 0;
	(my_end)=((my_c)->my_at);
	(my_emit_align)((my_c),(16UL),(my_OP_NOP));
	(my_s)=((struct my_section*)(my_alloc)(((my_c)->my_a),(72UL)));
	((my_s)->my_next)=((void *)0);
	((my_s)->my_name)=(my_name);
	((my_s)->my_start)=((my_c)->my_at);
	((my_s)->my_end)=((my_c)->my_at);
	((my_s)->my_index)=((my_c)->my_num_sections);
	((my_s)->my_type)=(my_type);
	((my_s)->my_link)=(0UL);
	((my_s)->my_entsize)=(0UL);
	(my_n)=((my_c)->my_sections);
	if (my_n) {
	while (1) {
	if ((unsigned long)(!((my_n)->my_next))) {
	break;
	}
	(my_n)=((my_n)->my_next);
	}
	((my_n)->my_end)=(my_end);
	((my_n)->my_next)=(my_s);
	} else {
	((my_c)->my_sections)=(my_s);
	}
	((my_c)->my_num_sections)=((unsigned long)(((unsigned long)((my_c)->my_num_sections))+((unsigned long)(1UL))));
}
void( my_add_symbol)(struct my_assembler* my_c,unsigned char* my_name,struct my_label* my_l){
	struct my_symbol* my_s = 0;
	(my_s)=((struct my_symbol*)(my_alloc)(((my_c)->my_a),(32UL)));
	((my_s)->my_next)=((my_c)->my_symbols);
	((my_s)->my_name)=(my_name);
	((my_s)->my_label)=(my_l);
	((my_c)->my_symbols)=(my_s);
}
void( my_addfixup)(struct my_assembler* my_c,struct my_label* my_l){
	struct my_fixup* my_f = 0;
	unsigned char* my_here = 0;
	if ((unsigned long)(((long)(((my_c)->my_text_end)->my_fill))<((long)(4UL)))) {
	(my_die)(((unsigned char *)"invalid fixup"));
	}
	(my_here)=(&((((my_c)->my_text_end)->my_buf)[(unsigned long)(((unsigned long)(((my_c)->my_text_end)->my_fill))-((unsigned long)(4UL)))]));
	if ((my_l)->my_fixed) {
	(my_fixup)((my_c),(my_here),((unsigned long)(((unsigned long)((my_l)->my_at))-((unsigned long)((my_c)->my_at)))));
	} else {
	(my_f)=((struct my_fixup*)(my_alloc)(((my_c)->my_a),(24UL)));
	((my_f)->my_next)=((my_l)->my_fix);
	((my_f)->my_ptr)=(my_here);
	((my_f)->my_at)=((my_c)->my_at);
	((my_l)->my_fix)=(my_f);
	}
}
unsigned char*( my_alloc)(struct my_alloc* my_c,unsigned long my_size){
	struct my_page* my_page = 0;
	unsigned long my_mret = 0;
	unsigned char* my_ret = 0;
	unsigned long my_psize = 0;
	if ((unsigned long)(((long)(my_size))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"invalid alloc"));
	}
	if ((unsigned long)(((long)(my_size))>=((long)(2048UL)))) {
	(my_size)=((unsigned long)(((unsigned long)(my_size))+((unsigned long)(4095UL))));
	(my_size)=((unsigned long)(((unsigned long)(my_size))&((unsigned long)((unsigned long)(~(unsigned long)(4095UL))))));
	(my_mret)=((my_mmap)((0UL),(my_size),(3UL),(34UL),((unsigned long)(-(unsigned long)(1UL))),(0UL)));
	if ((unsigned long)(((long)(my_mret))==((long)((unsigned long)(-(unsigned long)(1UL)))))) {
	(my_die)(((unsigned char *)"out of memory"));
	}
	(my_ret)=((unsigned char*)my_mret);
	return my_ret;
	}
	(my_page)=((my_c)->my_page);
	if (my_page) {
	if ((unsigned long)(((long)(my_size))<=((long)((unsigned long)(((unsigned long)((my_page)->my_size))-((unsigned long)((my_page)->my_fill))))))) {
	(my_mret)=((unsigned long)(((unsigned long)((unsigned long)(my_page)->my_ptr))+((unsigned long)((my_page)->my_fill))));
	((my_page)->my_fill)=((unsigned long)(((unsigned long)((my_page)->my_fill))+((unsigned long)(my_size))));
	(my_ret)=((unsigned char*)my_mret);
	return my_ret;
	}
	}
	(my_psize)=((unsigned long)(((long)(64UL))*((long)(1024UL))));
	(my_mret)=((my_mmap)((0UL),(my_psize),(3UL),(34UL),((unsigned long)(-(unsigned long)(1UL))),(0UL)));
	if ((unsigned long)(((long)(my_mret))==((long)((unsigned long)(-(unsigned long)(1UL)))))) {
	(my_die)(((unsigned char *)"out of memory"));
	}
	(my_page)=((struct my_page*)my_mret);
	((my_page)->my_ptr)=((unsigned char*)&((my_page)[1UL]));
	(my_ret)=((my_page)->my_ptr);
	((my_page)->my_size)=((unsigned long)(((unsigned long)(my_psize))-((unsigned long)(24UL))));
	((my_page)->my_fill)=(my_size);
	((my_c)->my_page)=(my_page);
	return my_ret;
}
unsigned long( my_any)(struct my_peg* my_c){
	unsigned long my_ch = 0;
	(my_ch)=((my_get)((my_c)));
	if ((unsigned long)(((long)(my_ch))==((long)((unsigned long)(-(unsigned long)(1UL)))))) {
	(my_fail)((my_c));
	return 0UL;
	}
	return 1UL;
}
void( my_as_emit)(struct my_assembler* my_a,unsigned long my_b){
	(my_emit)((my_a),(my_b));
}
void( my_as_jmp)(struct my_assembler* my_a,unsigned long my_op,struct my_label* my_l){
	(my_reserve)((my_a),(16UL));
	(my_as_op)((my_a),(my_op));
	(my_as_emit)((my_a),(0UL));
	(my_as_emit)((my_a),(0UL));
	(my_as_emit)((my_a),(0UL));
	(my_as_emit)((my_a),(0UL));
	(my_addfixup)((my_a),(my_l));
}
void( my_as_modm)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_b,unsigned long my_i,unsigned long my_s,unsigned long my_d){
	(my_as_modrm)((my_a),((unsigned long)(((unsigned long)(my_op))&((unsigned long)(65535UL)))),((unsigned long)(((unsigned long)(my_op))>>((unsigned long)(16UL)))),(my_b),(my_i),(my_s),(my_d));
}
void( my_as_modr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_b){
	(my_as_modrr)((my_a),((unsigned long)(((unsigned long)(my_op))&((unsigned long)(65535UL)))),((unsigned long)(((unsigned long)(my_op))>>((unsigned long)(16UL)))),(my_b));
}
void( my_as_modra)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_d){
	(my_as_rex)((my_a),(my_op),(my_r),(0UL),(0UL));
	(my_as_op)((my_a),(my_op));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_r))<<((unsigned long)(3UL)))))&((unsigned long)(56UL)))))+((unsigned long)(my_R_RSP)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_R_RSP))<<((unsigned long)(3UL)))))+((unsigned long)(my_R_RBP)))));
	(my_as_emit)((my_a),(my_d));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(24UL)))));
}
void( my_as_modri)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_x){
	if ((unsigned long)(((unsigned long)(((long)(my_x))<((long)((unsigned long)(-(unsigned long)((unsigned long)(((unsigned long)(1UL))<<((unsigned long)(31UL)))))))))||((unsigned long)(((long)(my_x))>=((long)((unsigned long)(((unsigned long)(1UL))<<((unsigned long)(31UL))))))))) {
	(my_die)(((unsigned char *)"immediate too large"));
	}
	(my_as_modrr)((my_a),((unsigned long)(((unsigned long)(my_op))&((unsigned long)(65535UL)))),((unsigned long)(((unsigned long)(my_op))>>((unsigned long)(16UL)))),(my_r));
	(my_as_emit)((my_a),(my_x));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(24UL)))));
}
void( my_as_modrm)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_b,unsigned long my_i,unsigned long my_s,unsigned long my_d){
	unsigned long my_sib = 0;
	unsigned long my_mod = 0;
	unsigned long my_rm = 0;
	unsigned long my_dw = 0;
	if ((unsigned long)(((unsigned long)(((long)(my_r))<((long)(0UL))))||((unsigned long)(((long)(my_r))>((long)(15UL)))))) {
	(my_die)(((unsigned char *)"invalid reg"));
	}
	(my_rm)=((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_r))<<((unsigned long)(3UL)))))&((unsigned long)(56UL))));
	if ((unsigned long)(((long)(my_d))!=((long)(0UL)))) {
	if ((unsigned long)(((unsigned long)(((long)(my_d))>=((long)((unsigned long)(-(unsigned long)(128UL))))))&&((unsigned long)(((long)(my_d))<=((long)(127UL)))))) {
	(my_mod)=(1UL);
	(my_dw)=(1UL);
	} else {
	(my_mod)=(2UL);
	(my_dw)=(4UL);
	}
	} else {
	(my_mod)=(0UL);
	(my_dw)=(0UL);
	}
	if ((unsigned long)(((long)(my_mod))==((long)(0UL)))) {
	if ((unsigned long)(((unsigned long)(((long)(my_b))<((long)(0UL))))||((unsigned long)(((long)(my_b))>((long)(16UL)))))) {
	(my_die)(((unsigned char *)"invalid reg"));
	}
	if (my_s) {
	if ((unsigned long)(((long)(my_b))==((long)(my_R_RIP)))) {
	(my_die)(((unsigned char *)"invalid base"));
	}
	if ((unsigned long)(((long)(my_i))==((long)(my_R_RSP)))) {
	(my_die)(((unsigned char *)"invalid index"));
	}
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RSP))));
	} else {
	if ((unsigned long)(((long)(my_i))!=((long)(0UL)))) {
	(my_die)(((unsigned char *)"invalid index"));
	}
	if ((unsigned long)(((long)(my_b))==((long)(my_R_RIP)))) {
	(my_mod)=(0UL);
	(my_dw)=(4UL);
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RBP))));
	} else if ((unsigned long)(((unsigned long)(((long)(my_b))==((long)(my_R_RSP))))||((unsigned long)(((long)(my_b))==((long)(my_R_R12)))))) {
	(my_s)=(1UL);
	(my_i)=(my_R_RSP);
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RSP))));
	} else if ((unsigned long)(((unsigned long)(((long)(my_b))==((long)(my_R_RBP))))||((unsigned long)(((long)(my_b))==((long)(my_R_R13)))))) {
	(my_mod)=(1UL);
	(my_dw)=(1UL);
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RBP))));
	} else {
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)((unsigned long)(((unsigned long)(my_b))&((unsigned long)(7UL)))))));
	}
	}
	} else {
	if ((unsigned long)(((unsigned long)(((long)(my_b))<((long)(0UL))))||((unsigned long)(((unsigned long)(((long)(my_b))>((long)(16UL))))||((unsigned long)(((unsigned long)(((long)(my_i))<((long)(0UL))))||((unsigned long)(((long)(my_i))>((long)(15UL)))))))))) {
	(my_die)(((unsigned char *)"invalid reg"));
	}
	if (my_s) {
	if ((unsigned long)(((long)(my_b))==((long)(my_R_RIP)))) {
	(my_die)(((unsigned char *)"invalid base"));
	}
	if ((unsigned long)(((long)(my_i))==((long)(my_R_RSP)))) {
	(my_die)(((unsigned char *)"invalid index"));
	}
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RSP))));
	} else {
	if ((unsigned long)(((long)(my_i))!=((long)(0UL)))) {
	(my_die)(((unsigned char *)"invalid index"));
	}
	if ((unsigned long)(((long)(my_b))==((long)(my_R_RIP)))) {
	(my_mod)=(0UL);
	(my_dw)=(4UL);
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RBP))));
	} else if ((unsigned long)(((unsigned long)(((long)(my_b))==((long)(my_R_RSP))))||((unsigned long)(((long)(my_b))==((long)(my_R_R12)))))) {
	(my_s)=(1UL);
	(my_i)=(my_R_RSP);
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)(my_R_RSP))));
	} else {
	(my_rm)=((unsigned long)(((unsigned long)(my_rm))+((unsigned long)((unsigned long)(((unsigned long)(my_b))&((unsigned long)(7UL)))))));
	}
	}
	}
	(my_as_rex)((my_a),(my_op),(my_r),(my_i),(my_b));
	(my_as_op)((my_a),(my_op));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_mod))<<((unsigned long)(6UL)))))+((unsigned long)(my_rm)))));
	if (my_s) {
	(my_sib)=((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_i))<<((unsigned long)(3UL)))))&((unsigned long)(56UL)))))+((unsigned long)((unsigned long)(((unsigned long)(my_b))&((unsigned long)(7UL)))))));
	if ((unsigned long)(((long)(my_s))==((long)(2UL)))) {
	(my_sib)=((unsigned long)(((unsigned long)(my_sib))+((unsigned long)(64UL))));
	} else if ((unsigned long)(((long)(my_s))==((long)(4UL)))) {
	(my_sib)=((unsigned long)(((unsigned long)(my_sib))+((unsigned long)(128UL))));
	} else if ((unsigned long)(((long)(my_s))==((long)(8UL)))) {
	(my_sib)=((unsigned long)(((unsigned long)(my_sib))+((unsigned long)(192UL))));
	} else if ((unsigned long)(((long)(my_s))!=((long)(1UL)))) {
	(my_die)(((unsigned char *)"invalid scale"));
	}
	(my_as_emit)((my_a),(my_sib));
	}
	if ((unsigned long)(((long)(my_dw))==((long)(1UL)))) {
	(my_as_emit)((my_a),(my_d));
	} else if ((unsigned long)(((long)(my_dw))==((long)(4UL)))) {
	(my_as_emit)((my_a),(my_d));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_d))>>((unsigned long)(24UL)))));
	}
}
void( my_as_modrr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_b){
	if ((unsigned long)(((unsigned long)(((long)(my_r))<((long)(0UL))))||((unsigned long)(((unsigned long)(((long)(my_r))>((long)(15UL))))||((unsigned long)(((unsigned long)(((long)(my_b))<((long)(0UL))))||((unsigned long)(((long)(my_b))>((long)(15UL)))))))))) {
	(my_die)(((unsigned char *)"invalid reg"));
	}
	(my_as_rex)((my_a),(my_op),(my_r),(0UL),(my_b));
	(my_as_op)((my_a),(my_op));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(192UL))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_r))<<((unsigned long)(3UL)))))&((unsigned long)(56UL))))))))+((unsigned long)((unsigned long)(((unsigned long)(my_b))&((unsigned long)(7UL))))))));
}
void( my_as_op)(struct my_assembler* my_a,unsigned long my_op){
	if ((unsigned long)(((long)(my_op))>((long)(255UL)))) {
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_op))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_a),(my_op));
	} else {
	(my_as_emit)((my_a),(my_op));
	}
}
void( my_as_opr)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r){
	if ((unsigned long)(((unsigned long)(((long)(my_r))<((long)(0UL))))||((unsigned long)(((long)(my_r))>((long)(15UL)))))) {
	(my_die)(((unsigned char *)"invalid reg"));
	}
	if ((unsigned long)(((unsigned long)(((long)(my_op))!=((long)(my_OP_PUSHR))))&&((unsigned long)(((unsigned long)(((long)(my_op))!=((long)(my_OP_POPR))))||((unsigned long)(((long)(my_r))>((long)(7UL)))))))) {
	(my_as_rex)((my_a),(my_op),(my_r),(0UL),(0UL));
	}
	(my_as_op)((my_a),((unsigned long)(((unsigned long)(my_op))+((unsigned long)((unsigned long)(((unsigned long)(my_r))&((unsigned long)(7UL))))))));
}
void( my_as_opri64)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_x){
	if ((unsigned long)(((long)(my_op))!=((long)(my_OP_MOVABS)))) {
	(my_die)(((unsigned char *)"only movabs"));
	}
	(my_as_opr)((my_a),(my_op),(my_r));
	(my_as_emit)((my_a),(my_x));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)(my_x))>>((unsigned long)(56UL)))));
}
void( my_as_rex)(struct my_assembler* my_a,unsigned long my_op,unsigned long my_r,unsigned long my_i,unsigned long my_b){
	unsigned long my_w = 0;
	if ((my_a)->my_bits32) {
	return;
	}
	(my_w)=(8UL);
	if ((unsigned long)(((unsigned long)(((long)(my_op))==((long)(my_OP_LOADB))))||((unsigned long)(((long)(my_op))==((long)(my_OP_STOREB)))))) {
	(my_w)=(0UL);
	}
	(my_as_emit)((my_a),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(64UL))+((unsigned long)(my_w)))))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_r))>>((unsigned long)(1UL)))))&((unsigned long)(4UL))))))))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_i))>>((unsigned long)(2UL)))))&((unsigned long)(2UL))))))))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_b))>>((unsigned long)(3UL)))))&((unsigned long)(1UL))))))));
}
void( my_assert)(unsigned long my_x,unsigned char* my_msg){
	if ((unsigned long)(!(my_x))) {
	(my_die)((my_msg));
	}
}
unsigned long( my_call_check)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned long my_result = 0;
	unsigned long my_ret = 0;
	(my_result)=(0UL);
	if ((unsigned long)(!(my_n))) {
	return my_result;
	}
	if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CALL)))) {
	(my_result)=((my_call_check)((my_c),((my_n)->my_a)));
	(my_n)=((my_n)->my_b);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_ret)=((my_call_check)((my_c),((my_n)->my_a)));
	if ((unsigned long)((my_result)&&(my_ret))) {
	(my_cdie)((my_c),((unsigned char *)"multiple calls in call"));
	}
	(my_result)=((unsigned long)(((unsigned long)(my_result))|((unsigned long)(my_ret))));
	(my_n)=((my_n)->my_b);
	}
	(my_result)=(1UL);
	} else if ((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BOR))))||((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BAND)))))) {
	(my_result)=((my_call_check)((my_c),((my_n)->my_a)));
	(my_result)=((unsigned long)(((unsigned long)(my_result))|((unsigned long)((my_call_check)((my_c),((my_n)->my_b))))));
	} else if ((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_ASSIGN))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_INDEX))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LT))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LE))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_GT))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_GE))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_EQ))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NE))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BNOT))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_ADD))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_SUB))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_MUL))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DIV))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_MOD))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LSH))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_RSH))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_AND))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_OR))))||((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_XOR)))))))))))))))))))))))))))))))))))))))) {
	(my_result)=((my_call_check)((my_c),((my_n)->my_a)));
	(my_ret)=((my_call_check)((my_c),((my_n)->my_b)));
	if ((unsigned long)((my_result)&&(my_ret))) {
	(my_cdie)((my_c),((unsigned char *)"multiple calls in expression"));
	}
	(my_result)=((unsigned long)(((unsigned long)(my_result))|((unsigned long)(my_ret))));
	} else if ((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_REF))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DEREF))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_POS))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NEG))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NOT))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CAST))))||((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DOT)))))))))))))))) {
	(my_result)=((my_call_check)((my_c),((my_n)->my_a)));
	} else if ((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_STR))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NUM))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CHAR))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_IDENT))))||((unsigned long)(((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_SIZEOF))))||((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NIL)))))))))))))) {
	} else {
	(my_fdputd)((2UL),((my_n)->my_kind));
	(my_die)(((unsigned char *)"invalid expr"));
	}
	return my_result;
}
void( my_cdie)(struct my_compiler* my_c,unsigned char* my_msg){
	(my_cshow_context)((my_c));
	(my_fdputs)((2UL),((unsigned char *)"cdie: "));
	(my_fdputs)((2UL),(my_msg));
	(my_fdputs)((2UL),((unsigned char *)"\012"));
	(my_exit)((1UL));
}
unsigned long( my_charset)(struct my_peg* my_c,unsigned char* my_s){
	unsigned long my_i = 0;
	unsigned long my_ch = 0;
	(my_ch)=((my_get)((my_c)));
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(!((my_s)[my_i]))) {
	(my_fail)((my_c));
	return 0UL;
	}
	if ((unsigned long)(((long)(my_ch))==((long)((unsigned long)(my_s)[my_i])))) {
	break;
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	return 1UL;
}
void( my_check_usage)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	while (1) {
	(my_d)=((my_c)->my_used_top);
	if ((unsigned long)(!(my_d))) {
	break;
	}
	((my_c)->my_used_top)=((my_d)->my_used_next);
	if ((my_d)->my_func_def) {
	(my_mark_stmt_used)((my_c),(my_d),(((my_d)->my_func_def)->my_b));
	}
	}
}
void( my_choice)(struct my_peg* my_c){
	if ((unsigned long)(((long)((my_c)->my_sp))==((long)((my_c)->my_limit)))) {
	(my_die)(((unsigned char *)"choice overflow"));
	}
	((((my_c)->my_stack)[(my_c)->my_sp]).my_pos)=((my_c)->my_pos);
	((((my_c)->my_stack)[(my_c)->my_sp]).my_depth)=((my_c)->my_depth);
	((((my_c)->my_stack)[(my_c)->my_sp]).my_op)=((my_c)->my_op);
	((((my_c)->my_stack)[(my_c)->my_sp]).my_tag)=((my_c)->my_tag);
	((((my_c)->my_stack)[(my_c)->my_sp]).my_line)=((my_c)->my_line);
	((((my_c)->my_stack)[(my_c)->my_sp]).my_col)=((my_c)->my_col);
	((my_c)->my_sp)=((unsigned long)(((unsigned long)((my_c)->my_sp))+((unsigned long)(1UL))));
}
unsigned long( my_close)(unsigned long my_fd){
	return (my_syscall)((3UL),(my_fd),(0UL),(0UL),(0UL),(0UL),(0UL));
}
void( my_commit)(struct my_peg* my_c){
	if ((unsigned long)(((long)((my_c)->my_sp))==((long)(0UL)))) {
	(my_die)(((unsigned char *)"commit underflow"));
	}
	((my_c)->my_sp)=((unsigned long)(((unsigned long)((my_c)->my_sp))-((unsigned long)(1UL))));
}
struct my_compiler*( my_comp_setup)(struct my_alloc* my_a){
	struct my_compiler* my_c = 0;
	(my_c)=((struct my_compiler*)(my_alloc)((my_a),(96UL)));
	((my_c)->my_a)=(my_a);
	((my_c)->my_p)=((my_setup_parser)((my_a)));
	((my_c)->my_filename)=((void *)0);
	((my_c)->my_lineno)=(0UL);
	((my_c)->my_colno)=(0UL);
	((my_c)->my_s)=((my_setup_assembler)((my_a)));
	((my_c)->my_decls)=((void *)0);
	((my_c)->my_do_cout)=(0UL);
	((my_c)->my_cout)=((void *)0);
	((my_c)->my_start)=((void *)0);
	((my_c)->my_kstart)=((void *)0);
	((my_c)->my_used_top)=((void *)0);
	return my_c;
}
void( my_compile)(struct my_compiler* my_c,struct my_node* my_p){
	struct my_node* my_n = 0;
	struct my_decl* my_d = 0;
	unsigned long my_kind = 0;
	(my_n)=(my_p);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_kind)=(((my_n)->my_a)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STRUCT)))) {
	(my_defstruct)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_UNION)))) {
	(my_defunion)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ENUM)))) {
	(my_defenum)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_N_FUNC))))&&((unsigned long)(((long)(my_kind))!=((long)(my_N_FUNCDECL)))))) {
	(my_cdie)((my_c),((unsigned char *)"invalid decl"));
	}
	(my_n)=((my_n)->my_b);
	}
	(my_n)=(my_p);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_kind)=(((my_n)->my_a)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNCDECL)))) {
	(my_defextern)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNC)))) {
	(my_defun)((my_c),((my_n)->my_a));
	}
	(my_n)=((my_n)->my_b);
	}
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((my_d)->my_struct_defined) {
	(my_layout_struct)((my_c),(my_d));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((my_d)->my_func_defined) {
	(my_typecheck_func)((my_c),(my_d));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_start"),((void *)0),(0UL)));
	if ((unsigned long)((my_d)&&((my_d)->my_func_defined))) {
	((my_c)->my_start)=((my_d)->my_func_label);
	(my_mark_func_used)((my_c),(my_d));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_kstart"),((void *)0),(0UL)));
	if ((unsigned long)((my_d)&&((my_d)->my_func_defined))) {
	((my_c)->my_kstart)=((my_d)->my_func_label);
	(my_mark_func_used)((my_c),(my_d));
	}
	(my_check_usage)((my_c));
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((unsigned long)(((my_d)->my_func_used)&&((my_d)->my_func_defined))) {
	(my_compile_func)((my_c),(my_d));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
}
void( my_compile_expr)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_rhs){
	struct my_label* my_no = 0;
	struct my_label* my_out = 0;
	struct my_decl* my_v = 0;
	unsigned long my_kind = 0;
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STR)))) {
	(my_emit_str)(((my_c)->my_s),((my_n)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NIL)))) {
	(my_emit_num)(((my_c)->my_s),(0UL));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NUM)))) {
	(my_emit_num)(((my_c)->my_s),((my_n)->my_n));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CHAR)))) {
	(my_emit_num)(((my_c)->my_s),((my_n)->my_n));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_EXPRLIST)))) {
	if ((my_n)->my_b) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	}
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CALL)))) {
	if ((my_n)->my_b) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	}
	if ((unsigned long)(((long)(((my_n)->my_a)->my_kind))==((long)(my_N_IDENT)))) {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_enum_defined))) {
	(my_cdie)((my_c),((unsigned char *)"type error"));
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_var_defined))) {
	(my_emit_lea)(((my_c)->my_s),((my_v)->my_var_offset));
	(my_emit_load)(((my_c)->my_s),(((my_n)->my_a)->my_t));
	(my_emit_call)(((my_c)->my_s),((my_count_args)((my_c),((((my_n)->my_a)->my_t)->my_arg))));
	} else if ((unsigned long)(!((my_strcmp)((((my_n)->my_a)->my_s),((unsigned char *)"_include"))))) {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	(my_compile_include)((my_c),(my_n));
	} else {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	(my_emit_lcall)(((my_c)->my_s),((my_v)->my_func_label),((my_count_args)((my_c),((((my_n)->my_a)->my_t)->my_arg))));
	}
	} else {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_call)(((my_c)->my_s),((my_count_args)((my_c),((((my_n)->my_a)->my_t)->my_arg))));
	}
	if ((my_n)->my_b) {
	(my_unify)((my_c),((((my_n)->my_a)->my_t)->my_arg),(((my_n)->my_b)->my_t));
	} else {
	(my_unify)((my_c),((((my_n)->my_a)->my_t)->my_arg),((void *)0));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DOT)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))==((long)(my_TY_PTR)))) {
	(my_v)=((my_find)((my_c),((((((my_n)->my_a)->my_t)->my_val)->my_st)->my_name),(((my_n)->my_b)->my_s),(0UL)));
	(my_emit_load)(((my_c)->my_s),(((my_n)->my_a)->my_t));
	} else {
	(my_v)=((my_find)((my_c),(((((my_n)->my_a)->my_t)->my_st)->my_name),(((my_n)->my_b)->my_s),(0UL)));
	}
	(my_emit_num)(((my_c)->my_s),((my_v)->my_member_offset));
	(my_emit_add)(((my_c)->my_s));
	if (my_rhs) {
	(my_emit_load)(((my_c)->my_s),((my_n)->my_t));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_IDENT)))) {
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_enum_defined))) {
	(my_emit_num)(((my_c)->my_s),((my_v)->my_enum_value));
	return;
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),((my_n)->my_s),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_var_defined))) {
	(my_emit_lea)(((my_c)->my_s),((my_v)->my_var_offset));
	if (my_rhs) {
	(my_emit_load)(((my_c)->my_s),((my_n)->my_t));
	}
	return;
	}
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_func_defined))) {
	(my_emit_ptr)(((my_c)->my_s),((my_v)->my_func_label));
	return;
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ASSIGN)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	(my_emit_store)(((my_c)->my_s),((my_n)->my_t));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_SIZEOF)))) {
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	(my_fixup_label)(((my_c)->my_s),(my_out));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_emit_num)(((my_c)->my_s),(1UL));
	} else {
	(my_emit_num)(((my_c)->my_s),((my_type_sizeof)((my_c),(((my_n)->my_a)->my_t))));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_REF)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DEREF)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if (my_rhs) {
	(my_emit_load)(((my_c)->my_s),((my_n)->my_t));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_INDEX)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	if ((unsigned long)(((long)(((my_n)->my_t)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_emit_num)(((my_c)->my_s),(1UL));
	} else {
	(my_emit_num)(((my_c)->my_s),((my_type_sizeof)((my_c),((my_n)->my_t))));
	}
	(my_emit_mul)(((my_c)->my_s));
	(my_emit_add)(((my_c)->my_s));
	if (my_rhs) {
	(my_emit_load)(((my_c)->my_s),((my_n)->my_t));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LT)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_lt)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GT)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_gt)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LE)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_le)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GE)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_ge)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_EQ)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_eq)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NE)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_ne)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BNOT)))) {
	(my_no)=((my_mklabel)(((my_c)->my_s)));
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(0UL));
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	(my_fixup_label)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(1UL));
	(my_fixup_label)(((my_c)->my_s),(my_out));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BOR)))) {
	(my_no)=((my_mklabel)(((my_c)->my_s)));
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(1UL));
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	(my_fixup_label)(((my_c)->my_s),(my_no));
	(my_no)=((my_mklabel)(((my_c)->my_s)));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(1UL));
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	(my_fixup_label)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(0UL));
	(my_fixup_label)(((my_c)->my_s),(my_out));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BAND)))) {
	(my_no)=((my_mklabel)(((my_c)->my_s)));
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(1UL));
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	(my_fixup_label)(((my_c)->my_s),(my_no));
	(my_emit_num)(((my_c)->my_s),(0UL));
	(my_fixup_label)(((my_c)->my_s),(my_out));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_POS)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NEG)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_neg)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NOT)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_not)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ADD)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_add)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_SUB)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_sub)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_MUL)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_mul)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DIV)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_div)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_MOD)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_mod)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LSH)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_lsh)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RSH)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_rsh)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_AND)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_and)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_OR)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_or)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_XOR)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_emit_xor)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CAST)))) {
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	} else {
	(my_cdie)((my_c),((unsigned char *)"not an expression"));
	}
}
void( my_compile_func)(struct my_compiler* my_c,struct my_decl* my_d){
	unsigned long my_pragma = 0;
	if ((unsigned long)(!((my_d)->my_func_def))) {
	return;
	}
	if ((unsigned long)(!((my_strcmp)(((my_d)->my_name),((unsigned char *)"_start"))))) {
	(my_pragma)=(1UL);
	} else if ((unsigned long)(!((my_strcmp)(((my_d)->my_name),((unsigned char *)"_kstart"))))) {
	(my_pragma)=(2UL);
	} else {
	(my_pragma)=(0UL);
	}
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),((my_d)->my_func_preamble),(my_pragma));
	(my_compile_stmt)((my_c),(my_d),(((my_d)->my_func_def)->my_b),((void *)0),((void *)0));
	(my_emit_num)(((my_c)->my_s),(0UL));
	if (my_pragma) {
	(my_emit_ud)(((my_c)->my_s));
	}
	(my_emit_ret)(((my_c)->my_s));
}
void( my_compile_include)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned char* my_filename = 0;
	unsigned long my_fd = 0;
	unsigned char* my_blob = 0;
	unsigned long my_len = 0;
	if ((unsigned long)(((long)((((my_n)->my_b)->my_a)->my_kind))!=((long)(my_N_STR)))) {
	(my_die)(((unsigned char *)"non literal include"));
	}
	(my_filename)=((((my_n)->my_b)->my_a)->my_s);
	(my_fd)=((my_open)((my_filename),(my_O_RDONLY),(0UL)));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"failed to open include"));
	}
	(my_blob)=((my_readall)((my_fd),(&(my_len)),((my_c)->my_a)));
	(my_close)((my_fd));
	(my_as_opr)(((my_c)->my_s),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_POPR),(my_R_RDI));
	(my_as_opri64)(((my_c)->my_s),(my_OP_MOVABS),(my_R_RAX),(my_len));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_emit_blob)(((my_c)->my_s),(my_blob),(my_len));
	(my_free)(((my_c)->my_a),(my_blob));
}
void( my_compile_stmt)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,struct my_label* my_top,struct my_label* my_out){
	struct my_label* my_no = 0;
	struct my_label* my_ifout = 0;
	struct my_decl* my_v = 0;
	unsigned long my_kind = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	(my_ifout)=((my_mklabel)(((my_c)->my_s)));
	(my_no)=((void *)0);
	while (1) {
	if (my_no) {
	(my_fixup_label)(((my_c)->my_s),(my_no));
	}
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_no)=((my_mklabel)(((my_c)->my_s)));
	if (((my_n)->my_a)->my_a) {
	(my_compile_expr)((my_c),(my_d),(((my_n)->my_a)->my_a),(1UL));
	(my_emit_jz)(((my_c)->my_s),(my_no));
	}
	(my_compile_stmt)((my_c),(my_d),(((my_n)->my_a)->my_b),(my_top),(my_out));
	(my_emit_jmp)(((my_c)->my_s),(my_ifout));
	(my_n)=((my_n)->my_b);
	}
	(my_fixup_label)(((my_c)->my_s),(my_ifout));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_compile_stmt)((my_c),(my_d),((my_n)->my_a),(my_top),(my_out));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	(my_top)=((my_mklabel)(((my_c)->my_s)));
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_fixup_label)(((my_c)->my_s),(my_top));
	(my_compile_stmt)((my_c),(my_d),((my_n)->my_a),(my_top),(my_out));
	(my_emit_jmp)(((my_c)->my_s),(my_top));
	(my_fixup_label)(((my_c)->my_s),(my_out));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BREAK)))) {
	if ((unsigned long)(!(my_out))) {
	(my_cdie)((my_c),((unsigned char *)"break outside loop"));
	}
	(my_emit_jmp)(((my_c)->my_s),(my_out));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONTINUE)))) {
	if ((unsigned long)(!(my_top))) {
	(my_cdie)((my_c),((unsigned char *)"continue outside loop"));
	}
	(my_emit_jmp)(((my_c)->my_s),(my_top));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RETURN)))) {
	if ((my_n)->my_a) {
	if ((unsigned long)(((long)((((my_d)->my_func_type)->my_val)->my_kind))==((long)(my_TY_VOID)))) {
	(my_cdie)((my_c),((unsigned char *)"returning a value in a void function"));
	}
	(my_compile_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	} else {
	if ((unsigned long)(((long)((((my_d)->my_func_type)->my_val)->my_kind))!=((long)(my_TY_VOID)))) {
	(my_cdie)((my_c),((unsigned char *)"returning void in a non void function"));
	}
	(my_emit_num)(((my_c)->my_s),(0UL));
	}
	(my_emit_ret)(((my_c)->my_s));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL)))) {
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	(my_fixup_label)(((my_c)->my_s),((my_v)->my_goto_label));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GOTO)))) {
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_goto_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"label not defined"));
	}
	(my_emit_jmp)(((my_c)->my_s),((my_v)->my_goto_label));
	} else if ((unsigned long)(((long)(my_kind))!=((long)(my_N_VARDECL)))) {
	(my_compile_expr)((my_c),(my_d),(my_n),(1UL));
	(my_emit_pop)(((my_c)->my_s),(1UL));
	}
}
struct my_node*( my_concat_program)(struct my_node* my_a,struct my_node* my_b){
	struct my_node* my_ret = 0;
	if ((unsigned long)(!(my_a))) {
	return my_b;
	}
	if ((unsigned long)(!(my_b))) {
	return my_a;
	}
	(my_ret)=(my_a);
	while (1) {
	if ((unsigned long)(!((my_a)->my_b))) {
	break;
	}
	(my_a)=((my_a)->my_b);
	}
	((my_a)->my_b)=(my_b);
	return my_ret;
}
struct my_peg_node*( my_construct)(struct my_peg* my_c,unsigned long my_sp){
	unsigned long my_i = 0;
	unsigned long my_j = 0;
	unsigned long my_nargs = 0;
	struct my_peg_node* my_n = 0;
	struct my_peg_node** my_link = 0;
	(((my_c)->my_nstack)[0UL])=((void *)0);
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)((my_c)->my_op)))) {
	return ((my_c)->my_nstack)[0UL];
	}
	if ((unsigned long)(((long)((((my_c)->my_out)[my_i]).my_tag))!=((long)(my_sp)))) {
	(my_n)=((struct my_peg_node*)(my_alloc)(((my_c)->my_a),(64UL)));
	((my_n)->my_tag)=((((my_c)->my_out)[my_i]).my_tag);
	((my_n)->my_next)=((void *)0);
	((my_n)->my_child)=((void *)0);
	((my_n)->my_str)=(&(((my_c)->my_src)[(((my_c)->my_out)[my_i]).my_start]));
	((my_n)->my_len)=((unsigned long)(((unsigned long)((((my_c)->my_out)[my_i]).my_end))-((unsigned long)((((my_c)->my_out)[my_i]).my_start))));
	((my_n)->my_filename)=((my_c)->my_filename);
	((my_n)->my_line)=((((my_c)->my_out)[my_i]).my_line);
	((my_n)->my_col)=((((my_c)->my_out)[my_i]).my_col);
	(my_nargs)=((((my_c)->my_out)[my_i]).my_nargs);
	if ((unsigned long)(((long)(my_nargs))>((long)((my_c)->my_np)))) {
	(my_die)(((unsigned char *)"node underflow"));
	}
	(my_link)=(&((my_n)->my_child));
	(my_j)=((unsigned long)(((unsigned long)((my_c)->my_np))-((unsigned long)(my_nargs))));
	while (1) {
	if ((unsigned long)(((long)(my_j))==((long)((my_c)->my_np)))) {
	break;
	}
	if (((my_c)->my_nstack)[my_j]) {
	(*(my_link))=(((my_c)->my_nstack)[my_j]);
	(my_link)=(&((((my_c)->my_nstack)[my_j])->my_next));
	}
	(my_j)=((unsigned long)(((unsigned long)(my_j))+((unsigned long)(1UL))));
	}
	((my_c)->my_np)=((unsigned long)(((unsigned long)((my_c)->my_np))-((unsigned long)(my_nargs))));
	if ((unsigned long)(((long)((my_c)->my_np))==((long)((my_c)->my_ncap)))) {
	(my_die)(((unsigned char *)"node overflow"));
	}
	} else {
	(my_n)=((void *)0);
	}
	(((my_c)->my_nstack)[(my_c)->my_np])=(my_n);
	((my_c)->my_np)=((unsigned long)(((unsigned long)((my_c)->my_np))+((unsigned long)(1UL))));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
void( my_copypos)(struct my_node* my_n,struct my_peg_node* my_pn){
	((my_n)->my_filename)=((my_pn)->my_filename);
	((my_n)->my_lineno)=((my_pn)->my_line);
	((my_n)->my_colno)=((my_pn)->my_col);
}
unsigned long( my_count_args)(struct my_compiler* my_c,struct my_type* my_t){
	unsigned long my_nargs = 0;
	(my_nargs)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_t))) {
	break;
	}
	(my_t)=((my_t)->my_arg);
	(my_nargs)=((unsigned long)(((unsigned long)(my_nargs))+((unsigned long)(1UL))));
	}
	return my_nargs;
}
void( my_cshow_context)(struct my_compiler* my_c){
	(my_fdputs)((2UL),((unsigned char *)"on "));
	if ((my_c)->my_filename) {
	(my_fdputs)((2UL),((my_c)->my_filename));
	}
	(my_fdputs)((2UL),((unsigned char *)":"));
	(my_fdputd)((2UL),((my_c)->my_lineno));
	(my_fdputs)((2UL),((unsigned char *)":"));
	(my_fdputd)((2UL),((my_c)->my_colno));
	(my_fdputs)((2UL),((unsigned char *)"\012"));
}
void( my_ctranslate)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	unsigned long my_seen = 0;
	unsigned long my_has_enum = 0;
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#ifndef my__start\012"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#define my__start main\012"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#endif\012"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#ifndef my_syscall\012"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#define my_syscall syscall\012"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"#endif\012"));
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((my_d)->my_struct_defined) {
	if ((unsigned long)(((long)(((my_d)->my_struct_def)->my_kind))==((long)(my_N_UNION)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"union my_"));
	} else {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"struct my_"));
	}
	(my_fputs)(((my_c)->my_cout),((my_d)->my_name));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((my_d)->my_struct_defined) {
	(my_ctranslate_struct)((my_c),(my_d));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_d)=((my_first_decl)((my_c)));
	(my_has_enum)=(0UL);
	(my_seen)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((my_d)->my_enum_defined) {
	if ((unsigned long)(!(my_has_enum))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"enum {\012"));
	(my_has_enum)=(1UL);
	}
	if (my_seen) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)",\012"));
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011my_"));
	(my_fputs)(((my_c)->my_cout),((my_d)->my_name));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" = "));
	(my_fputd)(((my_c)->my_cout),((my_d)->my_enum_value));
	(my_seen)=(1UL);
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	if (my_has_enum) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\012};\012"));
	}
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((unsigned long)(((my_d)->my_func_used)&&((my_d)->my_func_defined))) {
	(my_ctranslate_type)((my_c),((my_d)->my_func_type),((my_d)->my_name),(1UL),((((my_d)->my_func_decl)->my_b)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_d)=((my_first_decl)((my_c)));
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	if ((unsigned long)(((my_d)->my_func_used)&&((my_d)->my_func_defined))) {
	(my_ctranslate_func)((my_c),(my_d));
	}
	(my_d)=((my_next_decl)((my_c),(my_d)));
	}
	(my_flush_coutput)((my_c));
}
void( my_ctranslate_expr)(struct my_compiler* my_c,struct my_node* my_n){
	if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_STR)))) {
	(my_ctranslate_str)((my_c),((my_n)->my_s));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NUM)))) {
	(my_fputd)(((my_c)->my_cout),((my_n)->my_n));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"UL"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NIL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(void *)0"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CHAR)))) {
	(my_fputd)(((my_c)->my_cout),((my_n)->my_n));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CALL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_n)=((my_n)->my_b);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	(my_n)=((my_n)->my_b);
	if (my_n) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)","));
	}
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DOT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))==((long)(my_TY_PTR)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"->"));
	} else {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"."));
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"my_"));
	(my_fputs)(((my_c)->my_cout),(((my_n)->my_b)->my_s));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_IDENT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"my_"));
	(my_fputs)(((my_c)->my_cout),((my_n)->my_s));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_ASSIGN)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")=("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_SIZEOF)))) {
	(my_fputd)(((my_c)->my_cout),((my_type_sizeof)((my_c),(((my_n)->my_a)->my_t))));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"UL"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_REF)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"&("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DEREF)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"*("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_INDEX)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")["));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"]"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))<((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))<=((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_GT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))>((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_GE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))>=((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_EQ)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))==((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))!=((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BNOT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(!("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BOR)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")||("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_BAND)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned long)(("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")&&("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_POS)))) {
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NEG)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(-(unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NOT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(~(unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_ADD)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))+((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_SUB)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))-((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_MUL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))*((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_DIV)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))/((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_MOD)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))%((long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_LSH)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))<<((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_RSH)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))>>((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_AND)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))&((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_OR)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))|((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_XOR)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")(((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"))^((unsigned long)("));
	(my_ctranslate_expr)((my_c),((my_n)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")))"));
	} else if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_CAST)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	(my_ctranslate_type)((my_c),((my_n)->my_t),((unsigned char *)""),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	} else {
	(my_fdputd)((2UL),((my_n)->my_kind));
	(my_die)(((unsigned char *)"invalid expr"));
	}
}
void( my_ctranslate_func)(struct my_compiler* my_c,struct my_decl* my_d){
	struct my_node* my_n = 0;
	struct my_type* my_ty = 0;
	if ((my_d)->my_func_def) {
	(my_ctranslate_type)((my_c),((my_d)->my_func_type),((my_d)->my_name),(1UL),(((((my_d)->my_func_def)->my_a)->my_b)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"{\012"));
	(my_ctranslate_vars)((my_c),(((my_d)->my_func_def)->my_b));
	(my_ctranslate_stmt)((my_c),(((my_d)->my_func_def)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"}\012"));
	}
}
void( my_ctranslate_stmt)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned long my_kind = 0;
	struct my_node* my_child = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011"));
	while (1) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"if ("));
	(my_ctranslate_expr)((my_c),(((my_n)->my_a)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)") {\012"));
	(my_ctranslate_stmt)((my_c),(((my_n)->my_a)->my_b));
	(my_n)=((my_n)->my_b);
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011}"));
	if ((unsigned long)(!(my_n))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\012"));
	break;
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" else "));
	if ((unsigned long)(!(((my_n)->my_a)->my_a))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"{\012"));
	(my_ctranslate_stmt)((my_c),(((my_n)->my_a)->my_b));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011}\012"));
	break;
	}
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_ctranslate_stmt)((my_c),((my_n)->my_a));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011while (1) {\012"));
	(my_ctranslate_stmt)((my_c),((my_n)->my_a));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011}\012"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BREAK)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011break;\012"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONTINUE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011continue;\012"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RETURN)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011return"));
	if ((my_n)->my_a) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" "));
	(my_ctranslate_expr)((my_c),((my_n)->my_a));
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"my_"));
	(my_fputs)(((my_c)->my_cout),(((my_n)->my_a)->my_s));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)":\012"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GOTO)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011goto "));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"my_"));
	(my_fputs)(((my_c)->my_cout),(((my_n)->my_a)->my_s));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	} else if ((unsigned long)(((long)(my_kind))!=((long)(my_N_VARDECL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011"));
	(my_ctranslate_expr)((my_c),(my_n));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	}
}
void( my_ctranslate_str)(struct my_compiler* my_c,unsigned char* my_s){
	unsigned long my_i = 0;
	unsigned long my_ch = 0;
	(my_i)=(0UL);
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"(unsigned char *)\042"));
	while (1) {
	if ((unsigned long)(!((my_s)[my_i]))) {
	break;
	}
	(my_ch)=((unsigned long)(my_s)[my_i]);
	if ((unsigned long)(((unsigned long)(((long)(my_ch))<((long)(32UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))>((long)(127UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))==((long)(92))))||((unsigned long)(((long)(my_ch))==((long)(34)))))))))) {
	(my_fputc)(((my_c)->my_cout),(92));
	(my_fputc)(((my_c)->my_cout),((unsigned long)(((unsigned long)(48))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_ch))>>((unsigned long)(6UL)))))&((unsigned long)(7UL))))))));
	(my_fputc)(((my_c)->my_cout),((unsigned long)(((unsigned long)(48))+((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_ch))>>((unsigned long)(3UL)))))&((unsigned long)(7UL))))))));
	(my_fputc)(((my_c)->my_cout),((unsigned long)(((unsigned long)(48))+((unsigned long)((unsigned long)(((unsigned long)(my_ch))&((unsigned long)(7UL))))))));
	} else {
	(my_fputc)(((my_c)->my_cout),(my_ch));
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\042"));
}
void( my_ctranslate_struct)(struct my_compiler* my_c,struct my_decl* my_d){
	struct my_decl* my_v = 0;
	struct my_node* my_n = 0;
	if ((unsigned long)(((long)(((my_d)->my_struct_def)->my_kind))==((long)(my_N_UNION)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"union my_"));
	} else {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"struct my_"));
	}
	(my_fputs)(((my_c)->my_cout),((my_d)->my_name));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" {\012"));
	(my_n)=(((my_d)->my_struct_def)->my_b);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),((((my_n)->my_a)->my_a)->my_s),(0UL)));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011"));
	(my_ctranslate_type)((my_c),((my_v)->my_member_type),((((my_n)->my_a)->my_a)->my_s),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	(my_n)=((my_n)->my_b);
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"};\012"));
}
void( my_ctranslate_type)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,unsigned long my_decl,struct my_node* my_args){
	(my_ctranslate_type1)((my_c),(my_ty),(my_name),(my_decl));
	(my_ctranslate_type2)((my_c),(my_ty),(my_name),(my_args));
}
void( my_ctranslate_type1)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,unsigned long my_decl){
	if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_VOID)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"void"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_INT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"unsigned long"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"unsigned char"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_PTR)))) {
	(my_ctranslate_type1)((my_c),((my_ty)->my_val),((unsigned char *)""),(my_decl));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"*"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_FUNC)))) {
	(my_ctranslate_type1)((my_c),((my_ty)->my_val),((unsigned char *)""),(0UL));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"("));
	if ((unsigned long)(!(my_decl))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"*"));
	}
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_STRUCT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"struct my_"));
	(my_fputs)(((my_c)->my_cout),(((my_ty)->my_st)->my_name));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_UNION)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"union my_"));
	(my_fputs)(((my_c)->my_cout),(((my_ty)->my_st)->my_name));
	} else {
	(my_die)(((unsigned char *)"invalid type"));
	}
}
void( my_ctranslate_type2)(struct my_compiler* my_c,struct my_type* my_ty,unsigned char* my_name,struct my_node* my_args){
	struct my_type* my_arg = 0;
	if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_PTR)))) {
	(my_ctranslate_type2)((my_c),((my_ty)->my_val),(my_name),(my_args));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_FUNC)))) {
	(my_ctranslate_type2)((my_c),((my_ty)->my_val),(my_name),(my_args));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")("));
	(my_arg)=((my_ty)->my_arg);
	if (my_arg) {
	while (1) {
	if (my_args) {
	(my_ctranslate_type)((my_c),((my_arg)->my_val),((((my_args)->my_a)->my_a)->my_s),(0UL),((void *)0));
	} else {
	(my_ctranslate_type)((my_c),((my_arg)->my_val),((unsigned char *)""),(0UL),((void *)0));
	}
	(my_arg)=((my_arg)->my_arg);
	if (my_arg) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)","));
	} else {
	break;
	}
	if (my_args) {
	(my_args)=((my_args)->my_b);
	}
	}
	} else {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"void"));
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)")"));
	} else {
	if ((unsigned long)((my_name)&&((my_name)[0UL]))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" my_"));
	(my_fputs)(((my_c)->my_cout),(my_name));
	}
	}
}
void( my_ctranslate_vars)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned long my_kind = 0;
	struct my_node* my_child = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_ctranslate_vars)((my_c),(((my_n)->my_a)->my_b));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_ctranslate_vars)((my_c),((my_n)->my_a));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	(my_ctranslate_vars)((my_c),((my_n)->my_a));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_VARDECL)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"\011"));
	(my_ctranslate_type)((my_c),((my_n)->my_t),(((my_n)->my_a)->my_s),(0UL),((void *)0));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)" = "));
	(my_ctranslate_zero)((my_c),((my_n)->my_t));
	(my_fputs)(((my_c)->my_cout),((unsigned char *)";\012"));
	}
}
void( my_ctranslate_zero)(struct my_compiler* my_c,struct my_type* my_ty){
	struct my_node* my_n = 0;
	struct my_decl* my_v = 0;
	struct my_type* my_arg = 0;
	if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_VOID)))) {
	(my_die)(((unsigned char *)"invalid zero void"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_INT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"0"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"0"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_PTR)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"0"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_FUNC)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"0"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_STRUCT)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"{"));
	(my_n)=((((my_ty)->my_st)->my_struct_def)->my_b);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_v)=((my_find)((my_c),(((my_ty)->my_st)->my_name),((((my_n)->my_a)->my_a)->my_s),(0UL)));
	(my_ctranslate_zero)((my_c),((my_v)->my_member_type));
	(my_n)=((my_n)->my_b);
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"}"));
	} else if ((unsigned long)(((long)((my_ty)->my_kind))==((long)(my_TY_UNION)))) {
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"{"));
	(my_n)=((((my_ty)->my_st)->my_struct_def)->my_b);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_v)=((my_find)((my_c),(((my_ty)->my_st)->my_name),((((my_n)->my_a)->my_a)->my_s),(0UL)));
	(my_ctranslate_zero)((my_c),((my_v)->my_member_type));
	(my_n)=((my_n)->my_b);
	}
	(my_fputs)(((my_c)->my_cout),((unsigned char *)"}"));
	} else {
	(my_die)(((unsigned char *)"invalid type"));
	}
}
unsigned long( my_dec2int)(unsigned char* my_s,unsigned long my_len,unsigned long* my_ok){
	unsigned long my_x = 0;
	unsigned long my_d = 0;
	unsigned long my_i = 0;
	(my_x)=(0UL);
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(my_d)=((unsigned long)(my_s)[my_i]);
	if ((unsigned long)(((long)(my_d))==((long)(95)))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	continue;
	} else if ((unsigned long)(((unsigned long)(((long)(my_d))>=((long)(48))))&&((unsigned long)(((long)(my_d))<=((long)(57)))))) {
	(my_d)=((unsigned long)(((unsigned long)(my_d))-((unsigned long)(48))));
	} else {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	if ((unsigned long)(((long)(my_x))>((long)(461168601842738790UL)))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_x)=((unsigned long)(((long)(my_x))*((long)(10UL))));
	if ((unsigned long)(((long)(my_x))>((long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(1UL))<<((unsigned long)(63UL)))))-((unsigned long)(1UL)))))-((unsigned long)(my_d))))))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_x)=((unsigned long)(((unsigned long)(my_x))+((unsigned long)(my_d))));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(*(my_ok))=(1UL);
	return my_x;
}
unsigned long( my_decode_count)(struct my_peg_node* my_n){
	unsigned long my_ret = 0;
	(my_ret)=(my_EXACTLY_ONE);
	(my_n)=((my_n)->my_child);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	return my_ret;
	}
	if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_countop)))) {
	if ((unsigned long)(((long)(((my_n)->my_str)[0UL]))==((long)((unsigned char)63)))) {
	if ((unsigned long)(((long)(my_ret))==((long)(my_EXACTLY_ONE)))) {
	(my_ret)=(my_ZERO_OR_ONE);
	} else if ((unsigned long)(((long)(my_ret))==((long)(my_ONE_OR_MORE)))) {
	(my_ret)=(my_ZERO_OR_MORE);
	}
	} else if ((unsigned long)(((long)(((my_n)->my_str)[0UL]))==((long)((unsigned char)42)))) {
	(my_ret)=(my_ZERO_OR_MORE);
	} else if ((unsigned long)(((long)(((my_n)->my_str)[0UL]))==((long)((unsigned char)43)))) {
	if ((unsigned long)(((long)(my_ret))==((long)(my_ZERO_OR_ONE)))) {
	(my_ret)=(my_ZERO_OR_MORE);
	} else if ((unsigned long)(((long)(my_ret))==((long)(my_EXACTLY_ONE)))) {
	(my_ret)=(my_ONE_OR_MORE);
	} else if ((unsigned long)(((long)(my_ret))==((long)(my_ZERO_OR_MORE)))) {
	(my_ret)=(my_ZERO_OR_MORE);
	}
	} else {
	(my_die)(((unsigned char *)"invalid countop"));
	}
	}
	(my_n)=((my_n)->my_next);
	}
}
unsigned long( my_decode_look)(struct my_peg_node* my_n){
	unsigned long my_ret = 0;
	(my_ret)=(my_LOOK_NORMAL);
	if ((unsigned long)(((long)(((my_n)->my_child)->my_tag))==((long)(my_PEG_lookop)))) {
	if ((unsigned long)(((long)((((my_n)->my_child)->my_str)[0UL]))==((long)((unsigned char)33)))) {
	(my_ret)=(my_LOOK_NOT);
	} else if ((unsigned long)(((long)((((my_n)->my_child)->my_str)[0UL]))==((long)((unsigned char)38)))) {
	(my_ret)=(my_LOOK_AND);
	}
	}
	return my_ret;
}
void( my_defenum)(struct my_compiler* my_c,struct my_node* my_n){
	struct my_decl* my_d = 0;
	unsigned long my_i = 0;
	unsigned char* my_name = 0;
	(my_n)=((my_n)->my_b);
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	((my_c)->my_filename)=(((my_n)->my_a)->my_filename);
	((my_c)->my_lineno)=(((my_n)->my_a)->my_lineno);
	((my_c)->my_colno)=(((my_n)->my_a)->my_colno);
	(my_name)=((((my_n)->my_a)->my_a)->my_s);
	(my_d)=((my_find)((my_c),(my_name),((void *)0),(1UL)));
	if ((my_d)->my_enum_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate enum"));
	}
	if (((my_n)->my_a)->my_b) {
	(my_i)=((((my_n)->my_a)->my_b)->my_n);
	}
	((my_d)->my_enum_defined)=(1UL);
	((my_d)->my_enum_value)=(my_i);
	((my_d)->my_enum_def)=(my_n);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	(my_n)=((my_n)->my_b);
	}
}
struct my_decl*( my_defextern)(struct my_compiler* my_c,struct my_node* my_n){
	struct my_decl* my_d = 0;
	unsigned char* my_name = 0;
	struct my_type* my_t = 0;
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_name)=(((my_n)->my_a)->my_s);
	(my_t)=((my_prototype)((my_c),((my_n)->my_b)));
	(my_d)=((my_find)((my_c),(my_name),((void *)0),(1UL)));
	if ((my_d)->my_func_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate function"));
	}
	((my_d)->my_func_defined)=(1UL);
	((my_d)->my_func_type)=(my_t);
	((my_d)->my_func_decl)=(my_n);
	return my_d;
}
void( my_defstruct)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned char* my_name = 0;
	struct my_decl* my_d = 0;
	(my_name)=(((my_n)->my_a)->my_s);
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	if ((unsigned long)(((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"int")))))||((unsigned long)(((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"byte")))))||((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"func"))))))))) {
	(my_cdie)((my_c),((unsigned char *)"reserved word"));
	}
	(my_d)=((my_find)((my_c),(my_name),((void *)0),(1UL)));
	if ((my_d)->my_struct_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate struct"));
	}
	((my_d)->my_struct_defined)=(1UL);
	((my_d)->my_struct_def)=(my_n);
}
void( my_defun)(struct my_compiler* my_c,struct my_node* my_n){
	struct my_decl* my_d = 0;
	unsigned char* my_name = 0;
	struct my_decl* my_v = 0;
	struct my_type* my_t = 0;
	unsigned long my_offset = 0;
	(my_d)=((my_defextern)((my_c),((my_n)->my_a)));
	((my_d)->my_func_def)=(my_n);
	(my_n)=((((my_n)->my_a)->my_b)->my_a);
	(my_offset)=(16UL);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	((my_c)->my_filename)=(((my_n)->my_a)->my_filename);
	((my_c)->my_lineno)=(((my_n)->my_a)->my_lineno);
	((my_c)->my_colno)=(((my_n)->my_a)->my_colno);
	(my_name)=((((my_n)->my_a)->my_a)->my_s);
	(my_t)=((my_prototype)((my_c),(((my_n)->my_a)->my_b)));
	(my_v)=((my_find)((my_c),((my_d)->my_name),(my_name),(1UL)));
	if ((my_v)->my_var_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate argument"));
	}
	((my_v)->my_var_defined)=(1UL);
	((my_v)->my_var_type)=(my_t);
	((my_v)->my_var_offset)=(my_offset);
	((my_v)->my_var_def)=((my_n)->my_a);
	(my_offset)=((unsigned long)(((unsigned long)(my_offset))+((unsigned long)(8UL))));
	(my_n)=((my_n)->my_b);
	}
	(my_offset)=((my_hoist_locals)((my_c),(my_d),(((my_d)->my_func_def)->my_b),(0UL)));
	((my_d)->my_func_preamble)=(my_offset);
}
void( my_defunion)(struct my_compiler* my_c,struct my_node* my_n){
	unsigned char* my_name = 0;
	struct my_decl* my_d = 0;
	(my_name)=(((my_n)->my_a)->my_s);
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	if ((unsigned long)(((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"int")))))||((unsigned long)(((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"byte")))))||((unsigned long)(!((my_strcmp)((my_name),((unsigned char *)"func"))))))))) {
	(my_cdie)((my_c),((unsigned char *)"reserved word"));
	}
	(my_d)=((my_find)((my_c),(my_name),((void *)0),(1UL)));
	if ((my_d)->my_struct_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate struct"));
	}
	((my_d)->my_struct_defined)=(1UL);
	((my_d)->my_struct_def)=(my_n);
}
void( my_die)(unsigned char* my_msg){
	(my_fdputs)((2UL),(my_msg));
	(my_fdputs)((2UL),((unsigned char *)"\012"));
	(my_exit)((1UL));
}
void( my_emit)(struct my_assembler* my_c,unsigned long my_x){
	(my_reserve)((my_c),(1UL));
	((((my_c)->my_text_end)->my_buf)[((my_c)->my_text_end)->my_fill])=((unsigned char)my_x);
	(((my_c)->my_text_end)->my_fill)=((unsigned long)(((unsigned long)(((my_c)->my_text_end)->my_fill))+((unsigned long)(1UL))));
	((my_c)->my_at)=((unsigned long)(((unsigned long)((my_c)->my_at))+((unsigned long)(1UL))));
}
void( my_emit_add)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_ADDRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_align)(struct my_assembler* my_c,unsigned long my_n,unsigned long my_b){
	unsigned long my_pad = 0;
	(my_pad)=((unsigned long)(((unsigned long)((my_c)->my_at))&((unsigned long)((unsigned long)(((unsigned long)(my_n))-((unsigned long)(1UL)))))));
	if ((unsigned long)(((long)(my_pad))==((long)(0UL)))) {
	return;
	}
	while (1) {
	if ((unsigned long)(((long)(my_pad))==((long)(my_n)))) {
	break;
	}
	(my_as_emit)((my_c),(my_b));
	(my_pad)=((unsigned long)(((unsigned long)(my_pad))+((unsigned long)(1UL))));
	}
}
void( my_emit_and)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_ANDRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_blob)(struct my_assembler* my_c,unsigned char* my_s,unsigned long my_n){
	struct my_label* my_a = 0;
	struct my_label* my_b = 0;
	unsigned long my_i = 0;
	(my_a)=((my_mklabel)((my_c)));
	(my_b)=((my_mklabel)((my_c)));
	(my_as_jmp)((my_c),(my_OP_JMP),(my_b));
	(my_fixup_label)((my_c),(my_a));
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_n)))) {
	break;
	}
	(my_as_emit)((my_c),((unsigned long)(my_s)[my_i]));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_as_emit)((my_c),(0UL));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_fixup_label)((my_c),(my_b));
	(my_emit_ptr)((my_c),(my_a));
}
void( my_emit_builtin)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	(my_d)=((my_find)((my_c),((unsigned char *)"syscall"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_emit_syscall)(((my_c)->my_s));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_restorer"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_restorer)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_include"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_UD2));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"ud2"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_UD2));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"cpuid"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_CPUID));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RCX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"inb"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_op)(((my_c)->my_s),(my_OP_IN));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"outb"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_op)(((my_c)->my_s),(my_OP_OUT));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"inw"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_emit)(((my_c)->my_s),(my_OP_OS));
	(my_as_op)(((my_c)->my_s),(my_OP_IND));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"outw"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_emit)(((my_c)->my_s),(my_OP_OS));
	(my_as_op)(((my_c)->my_s),(my_OP_OUTD));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"ind"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_op)(((my_c)->my_s),(my_OP_IND));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"outd"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_op)(((my_c)->my_s),(my_OP_OUTD));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdmsr"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_op)(((my_c)->my_s),(my_OP_RDMSR));
	(my_as_modri)(((my_c)->my_s),(my_OP_MOVI),(my_R_RCX),(32UL));
	(my_as_modr)(((my_c)->my_s),(my_OP_SHLM),(my_R_RDX));
	(my_as_modrr)(((my_c)->my_s),(my_OP_ORRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrmsr"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_MOVE),(my_R_RDX),(my_R_RAX));
	(my_as_modri)(((my_c)->my_s),(my_OP_MOVI),(my_R_RCX),(32UL));
	(my_as_modr)(((my_c)->my_s),(my_OP_SHRM),(my_R_RDX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_op)(((my_c)->my_s),(my_OP_WRMSR));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdcr0"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDCRR),(my_R_CR0),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrcr0"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRCRR),(my_R_CR0),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdcr2"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDCRR),(my_R_CR2),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrcr2"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRCRR),(my_R_CR2),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdcr3"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDCRR),(my_R_CR3),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrcr3"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRCRR),(my_R_CR3),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdcr4"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDCRR),(my_R_CR4),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrcr4"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRCRR),(my_R_CR4),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"lgdt"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_SUBI),(my_R_RSP),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_SUBI),(my_R_RAX),(1UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(2UL));
	(my_as_modm)(((my_c)->my_s),(my_OP_LGDTM),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_ADDI),(my_R_RSP),(16UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"lidt"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_SUBI),(my_R_RSP),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_SUBI),(my_R_RAX),(1UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(2UL));
	(my_as_modm)(((my_c)->my_s),(my_OP_LIDTM),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_ADDI),(my_R_RSP),(16UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"lldt"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modr)(((my_c)->my_s),(my_OP_LLDTM),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"ltr"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modr)(((my_c)->my_s),(my_OP_LTRM),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"lseg"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRSR),(my_R_ES),(my_R_RAX));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRSR),(my_R_DS),(my_R_RAX));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRSR),(my_R_FS),(my_R_RAX));
	(my_as_modrr)(((my_c)->my_s),(my_OP_WRSR),(my_R_GS),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RBP));
	(my_as_op)(((my_c)->my_s),(my_OP_PUSHF));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_op)(((my_c)->my_s),(my_OP_CALL));
	(my_as_emit)(((my_c)->my_s),(5UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_JMP));
	(my_as_emit)(((my_c)->my_s),(2UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_IRETQ));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"hlt"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_HLT));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"cli"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_CLI));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"sti"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_STI));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"rdflags"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_PUSHF));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wrflags"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_PUSHF));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_op)(((my_c)->my_s),(my_OP_POPF));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"wbinvld"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modm)(((my_c)->my_s),(my_OP_WBINVD),(my_R_RAX),(0UL),(0UL),(0UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"invlpg"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modm)(((my_c)->my_s),(my_OP_INVLPGM),(my_R_RAX),(0UL),(0UL),(0UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_ssr0"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_ssr)((my_c));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_isr0"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_isr)((my_c));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_rgs"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_emit)(((my_c)->my_s),(my_OP_GS));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSI),(0UL),(0UL),(0UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_r32"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(16UL));
	(((my_c)->my_s)->my_bits32)=(1UL);
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSI),(0UL),(0UL),(0UL));
	(((my_c)->my_s)->my_bits32)=(0UL);
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_w32"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(((my_c)->my_s)->my_bits32)=(1UL);
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(((my_c)->my_s)->my_bits32)=(0UL);
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_r16"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(((my_c)->my_s)->my_bits32)=(1UL);
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD16),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(((my_c)->my_s)->my_bits32)=(0UL);
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_w16"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(((my_c)->my_s)->my_bits32)=(1UL);
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE16),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(((my_c)->my_s)->my_bits32)=(0UL);
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"_rdrand"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_emit_preamble)(((my_c)->my_s),(0UL),(0UL));
	(my_as_modr)(((my_c)->my_s),(my_OP_RDRAND),(my_R_RAX));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_emit_ret)(((my_c)->my_s));
	}
	(my_d)=((my_find)((my_c),((unsigned char *)"taskswitch"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&((unsigned long)(!(((my_d)->my_func_label)->my_fixed))))) {
	(my_fixup_label)(((my_c)->my_s),((my_d)->my_func_label));
	(my_add_symbol)(((my_c)->my_s),((my_d)->my_name),((my_d)->my_func_label));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RBP));
	(my_as_op)(((my_c)->my_s),(my_OP_PUSHF));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBP),(my_R_RSP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LEA),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R8),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R9),(my_R_RBP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R10),(my_R_RBP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R11),(my_R_RBP),(0UL),(0UL),(88UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R12),(my_R_RBP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R13),(my_R_RBP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R14),(my_R_RBP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R15),(my_R_RBP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(128UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(136UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDSR),(my_R_CS),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(144UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_RDSR),(my_R_SS),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(152UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBP),(my_R_RSP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R8),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R9),(my_R_RBP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R10),(my_R_RBP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R11),(my_R_RBP),(0UL),(0UL),(88UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R12),(my_R_RBP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R13),(my_R_RBP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R14),(my_R_RBP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R15),(my_R_RBP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(152UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(136UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(144UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(128UL));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBP),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_op)(((my_c)->my_s),(my_OP_IRETQ));
	}
}
void( my_emit_call)(struct my_assembler* my_c,unsigned long my_n){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modr)((my_c),(my_OP_ICALLM),(my_R_RAX));
	(my_emit_pop)((my_c),(my_n));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_div)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RDX),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_TESTRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_S)))),(0UL),(my_R_RDX));
	(my_as_modr)((my_c),(my_OP_NEGM),(my_R_RDX));
	(my_as_modr)((my_c),(my_OP_IDIVM),(my_R_RCX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_eq)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_E)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_ge)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_GE)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_gt)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_G)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_isr)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	struct my_label* my_out = 0;
	unsigned long my_i = 0;
	(my_out)=((my_mklabel)(((my_c)->my_s)));
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(256UL)))) {
	break;
	}
	(my_reserve)(((my_c)->my_s),(16UL));
	if ((unsigned long)(((unsigned long)(((long)(my_i))==((long)(8UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(10UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(11UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(12UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(13UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(14UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(17UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(21UL))))||((unsigned long)(((unsigned long)(((long)(my_i))==((long)(29UL))))||((unsigned long)(((long)(my_i))==((long)(30UL)))))))))))))))))))))) {
	(my_as_emit)(((my_c)->my_s),(144UL));
	(my_as_emit)(((my_c)->my_s),(144UL));
	} else {
	(my_as_emit)(((my_c)->my_s),(106UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	}
	(my_as_emit)(((my_c)->my_s),(104UL));
	(my_as_emit)(((my_c)->my_s),(my_i));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(233UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_as_emit)(((my_c)->my_s),(0UL));
	(my_addfixup)(((my_c)->my_s),(my_out));
	(my_as_emit)(((my_c)->my_s),(144UL));
	(my_as_emit)(((my_c)->my_s),(144UL));
	(my_as_emit)(((my_c)->my_s),(144UL));
	(my_as_emit)(((my_c)->my_s),(144UL));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_fixup_label)(((my_c)->my_s),(my_out));
	(my_as_modri)(((my_c)->my_s),(my_OP_SUBI),(my_R_RSP),(176UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBP),(my_R_RSP),(0UL),(0UL),(40UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_MOVE),(my_R_RBP),(my_R_RSP));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RBP));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R8),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R9),(my_R_RBP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R10),(my_R_RBP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R11),(my_R_RBP),(0UL),(0UL),(88UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R12),(my_R_RBP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R13),(my_R_RBP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R14),(my_R_RBP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R15),(my_R_RBP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(0UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(160UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(8UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(168UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(16UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(128UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(24UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(144UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(32UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(136UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(40UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(48UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(152UL));
	(my_d)=((my_find)((my_c),((unsigned char *)"_isr"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&(((my_d)->my_func_label)->my_fixed))) {
	(my_as_jmp)(((my_c)->my_s),(my_OP_CALL),((my_d)->my_func_label));
	}
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(128UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(16UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(144UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(24UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(136UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(32UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(40UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(152UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),((unsigned long)(((unsigned long)(176UL))+((unsigned long)(48UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R8),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R9),(my_R_RBP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R10),(my_R_RBP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R11),(my_R_RBP),(0UL),(0UL),(88UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R12),(my_R_RBP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R13),(my_R_RBP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R14),(my_R_RBP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R15),(my_R_RBP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBP),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_ADDI),(my_R_RSP),((unsigned long)(((unsigned long)(176UL))+((unsigned long)((unsigned long)(((long)(3UL))*((long)(8UL))))))));
	(my_as_op)(((my_c)->my_s),(my_OP_IRETQ));
}
void( my_emit_jmp)(struct my_assembler* my_c,struct my_label* my_l){
	(my_as_jmp)((my_c),(my_OP_JMP),(my_l));
}
void( my_emit_jz)(struct my_assembler* my_c,struct my_label* my_l){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_TESTRM),(my_R_RAX),(my_R_RAX));
	(my_as_jmp)((my_c),((unsigned long)(((unsigned long)(my_OP_JCC))+((unsigned long)(my_CC_E)))),(my_l));
}
void( my_emit_kstart)(struct my_assembler* my_c){
	struct my_label* my_hang = 0;
	struct my_label* my_do_iret = 0;
	struct my_label* my_do_ret = 0;
	struct my_label* my_done = 0;
	((my_c)->my_bits32)=(1UL);
	(my_hang)=((my_mklabel)((my_c)));
	(my_do_iret)=((my_mklabel)((my_c)));
	(my_do_ret)=((my_mklabel)((my_c)));
	(my_done)=((my_mklabel)((my_c)));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RDX),(732803074UL));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RAX),(my_R_RDX));
	(my_as_jmp)((my_c),((unsigned long)(((unsigned long)(my_OP_JCC))+((unsigned long)(my_CC_NE)))),(my_hang));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RSP),(3145728UL));
	(my_as_modri)((my_c),(my_OP_ANDI),(my_R_RSP),((unsigned long)(-(unsigned long)(4096UL))));
	(my_as_modri)((my_c),(my_OP_SUBI),(my_R_RSP),(4096UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(131UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RDX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RDX),(my_R_RSP),(0UL),(0UL),(4UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),((unsigned long)(((unsigned long)((unsigned long)(((long)(510UL))*((long)(8UL)))))+((unsigned long)(0UL)))));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RDX),(my_R_RSP),(0UL),(0UL),((unsigned long)(((unsigned long)((unsigned long)(((long)(510UL))*((long)(8UL)))))+((unsigned long)(4UL)))));
	(my_as_modrr)((my_c),(my_OP_MOVE),(my_R_RAX),(my_R_RSP));
	(my_as_modri)((my_c),(my_OP_SUBI),(my_R_RSP),(4096UL));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RAX),(3UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RDX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RDX),(my_R_RSP),(0UL),(0UL),(4UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),((unsigned long)(((unsigned long)((unsigned long)(((long)(511UL))*((long)(8UL)))))+((unsigned long)(0UL)))));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RDX),(my_R_RSP),(0UL),(0UL),((unsigned long)(((unsigned long)((unsigned long)(((long)(511UL))*((long)(8UL)))))+((unsigned long)(4UL)))));
	(my_as_modrr)((my_c),(my_OP_WRCRR),(my_R_CR3),(my_R_RSP));
	(my_as_modri)((my_c),(my_OP_SUBI),(my_R_RSP),(64UL));
	(my_as_modrr)((my_c),(my_OP_MOVE),(my_R_RBP),(my_R_RSP));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(12UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(2136064UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(20UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(37376UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(28UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(23UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(2UL));
	(my_as_modm)((my_c),(my_OP_LGDTM),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modr)((my_c),(my_OP_LLDTM),(my_R_RAX));
	(my_as_modm)((my_c),(my_OP_LIDTM),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(160UL));
	(my_as_modrr)((my_c),(my_OP_WRCRR),(my_R_CR4),(my_R_RAX));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RCX),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(-(unsigned long)(1UL))))<<((unsigned long)(32UL)))))+((unsigned long)((unsigned long)(((unsigned long)(192UL))<<((unsigned long)(24UL))))))))+((unsigned long)(128UL)))));
	(my_as_op)((my_c),(my_OP_RDMSR));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RAX),(256UL));
	(my_as_op)((my_c),(my_OP_WRMSR));
	(my_as_modrr)((my_c),(my_OP_RDCRR),(my_R_CR0),(my_R_RAX));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RAX),((unsigned long)(((unsigned long)((unsigned long)(-(unsigned long)(2147483648UL))))|((unsigned long)(1UL)))));
	(my_as_modrr)((my_c),(my_OP_WRCRR),(my_R_CR0),(my_R_RAX));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(8UL));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
	(my_as_jmp)((my_c),(my_OP_CALL),(my_do_iret));
	((my_c)->my_bits32)=(0UL);
	(my_as_jmp)((my_c),(my_OP_CALL),(my_do_ret));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RBP),((unsigned long)(-(unsigned long)(2147483648UL))));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(23UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(8UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(2UL));
	(my_as_modm)((my_c),(my_OP_LGDTM),(my_R_RBP),(0UL),(0UL),(0UL));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(16UL));
	(my_as_modrr)((my_c),(my_OP_WRSR),(my_R_ES),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_WRSR),(my_R_DS),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_WRSR),(my_R_FS),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_WRSR),(my_R_GS),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_WRSR),(my_R_SS),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_MOVE),(my_R_RSP),(my_R_RSP));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RSP),((unsigned long)(-(unsigned long)(2147483648UL))));
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(0UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(4096UL));
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	(my_as_modri)((my_c),(my_OP_ANDI),(my_R_RDI),(2147483647UL));
	(my_as_modrr)((my_c),(my_OP_WRCRR),(my_R_CR3),(my_R_RDI));
	(my_as_jmp)((my_c),(my_OP_JMP),(my_done));
	(my_fixup_label)((my_c),(my_hang));
	(my_as_op)((my_c),(my_OP_CLI));
	(my_as_op)((my_c),(my_OP_HLT));
	(my_as_jmp)((my_c),(my_OP_JMP),(my_hang));
	(my_fixup_label)((my_c),(my_do_iret));
	(my_as_op)((my_c),(my_OP_IRET));
	(my_fixup_label)((my_c),(my_do_ret));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modri)((my_c),(my_OP_ORI),(my_R_RAX),((unsigned long)(-(unsigned long)(2147483648UL))));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
	(my_as_op)((my_c),(my_OP_RET));
	(my_fixup_label)((my_c),(my_done));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RBP),(my_R_RBP));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RBX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RBP));
}
void( my_emit_lcall)(struct my_assembler* my_c,struct my_label* my_l,unsigned long my_n){
	(my_as_jmp)((my_c),(my_OP_CALL),(my_l));
	(my_emit_pop)((my_c),(my_n));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_le)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_LE)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_lea)(struct my_assembler* my_c,unsigned long my_offset){
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(my_offset));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_load)(struct my_assembler* my_c,struct my_type* my_t){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDI));
	if ((unsigned long)(((long)((my_t)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrm)((my_c),(my_OP_LOADB),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	} else if ((my_type_isprim)((my_t))) {
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	} else {
	(my_die)(((unsigned char *)"invalid load"));
	}
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_lsh)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modr)((my_c),(my_OP_SHLM),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_lt)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_L)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_mod)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RDX),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_TESTRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_S)))),(0UL),(my_R_RDX));
	(my_as_modr)((my_c),(my_OP_NEGM),(my_R_RDX));
	(my_as_modr)((my_c),(my_OP_IDIVM),(my_R_RCX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RDX));
}
void( my_emit_mul)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modr)((my_c),(my_OP_IMULM),(my_R_RCX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_ne)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_CMPRM),(my_R_RDX),(my_R_RCX));
	(my_as_modrr)((my_c),((unsigned long)(((unsigned long)(my_OP_SETCC))+((unsigned long)(my_CC_NE)))),(0UL),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_neg)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modr)((my_c),(my_OP_NEGM),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_not)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modr)((my_c),(my_OP_NOTM),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_num)(struct my_assembler* my_c,unsigned long my_x){
	(my_as_opri64)((my_c),(my_OP_MOVABS),(my_R_RDX),(my_x));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RDX));
}
void( my_emit_or)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_ORRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_pop)(struct my_assembler* my_c,unsigned long my_n){
	(my_as_modri)((my_c),(my_OP_ADDI),(my_R_RSP),((unsigned long)(((unsigned long)(my_n))<<((unsigned long)(3UL)))));
}
void( my_emit_preamble)(struct my_assembler* my_c,unsigned long my_n,unsigned long my_pragma){
	unsigned long my_i = 0;
	if ((unsigned long)(((long)(my_pragma))==((long)(1UL)))) {
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RBP),(my_R_RBP));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RDI),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RSI),(my_R_RSP),(0UL),(0UL),(8UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RDX),(my_R_RSI),(my_R_RDI),(8UL),(8UL));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RSI));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RDI));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RBP));
	} else if ((unsigned long)(((long)(my_pragma))>((long)(1UL)))) {
	(my_emit_kstart)((my_c));
	}
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RBP));
	(my_as_modrr)((my_c),(my_OP_MOVE),(my_R_RBP),(my_R_RSP));
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))>=((long)(my_n)))) {
	break;
	}
	(my_emit_num)((my_c),(0UL));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(8UL))));
	}
}
void( my_emit_ptr)(struct my_assembler* my_c,struct my_label* my_l){
	(my_reserve)((my_c),(16UL));
	(my_as_modrm)((my_c),(my_OP_LEA),(my_R_RAX),(my_R_RIP),(0UL),(0UL),(128UL));
	(my_addfixup)((my_c),(my_l));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_restorer)(struct my_assembler* my_c){
	(my_as_modri)((my_c),(my_OP_MOVI),(my_R_RAX),(15UL));
	(my_as_op)((my_c),(my_OP_SYSCALL));
}
void( my_emit_ret)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_modrr)((my_c),(my_OP_MOVE),(my_R_RSP),(my_R_RBP));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RBP));
	(my_as_op)((my_c),(my_OP_RET));
}
void( my_emit_rsh)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RCX));
	(my_as_modr)((my_c),(my_OP_SHRM),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
unsigned long( my_emit_sections)(struct my_assembler* my_c){
	unsigned long my_at = 0;
	struct my_section* my_s = 0;
	struct my_symbol* my_y = 0;
	unsigned long my_n = 0;
	(my_add_symbol)((my_c),((unsigned char *)""),((void *)0));
	(my_s)=((my_find_section)((my_c),((unsigned char *)"")));
	((my_s)->my_start)=(0UL);
	((my_s)->my_end)=(0UL);
	(my_add_section)((my_c),((unsigned char *)".strtab"),(my_SHT_STRTAB));
	(my_y)=((my_c)->my_symbols);
	while (1) {
	if ((unsigned long)(!(my_y))) {
	break;
	}
	((my_y)->my_name_offset)=(my_n);
	(my_n)=((unsigned long)(((unsigned long)(my_n))+((unsigned long)((my_emit_strtab_str)((my_c),((my_y)->my_name))))));
	(my_y)=((my_y)->my_next);
	}
	(my_add_section)((my_c),((unsigned char *)".symtab"),(my_SHT_SYMTAB));
	(my_y)=((my_c)->my_symbols);
	while (1) {
	if ((unsigned long)(!(my_y))) {
	break;
	}
	(my_n)=((my_y)->my_name_offset);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	if ((my_y)->my_label) {
	(my_n)=(18UL);
	} else {
	(my_n)=(0UL);
	}
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),(0UL));
	if ((my_y)->my_label) {
	(my_n)=(1UL);
	} else {
	(my_n)=(0UL);
	}
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	if ((my_y)->my_label) {
	(my_n)=((unsigned long)(((unsigned long)(((my_y)->my_label)->my_at))+((unsigned long)(1048576UL))));
	} else {
	(my_n)=(0UL);
	}
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(56UL)))));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_y)=((my_y)->my_next);
	}
	(my_s)=((my_find_section)((my_c),((unsigned char *)".strtab")));
	(my_n)=((my_s)->my_index);
	(my_s)=((my_find_section)((my_c),((unsigned char *)".symtab")));
	((my_s)->my_link)=(my_n);
	((my_s)->my_entsize)=(24UL);
	(my_add_section)((my_c),((unsigned char *)".shstrtab"),(my_SHT_STRTAB));
	(my_s)=((my_c)->my_sections);
	(my_n)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_s))) {
	break;
	}
	((my_s)->my_name_offset)=(my_n);
	(my_n)=((unsigned long)(((unsigned long)(my_n))+((unsigned long)((my_emit_strtab_str)((my_c),((my_s)->my_name))))));
	(my_s)=((my_s)->my_next);
	}
	(my_s)=((my_find_section)((my_c),((unsigned char *)".shstrtab")));
	((my_s)->my_end)=((my_c)->my_at);
	(my_emit_align)((my_c),(16UL),(my_OP_NOP));
	(my_at)=((my_c)->my_at);
	(my_s)=((my_c)->my_sections);
	while (1) {
	if ((unsigned long)(!(my_s))) {
	break;
	}
	(my_n)=((my_s)->my_name_offset);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_n)=((my_s)->my_type);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),(6UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	if ((unsigned long)(((long)((my_s)->my_start))==((long)((my_s)->my_end)))) {
	(my_n)=(0UL);
	} else {
	(my_n)=((unsigned long)(((unsigned long)((my_s)->my_start))+((unsigned long)(1048576UL))));
	}
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(56UL)))));
	(my_n)=((my_s)->my_start);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(56UL)))));
	(my_n)=((unsigned long)(((unsigned long)((my_s)->my_end))-((unsigned long)((my_s)->my_start))));
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(56UL)))));
	(my_n)=((my_s)->my_link);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	if ((unsigned long)(((long)((my_s)->my_type))==((long)(my_SHT_SYMTAB)))) {
	(my_n)=(1UL);
	} else {
	(my_n)=(0UL);
	}
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_as_emit)((my_c),(0UL));
	(my_n)=((my_s)->my_entsize);
	(my_as_emit)((my_c),(my_n));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(8UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(16UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(24UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(32UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(40UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(48UL)))));
	(my_as_emit)((my_c),((unsigned long)(((unsigned long)(my_n))>>((unsigned long)(56UL)))));
	(my_s)=((my_s)->my_next);
	}
	return my_at;
}
void( my_emit_ssr)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	struct my_decl* my_v = 0;
	(my_v)=((my_find)((my_c),((unsigned char *)"global"),((unsigned char *)"_save"),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_member_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no _save"));
	}
	(my_as_emit)(((my_c)->my_s),(my_OP_GS));
	(my_as_modra)(((my_c)->my_s),(my_OP_STORE),(my_R_RSP),((my_v)->my_member_offset));
	(my_v)=((my_find)((my_c),((unsigned char *)"global"),((unsigned char *)"curtask"),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_member_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no global.curtask"));
	}
	(my_as_emit)(((my_c)->my_s),(my_OP_GS));
	(my_as_modra)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSP),((my_v)->my_member_offset));
	(my_v)=((my_find)((my_c),((unsigned char *)"task"),((unsigned char *)"stack"),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_member_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no task.stack"));
	}
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSP),(my_R_RSP),(0UL),(0UL),((my_v)->my_member_offset));
	(my_as_modri)(((my_c)->my_s),(my_OP_ADDI),(my_R_RSP),((unsigned long)(((unsigned long)(4096UL))-((unsigned long)(176UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDX),(my_R_RSP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBX),(my_R_RSP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RBP),(my_R_RSP),(0UL),(0UL),(40UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RSI),(my_R_RSP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RDI),(my_R_RSP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R8),(my_R_RSP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R9),(my_R_RSP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R10),(my_R_RSP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R12),(my_R_RSP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R13),(my_R_RSP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R14),(my_R_RSP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R15),(my_R_RSP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RCX),(my_R_RSP),(0UL),(0UL),(128UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_R11),(my_R_RSP),(0UL),(0UL),(136UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_XORRM),(my_R_RAX),(my_R_RAX));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(8UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(88UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(160UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(168UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_MOVI),(my_R_RAX),(43UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(144UL));
	(my_as_modri)(((my_c)->my_s),(my_OP_MOVI),(my_R_RAX),(35UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(152UL));
	(my_v)=((my_find)((my_c),((unsigned char *)"global"),((unsigned char *)"_save"),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_member_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no _save"));
	}
	(my_as_emit)(((my_c)->my_s),(my_OP_GS));
	(my_as_modra)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),((my_v)->my_member_offset));
	(my_as_modrm)(((my_c)->my_s),(my_OP_STORE),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(32UL));
	(my_as_modrr)(((my_c)->my_s),(my_OP_MOVE),(my_R_RAX),(my_R_RSP));
	(my_as_modrr)(((my_c)->my_s),(my_OP_XORRM),(my_R_RBP),(my_R_RBP));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RBP));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RBP));
	(my_as_modrr)(((my_c)->my_s),(my_OP_MOVE),(my_R_RBP),(my_R_RSP));
	(my_as_opr)(((my_c)->my_s),(my_OP_PUSHR),(my_R_RAX));
	(my_d)=((my_find)((my_c),((unsigned char *)"_ssr"),((void *)0),(1UL)));
	if ((unsigned long)(((my_d)->my_func_defined)&&(((my_d)->my_func_label)->my_fixed))) {
	(my_as_jmp)(((my_c)->my_s),(my_OP_CALL),((my_d)->my_func_label));
	}
	(my_as_op)(((my_c)->my_s),(my_OP_CLI));
	(my_as_modri)(((my_c)->my_s),(my_OP_ADDI),(my_R_RSP),((unsigned long)(((long)(3UL))*((long)(8UL)))));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RAX),(my_R_RSP),(0UL),(0UL),(0UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDX),(my_R_RSP),(0UL),(0UL),(16UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBX),(my_R_RSP),(0UL),(0UL),(24UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RBP),(my_R_RSP),(0UL),(0UL),(40UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSI),(my_R_RSP),(0UL),(0UL),(48UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RDI),(my_R_RSP),(0UL),(0UL),(56UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R8),(my_R_RSP),(0UL),(0UL),(64UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R9),(my_R_RSP),(0UL),(0UL),(72UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R10),(my_R_RSP),(0UL),(0UL),(80UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R12),(my_R_RSP),(0UL),(0UL),(96UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R13),(my_R_RSP),(0UL),(0UL),(104UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R14),(my_R_RSP),(0UL),(0UL),(112UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R15),(my_R_RSP),(0UL),(0UL),(120UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RCX),(my_R_RSP),(0UL),(0UL),(128UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_R11),(my_R_RSP),(0UL),(0UL),(136UL));
	(my_as_modrm)(((my_c)->my_s),(my_OP_LOAD),(my_R_RSP),(my_R_RSP),(0UL),(0UL),(32UL));
	(my_as_rex)(((my_c)->my_s),(my_OP_SYSRET),(0UL),(0UL),(0UL));
	(my_as_op)(((my_c)->my_s),(my_OP_SYSRET));
}
void( my_emit_store)(struct my_assembler* my_c,struct my_type* my_t){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDI));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	if ((unsigned long)(((long)((my_t)->my_kind))==((long)(my_TY_BYTE)))) {
	(my_as_modrm)((my_c),(my_OP_STOREB),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	} else if ((my_type_isprim)((my_t))) {
	(my_as_modrm)((my_c),(my_OP_STORE),(my_R_RAX),(my_R_RDI),(0UL),(0UL),(0UL));
	} else {
	(my_die)(((unsigned char *)"invalid store"));
	}
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_str)(struct my_assembler* my_c,unsigned char* my_s){
	struct my_label* my_a = 0;
	struct my_label* my_b = 0;
	unsigned long my_i = 0;
	(my_a)=((my_mklabel)((my_c)));
	(my_b)=((my_mklabel)((my_c)));
	(my_as_jmp)((my_c),(my_OP_JMP),(my_b));
	(my_fixup_label)((my_c),(my_a));
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(!((my_s)[my_i]))) {
	break;
	}
	(my_as_emit)((my_c),((unsigned long)(my_s)[my_i]));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_as_emit)((my_c),(0UL));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_as_op)((my_c),(my_OP_NOP));
	(my_fixup_label)((my_c),(my_b));
	(my_emit_ptr)((my_c),(my_a));
}
unsigned long( my_emit_strtab_str)(struct my_assembler* my_c,unsigned char* my_s){
	unsigned long my_i = 0;
	(my_i)=(0UL);
	while (1) {
	(my_as_emit)((my_c),((unsigned long)(my_s)[my_i]));
	if ((unsigned long)(!((my_s)[my_i]))) {
	break;
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	return (unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL)));
}
void( my_emit_sub)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_SUBRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_syscall)(struct my_assembler* my_c){
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RAX),(my_R_RBP),(0UL),(0UL),(16UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RDI),(my_R_RBP),(0UL),(0UL),(24UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RSI),(my_R_RBP),(0UL),(0UL),(32UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_RDX),(my_R_RBP),(0UL),(0UL),(40UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_R10),(my_R_RBP),(0UL),(0UL),(48UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_R8),(my_R_RBP),(0UL),(0UL),(56UL));
	(my_as_modrm)((my_c),(my_OP_LOAD),(my_R_R9),(my_R_RBP),(0UL),(0UL),(64UL));
	(my_as_op)((my_c),(my_OP_SYSCALL));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_emit_ud)(struct my_assembler* my_c){
	(my_as_op)((my_c),(my_OP_UD2));
}
void( my_emit_xor)(struct my_assembler* my_c){
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RAX));
	(my_as_opr)((my_c),(my_OP_POPR),(my_R_RDX));
	(my_as_modrr)((my_c),(my_OP_XORRM),(my_R_RAX),(my_R_RDX));
	(my_as_opr)((my_c),(my_OP_PUSHR),(my_R_RAX));
}
void( my_enter)(struct my_peg* my_c,unsigned long my_tag){
	(my_choice)((my_c));
	((my_c)->my_tag)=(my_tag);
}
void( my_exit)(unsigned long my_n){
	(my_syscall)((60UL),(my_n),(0UL),(0UL),(0UL),(0UL),(0UL));
}
void( my_fail)(struct my_peg* my_c){
	if ((unsigned long)(((long)((my_c)->my_sp))==((long)(0UL)))) {
	(my_die)(((unsigned char *)"fail underflow"));
	}
	if ((unsigned long)(((long)((my_c)->my_depth))>((long)((my_c)->my_fail_depth)))) {
	((my_c)->my_fail_depth)=((my_c)->my_depth);
	((my_c)->my_fail_tag)=((my_c)->my_tag);
	((my_c)->my_fail_line)=((my_c)->my_line);
	((my_c)->my_fail_col)=((my_c)->my_col);
	((my_c)->my_fail_literal)=((void *)0);
	}
	((my_c)->my_sp)=((unsigned long)(((unsigned long)((my_c)->my_sp))-((unsigned long)(1UL))));
	((my_c)->my_pos)=((((my_c)->my_stack)[(my_c)->my_sp]).my_pos);
	((my_c)->my_depth)=((((my_c)->my_stack)[(my_c)->my_sp]).my_depth);
	((my_c)->my_op)=((((my_c)->my_stack)[(my_c)->my_sp]).my_op);
	((my_c)->my_tag)=((((my_c)->my_stack)[(my_c)->my_sp]).my_tag);
	((my_c)->my_line)=((((my_c)->my_stack)[(my_c)->my_sp]).my_line);
	((my_c)->my_col)=((((my_c)->my_stack)[(my_c)->my_sp]).my_col);
}
void( my_fclose)(struct my_file* my_f){
	(my_fflush)((my_f));
	if ((unsigned long)(((long)((my_close)(((my_f)->my_fd))))!=((long)(0UL)))) {
	(my_die)(((unsigned char *)"write failed"));
	}
	(my_free)(((my_f)->my_a),((my_f)->my_buf));
	(my_free)(((my_f)->my_a),((unsigned char*)my_f));
}
void( my_fdputc)(unsigned long my_fd,unsigned long my_ch){
	unsigned char my_b = 0;
	unsigned long my_ret = 0;
	(my_b)=((unsigned char)my_ch);
	(my_ret)=((my_write)((my_fd),(&(my_b)),(1UL)));
	if ((unsigned long)(((long)(my_ret))!=((long)(1UL)))) {
	(my_exit)((3UL));
	}
}
void( my_fdputd)(unsigned long my_fd,unsigned long my_n){
	unsigned long my_a = 0;
	if ((unsigned long)(((long)(my_n))<((long)(0UL)))) {
	(my_fdputc)((my_fd),(45));
	(my_a)=((unsigned long)(-(unsigned long)((unsigned long)(((long)(my_n))%((long)(10UL))))));
	(my_n)=((unsigned long)(((long)(my_n))/((long)((unsigned long)(-(unsigned long)(10UL))))));
	} else {
	(my_a)=((unsigned long)(((long)(my_n))%((long)(10UL))));
	(my_n)=((unsigned long)(((long)(my_n))/((long)(10UL))));
	}
	if ((unsigned long)(((long)(my_n))!=((long)(0UL)))) {
	(my_fdputd)((my_fd),(my_n));
	}
	(my_fdputc)((my_fd),((unsigned long)(((unsigned long)(48))+((unsigned long)(my_a)))));
}
void( my_fdputs)(unsigned long my_fd,unsigned char* my_msg){
	unsigned long my_len = 0;
	unsigned long my_ret = 0;
	unsigned long my_off = 0;
	(my_len)=((my_strlen)((my_msg)));
	(my_off)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_off))==((long)(my_len)))) {
	break;
	}
	(my_ret)=((my_write)((my_fd),(my_msg),((unsigned long)(((unsigned long)(my_len))-((unsigned long)(my_off))))));
	if ((unsigned long)(((long)(my_ret))<((long)(0UL)))) {
	(my_exit)((3UL));
	}
	(my_off)=((unsigned long)(((unsigned long)(my_off))+((unsigned long)(my_ret))));
	}
}
void( my_ffill)(struct my_file* my_f){
	unsigned long my_ret = 0;
	if ((my_f)->my_eof) {
	return;
	}
	if ((unsigned long)(((long)((my_f)->my_r))==((long)((my_f)->my_w)))) {
	((my_f)->my_r)=(0UL);
	((my_f)->my_w)=(0UL);
	}
	if ((unsigned long)(((long)((my_f)->my_w))==((long)((my_f)->my_cap)))) {
	(my_die)(((unsigned char *)"out of space"));
	}
	(my_ret)=((my_read)(((my_f)->my_fd),(&(((my_f)->my_buf)[(my_f)->my_w])),((unsigned long)(((unsigned long)((my_f)->my_cap))-((unsigned long)((my_f)->my_w))))));
	if ((unsigned long)(((long)(my_ret))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"read failed"));
	}
	if ((unsigned long)(((long)(my_ret))==((long)(0UL)))) {
	((my_f)->my_eof)=(1UL);
	}
	((my_f)->my_w)=((unsigned long)(((unsigned long)((my_f)->my_w))+((unsigned long)(my_ret))));
}
void( my_fflush)(struct my_file* my_f){
	unsigned long my_ret = 0;
	while (1) {
	if ((unsigned long)(((long)((my_f)->my_r))==((long)((my_f)->my_w)))) {
	((my_f)->my_r)=(0UL);
	((my_f)->my_w)=(0UL);
	return;
	}
	(my_ret)=((my_write)(((my_f)->my_fd),(&(((my_f)->my_buf)[(my_f)->my_r])),((unsigned long)(((unsigned long)((my_f)->my_w))-((unsigned long)((my_f)->my_r))))));
	if ((unsigned long)(((long)(my_ret))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"write failed"));
	}
	((my_f)->my_r)=((unsigned long)(((unsigned long)((my_f)->my_r))+((unsigned long)(my_ret))));
	}
}
unsigned long( my_fgetc)(struct my_file* my_f){
	unsigned long my_ch = 0;
	if ((unsigned long)(((long)((my_f)->my_r))==((long)((my_f)->my_w)))) {
	(my_ffill)((my_f));
	}
	if ((my_f)->my_eof) {
	return (unsigned long)(-(unsigned long)(1UL));
	}
	(my_ch)=((unsigned long)((my_f)->my_buf)[(my_f)->my_r]);
	((my_f)->my_r)=((unsigned long)(((unsigned long)((my_f)->my_r))+((unsigned long)(1UL))));
	return my_ch;
}
void( my_fillpos)(struct my_parser* my_c,struct my_node* my_n){
}
struct my_decl*( my_find)(struct my_compiler* my_c,unsigned char* my_name,unsigned char* my_member_name,unsigned long my_make){
	struct my_decl* my_p = 0;
	struct my_decl* my_d = 0;
	struct my_decl** my_link = 0;
	unsigned long my_dir = 0;
	(my_p)=((void *)0);
	(my_link)=(&((my_c)->my_decls));
	while (1) {
	(my_d)=(*(my_link));
	if ((unsigned long)(!(my_d))) {
	break;
	}
	(my_dir)=((my_strcmp)((my_name),((my_d)->my_name)));
	if ((unsigned long)(((long)(my_dir))==((long)(0UL)))) {
	if ((unsigned long)(((unsigned long)(!(my_member_name)))&&((unsigned long)(!((my_d)->my_member_name))))) {
	(my_dir)=(0UL);
	} else if ((unsigned long)(!(my_member_name))) {
	(my_dir)=((unsigned long)(-(unsigned long)(1UL)));
	} else if ((unsigned long)(!((my_d)->my_member_name))) {
	(my_dir)=(1UL);
	} else {
	(my_dir)=((my_strcmp)((my_member_name),((my_d)->my_member_name)));
	}
	}
	if ((unsigned long)(((long)(my_dir))<((long)(0UL)))) {
	(my_p)=(my_d);
	(my_link)=(&((my_d)->my_l));
	} else if ((unsigned long)(((long)(my_dir))>((long)(0UL)))) {
	(my_p)=(my_d);
	(my_link)=(&((my_d)->my_r));
	} else {
	return my_d;
	}
	}
	if ((unsigned long)(!(my_make))) {
	return (void *)0;
	}
	(my_d)=((struct my_decl*)(my_alloc)(((my_c)->my_a),(240UL)));
	((my_d)->my_name)=(my_name);
	((my_d)->my_member_name)=(my_member_name);
	((my_d)->my_p)=(my_p);
	((my_d)->my_l)=((void *)0);
	((my_d)->my_r)=((void *)0);
	((my_d)->my_used_next)=((void *)0);
	((my_d)->my_func_defined)=(0UL);
	((my_d)->my_func_type)=((void *)0);
	((my_d)->my_func_label)=((my_mklabel)(((my_c)->my_s)));
	((my_d)->my_func_def)=((void *)0);
	((my_d)->my_func_used)=(0UL);
	((my_d)->my_struct_defined)=(0UL);
	((my_d)->my_struct_size)=(0UL);
	((my_d)->my_struct_layout_done)=(0UL);
	((my_d)->my_struct_def)=((void *)0);
	((my_d)->my_member_defined)=(0UL);
	((my_d)->my_member_type)=((void *)0);
	((my_d)->my_member_offset)=(0UL);
	((my_d)->my_member_def)=((void *)0);
	((my_d)->my_enum_defined)=(0UL);
	((my_d)->my_enum_value)=(0UL);
	((my_d)->my_enum_def)=((void *)0);
	((my_d)->my_var_defined)=(0UL);
	((my_d)->my_var_type)=((void *)0);
	((my_d)->my_var_offset)=(0UL);
	((my_d)->my_var_def)=((void *)0);
	((my_d)->my_goto_defined)=(0UL);
	((my_d)->my_goto_label)=((my_mklabel)(((my_c)->my_s)));
	(*(my_link))=(my_d);
	return my_d;
}
struct my_section*( my_find_section)(struct my_assembler* my_c,unsigned char* my_name){
	struct my_section* my_n = 0;
	(my_n)=((my_c)->my_sections);
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	if ((unsigned long)(((long)((my_strcmp)(((my_n)->my_name),(my_name))))==((long)(0UL)))) {
	break;
	}
	(my_n)=((my_n)->my_next);
	}
	return my_n;
}
struct my_decl*( my_first_decl)(struct my_compiler* my_c){
	struct my_decl* my_d = 0;
	(my_d)=((my_c)->my_decls);
	if ((unsigned long)(!(my_d))) {
	return (void *)0;
	}
	while (1) {
	if ((unsigned long)(!((my_d)->my_l))) {
	return my_d;
	}
	(my_d)=((my_d)->my_l);
	}
}
void( my_fixup)(struct my_assembler* my_c,unsigned char* my_here,unsigned long my_delta){
	((my_here)[0UL])=((unsigned char)my_delta);
	((my_here)[1UL])=((unsigned char)(unsigned long)(((unsigned long)(my_delta))>>((unsigned long)(8UL))));
	((my_here)[2UL])=((unsigned char)(unsigned long)(((unsigned long)(my_delta))>>((unsigned long)(16UL))));
	((my_here)[3UL])=((unsigned char)(unsigned long)(((unsigned long)(my_delta))>>((unsigned long)(24UL))));
}
void( my_fixup_label)(struct my_assembler* my_c,struct my_label* my_l){
	struct my_fixup* my_f = 0;
	if ((my_l)->my_fixed) {
	(my_die)(((unsigned char *)"already fixed"));
	}
	((my_l)->my_at)=((my_c)->my_at);
	((my_l)->my_fixed)=(1UL);
	(my_f)=((my_l)->my_fix);
	while (1) {
	if ((unsigned long)(!(my_f))) {
	break;
	}
	(my_fixup)((my_c),((my_f)->my_ptr),((unsigned long)(((unsigned long)((my_l)->my_at))-((unsigned long)((my_f)->my_at)))));
	(my_f)=((my_f)->my_next);
	}
}
void( my_flush_coutput)(struct my_compiler* my_c){
	(my_fflush)(((my_c)->my_cout));
}
struct my_file*( my_fopen)(unsigned long my_fd,struct my_alloc* my_a){
	struct my_file* my_f = 0;
	(my_f)=((struct my_file*)(my_alloc)((my_a),(56UL)));
	((my_f)->my_fd)=(my_fd);
	((my_f)->my_a)=(my_a);
	((my_f)->my_r)=(0UL);
	((my_f)->my_w)=(0UL);
	((my_f)->my_cap)=(4096UL);
	((my_f)->my_eof)=(0UL);
	((my_f)->my_buf)=((my_alloc)((my_a),((my_f)->my_cap)));
	return my_f;
}
void( my_fputb)(struct my_file* my_f,unsigned char* my_s,unsigned long my_n){
	unsigned long my_i = 0;
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))>=((long)(my_n)))) {
	break;
	}
	(my_fputc)((my_f),((unsigned long)(my_s)[my_i]));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
void( my_fputc)(struct my_file* my_f,unsigned long my_ch){
	if ((unsigned long)(((long)((my_f)->my_w))==((long)((my_f)->my_cap)))) {
	(my_fflush)((my_f));
	}
	(((my_f)->my_buf)[(my_f)->my_w])=((unsigned char)my_ch);
	((my_f)->my_w)=((unsigned long)(((unsigned long)((my_f)->my_w))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))==((long)(10)))) {
	(my_fflush)((my_f));
	}
}
void( my_fputd)(struct my_file* my_out,unsigned long my_n){
	unsigned long my_a = 0;
	if ((unsigned long)(((long)(my_n))<((long)(0UL)))) {
	(my_fputc)((my_out),(45));
	(my_a)=((unsigned long)(-(unsigned long)((unsigned long)(((long)(my_n))%((long)(10UL))))));
	(my_n)=((unsigned long)(((long)(my_n))/((long)((unsigned long)(-(unsigned long)(10UL))))));
	} else {
	(my_a)=((unsigned long)(((long)(my_n))%((long)(10UL))));
	(my_n)=((unsigned long)(((long)(my_n))/((long)(10UL))));
	}
	if ((unsigned long)(((long)(my_n))!=((long)(0UL)))) {
	(my_fputd)((my_out),(my_n));
	}
	(my_fputc)((my_out),((unsigned long)(((unsigned long)(48))+((unsigned long)(my_a)))));
}
void( my_fputs)(struct my_file* my_f,unsigned char* my_s){
	unsigned long my_i = 0;
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(!((my_s)[my_i]))) {
	break;
	}
	(my_fputc)((my_f),((unsigned long)(my_s)[my_i]));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
unsigned char*( my_freadall)(struct my_file* my_f,unsigned long* my_size){
	unsigned long my_i = 0;
	unsigned long my_cap = 0;
	unsigned char* my_ret = 0;
	unsigned char* my_tmp = 0;
	unsigned long my_ch = 0;
	(my_i)=(0UL);
	(my_cap)=(0UL);
	while (1) {
	(my_ch)=((my_fgetc)((my_f)));
	if ((unsigned long)(((long)(my_ch))==((long)((unsigned long)(-(unsigned long)(1UL)))))) {
	(*(my_size))=(my_i);
	return my_ret;
	}
	if ((unsigned long)(((long)(my_i))==((long)(my_cap)))) {
	if ((unsigned long)(((long)(my_cap))==((long)(0UL)))) {
	(my_cap)=(4096UL);
	(my_ret)=((my_alloc)(((my_f)->my_a),(my_cap)));
	} else {
	(my_cap)=((unsigned long)(((long)(my_cap))*((long)(2UL))));
	(my_tmp)=((my_alloc)(((my_f)->my_a),(my_cap)));
	(my_memcpy)((my_tmp),(my_ret),(my_i));
	(my_free)(((my_f)->my_a),(my_ret));
	(my_ret)=(my_tmp);
	}
	}
	((my_ret)[my_i])=((unsigned char)my_ch);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
void( my_free)(struct my_alloc* my_a,unsigned char* my_p){
}
unsigned long( my_get)(struct my_peg* my_c){
	unsigned long my_ch = 0;
	if ((unsigned long)(((long)((my_c)->my_pos))==((long)((my_c)->my_size)))) {
	return (unsigned long)(-(unsigned long)(1UL));
	}
	(my_ch)=((unsigned long)((my_c)->my_src)[(my_c)->my_pos]);
	((my_c)->my_pos)=((unsigned long)(((unsigned long)((my_c)->my_pos))+((unsigned long)(1UL))));
	((my_c)->my_col)=((unsigned long)(((unsigned long)((my_c)->my_col))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))==((long)(10)))) {
	((my_c)->my_col)=(1UL);
	((my_c)->my_line)=((unsigned long)(((unsigned long)((my_c)->my_line))+((unsigned long)(1UL))));
	}
	if ((unsigned long)(((long)(my_ch))==((long)(0UL)))) {
	(my_die)(((unsigned char *)"invalid nul in source"));
	}
	return my_ch;
}
unsigned long( my_hex2int)(unsigned char* my_s,unsigned long my_len,unsigned long* my_ok){
	unsigned long my_i = 0;
	unsigned long my_x = 0;
	unsigned long my_d = 0;
	(my_x)=(0UL);
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(my_d)=((unsigned long)(my_s)[my_i]);
	if ((unsigned long)(((long)(my_d))==((long)(95)))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	continue;
	} else if ((unsigned long)(((unsigned long)(((long)(my_d))>=((long)(48))))&&((unsigned long)(((long)(my_d))<=((long)(57)))))) {
	(my_d)=((unsigned long)(((unsigned long)(my_d))-((unsigned long)(48))));
	} else if ((unsigned long)(((unsigned long)(((long)(my_d))>=((long)(97))))&&((unsigned long)(((long)(my_d))<=((long)(102)))))) {
	(my_d)=((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_d))-((unsigned long)(97)))))+((unsigned long)(10UL))));
	} else if ((unsigned long)(((unsigned long)(((long)(my_d))>=((long)(65))))&&((unsigned long)(((long)(my_d))<=((long)(70)))))) {
	(my_d)=((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_d))-((unsigned long)(65)))))+((unsigned long)(10UL))));
	} else {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	if ((unsigned long)(((long)(my_x))>((long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(1UL))<<((unsigned long)(59UL)))))-((unsigned long)(1UL))))))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_x)=((unsigned long)(((long)(my_x))*((long)(16UL))));
	if ((unsigned long)(((long)(my_x))>((long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(1UL))<<((unsigned long)(63UL)))))-((unsigned long)(1UL)))))-((unsigned long)(my_d))))))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_x)=((unsigned long)(((unsigned long)(my_x))+((unsigned long)(my_d))));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(*(my_ok))=(1UL);
	return my_x;
}
unsigned long( my_hexdig)(unsigned long my_ch,unsigned long* my_ok){
	if ((unsigned long)(((unsigned long)(((long)(my_ch))>=((long)(48))))&&((unsigned long)(((long)(my_ch))<=((long)(57)))))) {
	(*(my_ok))=(1UL);
	return (unsigned long)(((unsigned long)(my_ch))-((unsigned long)(48)));
	} else if ((unsigned long)(((unsigned long)(((long)(my_ch))>=((long)(65))))&&((unsigned long)(((long)(my_ch))<=((long)(70)))))) {
	(*(my_ok))=(1UL);
	return (unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_ch))-((unsigned long)(70)))))+((unsigned long)(10UL)));
	} else if ((unsigned long)(((unsigned long)(((long)(my_ch))>=((long)(97))))&&((unsigned long)(((long)(my_ch))<=((long)(102)))))) {
	(*(my_ok))=(1UL);
	return (unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(my_ch))-((unsigned long)(97)))))+((unsigned long)(10UL)));
	} else {
	(*(my_ok))=(0UL);
	return 0UL;
	}
}
unsigned long( my_hoist_locals)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_offset){
	unsigned long my_kind = 0;
	unsigned char* my_name = 0;
	struct my_type* my_t = 0;
	struct my_decl* my_v = 0;
	if ((unsigned long)(!(my_n))) {
	return my_offset;
	}
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	return my_offset;
	}
	(my_hoist_locals)((my_c),(my_d),(((my_n)->my_a)->my_b),(my_offset));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	return my_offset;
	}
	(my_offset)=((my_hoist_locals)((my_c),(my_d),((my_n)->my_a),(my_offset)));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	return (my_hoist_locals)((my_c),(my_d),((my_n)->my_a),(my_offset));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL)))) {
	(my_name)=(((my_n)->my_a)->my_s);
	(my_v)=((my_find)((my_c),((my_d)->my_name),(my_name),(1UL)));
	if ((my_v)->my_goto_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate goto"));
	}
	((my_v)->my_goto_defined)=(1UL);
	return my_offset;
	} else if ((unsigned long)(((long)(my_kind))!=((long)(my_N_VARDECL)))) {
	return my_offset;
	}
	(my_name)=(((my_n)->my_a)->my_s);
	(my_t)=((my_prototype)((my_c),((my_n)->my_b)));
	((my_n)->my_t)=(my_t);
	(my_v)=((my_find)((my_c),((my_d)->my_name),(my_name),(1UL)));
	if ((my_v)->my_var_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate variable"));
	}
	((my_v)->my_var_type)=(my_t);
	((my_v)->my_var_defined)=(1UL);
	(my_offset)=((unsigned long)(((unsigned long)(my_offset))+((unsigned long)((my_type_sizeof)((my_c),(my_t))))));
	((my_v)->my_var_offset)=((unsigned long)(-(unsigned long)(my_offset)));
	return my_offset;
}
void( my_layout_struct)(struct my_compiler* my_c,struct my_decl* my_d){
	struct my_node* my_m = 0;
	unsigned long my_offset = 0;
	unsigned char* my_name = 0;
	struct my_decl* my_md = 0;
	struct my_type* my_t = 0;
	if ((my_d)->my_struct_layout_done) {
	if ((unsigned long)(((long)((my_d)->my_struct_layout_done))==((long)(2UL)))) {
	(my_cdie)((my_c),((unsigned char *)"circular struct definition"));
	}
	return;
	}
	((my_d)->my_struct_layout_done)=(2UL);
	(my_m)=(((my_d)->my_struct_def)->my_b);
	(my_offset)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_m))) {
	break;
	}
	((my_c)->my_filename)=(((my_m)->my_a)->my_filename);
	((my_c)->my_lineno)=(((my_m)->my_a)->my_lineno);
	((my_c)->my_colno)=(((my_m)->my_a)->my_colno);
	(my_name)=((((my_m)->my_a)->my_a)->my_s);
	(my_t)=((my_prototype)((my_c),(((my_m)->my_a)->my_b)));
	(my_md)=((my_find)((my_c),((my_d)->my_name),(my_name),(1UL)));
	if ((my_d)->my_member_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate member"));
	}
	((my_md)->my_member_defined)=(1UL);
	((my_md)->my_member_type)=(my_t);
	((my_md)->my_member_offset)=(my_offset);
	((my_md)->my_member_def)=(my_m);
	(my_offset)=((unsigned long)(((unsigned long)(my_offset))+((unsigned long)((my_type_sizeof)((my_c),(my_t))))));
	(my_m)=((my_m)->my_b);
	}
	((my_d)->my_struct_size)=(my_offset);
	((my_d)->my_struct_layout_done)=(1UL);
}
void( my_layout_union)(struct my_compiler* my_c,struct my_decl* my_d){
	struct my_node* my_m = 0;
	unsigned long my_size = 0;
	unsigned long my_member_size = 0;
	unsigned char* my_name = 0;
	struct my_decl* my_md = 0;
	struct my_type* my_t = 0;
	if ((my_d)->my_struct_layout_done) {
	if ((unsigned long)(((long)((my_d)->my_struct_layout_done))==((long)(2UL)))) {
	(my_cdie)((my_c),((unsigned char *)"circular struct definition"));
	}
	return;
	}
	((my_d)->my_struct_layout_done)=(2UL);
	(my_m)=(((my_d)->my_struct_def)->my_b);
	(my_size)=(0UL);
	while (1) {
	if ((unsigned long)(!(my_m))) {
	break;
	}
	((my_c)->my_filename)=(((my_m)->my_a)->my_filename);
	((my_c)->my_lineno)=(((my_m)->my_a)->my_lineno);
	((my_c)->my_colno)=(((my_m)->my_a)->my_colno);
	(my_name)=((((my_m)->my_a)->my_a)->my_s);
	(my_t)=((my_prototype)((my_c),(((my_m)->my_a)->my_b)));
	(my_md)=((my_find)((my_c),((my_d)->my_name),(my_name),(1UL)));
	if ((my_d)->my_member_defined) {
	(my_cdie)((my_c),((unsigned char *)"duplicate member"));
	}
	((my_md)->my_member_defined)=(1UL);
	((my_md)->my_member_type)=(my_t);
	((my_md)->my_member_offset)=(0UL);
	((my_md)->my_member_def)=(my_m);
	(my_member_size)=((my_type_sizeof)((my_c),(my_t)));
	if ((unsigned long)(((long)(my_member_size))>((long)(my_size)))) {
	(my_size)=(my_member_size);
	}
	(my_m)=((my_m)->my_b);
	}
	((my_d)->my_struct_size)=(my_size);
	((my_d)->my_struct_layout_done)=(1UL);
}
void( my_leave)(struct my_peg* my_c,unsigned long my_tag){
	unsigned long my_nargs = 0;
	unsigned long my_start = 0;
	unsigned long my_line = 0;
	unsigned long my_col = 0;
	unsigned long my_end = 0;
	unsigned char* my_tmp = 0;
	(my_commit)((my_c));
	((my_c)->my_fail_depth)=(0UL);
	(my_nargs)=((unsigned long)(((unsigned long)((my_c)->my_depth))-((unsigned long)((((my_c)->my_stack)[(my_c)->my_sp]).my_depth))));
	(my_line)=((((my_c)->my_stack)[(my_c)->my_sp]).my_line);
	(my_col)=((((my_c)->my_stack)[(my_c)->my_sp]).my_col);
	(my_start)=((((my_c)->my_stack)[(my_c)->my_sp]).my_pos);
	(my_end)=((my_c)->my_pos);
	if ((unsigned long)(((long)((my_c)->my_op))==((long)((my_c)->my_cap)))) {
	if ((unsigned long)(((long)((my_c)->my_cap))==((long)(0UL)))) {
	((my_c)->my_cap)=(1024UL);
	((my_c)->my_out)=((struct my_peg_op*)(my_alloc)(((my_c)->my_a),((unsigned long)(((long)((my_c)->my_cap))*((long)(48UL))))));
	} else {
	((my_c)->my_cap)=((unsigned long)(((long)((my_c)->my_cap))*((long)(2UL))));
	(my_tmp)=((my_alloc)(((my_c)->my_a),((unsigned long)(((long)((my_c)->my_cap))*((long)(48UL))))));
	(my_memcpy)((my_tmp),((unsigned char*)(my_c)->my_out),((unsigned long)(((long)((my_c)->my_op))*((long)(48UL)))));
	(my_free)(((my_c)->my_a),((unsigned char*)(my_c)->my_out));
	((my_c)->my_out)=((struct my_peg_op*)my_tmp);
	}
	}
	((((my_c)->my_out)[(my_c)->my_op]).my_tag)=(my_tag);
	((((my_c)->my_out)[(my_c)->my_op]).my_nargs)=(my_nargs);
	((((my_c)->my_out)[(my_c)->my_op]).my_start)=(my_start);
	((((my_c)->my_out)[(my_c)->my_op]).my_end)=(my_end);
	((((my_c)->my_out)[(my_c)->my_op]).my_line)=(my_line);
	((((my_c)->my_out)[(my_c)->my_op]).my_col)=(my_col);
	((my_c)->my_op)=((unsigned long)(((unsigned long)((my_c)->my_op))+((unsigned long)(1UL))));
	((my_c)->my_depth)=((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((my_c)->my_depth))-((unsigned long)(my_nargs)))))+((unsigned long)(1UL))));
}
unsigned long( my_literal)(struct my_peg* my_c,unsigned char* my_s){
	unsigned long my_i = 0;
	unsigned long my_ch = 0;
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(!((my_s)[my_i]))) {
	break;
	}
	(my_ch)=((my_get)((my_c)));
	if ((unsigned long)(((long)(my_ch))!=((long)((unsigned long)(my_s)[my_i])))) {
	(my_fail)((my_c));
	((my_c)->my_fail_literal)=(my_s);
	return 0UL;
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	return 1UL;
}
void( my_main)(unsigned long my_argc,unsigned char** my_argv,unsigned char** my_envp){
	struct my_alloc my_a = {0};
	struct my_compiler* my_c = 0;
	struct my_node* my_p = 0;
	unsigned long my_i = 0;
	unsigned long my_show = 0;
	unsigned char* my_filename = 0;
	struct my_file* my_err = 0;
	struct my_name_node* my_input = 0;
	struct my_name_node* my_tmp = 0;
	struct my_name_node** my_link = 0;
	struct my_peg_compiler* my_peg = 0;
	(my_link)=(&(my_input));
	(my_setup_alloc)((&(my_a)));
	(my_c)=((my_comp_setup)((&(my_a))));
	(my_show)=(0UL);
	(my_filename)=((unsigned char *)"a.out");
	(my_i)=(1UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))>=((long)(my_argc)))) {
	break;
	}
	if ((unsigned long)(!((my_strcmp)(((my_argv)[my_i]),((unsigned char *)"-o"))))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_i))>=((long)(my_argc)))) {
	(my_die)(((unsigned char *)"invalid -o at end of argument list"));
	}
	(my_filename)=((my_argv)[my_i]);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	continue;
	}
	if ((unsigned long)(!((my_strcmp)(((my_argv)[my_i]),((unsigned char *)"-fdump"))))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	(my_show)=(1UL);
	continue;
	}
	if ((unsigned long)(!((my_strcmp)(((my_argv)[my_i]),((unsigned char *)"-C"))))) {
	((my_c)->my_do_cout)=(1UL);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	continue;
	}
	if ((unsigned long)(!((my_strcmp)(((my_argv)[my_i]),((unsigned char *)"-P"))))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_i))>=((long)(my_argc)))) {
	(my_die)(((unsigned char *)"invalid -P at end of argument list"));
	}
	(my_peg)=((my_setup_peg)((&(my_a)),((my_argv)[my_i])));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	continue;
	}
	if ((unsigned long)(((long)(((my_argv)[my_i])[0UL]))==((long)((unsigned char)45)))) {
	(my_die)(((unsigned char *)"invalid argument"));
	}
	(my_tmp)=((struct my_name_node*)(my_alloc)((&(my_a)),(16UL)));
	((my_tmp)->my_next)=((void *)0);
	((my_tmp)->my_name)=((my_argv)[my_i]);
	(*(my_link))=(my_tmp);
	(my_link)=(&((my_tmp)->my_next));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	if (my_peg) {
	if ((unsigned long)(!(my_input))) {
	(my_die)(((unsigned char *)"expected input"));
	}
	(my_peg_open_output)((my_peg),(my_filename));
	(my_tmp)=(my_input);
	while (1) {
	if ((unsigned long)(!(my_tmp))) {
	break;
	}
	(my_peg_compile)((my_peg),((my_tmp)->my_name));
	(my_tmp)=((my_tmp)->my_next);
	}
	return;
	}
	(my_tmp)=(my_input);
	while (1) {
	if ((unsigned long)(!(my_tmp))) {
	break;
	}
	(my_p)=((my_concat_program)((my_p),((my_parse)(((my_c)->my_p),((my_tmp)->my_name)))));
	(my_tmp)=((my_tmp)->my_next);
	}
	if (my_show) {
	(my_err)=((my_fopen)((2UL),(&(my_a))));
	(my_show_node)((my_err),(my_p));
	(my_fflush)((my_err));
	return;
	}
	(my_compile)((my_c),(my_p));
	if ((my_c)->my_do_cout) {
	(my_open_coutput)((my_c),(my_filename));
	(my_ctranslate)((my_c));
	return;
	}
	(my_emit_builtin)((my_c));
	(my_open_output)(((my_c)->my_s),(my_filename));
	(my_writeout)(((my_c)->my_s),((my_c)->my_start),((my_c)->my_kstart));
}
void( my_mark_expr_used)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n){
	struct my_decl* my_v = 0;
	unsigned long my_kind = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_EXPRLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_mark_expr_used)((my_c),(my_d),((my_n)->my_a));
	(my_n)=((my_n)->my_b);
	}
	return;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_IDENT)))) {
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_enum_defined))) {
	return;
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),((my_n)->my_s),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_var_defined))) {
	return;
	}
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_func_defined))) {
	(my_mark_func_used)((my_c),(my_v));
	return;
	}
	(my_cdie)((my_c),((unsigned char *)"no such variable"));
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_CALL))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_ASSIGN))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_INDEX))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_LT))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_GT))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_LE))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_GE))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_EQ))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_NE))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_BOR))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_BAND))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_ADD))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_SUB))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_MUL))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_DIV))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_MOD))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_LSH))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_RSH))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_AND))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_OR))))||((unsigned long)(((long)(my_kind))==((long)(my_N_XOR)))))))))))))))))))))))))))))))))))))))))))) {
	(my_mark_expr_used)((my_c),(my_d),((my_n)->my_a));
	(my_mark_expr_used)((my_c),(my_d),((my_n)->my_b));
	return;
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_REF))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_DEREF))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_BNOT))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_POS))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_NEG))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_NOT))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_CAST))))||((unsigned long)(((long)(my_kind))==((long)(my_N_DOT)))))))))))))))))) {
	(my_mark_expr_used)((my_c),(my_d),((my_n)->my_a));
	return;
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_SIZEOF))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_STR))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_NUM))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_CHAR))))||((unsigned long)(((long)(my_kind))==((long)(my_N_NIL)))))))))))) {
	return;
	} else {
	(my_cdie)((my_c),((unsigned char *)"not an expression"));
	}
}
void( my_mark_func_used)(struct my_compiler* my_c,struct my_decl* my_d){
	if ((my_d)->my_func_used) {
	return;
	}
	((my_d)->my_func_used)=(1UL);
	((my_d)->my_used_next)=((my_c)->my_used_top);
	((my_c)->my_used_top)=(my_d);
}
void( my_mark_stmt_used)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n){
	unsigned long my_kind = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_mark_expr_used)((my_c),(my_d),(((my_n)->my_a)->my_a));
	(my_mark_stmt_used)((my_c),(my_d),(((my_n)->my_a)->my_b));
	(my_n)=((my_n)->my_b);
	}
	return;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_mark_stmt_used)((my_c),(my_d),((my_n)->my_a));
	(my_n)=((my_n)->my_b);
	}
	return;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	(my_mark_stmt_used)((my_c),(my_d),((my_n)->my_a));
	return;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RETURN)))) {
	(my_mark_expr_used)((my_c),(my_d),((my_n)->my_a));
	return;
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_BREAK))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_CONTINUE))))||((unsigned long)(((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL))))||((unsigned long)(((long)(my_kind))==((long)(my_N_GOTO)))))))))) {
	return;
	} else if ((unsigned long)(((long)(my_kind))!=((long)(my_N_VARDECL)))) {
	(my_mark_expr_used)((my_c),(my_d),(my_n));
	return;
	}
}
void( my_memcpy)(unsigned char* my_dest,unsigned char* my_src,unsigned long my_size){
	unsigned long my_i = 0;
	if ((unsigned long)(((long)(my_size))<((long)(0UL)))) {
	return;
	}
	if ((unsigned long)(((long)(my_src))>((long)(my_dest)))) {
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_size)))) {
	break;
	}
	((my_dest)[my_i])=((my_src)[my_i]);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	} else if ((unsigned long)(((long)(my_src))<((long)(my_dest)))) {
	(my_i)=(my_size);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(0UL)))) {
	break;
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))-((unsigned long)(1UL))));
	((my_dest)[my_i])=((my_src)[my_i]);
	}
	}
}
void( my_memset)(unsigned char* my_dest,unsigned long my_c,unsigned long my_size){
	unsigned long my_i = 0;
	if ((unsigned long)(((long)(my_size))<((long)(0UL)))) {
	return;
	}
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_size)))) {
	break;
	}
	((my_dest)[my_i])=((unsigned char)my_c);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
struct my_label*( my_mklabel)(struct my_assembler* my_c){
	struct my_label* my_l = 0;
	(my_l)=((struct my_label*)(my_alloc)(((my_c)->my_a),(24UL)));
	((my_l)->my_fix)=((void *)0);
	((my_l)->my_at)=(0UL);
	((my_l)->my_fixed)=(0UL);
	return my_l;
}
struct my_node*( my_mknode)(struct my_parser* my_c,unsigned long my_kind,struct my_node* my_a,struct my_node* my_b){
	struct my_node* my_ret = 0;
	(my_ret)=((struct my_node*)(my_alloc)(((my_c)->my_a),(72UL)));
	((my_ret)->my_kind)=(my_kind);
	((my_ret)->my_a)=(my_a);
	((my_ret)->my_b)=(my_b);
	((my_ret)->my_filename)=((void *)0);
	((my_ret)->my_lineno)=(0UL);
	((my_ret)->my_colno)=(0UL);
	((my_ret)->my_n)=(0UL);
	((my_ret)->my_s)=((void *)0);
	((my_ret)->my_t)=((void *)0);
	(my_fillpos)((my_c),(my_ret));
	return my_ret;
}
struct my_node*( my_mknode0)(struct my_parser* my_c,unsigned long my_kind){
	return (my_mknode)((my_c),(my_kind),((void *)0),((void *)0));
}
struct my_node*( my_mknode1)(struct my_parser* my_c,unsigned long my_kind,struct my_node* my_a){
	return (my_mknode)((my_c),(my_kind),(my_a),((void *)0));
}
struct my_type*( my_mktype)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a,struct my_type* my_b,struct my_decl* my_st){
	struct my_type* my_t = 0;
	(my_t)=((struct my_type*)(my_alloc)(((my_c)->my_a),(32UL)));
	((my_t)->my_kind)=(my_kind);
	((my_t)->my_st)=(my_st);
	((my_t)->my_val)=(my_a);
	((my_t)->my_arg)=(my_b);
	return my_t;
}
struct my_type*( my_mktype0)(struct my_compiler* my_c,unsigned long my_kind){
	return (my_mktype)((my_c),(my_kind),((void *)0),((void *)0),((void *)0));
}
struct my_type*( my_mktype1)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a){
	return (my_mktype)((my_c),(my_kind),(my_a),((void *)0),((void *)0));
}
struct my_type*( my_mktype2)(struct my_compiler* my_c,unsigned long my_kind,struct my_type* my_a,struct my_type* my_b){
	return (my_mktype)((my_c),(my_kind),(my_a),(my_b),((void *)0));
}
struct my_type*( my_mktype_struct)(struct my_compiler* my_c,struct my_decl* my_st){
	return (my_mktype)((my_c),(my_TY_STRUCT),((void *)0),((void *)0),(my_st));
}
struct my_type*( my_mktype_union)(struct my_compiler* my_c,struct my_decl* my_st){
	return (my_mktype)((my_c),(my_TY_UNION),((void *)0),((void *)0),(my_st));
}
unsigned long( my_mmap)(unsigned long my_addr,unsigned long my_len,unsigned long my_prot,unsigned long my_flags,unsigned long my_fd,unsigned long my_off){
	return (my_syscall)((9UL),(my_addr),(my_len),(my_prot),(my_flags),(my_fd),(my_off));
}
struct my_decl*( my_next_decl)(struct my_compiler* my_c,struct my_decl* my_d){
	if ((unsigned long)(!(my_d))) {
	return (void *)0;
	}
	if ((my_d)->my_r) {
	(my_d)=((my_d)->my_r);
	while (1) {
	if ((unsigned long)(!((my_d)->my_l))) {
	return my_d;
	}
	(my_d)=((my_d)->my_l);
	}
	}
	while (1) {
	if ((unsigned long)(!((my_d)->my_p))) {
	return (void *)0;
	}
	if ((unsigned long)(((long)(((my_d)->my_p)->my_l))==((long)(my_d)))) {
	return (my_d)->my_p;
	}
	(my_d)=((my_d)->my_p);
	}
}
unsigned char*( my_node_to_str)(unsigned long my_kind){
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_IDENT)))) {
	return (unsigned char *)"N_IDENT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_NUM)))) {
	return (unsigned char *)"N_NUM";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CHAR)))) {
	return (unsigned char *)"N_CHAR";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STR)))) {
	return (unsigned char *)"N_STR";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_NIL)))) {
	return (unsigned char *)"N_NIL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	return (unsigned char *)"N_STMTLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_EXPRLIST)))) {
	return (unsigned char *)"N_EXPRLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CALL)))) {
	return (unsigned char *)"N_CALL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_DOT)))) {
	return (unsigned char *)"N_DOT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ARGLIST)))) {
	return (unsigned char *)"N_ARGLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNC)))) {
	return (unsigned char *)"N_FUNC";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ARGDECL)))) {
	return (unsigned char *)"N_ARGDECL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNCDECL)))) {
	return (unsigned char *)"N_FUNCDECL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_PROGRAM)))) {
	return (unsigned char *)"N_PROGRAM";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNCTYPE)))) {
	return (unsigned char *)"N_FUNCTYPE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_PTRTYPE)))) {
	return (unsigned char *)"N_PTRTYPE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STRUCT)))) {
	return (unsigned char *)"N_STRUCT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_UNION)))) {
	return (unsigned char *)"N_UNION";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_MEMBERDECL)))) {
	return (unsigned char *)"N_MEMBERDECL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_MEMBERLIST)))) {
	return (unsigned char *)"N_MEMBERLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	return (unsigned char *)"N_CONDLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_COND)))) {
	return (unsigned char *)"N_COND";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ENUM)))) {
	return (unsigned char *)"N_ENUM";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ENUMITEM)))) {
	return (unsigned char *)"N_ENUMITEM";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ENUMLIST)))) {
	return (unsigned char *)"N_ENUMLIST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	return (unsigned char *)"N_LOOP";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_BREAK)))) {
	return (unsigned char *)"N_BREAK";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONTINUE)))) {
	return (unsigned char *)"N_CONTINUE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_RETURN)))) {
	return (unsigned char *)"N_RETURN";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_VARDECL)))) {
	return (unsigned char *)"N_VARDECL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL)))) {
	return (unsigned char *)"N_LABEL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_GOTO)))) {
	return (unsigned char *)"N_GOTO";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ASSIGN)))) {
	return (unsigned char *)"N_ASSIGN";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_SIZEOF)))) {
	return (unsigned char *)"N_SIZEOF";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_REF)))) {
	return (unsigned char *)"N_REF";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_DEREF)))) {
	return (unsigned char *)"N_DEREF";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CAST)))) {
	return (unsigned char *)"N_CAST";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_INDEX)))) {
	return (unsigned char *)"N_INDEX";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_LT)))) {
	return (unsigned char *)"N_LT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_GT)))) {
	return (unsigned char *)"N_GT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_LE)))) {
	return (unsigned char *)"N_LE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_GE)))) {
	return (unsigned char *)"N_GE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_EQ)))) {
	return (unsigned char *)"N_EQ";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_NE)))) {
	return (unsigned char *)"N_NE";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_ADD)))) {
	return (unsigned char *)"N_ADD";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_SUB)))) {
	return (unsigned char *)"N_SUB";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_MUL)))) {
	return (unsigned char *)"N_MUL";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_LSH)))) {
	return (unsigned char *)"N_LSH";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_RSH)))) {
	return (unsigned char *)"N_RSH";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_BNOT)))) {
	return (unsigned char *)"N_BNOT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_BOR)))) {
	return (unsigned char *)"N_BOR";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_BAND)))) {
	return (unsigned char *)"N_BAND";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_AND)))) {
	return (unsigned char *)"N_AND";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_OR)))) {
	return (unsigned char *)"N_OR";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_XOR)))) {
	return (unsigned char *)"N_XOR";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_NOT)))) {
	return (unsigned char *)"N_NOT";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_POS)))) {
	return (unsigned char *)"N_POS";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_NEG)))) {
	return (unsigned char *)"N_NEG";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_DIV)))) {
	return (unsigned char *)"N_DIV";
	}
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_MOD)))) {
	return (unsigned char *)"N_MOD";
	}
	return (unsigned char *)"(invalid)";
}
unsigned long( my_open)(unsigned char* my_name,unsigned long my_flags,unsigned long my_mode){
	return (my_syscall)((2UL),((unsigned long)my_name),(my_flags),(my_mode),(0UL),(0UL),(0UL));
}
void( my_open_coutput)(struct my_compiler* my_c,unsigned char* my_filename){
	unsigned long my_fd = 0;
	if ((my_c)->my_cout) {
	(my_die)(((unsigned char *)"multiple output files"));
	}
	(my_unlink)((my_filename));
	(my_fd)=((my_open)((my_filename),((unsigned long)(((unsigned long)(my_O_CREAT))|((unsigned long)(my_O_WRONLY)))),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(6UL))<<((unsigned long)(6UL)))))+((unsigned long)((unsigned long)(((unsigned long)(6UL))<<((unsigned long)(3UL))))))))+((unsigned long)(6UL))))));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"failed to open output"));
	}
	((my_c)->my_cout)=((my_fopen)((my_fd),((my_c)->my_a)));
}
void( my_open_output)(struct my_assembler* my_c,unsigned char* my_filename){
	unsigned long my_fd = 0;
	if ((my_c)->my_out) {
	(my_die)(((unsigned char *)"multiple output files"));
	}
	(my_unlink)((my_filename));
	(my_fd)=((my_open)((my_filename),((unsigned long)(((unsigned long)(my_O_CREAT))|((unsigned long)(my_O_WRONLY)))),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(7UL))<<((unsigned long)(6UL)))))+((unsigned long)((unsigned long)(((unsigned long)(7UL))<<((unsigned long)(3UL))))))))+((unsigned long)(7UL))))));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"failed to open output"));
	}
	((my_c)->my_out)=((my_fopen)((my_fd),((my_c)->my_a)));
}
struct my_node*( my_parse)(struct my_parser* my_c,unsigned char* my_filename){
	struct my_file* my_f = 0;
	unsigned long my_fd = 0;
	unsigned long my_len = 0;
	unsigned char* my_src = 0;
	struct my_peg_node* my_pn = 0;
	(my_fd)=((my_open)((my_filename),(0UL),(0UL)));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_fdputs)((2UL),((unsigned char *)"failed to open "));
	(my_fdputs)((2UL),(my_filename));
	(my_fdputs)((2UL),((unsigned char *)"\012"));
	(my_exit)((1UL));
	}
	(my_f)=((my_fopen)((my_fd),((my_c)->my_a)));
	(my_src)=((my_freadall)((my_f),(&(my_len))));
	(my_fclose)((my_f));
	(my_peg_reset)(((my_c)->my_p),(my_filename),(my_src),(my_len));
	(my_pn)=((my_peg_parse)(((my_c)->my_p),(my_P_sp)));
	return (my_reconstruct)((my_c),(my_pn));
}
unsigned long( my_parse_escape)(unsigned char* my_s,unsigned long* my_i,unsigned long my_n){
	unsigned long my_nc = 0;
	unsigned long my_ok = 0;
	if ((unsigned long)(((long)(*(my_i)))==((long)(my_n)))) {
	(my_die)(((unsigned char *)"invalid escape"));
	}
	(my_nc)=((unsigned long)(my_s)[*(my_i)]);
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_nc))==((long)(116)))) {
	return 9;
	} else if ((unsigned long)(((long)(my_nc))==((long)(114)))) {
	return 13;
	} else if ((unsigned long)(((long)(my_nc))==((long)(110)))) {
	return 10;
	} else if ((unsigned long)(((long)(my_nc))==((long)(92)))) {
	return 92;
	} else if ((unsigned long)(((long)(my_nc))==((long)(39)))) {
	return 39;
	} else if ((unsigned long)(((long)(my_nc))==((long)(34)))) {
	return 34;
	} else if ((unsigned long)(((long)(my_nc))==((long)(45)))) {
	return 45;
	} else if ((unsigned long)(((long)(my_nc))==((long)(91)))) {
	return 91;
	} else if ((unsigned long)(((long)(my_nc))==((long)(93)))) {
	return 93;
	} else if ((unsigned long)(((long)(my_nc))==((long)(120)))) {
	if ((unsigned long)(((long)((unsigned long)(((unsigned long)(my_n))-((unsigned long)(*(my_i))))))<((long)(2UL)))) {
	(my_die)(((unsigned char *)"invalid escape"));
	}
	(my_nc)=((unsigned long)(((long)((my_hexdig)(((unsigned long)(my_s)[*(my_i)]),(&(my_ok)))))*((long)(16UL))));
	if ((unsigned long)(!(my_ok))) {
	(my_die)(((unsigned char *)"invalid hex"));
	}
	(my_nc)=((unsigned long)(((unsigned long)(my_nc))+((unsigned long)((my_hexdig)(((unsigned long)(my_s)[(unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL)))]),(&(my_ok)))))));
	if ((unsigned long)(!(my_ok))) {
	(my_die)(((unsigned char *)"invalid hex"));
	}
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(2UL))));
	return my_nc;
	} else {
	(my_die)(((unsigned char *)"invalid escape"));
	}
}
unsigned long( my_peg_PEG_alternative)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_alternative));
	(my_ok)=((my_peg_PEG_lookahead)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_lookahead)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_alternative));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_any)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_any));
	(my_ok)=((my_literal)((my_c),((unsigned char *)".")));
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_any));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_call)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_call));
	(my_ok)=((my_peg_PEG_identifier)((my_c)));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<-")));
	}
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_call));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_class)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_class));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"[")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"]")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_any)((my_c)));
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"-")));
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_any)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"]")));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_class));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_countop)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_countop));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"*+?")));
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_countop));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_grammar)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_grammar));
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_PEG_rule)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_rule)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_any)((my_c)));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_grammar));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_identifier)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_identifier));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_identifier));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_literal)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_literal));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_literal));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_lookahead)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_lookahead));
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_lookop)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_suffix)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_lookahead));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_lookop)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_lookop));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"!&")));
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_lookop));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_pattern)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_pattern));
	(my_ok)=((my_peg_PEG_alternative)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"/")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"/")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_alternative)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_pattern));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_primary)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_primary));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_pattern)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_literal)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_class)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_call)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_primary));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_rule)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_rule));
	(my_ok)=((my_peg_PEG_identifier)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<-")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_PEG_pattern)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_rule));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_sp)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_sp));
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"\011\012\015 ")));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"//")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"\012\015")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_sp));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_PEG_suffix)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_PEG_suffix));
	(my_ok)=((my_peg_PEG_primary)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_PEG_countop)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_PEG_sp)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_PEG_suffix));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_add_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_add_expr));
	(my_ok)=((my_peg_P_mul_expr)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_add_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_sub_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_or_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_xor_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_mul_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_add_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_add_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_add_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"+")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_add_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_and_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_and_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"&")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"&")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_and_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_arg_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_arg_decl));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)":")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_arg_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_as)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_as));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"as")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_as));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_assign_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_assign_stmt));
	(my_ok)=((my_peg_P_unary_expr)((my_c)));
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"=")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_assign_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_band_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_band_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"&&")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_band_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_bnot_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_bnot_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"!")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"=")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_bnot_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_bool_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_bool_expr));
	(my_ok)=((my_peg_P_comp_expr)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_band_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_bor_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_comp_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_bool_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_bor_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_bor_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"||")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_bor_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_break)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_break));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"break")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_break));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_break_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_break_stmt));
	(my_ok)=((my_peg_P_break)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_break_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_byte)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_byte));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"byte")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_byte));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_call_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_call_expr));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_expr)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_call_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_cast_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_cast_expr));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"as")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_cast_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_char)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_char));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\134")));
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\134")));
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"'")));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_char));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_comp_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_comp_expr));
	(my_ok)=((my_peg_P_add_expr)((my_c)));
	if (my_ok) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_le_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ge_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_lt_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_gt_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_eq_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ne_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_add_expr)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_comp_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_compound_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_compound_stmt));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"{")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_stmt)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"}")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_compound_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_continue)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_continue));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"continue")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_continue));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_continue_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_continue_stmt));
	(my_ok)=((my_peg_P_continue)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_continue_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_dec)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_dec));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"_")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789_")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789_")));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_dec));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_deref_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_deref_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"*")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_deref_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_div_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_div_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"/")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"/")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_div_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_elif_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_elif_stmt));
	(my_ok)=((my_peg_P_else)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_if)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_elif_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_else)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_else));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"else")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_else));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_else_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_else_stmt));
	(my_ok)=((my_peg_P_else)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_else_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_empty_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_empty_stmt));
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_empty_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_enum)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_enum));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"enum")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_enum));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_enum_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_enum_decl));
	(my_ok)=((my_peg_P_enum)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"{")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_enum_item)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_enum_item)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"}")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_enum_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_enum_item)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_enum_item));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"=")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_enum_item));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_eq_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_eq_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"==")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_eq_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_expr));
	(my_ok)=((my_peg_P_bool_expr)((my_c)));
	if (my_ok) {
	(my_leave)((my_c),(my_P_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_expr_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_expr_stmt));
	(my_ok)=((my_peg_P_expr)((my_c)));
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_expr_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_func)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_func));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"func")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_func));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_func_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_func_decl));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"func")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_func_type)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_func_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_func_type)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_func_type));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_arg_decl)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_arg_decl)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)",")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)":")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_func_type));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_ge_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_ge_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)">=")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_ge_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_goto)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_goto));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"goto")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_goto));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_goto_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_goto_stmt));
	(my_ok)=((my_peg_P_goto)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_goto_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_grammar)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_grammar));
	(my_ok)=((my_peg_P_sp)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_enum_decl)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_struct_decl)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_union_decl)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_func_decl)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_any)((my_c)));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_grammar));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_gt_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_gt_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)">")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"=")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)">")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_gt_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_hex)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_hex));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"0x")));
	if (my_ok) {
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEF_abcdef")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEF_abcdef")));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_hex));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_ident)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_ident));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_reserved)((my_c)));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_charset)((my_c),((unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_ident));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_if)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_if));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"if")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_if));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_if_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_if_stmt));
	(my_ok)=((my_peg_P_if)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_elif_stmt)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_else_stmt)((my_c)));
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_if_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_index_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_index_expr));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"[")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"]")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_index_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_int)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_int));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"int")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_int));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_label_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_label_stmt));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)":")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_label_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_le_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_le_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<=")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_le_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_loop)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_loop));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"loop")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_loop));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_loop_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_loop_stmt));
	(my_ok)=((my_peg_P_loop)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_loop_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_lsh_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_lsh_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<<")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_lsh_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_lt_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_lt_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"=")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"<")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_lt_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_member_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_member_decl));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)":")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_member_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_member_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_member_expr));
	(my_ok)=((my_literal)((my_c),((unsigned char *)".")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_member_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_mod_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_mod_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"%")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_mod_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_mul_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_mul_expr));
	(my_ok)=((my_peg_P_shift_expr)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_mul_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_div_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_mod_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_and_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_shift_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_mul_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_mul_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_mul_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"*")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_mul_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_ne_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_ne_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"!=")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_ne_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_neg_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_neg_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"-")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_neg_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_nil)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_nil));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"nil")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_nil));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_not_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_not_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"~")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_not_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_or_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_or_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"|")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"|")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_or_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_pos_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_pos_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"+")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_pos_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_post_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_post_expr));
	(my_ok)=((my_peg_P_primary)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_index_expr)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_call_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_member_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_cast_expr)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_post_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_primary)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_primary));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_hex)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_dec)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_str)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_char)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_sizeof_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_nil)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_primary));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_ptr_type)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_ptr_type));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"*")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_ptr_type));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_ref_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_ref_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"&")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"&")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_ref_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_reserved)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_reserved));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_return)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_break)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_sizeof)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_if)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_else)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_loop)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_continue)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_goto)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_var)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_enum)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_struct)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_union)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_byte)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_int)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_void)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_func)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_as)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_nil)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_reserved));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_return)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_return));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"return")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_return));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_return_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_return_stmt));
	(my_ok)=((my_peg_P_return)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_expr)((my_c)));
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_return_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_rsh_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_rsh_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)">>")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_rsh_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_shift_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_shift_expr));
	(my_ok)=((my_peg_P_unary_expr)((my_c)));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_lsh_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_rsh_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_unary_expr)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_shift_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_sizeof)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_sizeof));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"sizeof")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_sizeof));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_sizeof_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_sizeof_expr));
	(my_ok)=((my_peg_P_sizeof)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_expr)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_sizeof_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_sp)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_sp));
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"\011\012\015 ")));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"//")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"\012\015")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_sp));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_stmt));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_if_stmt)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_loop_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_break_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_continue_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_return_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_var_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_label_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_goto_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_assign_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_expr_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_empty_stmt)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_compound_stmt)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_str)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_str));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\042")));
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\134")));
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\042")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	if (my_ok) {
	(my_ok)=((my_any)((my_c)));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"\042")));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_str));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_struct)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_struct));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"struct")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_struct));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_struct_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_struct_decl));
	(my_ok)=((my_peg_P_struct)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"{")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_member_decl)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"}")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_struct_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_sub_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_sub_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"-")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_sub_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_type)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_type));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ident)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_byte)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_int)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_void)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_func)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_func_type)((my_c)));
	}
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ptr_type)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"(")));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)")")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_type));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_unary_expr)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_unary_expr));
	while (1) {
	(my_choice)((my_c));
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_ref_op)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_deref_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_pos_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_neg_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_not_op)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_bnot_op)((my_c)));
	}
	if (my_ok) {
	(my_commit)((my_c));
	} else {
	(my_fail)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_post_expr)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_unary_expr));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_union)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_union));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"union")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_union));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_union_decl)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_union_decl));
	(my_ok)=((my_peg_P_union)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"{")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	while (1) {
	(my_choice)((my_c));
	(my_ok)=((my_peg_P_member_decl)((my_c)));
	if ((unsigned long)(!(my_ok))) {
	(my_ok)=(1UL);
	break;
	}
	(my_commit)((my_c));
	}
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)"}")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_union_decl));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_var)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_var));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"var")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_var));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_var_stmt)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_var_stmt));
	(my_ok)=((my_peg_P_var)((my_c)));
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_ident)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)":")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_type)((my_c)));
	}
	if (my_ok) {
	(my_ok)=((my_literal)((my_c),((unsigned char *)";")));
	}
	if (my_ok) {
	(my_ok)=((my_peg_P_sp)((my_c)));
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_var_stmt));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_void)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_void));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"void")));
	if (my_ok) {
	(my_choice)((my_c));
	(my_ok)=((my_charset)((my_c),((unsigned char *)"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")));
	if (my_ok) {
	(my_fail)((my_c));
	(my_fail)((my_c));
	(my_ok)=(0UL);
	} else {
	(my_ok)=(1UL);
	}
	}
	if (my_ok) {
	(my_leave)((my_c),(my_P_void));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
unsigned long( my_peg_P_xor_op)(struct my_peg* my_c){
	unsigned long my_ok = 0;
	(my_enter)((my_c),(my_P_xor_op));
	(my_ok)=((my_literal)((my_c),((unsigned char *)"^")));
	if (my_ok) {
	(my_leave)((my_c),(my_P_xor_op));
	} else {
	(my_fail)((my_c));
	}
	return my_ok;
}
void( my_peg_compile)(struct my_peg_compiler* my_c,unsigned char* my_filename){
	unsigned long my_fd = 0;
	struct my_file* my_f = 0;
	unsigned char* my_src = 0;
	unsigned long my_len = 0;
	struct my_peg_node* my_node = 0;
	if ((unsigned long)(((long)((my_strcmp)((my_filename),((unsigned char *)"-"))))==((long)(0UL)))) {
	(my_fd)=(0UL);
	} else {
	(my_fd)=((my_open)((my_filename),(my_O_RDONLY),(0UL)));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"failed to open output"));
	}
	}
	(my_f)=((my_fopen)((my_fd),((my_c)->my_a)));
	(my_src)=((my_freadall)((my_f),(&(my_len))));
	(my_fclose)((my_f));
	((my_c)->my_p)=((my_peg_new)((my_filename),(my_src),(my_len),((my_c)->my_a),(my_peg_PEG_grammar),(my_PEG_tag_to_str)));
	(my_node)=((my_peg_parse)(((my_c)->my_p),(my_PEG_sp)));
	(my_translate)((my_c),(my_node));
	(my_fflush)(((my_c)->my_out));
}
struct my_peg*( my_peg_new)(unsigned char* my_filename,unsigned char* my_src,unsigned long my_len,struct my_alloc* my_a,unsigned long(* my_grammar)(struct my_peg*),unsigned char*(* my_tag_to_str)(unsigned long)){
	struct my_peg* my_c = 0;
	(my_c)=((struct my_peg*)(my_alloc)((my_a),(200UL)));
	((my_c)->my_a)=(my_a);
	((my_c)->my_grammar)=(my_grammar);
	((my_c)->my_tag_to_str)=(my_tag_to_str);
	((my_c)->my_filename)=(my_filename);
	((my_c)->my_src)=(my_src);
	((my_c)->my_size)=(my_len);
	((my_c)->my_pos)=(0UL);
	((my_c)->my_tag)=(0UL);
	((my_c)->my_line)=(1UL);
	((my_c)->my_col)=(1UL);
	((my_c)->my_tag)=(0UL);
	((my_c)->my_fail_depth)=(0UL);
	((my_c)->my_fail_tag)=(0UL);
	((my_c)->my_fail_line)=(0UL);
	((my_c)->my_fail_col)=(0UL);
	((my_c)->my_fail_literal)=((void *)0);
	((my_c)->my_limit)=(1024UL);
	((my_c)->my_stack)=((struct my_peg_frame*)(my_alloc)((my_a),((unsigned long)(((long)((my_c)->my_limit))*((long)(48UL))))));
	((my_c)->my_sp)=(0UL);
	((my_c)->my_depth)=(0UL);
	((my_c)->my_op)=(0UL);
	((my_c)->my_out)=((void *)0);
	((my_c)->my_cap)=(0UL);
	((my_c)->my_ncap)=(1024UL);
	((my_c)->my_nstack)=((struct my_peg_node**)(my_alloc)((my_a),((unsigned long)(((long)((my_c)->my_ncap))*((long)(8UL))))));
	((my_c)->my_np)=(0UL);
	return my_c;
}
void( my_peg_open_output)(struct my_peg_compiler* my_c,unsigned char* my_filename){
	unsigned long my_fd = 0;
	struct my_file* my_f = 0;
	(my_unlink)((my_filename));
	(my_fd)=((my_open)((my_filename),((unsigned long)(((unsigned long)(my_O_CREAT))|((unsigned long)(my_O_WRONLY)))),((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)((unsigned long)(((unsigned long)(6UL))<<((unsigned long)(6UL)))))+((unsigned long)((unsigned long)(((unsigned long)(6UL))<<((unsigned long)(3UL))))))))+((unsigned long)(6UL))))));
	if ((unsigned long)(((long)(my_fd))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"failed to open output"));
	}
	(my_f)=((my_fopen)((my_fd),((my_c)->my_a)));
	((my_c)->my_out)=(my_f);
}
struct my_peg_node*( my_peg_parse)(struct my_peg* my_c,unsigned long my_sp){
	(my_choice)((my_c));
	if ((unsigned long)(!(((my_c)->my_grammar)((my_c))))) {
	(my_fdputs)((2UL),((unsigned char *)"syntax error at "));
	(my_fdputs)((2UL),((my_c)->my_filename));
	(my_fdputs)((2UL),((unsigned char *)":"));
	(my_fdputd)((2UL),((my_c)->my_fail_line));
	(my_fdputs)((2UL),((unsigned char *)":"));
	(my_fdputd)((2UL),((my_c)->my_fail_col));
	(my_fdputs)((2UL),((unsigned char *)" expected "));
	(my_fdputs)((2UL),(((my_c)->my_tag_to_str)(((my_c)->my_fail_tag))));
	if ((my_c)->my_fail_literal) {
	(my_fdputs)((2UL),((unsigned char *)" '"));
	(my_fdputs)((2UL),((my_c)->my_fail_literal));
	(my_fdputs)((2UL),((unsigned char *)"'"));
	}
	(my_fdputs)((2UL),((unsigned char *)"\012"));
	(my_exit)((1UL));
	}
	(my_commit)((my_c));
	return (my_construct)((my_c),(my_sp));
}
void( my_peg_reset)(struct my_peg* my_c,unsigned char* my_filename,unsigned char* my_src,unsigned long my_len){
	((my_c)->my_filename)=(my_filename);
	((my_c)->my_src)=(my_src);
	((my_c)->my_size)=(my_len);
	((my_c)->my_pos)=(0UL);
	((my_c)->my_tag)=(0UL);
	((my_c)->my_line)=(1UL);
	((my_c)->my_col)=(1UL);
	((my_c)->my_tag)=(0UL);
	((my_c)->my_fail_depth)=(0UL);
	((my_c)->my_fail_tag)=(0UL);
	((my_c)->my_fail_line)=(0UL);
	((my_c)->my_fail_col)=(0UL);
	((my_c)->my_fail_literal)=((void *)0);
	((my_c)->my_depth)=(0UL);
	((my_c)->my_sp)=(0UL);
	((my_c)->my_op)=(0UL);
	((my_c)->my_np)=(0UL);
}
struct my_type*( my_prototype)(struct my_compiler* my_c,struct my_node* my_n){
	struct my_type* my_a = 0;
	struct my_type* my_b = 0;
	struct my_decl* my_st = 0;
	unsigned long my_kind = 0;
	if ((unsigned long)(!(my_n))) {
	return (void *)0;
	}
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=(0UL);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_IDENT)))) {
	if ((unsigned long)(!((my_strcmp)(((my_n)->my_s),((unsigned char *)"void"))))) {
	return (my_mktype0)((my_c),(my_TY_VOID));
	}
	if ((unsigned long)(!((my_strcmp)(((my_n)->my_s),((unsigned char *)"int"))))) {
	return (my_mktype0)((my_c),(my_TY_INT));
	}
	if ((unsigned long)(!((my_strcmp)(((my_n)->my_s),((unsigned char *)"byte"))))) {
	return (my_mktype0)((my_c),(my_TY_BYTE));
	}
	(my_st)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_st)))||((unsigned long)(!((my_st)->my_struct_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"unknown struct"));
	}
	if ((unsigned long)(((long)(((my_st)->my_struct_def)->my_kind))==((long)(my_N_STRUCT)))) {
	return (my_mktype_struct)((my_c),(my_st));
	} else {
	return (my_mktype_union)((my_c),(my_st));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ARGLIST)))) {
	(my_a)=((my_prototype)((my_c),(((my_n)->my_a)->my_b)));
	(my_b)=((my_prototype)((my_c),((my_n)->my_b)));
	(my_kind)=((my_a)->my_kind);
	if ((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_INT))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_BYTE))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_PTR))))&&((unsigned long)(((long)(my_kind))!=((long)(my_TY_FUNC)))))))))) {
	(my_cdie)((my_c),((unsigned char *)"not a ptr arg"));
	}
	return (my_mktype2)((my_c),(my_TY_ARG),(my_a),(my_b));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_FUNCTYPE)))) {
	if ((my_n)->my_b) {
	(my_a)=((my_prototype)((my_c),((my_n)->my_b)));
	} else {
	(my_a)=((my_mktype0)((my_c),(my_TY_VOID)));
	}
	(my_b)=((my_prototype)((my_c),((my_n)->my_a)));
	(my_kind)=((my_a)->my_kind);
	if ((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_VOID))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_INT))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_BYTE))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_PTR))))&&((unsigned long)(((long)(my_kind))!=((long)(my_TY_FUNC)))))))))))) {
	(my_cdie)((my_c),((unsigned char *)"not a ptr return"));
	}
	return (my_mktype2)((my_c),(my_TY_FUNC),(my_a),(my_b));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_PTRTYPE)))) {
	return (my_mktype1)((my_c),(my_TY_PTR),((my_prototype)((my_c),((my_n)->my_a))));
	} else {
	(my_cdie)((my_c),((unsigned char *)"prototype: invalid type"));
	}
}
void( my_putchar)(struct my_assembler* my_c,unsigned long my_ch){
	(my_fputc)(((my_c)->my_out),(my_ch));
}
unsigned long( my_read)(unsigned long my_fd,unsigned char* my_buf,unsigned long my_n){
	return (my_syscall)((0UL),(my_fd),((unsigned long)my_buf),(my_n),(0UL),(0UL),(0UL));
}
unsigned char*( my_readall)(unsigned long my_fd,unsigned long* my_len,struct my_alloc* my_a){
	unsigned char* my_buf = 0;
	unsigned char* my_tmp = 0;
	unsigned long my_cap = 0;
	unsigned long my_newcap = 0;
	unsigned long my_ret = 0;
	unsigned long my_n = 0;
	(my_cap)=(0UL);
	(my_n)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_n))==((long)(my_cap)))) {
	if ((unsigned long)(((long)(my_cap))==((long)(0UL)))) {
	(my_newcap)=(4096UL);
	} else {
	(my_newcap)=((unsigned long)(((long)(my_cap))*((long)(2UL))));
	}
	(my_tmp)=((my_alloc)((my_a),(my_newcap)));
	(my_memcpy)((my_tmp),(my_buf),(my_n));
	(my_free)((my_a),(my_buf));
	(my_buf)=(my_tmp);
	(my_cap)=(my_newcap);
	}
	(my_ret)=((my_read)((my_fd),(&((my_buf)[my_n])),((unsigned long)(((unsigned long)(my_cap))-((unsigned long)(my_n))))));
	if ((unsigned long)(((long)(my_ret))<((long)(0UL)))) {
	(my_die)(((unsigned char *)"readall failed"));
	}
	if ((unsigned long)(((long)(my_ret))==((long)(0UL)))) {
	break;
	}
	(my_n)=((unsigned long)(((unsigned long)(my_n))+((unsigned long)(my_ret))));
	}
	(*(my_len))=(my_n);
	return my_buf;
}
struct my_node*( my_reconstruct)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node** my_link = 0;
	struct my_node* my_n = 0;
	struct my_node* my_p = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_grammar)))),((unsigned char *)"grammar"));
	(my_ret)=((void *)0);
	(my_link)=(&(my_ret));
	(my_pn)=((my_pn)->my_child);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_enum_decl)))) {
	(my_n)=((my_reconstruct_enum)((my_c),(my_pn)));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_struct_decl)))) {
	(my_n)=((my_reconstruct_struct)((my_c),(my_pn)));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_union_decl)))) {
	(my_n)=((my_reconstruct_union)((my_c),(my_pn)));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_func_decl)))) {
	(my_n)=((my_reconstruct_func)((my_c),(my_pn)));
	} else {
	(my_die)(((unsigned char *)"invalid decl"));
	}
	(my_p)=((my_mknode1)((my_c),(my_N_PROGRAM),(my_n)));
	(my_copypos)((my_p),(my_pn));
	(*(my_link))=(my_p);
	(my_link)=(&((my_p)->my_b));
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_add)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	struct my_node** my_link = 0;
	unsigned long my_kind = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_add_expr)))),((unsigned char *)"add_expr"));
	(my_ret)=((my_reconstruct_mul)((my_c),((my_pn)->my_child)));
	(my_link)=(&(my_ret));
	(my_pn)=(((my_pn)->my_child)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_add_op)))) {
	(my_kind)=(my_N_ADD);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_sub_op)))) {
	(my_kind)=(my_N_SUB);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_or_op)))) {
	(my_kind)=(my_N_OR);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_xor_op)))) {
	(my_kind)=(my_N_XOR);
	} else {
	(my_die)(((unsigned char *)"invalid addop"));
	}
	(my_n)=((my_reconstruct_mul)((my_c),((my_pn)->my_next)));
	(*(my_link))=((my_mknode)((my_c),(my_kind),(*(my_link)),(my_n)));
	(my_copypos)((*(my_link)),(my_pn));
	(my_pn)=(((my_pn)->my_next)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_arg_decl)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_arg_decl)))),((unsigned char *)"arg_decl"));
	(my_a)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_b)=((my_reconstruct_type)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_ARGDECL),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_args)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node** my_link = 0;
	struct my_node* my_a = 0;
	struct my_node* my_n = 0;
	(my_ret)=((void *)0);
	(my_link)=(&(my_ret));
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	(my_a)=((my_reconstruct_expr)((my_c),(my_pn)));
	(my_n)=((my_mknode1)((my_c),(my_N_EXPRLIST),(my_a)));
	(my_copypos)((my_n),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_b));
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_assign)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_assign_stmt)))),((unsigned char *)"assign_stmt"));
	(my_a)=((my_reconstruct_unary)((my_c),((my_pn)->my_child)));
	(my_b)=((my_reconstruct_expr)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_ASSIGN),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_bool)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	struct my_node** my_link = 0;
	unsigned long my_kind = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_bool_expr)))),((unsigned char *)"bool_expr"));
	(my_ret)=((my_reconstruct_comp)((my_c),((my_pn)->my_child)));
	(my_link)=(&(my_ret));
	(my_pn)=(((my_pn)->my_child)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_band_op)))) {
	(my_kind)=(my_N_BAND);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_bor_op)))) {
	(my_kind)=(my_N_BOR);
	} else {
	(my_die)(((unsigned char *)"invalid boolop"));
	}
	(my_n)=((my_reconstruct_comp)((my_c),((my_pn)->my_next)));
	(*(my_link))=((my_mknode)((my_c),(my_kind),(*(my_link)),(my_n)));
	(my_copypos)((*(my_link)),(my_pn));
	(my_link)=(&((*(my_link))->my_b));
	(my_pn)=(((my_pn)->my_next)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_break)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_break_stmt)))),((unsigned char *)"break_stmt"));
	(my_n)=((my_mknode0)((my_c),(my_N_BREAK)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_char)(struct my_parser* my_c,struct my_peg_node* my_pn){
	unsigned long my_i = 0;
	unsigned long my_x = 0;
	unsigned long my_ok = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_char)))),((unsigned char *)"char"));
	(my_i)=(1UL);
	(my_x)=((my_unescape)(((my_pn)->my_str),(&(my_i)),((unsigned long)(((unsigned long)((my_pn)->my_len))-((unsigned long)(1UL)))),(&(my_ok))));
	if ((unsigned long)(((unsigned long)(!(my_ok)))||((unsigned long)(((long)(my_i))!=((long)((unsigned long)(((unsigned long)((my_pn)->my_len))-((unsigned long)(1UL))))))))) {
	(my_die)(((unsigned char *)"invalid character"));
	}
	(my_n)=((my_mknode0)((my_c),(my_N_CHAR)));
	(my_copypos)((my_n),(my_pn));
	((my_n)->my_n)=(my_x);
	return my_n;
}
struct my_node*( my_reconstruct_comp)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	unsigned long my_kind = 0;
	unsigned long my_tag = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_comp_expr)))),((unsigned char *)"comp_expr"));
	(my_a)=((my_reconstruct_add)((my_c),((my_pn)->my_child)));
	if ((unsigned long)(!(((my_pn)->my_child)->my_next))) {
	return my_a;
	}
	(my_tag)=((((my_pn)->my_child)->my_next)->my_tag);
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_le_op)))) {
	(my_kind)=(my_N_LE);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_ge_op)))) {
	(my_kind)=(my_N_GE);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_lt_op)))) {
	(my_kind)=(my_N_LT);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_gt_op)))) {
	(my_kind)=(my_N_GT);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_eq_op)))) {
	(my_kind)=(my_N_EQ);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_ne_op)))) {
	(my_kind)=(my_N_NE);
	} else {
	(my_die)(((unsigned char *)"invalid compop"));
	}
	(my_b)=((my_reconstruct_add)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_kind),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_compound)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	struct my_node** my_link = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_compound_stmt)))),((unsigned char *)"compound_stmt"));
	(my_pn)=((my_pn)->my_child);
	(my_ret)=((void *)0);
	(my_link)=(&(my_ret));
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	(my_a)=((my_reconstruct_stmt)((my_c),(my_pn)));
	if (my_a) {
	(my_n)=((my_mknode1)((my_c),(my_N_STMTLIST),(my_a)));
	(my_copypos)((my_n),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_b));
	}
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_continue)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_continue_stmt)))),((unsigned char *)"continue_stmt"));
	(my_n)=((my_mknode0)((my_c),(my_N_CONTINUE)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_dec)(struct my_parser* my_c,struct my_peg_node* my_pn){
	unsigned long my_x = 0;
	struct my_node* my_n = 0;
	unsigned long my_ok = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_dec)))),((unsigned char *)"dec"));
	(my_x)=((my_dec2int)(((my_pn)->my_str),((my_pn)->my_len),(&(my_ok))));
	if ((unsigned long)(!(my_ok))) {
	(my_die)(((unsigned char *)"invalid dec"));
	}
	(my_n)=((my_mknode0)((my_c),(my_N_NUM)));
	(my_copypos)((my_n),(my_pn));
	((my_n)->my_n)=(my_x);
	return my_n;
}
struct my_node*( my_reconstruct_empty)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_empty_stmt)))),((unsigned char *)"empty_stmt"));
	return (void *)0;
}
struct my_node*( my_reconstruct_enum)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node** my_link = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_enum_decl)))),((unsigned char *)"enum_decl"));
	(my_n)=((my_mknode)((my_c),(my_N_ENUM),((void *)0),((void *)0)));
	(my_copypos)((my_n),(my_pn));
	(my_pn)=(((my_pn)->my_child)->my_next);
	(my_link)=(&((my_n)->my_b));
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	(my_a)=((my_mknode1)((my_c),(my_N_ENUMLIST),((my_reconstruct_enum_item)((my_c),(my_pn)))));
	(my_copypos)((my_a),(my_pn));
	(*(my_link))=(my_a);
	(my_link)=(&((my_a)->my_b));
	(my_pn)=((my_pn)->my_next);
	}
	return my_n;
}
struct my_node*( my_reconstruct_enum_item)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_enum_item)))),((unsigned char *)"enum_item"));
	(my_a)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_b)=((void *)0);
	if (((my_pn)->my_child)->my_next) {
	(my_b)=((my_reconstruct_expr)((my_c),(((my_pn)->my_child)->my_next)));
	(my_assert)(((unsigned long)(((long)((my_b)->my_kind))==((long)(my_N_NUM)))),((unsigned char *)"enum must be num"));
	}
	(my_n)=((my_mknode)((my_c),(my_N_ENUMITEM),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_expr)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_expr)))),((unsigned char *)"expr"));
	return (my_reconstruct_bool)((my_c),((my_pn)->my_child));
}
struct my_node*( my_reconstruct_expr_stmt)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_expr_stmt)))),((unsigned char *)"expr_stmt"));
	return (my_reconstruct_expr)((my_c),((my_pn)->my_child));
}
struct my_node*( my_reconstruct_func)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_d = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_func_decl)))),((unsigned char *)"func_def"));
	(my_a)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_b)=((my_reconstruct_func_type)((my_c),(((my_pn)->my_child)->my_next)));
	(my_d)=((my_mknode)((my_c),(my_N_FUNCDECL),(my_a),(my_b)));
	(my_copypos)((my_d),(my_pn));
	if ((unsigned long)(!((((my_pn)->my_child)->my_next)->my_next))) {
	return my_d;
	}
	(my_b)=((my_reconstruct_compound)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_FUNC),(my_d),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_func_type)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	struct my_node** my_link = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_func_type)))),((unsigned char *)"func_type"));
	(my_n)=((my_mknode)((my_c),(my_N_FUNCTYPE),((void *)0),((void *)0)));
	(my_copypos)((my_n),(my_pn));
	(my_pn)=((my_pn)->my_child);
	(my_link)=(&((my_n)->my_a));
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_arg_decl)))) {
	(my_a)=((my_mknode1)((my_c),(my_N_ARGLIST),((my_reconstruct_arg_decl)((my_c),(my_pn)))));
	(my_copypos)((my_a),(my_pn));
	(*(my_link))=(my_a);
	(my_link)=(&((my_a)->my_b));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_type)))) {
	((my_n)->my_b)=((my_reconstruct_type)((my_c),(my_pn)));
	} else {
	(my_die)(((unsigned char *)"invalid func_type"));
	}
	(my_pn)=((my_pn)->my_next);
	}
	return my_n;
}
struct my_node*( my_reconstruct_goto)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_goto_stmt)))),((unsigned char *)"goto_stmt"));
	(my_a)=((my_reconstruct_ident)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode1)((my_c),(my_N_GOTO),(my_a)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_hex)(struct my_parser* my_c,struct my_peg_node* my_pn){
	unsigned long my_x = 0;
	struct my_node* my_n = 0;
	unsigned long my_ok = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_hex)))),((unsigned char *)"hex"));
	(my_x)=((my_hex2int)((&(((my_pn)->my_str)[2UL])),((unsigned long)(((unsigned long)((my_pn)->my_len))-((unsigned long)(2UL)))),(&(my_ok))));
	if ((unsigned long)(!(my_ok))) {
	(my_die)(((unsigned char *)"invalid hex"));
	}
	(my_n)=((my_mknode0)((my_c),(my_N_NUM)));
	(my_copypos)((my_n),(my_pn));
	((my_n)->my_n)=(my_x);
	return my_n;
}
struct my_node*( my_reconstruct_ident)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_ident)))),((unsigned char *)"ident"));
	return (my_reconstruct_intern)((my_c),(my_pn),((my_pn)->my_str),((my_pn)->my_len));
}
struct my_node*( my_reconstruct_if)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node** my_link = 0;
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_if_stmt)))),((unsigned char *)"if_stmt"));
	(my_a)=((my_reconstruct_expr)((my_c),(((my_pn)->my_child)->my_next)));
	(my_b)=((my_reconstruct_compound)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_ret)=((my_mknode1)((my_c),(my_N_CONDLIST),((my_mknode)((my_c),(my_N_COND),(my_a),(my_b)))));
	(my_copypos)((my_ret),(my_pn));
	(my_copypos)(((my_ret)->my_a),(my_pn));
	(my_link)=(&((my_ret)->my_b));
	(my_pn)=(((((my_pn)->my_child)->my_next)->my_next)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_elif_stmt)))) {
	(my_a)=((my_reconstruct_expr)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_b)=((my_reconstruct_compound)((my_c),(((((my_pn)->my_child)->my_next)->my_next)->my_next)));
	(my_n)=((my_mknode1)((my_c),(my_N_CONDLIST),((my_mknode)((my_c),(my_N_COND),(my_a),(my_b)))));
	(my_copypos)((my_n),(my_pn));
	(my_copypos)(((my_n)->my_a),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_b));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_else_stmt)))) {
	(my_a)=((void *)0);
	(my_b)=((my_reconstruct_compound)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode1)((my_c),(my_N_CONDLIST),((my_mknode)((my_c),(my_N_COND),(my_a),(my_b)))));
	(my_copypos)((my_n),(my_pn));
	(my_copypos)(((my_n)->my_a),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_b));
	break;
	} else {
	(my_die)(((unsigned char *)"invalid if_stmt"));
	}
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_intern)(struct my_parser* my_c,struct my_peg_node* my_pn,unsigned char* my_s,unsigned long my_len){
	struct my_node* my_n = 0;
	unsigned long my_i = 0;
	(my_n)=((my_mknode0)((my_c),(my_N_IDENT)));
	(my_copypos)((my_n),(my_pn));
	(my_i)=(0UL);
	((my_n)->my_s)=((my_alloc)(((my_c)->my_a),((unsigned long)(((unsigned long)(my_len))+((unsigned long)(1UL))))));
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(((my_n)->my_s)[my_i])=((my_s)[my_i]);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(((my_n)->my_s)[my_i])=((unsigned char)0UL);
	return my_n;
}
struct my_node*( my_reconstruct_label)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_label_stmt)))),((unsigned char *)"label_stmt"));
	(my_a)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_n)=((my_mknode1)((my_c),(my_N_LABEL),(my_a)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_loop)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_loop_stmt)))),((unsigned char *)"loop_stmt"));
	(my_a)=((my_reconstruct_compound)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode1)((my_c),(my_N_LOOP),(my_a)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_member_decl)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_member_decl)))),((unsigned char *)"member_decl"));
	(my_a)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_b)=((my_reconstruct_type)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_MEMBERDECL),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_member_list)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	struct my_node* my_ret = 0;
	struct my_node** my_link = 0;
	(my_ret)=((void *)0);
	(my_link)=(&(my_ret));
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	(my_a)=((my_reconstruct_member_decl)((my_c),(my_pn)));
	(my_n)=((my_mknode1)((my_c),(my_N_MEMBERLIST),(my_a)));
	(my_copypos)((my_n),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_b));
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_mul)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	struct my_node** my_link = 0;
	unsigned long my_kind = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_mul_expr)))),((unsigned char *)"mul_expr"));
	(my_ret)=((my_reconstruct_shift)((my_c),((my_pn)->my_child)));
	(my_link)=(&(my_ret));
	(my_pn)=(((my_pn)->my_child)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_mul_op)))) {
	(my_kind)=(my_N_MUL);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_div_op)))) {
	(my_kind)=(my_N_DIV);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_mod_op)))) {
	(my_kind)=(my_N_MOD);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_and_op)))) {
	(my_kind)=(my_N_AND);
	} else {
	(my_die)(((unsigned char *)"invalid mulop"));
	}
	(my_n)=((my_reconstruct_shift)((my_c),((my_pn)->my_next)));
	(*(my_link))=((my_mknode)((my_c),(my_kind),(*(my_link)),(my_n)));
	(my_copypos)((*(my_link)),(my_pn));
	(my_pn)=(((my_pn)->my_next)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_nil)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_nil)))),((unsigned char *)"nil"));
	(my_n)=((my_mknode0)((my_c),(my_N_NIL)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_post)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	unsigned long my_tag = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_post_expr)))),((unsigned char *)"post_expr"));
	(my_ret)=((my_reconstruct_primary)((my_c),((my_pn)->my_child)));
	(my_pn)=(((my_pn)->my_child)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	(my_tag)=((my_pn)->my_tag);
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_index_expr)))) {
	(my_n)=((my_reconstruct_expr)((my_c),((my_pn)->my_child)));
	(my_ret)=((my_mknode)((my_c),(my_N_INDEX),(my_ret),(my_n)));
	(my_copypos)((my_ret),(my_pn));
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_call_expr)))) {
	(my_n)=((my_reconstruct_args)((my_c),((my_pn)->my_child)));
	(my_ret)=((my_mknode)((my_c),(my_N_CALL),(my_ret),(my_n)));
	(my_copypos)((my_ret),(my_pn));
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_member_expr)))) {
	(my_n)=((my_reconstruct_ident)((my_c),((my_pn)->my_child)));
	(my_ret)=((my_mknode)((my_c),(my_N_DOT),(my_ret),(my_n)));
	(my_copypos)((my_ret),(my_pn));
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_cast_expr)))) {
	(my_n)=((my_reconstruct_type)((my_c),((my_pn)->my_child)));
	(my_ret)=((my_mknode)((my_c),(my_N_CAST),(my_ret),(my_n)));
	(my_copypos)((my_ret),(my_pn));
	} else {
	(my_die)(((unsigned char *)"invalid post_expr"));
	}
	(my_pn)=((my_pn)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_primary)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_primary)))),((unsigned char *)"primary"));
	(my_pn)=((my_pn)->my_child);
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_ident)))) {
	return (my_reconstruct_ident)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_hex)))) {
	return (my_reconstruct_hex)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_dec)))) {
	return (my_reconstruct_dec)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_str)))) {
	return (my_reconstruct_str)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_char)))) {
	return (my_reconstruct_char)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_sizeof_expr)))) {
	return (my_reconstruct_sizeof)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_nil)))) {
	return (my_reconstruct_nil)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_expr)))) {
	return (my_reconstruct_expr)((my_c),(my_pn));
	} else {
	(my_die)(((unsigned char *)"invalid primary"));
	}
}
struct my_node*( my_reconstruct_return)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_return_stmt)))),((unsigned char *)"return_stmt"));
	(my_a)=((void *)0);
	if (((my_pn)->my_child)->my_next) {
	(my_a)=((my_reconstruct_expr)((my_c),(((my_pn)->my_child)->my_next)));
	}
	(my_n)=((my_mknode1)((my_c),(my_N_RETURN),(my_a)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_shift)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_ret = 0;
	struct my_node* my_n = 0;
	struct my_node** my_link = 0;
	unsigned long my_kind = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_shift_expr)))),((unsigned char *)"shift_expr"));
	(my_ret)=((my_reconstruct_unary)((my_c),((my_pn)->my_child)));
	(my_link)=(&(my_ret));
	(my_pn)=(((my_pn)->my_child)->my_next);
	while (1) {
	if ((unsigned long)(!(my_pn))) {
	break;
	}
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_lsh_op)))) {
	(my_kind)=(my_N_LSH);
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_rsh_op)))) {
	(my_kind)=(my_N_RSH);
	} else {
	(my_die)(((unsigned char *)"invalid shift"));
	}
	(my_n)=((my_reconstruct_unary)((my_c),((my_pn)->my_next)));
	(*(my_link))=((my_mknode)((my_c),(my_kind),(*(my_link)),(my_n)));
	(my_copypos)((*(my_link)),(my_pn));
	(my_pn)=(((my_pn)->my_next)->my_next);
	}
	return my_ret;
}
struct my_node*( my_reconstruct_sizeof)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_sizeof_expr)))),((unsigned char *)"sizeof_expr"));
	(my_a)=((my_reconstruct_expr)((my_c),(((my_pn)->my_child)->my_next)));
	(my_n)=((my_mknode1)((my_c),(my_N_SIZEOF),(my_a)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_stmt)(struct my_parser* my_c,struct my_peg_node* my_pn){
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_stmt)))),((unsigned char *)"stmt"));
	(my_pn)=((my_pn)->my_child);
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_if_stmt)))) {
	return (my_reconstruct_if)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_loop_stmt)))) {
	return (my_reconstruct_loop)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_break_stmt)))) {
	return (my_reconstruct_break)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_continue_stmt)))) {
	return (my_reconstruct_continue)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_return_stmt)))) {
	return (my_reconstruct_return)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_var_stmt)))) {
	return (my_reconstruct_var)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_label_stmt)))) {
	return (my_reconstruct_label)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_goto_stmt)))) {
	return (my_reconstruct_goto)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_assign_stmt)))) {
	return (my_reconstruct_assign)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_expr_stmt)))) {
	return (my_reconstruct_expr_stmt)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_empty_stmt)))) {
	return (my_reconstruct_empty)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_compound_stmt)))) {
	return (my_reconstruct_compound)((my_c),(my_pn));
	} else {
	(my_die)(((unsigned char *)"invalid stmt"));
	}
}
struct my_node*( my_reconstruct_str)(struct my_parser* my_c,struct my_peg_node* my_pn){
	unsigned long my_i = 0;
	unsigned long my_j = 0;
	unsigned long my_len = 0;
	unsigned long my_ch = 0;
	unsigned long my_ok = 0;
	unsigned char* my_s = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_str)))),((unsigned char *)"str"));
	(my_s)=((my_alloc)(((my_c)->my_a),((my_pn)->my_len)));
	(my_i)=(1UL);
	(my_j)=(0UL);
	(my_len)=((unsigned long)(((unsigned long)((my_pn)->my_len))-((unsigned long)(1UL))));
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(my_ch)=((my_unescape)(((my_pn)->my_str),(&(my_i)),(my_len),(&(my_ok))));
	if ((unsigned long)(!(my_ok))) {
	(my_die)(((unsigned char *)"invalid escape"));
	}
	((my_s)[my_j])=((unsigned char)my_ch);
	(my_j)=((unsigned long)(((unsigned long)(my_j))+((unsigned long)(1UL))));
	}
	((my_s)[my_j])=((unsigned char)0UL);
	(my_n)=((my_mknode0)((my_c),(my_N_STR)));
	(my_copypos)((my_n),(my_pn));
	((my_n)->my_s)=(my_s);
	return my_n;
}
struct my_node*( my_reconstruct_struct)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_struct_decl)))),((unsigned char *)"struct_def"));
	(my_a)=((my_reconstruct_ident)((my_c),(((my_pn)->my_child)->my_next)));
	(my_b)=((my_reconstruct_member_list)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_STRUCT),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_type)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_type)))),((unsigned char *)"type"));
	(my_pn)=((my_pn)->my_child);
	if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_ident)))) {
	return (my_reconstruct_ident)((my_c),(my_pn));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_byte)))) {
	return (my_reconstruct_intern)((my_c),(my_pn),((unsigned char *)"byte"),(4UL));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_int)))) {
	return (my_reconstruct_intern)((my_c),(my_pn),((unsigned char *)"int"),(3UL));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_void)))) {
	return (my_reconstruct_intern)((my_c),(my_pn),((unsigned char *)"void"),(4UL));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_ptr_type)))) {
	(my_n)=((my_mknode1)((my_c),(my_N_PTRTYPE),((my_reconstruct_type)((my_c),((my_pn)->my_child)))));
	(my_copypos)((my_n),(my_pn));
	return my_n;
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_func)))) {
	return (my_reconstruct_func_type)((my_c),((my_pn)->my_next));
	} else if ((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_type)))) {
	return (my_reconstruct_type)((my_c),(my_pn));
	} else {
	(my_die)(((unsigned char *)"reconstruct_type: invalid type"));
	}
}
struct my_node*( my_reconstruct_unary)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_ret = 0;
	struct my_node** my_link = 0;
	unsigned long my_tag = 0;
	unsigned long my_kind = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_unary_expr)))),((unsigned char *)"unary_expr"));
	(my_link)=(&(my_ret));
	(my_pn)=((my_pn)->my_child);
	while (1) {
	(my_tag)=((my_pn)->my_tag);
	if ((unsigned long)(((long)(my_tag))==((long)(my_P_ref_op)))) {
	(my_kind)=(my_N_REF);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_deref_op)))) {
	(my_kind)=(my_N_DEREF);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_pos_op)))) {
	(my_kind)=(my_N_POS);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_neg_op)))) {
	(my_kind)=(my_N_NEG);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_not_op)))) {
	(my_kind)=(my_N_NOT);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_bnot_op)))) {
	(my_kind)=(my_N_BNOT);
	} else if ((unsigned long)(((long)(my_tag))==((long)(my_P_post_expr)))) {
	(*(my_link))=((my_reconstruct_post)((my_c),(my_pn)));
	return my_ret;
	} else {
	(my_die)(((unsigned char *)"invalid unary_expr"));
	}
	(my_n)=((my_mknode0)((my_c),(my_kind)));
	(my_copypos)((my_n),(my_pn));
	(*(my_link))=(my_n);
	(my_link)=(&((my_n)->my_a));
	(my_pn)=((my_pn)->my_next);
	}
}
struct my_node*( my_reconstruct_union)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_n = 0;
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_union_decl)))),((unsigned char *)"union_def"));
	(my_a)=((my_reconstruct_ident)((my_c),(((my_pn)->my_child)->my_next)));
	(my_b)=((my_reconstruct_member_list)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_UNION),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
struct my_node*( my_reconstruct_var)(struct my_parser* my_c,struct my_peg_node* my_pn){
	struct my_node* my_a = 0;
	struct my_node* my_b = 0;
	struct my_node* my_n = 0;
	(my_assert)(((unsigned long)(((long)((my_pn)->my_tag))==((long)(my_P_var_stmt)))),((unsigned char *)"var_stmt"));
	(my_a)=((my_reconstruct_ident)((my_c),(((my_pn)->my_child)->my_next)));
	(my_b)=((my_reconstruct_type)((my_c),((((my_pn)->my_child)->my_next)->my_next)));
	(my_n)=((my_mknode)((my_c),(my_N_VARDECL),(my_a),(my_b)));
	(my_copypos)((my_n),(my_pn));
	return my_n;
}
void( my_reserve)(struct my_assembler* my_c,unsigned long my_n){
	unsigned char* my_m = 0;
	struct my_chunk* my_b = 0;
	if ((unsigned long)(((my_c)->my_text_end)&&((unsigned long)(((long)((unsigned long)(((unsigned long)(((my_c)->my_text_end)->my_cap))-((unsigned long)(((my_c)->my_text_end)->my_fill)))))>=((long)(my_n)))))) {
	return;
	}
	if ((unsigned long)(((long)(my_n))<((long)(4096UL)))) {
	(my_n)=(4096UL);
	}
	(my_m)=((my_alloc)(((my_c)->my_a),(my_n)));
	(my_b)=((struct my_chunk*)(my_alloc)(((my_c)->my_a),(32UL)));
	((my_b)->my_buf)=(my_m);
	((my_b)->my_fill)=(0UL);
	((my_b)->my_cap)=(my_n);
	((my_b)->my_next)=((void *)0);
	if ((my_c)->my_text_end) {
	(((my_c)->my_text_end)->my_next)=(my_b);
	((my_c)->my_text_end)=(my_b);
	} else {
	((my_c)->my_text)=(my_b);
	((my_c)->my_text_end)=(my_b);
	}
}
void( my_setup_alloc)(struct my_alloc* my_c){
	((my_c)->my_page)=((void *)0);
}
struct my_assembler*( my_setup_assembler)(struct my_alloc* my_a){
	struct my_assembler* my_c = 0;
	(my_c)=((struct my_assembler*)(my_alloc)((my_a),(72UL)));
	((my_c)->my_a)=(my_a);
	((my_c)->my_out)=((void *)0);
	((my_c)->my_at)=(160UL);
	((my_c)->my_text)=((void *)0);
	((my_c)->my_text_end)=((void *)0);
	((my_c)->my_bits32)=(0UL);
	((my_c)->my_symbols)=((void *)0);
	((my_c)->my_num_sections)=(0UL);
	(my_add_section)((my_c),((unsigned char *)""),(my_SHT_NULL));
	(my_add_section)((my_c),((unsigned char *)".text"),(my_SHT_PROGBITS));
	return my_c;
}
struct my_parser*( my_setup_parser)(struct my_alloc* my_a){
	struct my_parser* my_c = 0;
	(my_c)=((struct my_parser*)(my_alloc)((my_a),(16UL)));
	((my_c)->my_a)=(my_a);
	((my_c)->my_p)=((my_peg_new)(((unsigned char *)""),((unsigned char *)""),(0UL),(my_a),(my_peg_P_grammar),(my_P_tag_to_str)));
	return my_c;
}
struct my_peg_compiler*( my_setup_peg)(struct my_alloc* my_a,unsigned char* my_prefix){
	struct my_peg_compiler* my_c = 0;
	(my_c)=((struct my_peg_compiler*)(my_alloc)((my_a),(40UL)));
	((my_c)->my_a)=(my_a);
	((my_c)->my_prefix)=(my_prefix);
	((my_c)->my_scratch)=((my_alloc)(((my_c)->my_a),(256UL)));
	return my_c;
}
void( my_show_node)(struct my_file* my_out,struct my_node* my_n){
	unsigned long my_i = 0;
	unsigned long my_ch = 0;
	unsigned char* my_hex = 0;
	(my_hex)=((unsigned char *)"0123456789abcdef");
	if ((unsigned long)(!(my_n))) {
	return;
	}
	(my_fputc)((my_out),(40));
	(my_fputs)((my_out),((my_node_to_str)(((my_n)->my_kind))));
	if ((unsigned long)(((long)((my_n)->my_kind))==((long)(my_N_NUM)))) {
	(my_fputc)((my_out),(32));
	(my_fputd)((my_out),((my_n)->my_n));
	}
	if ((my_n)->my_s) {
	(my_fputc)((my_out),(32));
	(my_fputc)((my_out),(34));
	(my_i)=(0UL);
	while (1) {
	(my_ch)=((unsigned long)((my_n)->my_s)[my_i]);
	if ((unsigned long)(!(my_ch))) {
	break;
	}
	if ((unsigned long)(((unsigned long)(((long)(my_ch))<((long)(32UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))>((long)(127UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))==((long)(92))))||((unsigned long)(((long)(my_ch))==((long)(34)))))))))) {
	(my_fputc)((my_out),(92));
	(my_fputc)((my_out),(120));
	(my_fputc)((my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_ch))>>((unsigned long)(4UL)))]));
	(my_fputc)((my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_ch))&((unsigned long)(15UL)))]));
	} else {
	(my_fputc)((my_out),(my_ch));
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_fputc)((my_out),(34));
	}
	if ((my_n)->my_a) {
	(my_fputc)((my_out),(32));
	(my_show_node)((my_out),((my_n)->my_a));
	}
	if ((my_n)->my_b) {
	(my_fputc)((my_out),(32));
	(my_show_node)((my_out),((my_n)->my_b));
	}
	(my_fputc)((my_out),(41));
}
unsigned long( my_strcmp)(unsigned char* my_a,unsigned char* my_b){
	unsigned long my_i = 0;
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)((my_a)[my_i]))>((long)((my_b)[my_i])))) {
	return 1UL;
	}
	if ((unsigned long)(((long)((my_a)[my_i]))<((long)((my_b)[my_i])))) {
	return (unsigned long)(-(unsigned long)(1UL));
	}
	if ((unsigned long)(((long)((my_a)[my_i]))==((long)((unsigned char)0UL)))) {
	return 0UL;
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
}
unsigned long( my_strlen)(unsigned char* my_s){
	unsigned long my_ret = 0;
	(my_ret)=(0UL);
	while (1) {
	if ((unsigned long)(((long)((my_s)[my_ret]))==((long)((unsigned char)0UL)))) {
	return my_ret;
	}
	(my_ret)=((unsigned long)(((unsigned long)(my_ret))+((unsigned long)(1UL))));
	}
}
void( my_translate)(struct my_peg_compiler* my_c,struct my_peg_node* my_n){
	struct my_peg_node* my_v = 0;
	(my_fputs)(((my_c)->my_out),((unsigned char *)"enum {\012"));
	(my_v)=((my_n)->my_child);
	while (1) {
	if ((unsigned long)(!(my_v))) {
	break;
	}
	if ((unsigned long)(((long)((my_v)->my_tag))==((long)(my_PEG_rule)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    "));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)",\012"));
	}
	(my_v)=((my_v)->my_next);
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"}\012\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"func "));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"tag_to_str(tag: int): *byte {\012"));
	(my_v)=((my_n)->my_child);
	while (1) {
	if ((unsigned long)(!(my_v))) {
	break;
	}
	if ((unsigned long)(((long)((my_v)->my_tag))==((long)(my_PEG_rule)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if tag == "));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)" { return \042"));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"\042; }\012"));
	}
	(my_v)=((my_v)->my_next);
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    die(\042invalid tag\042);\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"}\012"));
	(my_v)=((my_n)->my_child);
	while (1) {
	if ((unsigned long)(!(my_v))) {
	break;
	}
	if ((unsigned long)(((long)((my_v)->my_tag))==((long)(my_PEG_rule)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"\012func peg_"));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"(c: *peg): int {\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    var ok: int;\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    enter(c, "));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)");\012"));
	(my_translate_pattern)((my_c),(((my_v)->my_child)->my_next));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok { leave(c, "));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_v)->my_child)->my_str),(((my_v)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"); } else { fail(c); }\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    return ok;\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"}\012"));
	}
	(my_v)=((my_v)->my_next);
	}
}
void( my_translate_charset)(struct my_peg_compiler* my_c,struct my_peg_node* my_n){
	unsigned long my_i = 0;
	unsigned long my_len = 0;
	unsigned long my_ch = 0;
	unsigned long my_a = 0;
	unsigned long my_b = 0;
	unsigned char* my_hex = 0;
	unsigned long my_count = 0;
	(my_hex)=((unsigned char *)"0123456789abcdef");
	(my_memset)(((my_c)->my_scratch),(0UL),(256UL));
	(my_i)=(1UL);
	(my_len)=((unsigned long)(((unsigned long)((my_n)->my_len))-((unsigned long)(1UL))));
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(my_ch)=((unsigned long)((my_n)->my_str)[my_i]);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))==((long)(92)))) {
	(my_ch)=((my_parse_escape)(((my_n)->my_str),(&(my_i)),(my_len)));
	}
	if ((unsigned long)(((unsigned long)(((long)(my_i))<((long)(my_len))))&&((unsigned long)(((long)(((my_n)->my_str)[my_i]))==((long)((unsigned char)45)))))) {
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	(my_die)(((unsigned char *)"invalid range"));
	}
	(my_a)=(my_ch);
	(my_ch)=((unsigned long)((my_n)->my_str)[my_i]);
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))==((long)(92)))) {
	(my_ch)=((my_parse_escape)(((my_n)->my_str),(&(my_i)),(my_len)));
	}
	(my_b)=(my_ch);
	while (1) {
	if ((unsigned long)(((long)(my_a))>((long)(my_b)))) {
	break;
	}
	(((my_c)->my_scratch)[my_a])=((unsigned char)1UL);
	(my_a)=((unsigned long)(((unsigned long)(my_a))+((unsigned long)(1UL))));
	}
	} else {
	(((my_c)->my_scratch)[my_ch])=((unsigned char)1UL);
	}
	}
	(my_count)=(0UL);
	(my_i)=(1UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(256UL)))) {
	break;
	}
	(my_count)=((unsigned long)(((unsigned long)(my_count))+((unsigned long)((unsigned long)((my_c)->my_scratch)[my_i]))));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	if ((unsigned long)(((long)(my_count))==((long)(0UL)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    fail(c);\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = 0;\012"));
	return;
	} else if ((unsigned long)(((long)(my_count))>=((long)(255UL)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = any(c);\012"));
	return;
	} else if ((unsigned long)(((long)(my_count))==((long)(1UL)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = literal(c, \042"));
	} else {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = charset(c, \042"));
	}
	(my_i)=(1UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(256UL)))) {
	break;
	}
	if (((my_c)->my_scratch)[my_i]) {
	if ((unsigned long)(((unsigned long)(((long)(my_ch))<((long)(32UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))>((long)(127UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))==((long)(92))))||((unsigned long)(((long)(my_ch))==((long)(34)))))))))) {
	(my_fputc)(((my_c)->my_out),(92));
	(my_fputc)(((my_c)->my_out),(120));
	(my_fputc)(((my_c)->my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_i))>>((unsigned long)(4UL)))]));
	(my_fputc)(((my_c)->my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_i))&((unsigned long)(15UL)))]));
	} else {
	(my_fputc)(((my_c)->my_out),(my_i));
	}
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"\042);\012"));
}
void( my_translate_literal)(struct my_peg_compiler* my_c,struct my_peg_node* my_n){
	unsigned long my_i = 0;
	unsigned long my_len = 0;
	unsigned long my_ch = 0;
	unsigned char* my_hex = 0;
	(my_hex)=((unsigned char *)"0123456789abcdef");
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = literal(c, \042"));
	(my_i)=(1UL);
	(my_len)=((unsigned long)(((unsigned long)((my_n)->my_len))-((unsigned long)(1UL))));
	while (1) {
	if ((unsigned long)(((long)(my_i))==((long)(my_len)))) {
	break;
	}
	(my_ch)=((unsigned long)((my_n)->my_str)[my_i]);
	if ((unsigned long)(((unsigned long)(((long)(my_ch))<((long)(32UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))>((long)(127UL))))||((unsigned long)(((unsigned long)(((long)(my_ch))==((long)(92))))||((unsigned long)(((long)(my_ch))==((long)(34)))))))))) {
	(my_fputc)(((my_c)->my_out),(92));
	(my_fputc)(((my_c)->my_out),(120));
	(my_fputc)(((my_c)->my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_ch))>>((unsigned long)(4UL)))]));
	(my_fputc)(((my_c)->my_out),((unsigned long)(my_hex)[(unsigned long)(((unsigned long)(my_ch))&((unsigned long)(15UL)))]));
	} else {
	(my_fputc)(((my_c)->my_out),(my_ch));
	}
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"\042);\012"));
}
void( my_translate_pattern)(struct my_peg_compiler* my_c,struct my_peg_node* my_n){
	unsigned long my_count = 0;
	unsigned long my_look = 0;
	struct my_peg_node* my_d = 0;
	while (1) {
	if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_pattern)))) {
	(my_d)=((my_n)->my_child);
	if ((unsigned long)(!((my_d)->my_next))) {
	(my_translate_pattern)((my_c),(my_d));
	} else {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),(my_d));
	(my_d)=((my_d)->my_next);
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if !ok { choice(c);\012"));
	(my_translate_pattern)((my_c),(my_d));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    }\012"));
	(my_d)=((my_d)->my_next);
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok { commit(c); } else { fail(c); }\012"));
	}
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_alternative)))) {
	(my_d)=((my_n)->my_child);
	(my_translate_pattern)((my_c),(my_d));
	(my_d)=((my_d)->my_next);
	while (1) {
	if ((unsigned long)(!(my_d))) {
	break;
	}
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok {\012"));
	(my_translate_pattern)((my_c),(my_d));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    }\012"));
	(my_d)=((my_d)->my_next);
	}
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_lookahead)))) {
	(my_look)=((my_decode_look)((my_n)));
	(my_d)=((my_n)->my_child);
	if ((unsigned long)(((long)((my_d)->my_tag))==((long)(my_PEG_lookop)))) {
	(my_d)=((my_d)->my_next);
	}
	if ((unsigned long)(((long)(my_look))==((long)(my_LOOK_AND)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),(my_d));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    fail(c);\012"));
	} else if ((unsigned long)(((long)(my_look))==((long)(my_LOOK_NOT)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),(my_d));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok { fail(c); fail(c); ok = 0; } else { ok = 1; }\012"));
	} else if ((unsigned long)(((long)(my_look))==((long)(my_LOOK_NORMAL)))) {
	(my_translate_pattern)((my_c),(my_d));
	} else {
	(my_die)(((unsigned char *)"invalid lookop"));
	}
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_suffix)))) {
	(my_count)=((my_decode_count)((my_n)));
	if ((unsigned long)(((long)(my_count))==((long)(my_ZERO_OR_ONE)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),((my_n)->my_child));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok { commit(c); } else { ok = 1; }\012"));
	} else if ((unsigned long)(((long)(my_count))==((long)(my_EXACTLY_ONE)))) {
	(my_translate_pattern)((my_c),((my_n)->my_child));
	} else if ((unsigned long)(((long)(my_count))==((long)(my_ZERO_OR_MORE)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    loop {\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),((my_n)->my_child));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if !ok { ok = 1; break; }\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    commit(c);\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    }\012"));
	} else if ((unsigned long)(((long)(my_count))==((long)(my_ONE_OR_MORE)))) {
	(my_translate_pattern)((my_c),((my_n)->my_child));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if ok {\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    loop {\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    choice(c);\012"));
	(my_translate_pattern)((my_c),((my_n)->my_child));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    if !ok { ok = 1; break; }\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    commit(c);\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    }\012"));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    }\012"));
	} else {
	(my_die)(((unsigned char *)"invalid countop"));
	}
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_primary)))) {
	(my_translate_pattern)((my_c),((my_n)->my_child));
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_any)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = any(c);\012"));
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_literal)))) {
	(my_translate_literal)((my_c),(my_n));
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_class)))) {
	(my_translate_charset)((my_c),(my_n));
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_call)))) {
	(my_fputs)(((my_c)->my_out),((unsigned char *)"    ok = peg_"));
	(my_fputs)(((my_c)->my_out),((my_c)->my_prefix));
	(my_fputb)(((my_c)->my_out),(((my_n)->my_child)->my_str),(((my_n)->my_child)->my_len));
	(my_fputs)(((my_c)->my_out),((unsigned char *)"(c);\012"));
	} else if ((unsigned long)(((long)((my_n)->my_tag))==((long)(my_PEG_sp)))) {
	(my_n)=((my_n)->my_next);
	continue;
	} else {
	(my_fdputs)((2UL),((my_PEG_tag_to_str)(((my_n)->my_tag))));
	(my_die)(((unsigned char *)"invalid tag"));
	}
	break;
	}
}
unsigned long( my_type_isint)(struct my_type* my_t){
	return (unsigned long)(((unsigned long)(((long)((my_t)->my_kind))==((long)(my_TY_INT))))||((unsigned long)(((long)((my_t)->my_kind))==((long)(my_TY_BYTE)))));
}
unsigned long( my_type_isprim)(struct my_type* my_t){
	return (unsigned long)(((unsigned long)(((long)((my_t)->my_kind))!=((long)(my_TY_VOID))))&&((unsigned long)(((unsigned long)(((long)((my_t)->my_kind))!=((long)(my_TY_STRUCT))))&&((unsigned long)(((long)((my_t)->my_kind))!=((long)(my_TY_UNION)))))));
}
unsigned long( my_type_sizeof)(struct my_compiler* my_c,struct my_type* my_t){
	unsigned long my_kind = 0;
	(my_kind)=((my_t)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_TY_INT)))) {
	return 8UL;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_BYTE)))) {
	return 8UL;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_PTR)))) {
	return 8UL;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_FUNC)))) {
	return 8UL;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_STRUCT)))) {
	(my_layout_struct)((my_c),((my_t)->my_st));
	return ((my_t)->my_st)->my_struct_size;
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_UNION)))) {
	(my_layout_union)((my_c),((my_t)->my_st));
	return ((my_t)->my_st)->my_struct_size;
	} else {
	(my_cdie)((my_c),((unsigned char *)"sizeof: invalid type"));
	}
}
void( my_typecheck_expr)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n,unsigned long my_rhs){
	struct my_decl* my_v = 0;
	unsigned long my_kind = 0;
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_STR)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"str is not an lexpr"));
	}
	((my_n)->my_t)=((my_mktype1)((my_c),(my_TY_PTR),((my_mktype0)((my_c),(my_TY_BYTE)))));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NIL)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"nil is not an lexpr"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_NIL)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NUM)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"num is not an lexpr"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CHAR)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"char is not an lexpr"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_EXPRLIST)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"call is not an lexpr"));
	}
	if ((my_n)->my_b) {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((my_n)->my_b) {
	((my_n)->my_t)=((my_mktype2)((my_c),(my_TY_ARG),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t)));
	} else {
	((my_n)->my_t)=((my_mktype1)((my_c),(my_TY_ARG),(((my_n)->my_a)->my_t)));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CALL)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"call is not an lexpr"));
	}
	if ((my_n)->my_b) {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	}
	if ((unsigned long)(((long)(((my_n)->my_a)->my_kind))==((long)(my_N_IDENT)))) {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_enum_defined))) {
	(my_cdie)((my_c),((unsigned char *)"type error"));
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_var_defined))) {
	(((my_n)->my_a)->my_t)=((my_v)->my_var_type);
	} else if ((unsigned long)(!((my_strcmp)((((my_n)->my_a)->my_s),((unsigned char *)"_include"))))) {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_func_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no such function"));
	}
	(((my_n)->my_a)->my_t)=((my_v)->my_func_type);
	} else {
	(my_v)=((my_find)((my_c),(((my_n)->my_a)->my_s),((void *)0),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_func_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no such function"));
	}
	(((my_n)->my_a)->my_t)=((my_v)->my_func_type);
	}
	} else {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	}
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))!=((long)(my_TY_FUNC)))) {
	(my_cdie)((my_c),((unsigned char *)"calling not a function"));
	}
	if ((my_n)->my_b) {
	(my_unify)((my_c),((((my_n)->my_a)->my_t)->my_arg),(((my_n)->my_b)->my_t));
	} else {
	(my_unify)((my_c),((((my_n)->my_a)->my_t)->my_arg),((void *)0));
	}
	((my_n)->my_t)=((((my_n)->my_a)->my_t)->my_val);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DOT)))) {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))==((long)(my_TY_PTR)))) {
	if ((unsigned long)(((unsigned long)(((long)(((((my_n)->my_a)->my_t)->my_val)->my_kind))!=((long)(my_TY_STRUCT))))&&((unsigned long)(((long)(((((my_n)->my_a)->my_t)->my_val)->my_kind))!=((long)(my_TY_UNION)))))) {
	(my_cdie)((my_c),((unsigned char *)"dot not a struct"));
	}
	(my_v)=((my_find)((my_c),((((((my_n)->my_a)->my_t)->my_val)->my_st)->my_name),(((my_n)->my_b)->my_s),(0UL)));
	} else {
	if ((unsigned long)(((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))!=((long)(my_TY_STRUCT))))&&((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))!=((long)(my_TY_UNION)))))) {
	(my_cdie)((my_c),((unsigned char *)"dot not a struct"));
	}
	(my_v)=((my_find)((my_c),(((((my_n)->my_a)->my_t)->my_st)->my_name),(((my_n)->my_b)->my_s),(0UL)));
	}
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_member_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"no such member"));
	}
	((my_n)->my_t)=((my_v)->my_member_type);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_IDENT)))) {
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_enum_defined))) {
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	return;
	}
	(my_v)=((my_find)((my_c),((my_d)->my_name),((my_n)->my_s),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_var_defined))) {
	((my_n)->my_t)=((my_v)->my_var_type);
	return;
	}
	(my_v)=((my_find)((my_c),((my_n)->my_s),((void *)0),(0UL)));
	if ((unsigned long)((my_v)&&((my_v)->my_func_defined))) {
	((my_n)->my_t)=((my_v)->my_func_type);
	return;
	}
	(my_cdie)((my_c),((unsigned char *)"no such variable"));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ASSIGN)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"assign is not an lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_SIZEOF)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"sizeof is not an lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_REF)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"ref is not an lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(0UL));
	((my_n)->my_t)=((my_mktype1)((my_c),(my_TY_PTR),(((my_n)->my_a)->my_t)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DEREF)))) {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))!=((long)(my_TY_PTR)))) {
	(my_cdie)((my_c),((unsigned char *)"deref not a pointer"));
	}
	((my_n)->my_t)=((((my_n)->my_a)->my_t)->my_val);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_INDEX)))) {
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	if ((unsigned long)(((long)((((my_n)->my_a)->my_t)->my_kind))!=((long)(my_TY_PTR)))) {
	(my_cdie)((my_c),((unsigned char *)"not a pointer"));
	}
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_b)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"index: not an int"));
	}
	((my_n)->my_t)=((((my_n)->my_a)->my_t)->my_val);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LT)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"lt: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GT)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"gt: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LE)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"le: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GE)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"ge: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_EQ)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"eq: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NE)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"ne: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BNOT)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not an prim"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BOR)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not an prim"));
	}
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_b)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not an prim"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BAND)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not an prim"));
	}
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_b)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not an prim"));
	}
	((my_n)->my_t)=((my_mktype0)((my_c),(my_TY_INT)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_POS)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"pos: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NEG)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"neg: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_NOT)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_ADD)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"add: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_SUB)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"sub: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_MUL)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"mul: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_DIV)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"div: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_MOD)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"mod: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LSH)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"lsh: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RSH)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"rsh: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_AND)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"and: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_OR)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"or: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_XOR)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_b),(1UL));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_n)->my_b)->my_t));
	if ((unsigned long)(!((my_type_isint)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"xor: not an int"));
	}
	((my_n)->my_t)=(((my_n)->my_a)->my_t);
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CAST)))) {
	if ((unsigned long)(!(my_rhs))) {
	(my_cdie)((my_c),((unsigned char *)"not lexpr"));
	}
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	if ((unsigned long)(!((my_type_isprim)((((my_n)->my_a)->my_t))))) {
	(my_cdie)((my_c),((unsigned char *)"not a primitive"));
	}
	((my_n)->my_t)=((my_prototype)((my_c),((my_n)->my_b)));
	} else {
	(my_cdie)((my_c),((unsigned char *)"not an expression"));
	}
}
void( my_typecheck_func)(struct my_compiler* my_c,struct my_decl* my_d){
	if ((unsigned long)(!((my_d)->my_func_def))) {
	return;
	}
	(my_typecheck_stmt)((my_c),(my_d),(((my_d)->my_func_def)->my_b));
}
void( my_typecheck_stmt)(struct my_compiler* my_c,struct my_decl* my_d,struct my_node* my_n){
	struct my_decl* my_v = 0;
	unsigned long my_kind = 0;
	if ((unsigned long)(!(my_n))) {
	return;
	}
	((my_c)->my_filename)=((my_n)->my_filename);
	((my_c)->my_lineno)=((my_n)->my_lineno);
	((my_c)->my_colno)=((my_n)->my_colno);
	(my_kind)=((my_n)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONDLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	if (((my_n)->my_a)->my_a) {
	(my_typecheck_expr)((my_c),(my_d),(((my_n)->my_a)->my_a),(1UL));
	}
	(my_typecheck_stmt)((my_c),(my_d),(((my_n)->my_a)->my_b));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_STMTLIST)))) {
	while (1) {
	if ((unsigned long)(!(my_n))) {
	break;
	}
	(my_typecheck_stmt)((my_c),(my_d),((my_n)->my_a));
	(my_n)=((my_n)->my_b);
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LOOP)))) {
	(my_typecheck_stmt)((my_c),(my_d),((my_n)->my_a));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_BREAK)))) {
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_CONTINUE)))) {
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_RETURN)))) {
	if ((my_n)->my_a) {
	if ((unsigned long)(((long)((((my_d)->my_func_type)->my_val)->my_kind))==((long)(my_TY_VOID)))) {
	(my_cdie)((my_c),((unsigned char *)"returning a value in a void function"));
	}
	(my_call_check)((my_c),((my_n)->my_a));
	(my_typecheck_expr)((my_c),(my_d),((my_n)->my_a),(1UL));
	(my_unify)((my_c),(((my_n)->my_a)->my_t),(((my_d)->my_func_type)->my_val));
	} else {
	if ((unsigned long)(((long)((((my_d)->my_func_type)->my_val)->my_kind))!=((long)(my_TY_VOID)))) {
	(my_cdie)((my_c),((unsigned char *)"returning void in a non void function"));
	}
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_LABEL)))) {
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_N_GOTO)))) {
	(my_v)=((my_find)((my_c),((my_d)->my_name),(((my_n)->my_a)->my_s),(0UL)));
	if ((unsigned long)(((unsigned long)(!(my_v)))||((unsigned long)(!((my_v)->my_goto_defined))))) {
	(my_cdie)((my_c),((unsigned char *)"label not defined"));
	}
	} else if ((unsigned long)(((long)(my_kind))!=((long)(my_N_VARDECL)))) {
	(my_call_check)((my_c),(my_n));
	(my_typecheck_expr)((my_c),(my_d),(my_n),(1UL));
	}
}
unsigned long( my_unescape)(unsigned char* my_s,unsigned long* my_i,unsigned long my_len,unsigned long* my_ok){
	unsigned long my_ch = 0;
	unsigned long my_hex = 0;
	(*(my_ok))=(1UL);
	if ((unsigned long)(((long)(*(my_i)))>=((long)(my_len)))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_ch)=((unsigned long)(my_s)[*(my_i)]);
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))!=((long)(92)))) {
	return my_ch;
	}
	if ((unsigned long)(((long)(*(my_i)))>=((long)(my_len)))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_ch)=((unsigned long)(my_s)[*(my_i)]);
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL))));
	if ((unsigned long)(((long)(my_ch))==((long)(116)))) {
	return 9;
	} else if ((unsigned long)(((long)(my_ch))==((long)(114)))) {
	return 13;
	} else if ((unsigned long)(((long)(my_ch))==((long)(110)))) {
	return 10;
	} else if ((unsigned long)(((long)(my_ch))==((long)(92)))) {
	return 92;
	} else if ((unsigned long)(((long)(my_ch))==((long)(39)))) {
	return 39;
	} else if ((unsigned long)(((long)(my_ch))==((long)(34)))) {
	return 34;
	} else if ((unsigned long)(((long)(my_ch))==((long)(120)))) {
	if ((unsigned long)(((long)(*(my_i)))>=((long)(my_len)))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_ch)=((unsigned long)(my_s)[*(my_i)]);
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL))));
	(my_hex)=((unsigned long)(((long)((my_hexdig)((my_ch),(my_ok))))*((long)(16UL))));
	if ((unsigned long)(!(*(my_ok)))) {
	return 0UL;
	}
	if ((unsigned long)(((long)(*(my_i)))>=((long)(my_len)))) {
	(*(my_ok))=(0UL);
	return 0UL;
	}
	(my_ch)=((unsigned long)(my_s)[*(my_i)]);
	(*(my_i))=((unsigned long)(((unsigned long)(*(my_i)))+((unsigned long)(1UL))));
	(my_hex)=((unsigned long)(((unsigned long)(my_hex))|((unsigned long)((my_hexdig)((my_ch),(my_ok))))));
	if ((unsigned long)(!(*(my_ok)))) {
	return 0UL;
	}
	return my_hex;
	} else {
	(*(my_ok))=(0UL);
	return 0UL;
	}
}
void( my_unify)(struct my_compiler* my_c,struct my_type* my_a,struct my_type* my_b){
	unsigned long my_kind = 0;
	if ((unsigned long)(((long)(my_a))==((long)(my_b)))) {
	return;
	}
	if ((unsigned long)((my_a)&&((unsigned long)((my_b)&&((unsigned long)(((unsigned long)(((unsigned long)(((long)((my_a)->my_kind))==((long)(my_TY_NIL))))&&((unsigned long)(((long)((my_b)->my_kind))==((long)(my_TY_PTR))))))||((unsigned long)(((unsigned long)(((long)((my_a)->my_kind))==((long)(my_TY_PTR))))&&((unsigned long)(((long)((my_b)->my_kind))==((long)(my_TY_NIL)))))))))))) {
	return;
	}
	if ((unsigned long)(((unsigned long)((my_a)&&((unsigned long)(!(my_b)))))||((unsigned long)(((unsigned long)((my_b)&&((unsigned long)(!(my_a)))))||((unsigned long)(((long)((my_a)->my_kind))!=((long)((my_b)->my_kind)))))))) {
	(my_cdie)((my_c),((unsigned char *)"type error"));
	}
	(my_kind)=((my_a)->my_kind);
	if ((unsigned long)(((long)(my_kind))==((long)(my_TY_PTR)))) {
	(my_unify)((my_c),((my_a)->my_val),((my_b)->my_val));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_FUNC)))) {
	(my_unify)((my_c),((my_a)->my_val),((my_b)->my_val));
	(my_unify)((my_c),((my_a)->my_arg),((my_b)->my_arg));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_ARG)))) {
	(my_unify)((my_c),((my_a)->my_val),((my_b)->my_val));
	(my_unify)((my_c),((my_a)->my_arg),((my_b)->my_arg));
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_STRUCT)))) {
	if ((unsigned long)(((long)((my_a)->my_st))!=((long)((my_b)->my_st)))) {
	(my_cdie)((my_c),((unsigned char *)"type error"));
	}
	} else if ((unsigned long)(((long)(my_kind))==((long)(my_TY_UNION)))) {
	if ((unsigned long)(((long)((my_a)->my_st))!=((long)((my_b)->my_st)))) {
	(my_cdie)((my_c),((unsigned char *)"type error"));
	}
	} else if ((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_VOID))))&&((unsigned long)(((unsigned long)(((long)(my_kind))!=((long)(my_TY_INT))))&&((unsigned long)(((long)(my_kind))!=((long)(my_TY_BYTE)))))))) {
	(my_cdie)((my_c),((unsigned char *)"unify: invalid type"));
	}
}
unsigned long( my_unlink)(unsigned char* my_name){
	return (my_syscall)((87UL),((unsigned long)my_name),(0UL),(0UL),(0UL),(0UL),(0UL));
}
unsigned long( my_write)(unsigned long my_fd,unsigned char* my_buf,unsigned long my_n){
	return (my_syscall)((1UL),(my_fd),((unsigned long)my_buf),(my_n),(0UL),(0UL),(0UL));
}
void( my_writeout)(struct my_assembler* my_c,struct my_label* my_start,struct my_label* my_kstart){
	struct my_chunk* my_b = 0;
	unsigned long my_i = 0;
	unsigned long my_text_size = 0;
	unsigned long my_text_end = 0;
	unsigned long my_load_addr = 0;
	unsigned long my_entry = 0;
	unsigned long my_kentry = 0;
	unsigned long my_mb_magic = 0;
	unsigned long my_mb_flags = 0;
	unsigned long my_mb_checksum = 0;
	unsigned long my_mb_addr = 0;
	struct my_section* my_s = 0;
	unsigned long my_shoff = 0;
	if ((unsigned long)(!((my_c)->my_out))) {
	(my_die)(((unsigned char *)"output not opened"));
	}
	(my_load_addr)=(1048576UL);
	(my_text_size)=((my_c)->my_at);
	if ((unsigned long)(((unsigned long)(!(my_start)))||((unsigned long)(!((my_start)->my_fixed))))) {
	if ((unsigned long)(!(my_kstart))) {
	(my_die)(((unsigned char *)"_start is not defined"));
	}
	} else {
	(my_entry)=((unsigned long)(((unsigned long)(my_load_addr))+((unsigned long)((my_start)->my_at))));
	}
	(my_text_size)=(my_text_size);
	(my_text_end)=((unsigned long)(((unsigned long)(my_load_addr))+((unsigned long)(my_text_size))));
	(my_mb_magic)=(464367618UL);
	(my_mb_flags)=(65539UL);
	(my_mb_checksum)=((unsigned long)(-(unsigned long)((unsigned long)(((unsigned long)(my_mb_magic))+((unsigned long)(my_mb_flags))))));
	(my_mb_addr)=((unsigned long)(((unsigned long)(my_load_addr))+((unsigned long)(120UL))));
	if ((unsigned long)((my_kstart)&&((my_kstart)->my_fixed))) {
	(my_kentry)=((unsigned long)(((unsigned long)(my_load_addr))+((unsigned long)((my_kstart)->my_at))));
	} else {
	(my_mb_magic)=(0UL);
	(my_kentry)=(0UL);
	}
	(my_shoff)=((my_emit_sections)(((struct my_assembler*)my_c)));
	(my_putchar)((my_c),(127UL));
	(my_putchar)((my_c),(69));
	(my_putchar)((my_c),(76));
	(my_putchar)((my_c),(70));
	(my_putchar)((my_c),(2UL));
	(my_putchar)((my_c),(1UL));
	(my_putchar)((my_c),(1UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(2UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(62UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(1UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_entry));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_entry))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_entry))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_entry))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(64UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_shoff));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(32UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(40UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(48UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_shoff))>>((unsigned long)(56UL)))));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(64UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(56UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(1UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(64UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),((my_c)->my_num_sections));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)((my_c)->my_num_sections))>>((unsigned long)(8UL)))));
	(my_s)=((my_find_section)((my_c),((unsigned char *)".shstrtab")));
	(my_putchar)((my_c),((my_s)->my_index));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)((my_s)->my_index))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),(1UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(5UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(16UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_text_size));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_text_size));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_size))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_mb_magic));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_magic))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_magic))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_magic))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_mb_flags));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_flags))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_flags))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_flags))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_mb_checksum));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_checksum))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_checksum))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_checksum))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_mb_addr));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_addr))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_addr))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_mb_addr))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_load_addr));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_load_addr))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_load_addr))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_load_addr))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_text_end));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_end))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_end))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_text_end))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(0UL));
	(my_putchar)((my_c),(my_kentry));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_kentry))>>((unsigned long)(8UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_kentry))>>((unsigned long)(16UL)))));
	(my_putchar)((my_c),((unsigned long)(((unsigned long)(my_kentry))>>((unsigned long)(24UL)))));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_putchar)((my_c),(my_OP_NOP));
	(my_b)=((my_c)->my_text);
	while (1) {
	if ((unsigned long)(!(my_b))) {
	break;
	}
	(my_i)=(0UL);
	while (1) {
	if ((unsigned long)(((long)(my_i))>=((long)((my_b)->my_fill)))) {
	break;
	}
	(my_putchar)((my_c),((unsigned long)((my_b)->my_buf)[my_i]));
	(my_i)=((unsigned long)(((unsigned long)(my_i))+((unsigned long)(1UL))));
	}
	(my_b)=((my_b)->my_next);
	}
	(my_fflush)(((my_c)->my_out));
}
