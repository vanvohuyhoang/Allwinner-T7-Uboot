/*
 * (C) Copyright 2013-2016
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
/*
**********************************************************************************************************************
*
*						           the Embedded Secure Bootloader System
*
*
*						       Copyright(C), 2006-2014, Allwinnertech Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include <common.h>
#include <asm/io.h>
#include <asm/arch/timer.h>
#include "asm/arch/rtc_region.h"

/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
uint rtc_region_probe_fel_flag(void)
{
	uint fel_flag, reg_value;
	int  i;

	fel_flag = readl(RTC_DATA_HOLD_REG_FEL);

	for(i=0;i<=5;i++)
	{
		reg_value = readl(RTC_DATA_HOLD_REG_BASE + i*4);
		if(reg_value)
			printf("rtc[%d] value = 0x%x\n", i, reg_value);
	}

	return fel_flag;
}
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
void rtc_region_clear_fel_flag(void)
{
	volatile uint flag = 0;
	do
	{
		writel(0, RTC_DATA_HOLD_REG_FEL);
		asm volatile("DSB");
		asm volatile("ISB");
		flag  = readl(RTC_DATA_HOLD_REG_FEL);
	}
	while(flag != 0);
}




