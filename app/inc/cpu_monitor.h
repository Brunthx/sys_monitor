#pragma once
#ifndef __CPU_MONITOR_H__
#define __CPU_MONITOR_H__

#include "common_head.h"

#ifdef __cplusplus
extern "C"{
#endif
// Read raw CPU time data from /proc/stat
// Parameters:
//   total: total CPU time (user+nice+system+idle+...)
//   idle: CPU idle time
// Return 0 on success, -1 on failure
s16 read_cpu_raw_data(u64 *total, u64 *idle);
#ifdef __cplusplus
}
#endif
#endif//__CPU_MONITOR_H__
