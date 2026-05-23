ENTRY(start_pi4)


SECTIONS{

    . = 0x00000000;

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    . = ALIGN(2048);
    .vectors : {
        KEEP(*(.vectors))
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    . = ALIGN(8);
    __bss_start = .;
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }
    __bss_end = .;

    __end = .;
}