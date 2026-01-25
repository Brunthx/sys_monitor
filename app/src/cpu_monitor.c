#include "../inc/app_lib.h"

s16 read_cpu_raw_data(u64 *total, u64 *idle){
    FILE *fp = fopen("/proc/stat","r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/stat: %m");
        return -1;
    }
    s8 line[256];
    u64 user, nice, sys, idle_, iowait, irq, softirq;
    if ( fgets(line, sizeof(line), fp) )
    {
        sscanf(line, "cpu %llu %llu %llu %llu %llu %llu %llu",
                &user, &nice, &sys, &idle_, &iowait, &irq, &softirq);
        *total = user + nice + sys +idle_ + iowait + irq + softirq;
        *idle = idle_;
    }
    fclose(fp);
    return 0;
}

s16 collect_cpu_data(MonitorData *data){
    static u64 prev_total = 0, prev_idle = 0;
    u64 curr_total, curr_idle;

    if ( read_cpu_raw_data(&curr_total, &curr_idle) != 0 )
    {
        data->cpu_usage = -1.0f;
        return -1;
    }
    
    if ( prev_total == 0 || prev_idle == 0 )
    {
        prev_total = curr_total;
        prev_idle = curr_idle;
        data->cpu_usage = 0.0f;
        return 0;
    }
    
    u64 total_diff = curr_total - prev_total;
    u64 idle_diff = curr_idle - prev_idle;
    if ( total_diff == 0 )
    {
        data->cpu_usage = 0.0f;
    }
    else
    {
        data->cpu_usage = ( 1.0f - (float)idle_diff / total_diff ) * 100;
    }
    
    prev_total = curr_total;
    prev_idle = curr_idle;
    return 0;
}
