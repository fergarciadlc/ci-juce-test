/*
 * Copyright 2014 by Avid Technology, Inc.
 *
 */

-mv67p
--abi=eabi
--mem_model:data=far
--gcc
--long_precision_bits=32
--keep_asm
--src_interlist
--asm_listing
--diag_warning=225
--debug_software_pipeline	/* debug_software_pipeline puts extra software pipeline optimization info in the asm files, has no performance effect on debug or release builds. */
