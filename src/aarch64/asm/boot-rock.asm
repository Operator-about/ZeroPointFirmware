.section ".text.boot", "ax"
.global start_rock

start_rock:
    MRS X0, MPIDR_EL1
    AND X0, X0, 0xFF
    CMP X0, #0
    B.NE go_to_sleep
    
    LDR X0, =EL3h_SP_top
    MOV SP, X0

    B main_rock

.section .text
.global main_rock
.global go_to_sleep

go_to_sleep:
    WFE

.section .bss
.align 15
EL3h_SP_top:
    .SPACE 32768
EL3h_SP_bottom: