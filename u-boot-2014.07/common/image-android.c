/*
 * Copyright (c) 2011 Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <android_image.h>
#include <malloc.h>
#include <errno.h>


static char andr_tmp_str[ANDR_BOOT_ARGS_SIZE + 1];


/**
 * android_image_get_kernel() - processes kernel part of Android boot images
 * @hdr:	Pointer to image header, which is at the start
 *			of the image.
 * @verify:	Checksum verification flag. Currently unimplemented.
 * @os_data:	Pointer to a ulong variable, will hold os data start
 *			address.
 * @os_len:	Pointer to a ulong variable, will hold os data length.
 *
 * This function returns the os image's start address and length. Also,
 * it appends the kernel command line to the bootargs env variable.
 *
 * Return: Zero, os start address and length on success,
 *		otherwise on failure.
 */
int android_image_get_kernel(const struct andr_img_hdr *hdr, int verify,
			     ulong *os_data, ulong *os_len)
{

	/*
	 * Not all Android tools use the id field for signing the image with
	 * sha1 (or anything) so we don't check it. It is not obvious that the
	 * string is null terminated so we take care of this.
	 */
	strncpy(andr_tmp_str, hdr->name, ANDR_BOOT_NAME_SIZE);
	andr_tmp_str[ANDR_BOOT_NAME_SIZE] = '\0';
	if (strlen(andr_tmp_str))
		printf("Android's image name: %s\n", andr_tmp_str);

	printf("Kernel load addr 0x%08x size %u KiB\n",
	       hdr->kernel_addr, DIV_ROUND_UP(hdr->kernel_size, 1024));
	strncpy(andr_tmp_str, hdr->cmdline, ANDR_BOOT_ARGS_SIZE);
	andr_tmp_str[ANDR_BOOT_ARGS_SIZE] = '\0';
	if (strlen(andr_tmp_str)) {
		int len = strlen(andr_tmp_str);
		char *bootargs = getenv("bootargs");
		char *newbootargs = NULL;
		printf("Kernel command line: %s\n", andr_tmp_str);
		if(bootargs != NULL)
			len += strlen(bootargs);
		newbootargs = malloc(len + 2);
		if (!newbootargs){
			printf("%s: malloc fail\n",__func__);
			return -ENOMEM;
		}
		memset(newbootargs,0, len+2);

		if (bootargs) {
			strcpy(newbootargs, bootargs);
			strcat(newbootargs, " ");
		}
		strcat(newbootargs, andr_tmp_str);
		setenv("bootargs", newbootargs);
	}

	if (hdr->ramdisk_size)
		printf("RAM disk load addr 0x%08x size %u KiB\n",
		       hdr->ramdisk_addr,
		       DIV_ROUND_UP(hdr->ramdisk_size, 1024));

	if (os_data) {
		*os_data = (ulong)hdr;
		*os_data += hdr->page_size;
	}
	if (os_len)
		*os_len = hdr->kernel_size;
	return 0;
}

int android_image_check_header(const struct andr_img_hdr *hdr)
{
	return memcmp(ANDR_BOOT_MAGIC, hdr->magic, ANDR_BOOT_MAGIC_SIZE);
}

ulong android_image_get_end(const struct andr_img_hdr *hdr)
{
	u32 size = 0;
	/*
	 * The header takes a full page, the remaining components are aligned
	 * on page boundary
	 */
	size += hdr->page_size;
	size += ALIGN(hdr->kernel_size, hdr->page_size);
	size += ALIGN(hdr->ramdisk_size, hdr->page_size);
	size += ALIGN(hdr->second_size, hdr->page_size);

	return size;
}

ulong android_image_get_kload(const struct andr_img_hdr *hdr)
{
	return hdr->kernel_addr;
}

int android_image_get_ramdisk(const struct andr_img_hdr *hdr,
			      ulong *rd_data, ulong *rd_len)
{
	if (!hdr->ramdisk_size)
		return -1;
	*rd_data = (unsigned long)hdr;
	*rd_data += hdr->page_size;
	*rd_data += ALIGN(hdr->kernel_size, hdr->page_size);

	*rd_len = hdr->ramdisk_size;
	return 0;
}
