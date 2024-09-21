	bits 16

_start:
	mov bp, sp

	call .1
.1:
	pop di

	mov cs:[di + pxenv_ptr - .1], bx
	mov cs:[di + pxenv_ptr - .1 + 2], es

	mov eax, ss:[bp + 4]
	mov cs:[di + pxe_ptr - .1], eax

	push booting_msg
	call puts
	pop ax

	call enable_a20
	test ax, ax
	jnz .no_a20

	call detect_mem
	test ax, ax
	jnz .no_mem

	call enter_unreal
	test ax, ax
	jnz .no_unreal

	call check_unreal
	test ax, ax
	jnz .no_unreal

	call find_pxe
	test ax, ax
	jnz .no_pxe

	push dot
	call puts
	pop ax

	call download
	test ax, ax
	jnz .no_download

	push dot
	call puts
	pop ax

	call parse_mb
	test ax, ax
	jnz .no_mb

	call boot

	push fail_errmsg
	call puts
	pop ax
	jmp hang
.no_a20:
	push a20_errmsg
	call puts
	pop ax
	jmp hang
.no_mem:
	push mem_errmsg
	call puts
	pop ax
	jmp hang
.no_unreal:
	push unreal_errmsg
	call puts
	pop ax
	jmp hang
.no_pxe:
	push pxe_errmsg
	call puts
	pop ax
	jmp hang
.no_download:
	push download_errmsg
	call puts
	pop ax
	jmp hang
.no_mb:
	push mb_errmsg
	call puts
	pop ax
	jmp hang

dot:
	db `.`, 0
booting_msg:
	db `Booting.`, 0
fail_errmsg:
	db `\r\nBoot failed!`, 0
a20_errmsg:
	db `\r\nFailed to enable A20!`, 0
mem_errmsg:
	db `\r\nFailed to detect memory!`, 0
unreal_errmsg:
	db `\r\nFailed to enter unreal mode!`, 0
pxe_errmsg:
	db `\r\nNo pxe!`, 0
download_errmsg:
	db `\r\nFailed to download!`, 0
mb_errmsg:
	db `\r\nFailed to find a.out multiboot header!`, 0

hang:
	cli
	hlt
	jmp hang

enable_a20:
	pusha
	; Check if we need to do anything
	call test_a20
	test ax, ax
	jz .a20_ok
	; BIOS - Query A20 Gate Support
	mov ax, 0x2403
	clc
	int 0x15
	jc .a20_bios
	test ah, ah
	jnz .a20_bios
	test bx, 2
	jz .a20_bios
.a20_fast:
	; Fast A20
	in al, 0x92
	or al, 2
	out 0x92, al
	; Check if that worked
	call test_a20
	test ax, ax
	jz .a20_ok
.a20_bios:
	; BIOS - Enable A20 Gate
	mov ax, 0x2401
	clc
	int 0x15
	jc .a20_slow
	test ah, ah
	jnz .a20_slow
	; Double check the bios
	call test_a20
	test ax, ax
	jz .a20_ok
.a20_slow:
	; Give up
	popa
	mov ax, 1
	ret
.a20_ok:
	popa
	mov ax, 0
	ret

	; Result ax=0 if A20 is enabled
test_a20:
	push es
	push ds
	push si
	push di
	; ds:[di] es:[si] are aliases if A20=0
	mov ax, 0
	mov ds, ax
	mov di, 0x0500
	mov ax, -1
	mov es, ax
	mov si, 0x1500
	; Write different values to ds:[di] to es:[si]
	mov ax, 0
	mov ds:[di], ax
	mov ax, -1
	mov es:[si], ax
	; Compare the values
	mov ax, ds:[di]
	xor ax, es:[si]
	not ax
	pop di
	pop si
	pop ds
	pop es
	ret

detect_mem:
	pusha
	push ds
	push es
	call .1
.1:
	pop bp
	sub bp, .1
	push cs
	pop es
	; BIOS  - Get memory size
	clc
	int 0x12
	jc .no_mem
	push ax
	xor eax, eax
	pop ax
	shl eax, 10
	mov es:[bp + low_mem_size], eax
	; BIOS - Get high memory size
	mov ah, 0x88
	clc
	int 0x15
	jc .no_mem
	push ax
	xor eax, eax
	pop ax
	shl eax, 10
	mov es:[bp + high_mem_size], eax
	; Zero kernel size
	mov dword es:[bp + kernel_size], 0
	mov di, bp
	add di, mmap + 4
	mov ebx, 0
	mov word es:[bp + mmap_size], 0
