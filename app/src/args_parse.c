#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
void init_default_config(MonitorConfig *config);
// Parse command line arguments and update monitor configuration
// Return 0 on success, -1 on invalid arguments
int parse_args(int argc, char *argv[], MonitorConfig *config);
#ifdef __cplusplus
}
#endif

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
    memset(config, 0, sizeof(MonitorConfig));
    // Default enable CPU and memory monitoring
    config->monitor_cpu = 1;
    config->monitor_mem = 1;
    // Default disable disk, process monitoring
    config->monitor_disk = 0;
    config->monitor_load = 1;
    config->monitor_proc = 0;
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
    config->is_running = 1;
}

// Parse command line arguments and update monitor configuration
int parse_args(int argc, char *argv[], MonitorConfig *config) {
    int opt;
    // Short command line options definition
    const char *short_opts = "cmdl:i:t:p:l:f:h";
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
                config->monitor_cpu = 1; 
                break;
            case 'm': 
                config->monitor_mem = 1; 
                break;
            case 'd': // Set disk device for IO monitoring
                config->monitor_disk = 1;
                snprintf(config->disk_dev, sizeof(config->disk_dev), "%s", optarg);
                break;
            case 'l': 
                config->monitor_load = 1; 
                break;
            case 'i': // Set collection interval (seconds)
                config->interval = atoi(optarg);
                if ( config->interval < 1 ) 
                {
                    MSLOG_ERROR(MSLOG_ERROR,"Collection interval cannot be less than 1 second");
                    return -1;
                }
                break;
            case 't': // Set monitor duration (seconds)
                config->duration = atoi(optarg);
                if ( config->duration < 1 ) 
                {
                    MSLOG_ERROR(MSLOG_ERROR,"Monitor duration cannot be less than 1 second");
                    return -1;
                }
                break;
            case 'p': // Set target process name to monitor
                config->monitor_proc = 1;
                snprintf(config->proc_name, sizeof(config->proc_name), "%s", optarg);
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
                else 
                {
                    MSLOG_ERROR(MSLOG_ERROR,"Invalid log level: %s, support debug/info/warn/error", optarg);
                    return -1;
                }
                break;
            case 'f': // Set log file output path
                snprintf(config->log_path, sizeof(config->log_path), "%s", optarg);
                break;
            case 'h': // Print help information and exit
                print_help();
                exit(0);
            default: // Invalid argument
                MSLOG_ERROR(MSLOG_ERROR,"Invalid argument: %c", opt);
                print_help();
                return -1;
        }
    }
    return 0;
}
