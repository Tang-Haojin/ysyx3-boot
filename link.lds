OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{ 
	flash (rxai!w) : ORIGIN = 0x30000000, LENGTH = 16M
}

SECTIONS
{
  . = 0x30000000;
  .text :
  {
    *(.text)
  } >flash
}
