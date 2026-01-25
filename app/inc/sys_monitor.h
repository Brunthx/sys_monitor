#pragma once
#ifndef __SYS_MONITOR_H__
#define __SYS_MONITOR_H__

#include"common_head.h"
typedef enum{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
}LogLevel;

typedef struct {
    Bool monitor_cpu;        // Whether to monitor CPU: 1=enable, 0=disable
    Bool monitor_mem;        // Whether to monitor memory: 1=enable, 0=disable
    Bool monitor_disk;       // Whether to monitor disk IO: 1=enable, 0=disable
    Bool monitor_load;       // Whether to monitor system load: 1=enable, 0=disable
    Bool monitor_proc;       // Whether to monitor specified process: 1=enable, 0=disable
    s8 disk_dev[32];      // Disk device name (e.g., sda, sdb)
    s8 proc_name[64];     // Target process name to monitor (e.g., log_agent)
    s16 interval;           // Data collection interval (seconds, minimum 1)
    s16 duration;           // Monitor running duration (seconds, -1=infinite running)
    LogLevel log_level;     // Log output level (control log detail)
    s8 log_path[256];
    Bool is_running;         // Monitor running flag (0=stop, 1=running) for graceful exit
} MonitorConfig;

typedef struct {
    float cpu_usage;        // Total CPU usage percentage (%)
    u32 mem_total;// Total physical memory (KB)
    u32 mem_used; // Used physical memory (KB)
    float mem_usage;        // Memory usage percentage (%)
    float disk_read_speed;  // Disk read speed (MB/s)
    float disk_write_speed; // Disk write speed (MB/s)
    float load1;            // 1-minute system load average
    float load5;            // 5-minute system load average
    float load15;           // 15-minute system load average
    s16 proc_pid;           // Target process PID (-1=process not found)
    float proc_cpu;         // Target process CPU usage (%)
    float proc_mem;         // Target process memory usage (%)
    s8 timestamp[32];     // Formatted timestamp: YYYY-MM-DD HH:MM:SS
} MonitorData;

#endif//__SYS_MONITOR_H__
