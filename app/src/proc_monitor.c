#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
static s32 get_pid_by_name(const s8 *proc_name, s32 *pid){
    DIR *dir;
    struct dirent *entry;
    s8 path[256], cmdline[256];
    FILE *fp;

    dir = opendir("/proc");
    if ( !dir )
    {
        MSLOG_ERROR(MSLOG_ERROR,"Failed to open /proc directory: %m");
        return -1;
    }
    
    *pid = -1;
    while ( ( entry = readdir(dir) ) != NULL )
    {
        if ( !isdigit(entry->d_name[0]) )
        {
            continue;
        }
        
        snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
        fp = fopen(path, "r");
        if ( !fp )
        {
            continue;
        }
        
        if ( fgets(cmdline, sizeof(cmdline), fp) )
        {
            if ( strstr(cmdline,proc_name) != NULL )
            {
                *pid = atoi(entry->d_name);
                fclose(fp);
                closedir(dir);
                return 0;
            }
        }
        fclose(fp);
    }
    
    closedir(dir);
    MSLOG_WARN(MSLOG_WARN, "Process %s not found in system", proc_name);
    return -1;
}

static s32 read_proc_cpu(int pid, float *cpu_usage){
    static u64 prev_utime = 0, prev_stime =0, prev_total = 0;
    s8 path[256];
    FILE *fp;
    s8 line[256];
    u64 utime, stime, total_cpu;
    s32 i;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    fp = fopen(path, "r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/%d/stat: %m", pid);
        return -1;
    }
    
    if ( fgets(line, sizeof(line), fp) )
    {
        sscanf(line, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu",
               &utime, &stime);
    }
    fclose(fp);

    FILE *cpu_fp = fopen("/proc/stat", "r");
    if ( !cpu_fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/stat: %m");
        return -1;
    }
    
    if ( fgets(line, sizeof(line), cpu_fp) )
    {
        total_cpu = 0;
        for ( i = 0; i < 10; i++ )
        {
            u64 val;
            if ( i == 0 )
            {
                sscanf(line, "cpu %llu", &val);
            }
            else
            {
                sscanf(line, "%*s %llu", &val);
            }
            total_cpu += val;
        }
    }
    fclose(cpu_fp);

    if ( prev_utime == 0 || prev_stime == 0 || prev_total == 0 )
    {
        prev_utime = utime;
        prev_stime = stime;
        prev_total = total_cpu;
        *cpu_usage = 0.0f;
        return 0;
    }
    
    u64 proc_diff = ( utime + stime ) - ( prev_utime + prev_stime );
    u64 total_diff = total_cpu - prev_total;

    if ( total_diff == 0 )
    {
        *cpu_usage = 0.0f;
    }
    else
    {
        *cpu_usage = (float)proc_diff / total_diff * 100;
    }
    
    prev_utime = utime;
    prev_stime = stime;
    prev_total = total_cpu;

    return 0;
}

static s32 read_proc_mem(s32 pid, float *mem_usage){
    s8 path[256];
    FILE *fp;
    s8 line[256];
    u32 vm_rss = 0, mem_total = 0;

    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/%d/statm: %m", pid);
        return -1;
    }
    
    if ( fgets(line, sizeof(line), fp) )
    {
        sscanf(line, "%*lu %lu", &vm_rss);
    }
    fclose(fp);

    fp = fopen("/proc/meminfo", "r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/meminfo: %m");
        return -1;
    }
    
    while ( fgets(line, sizeof(line), fp) )
    {
        if ( !strncmp(line, "MemTotal:", 8) )
        {
            sscanf(line, "MemTotal: %lu kB", &mem_total);
            break;
        }
    }
    fclose(fp);

    s32 page_size = sysconf(_SC_PAGE_SIZE) / MB_UNIT;
    u32 rss_kb = vm_rss * page_size;
    
    *mem_usage = (float)rss_kb / mem_total * 100;
    return 0;
}
#ifdef __cplusplus
}
#endif

s32 collect_proc_data(const s8 *proc_name, MonitorData *data){
    s32 pid;

    if ( get_pid_by_name(proc_name, &pid) != 0 )
    {
        data->proc_pid = -1;
        data->proc_cpu = 0.0f;
        data->proc_mem = 0.0f;
        return -1;
    }
    
    data->proc_pid = pid;

    if ( read_proc_cpu(pid, &data->proc_cpu) != 0 )
    {
        data->proc_cpu = 0.0f;
    }
    
    if ( read_proc_mem(pid, &data->proc_mem) != 0 )
    {
        data->proc_mem = 0.0f;
    }
    return 0;
}
