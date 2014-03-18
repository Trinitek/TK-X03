; SEGCOUNT.ASM
; Blake Burgess
; 17 March 2014
;
; Continuously increments A and sends the result to the second segment display

	outii	03 00	; display 0 on first display (port 3)
	
;:main:
	inc
	outai	04		; send A data to the second segment display (port 4)
	
	jmpi	00 03	; infinite loop back to 