/*
 * arch/arm/mach-tegra/board-tf101.h
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MACH_TEGRA_BOARD_TF101_H
#define _MACH_TEGRA_BOARD_TF101_H

extern uint8_t tf101_hw;

int tf101_charge_init(void);
int tf101_regulator_init(void);
int tf101_sdhci_init(void);
int tf101_pinmux_init(void);
int tf101_panel_init(void);
int tf101_sensors_init(void);
int tf101_emc_init(void);
int tf101_charger_init(void);

/* external gpios */

/* TPS6586X gpios */
#define TPS6586X_GPIO_BASE	TEGRA_NR_GPIOS
#define AVDD_DSI_CSI_ENB_GPIO	(TPS6586X_GPIO_BASE + 1) /* gpio2 */

/* TCA6416 gpios */
#define TCA6416_GPIO_BASE	(TEGRA_NR_GPIOS + 4)
#define CAM1_PWR_DN_GPIO	(TCA6416_GPIO_BASE + 0) /* gpio0 */
#define CAM1_RST_L_GPIO		(TCA6416_GPIO_BASE + 1) /* gpio1 */
#define CAM1_AF_PWR_DN_L_GPIO	(TCA6416_GPIO_BASE + 2) /* gpio2 */
#define CAM1_LDO_SHUTDN_L_GPIO	(TCA6416_GPIO_BASE + 3) /* gpio3 */
#define CAM2_PWR_DN_GPIO	(TCA6416_GPIO_BASE + 4) /* gpio4 */
#define CAM2_RST_L_GPIO		(TCA6416_GPIO_BASE + 5) /* gpio5 */
#define CAM2_AF_PWR_DN_L_GPIO	(TCA6416_GPIO_BASE + 6) /* gpio6 */
#define CAM2_LDO_SHUTDN_L_GPIO	(TCA6416_GPIO_BASE + 7) /* gpio7 */
#define CAM3_PWR_DN_GPIO	(TCA6416_GPIO_BASE + 8) /* gpio8 */
#define CAM3_RST_L_GPIO		(TCA6416_GPIO_BASE + 9) /* gpio9 */
#define CAM3_AF_PWR_DN_L_GPIO	(TCA6416_GPIO_BASE + 10) /* gpio10 */
#define CAM3_LDO_SHUTDN_L_GPIO	(TCA6416_GPIO_BASE + 11) /* gpio11 */
#define CAM_I2C_MUX_RST_GPIO	(TCA6416_GPIO_BASE + 15) /* gpio15 */
#define TCA6416_GPIO_END	(TCA6416_GPIO_BASE + 31)

/* WM8903 GPIOs */
#define TF101_GPIO_WM8903(_x_)	(TEGRA_NR_GPIOS + 32 + (_x_))
#define TF101_GPIO_WM8903_END		TF101_GPIO_WM8903(4)

/* Audio-related GPIOs */
#define TEGRA_GPIO_CDC_IRQ		TEGRA_GPIO_PX3
#define TEGRA_GPIO_SPKR_EN		TF101_GPIO_WM8903(2)
#define TEGRA_GPIO_HP_DET		TEGRA_GPIO_PW2
#define TEGRA_GPIO_INT_MIC_EN		TEGRA_GPIO_PX0
#define TEGRA_GPIO_EXT_MIC_EN		TEGRA_GPIO_PX1

/* AC detect GPIO */
#define AC_PRESENT_GPIO			TEGRA_GPIO_PV3

/* Interrupt numbers from external peripherals */
#define TPS6586X_INT_BASE	TEGRA_NR_IRQS
#define TPS6586X_INT_END	(TPS6586X_INT_BASE + 32)

/* Invensense MPU Definitions */
#define MPU_GYRO_NAME		"mpu3050"
#define MPU_ACCEL_NAME		"kxtf9"
#define MPU_ACCEL_ADDR		0x0f
#define MPU_COMPASS_NAME	"ami306"
#define MPU_COMPASS_ADDR	0x0e

