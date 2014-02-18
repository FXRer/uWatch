;==============================================================================|
;    Copyright (c) 2011, Greg Davill
;    All rights reserved.
;
;	  Redistribution and use in source and binary forms, with or without 
;	  modification, are permitted provided that the following conditions 
;	  are met:
;	
;	    Redistributions of source code must retain the above copyright 
;	    notice, this list of conditions and the following disclaimer.
;	 
;	    Redistributions in binary form must reproduce the above copyright
;	    notice, this list of conditions and the following disclaimer in the 
;	    documentation and/or other materials provided with the   
;	    distribution.
;	 
;	  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
;	  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
;	  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;	  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
;	  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
;	  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
;	  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;	  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;	  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
;	  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
;	  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;==============================================================================|


.section .bss
.global out_func
out_func:   .word 0
xprintString: .word 0
xsprintReturn: .word 0


;  xprint.s (printf fuctions)
;=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "xprint.h"

;      eXtended string print
;  SP[0] Return Address
;  SP[2] Storage String
;  SP[4] Format String
;  SP[6+] Extra Params
;--------------------------------------

.section .text
.global xsprint
.func xsprint
xsprint:
    pop.w   &xsprintReturn
    pop.w   &xprintString
    mov.w   #putcString,&out_func
    call    #xprint
    push.w    #0
    push.w  &xsprintReturn

    ret
.endfunc



;       putcString
;  r15 data to store in string
;  xprintString current pointer in string.
putcString:
    mov.w   &xprintString,r14
    mov.b   r15,@r14
    inc.w   &xprintString

    ret


;      eXtended PRINT
;
;  All paramaters on stack
;  SP[0]   Return Address
;  SP[2]   Format String
;  SP[4+]  Extra Parameters
;-------------------------------------
.section .text
.global xprint
.func xprint
xprint: 
    //    mov.b   #3,&lcdChar

        pushm.w #11,r15
   //     push.w r15
   //     push.w r14
   //     push.w r13
   //     push.w r12
   //     push.w r11
   //     push.w r10
   //     push.w r9
   //     push.w r8
   //     push.w r7
   //     push.w r6
   //     push.w r5



        mov.w	24(r1),r14	; SP[2] -> r14	
        mov.w	r1,r12		; SP -> r12  
        add.w   #22,r12      
xp0:	
        mov.b   @r14+,r15   ; Parse format string
        tst.b   r15
        jz      xp2
        cmp.b   #'%',r15	; Is special?
        jeq	xp3
xp1:	
        
        push.w r14
        push.w r13
        push.w r12
        call    &out_func
        pop.w r12
        pop.w r13
        pop.w r14
        
	jmp     xp0	

xp3:	mov.b   #0,r13		; r13 digits
        mov.b	@r14+,r15	; get flags
        cmp.b	#'%',r15	; '%'?
        jeq	xp1		
        incd.w	r12         ; next parameter	

	
#ifdef SUPPORT_CHAR
        cmp.b   #'c',r15    ; If character
        jne     xp8
        mov.w   2(r12),r15  ; load char from stack
        jmp     xp1		    ; -> print character
#endif
xp8:         
#ifdef SUPPORT_STR
        cmp.b   #'s',r15
        jne     xp7
        
        mov.w   2(r12),r11	; get &str from stack
xpD:	mov.b   @r11+,r15	; r15 = *str++
        tst.b   r15		    ; is not \0
        jz      xp0         ; exit
        push.w r14
        push.w r13
        push.w r12
        call    &out_func
        pop.w r12
        pop.w r13
        pop.w r14
        jmp     xpD 	    ; loop time
#endif

xp7:    clr.w   r11
        mov.b   #' ',r6
        cmp.b   #'0',r15	; zero filled
        jne     xp5
        mov.w   #'0',r6
xp4:    mov.b   @r14+,r15	; get width
xp5:    cmp.b   #'9'+1,r15
        jge     xp70
        sub.b   #'0',r15
        mov.w   r15,r11

