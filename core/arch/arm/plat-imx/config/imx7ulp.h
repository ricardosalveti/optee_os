/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright 2017-2018 NXP
 *
 */

#ifndef CONFIG_IMX7ULP_H
#define CONFIG_IMX7ULP_H

#ifndef CFG_UART_BASE
#define CFG_UART_BASE	(UART4_BASE)
#endif

#define DRAM0_BASE		(0x80000000 - 0x20000000)
#define DRAM0_SIZE		CFG_DDR_SIZE

#define CONSOLE_UART_BASE	(CFG_UART_BASE)
#endif