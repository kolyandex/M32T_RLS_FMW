#include "a_light.h"
#include "sma.h"

/**
 * @brief Final variable txed to car
 *
 */
bool TurnOnLights = false;
/**
 * @brief Long-time average ambient light data
 *
 */
static s_sma_flt light_long_time_average;
/**
 * @brief Short-time average ambient light data
 *
 */
static s_sma_flt light_short_time_average;

unsigned short l_long_avg = 0;
unsigned short l_short_avg = 0;

static bool a_light_on_request_fast = false;
static bool a_light_on_request_slow = false;

// TODO: MOVE TO CALIBRATIONS
static const unsigned short fast_on_thresold = 14000;
static const unsigned short fast_off_thresold = 20000;
static const unsigned short slow_on_thresold = 16500;
static const unsigned short slow_off_thresold = 18500;

void a_light_init(void)
{
    float initial_val = (float)Ambient_light_channel_data[1];
    SMA_InitFlt(&light_long_time_average, initial_val, (5 * 60 * 2)); // 5 minutes
    SMA_InitFlt(&light_short_time_average, initial_val, (3 * 2));     // 3 seconds
}

void a_light_poll_500ms(void)
{
    float v = (float)Ambient_light_channel_data[1];
    float long_time_avg = SMA_GetFlt(&light_long_time_average, v, 1);
    float short_time_avg = SMA_GetFlt(&light_short_time_average, v, 1);
    l_long_avg = (unsigned short)long_time_avg;
    l_short_avg = (unsigned short)short_time_avg;

    // In case of ambient light level is higher than fast_off_thresold no need to hold lights on according light_long_time_average
    if (l_short_avg >= fast_off_thresold)
    {
        SMA_SetFlt(&light_long_time_average, v);
    }
    if (a_light_on_request_fast && (l_short_avg >= fast_off_thresold))
    {
        a_light_on_request_fast = false;
    }
    if (!a_light_on_request_fast && (l_short_avg <= fast_on_thresold))
    {
        a_light_on_request_fast = true;
    }
    if (a_light_on_request_slow && (l_long_avg >= slow_off_thresold))
    {
        a_light_on_request_slow = false;
    }
    if (!a_light_on_request_slow && (l_long_avg <= slow_on_thresold))
    {
        a_light_on_request_slow = true;
    }

    TurnOnLights = (a_light_on_request_fast || a_light_on_request_slow);
}