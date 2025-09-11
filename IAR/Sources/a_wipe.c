#include "a_wipe.h"
#include "sma.h"
#include "stdlib.h"

static s_sma_flt rain_long_time_average[2];
static s_sma_flt rain_short_time_average[2];
static int rain_long_time_average_val[2];
static int rain_short_time_average_val[2];
static int rain_long_time_average_val_difference[2];
static int rain_immediately_average_val_difference[2];

void a_wipe_init(void)
{
    float initial_val = (float)IR_Channel_A_data[0];
    SMA_InitFlt(&rain_long_time_average[0], initial_val, (5 * 60 * 10)); // 5 minutes
    SMA_InitFlt(&rain_short_time_average[0], initial_val, (5 * 10));     // 5 sec
    initial_val = (float)IR_Channel_B_data[0];
    SMA_InitFlt(&rain_long_time_average[1], initial_val, (5 * 60 * 10)); // 5 minutes
    SMA_InitFlt(&rain_short_time_average[1], initial_val, (5 * 10));     // 5 sec
}
void a_wipe_poll_100ms(void)
{
    float v[2];
    v[0] = (float)IR_Channel_A_data[0];
    v[1] = (float)IR_Channel_B_data[0];

    rain_short_time_average_val[0] = (int)SMA_GetFlt(&rain_short_time_average[0], v[0], 1);
    rain_short_time_average_val[1] = (int)SMA_GetFlt(&rain_short_time_average[1], v[1], 1);
    if ((rain_long_time_average_val[0] == 0) || (rain_long_time_average_val[1] == 0))
    {
        rain_long_time_average_val[0] = (int)SMA_GetFlt(&rain_long_time_average[0], v[0], 1);
        rain_long_time_average_val[1] = (int)SMA_GetFlt(&rain_long_time_average[1], v[1], 1);
    }

    rain_long_time_average_val_difference[0] = rain_long_time_average_val[0] - rain_short_time_average_val[0];
    rain_long_time_average_val_difference[1] = rain_long_time_average_val[1] - rain_short_time_average_val[1];

    rain_immediately_average_val_difference[0] = rain_long_time_average_val[0] - (int)IR_Channel_A_data[0];
    rain_immediately_average_val_difference[1] = rain_long_time_average_val[1] - (int)IR_Channel_B_data[0];

    int avg_counts[2] = {0, 0};
    if (rain_immediately_average_val_difference[0] < 800)
    {
        avg_counts[0]++;
    }
    if (rain_immediately_average_val_difference[1] < 800)
    {
        avg_counts[1]++;
    }
    if (rain_immediately_average_val_difference[0] < 0)
    {
        avg_counts[0]++;
    }
    if (rain_immediately_average_val_difference[1] < 0)
    {
        avg_counts[1]++;
    }
    rain_long_time_average_val[0] = (int)SMA_GetFlt(&rain_long_time_average[0], v[0], avg_counts[0]);
    rain_long_time_average_val[1] = (int)SMA_GetFlt(&rain_long_time_average[1], v[1], avg_counts[1]);
}