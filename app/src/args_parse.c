#include "../inc/app_lib.h"

// Print help information for command line arguments
void print_help() {
    printf("==================== sys_monitor Usage Help ====================\n");
    printf("Argument Description:\n");
    printf("  -c/--cpu        Monitor CPU (enabled by default)\n");
    printf("  -m/--mem        Monitor memory (enabled by default)\n");
    printf("  -d/--disk <dev> Monitor disk IO (e.g., sda, disabled by default)\n");
    printf("  -l/--load       Monitor system load (enabled by default)\n");
    printf("  -i/--interval <sec> Collection interval (seconds, default 1)\n");
    printf("  -t/--duration <sec> Monitor duration (seconds, -1=infinite, default -1)\n");
    printf("  -p/--proc <name> Monitor specified process (e.g., log_agent, disabled by default)\n");
    printf("  -L/--log-level <level> Log level (debug/info/warn/error, default info)\n");
    printf("  -f/--log-file <path> Log file path (console output by default)\n");
    printf("  -h/--help       Print help information\n");
    printf("Examples:\n");
    printf("  ./sys_monitor -d sda -i 2 -t 60 -p log_agent -L debug -f /var/log/sys_mon.log\n");
}

// Initialize monitor configuration with default values
void init_default_config(MonitorConfig *config) {
    if ( config == NULL )
    {
        MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, "init_default_config failed: config is NULL");
        return;
    }

    memset(config, 0, sizeof(MonitorConfig));
    // Default enable CPU and memory monitoring
    config->monitor_cpu = TRUE;
    config->monitor_mem = TRUE;
    // Default disable disk, process monitoring
    config->monitor_disk = FALSE;
    config->monitor_load = TRUE;
    config->monitor_proc = FALSE;
    // Empty default values for disk device and process name
    config->disk_dev[0] = '\0';
    config->proc_name[0] = '\0';
    // Default collection interval: 1 second
    config->interval = 1;
    // Default duration: infinite (-1)
    config->duration = -1;
    // Default log level: INFO
    config->log_level = LOG_INFO;
    // Default log output: console (empty path)
    config->log_path[0] = '\0';
    // Default running state: enable
    config->is_running = TRUE;

    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, "default config init success");
}

// Parse command line arguments and update monitor configuration
s16 parse_args(s16 argc, s8 *argv[], MonitorConfig *config) {
    if ( argc < 1 || argv == NULL || config == NULL )
    {
        MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, "parse_args failed: invalid input param");
        return -1;
    }
    

    s16 opt;
    // Short command line options definition
    const s8 *short_opts = "cmdl:i:t:p:L:f:h";
    // Long command line options definition (for better usability)
    struct option long_opts[] = {
        {"cpu", no_argument, NULL, 'c'},
        {"mem", no_argument, NULL, 'm'},
        {"disk", required_argument, NULL, 'd'},
        {"load", no_argument, NULL, 'l'},
        {"interval", required_argument, NULL, 'i'},
        {"duration", required_argument, NULL, 't'},
        {"proc", required_argument, NULL, 'p'},
        {"log-level", required_argument, NULL, 'L'},
        {"log-file", required_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    while ( ( opt = getopt_long(argc, argv, short_opts, long_opts, NULL) ) != -1 ) 
    {
        switch (opt) 
        {
            case 'c': 
                config->monitor_cpu = TRUE; 
                break;
            case 'm': 
                config->monitor_mem = TRUE; 
                break;
            case 'd': // Set disk device for IO monitoring
                config->monitor_disk = TRUE;
                snprintf(config->disk_dev, sizeof(config->disk_dev) - 1, "%s", optarg);
                config->disk_dev[sizeof(config->disk_dev) - 1] = '\0';
                MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                    "disk monitor enabled, dev: %s", config->disk_dev);
                break;
            case 'l': 
                config->monitor_load = TRUE; 
                break;
            case 'i': // Set collection interval (seconds)
                config->interval = atoi(optarg);
                if ( config->interval < 1 ) 
                {
                    MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                        "invalid interval: %d (must >=1)", config->interval);
                    return -1;
                }
                break;
            case 't': // Set monitor duration (seconds)
                config->duration = atoi(optarg);
                if ( config->duration < 1 ) 
                {
                    MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                        "invalid duration: %d (must >=1 or -1)", config->duration);
                    return -1;
                }
                break;
            case 'p': // Set target process name to monitor
                config->monitor_proc = TRUE;
                snprintf(config->proc_name, sizeof(config->proc_name) - 1, "%s", optarg);
                config->proc_name[sizeof(config->proc_name) - 1] = '\0';
                MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                    "process monitor enabled, name: %s", config->proc_name);
                break;
            case 'L': // Set log level (debug/info/warn/error)
                if ( !strcmp(optarg, "debug") ) 
                {
                    config->log_level = LOG_DEBUG;
                }
                else if ( !strcmp(optarg, "info") ) 
                {
                    config->log_level = LOG_INFO;
                }
                else if ( !strcmp(optarg, "warn") ) 
                {
                    config->log_level = LOG_WARN;
                }
                else if ( !strcmp(optarg, "error") ) 
                {
                    config->log_level = LOG_ERROR;
                }
                else if ( !strcmp(optarg, "fatal") )
                {
                    config->log_level = LOG_FATAL;
                }
                else 
                {
                    MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                        "invalid log level: %s (support debug/info/warn/error/fatal)", optarg);
                    return -1;
                }
                MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                    "log level set to: %s", optarg);
                break;
            case 'f': // Set log file output path
                snprintf(config->log_path, sizeof(config->log_path) - 1, "%s", optarg);
                config->log_path[sizeof(config->log_path) - 1] = '\0';
                MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
                    "log file path set to: %s", config->log_path);
                break;
            case 'h': // Print help information and exit
                print_help();
                exit(0);
            default: // Invalid argument
                MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, "invalid argument: %c", opt);
                print_help();
                return -1;
        }
    }

    if ( config->interval < 1 )
    {
        MSLOG_ERROR(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
            "interval must be >=1 (current: %d)", config->interval);
        return -1;
    }
    
    MSLOG_INFO(SYS_MON_TAG, __FILE__, __LINE__, __func__, 
        "args parse success | interval: %ds | duration: %ds | log-level: %d | log-path: %s", 
        config->interval, config->duration, config->log_level, config->log_path);
    return 0;
}
