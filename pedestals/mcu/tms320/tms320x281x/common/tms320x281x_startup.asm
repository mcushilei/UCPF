;// TI File $Revision: /main/2 $
;// Checkin $Date: April 29, 2005   11:11:32 $
;//###########################################################################
;//
;// FILE:  DSP281x_CodeStartBranch.asm	
;//
;// TITLE: Branch for redirecting code execution after boot. 
;//
;//###########################################################################
;// $TI Release: DSP281x C/C++ Header Files V1.20 $
;// $Release Date: July 27, 2009 $
;//###########################################################################

***********************************************************************

WD_DISABLE	.set	1		;set to 1 to disable WD, else set to 0

    .ref _c_int00

***********************************************************************
* Function: codestart section
*
* Description: Branch to code starting point
***********************************************************************

    .sect "codestart"

code_start:
    LB reset_handler        ;Branch to watchdog disable code


***********************************************************************
* Function: wd_disable
*
* Description: Disables the watchdog timer
***********************************************************************

    .text

reset_handler:
    SETC    OBJMODE         ;Set OBJMODE for C28x object code
    .if WD_DISABLE == 1
    EALLOW                  ;Enable EALLOW protected register access
    MOVZ    DP, #7029h>>6   ;Set data page for WDCR register
    MOV     @7029h, #0068h  ;Set WDDIS bit in WDCR to disable WD
    EDIS                    ;Disable EALLOW protected register access
    .endif
    LB      _c_int00        ;Branch to start of boot.asm in RTS library

	.end
	
; end of file CodeStartBranch.asm
