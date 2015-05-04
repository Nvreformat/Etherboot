SECTIONS
{
	. = 0x8000;
	
	.init : {
		*(.init*)
	}

	. = 0x600000;
	
	.text : {
		*(.text*)
	}

	_etext = .;

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	__BSS_START = .;

	.bss : {
		*(.bss*)
	}

	__BSS_END = .;
}
