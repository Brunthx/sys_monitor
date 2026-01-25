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

#ifdef __cplusplus
extern "C"{
#endif
// Collect disk IO data for specified device
// Parameters:
//   disk_dev: target disk device name (e.g., sda)
//   data: monitor data structure to store result
// Return 0 on success, -1 on failure
s16 collect_disk_data(const s8 *disk_dev, MonitorData *data);
#ifdef __cplusplus
}
#endif

#endif//__DISK_MONITOR_H__
