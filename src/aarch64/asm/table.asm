.section .vectors, "ax", %progbits
.global table_vector
.global IRQh_handel

.align 11
table_vector:
    synchronous_elxt:
        ERET
    .align 7
    irq_handel_elxt:
        ERET
    .align 7
    fiq_handel_elxt:
        ERET
    .align 7
    serror_elxt:
        ERET
    .align 7
    synchronous_elxh:
        ERET
    .align 7
    irq_handel_elxh:
        STP X29, X30, [SP, #-16]!

        BL IRQh_handel

        LDP X29, X30, [SP], #16
        ERET
    .align 7
    fiq_handel_elxh:
        ERET
    .align 7
    serror_elxh:
        ERET
    .align 7
    .SPACE 1024