; Robot instructions
		dscn    $1				; D: distance scan
		fir     #0($1)			; F ?: fire
		ascn	$2				; A: angle scan
		rot     #0($2)			; R ?: rotate
		spd     #0($0)			; M 0: move

; ALU
		addi    $1, $0, #1		; N: I-type ALU, $1 = 1
		fir     #0($1)			; F 1
		add     $2, $1, $1		; N: R-type ALU, $2 = 2
		fir     #0($2)			; F 2

; I-type memory
		addi    $4, $2, #1		; N: $4 = 3
		sw      $4, :data($0)	; N: store $4(=3)
		lw      $3, :data($0)	; N: load $3(=3)
		fir		#0($3)			; F 3: lw and sw work

; jumps and branches
        jal     :b1         ; N: skip to F 4
        fir     #5($0)      ; F 5: jr works
        beq     $0 $0 :b2   ; N: should branch to F 6
:b1		fir		#4($0)		; F 4: jal works
        jr      $31			; N: return to instruction *after* jal = F 5
:b2     fir     #6($0)		; F 6: beq works

; done, print recognizable end, then loop forever
        fir     #999($0)		; F 999
:end    j       :end            ; N forever
:data
