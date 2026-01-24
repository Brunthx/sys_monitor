#include "../inc/app_lib.h"

#ifdef __cplusplus
extern "C"{
#endif
// Collect disk IO data for specified device
// Parameters:
//   disk_dev: target disk device name (e.g., sda)
//   data: monitor data structure to store result
// Return 0 on success, -1 on failure
s16 collect_disk_data(const s8 *disk_dev, MonitorData *data);

static s16 read_disk_raw_data(const s8 *disk_dev, DiskRawData * data){
    FILE *fp = fopen("/proc/diskstats","r");
    if ( !fp )
    {
        MSLOG_ERROR(MSLOG_ERROR, "Failed to open /proc/diskstats: %m");
        return -1;
    }
    
    s8 line[512];
    u16 major, minor;
    s8 dev_name[32];
    u64 r_ops, r_merged, r_sectors, r_ms, w_ops, w_merged, w_sectors, w_ms;

    while ( fgets(line, sizeof(line), fp) )
    {
        sscanf(line, "%u %u %s %llu %llu %llu %llu %llu %llu %llu %llu",
               &major, &minor, dev_name, &r_ops, &r_merged, &r_sectors, &r_ms,
               &w_ops, &w_merged, &w_sectors, &w_ms);

        if ( strcmp(dev_name, disk_dev) == 0 )
        {
            data->read_sectors = r_sectors;
            data->write_sectors = w_sectors;
            data->read_ops = r_ops;
            data->write_ops = w_ops;
            fclose(fp);
            return 0;
        }
    }
    
    fclose(fp);
    MSLOG_ERROR(MSLOG_ERROR, "Disk device %s not found in /proc/diskstats", disk_dev);
    return -1;
}
#ifdef __cplusplus
}
#endif

s16 collect_disk_data(const s8 *disk_dev, MonitorData *data){
    static DiskRawData prev_data = {0};
    DiskRawData curr_data = {0};
    static Bool first_collect = TRUE;

    if ( read_disk_raw_data(disk_dev, &curr_data) != 0 )
    {
        data->disk_read_speed = -1.0f;
        data->disk_write_speed = -1.0f;
        return -1;
    }
    
    if ( first_collect )
    {
        prev_data = curr_data;
        first_collect = FALSE;
        data->disk_read_speed = 0.0f;
        data->disk_write_speed = 0.0f;
        return 0;
    }
    
    u64 r_sectors_diff = curr_data.read_sectors - prev_data.read_sectors;
    u64 w_sectors_diff = curr_data.write_sectors - prev_data.write_sectors;

    data->disk_read_speed = (float)( r_sectors_diff * SECTOR_SIZE ) / MB_CONVERT;
    data->disk_write_speed = (float)( w_sectors_diff * SECTOR_SIZE ) / MB_CONVERT;

    prev_data = curr_data;
    return 0;
}
