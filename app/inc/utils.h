#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef __cplusplus
extern "C"{
#endif
extern MonitorConfig *config;
// Generate formatted timestamp (YYYY-MM-DD HH:MM:SS)
// Parameters:
//   timestamp: buffer to store timestamp string
//   len: buffer length (must be >= 20)
void generate_timestamp(s8 *timestamp, s16 len);

// Clear console screen (use ANSI escape sequence)
void clear_screen();

s16 init_mslog(MonitorConfig *config);
#ifdef __cplusplus
}
#endif

#endif//__UTILS_H__
