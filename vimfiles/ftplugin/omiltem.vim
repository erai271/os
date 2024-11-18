if exists("b:did_ftplugin")
	finish
endif
let b:did_ftplugin = 1

if !exists("g:omiltem_recommended_style") || g:omiltem_recommended_style != 0
	set noexpandtab tabstop=8 softtabstop=8 shiftwidth=8
endif