/*
 * The HW_FIELD_* macros are helper macros for the public HW_DRF_* macros.
 */
#define HW_FIELD_LOWBIT(x)      (0?x)
#define HW_FIELD_HIGHBIT(x)     (1?x)
#define HW_FIELD_SIZE(x)        (HW_FIELD_HIGHBIT(x)-HW_FIELD_LOWBIT(x)+1)
#define HW_FIELD_SHIFT(x)       ((0?x)%32)
#define HW_FIELD_MASK(x)        (0xFFFFFFFFUL>>(31-((1?x)%32)+((0?x)%32)))
#define HW_FIELD_BITS(val, x)   (((val) & HW_FIELD_MASK(x))<<HW_FIELD_SHIFT(x))
#define HW_FIELD_SHIFTMASK(x)   (HW_FIELD_MASK(x)<< (HW_FIELD_SHIFT(x)))

/** HW_DRF_VAL - read a field from a register.

    @param d register domain (hardware block)
    @param r register name
    @param f register field
    @param v register value
 */
#define HW_DRF_VAL(d,r,f,v) \
    (((v)>> HW_FIELD_SHIFT(d##_##r##_0_##f##_RANGE)) & \
        HW_FIELD_MASK(d##_##r##_0_##f##_RANGE))

/** HW_DRF_SIZE - read a occupied length from a register.

    @param d register domain (hardware block)
    @param r register name
    @param f register field
 */
#define HW_DRF_SIZE(d,r,f) \
    (HW_FIELD_SIZE(d##_##r##_0_##f##_RANGE))

//Memory Type
#define TEGRA_DEVKIT_MISC_HW_0_MEMTYPE_RANGE	1:0
#define TEGRA_DEVKIT_MISC_HW_0_MEMTYPE_DEFAULT	0x0UL //EPD 8Gb
#define TEGRA_DEVKIT_MISC_HW_0_MEMTYPE_1	0x1UL //EPD 4Gb
#define TEGRA_DEVKIT_MISC_HW_0_MEMTYPE_2	0x2UL //HYN 8Gb
#define TEGRA_DEVKIT_MISC_HW_0_MEMTYPE_3	0x3UL //HYN 4Gb

//MMC Type
#define TEGRA_DEVKIT_MISC_HW_0_MMCTYPE_RANGE	3:2
#define TEGRA_DEVKIT_MISC_HW_0_MMCTYPE_DEFAULT	0x0UL //TOS/KIN 16G
#define TEGRA_DEVKIT_MISC_HW_0_MMCTYPE_1	0x1UL //TBD
#define TEGRA_DEVKIT_MISC_HW_0_MMCTYPE_2	0x2UL //TOS/KIN 32G
#define TEGRA_DEVKIT_MISC_HW_0_MMCTYPE_3     	0x3UL //HYN 64G

//SKU Identification
#define TEGRA_DEVKIT_MISC_HW_0_SKU_RANGE        4:4
#define TEGRA_DEVKIT_MISC_HW_0_SKU_DEFAULT      0x0UL //W/O 3G
#define TEGRA_DEVKIT_MISC_HW_0_SKU_1            0x1UL //3G

//BT/WLAN Module Vendor
#define TEGRA_DEVKIT_MISC_HW_0_VENDOR_RANGE     5:5
#define TEGRA_DEVKIT_MISC_HW_0_VENDOR_DEFAULT   0x0UL //Murata BT/WLAN
#define TEGRA_DEVKIT_MISC_HW_0_VENDOR_1         0x1UL //AZW BT/WLAN

//Touch Panel Vendor
#define TEGRA_DEVKIT_MISC_HW_0_TOUCH_RANGE      6:6
#define TEGRA_DEVKIT_MISC_HW_0_TOUCH_DEFAULT    0x0UL //SINTEK
#define TEGRA_DEVKIT_MISC_HW_0_TOUCH_1          0x1UL //WINTEK

//Project Identification
#define TEGRA_DEVKIT_MISC_HW_0_PROJECT_RANGE    7:7
#define TEGRA_DEVKIT_MISC_HW_0_PROJECT_DEFAULT  0x0UL //TF101(EP101)
#define TEGRA_DEVKIT_MISC_HW_0_PROJECT_1        0x1UL //SL101(EP102)

//WiMAX Identification
#define TEGRA_DEVKIT_MISC_HW_0_WIMAX_RANGE      8:8
#define TEGRA_DEVKIT_MISC_HW_0_WIMAX_DEFAULT    0x0UL //W/O WiMAX
#define TEGRA_DEVKIT_MISC_HW_0_WIMAX_1          0x1UL //WiMAX


//The byte-field definition of Tegra2 ReservedOdm fuse is defined as follows
//31    26       20                        0
//+------+--------+------------------------+
//|  BT  |  WiFi  |         Reserved       |
//+------+--------+------------------------+

//Tegra2 ReservedOdm fuse
#define TEGRA_DEVKIT_MISC_HW_0_RSV_RANGE 31:0

//BT Mac address
#define TEGRA_DEVKIT_MISC_HW_0_BT_RANGE 31:26

//WiFi Mac address
#define TEGRA_DEVKIT_MISC_HW_0_WIFI_RANGE 25:20

#define VENTANA_PCBID_OUTPUT_LENGTH 7

/* Acquire project identification
 *   @ret unsigned int
 *      Project identification will be returned. (eg, TF101(EP101) <-> 101)
 *      Otherwise, 0 will be returned instead.
 */
unsigned int ASUSGetProjectID(void);

/* Detect if 3G is equipped
 *   @ret unsigned int
 *      If 3G is equipped, 1 will be returned; Otherwise, 0 will be instead.
 */
unsigned int ASUS3GAvailable(void);

/* Detect if WiMAX is equipped
 *   @ret unsigned int
 *      If WiMAX is equipped, 1 will be returned; Otherwise, 0 will be instead.
 */
unsigned int ASUSWiMAXAvailable(void);


#define BT_WLAN_VENDOR_AZW          TEGRA_DEVKIT_MISC_HW_0_VENDOR_DEFAULT
#define BT_WLAN_VENDOR_MURATA       TEGRA_DEVKIT_MISC_HW_0_VENDOR_1

/* Check if BT/WLAN module vendor is equipped
 *   @param v module name
 *   @ret unsigned int
 *      If specified wireless module is equipped, 1 will be returned;
 *      Otherwise, 0 will be instead.
 */
unsigned int ASUSCheckWLANVendor(unsigned int vendor);


#define TOUCH_VENDOR_SINTEK		TEGRA_DEVKIT_MISC_HW_0_TOUCH_DEFAULT
#define TOUCH_VENDOR_WINTEK		TEGRA_DEVKIT_MISC_HW_0_TOUCH_1
/* Check if Touch module vendor is equipped
 *   @param v module name
 *   @ret unsigned int
 *      If specified touch panel vendor is equipped, 1 will be returned;
 *      Otherwise, 0 will be instead.
 */
unsigned int ASUSCheckTouchVendor(unsigned int vendor);

/* Acquire project name
 *      No return parameter
 *      Project name will be assigned according to different kinds devices. (TF101, TF101G, TF101-WIMAX, and SL101)
 */
static void ASUSGetProjectName(void);

/* Acquire pcb id
 *      No return parameter
 *      This function will perform the assignment for ventana_pcbid in string format
 *      Format of ventana_pcbid:
 *      =>TF101:x00xx0b
 *      =>TF101G:x00xx1b
 *      =>TF101-WiMax:x10xx0b
 *      =>SL101:x01xx0b
 */
static void ASUSGetPcbid(void);

/* ASUS uses the following to store the lcd pixel clock freq. Then decide the vaule by project name.*/
extern u32 lcd_pclk_khz;

#endif