.loop:
	; BIOS - Get system memory map
	mov edx, 0x534D4150
	mov eax, 0xe820
	mov ecx, 20
	clc
	int 0x15
	jc .no_mem
	mov edx, 0x534d4150
	cmp eax, edx
	jnz .no_mem
	cmp ecx, 20
	jnz .no_mem
	mov es:[di - 4], ecx
	add word es:[bp + mmap_size], 24
	cmp word es:[bp + mmap_size], (mmap_end - mmap)
	jz .no_mem
	add di, 24
	test ebx, ebx
	jnz .loop
	pop es
	pop ds
	popa
	mov ax, 0
	ret
.no_mem:
	pop es
	pop ds
	popa
	mov ax, 1
	ret

enter_unreal:
	pop ax
	push cs
	push ax
	pusha
	push ds
	push es
	push fs
	push gs
	pushf
	cli
	call .1
.1:
	xor ebx, ebx
	pop bx
	; Load the gdt
	sub sp, 6
	mov bp, sp
	mov word ss:[bp], 39
	xor eax, eax
	mov ax, cs
	shl eax, 4
	add eax, ebx
	add eax, (gdt - .1)
	mov ss:[bp + 2], eax
	lgdt ss:[bp]
	add sp, 6

	mov bx, sp
	mov dx, ss

	xor ebp, ebp
	mov bp, sp
	xor eax, eax
	mov ax, ss
	shl eax, 4
	add ebp, eax

	mov ecx, cr0
	or ecx, 1
	mov cr0, ecx

	push 24
	call .retf

	mov ax, 32
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, ebp

	mov ecx, cr0
	and ecx, -2
	mov cr0, ecx

	mov ss, dx
	mov sp, bx

	popf
	pop gs
	pop fs
	pop es
	pop ds
	popa
	mov ax, 0
.retf:
	retf

gdt:
	; 0 = null
	dq 0x0000000000000000
	; 8 = 32 code
	dq 0x00cf9b000000ffff
	; 16 = 32 data
	dq 0x00cf93000000ffff
	; 24 = 16 code
	dq 0x008f9b000000ffff
	; 32 = 16 data
	dq 0x008f93000000ffff

check_unreal:
	push ds
	push es

	mov ax, 0
	mov ds, ax
	mov ax, -1
	mov es, ax

	mov word ds:[0x500], 0
	mov eax, 0x100500
	mov word ds:[eax], 44
	mov ax, ds:[0x500]
	cmp ax, 0
	jnz .fail

	mov ax, es:[0x510]
	cmp ax, 44
	jnz .fail

	mov ax, 0
	pop es
	pop ds
	ret
.fail:
	mov ax, 1
	pop es
	pop ds
	ret

getframe:
	call .1
.1:
	pop di
	add di, (frame - .1)
	push cs
	pop ds
	push cs
	pop es
	ret

download:
	push ds
	push es
	pusha
	pushf

	call getframe

	mov word [di], 0		; status
	mov word [di + 2], 2		; PXENV_PACKET_TYPE_DHCP_ACK
	mov word [di + 4], 0		; buffer size
	mov word [di + 6], 0		; buffer base
	mov word [di + 8], 0		; buffer segment
	mov word [di + 10], 0		; buffer limit
	push ds
	push di
	mov bx, 0x71
	push bx				; PXENV_GET_CACHED_INFO = 0x71
	call far word [di + (pxe_entry - frame)]
	add sp, 6
	test ax, ax
	jnz .fail

	call getframe

	mov si, di
	add si, (filename - frame)
	add di, 10
	mov cx, 128
	rep movsb

	call getframe

	mov word [di], 0		; status
	mov byte [di + 2], 192		; server ip
	mov byte [di + 3], 168
	mov byte [di + 4], 1
	mov byte [di + 5], 178
	mov dword [di + 6], 0		; gateway ip
	mov word [di + 138], 69		; port
	mov word [di + 140], 512	; packet size
	push ds
	push di
	mov bx, 0x20
	push bx				; PXENV_TFTP_OPEN = 0x0020
	call far [di + (pxe_entry - frame)]
	add sp, 6
	test ax, ax
	jnz .fail

	call getframe
	mov eax, 0x100000
	mov cs:[di + (download_ptr - frame)], eax

.loop:

	call getframe

	mov ax, di
	add ax, (page - frame)
	mov word [di], 0		; status
	mov word [di + 2], 0		; packet number
	mov word [di + 4], 0		; buffer size
	mov word [di + 6], ax		; buffer
	mov word [di + 8], ds
	push ds
	push di
	mov bx, 0x22
	push bx				; PXENV_TFTP_READ = 0x0022
	call far [di + (pxe_entry - frame)]
	add sp, 6
	test ax, ax
	jnz .fail

	call getframe

	; Push the kernel pointer
	xor ecx, ecx
	mov cx, cs:[di + 4]
	mov ebx, cs:[di + (kernel_size - frame)]
	mov eax, ebx
	add eax, ecx
	mov cs:[di + (kernel_size - frame)], eax
	add ebx, cs:[di + (download_ptr - frame)]

	cmp cs:[di + (high_mem_size - frame)], eax
	jb .fail

	xor esi, esi
	mov si, di
	add si, (page - frame)

	mov ax, 0
	mov es, ax
	mov edi, ebx

	cld
	rep a32 movsb

	call getframe

	; Keep reading blocks until we get a small one
	cmp word cs:[di + 4], 512
	jz .loop

	call getframe

	mov word cs:[di], 0		; status
	push ds
	push di
	mov bx, 0x21
	push bx				; PXENV_TFTP_CLOSE = 0x0021
	call far [di + (pxe_entry - frame)]
	add sp, 6
	test ax, ax
	jnz .fail

	popf
	popa
	pop es
	pop ds
	mov ax, 0
	ret
.fail:
	popf
	popa
	pop es
	pop ds
	mov ax, 1
	ret

find_pxe:
	pusha
	push ds
	push es

	mov ax, cs
	mov es, ax

	call .1
.1:
	pop bx

	mov ax, es:[bx + (pxenv_ptr - .1 + 2)]
	mov ds, ax
	mov si, es:[bx + (pxenv_ptr - .1)]

	; PXENV+
	cmp word [si], 0x5850
	jnz .fail
	cmp word [si + 2], 0x4e45
	jnz .fail
	cmp word [si + 4], 0x2b56
	jnz .fail
	cmp word [si + 6], 0x0201
	jb .fail
	cmp byte [si + 8], 44
	jb .fail
	mov ch, 0
	mov cl, [si + 8]
	call bsum
	test ax, ax
	jnz .fail

	mov ax, [si + 40]
	mov es:[bx + (pxe_ptr - .1)], ax
	mov ax, [si + 42]
	mov es:[bx + (pxe_ptr - .1 + 2)], ax
	mov ds, ax
	mov si, es:[bx + (pxe_ptr - .1)]

	; !PXE
	cmp word [si], 0x5021
	jnz .fail
	cmp word [si + 2], 0x4558
	jnz .fail
	cmp byte [si + 4], 20
	jb .fail
	mov ch, 0
	mov cl, [si + 4]
	call bsum
	test ax, ax
	jnz .fail

	; Save pxe entry point
	mov ax, [si + 16]
	mov es:[bx + pxe_entry - .1], ax
	mov ax, [si + 18]
	mov es:[bx + pxe_entry - .1 + 2], ax

	pop es
	pop ds
	popa
	mov ax, 0
	ret
.fail:
	pop es
	pop ds
	popa
	mov ax, 1
	ret

bsum:
	push si
	push cx
	xor ax, ax
	test cx, cx
	jz .out
.loop:
	add al, ds:[si]
	inc si
	dec cx
	jnz .loop
.out:
	pop cx
	pop si
	ret

bzero32:
	push edi
	push ecx
	test ecx, ecx
	jz .out
.loop:
	mov byte ds:[edi], 0
	inc edi
	dec ecx
	jz .out
	jmp .loop
.out:
	pop ecx
	pop edi
	ret

bcopy32:
	push edi
	push esi
	push ecx
	test ecx, ecx
	jz .out
	cmp edi, esi
	je .out
	jb .loop_forward
	add esi, ecx
	add edi, ecx
.loop_backward:
	dec edi
	dec esi
	mov al, ds:[esi]
	mov ds:[edi], al
	dec ecx
	jz .out
	jmp .loop_backward
.loop_forward:
	mov al, ds:[esi]
	mov ds:[edi], al
	inc esi
	inc edi
	dec ecx
	jz .out
	jmp .loop_forward
