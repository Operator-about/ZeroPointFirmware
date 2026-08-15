.section ".text.boot", "ax"
.global start_pi4
start_pi4:
    MRS X0, MPIDR_EL1
    AND X0, X0, 0xFF
    CMP X0, #0
    B.NE go_to_sleep

    LDR X0, =table_vector
    MSR VBAR_EL3, X0

    LDR X0, =EL3h_SP_bottom
    MOV SP, X0

    MRS X0, CPTR_EL3
    AND X0, X0, #~(1ULL << 10)
    MSR CPTR_EL3, X0

    B main_pi4

.section .text
.global main_pi4
.global jump_to_kernel

jump_to_kernel:
    MOV X0, XZR
    ORR X0, X0, #(1ULL << 10)
    AND X0, X0, #~(1ULL << 1)
    ORR X0, X0, #(1ULL << 0)
    MSR SCR_EL3, X0

    MOV X1, XZR
    MOV X0, XZR

    MOV X1, #(9ULL << 0)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 4)
    ORR X0, X0, #(1ULL << 7)
    MSR SPSR_EL3, X0

    LDR X0, =EL2h_SP_bottom
    MSR SP_EL2, X0

    LDR X0, =0x00080000
    MSR ELR_EL3, X0

    ISB
    ERET //Переход в ядро
go_to_sleep:
    WFE
    B go_to_sleep

.section .bss
.align 15 
EL3h_SP_top:
    .SPACE 32768
EL3h_SP_bottom: 
.align 15 
EL2h_SP_top:
    .SPACE 32768
EL2h_SP_bottom: 