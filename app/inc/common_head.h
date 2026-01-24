#pragma once
#ifndef __COMMON_HEAD_H__
#define __COMMON_HEAD_H__

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#include "../../libmslog/inc/mslog.h"
#include "../../libmslog/inc/mslog_thread.h"

//type define
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum{
    FALSE = 0,
    TRUE
}Bool;

//marco define
#define SECTOR_SIZE         ( 512 )
#define MB_UNIT             ( 1024 )
#define MB_CONVERT          ( 1024 * 1024 )

#endif//__COMMON_HEAD_H__
