#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
// Collect memory usage data from /proc/meminfo
// Update data to MonitorData structure
// Return 0 on success, -1 on failure
s16 collect_mem_data(MonitorData *data);
#ifdef __cplusplus
}
#endif

s16 collect_mem_data(MonitorData *data){
    FILE *fp = fopen("/proc/meminfo", "r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/meminfo: %m");
        return -1;
    }
    s8 line[256];
    u32 mem_total = 0, mem_free = 0, buffers = 0, cached = 0, s_reclaimable = 0;
    while ( fgets(line, sizeof(line), fp) )
    {
        if ( !strncmp(line, "MemTotal:", 8) ) 
        {
            sscanf(line, "MemTotal: %lu kB", &mem_total);
        } 
        else if ( !strncmp(line, "MemFree:", 7) ) 
        {
            sscanf(line, "MemFree: %lu kB", &mem_free);
        } 
        else if ( !strncmp(line, "Buffers:", 7) ) 
        {
            sscanf(line, "Buffers: %lu kB", &buffers);
        } 
        else if ( !strncmp(line, "Cached:", 6) ) 
        {
            sscanf(line, "Cached: %lu kB", &cached);
        } 
        else if ( !strncmp(line, "SReclaimable:", 12) ) 
        {
            sscanf(line, "SReclaimable: %lu kB", &s_reclaimable);
        }
    }
    fclose(fp);

    data->mem_total = mem_total;
    data->mem_used = mem_total - mem_free - buffers - cached - s_reclaimable;

    if ( mem_total == 0 )
    {
        data->mem_usage = 0.0f;
    }
    else
    {
        data->mem_usage = (float)data->mem_used / mem_total *100;
    }
    return 0;
}
