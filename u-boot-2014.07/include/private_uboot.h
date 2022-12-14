/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef  __private_uboot_h__
#define  __private_uboot_h__

#include "spare_head.h"

#ifndef CONFIG_SUNXI_DEBUG_BUF
#define CONFIG_SUNXI_DEBUG_BUF
#ifndef CONFIG_SUNXI_DEBUG_BUF_SIZE
#define CONFIG_SUNXI_DEBUG_BUF_SIZE   (1 * 1024 * 1024)
#endif
#endif
/******************************************************************************/
/*               the control information stored in file head                  */
/******************************************************************************/
struct spare_boot_ctrl_head
{
	unsigned int  jump_instruction;   // one intruction jumping to real code
	unsigned char magic[MAGIC_SIZE];  // ="u-boot"
	unsigned int  check_sum;          // generated by PC
	unsigned int  align_size;		  // align size in byte
	unsigned int  length;             // the size of all file
	unsigned int  uboot_length;       // the size of uboot
	unsigned char version[8];         // uboot version
	unsigned char platform[8];        // platform information
	int           reserved[1];        //stamp space, 16bytes align
};

/******************************************************************************/
/*                          the data stored in file head                      */
/******************************************************************************/
struct spare_boot_data_head
{
	unsigned int                dram_para[32];
	int                         run_clock;              // Mhz
	int                         run_core_vol;           // mV
	int                         uart_port;              // UART ctrl num
	normal_gpio_cfg             uart_gpio[2];           // UART GPIO info
	int                         twi_port;               // TWI ctrl num
	normal_gpio_cfg             twi_gpio[2];            // TWI GPIO info
	int                         work_mode;              // boot,usb-burn, card-burn
	int                         storage_type;           // 0:nand 1:sdcard 2:spinor
	normal_gpio_cfg             nand_gpio[32];          // nand GPIO info
	char                        nand_spare_data[256];	// nand info
	normal_gpio_cfg             sdcard_gpio[32];		// sdcard GPIO info
	char                        sdcard_spare_data[256];	// sdcard info
	unsigned char               secureos_exist;
	unsigned char               monitor_exist;
	unsigned char               res[2];
	uint                        uboot_start_sector_in_mmc;  //use in OTA update
	int                         dtb_offset;                 //device tree in uboot
	int                         boot_package_size;          //boot package size, boot0 pass this value
	uint                        dram_scan_size;             //dram real size
	int                         reserved[1];                //reseved,256bytes align

};

/*******************************************
*
*
*   boot_ext[0]: pmu type
*   boot_ext[1]: uart input value
*   boot_ext[2]: lradc key input value
*   boot_ext[3]: debug mode, boot0 send it
*
*******************************************/
struct spare_boot_ext_head
{
	int data[4];
};

struct spare_boot_head_t
{
	struct spare_boot_ctrl_head    boot_head;
	struct spare_boot_data_head    boot_data;
	struct spare_boot_ext_head     boot_ext[16];
};

struct spare_monitor_head
{
	unsigned int  jump_instruction;   // one intruction jumping to real code
	unsigned char magic[MAGIC_SIZE];  // ="u-boot"
	unsigned int  scp_base;           // generated by PC
	unsigned int  nboot_base;         // align size in byte
	unsigned int  nos_base;           // the size of all file
	unsigned int  secureos_base;      // the size of uboot
	unsigned char version[8];         // uboot version
	unsigned char platform[8];        // platform information
	int           reserved[1];         //stamp space, 16bytes align
};

extern struct spare_boot_head_t  uboot_spare_head;
extern unsigned long get_spare_head_size(void);
#endif

