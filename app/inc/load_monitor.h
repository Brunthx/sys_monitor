#pragma once
#ifndef __LOAD_MONITOR_H__
#define __LOAD_MONITOR_H__

#ifdef __cplusplus
extern "C"{
#endif
extern MonitorData *data;
// Collect system load average data from /proc/loadavg
// Update 1/5/15 minute load to MonitorData
// Return 0 on success, -1 on failure
s16 collect_load_data(MonitorData *data);
#ifdef __cplusplus
}
#endif

#endif//__LOAD_MONITOR_H__
