/*
 * ES plugin for Custom IOS.
 *
 * Copyright (C) 2010 Waninkoko.
 * Copyright (C) 2011 davebaol.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "isfs.h"
#include "syscalls.h"
#include "types.h"

/* Constants */
#define FILENAME	"/sys/esconfig.cfg"


s32 __Config_Create(void)
{
	s32 ret;

	/* Open ISFS */
	ret = ISFS_Open();
	if (ret < 0)
		return ret;

	/* Create file */
	ret = ISFS_CreateFile(FILENAME);

	/* Close ISFS */
	ISFS_Close();

	return ret;
}

s32 __Config_Delete(void)
{
	s32 ret;

	/* Open ISFS */
	ret = ISFS_Open();
	if (ret < 0)
		return ret;

	/* Delete file */
	ret = ISFS_Delete(FILENAME);

	/* Close ISFS */
	ISFS_Close();

	return ret;
}

// NOTE:
// This function is called by the main that is 
// out of the patched es life cycle. 
// So for debugging purpose don't use ES_printf here,
// use write instead.
s32 Config_Load(void *cfg, u32 size)
{
	s32 fd, ret;

	//write("ESP: Loading config file "FILENAME"\n");

	/* Open config file */
	fd = os_open(FILENAME, ISFS_OPEN_READ);
	//write("ESP: Config file ");write(fd<0? "found\n": "NOT found\n");
	if (fd < 0)
		return fd;

	/* Read config */
	ret = os_read(fd, cfg, size);

	/* Close config */
	os_close(fd);

	/* Delete config file */
	__Config_Delete();

	return ret;
}

s32 Config_Save(void *cfg, u32 size)
{
	s32 fd, ret;

	/* Create config file */
	__Config_Create();

	/* Open config file */
	fd = os_open(FILENAME, ISFS_OPEN_WRITE);
	if (fd < 0)
		return fd;

	/* Write config */
	ret = os_write(fd, cfg, size);

	/* Close config */
	os_close(fd);

	return ret;
}                   