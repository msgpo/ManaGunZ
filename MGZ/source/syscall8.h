/* syscall8.h

Copyright (c) 2010 Hermes <www.elotrolado.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are 
permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of 
  conditions and the following disclaimer. 
- Redistributions in binary form must reproduce the above copyright notice, this list 
  of conditions and the following disclaimer in the documentation and/or other 
  materials provided with the distribution. 
- The names of the contributors may not be used to endorse or promote products derived 
  from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/



#ifndef _SYS8_H_
#define _SYS8_H_

#include <stdint.h>
#include <stddef.h>
#include <ppu-lv2.h>


#ifdef __cplusplus
extern "C" {
#endif

#define SYSCALL8_OPCODE_GET_MAMBA						0x7FFF
#define SYSCALL8_OPCODE_GET_VERSION						0x7000
#define SYSCALL8_OPCODE_GET_VERSION2					0x7001
		
#define SYSCALL8_OPCODE_DISABLE_COBRA					0x0000
#define SYSCALL8_OPCODE_ENABLE_COBRA					0x0001

#define SYSCALL8_DISABLE_COBRA_CAPABILITY				0x0002
#define SYSCALL8_DISABLE_COBRA_STATUS					0x0003
#define SYSCALL8_DISABLE_COBRA_OK						0x5555

#define SYSCALL8_OPCODE_POKE_LV2						0x7003
#define SYSCALL8_OPCODE_IS_HEN							0x1337
#define SYSCALL8_OPCODE_HEN_REV							0x1339
#define SYSCALL8_OPCODE_GET_MAP_PATH					0x7967

#define SYSCALL8_OPCODE_GET_DISC_TYPE					0x7020
#define SYSCALL8_OPCODE_READ_PS3_DISC					0x7021
#define SYSCALL8_OPCODE_FAKE_STORAGE_EVENT				0x7022
#define SYSCALL8_OPCODE_GET_EMU_STATE					0x7023
#define SYSCALL8_OPCODE_MOUNT_PS3_DISCFILE				0x7024
#define SYSCALL8_OPCODE_MOUNT_DVD_DISCFILE				0x7025
#define SYSCALL8_OPCODE_MOUNT_BD_DISCFILE				0x7026
#define SYSCALL8_OPCODE_MOUNT_PSX_DISCFILE				0x7027
#define SYSCALL8_OPCODE_MOUNT_PS2_DISCFILE				0x7028
#define SYSCALL8_OPCODE_MOUNT_DISCFILE_PROXY			0x6808
#define SYSCALL8_OPCODE_UMOUNT_DISCFILE					0x702C
#define SYSCALL8_OPCODE_MOUNT_ENCRYPTED_IMAGE			0x702D

#define SYSCALL8_OPCODE_GET_ACCESS						0x8097
#define SYSCALL8_OPCODE_REMOVE_ACCESS					0x8654

#define SYSCALL8_OPCODE_READ_COBRA_CONFIG				0x7050
#define SYSCALL8_OPCODE_WRITE_COBRA_CONFIG				0x7051

#define SYSCALL8_OPCODE_SET_PSP_UMDFILE					0x9003
#define SYSCALL8_OPCODE_SET_PSP_DECRYPT_OPTIONS			0x9002
#define SYSCALL8_OPCODE_READ_PSP_HEADER					0x1028
#define SYSCALL8_OPCODE_READ_PSP_UMD					0x1029
#define SYSCALL8_OPCODE_PSP_PRX_PATCH					0x204C 
#define SYSCALL8_OPCODE_PSP_POST_SAVEDATA_INITSTART		0x3008
#define SYSCALL8_OPCODE_PSP_POST_SAVEDATA_SHUTDOWNSTART	0x3009
#define SYSCALL8_OPCODE_PSP_CHANGE_EMU					0x9752

#define SYSCALL8_OPCODE_COBRA_USB_COMMAND				0x7714

#define SYSCALL8_OPCODE_AIO_COPY_ROOT					0x6637
#define SYSCALL8_OPCODE_MAP_PATHS						0x7964

#define SYSCALL8_OPCODE_VSH_SPOOF_VERSION				0x2C0F

#define SYSCALL8_OPCODE_LOAD_VSH_PLUGIN					0x1EE7
#define SYSCALL8_OPCODE_UNLOAD_VSH_PLUGIN				0x364F

#define SYSCALL8_OPCODE_USE_PS2NETEMU					0x1EE9

#define SYSCALL8_OPCODE_DRM_GET_DATA					0x6A11

#define SYSCALL8_OPCODE_SEND_POWEROFF_EVENT				0x6CDD

#define SYSCALL8_OPCODE_RUN_PAYLOAD						0x6CDF
#define SYSCALL8_OPCODE_RUN_PAYLOAD_DYNAMIC				0x6CE1
#define SYSCALL8_OPCODE_UNLOAD_PAYLOAD_DYNAMIC			0x6CE3
#define SYSCALL8_OPCODE_DISABLE_COBRA_STAGE				0x6A13

#ifdef DEBUG
// These debug opcode changed to odd numbers in version 7.0 to minmize crashes with lv1 peek apps
#define SYSCALL8_OPCODE_DUMP_STACK_TRACE				0x5003
#define SYSCALL8_OPCODE_GENERIC_DEBUG					0x5005
#define SYSCALL8_OPCODE_PSP_SONY_BUG					0x5007
#endif

extern int sys8_disable_all; // disable all sys8 functions

/* 

FUNCTIONS TO USE IN KERNEL AREA 0x8000000000000000 - 0x80000000007fffff

payload V3 is loaded at 0x80000000007ff000

*/

// lv1 registers struct
typedef struct {

    uint64_t reg3;
    uint64_t reg4;
    uint64_t reg5;
    uint64_t reg6;
    uint64_t reg7;
    uint64_t reg8;
    uint64_t reg9;
    uint64_t reg10;
    uint64_t reg11;

} lv1_reg;


// hook_open
typedef struct
{
	uint64_t compare_addr; // kernel address to compare string
	uint64_t replace_addr; // kernel address to replace string
	int compare_len;       // len of compare string
	int replace_len;       // len of replace string

} path_open_entry;


/* NOTES about path_open_entry:

compare_addr can be a string full path for files or not. The size of the string can be len+1
replacea_ddr can replace a file, files or directories. The size of the string must be replace_string + hook_open string remnant+1 
(recommended 0x800 to work with directories)

// example 1: replacing a file

compare_addr: "/app_home/PS3_GAME/USRDIR/EBOOT.BIN" 
replace_addr: "/dev_usb000/PS3_GAME/USRDIR/EBOOT.BIN" 

compare_len= strlen(compare_addr);
replace_len= strlen(replace_addr);

// example 2: replacing files by a unique file

compare_addr: "/app_home/PS3_GAME/ICON" 
replace_addr: "/dev_usb000/PS3_GAME/ICON0.PNG" 

compare_len= strlen(compare_addr);
replace_len= strlen(replace_addr)+1;

// example 3: replacing directories

compare_addr: "/app_home/PS3_GAME" 
replace_addr: "/dev_usb000/PS3_GAME" 

compare_len= strlen(compare_addr);
replace_len= strlen(replace_addr);

*/

/*
	sys8_disable: disable syscalls 6,7, 36 and stealth syscall 8

	key: 64 bits key to enable syscalls
*/

int sys8_disable(uint64_t key); // don't supported yet

/*
	sys8_enable: enable syscalls 6,7, 36 and 8 when 

	key: 64 bits key to enable syscalls or 0 to test

	return: 0x80010003 (<0) or current version (0x100)

*/

int sys8_enable(uint64_t key); // don't supported yet

/*
	sys8_pokeinstr: 64 bits poke for instructions (with dcbf and icibi instructions)

	addr: destination addr

	data: 64 bits data to set

	return: error code

*/

