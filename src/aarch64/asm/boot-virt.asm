.section ".text.boot", "ax"
.global start_virt

start_virt:
    MRS X0, MPIDR_EL1
    AND X0, X0, 0xFF
    CMP X0, #0
    B.NE go_sleep

    LDR X0, =EL3h_SP_bottom
    MOV SP, X0

    MOV X0, XZR
    AND X0, X0, #~(1ULL << 10)
    MSR CPTR_EL3, X0
    
    B main_virt

.section .text
.global main_virt
.global go_sleep

go_sleep:
    WFE

.section .bss
.align 15
EL3h_SP_top:
    .SPACE 32768
EL3h_SP_bottom: