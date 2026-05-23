.section .vectors, "ax"
.global table_vector

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
        ERET
    .align 7
    fiq_handel_elxh:
        ERET
    .align 7
    serror_elxh:
        ERET
    .align 7
    .SPACE 1024