; This file contains errors in relative addressing mode.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not necessarily exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok
int:     .data 0

; 2. this line is ok 
start:   jsr  %end

; 3. this line is ok
         bne  %start

; 4. this line is ok 
         jmp  %start         

; 5. invalid source operand (relative mode) 
         lea  %int,r1

; 6. invalid target operand (relative mode) 
         add  #45,%int
         
; 7. invalid operand (relative mode) 
         inc  %int

; 8. invlid characters (%start) 
         bne  %%start

; 9. invalid operand (incomplete relative)
         jmp  % start         

; 10. invlid label (42) (or invalid characters (42))
         jsr  %42

; 11. invlid label (5xy) (or invalid characters (5xy))
         bne  %5xy

; 12. invalid operand (or invalid characters (%start))
end:     stop %start
