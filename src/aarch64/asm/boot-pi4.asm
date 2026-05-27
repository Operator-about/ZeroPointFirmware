.section ".text.boot", "ax"
.global start_pi4
start_pi4:
    MRS X0, MPIDR_EL1
    AND X0, X0, 0xFF
    CMP X0, #0
    B.NE go_to_sleep

    ADR X0, table_vector
    MSR VBAR_EL3, X0

    LDR X0, =EL3h_SP_bottom
    MOV SP, X0

    MSR DAIFSet, #2

    B main_pi4

.section .text
.global main_pi4
.global jump_in_kernel

jump_in_kernel:
    MRS X0, SCR_EL3
    ORR X0, X0, #(1ULL << 10)
    AND X0, X0, #~(1ULL << 1)
    ORR X0, X0, #(1ULL << 0)
    MSR SCR_EL3, X0

    MRS X0, SPSR_EL3
    MOV X1, #(9ULL << 0)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 4)
    ORR X0, X0, #(1ULL << 7)
    MSR SPSR_EL3, X0

    MOV X0, 0x00080000
    MSR ELR_EL3, X0

    ADR X0, EL2h_SP_bottom
    MSR SP_EL2, X0

    LDR X10, =0xFE201000 //UARTPL011 - База
    LDR X11, =0xFE201018 //UARTPL011 - FR
    LDR X12, =0xFE201040 //UARTPL011 - MIS
    LDR X13, =0xFF842000 //GIC - GICC

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