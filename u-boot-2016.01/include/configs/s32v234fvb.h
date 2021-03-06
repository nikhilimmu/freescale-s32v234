/*
 * (C) Copyright 2015-2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Configuration settings for the Freescale S32V234 FVB board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* The configurations of this board depend on the definitions in this file and
* the ones in the header included at the end, configs/s32v234_common.h */

#define CONFIG_DDR_INIT_DELAY		1000


#define CONFIG_SYS_FSL_ESDHC_ADDR	0

#define CONFIG_CMD_EXT2 /* EXT2 Support */

#define CONFIG_FEC_XCV_TYPE     RMII
#define CONFIG_FEC_MXC_PHYADDR  1
#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL

#define FDT_FILE		s32v234-fvb.dtb


#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)

/* #define CONFIG_CMD_PCI */

/* we include this file here because it depends on the above definitions */
#include <configs/s32v234_common.h>

#endif