int sys8_pokeinstr(uint64_t addr, uint64_t data);

/*
	sys8_memcpy: 64 bits address memory copy

	dst: destination addr

	src: source addr

	size: number of bytes to copy

	return: destination addr

*/

uint64_t sys8_memcpy(uint64_t dst, uint64_t src, uint64_t size);

/*
    note: this functions uses dcbf and icibi instructions

	sys8_memcpyinstr: 64 bits address memory copy instructions

	dst: destination addr for instructions

	src: source addr for instructions

	size: number of bytes to copy

	return: destination addr

*/

uint64_t sys8_memcpyinstr(uint64_t dst, uint64_t src, uint64_t size);

/*
	sys8_memset: 64 bits address memory set

	dst: destination addr

	val: value to set (only the 8 bits lower)

	size: number of bytes to set

	return: destination addr

*/

uint64_t sys8_memset(uint64_t dst, uint64_t val, uint64_t size);

/*
	sys8_call: kernel 64 bits address to call

	param1: first 64 bits param (maybe IN datas addr)

	param2: second 64 bits param (maybe OUT datas addr)

	return: value returned by kernel call

*/


uint64_t  sys8_call(uint64_t addr, uint64_t param1, uint64_t param2);

/*
	sys8_alloc: kernel function to alloc memory

	size: bytes to alloc

	pool: Usually 0x27 from psjailbreak

	return: 64 bits addr allocated

*/

uint64_t  sys8_alloc(uint64_t size, uint64_t pool);

/*
	sys8_free: kernel function to free memory

	addr: 64 bits addr to free

	pool: Usually 0x27 from psjailbreak

	return: unknown 64 bits addr

*/

uint64_t  sys8_free(uint64_t addr, uint64_t pool);

/*
	sys8_panic: kernel panic function

*/

void sys8_panic(void); // don't supported yet

/*
	sys8_perm_mode: function to changes as work 0x80000000000505d0 calls (connected with access permissions)

	mode: 0 -> PS3 perms normally, 1-> Psjailbreak by default, 2-> Special for games as F1 2010 (option by default)

	return: 0 - Ok

*/

int sys8_perm_mode(uint64_t mode);

/*
	sys8_sys_configure: function to change the XMB or releases devices redirected

	mode: 0 -> XMB retail, 1-> XMB debug, 2-> free /apps_home and /dev_usb redirections and unpatch APP_VER in PARAM.SFO
          3 -> unpatch APP_VER in PARAM.SFO 4 -> patch APP_VER in PARAM.SFO

	return: 0 - Ok

*/

#define CFG_XMB_RETAIL          0
#define CFG_XMB_DEBUG           1
#define CFG_UNMOUNT_DEVICES     2
#define CFG_UNPATCH_APPVER      3
#define CFG_PATCH_APPVER        4



int sys8_sys_configure(uint64_t mode); // don't supported yet

/*
	sys8_lv1_syscall: call to lv1 syscall from lv2

	in:  input lv1_reg struct registers

    out:  output lv1_reg struct registers

	return:  lv1 syscall return

*/

int sys8_lv1_syscall(lv1_reg *in, lv1_reg *out);

/*
	sys8_path_table: function to add one table for path redirections in hook_open

	addr_table: kernel 64 bits addr from one path_open_entry array, starts. The last path_open_entry.compare_addr must be 0 to stop

	return: last addr_table

*/


uint64_t sys8_path_table(uint64_t addr_table);


/*
    sys8_mamba: return the mark of 'Mamba'

    return: 0x666 -> is 'mamba', other wise -> is not 'mamba'
*/

int sys8_mamba(void);