xp6:	mov.b   @r14+,r15

xp70:   mov.w   2(r12),r5   ; Load parameter value from stack

#ifdef SUPPORT_LONG
        clr.w   r9
        cmp.b   #'l',r15    ; Is long?
        jne     xp80
        incd.w   r12        ; point to low word
        mov.w   2(r12),r9   ; get upper word from stack
        mov.b   @r14+,r15   ; Next byte in string
#endif

xp80:           
#ifdef SUPPORT_BINARY
        cmp.b   #'b',r15
        mov.w   #2,r13
        jeq     xpA
#endif
#ifdef SUPPORT_OCTAL
        cmp.b   #'o',r15
        mov.w   #8,r13
        jeq     xpA
#endif	
#ifdef SUPPORT_UNSIGNED
        cmp.b   #'u',r15
        mov.w   #10,r13
        jeq     xpA
#endif
#ifdef SUPPORT_SIGNED
        cmp.b   #'d',r15
        mov.w   #-10-1,r13      ; inv will become 10
        jeq     xpA
#endif
#ifdef SUPPORT_HEX
        cmp.b   #'X',r15
        mov.w   #16,r13
        jeq     xpA
#endif
xp2:    
   //     pop.w r5
    //    pop.w r6
    //    pop.w r7
    //    pop.w r8
    //    pop.w r9
    //    pop.w r10
    //    pop.w r11
    //    pop.w r12
    //    pop.w r13
    //    pop.w r14
    //    pop.w r15
        popm.w  #11,r15
        ret	                ; exit

xpA:	     
;------XITOA--------------------------
xitoa:  clr.w   r8              
x0: 
#ifdef SUPPORT_SIGNED
        mov.b   #0,r15
        tst.w   r13         ; handle signed
        jge     x1          ; not signed?
        inv.w   r13
#ifdef SUPPORT_LONG        
        tst.w   r9
        jge     x01
        inv.w   r9
        jmp     x02
#endif
x01:    tst.w   r5          ; is signed. is x<0?
        jge     x1
x02:    inv.w   r5          ; Make it positive
        inc.w   r5          ; add 1 as -(x) xor -1 = (x)-1
        
        mov.b   #'-',r15    ; add on '-'
#endif
x1:
#ifdef SUPPORT_LONG
        mov.w   #32,r7	    ; 32bits
#else
        mov.w   #16,r7	    ; 16bits
#endif
        clr.w   r10         ; clr r10
        
x2: 	clrc
        rla.w   r5          ; rotate r5 into r9 32bits!
#ifdef SUPPORT_LONG
        rlc.w   r9
#endif
        rlc.b   r10         ; rotate r9 into r10
        cmp.b   r13,r10     ; compare radix
        jlo     x3

        sub.w   r13,r10	    ; subtract radix from r10
        inc.w   r5
x3: 	dec.w   r7
        jnz     x2
#ifdef SUPPORT_HEX
        cmp.b   #10,r10	    ; is numeral '0' - 'F' only hex type
        jlo     x4
        add.b   #7,r10      ; 7 = 'A' - '9'
#endif
x4:	    add.b   #'0',r10    ; convert to printable ascii
        push.b  r10         ; store on the stack as we are 
        inc.w   r8
        tst.w   r5
        jnz     x1

x5: 	
#ifdef SUPPORT_SIGNED
        tst.b   r15
        jz      x6
        push.b  r15
        inc.b   r8
#endif
x6: 
#ifdef SUPPORT_FIXED
        cmp.w   r11,r8
        jhs     x7
        push    r6
        inc.w   r8
        jmp     x6	
#endif        
      
x7:	pop.b   r15	        ; flush stack
        push.w r14
        push.w r13
        push.w r12
        call    &out_func
        pop.w r12
        pop.w r13
        pop.w r14
        dec.w   r8
        jnz     x7
;-------------------------------------
xpB:	jmp     xp0
.endfunc

