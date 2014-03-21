; SEGCOUNT_CARRY.ASM
; Blake Burgess
; 19 March 2014
;
; Continuously increments A and sends the result to the first and second segment display,
; and uses the ROL and ROR instructions to do so.
	
;:main:
    ror             ; rotate A right four times to select high nibble
    ror             ; ...to write nibble data to first segment
    ror
    ror

    outai   03      ; write data to first segment display

    rol             ; rotate A back
    rol
    rol
    rol

    outai   04      ; write data to second segment display

    inc             ; A++

	jmpi	00 00	; loop back to main