.out:
	pop ecx
	pop esi
	pop edi
	ret

parse_mb:
	pusha
	push ds
	push es
	pushf
	cld
	mov ax, 0
	mov ds, ax
	mov es, ax
	call .1
.1:
	xor ebx, ebx
	pop bx
	; Bound the search to the first 8k (2k dwords)
	mov ecx, 2048

	sub ecx, 7
	jbe .fail
	; Find the multiboot header
	mov edi, cs:[bx + (download_ptr - .1)]
.loop:
	mov eax, ds:[edi]
	cmp eax, 0x1badb002
	jnz .next
	add eax, ds:[edi + 4]
	add eax, ds:[edi + 8]
	jz .found
.next:
	add edi, 4
	dec ecx
	jz .fail
	jmp .loop
.found:
	test dword ds:[edi + 4], 0x10000
	jz .fail

	mov cs:[bx + (mb_ptr - .1)], edi

	mov eax, ds:[edi + 12]
	mov cs:[bx + (mb_header_addr - .1)], eax

	mov eax, ds:[edi + 16]
	mov cs:[bx + (mb_load_addr - .1)], eax

	mov eax, ds:[edi + 20]
	test eax, eax
	jnz .endok
	mov eax, cs:[bx + (mb_load_addr - .1)]
	add eax, cs:[bx + (kernel_size - .1)]
.endok:
	mov cs:[bx + (mb_load_end_addr - .1)], eax

	mov eax, ds:[edi + 24]
	test eax, eax
	jnz .bssok
	mov eax, cs:[bx + (mb_load_end_addr - .1)]
.bssok:
	mov cs:[bx + (mb_bss_end_addr - .1)], eax

	mov eax, ds:[edi + 28]
	mov cs:[bx + (mb_entry_addr - .1)], eax

	; sanity checks

	; 0x100000 <= load_addr
	cmp dword cs:[bx + (mb_load_addr - .1)], 0x100000
	jb .fail

	; load_addr <= header_addr
	mov eax, cs:[bx + (mb_load_addr - .1)]
	cmp eax, cs:[bx + (mb_header_addr - .1)]
	ja .fail

	; header_addr <= load_end_addr
	mov eax, cs:[bx + (mb_header_addr - .1)]
	cmp eax, cs:[bx + (mb_load_end_addr - .1)]
	ja .fail

	; load_addr <= entry_addr
	mov eax, cs:[bx + (mb_load_addr - .1)]
	cmp eax, cs:[bx + (mb_entry_addr - .1)]
	ja .fail

	; entry_addr <= load_end_addr
	mov eax, cs:[bx + (mb_entry_addr - .1)]
	cmp eax, cs:[bx + (mb_load_end_addr - .1)]
	ja .fail

	; load_end_addr <= bss_end_addr
	mov eax, cs:[bx + (mb_load_end_addr - .1)]
	cmp eax, cs:[bx + (mb_bss_end_addr - .1)]
	ja .fail

	; load_end_addr - load_addr <= kernel_size
	mov ecx, cs:[bx + (mb_load_end_addr - .1)]
	sub ecx, cs:[bx + (mb_load_addr - .1)]
	cmp ecx, cs:[bx + (kernel_size - .1)]
	ja .fail

	; bss_end_addr - load_addr <= high_mem_size
	mov ecx, cs:[bx + (mb_bss_end_addr - .1)]
	sub ecx, cs:[bx + (mb_load_addr - .1)]
	cmp ecx, cs:[bx + (high_mem_size - .1)]
	ja .fail

	; header_addr - load_addr <= mb_ptr - download_ptr
	mov eax, cs:[bx + (mb_header_addr - .1)]
	sub eax, cs:[bx + (mb_load_addr - .1)]
	mov ecx, cs:[bx + (mb_ptr - .1)]
	sub ecx, cs:[bx + (download_ptr - .1)]
	cmp eax, ecx
	ja .fail

	; copy to load_addr
	; from mb_ptr - header_addr + load_addr
	mov edi, cs:[bx + (mb_load_addr - .1)]
	mov esi, cs:[bx + (mb_ptr - .1)]
	sub esi, cs:[bx + (mb_header_addr - .1)]
	add esi, edi
	mov ecx, cs:[bx + (mb_load_end_addr - .1)]
	sub ecx, edi
	call bcopy32

	; zero bss
	mov edi, cs:[bx + (mb_load_end_addr - .1)]
	mov ecx, cs:[bx + (mb_bss_end_addr - .1)]
	sub ecx, edi
	call bzero32

	; Align to dword
	xor edi, edi
	mov di, cs
	shl edi, 4
	add edi, ebx
	add edi, (frame - .1 + 3)
	and edi, -4

	mov cs:[bx + (mb_info_ptr - .1)], edi

	; Write multiboot info
	mov dword ds:[edi], ((1 << 0) + (1 << 6))

	; Save mem_lower
	mov eax, cs:[bx + low_mem_size - .1]
	shr eax, 10
	mov dword ds:[edi + 4], eax

	; Save mem_upper
	mov eax, cs:[bx + high_mem_size - .1]
	shr eax, 10
	mov dword ds:[edi + 8], eax

	; Save mmap_len
	xor eax, eax
	mov ax, cs:[bx + mmap_size - .1]
	mov dword ds:[edi + 44], eax

	; Save mmap_addr
	xor eax, eax
	mov ax, cs
	shl eax, 4
	add eax, ebx
	add eax, (mmap + 4 - .1)
	mov dword ds:[edi + 48], eax

	popf
	pop es
	pop ds
	popa
	mov ax, 0
	ret
