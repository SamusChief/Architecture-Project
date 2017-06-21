; C-type, immediates
        ascn    $1          ; A, wait for result: check ascn I/O
        fir     #1($1)      ; F ?: A result + 1
        dscn    $2          ; D, wait for result: check dscn I/O
        fir     #2($2)      ; F ?: D result + 2
        rot     #-1($0)		; R -1: check negative immediate
        spd     #0($0)      ; M 0: check spd output

; I-type ALU
		addi	$0, $0, #1	; N: $0 shouldn't change
        addi    $1, $0, #1  ; N: addi, $1 = 1
        rot     #0($1)      ; R 1
        addiu   $2, $1, #1  ; N: addui, $2 = 2, use of result register
        rot     #0($2)      ; R 2
        andi    $3, $2, #6	; N: andi, 0110b and 0010b, $3 = 2
        ori     $3, $3, #1  ; N: ori,  0010b or 0001b, $3 = 3
        rot     #0($3)      ; R 3
        xori    $4, $3, #7  ; N: xori, 0011b ^ 0111b, $4 = 4
        rot     #0($4)      ; R 4

; I-type memory
        addi    $6, $0, #5   ; N: $6 = 5
        addi    $7, $0, #500 ; N: $7 = 500
        addi    $8, $0, #501 ; N: $8 = 501
        sw      $6, #12($7)  ; N: mem[512] = 5
        lw      $5, #11($8)  ; N: $5 = mem[512] = 5
        rot     #0($5)       ; R 5

; I-type branch
        beq     $0 $0 :b1   ; N: should branch
        rot     #0($0)      ; (R 0): should not reach this line
:b1     rot     #6($0)		; R 6: beq true works
		beq     $0 $2 :b2   ; N: don't branch
        rot     #7($0)      ; R 7: beq false works
:b2     bne     $2 $0 :b3   ; N: should branch
        rot     #0($0)      ; (R 0): should not reach this line
:b3     rot     #8($0)		; R 8: bne true works
		bne     $0 $0 :b4   ; N: don't branch
        rot     #9($0)		; R 9: bne false works
:b4		addi    $6 $0 #-1	; N: $6 = -1
		blez    $6 :b5		; N: should branch
        rot     #0($0)      ; (R 0): should not reach this line
:b5     rot     #10($0)		; R 10: blez true works
		blez    $1 :b6		; N: don't branch
        rot     #11($0)     ; R 11: blez false works
:b6     bgtz    $1 :b7		; N: should branch
        rot     #0($0)      ; (R 0): should not reach this line
:b7     rot		#12($0)		; R 12: bgtz true works
		bgtz    $0 :b8		; N: don't branch
        rot     #13($0)     ; R 13: bgtz false works
:b8

; J-type and R-type jumps
        j       :b9         ; N: skip next instruction
        rot     #0($0)      ; (R 0): should not reach this line
:b9		rot		#14($0)		; R 14: j works
		jal     :b10        ; N: jump to b10
        rot     #16($0)     ; R 16: jal return works, jalr works
        jr      $31 $31 $31	; N: return to instruction *after* jalr

; R-type jumps
:b10    rot     #15($0)     ; R 15: reach after b9, jal works
        jalr    $31 $31 $31	; N: return to instruction after jal
        rot     #17($0)     ; R 17: jalr return works, jr works

; R-type alu
        addi    $6, $0, #14     ; N: $6 = 14
        add     $7, $6, $4		; N: $4 = 4, so $17 = 18
        rot     #0($7)			; R 18
        addu    $8, $7, $1		; N: $8 = 18+1 = 19
        rot     #0($8)			; R 19
        addi    $9, $0, #25		; N: $9 = 25
        sub     $10, $9, $5		; N: $10 = 25 - 5 = 20
        rot     #0($10)         ; R 20
        addi    $11, $0, #23    ; N: $11 = 10111b
        addi    $12, $0, #29    ; N: $12 = 11101b
        and     $13, $11, $12   ; N: $13 = 10101b
        rot     #0($13)         ; R 21
        xor     $14, $13, $3	; N: $14 = 10110b = 22
        rot     #0($14)         ; R 22
        or      $15, $14, $1	; N: $15 = 10111b = 23
        rot     #0($15)         ; R 23

; done, print recognizable end, then loop forever
        fir     #999($0)		; F 999
:end    j       :end            ; N forever
