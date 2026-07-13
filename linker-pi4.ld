ENTRY(start_pi4)

MEMORY{
    RAM(rwx) : ORIGIN = 0x00000000, LENGTH = 1M
}

SECTIONS{

    . = ORIGIN(RAM);

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

    .kernel_buffer 0x00080000 : {
        KEEP(*(.kernel_buffer))
    } > RAM

    . = ALIGN(8);
    __bss_start = .;
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }
    __bss_end = .;

    __end = .;
}