/*链接脚本文件，告诉编译器代码和数据在内存中如何排布*/
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
/*OUTPUT_FORMAT("elf32-arm", "elf32-arm", "elf32-arm")*/
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0xc0008000;  /*程序的入口地址*/
	. = ALIGN(4);    /*对齐*/
	.text      :     /*代码段*/
	{
		start/start.o(.text)  /*第一个.o文件*/
		*(.text)   /*其他.o文件，编译器随意*/
	}
	. = ALIGN(4);
    .rodata :     /*只读数据端*/
	{ *(.rodata) }
    . = ALIGN(4);
    .data :      /*初始化的全局变量*/
	{ *(.data) }
    . = ALIGN(4);
	__bss_start = .; 
    .bss :       /*未初始化的变量*/
     { *(.bss) }
	__bss_end__ = .;
}
