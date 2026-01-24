#pragma once
#ifndef __DISK_MONITOR_H__
#define __DISK_MONITOR_H__

#include "common_head.h"

// Disk IO raw data structure (store sector/operation count)
typedef struct {
    u64 read_sectors;  // Total read sectors from disk
    u64 write_sectors; // Total write sectors to disk
    u64 read_ops;      // Total read operations
    u64 write_ops;     // Total write operations
} DiskRawData;

#endif//__DISK_MONITOR_H__
