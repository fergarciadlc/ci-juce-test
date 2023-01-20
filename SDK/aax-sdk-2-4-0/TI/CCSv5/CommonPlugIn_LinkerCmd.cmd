/*
 * Copyright 2014 by Avid Technology, Inc.
 *
 */

/******************************************************************
	CommonPlugIn_LinkerCmd.cmd

	Copyright (C) 2010 by Avid Technology, Inc.
	All Rights Reserved.

	Common linker settings for TI Plug-In's building
	as relocatable ELF DLL's
******************************************************************/

--relocatable
--dynamic=lib
/*-x*/

-lrts67plus_elf.lib
--export=_auto_init_elf
-stack 0x0 /* Shell owns the stack; none defined for the plug-ins */

-heap  0x0100 /* don't need heap */ 



/****************************************************************************/
/*  Memory Specification                                                    */
/****************************************************************************/

MEMORY
{
  INTERNAL_MEM			: origin = 0x00000000 	length = 0x80100000 
  EXTERNAL_MEM    		: origin = 0x80100000 	length = 0x7FEFFFFF
}

/****************************************************************************/
/*  Specify the Output Sections                                             */
/****************************************************************************/

SECTIONS
{
	/* Read-only (RO) Sections */
	.text:			load = INTERNAL_MEM align 32 		/* Code */
	.switch:		load = INTERNAL_MEM					/* Switch tables */
	.const:			load = INTERNAL_MEM 				/* Far const data */
	.data:			load = INTERNAL_MEM
	.cinit:			load = INTERNAL_MEM 				/* Boot-time init tables */
	
	/* DP-relative Read-Write (RW) Sections - TODO: Not using DP-relative data for Plug-Ins*/
    GROUP (NEARDP_DATA)	/* from http://focus.ti.com/lit/an/sprab90/sprab90.pdf */
    {
	    .neardata						/* Near RW data (initialized) */
	    .rodata							/* Near const data */
	    .bss							/* Near RW data (uninitialized) */
    }	load = INTERNAL_MEM
    
    /* Absolute Read-Write (RW) Sections */
	.far:			load = INTERNAL_MEM						/* Far RW data (initialized) */
	.fardata:		load = INTERNAL_MEM						/* Far RW data (uninitialized) */
	.stack:			load = INTERNAL_MEM, fill = 0xdeadbeef	/* Program stack */
	.sysmem:		load = INTERNAL_MEM						/* Dynamic data (heap) */
	
	/* Other c6x EABI sections */
	.init_array:	load = INTERNAL_MEM	/* Pre-main constructors */
	.fini_array:	load = INTERNAL_MEM	/* Pre-main constructors */
    .c6xabi.exidx:	load = INTERNAL_MEM	/* Index table for exception handling */
    .c6xabi.extab:	load = INTERNAL_MEM	/* Unwinding instructions for exception handling */
    .binit:			load = EXTERNAL_MEM	/* Boot-time copy tables */
	.cio:			load = INTERNAL_MEM	/* Handshaking buffer for host-based stdio */
	.args:			load = INTERNAL_MEM	/* Command arguments for host-based loader */
	.ppinfo:		load = INTERNAL_MEM	/* Correlation tables for compiler-based profiling */
	.ppdata:		load = INTERNAL_MEM

	.vectors:		load = INTERNAL_MEM
	.plt:			load = INTERNAL_MEM 		
	
	/* User-defined sections */
	.intmem:		load = INTERNAL_MEM
	.extmem:		load = EXTERNAL_MEM align 32
	.extmemdata:	load = EXTERNAL_MEM
}
