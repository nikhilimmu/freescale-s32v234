/*
 * s32v234 pinctrl driver based on imx pinmux and pinconf core
 *
 * Copyright 2015 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/err.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>

#include "pinctrl-s32v.h"

enum s32v234_pads {
	S32V234_PAD_PA12_UART0_TXD = 12,
	S32V234_PAD_PA11_UART0_RXD_OUT = 11,
	S32V234_PAD_PA11_UART0_RXD_IN = 712,
	S32V234_PAD_PA14_UART1_TXD = 14,
	S32V234_PAD_PA13_UART1_RXD_OUT = 13,
	S32V234_PAD_PA13_UART1_RXD_IN = 714,
	S32V234_PAD_PK6_USDHC_CLK_OUT = 150,
	S32V234_PAD_PK6_USDHC_CLK_IN = 902,
	S32V234_PAD_PK7_USDHC_CMD_OUT = 151,
	S32V234_PAD_PK7_USDHC_CMD_IN = 901,
	S32V234_PAD_PK8_USDHC_DAT0_OUT = 152,
	S32V234_PAD_PK8_USDHC_DAT0_IN = 903,
	S32V234_PAD_PK9_USDHC_DAT1_OUT = 153,
	S32V234_PAD_PK9_USDHC_DAT1_IN = 904,
	S32V234_PAD_PK10_USDHC_DAT2_OUT = 154,
	S32V234_PAD_PK10_USDHC_DAT2_IN = 905,
	S32V234_PAD_PK11_USDHC_DAT3_OUT = 155,
	S32V234_PAD_PK11_USDHC_DAT3_IN = 906,
	S32V234_PAD_PK15_USDHC_DAT4_OUT = 159,
	S32V234_PAD_PK15_USDHC_DAT4_IN = 907,
	S32V234_PAD_PL0_USDHC_DAT5_OUT = 160,
	S32V234_PAD_PL0_USDHC_DAT5_IN = 908,
	S32V234_PAD_PL1_USDHC_DAT6_OUT = 161,
	S32V234_PAD_PL1_USDHC_DAT6_IN = 909,
	S32V234_PAD_PL2_USDHC_DAT7_OUT = 162,
	S32V234_PAD_PL2_USDHC_DAT7_IN = 910,
	S32V234_PAD_PG3_I2C0_DATA_OUT = 99,
	S32V234_PAD_PG3_I2C0_DATA_IN = 781,
	S32V234_PAD_PG4_I2C0_SCLK_OUT = 100,
	S32V234_PAD_PG4_I2C0_SCLK_IN = 780,
	S32V234_PAD_PG5_I2C1_DATA_OUT = 101,
	S32V234_PAD_PG5_I2C1_DATA_IN = 783,
	S32V234_PAD_PG6_I2C1_SCLK_OUT = 102,
	S32V234_PAD_PG6_I2C1_SCLK_IN = 782,
	S32V234_PAD_PB3_I2C2_DATA_OUT = 19,
	S32V234_PAD_PB3_I2C2_DATA_IN = 785,
	S32V234_PAD_PB4_I2C2_SCLK_OUT = 20,
	S32V234_PAD_PB5_I2C2_SCLK_IN = 784,
	S32V234_PAD_PC13_MDC = 45,
	S32V234_PAD_PC14_MDIO_OUT = 46,
	S32v234_PAD_PC14_MDIO_IN = 981,
	S32V234_PAD_PC15_TXCLK_OUT = 47,
	S32V234_PAD_PC15_TXCLK_IN = 978,
	S32V234_PAD_PD0_RXCLK_OUT = 48,
	S32V234_PAD_PD0_RXCLK_IN = 979,
	S32V234_PAD_PD1_RX_D1_OUT = 49,
	S32V234_PAD_PD1_RX_D1_IN = 974,
	S32V234_PAD_PD2_RX_D2_OUT = 50,
	S32V234_PAD_PD2_RX_D2_IN = 975,
	S32V234_PAD_PD3_RX_D3_OUT = 51,
	S32V234_PAD_PD3_RX_D3_IN = 976,
	S32V234_PAD_PD4_RX_D4_OUT = 52,
	S32V234_PAD_PD4_RX_D4_IN = 977,
	S32V234_PAD_PD4_RX_DV_OUT = 53,
	S32V234_PAD_PD4_RX_DV_IN = 973,
	S32V234_PAD_PD7_TX_D0_OUT = 55,
	S32V234_PAD_PD8_TX_D1_OUT = 56,
	S32V234_PAD_PD9_TX_D2_OUT = 57,
	S32V234_PAD_PD10_TX_D3_OUT = 58,
	S32V234_PAD_PD11_TX_EN_OUT = 59,
	S32V234_PAD_PH8__DCU_HSYNC_C1 = 120,
	S32V234_PAD_PH9__DCU_VSYNC_C2 = 121,
	S32V234_PAD_PH10__DCU_DE_C3 = 122,
	S32V234_PAD_PH10__DCU_PCLK_D1 = 124,
	S32V234_PAD_PH13__DCU_R0_D2 = 125,
	S32V234_PAD_PH14__DCU_R1_D3 = 126,
	S32V234_PAD_PH15__DCU_R2_D4 = 127,
	S32V234_PAD_PJ0__DCU_R3_D5 = 128,
	S32V234_PAD_PJ1__DCU_R4_D6 = 129,
	S32V234_PAD_PJ2__DCU_R5_D7 = 130,
	S32V234_PAD_PJ3__DCU_R6_D8 = 131,
	S32V234_PAD_PJ4__DCU_R7_D9 = 132,
	S32V234_PAD_PJ5__DCU_G0_D10 = 133,
	S32V234_PAD_PJ6__DCU_G1_D11 = 134,
	S32V234_PAD_PJ7__DCU_G2_D12 = 135,
	S32V234_PAD_PJ8__DCU_G3_D13 = 136,
	S32V234_PAD_PJ9__DCU_G4_D14 = 137,
	S32V234_PAD_PJ10__DCU_G5_D15 = 138,
	S32V234_PAD_PJ11__DCU_G6_D16 = 139,
	S32V234_PAD_PJ12__DCU_G7_D17 = 140,
	S32V234_PAD_PJ13__DCU_B0_D18 = 141,
	S32V234_PAD_PJ14__DCU_B1_D19 = 142,
	S32V234_PAD_PJ15__DCU_B2_D20 = 143,
	S32V234_PAD_PK0__DCU_B3_D21 = 144,
	S32V234_PAD_PK1__DCU_B4_D22 = 145,
	S32V234_PAD_PK2__DCU_B5_D23 = 146,
	S32V234_PAD_PK3__DCU_B6_D24 = 147,
	S32V234_PAD_PK4__DCU_B7_D25 = 148,
	S32V234_PAD_PD13_VIU0_EN = 61,
	S32V234_PAD_PD13_VIU0_PCLK  = 624,
	S32V234_PAD_PD14_VIU0_EN = 62,
	S32V234_PAD_PD14_VIU0_HSYNC = 622,
	S32V234_PAD_PD15_VIU0_EN = 63,
	S32V234_PAD_PD15_VIU0_VSYNC = 623,
	S32V234_PAD_PF3_VIU_EN = 83,
	S32V234_PAD_PF3_VIU0_D4 = 629,
	S32V234_PAD_PF4_VIU_EN = 84,
	S32V234_PAD_PF4_VIU0_D5 = 630,
	S32V234_PAD_PF5_VIU_EN = 85,
	S32V234_PAD_PF5_VIU0_D6 = 631,
	S32V234_PAD_PF6_VIU_EN = 86,
	S32V234_PAD_PF6_VIU0_D7 = 632,
	S32V234_PAD_PE0_VIU0_EN = 64,
	S32V234_PAD_PE0_VIU0_D8	= 633,
	S32V234_PAD_PE1_VIU0_EN = 65,
	S32V234_PAD_PE1_VIU0_D9 = 634,
	S32V234_PAD_PE2_VIU0_EN = 66,
	S32V234_PAD_PE2_VIU0_D10 = 635,
	S32V234_PAD_PE3_VIU0_EN = 67,
	S32V234_PAD_PE3_VIU0_D11 = 636,
	S32V234_PAD_PE4_VIU0_EN = 68,
	S32V234_PAD_PE4_VIU0_D12 = 637,
	S32V234_PAD_PE5_VIU0_EN = 69,
	S32V234_PAD_PE5_VIU0_D13 = 638,
	S32V234_PAD_PE6_VIU0_EN = 70,
	S32V234_PAD_PE6_VIU0_D14 = 639,
	S32V234_PAD_PE7_VIU0_EN = 71,
	S32V234_PAD_PE7_VIU0_D15 = 640,
	S32V234_PAD_PE8_VIU0_EN = 72,
	S32V234_PAD_PE8_VIU0_D16 = 641,
	S32V234_PAD_PE9_VIU0_EN = 73,
	S32V234_PAD_PE9_VIU0_D17 = 642,
	S32V234_PAD_PE10_VIU0_EN = 74,
	S32V234_PAD_PE10_VIU0_D18 = 643,
	S32V234_PAD_PE11_VIU0_EN = 75,
	S32V234_PAD_PE11_VIU0_D19 = 644,
	S32V234_PAD_PE12_VIU0_EN = 76,
	S32V234_PAD_PE12_VIU0_D20 = 645,
	S32V234_PAD_PE13_VIU0_EN = 77,
	S32V234_PAD_PE13_VIU0_D21 = 646,
	S32V234_PAD_PE14_VIU0_EN = 78,
	S32V234_PAD_PE14_VIU0_D22 = 647,
	S32V234_PAD_PE15_VIU0_EN = 79,
	S32V234_PAD_PE15_VIU0_D23 = 648,
	S32V234_PAD_PF0_VIU1_EN = 80,
	S32V234_PAD_PF0_VIU1_PCLK = 659,
	S32V234_PAD_PF1_VIU1_EN = 81,
	S32V234_PAD_PF1_VIU1_HSYNC = 657,
	S32V234_PAD_PF2_VIU1_EN = 82,
	S32V234_PAD_PF2_VIU1_VSYNC = 658,
	S32V234_PAD_PF3_VIU1_D8 = 668,
	S32V234_PAD_PF4_VIU1_D9 = 669,
	S32V234_PAD_PF5_VIU1_D10 = 670,
	S32V234_PAD_PF6_VIU1_D11 = 671,
	S32V234_PAD_PF7_VIU1_EN = 87,
	S32V234_PAD_PF7_VIU1_D12 = 672,
	S32V234_PAD_PF8_VIU1_EN = 88,
	S32V234_PAD_PF8_VIU1_D13 = 673,
	S32V234_PAD_PF9_VIU1_EN = 89,
	S32V234_PAD_PF9_VIU1_D14 = 674,
	S32V234_PAD_PF10_VIU1_EN = 90,
	S32V234_PAD_PF10_VIU1_D15 = 675,
	S32V234_PAD_PF11_VIU1_EN = 91,
	S32V234_PAD_PF11_VIU1_D16 = 676,
	S32V234_PAD_PF12_VIU1_EN = 92,
	S32V234_PAD_PF12_VIU1_D17 = 677,
	S32V234_PAD_PF13_VIU1_EN = 93,
	S32V234_PAD_PF13_VIU1_D18 = 678,
	S32V234_PAD_PF14_VIU1_EN = 94,
	S32V234_PAD_PF14_VIU1_D19 = 679,
	S32V234_PAD_PF15_VIU1_EN = 95,
	S32V234_PAD_PF15_VIU1_D20 = 680,
	S32V234_PAD_PG0_VIU1_EN = 96,
	S32V234_PAD_PG0_VIU1_D21 = 681,
	S32V234_PAD_PG1_VIU1_EN = 97,
	S32V234_PAD_PG1_VIU1_D22 = 682,
	S32V234_PAD_PG2_VIU1_EN = 98,
	S32V234_PAD_PG2_VIU1_D23 = 683,
	S32V234_PAD_PB6__SPI0_SOUT_OUT = 22,
	S32V234_PAD_PB7__SPI0_SIN_OUT = 23,
	S32V234_PAD_PB7__SPI0_SIN_IN = 800,
	S32V234_PAD_PB5__SPI0_SCK_OUT = 21,
	S32V234_PAD_PB8__SPI0_CS0_OUT = 24,
	S32V234_PAD_PB10__SPI1_SOUT_OUT = 26,
	S32V234_PAD_PB11__SPI1_SIN_OUT = 27,
	S32V234_PAD_PB11__SPI1_SIN_IN = 803,
	S32V234_PAD_PB9__SPI1_SCK_OUT = 25,
	S32V234_PAD_PB12__SPI1_CS0_OUT = 28,
	S32V234_PAD_PB14__SPI2_SOUT_OUT = 30,
	S32V234_PAD_PB15__SPI2_SIN_OUT = 31,
	S32V234_PAD_PB15__SPI2_SIN_IN = 806,
	S32V234_PAD_P13__SPI2_SCK_OUT = 29,
	S32V234_PAD_PC0__SPI2_CS0_OUT = 32,
	S32V234_PAD_PC2__SPI3_SOUT_OUT = 34,
	S32V234_PAD_PC3__SPI3_SIN_OUT = 35,
	S32V234_PAD_PC3__SPI3_SIN_IN = 809,
	S32V234_PAD_PC1__SPI3_SCK_OUT = 33,
	S32V234_PAD_PC4__SPI3_CS0_OUT = 36,
	S32V234_PAD_PA0__SIUL_GPIO0 = 0,
	S32V234_PAD_PA1__SIUL_GPIO1 = 1,
	S32V234_PAD_PA6__SIUL_GPIO6 = 6,
	S32V234_PAD_PA7__SIUL_GPIO7 = 7,
	S32V234_PAD_PA8__SIUL_GPIO8 = 8,
	S32V234_PAD_PA9__SIUL_GPIO9 = 9,
	S32V234_PAD_PA10__SIUL_GPIO10 = 10,
	S32V234_PAD_PA15__SIUL_GPIO15 = 15,
	S32V234_PAD_PB0__SIUL_GPIO16 = 16,
	S32V234_PAD_PB1__SIUL_GPIO17 = 17,
	S32V234_PAD_PB2__SIUL_GPIO18 = 18,
	S32V234_PAD_PC5__SIUL_GPIO37 = 37,
	S32V234_PAD_PC6__SIUL_GPIO38 = 38,
	S32V234_PAD_PC7__SIUL_GPIO39 = 39,
	S32V234_PAD_PC8__SIUL_GPIO40 = 40,
	S32V234_PAD_PC9__SIUL_GPIO41 = 41,
	S32V234_PAD_PC10__SIUL_GPIO42 = 42,
	S32V234_PAD_PC11__SIUL_GPIO43 = 43,
	S32V234_PAD_PC12__SIUL_GPIO44 = 44,
	S32V234_PAD_PD6__SIUL_GPIO54 = 54,
	S32V234_PAD_PD12__SIUL_GPIO60 = 60,
	S32V234_PAD_PG7__SIUL_GPIO103 = 103,
	S32V234_PAD_PG8__SIUL_GPIO104 = 104,
	S32V234_PAD_PG9__SIUL_GPIO105 = 105,
	S32V234_PAD_PG10__SIUL_GPIO106 = 106,
	S32V234_PAD_PG11__SIUL_GPIO107 = 107,
	S32V234_PAD_PG12__SIUL_GPIO108 = 108,
	S32V234_PAD_PG13__SIUL_GPIO109 = 109,
	S32V234_PAD_PG14__SIUL_GPIO110 = 110,
	S32V234_PAD_PG15__SIUL_GPIO111 = 111,
	S32V234_PAD_PH0__SIUL_GPIO112 = 112,
	S32V234_PAD_PH1__SIUL_GPIO113 = 113,
	S32V234_PAD_PH2__SIUL_GPIO114 = 114,
	S32V234_PAD_PH3__SIUL_GPIO115 = 115,
	S32V234_PAD_PH4__SIUL_GPIO116 = 116,
	S32V234_PAD_PH5__SIUL_GPIO117 = 117,
	S32V234_PAD_PH6__SIUL_GPIO118 = 118,
	S32V234_PAD_PH7__SIUL_GPIO119 = 119,
	S32V234_PAD_PH11__SIUL_GPIO123 = 123,
	S32V234_PAD_PK5__SIUL_GPIO149 = 149,
	S32V234_PAD_PK12__SIUL_GPIO156 = 156,
	S32V234_PAD_PK13__SIUL_GPIO157 = 157,
	S32V234_PAD_PK14__SIUL_GPIO158 = 158,
	S32V234_PAD_PA0__SIUL_EIRQ0 = 590,
	S32V234_PAD_PA1__SIUL_EIRQ1 = 591,
	S32V234_PAD_PA2__SIUL_EIRQ2 = 592,
	S32V234_PAD_PA3__SIUL_EIRQ3 = 593,
	S32V234_PAD_PA4__SIUL_EIRQ4 = 594,
	S32V234_PAD_PA5__SIUL_EIRQ5 = 595,
	S32V234_PAD_PA6__SIUL_EIRQ6 = 596,
	S32V234_PAD_PA7__SIUL_EIRQ7 = 597,
	S32V234_PAD_PA8__SIUL_EIRQ8 = 598,
	S32V234_PAD_PA9__SIUL_EIRQ9 = 599,
	S32V234_PAD_PA10__SIUL_EIRQ10 = 600,
	S32V234_PAD_PA11__SIUL_EIRQ11 = 601,
	S32V234_PAD_PA12__SIUL_EIRQ12 = 602,
	S32V234_PAD_PA13__SIUL_EIRQ13 = 603,
	S32V234_PAD_PA14__SIUL_EIRQ14 = 604,
	S32V234_PAD_PA15__SIUL_EIRQ15 = 605,
	S32V234_PAD_PB0__SIUL_EIRQ16 = 606,
	S32V234_PAD_PB1__SIUL_EIRQ17 = 607,
	S32V234_PAD_PB2__SIUL_EIRQ18 = 608,
	S32V234_PAD_PB3__SIUL_EIRQ19 = 609,
	S32V234_PAD_PB4__SIUL_EIRQ20 = 610,
	S32V234_PAD_PB5__SIUL_EIRQ21 = 611,
	S32V234_PAD_PB6__SIUL_EIRQ22 = 612,
	S32V234_PAD_PB7__SIUL_EIRQ23 = 613,
	S32V234_PAD_PB8__SIUL_EIRQ24 = 614,
	S32V234_PAD_PB9__SIUL_EIRQ25 = 615,
	S32V234_PAD_PB10__SIUL_EIRQ26 = 616,
	S32V234_PAD_PB11__SIUL_EIRQ27 = 617,
	S32V234_PAD_PB12__SIUL_EIRQ28 = 618,
	S32V234_PAD_PB13__SIUL_EIRQ29 = 619,
	S32V234_PAD_PB14__SIUL_EIRQ30 = 620,
	S32V234_PAD_PB15__SIUL_EIRQ31 = 621,
	S32V234_PAD_PA2__CAN_FD0_TXD = 2,
	S32V234_PAD_PA3__CAN_FD0_RXD_OUT = 3,
	S32V234_PAD_PA3__CAN_FD0_RXD_IN = 700,
	S32V234_PAD_PA4__CAN_FD1_TXD = 4,
	S32V234_PAD_PA5__CAN_FD1_RXD_OUT = 5,
	S32V234_PAD_PA5__CAN_FD1_RXD_IN = 701,
};

/* Pad names for the pinmux subsystem */
static const struct pinctrl_pin_desc s32v234_pinctrl_pads[] = {
	S32V_PINCTRL_PIN(S32V234_PAD_PA12_UART0_TXD),
	S32V_PINCTRL_PIN(S32V234_PAD_PA11_UART0_RXD_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PA11_UART0_RXD_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PA14_UART1_TXD),
	S32V_PINCTRL_PIN(S32V234_PAD_PA13_UART1_RXD_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PA13_UART1_RXD_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK6_USDHC_CLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK6_USDHC_CLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK7_USDHC_CMD_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK7_USDHC_CMD_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK8_USDHC_DAT0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK8_USDHC_DAT0_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK9_USDHC_DAT1_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK9_USDHC_DAT1_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK10_USDHC_DAT2_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK10_USDHC_DAT2_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK11_USDHC_DAT3_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK11_USDHC_DAT3_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PK15_USDHC_DAT4_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PK15_USDHC_DAT4_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PL0_USDHC_DAT5_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PL0_USDHC_DAT5_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PL1_USDHC_DAT6_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PL1_USDHC_DAT6_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PL2_USDHC_DAT7_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PL2_USDHC_DAT7_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG3_I2C0_DATA_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PG3_I2C0_DATA_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG4_I2C0_SCLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PG4_I2C0_SCLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG5_I2C1_DATA_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PG5_I2C1_DATA_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG6_I2C1_SCLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PG6_I2C1_SCLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PB3_I2C2_DATA_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB3_I2C2_DATA_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PB4_I2C2_SCLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB5_I2C2_SCLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PC13_MDC),
	S32V_PINCTRL_PIN(S32V234_PAD_PC14_MDIO_OUT),
	S32V_PINCTRL_PIN(S32v234_PAD_PC14_MDIO_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PC15_TXCLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC15_TXCLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD0_RXCLK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD0_RXCLK_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD1_RX_D1_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD1_RX_D1_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD2_RX_D2_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD2_RX_D2_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD3_RX_D3_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD3_RX_D3_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD4_RX_D4_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD4_RX_D4_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD4_RX_DV_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD4_RX_DV_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD7_TX_D0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD8_TX_D1_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD9_TX_D2_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD10_TX_D3_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PD11_TX_EN_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PH8__DCU_HSYNC_C1),
	S32V_PINCTRL_PIN(S32V234_PAD_PH9__DCU_VSYNC_C2),
	S32V_PINCTRL_PIN(S32V234_PAD_PH10__DCU_DE_C3),
	S32V_PINCTRL_PIN(S32V234_PAD_PH10__DCU_PCLK_D1),
	S32V_PINCTRL_PIN(S32V234_PAD_PH13__DCU_R0_D2),
	S32V_PINCTRL_PIN(S32V234_PAD_PH14__DCU_R1_D3),
	S32V_PINCTRL_PIN(S32V234_PAD_PH15__DCU_R2_D4),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ0__DCU_R3_D5),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ1__DCU_R4_D6),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ2__DCU_R5_D7),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ3__DCU_R6_D8),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ4__DCU_R7_D9),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ5__DCU_G0_D10),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ6__DCU_G1_D11),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ7__DCU_G2_D12),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ8__DCU_G3_D13),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ9__DCU_G4_D14),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ10__DCU_G5_D15),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ11__DCU_G6_D16),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ12__DCU_G7_D17),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ13__DCU_B0_D18),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ14__DCU_B1_D19),
	S32V_PINCTRL_PIN(S32V234_PAD_PJ15__DCU_B2_D20),
	S32V_PINCTRL_PIN(S32V234_PAD_PK0__DCU_B3_D21),
	S32V_PINCTRL_PIN(S32V234_PAD_PK1__DCU_B4_D22),
	S32V_PINCTRL_PIN(S32V234_PAD_PK2__DCU_B5_D23),
	S32V_PINCTRL_PIN(S32V234_PAD_PK3__DCU_B6_D24),
	S32V_PINCTRL_PIN(S32V234_PAD_PK4__DCU_B7_D25),
	S32V_PINCTRL_PIN(S32V234_PAD_PD13_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD13_VIU0_PCLK),
	S32V_PINCTRL_PIN(S32V234_PAD_PD14_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD14_VIU0_HSYNC),
	S32V_PINCTRL_PIN(S32V234_PAD_PD15_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PD15_VIU0_VSYNC),
	S32V_PINCTRL_PIN(S32V234_PAD_PF3_VIU_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF3_VIU0_D4),
	S32V_PINCTRL_PIN(S32V234_PAD_PF4_VIU_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF4_VIU0_D5),
	S32V_PINCTRL_PIN(S32V234_PAD_PF5_VIU_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF5_VIU0_D6),
	S32V_PINCTRL_PIN(S32V234_PAD_PF6_VIU_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF6_VIU0_D7),
	S32V_PINCTRL_PIN(S32V234_PAD_PE0_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE0_VIU0_D8),
	S32V_PINCTRL_PIN(S32V234_PAD_PE1_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE1_VIU0_D9),
	S32V_PINCTRL_PIN(S32V234_PAD_PE2_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE2_VIU0_D10),
	S32V_PINCTRL_PIN(S32V234_PAD_PE3_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE3_VIU0_D11),
	S32V_PINCTRL_PIN(S32V234_PAD_PE4_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE4_VIU0_D12),
	S32V_PINCTRL_PIN(S32V234_PAD_PE5_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE5_VIU0_D13),
	S32V_PINCTRL_PIN(S32V234_PAD_PE6_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE6_VIU0_D14),
	S32V_PINCTRL_PIN(S32V234_PAD_PE7_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE7_VIU0_D15),
	S32V_PINCTRL_PIN(S32V234_PAD_PE8_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE8_VIU0_D16),
	S32V_PINCTRL_PIN(S32V234_PAD_PE9_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE9_VIU0_D17),
	S32V_PINCTRL_PIN(S32V234_PAD_PE10_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE10_VIU0_D18),
	S32V_PINCTRL_PIN(S32V234_PAD_PE11_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE11_VIU0_D19),
	S32V_PINCTRL_PIN(S32V234_PAD_PE12_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE12_VIU0_D20),
	S32V_PINCTRL_PIN(S32V234_PAD_PE13_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE13_VIU0_D21),
	S32V_PINCTRL_PIN(S32V234_PAD_PE14_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE14_VIU0_D22),
	S32V_PINCTRL_PIN(S32V234_PAD_PE15_VIU0_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PE15_VIU0_D23),
	S32V_PINCTRL_PIN(S32V234_PAD_PF0_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF0_VIU1_PCLK),
	S32V_PINCTRL_PIN(S32V234_PAD_PF1_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF1_VIU1_HSYNC),
	S32V_PINCTRL_PIN(S32V234_PAD_PF2_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF2_VIU1_VSYNC),
	S32V_PINCTRL_PIN(S32V234_PAD_PF3_VIU1_D8),
	S32V_PINCTRL_PIN(S32V234_PAD_PF4_VIU1_D9),
	S32V_PINCTRL_PIN(S32V234_PAD_PF5_VIU1_D10),
	S32V_PINCTRL_PIN(S32V234_PAD_PF6_VIU1_D11),
	S32V_PINCTRL_PIN(S32V234_PAD_PF7_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF7_VIU1_D12),
	S32V_PINCTRL_PIN(S32V234_PAD_PF8_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF8_VIU1_D13),
	S32V_PINCTRL_PIN(S32V234_PAD_PF9_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF9_VIU1_D14),
	S32V_PINCTRL_PIN(S32V234_PAD_PF10_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF10_VIU1_D15),
	S32V_PINCTRL_PIN(S32V234_PAD_PF11_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF11_VIU1_D16),
	S32V_PINCTRL_PIN(S32V234_PAD_PF12_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF12_VIU1_D17),
	S32V_PINCTRL_PIN(S32V234_PAD_PF13_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF13_VIU1_D18),
	S32V_PINCTRL_PIN(S32V234_PAD_PF14_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF14_VIU1_D19),
	S32V_PINCTRL_PIN(S32V234_PAD_PF15_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PF15_VIU1_D20),
	S32V_PINCTRL_PIN(S32V234_PAD_PG0_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG0_VIU1_D21),
	S32V_PINCTRL_PIN(S32V234_PAD_PG1_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG1_VIU1_D22),
	S32V_PINCTRL_PIN(S32V234_PAD_PG2_VIU1_EN),
	S32V_PINCTRL_PIN(S32V234_PAD_PG2_VIU1_D23),
	S32V_PINCTRL_PIN(S32V234_PAD_PB6__SPI0_SOUT_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB7__SPI0_SIN_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB7__SPI0_SIN_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PB5__SPI0_SCK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB8__SPI0_CS0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB10__SPI1_SOUT_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB11__SPI1_SIN_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB11__SPI1_SIN_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PB9__SPI1_SCK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB12__SPI1_CS0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB14__SPI2_SOUT_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB15__SPI2_SIN_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PB15__SPI2_SIN_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_P13__SPI2_SCK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC0__SPI2_CS0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC2__SPI3_SOUT_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC3__SPI3_SIN_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC3__SPI3_SIN_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PC1__SPI3_SCK_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PC4__SPI3_CS0_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PA0__SIUL_GPIO0),
	S32V_PINCTRL_PIN(S32V234_PAD_PA1__SIUL_GPIO1),
	S32V_PINCTRL_PIN(S32V234_PAD_PA6__SIUL_GPIO6),
	S32V_PINCTRL_PIN(S32V234_PAD_PA7__SIUL_GPIO7),
	S32V_PINCTRL_PIN(S32V234_PAD_PA8__SIUL_GPIO8),
	S32V_PINCTRL_PIN(S32V234_PAD_PA9__SIUL_GPIO9),
	S32V_PINCTRL_PIN(S32V234_PAD_PA10__SIUL_GPIO10),
	S32V_PINCTRL_PIN(S32V234_PAD_PA15__SIUL_GPIO15),
	S32V_PINCTRL_PIN(S32V234_PAD_PB0__SIUL_GPIO16),
	S32V_PINCTRL_PIN(S32V234_PAD_PB1__SIUL_GPIO17),
	S32V_PINCTRL_PIN(S32V234_PAD_PB2__SIUL_GPIO18),
	S32V_PINCTRL_PIN(S32V234_PAD_PC5__SIUL_GPIO37),
	S32V_PINCTRL_PIN(S32V234_PAD_PC6__SIUL_GPIO38),
	S32V_PINCTRL_PIN(S32V234_PAD_PC7__SIUL_GPIO39),
	S32V_PINCTRL_PIN(S32V234_PAD_PC8__SIUL_GPIO40),
	S32V_PINCTRL_PIN(S32V234_PAD_PC9__SIUL_GPIO41),
	S32V_PINCTRL_PIN(S32V234_PAD_PC10__SIUL_GPIO42),
	S32V_PINCTRL_PIN(S32V234_PAD_PC11__SIUL_GPIO43),
	S32V_PINCTRL_PIN(S32V234_PAD_PC12__SIUL_GPIO44),
	S32V_PINCTRL_PIN(S32V234_PAD_PD6__SIUL_GPIO54),
	S32V_PINCTRL_PIN(S32V234_PAD_PD12__SIUL_GPIO60),
	S32V_PINCTRL_PIN(S32V234_PAD_PG7__SIUL_GPIO103),
	S32V_PINCTRL_PIN(S32V234_PAD_PG8__SIUL_GPIO104),
	S32V_PINCTRL_PIN(S32V234_PAD_PG9__SIUL_GPIO105),
	S32V_PINCTRL_PIN(S32V234_PAD_PG10__SIUL_GPIO106),
	S32V_PINCTRL_PIN(S32V234_PAD_PG11__SIUL_GPIO107),
	S32V_PINCTRL_PIN(S32V234_PAD_PG12__SIUL_GPIO108),
	S32V_PINCTRL_PIN(S32V234_PAD_PG13__SIUL_GPIO109),
	S32V_PINCTRL_PIN(S32V234_PAD_PG14__SIUL_GPIO110),
	S32V_PINCTRL_PIN(S32V234_PAD_PG15__SIUL_GPIO111),
	S32V_PINCTRL_PIN(S32V234_PAD_PH0__SIUL_GPIO112),
	S32V_PINCTRL_PIN(S32V234_PAD_PH1__SIUL_GPIO113),
	S32V_PINCTRL_PIN(S32V234_PAD_PH2__SIUL_GPIO114),
	S32V_PINCTRL_PIN(S32V234_PAD_PH3__SIUL_GPIO115),
	S32V_PINCTRL_PIN(S32V234_PAD_PH4__SIUL_GPIO116),
	S32V_PINCTRL_PIN(S32V234_PAD_PH5__SIUL_GPIO117),
	S32V_PINCTRL_PIN(S32V234_PAD_PH6__SIUL_GPIO118),
	S32V_PINCTRL_PIN(S32V234_PAD_PH7__SIUL_GPIO119),
	S32V_PINCTRL_PIN(S32V234_PAD_PH11__SIUL_GPIO123),
	S32V_PINCTRL_PIN(S32V234_PAD_PK5__SIUL_GPIO149),
	S32V_PINCTRL_PIN(S32V234_PAD_PK12__SIUL_GPIO156),
	S32V_PINCTRL_PIN(S32V234_PAD_PK13__SIUL_GPIO157),
	S32V_PINCTRL_PIN(S32V234_PAD_PK14__SIUL_GPIO158),
	S32V_PINCTRL_PIN(S32V234_PAD_PA0__SIUL_EIRQ0),
	S32V_PINCTRL_PIN(S32V234_PAD_PA1__SIUL_EIRQ1),
	S32V_PINCTRL_PIN(S32V234_PAD_PA2__SIUL_EIRQ2),
	S32V_PINCTRL_PIN(S32V234_PAD_PA3__SIUL_EIRQ3),
	S32V_PINCTRL_PIN(S32V234_PAD_PA4__SIUL_EIRQ4),
	S32V_PINCTRL_PIN(S32V234_PAD_PA5__SIUL_EIRQ5),
	S32V_PINCTRL_PIN(S32V234_PAD_PA6__SIUL_EIRQ6),
	S32V_PINCTRL_PIN(S32V234_PAD_PA7__SIUL_EIRQ7),
	S32V_PINCTRL_PIN(S32V234_PAD_PA8__SIUL_EIRQ8),
	S32V_PINCTRL_PIN(S32V234_PAD_PA9__SIUL_EIRQ9),
	S32V_PINCTRL_PIN(S32V234_PAD_PA10__SIUL_EIRQ10),
	S32V_PINCTRL_PIN(S32V234_PAD_PA11__SIUL_EIRQ11),
	S32V_PINCTRL_PIN(S32V234_PAD_PA12__SIUL_EIRQ12),
	S32V_PINCTRL_PIN(S32V234_PAD_PA13__SIUL_EIRQ13),
	S32V_PINCTRL_PIN(S32V234_PAD_PA14__SIUL_EIRQ14),
	S32V_PINCTRL_PIN(S32V234_PAD_PA15__SIUL_EIRQ15),
	S32V_PINCTRL_PIN(S32V234_PAD_PB0__SIUL_EIRQ16),
	S32V_PINCTRL_PIN(S32V234_PAD_PB1__SIUL_EIRQ17),
	S32V_PINCTRL_PIN(S32V234_PAD_PB2__SIUL_EIRQ18),
	S32V_PINCTRL_PIN(S32V234_PAD_PB3__SIUL_EIRQ19),
	S32V_PINCTRL_PIN(S32V234_PAD_PB4__SIUL_EIRQ20),
	S32V_PINCTRL_PIN(S32V234_PAD_PB5__SIUL_EIRQ21),
	S32V_PINCTRL_PIN(S32V234_PAD_PB6__SIUL_EIRQ22),
	S32V_PINCTRL_PIN(S32V234_PAD_PB7__SIUL_EIRQ23),
	S32V_PINCTRL_PIN(S32V234_PAD_PB8__SIUL_EIRQ24),
	S32V_PINCTRL_PIN(S32V234_PAD_PB9__SIUL_EIRQ25),
	S32V_PINCTRL_PIN(S32V234_PAD_PB10__SIUL_EIRQ26),
	S32V_PINCTRL_PIN(S32V234_PAD_PB11__SIUL_EIRQ27),
	S32V_PINCTRL_PIN(S32V234_PAD_PB12__SIUL_EIRQ28),
	S32V_PINCTRL_PIN(S32V234_PAD_PB13__SIUL_EIRQ29),
	S32V_PINCTRL_PIN(S32V234_PAD_PB14__SIUL_EIRQ30),
	S32V_PINCTRL_PIN(S32V234_PAD_PB15__SIUL_EIRQ31),
	S32V_PINCTRL_PIN(S32V234_PAD_PA2__CAN_FD0_TXD),
	S32V_PINCTRL_PIN(S32V234_PAD_PA3__CAN_FD0_RXD_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PA3__CAN_FD0_RXD_IN),
	S32V_PINCTRL_PIN(S32V234_PAD_PA4__CAN_FD1_TXD),
	S32V_PINCTRL_PIN(S32V234_PAD_PA5__CAN_FD1_RXD_OUT),
	S32V_PINCTRL_PIN(S32V234_PAD_PA5__CAN_FD1_RXD_IN),
};