/*

Developers notes:

To test if syscall8 is working:

	int ret= sys8_enable(0);

	if(ret<0) 
		{
		//try to put the correct key

		ret= sys8_enable(key);
		}

	if(ret<0) printf("Sorry, syscall8 unavailable\n");
	else printf("Current version %x\n", ret);

------------------------------------------------------------

To Test sys8_alloc(), sys8_free(), sys8_memcpy() and sys8_call():

	uint32_t my_fun[0x10/4]={0x38601234, 0x4e800020}; // li %r3, 0x1234, blr

	..................................................

	uint64_t memo= sys8_alloc(0x10ULL, 0x27ULL); // alloc 16 bytes

	printf("ret: %llx\n", sys8_call( memo , 0ULL, 0ULL));

	sys8_free( memo  , 0x27ULL)

------------------------------------------------------------

To test path redirections:

	// struct for 3 paths

	typedef struct
	{
		path_open_entry entries[4];

		char arena[0x2000];
	} path_open_table;

	path_open_table open_table;

	uint64_t dest_table_addr;

	..................................................

	// disable table

	printf(" last table %llx\n", sys8_path_table(0ULL));

	// calculate dest_table addr from payload start to back

	dest_table_addr= 0x80000000007FF000ULL-((sizeof(path_open_table)+15) & ~15);

	// fix the start addresses

	open_table.entries[0].compare_addr= ((uint64_t) &open_table.arena[0]) - ((uint64_t) &open_table) + dest_table_addr;
	open_table.entries[0].replace_addr= ((uint64_t) &open_table.arena[0x800])- ((uint64_t) &open_table) + dest_table_addr;

	open_table.entries[1].compare_addr= ((uint64_t) &open_table.arena[0x100]) - ((uint64_t) &open_table) + dest_table_addr;
	open_table.entries[1].replace_addr= ((uint64_t) &open_table.arena[0x1000])- ((uint64_t) &open_table) + dest_table_addr;

	open_table.entries[2].compare_addr= ((uint64_t) &open_table.arena[0x200]) - ((uint64_t) &open_table) + dest_table_addr;
	open_table.entries[2].replace_addr= ((uint64_t) &open_table.arena[0x1800])- ((uint64_t) &open_table) + dest_table_addr;

	open_table.entries[3].compare_addr= 0ULL; // the last entry always 0

	// copy the paths

	strncpy(&open_table.arena[0], "/app_home/PS3_GAME/USRDIR", 0x100);           // compare 1
	strncpy(&open_table.arena[0x800], "/dev_usb000/PS3_GAME/USRDIR", 0x800);     // replace 1: replaces all content or USRDIR

	strncpy(&open_table.arena[0x100], "/app_home/PS3_GAME/ICON0.PNG", 0x100);	 // compare 2
	strncpy(&open_table.arena[0x1000], "/dev_usb000/PS3_GAME/ICON0.PNG", 0x800); // replace 2: replace only ICON0.PNG

	strncpy(&open_table.arena[0x200], "/app_home/PS3_GAME/ICO", 0x100);          // compare 3
	strncpy(&open_table.arena[0x1800], "/dev_usb000/PS3_GAME/ICON0.PNG", 0x800); // replace 3: // replace all ICONxxxx by ICON0.PNG


    // fix the string len

	open_table.entries[0].compare_len= strlen(&open_table.arena[0]);		// 1
	open_table.entries[0].replace_len= strlen(&open_table.arena[0x800]);
	
	open_table.entries[1].compare_len= strlen(&open_table.arena[0x100]);	// 2
	open_table.entries[1].replace_len= strlen(&open_table.arena[0x1000]);

	open_table.entries[2].compare_len= strlen(&open_table.arena[0x200]);    // 3
	open_table.entries[2].replace_len= strlen(&open_table.arena[0x1800]) +1; // truncate the name because it skip  '\0'

	// copy the datas to  the destination address

	sys8_memcpy(dest_table_addr, (uint64_t) &open_table, sizeof(path_open_table));

	// set the path table
	sys8_path_table( dest_table_addr);


*/


#ifdef __cplusplus
  }
#endif

#endif
