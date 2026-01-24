#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
// Collect system load average data from /proc/loadavg
// Update 1/5/15 minute load to MonitorData
// Return 0 on success, -1 on failure
s16 collect_load_data(MonitorData *data);
#ifdef __cplusplus
}
#endif

s16 collect_load_data(MonitorData *data){
    FILE *fp = fopen("/proc/loadavg", "r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/loadavg: %m");
        return -1;
    }
    
    s8 line[256];
    if ( fgets(line, sizeof(line), fp) )
    {
        // Parse 1/5/15 minute load average
        sscanf(line, "%f %f %f", &data->load1, &data->load5, &data->load15);
    }
    
    fclose(fp);
    return 0;
}
