; make segment A display the number 8
; 0x03 = port 3, where segment A lives
; 0x38 = "8"
outii 03 38
hlt