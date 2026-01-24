#pragma once
#ifndef __SYS_MONITOR_H__
#define __SYS_MONITOR_H__

typedef enum{
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
}LogLevel;

typedef struct {
    int monitor_cpu;        // Whether to monitor CPU: 1=enable, 0=disable
    int monitor_mem;        // Whether to monitor memory: 1=enable, 0=disable
    int monitor_disk;       // Whether to monitor disk IO: 1=enable, 0=disable
    int monitor_load;       // Whether to monitor system load: 1=enable, 0=disable
    int monitor_proc;       // Whether to monitor specified process: 1=enable, 0=disable
    char disk_dev[32];      // Disk device name (e.g., sda, sdb)
    char proc_name[64];     // Target process name to monitor (e.g., log_agent)
    int interval;           // Data collection interval (seconds, minimum 1)
    int duration;           // Monitor running duration (seconds, -1=infinite running)
    LogLevel log_level;     // Log output level (control log detail)
    char log_path[256];
    int is_running;         // Monitor running flag (0=stop, 1=running) for graceful exit
} MonitorConfig;

typedef struct {
    float cpu_usage;        // Total CPU usage percentage (%)
    unsigned long mem_total;// Total physical memory (KB)
    unsigned long mem_used; // Used physical memory (KB)
    float mem_usage;        // Memory usage percentage (%)
    float disk_read_speed;  // Disk read speed (MB/s)
    float disk_write_speed; // Disk write speed (MB/s)
    float load1;            // 1-minute system load average
    float load5;            // 5-minute system load average
    float load15;           // 15-minute system load average
    int proc_pid;           // Target process PID (-1=process not found)
    float proc_cpu;         // Target process CPU usage (%)
    float proc_mem;         // Target process memory usage (%)
    char timestamp[32];     // Formatted timestamp: YYYY-MM-DD HH:MM:SS
} MonitorData;

#endif//__SYS_MONITOR_H__
