#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
// Generate formatted timestamp (YYYY-MM-DD HH:MM:SS)
// Parameters:
//   timestamp: buffer to store timestamp string
//   len: buffer length (must be >= 20)
void generate_timestamp(s8 *timestamp, s16 len);

// Clear console screen (use ANSI escape sequence)
void clear_screen();
#ifdef __cplusplus
}
#endif

void generate_timestamp(s8 *timestamp, s16 len){
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(timestamp, len, "%Y-%m-%d %H:%M:%S", tm);
}

void clear_screen(){
    printf("\033[H\033[2J");
    fflush(stdout);
}
