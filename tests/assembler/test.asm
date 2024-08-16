global _start
_start:
    mov rax, 60
    mov rdi, 0
    syscall
.end:
    ret
    
; TODO: These fuckers
; mov ebx, 0
; mov ebx, [ebx]
; mov DWORD [ebx], 0
