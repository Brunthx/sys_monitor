#pragma once
#ifndef __MEM_MONITOR_H__
#define __MEM_MONITOR_H__

#ifdef __cplusplus
extern "C"{
#endif
extern MonitorData *data;
// Collect memory usage data from /proc/meminfo
// Update data to MonitorData structure
// Return 0 on success, -1 on failure
s16 collect_mem_data(MonitorData *data);
#ifdef __cplusplus
}
#endif

#endif//__MEM_MONITOR_H__
