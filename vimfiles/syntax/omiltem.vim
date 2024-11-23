if exists("b:current_syntax")
	finish
endif

" return break sizeof if else loop continue goto var enum struct union byte
" int void func

syn keyword oReturn return
syn keyword oBreak break
syn keyword oSizeof sizeof
syn keyword oIf if
syn keyword oElse else
syn keyword oLoop loop
syn keyword oContinue continue
syn keyword oGoto goto
syn keyword oVar var
syn keyword oEnum enum
syn keyword oStruct struct
syn keyword oUnion union
syn keyword oByte byte
syn keyword oInt int
syn keyword oVoid void
syn keyword oFunc func
syn keyword oAs as
syn keyword oType type
syn keyword oNil nil

syn keyword oTodo contained TODO FIXME XXX BUG
syn cluster oCommentGroup contains=oTodo
syn region oComment start="//" end="$" contains=@oCommentGroup,@Spell

syn region oString start=+"+ skip=+\\\\\|\\"+ end=+"+ contains=@Spell

syn region oChar start=+'+ skip=+\\\\\|\\'+ end=+'+

syn match oDecInt "\<-\=\%(0\|\d\+\)\>"
syn match oHexInt "\<-\=0[xX]\%(\x\+\)\>"

syn region oBlock start="{" end="}" transparent

hi def link oDecInt Integer
hi def link oHexInt Integer
hi def link Integer Number

hi def link oChar Character

hi def link oString String

hi def link oComment Comment
hi def link oTodo Todo

hi def link oReturn Keyword
hi def link oBreak Keyword
hi def link oSizeof Keyword

hi def link oIf Conditional
hi def link oElse Conditional

hi def link oLoop Repeat
hi def link oContinue Statement
hi def link oGoto Statement

hi def link oVar Keyword
hi def link oAs Keyword
hi def link oNil Keyword

hi def link oEnum Structure
hi def link oStruct Structure
hi def link oUnion Structure

hi def link oByte Type
hi def link oInt Type
hi def link oVoid Type
hi def link oFunc Type
hi def link oType Type

let b:current_syntax = "omiltem"
