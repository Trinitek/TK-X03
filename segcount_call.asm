; SEGCOUNT_CALL.ASM
; Blake Burgess
; 19 March 2014
;
; Continuously increments A and sends the result to the second segment display,
; and uses the CALL instruction to do so.

	outii	03 00	; display 0 on first display (port 3)
	
;:main:
	inc

    calli   00 0a   ; call writeSegment
	
	jmpi	00 03	; infinite loop back to

;:writeSegment:
    outai	04		; send A data to the second segment display (port 4)

    ret