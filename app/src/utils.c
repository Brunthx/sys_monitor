#include "../inc/app_lib.h"

void generate_timestamp(s8 *timestamp, s16 len){
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(timestamp, len, "%Y-%m-%d %H:%M:%S", tm);
}

void clear_screen(){
    printf("\033[H\033[2J");
    fflush(stdout);
}

s16 init_mslog(MonitorConfig *config){
    if ( config == NULL )
    {
        fprintf(stderr, "[sys_monitor] init mslog failed: config is NULL\n");
        return -1;
    }
    
    const s8 *log_path = ( config->log_path[0] != '\0' ) ? config->log_path : MSLOG_DEFAULT_LOG_PATH;

    mslog_level_t log_level = MSLOG_INFO;
    switch ( config->log_level )
    {
    case LOG_DEBUG:
        log_level = MSLOG_DEBUG;
        break;
    case LOG_INFO:
        log_level = MSLOG_INFO;
        break;
    case LOG_WARN:
        log_level = MSLOG_WARN;
        break;
    case LOG_ERROR:
        log_level = MSLOG_ERROR;
        break;
    case LOG_FATAL:
        log_level = MSLOG_FATAL;
        break;
    default:
        fprintf(stderr, "[sys_monitor] invalid log level: %d, use INFO instead\n", config->log_level);
        log_level = LOG_INFO;
        break;
    }
    
    s16 ret = mslog_init_default(
        log_path, 
        log_level, 
        MSLOG_DEFAULT_MAX_FILE_SIZE, 
        MSLOG_DEFAULT_MAX_FILE_COUNT,
        MSLOG_FLUSH_BATCH
    );
    if ( ret != 0 )
    {
        MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__,
            "Failed to initialize mslog, log path: %s, error: %s", 
            log_path, strerror(errno));
        return -1;
    }

    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__,
        "mslog initialized successfully, log level: %d, output path: %s",
        config->log_level,
        log_path);
    return 0;
}