.fail:
	popf
	pop es
	pop ds
	popa
	mov ax, 1
	ret

boot:
	mov ax, 0
	mov ds, ax
	mov es, ax
	call .1
.1:
	xor ebx, ebx
	pop bx

	mov esi, cs:[bx + mb_info_ptr - .1]
	mov edi, cs:[bx + mb_entry_addr - .1]

	cli

	; Load the gdt
	sub sp, 6
	mov bp, sp
	mov word ss:[bp], 39
	xor eax, eax
	mov ax, cs
	shl eax, 4
	add eax, ebx
	add eax, (gdt - .1)
	mov ss:[bp + 2], eax
	lgdt ss:[bp]
	add sp, 6

	mov ecx, cr0
	or ecx, 1
	mov cr0, ecx


	xor ebp, ebp
	mov bp, sp
	xor eax, eax
	mov ax, ss
	shl eax, 4
	add ebp, eax

	push 8
	call .retf

	bits 32

.3:

	mov ax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, ebp

	mov eax, 0x2badb002
	mov ebx, esi
	jmp edi

	bits 16

.retf:
	retf

puts:
	push bp
	mov bp, sp
	pusha
	push ds
	push es
	push fs
	push gs
	pushf
	call .1
.1:
	pop ax
	sub ax, .1
	mov si, ss:[bp + 4]
	add si, ax
	cld
.loop:
	mov ah, 0x0e
	mov al, cs:[si]
	test al, al
	jz .out
	mov bx, 0x0001
	int 0x10
	inc si
	jmp .loop
.out:
	popf
	pop gs
	pop fs
	pop es
	pop ds
	popa
	pop bp
	ret

puth:
	push bp
	mov bp, sp
	pusha
	push ds
	push es
	push fs
	push gs
	pushf
	mov cx, 32
.loop:
	sub cx, 4
	mov eax, [bp + 4]
	shr eax, cl
	and eax, 15
	cmp eax, 10
	jb .a
	add eax, 'a' - '0' - 10
.a:
	add eax, 0x0e00 + '0'
	mov bx, 0x0001
	int 0x10
	test cx, cx
	jnz .loop
.out:
	popf
	pop gs
	pop fs
	pop es
	pop ds
	popa
	pop bp
	ret

low_mem_size:
	dd 0
high_mem_size:
	dd 0
kernel_size:
	dd 0
mmap_size:
	dw 0
pxenv_ptr:
	dd 0
pxe_ptr:
	dd 0
pxe_entry:
	dd 0
download_ptr:
	dd 0
mb_ptr:
	dd 0
mb_header_addr:
	dd 0
mb_load_addr:
	dd 0
mb_load_end_addr:
	dd 0
mb_bss_end_addr:
	dd 0
mb_entry_addr:
	dd 0
mb_info_ptr:
	dd 0
	align 16, db 0
filename:
	db `/kernel`, 0
	times 128 - ($ - filename) db 0
frame equ $
	;times 256 db 0
page equ frame + 256
	;times 512 db 0
mmap equ page + 512
	;times (8192 + 8192) db 0
mmap_end equ mmap + (8192 + 8192)
