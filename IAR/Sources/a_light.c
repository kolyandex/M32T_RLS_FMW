#include "a_light.h"
#include "sma.h"

unsigned char TurnOnLights = 0;

static s_sma_flt light_long_time_average;
static s_sma_flt light_short_time_average;

unsigned short l_long_avg = 0;
unsigned short l_short_avg = 0;

void a_light_init(void)
{
    float initial_val = (float)Ambient_light_channel_data[1];
    SMA_InitFlt(&light_long_time_average, initial_val, (5 * 60 * 2)); // 5 minutes
    SMA_InitFlt(&light_short_time_average, initial_val, (5 * 2));      // 5 seconds
}
void a_light_poll_500ms(void)
{
    float v = (float)Ambient_light_channel_data[1];
    float long_time_avg = SMA_GetFlt(&light_long_time_average, v);
    float short_time_avg = SMA_GetFlt(&light_short_time_average, v);
    l_long_avg = (unsigned short)long_time_avg;
    l_short_avg = (unsigned short)short_time_avg;
}