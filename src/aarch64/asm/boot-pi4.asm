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
.global EL3_to_EL1

EL3_to_EL1:
    MRS X0, SCR_EL3
    ORR X0, X0, #(1ULL << 10)
    AND X0, X0, #~(1ULL << 1)
    ORR X0, X0, #(1ULL << 0)
    MSR SCR_EL3, X0

    MRS X0, SPSR_EL3
    MOV X1, #(5ULL << 0)
    ORR X0, X0, X1
    AND X0, X0, #~(1ULL << 4)
    ORR X0, X0, #(1ULL << 7)
    MSR SPSR_EL3, X0

    ADR X0, EL1h_SP_bottom
    MSR SP_EL1, X0

    LDR X1, =0xFE201000 //UART
    LDR X2, =0xFF841000 //GICD
    LDR X3, =0xFF842000 //GICC

    ERET //Переход в ядро

go_to_sleep:
    WFE
    B go_to_sleep

.section .bss
.align 15 
EL3h_SP_top:
    .SPACE 32768
EL3h_SP_bottom: 
.align 12
EL1h_SP_top:
    .SPACE 4096
EL1h_SP_bottom: