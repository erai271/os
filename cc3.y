start program;

primary: ident
	| num
	| hex
	| str
	| chr
	| lpar expr rpar
;

expr_list: expr
	| expr comma expr_list
;

post_expr: primary
	| sizeof lpar expr rpar
	| post_expr lsq expr rsq
	| post_expr lpar expr_list rpar
	| post_expr dot ident
	| post_expr colon type
;

unary_expr : post_expr
	| and unary_expr
	| star unary_expr
	| plus unary_expr
	| minus unary_expr
	| tilde unary_expr
	| bang unary_expr
;

shift_expr : unary_expr
	| unary_expr lsh shift_expr
	| unary_expr rsh shift_expr
;

mul_expr : shift_expr
	| shift_expr star mul_expr
	| shift_expr div mul_expr
	| shift_expr mod mul_expr
	| shift_expr and mul_expr
;

add_expr : mul_expr
	| mul_expr plus add_expr
	| mul_expr minus add_expr
	| mul_expr or add_expr
	| mul_expr xor add_expr
;

comp_expr : add_expr
	| add_expr lt add_expr
	| add_expr gt add_expr
	| add_expr le add_expr
	| add_expr gt add_expr
	| add_expr eq add_expr
	| add_expr ne add_expr
;

bool_expr : bool_expr
	| add_expr and_then bool_expr
	| add_expr or_else bool_expr
;

expr : bool_expr
	| bool_expr equal expr
;

if_stmt : if expr lbra stmt_list rbra
	| if expr lbra stmt_list rbra else if_stmt
	| if expr lbra stmt_list rbra else lbra stmt_list rbra
;

loop_stmt : loop lbra stmt_list rbra;

break_stmt : break;

continue_stmt : continue;

return_stmt : return;
	| return expr
;

var_decl : ident colon type;

var_stmt : var var_decl;

label_stmt : colon ident

goto_stmt : goto ident;

stmt : if_stmt
	| loop_stmt
	| break_stmt semi
	| continue_stmt semi
	| return_stmt semi
	| var_stmt semi
	| label_stmt semi
	| goto_stmt semi
	| expr semi
;

stmt_list : stmt
	| stmt stmt_list
;

enum_list : ident
	| enum_list comma enum_list
;

enum_decl : enum ident lbra enum_list rbra;

type : ident
	| star type
	| lpar type rpar
	| func func_type
;

member_decl : ident colon type;

member_list : member_decl
	| member_decl semi member_list
;

struct_decl : struct ident lbra member_list rbra;

arg_decl : colon type
	ident colon type
;

arg_list : arg_decl
	| arg_decl comma arg_list
;

func_type : lpar rpar colon type
	| lpar arg_list rpar colon type
;

func_decl : ident func_type;

func_def : func_decl lbra stmt_list rbra
	| func_decl semi
;

decl : enum_decl
	| struct_decl
	| func_def
;

program : decl
	| decl program
;