static struct s32v_pinctrl_soc_info s32v234_pinctrl_info = {
	.pins = s32v234_pinctrl_pads,
	.npins = ARRAY_SIZE(s32v234_pinctrl_pads),
};


static struct of_device_id s32v234_pinctrl_of_match[] = {
	{
		.compatible = "fsl,s32v234-siul2",
		.data = (void *) PINCTRL_V1,
	},
	{
		.compatible = "fsl,s32v234-siul2-pinctrl",
		.data = (void *) PINCTRL_V2,
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, s32v234_pinctrl_of_match);

static int s32v234_pinctrl_probe(struct platform_device *pdev)
{
	enum s32v_pinctrl_version vers;
	const struct of_device_id *of_id =
		of_match_device(s32v234_pinctrl_of_match, &pdev->dev);

	vers = (enum s32v_pinctrl_version) of_id->data;

	return s32v_pinctrl_probe(pdev, &s32v234_pinctrl_info, vers);
}

static struct platform_driver s32v234_pinctrl_driver = {
	.driver = {
		.name = "s32v234-siul2-pinctrl",
		.owner = THIS_MODULE,
		.of_match_table = s32v234_pinctrl_of_match,
	},
	.probe = s32v234_pinctrl_probe,
	.remove = s32v_pinctrl_remove,
};

static int __init s32v234_pinctrl_init(void)
{
	return platform_driver_register(&s32v234_pinctrl_driver);
}
arch_initcall(s32v234_pinctrl_init);

static void __exit s32v234_pinctrl_exit(void)
{
	platform_driver_unregister(&s32v234_pinctrl_driver);
}
module_exit(s32v234_pinctrl_exit);

MODULE_DESCRIPTION("Freescale S32V234 pinctrl driver");
MODULE_LICENSE("GPL v2");
