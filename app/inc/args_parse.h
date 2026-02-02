#pragma once
#ifndef __ARGS_PARSE_H__
#define __ARGS_PARSE_H__

#ifdef __cplusplus
extern "C"{
#endif
extern MonitorConfig *config;
void init_default_config(MonitorConfig *config);
// Parse command line arguments and update monitor configuration
// Return 0 on success, -1 on invalid arguments
s32 parse_args(s32 argc, s8 *argv[], MonitorConfig *config);
void print_help();
#ifdef __cplusplus
}
#endif
#endif//__ARGS_PARSE_H__
