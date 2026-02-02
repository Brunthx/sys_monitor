#include "app_lib.h"

MonitorConfig g_config;

#ifdef __cplusplus
extern "C"{
#endif
void sig_handler(s16 sig){
    MSLOG_WARN("SYS_MONITOR", __FILE__, __LINE__, __func__, 
        "Received exit signal: %d, starting graceful exit...", sig);
    g_config.is_running = FALSE;
}

s16 collect_all_data(MonitorConfig *config, MonitorData *data){
    generate_timestamp(data->timestamp, sizeof(data->timestamp));

    if ( config->monitor_cpu )
    {
        collect_cpu_data(data);
    }

    if ( config->monitor_mem )
    {
        collect_mem_data(data);
    }

    if ( config->monitor_disk )
    {
        collect_disk_data(config->disk_dev, data);
    }

    if ( config->monitor_load )
    {
        collect_load_data(data);
    }
    
    if ( config->monitor_proc )
    {
        collect_proc_data(config->proc_name, data);
    }/* code */
    return 0;
}

void output_data(MonitorConfig *config, MonitorData *data){
    clear_screen();

    printf("==================== Linux System Resource Monitor ====================\n");
    printf("Collection Time: %s\n", data->timestamp);

    if ( config->monitor_cpu )
    {
        printf("CPU Usage: %.2f%%\n", data->cpu_usage);
    }

    if ( config->monitor_mem )
    {
        printf("Memory: %luKB Total | %luKB Used | %.2f%% Usage\n", 
            data->mem_total, data->mem_used, data->mem_usage);
    }

    if ( config->monitor_disk )
    {
        printf("Disk IO: Read=%.2fMB/s | Write=%.2fMB/s\n", 
            data->disk_read_speed, data->disk_write_speed);
    }

    if ( config->monitor_load )
    {
        printf("System Load: 1min=%.2f | 5min=%.2f | 15min=%.2f\n", 
            data->load1, data->load5, data->load15);
    }
    
    if ( config->monitor_proc )
    {
        printf("Process<%s>: PID=%d | CPU=%.2f%% | Memory=%.2f%%\n", 
            config->proc_name, data->proc_pid, data->proc_cpu, data->proc_mem);
    }
    printf("======================================================================\n");

    s8 log_buf[1024];
    snprintf(log_buf, sizeof(log_buf), 
        "timestamp=%s cpu_usage=%.2f%% mem_total=%luKB mem_used=%luKB mem_usage=%.2f%% "
        "disk_read=%.2fMB/s disk_write=%.2fMB/s load1=%.2f load5=%.2f load15=%.2f "
        "proc_name=%s proc_pid=%d proc_cpu=%.2f%% proc_mem=%.2f%%",
        data->timestamp,
        data->cpu_usage,
        data->mem_total, data->mem_used, data->mem_usage,
        data->disk_read_speed, data->disk_write_speed,
        data->load1, data->load5, data->load15,
        config->proc_name, data->proc_pid, data->proc_cpu, data->proc_mem);

    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, "%s", log_buf);
}

s16 main(int argc, char const *argv[])
{
    init_default_config(&g_config);

    if ( parse_args(argc, argv, &g_config) != 0 )
    {
        exit(1);
    }
    
    if ( init_mslog(&g_config) != 0 )
    {
        exit(1);
    }
    
    signal(SIGINT, sig_handler);//Ctrl+C
    signal(SIGTERM, sig_handler);//Kill cmd

    MonitorData data;

    s16 count = 0;
    s16 total_count = ( g_config.duration > 0 ) ? ( g_config.duration / g_config.interval ) : -1;

    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
        "System resource monitor started, collection interval: %ds, monitor duration: %s",
        g_config.interval,
        g_config.duration > 0 ? "specified duration" : "infinite");

    while ( g_config.is_running )
    {
        memset(&data, 0, sizeof(data));
        collect_all_data(&g_config, &data);
        output_data(&g_config, &data);
        mslog_keep_alive();
        count++;

        if ( total_count > 0 && count >= total_count )
        {
            MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                "Monitor duration reached, total collections: %d, exiting monitor", count);
            break;
        }
        sleep(g_config.interval);
    }
    
    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
        "System resource monitor exited, cleaning up resources...");
    mslog_deinit();
    return 0;
}

#ifdef __cplusplus
}
#endif