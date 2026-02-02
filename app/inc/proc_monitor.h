#pragma once
#ifndef __PROC_MONITOR_H__
#define __PROC_MONITOR_H__

#ifdef __cplusplus
extern "C"{
#endif
extern MonitorData *data;
// Collect resource usage data of specified process
// Parameters:
//   proc_name: target process name (e.g., log_agent)
//   data: monitor data structure to store PID/CPU/MEM usage
// Return 0 on success, -1 on failure (process not found)
s32 collect_proc_data(const s8 *proc_name, MonitorData *data);
#ifdef __cplusplus
}
#endif

#endif//__PROC_MONITOR_H__
