
.686
.MODEL FLAT, C

.CODE

MAS_SWAP PROC C array:DWORD
    mov eax, array

    mov ecx, [eax+4]
    mov edx, [eax+8]

    mov [eax+4], edx
    mov [eax+8], ecx

    ret
MAS_SWAP ENDP

END
