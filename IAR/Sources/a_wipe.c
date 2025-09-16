#include "a_wipe.h"
#include "sma.h"
#include "stdlib.h"
#include "string.h"

enum
{
    IR_CH_A = 0,
    IR_CH_B = 1,
    IR_CH_TOTAL = 2
};

typedef struct
{
    s_sma_flt rain_long_time_average;
    s_sma_flt rain_short_time_average;
    int rain_long_time_average_val;
    int rain_short_time_average_val;
    int rain_long_time_average_val_difference;
    int rain_immediately_average_val_difference;
    int rain_max_value_during_wipers_in_operation;
    int rain_max_value_during_wipers_in_operation_final;
    int min_val_for_last_10_sec;
    int max_val_for_last_10_sec;
    int val_for_last_10_sec_counter;
    int diff_val_for_last_10_sec;
    int wipers_one_time_counter;
    int abs_diff; //tmp
} s_ir_ch_ctx;

static s_ir_ch_ctx ir_ch[IR_CH_TOTAL];

static void ir_ch_init(s_ir_ch_ctx *ctx, float val)
{
    if (ctx)
    {
        memset(ctx, 0x00, sizeof(*ctx));
        SMA_InitFlt(&ctx->rain_long_time_average, val, (5 * 60 * 10)); // 5 minutes
        SMA_InitFlt(&ctx->rain_short_time_average, val, (3 * 10));     // 3 sec
    }
}
/**
 * @brief ir_ch_process
 *
 * @param ctx s_ir_ch_ctx
 * @param val Current channel data
 */
static void ir_ch_process(s_ir_ch_ctx *ctx, float val)
{
    if (ctx)
    {
        if (ctx->val_for_last_10_sec_counter == 0)
        {
            ctx->max_val_for_last_10_sec = INT32_MIN;
            ctx->min_val_for_last_10_sec = INT32_MAX;
        }
        ctx->val_for_last_10_sec_counter++;
        if (ctx->min_val_for_last_10_sec > (int)val)
        {
            ctx->min_val_for_last_10_sec = (int)val;
        }
        if (ctx->max_val_for_last_10_sec < (int)val)
        {
            ctx->max_val_for_last_10_sec = (int)val;
        }
        if (ctx->val_for_last_10_sec_counter >= 100)
        {
            ctx->val_for_last_10_sec_counter = 0;
            ctx->diff_val_for_last_10_sec = ctx->max_val_for_last_10_sec - ctx->min_val_for_last_10_sec;
        }

        if (WipersInOperationNow)
        {
            if ((int)val > ctx->rain_max_value_during_wipers_in_operation)
            {
                ctx->rain_max_value_during_wipers_in_operation = (int)val;
            }
        }
        else
        {
            ctx->rain_max_value_during_wipers_in_operation_final = ctx->rain_max_value_during_wipers_in_operation;
            ctx->rain_max_value_during_wipers_in_operation = 0;
        }
        ctx->rain_short_time_average_val = (int)SMA_GetFlt(&ctx->rain_short_time_average, val, 1);
        if (ctx->rain_long_time_average_val == 0)
        {
            ctx->rain_long_time_average_val = (int)val;
        }
        ctx->rain_long_time_average_val_difference = ctx->rain_long_time_average_val - ctx->rain_short_time_average_val;
        ctx->rain_immediately_average_val_difference = ctx->rain_long_time_average_val - (int)val;
        int avg_counts = 0;
        if (ctx->rain_immediately_average_val_difference < 800)
        {
            avg_counts++;
        }
        if (ctx->rain_immediately_average_val_difference < 0)
        {
            avg_counts++;
        }
        if (ctx->diff_val_for_last_10_sec < 50)
        {
            ctx->rain_long_time_average_val = (int)SMA_GetFlt(&ctx->rain_long_time_average, val, avg_counts);
        }

        ctx->abs_diff = abs(ctx->rain_long_time_average_val - ctx->rain_short_time_average_val);

        if (ctx->abs_diff > 100 && ctx->abs_diff < 500)
        {
            if (ctx->wipers_one_time_counter == 0)
            {
                lin_wipers_set_mode(WM_1_TIME, 0);
            }
            ctx->wipers_one_time_counter++;
            if (ctx->wipers_one_time_counter >= 50)
            {
                ctx->wipers_one_time_counter = 0;
            }
        }
        else if (ctx->abs_diff >= 500 && ctx->abs_diff < 2500)
        {
            ctx->wipers_one_time_counter = 0;
            lin_wipers_set_mode(WM_SLOW, 50);
        }
        else if (ctx->abs_diff >= 2500)
        {
            ctx->wipers_one_time_counter = 0;
            lin_wipers_set_mode(WM_FAST, 50);
        }
        else
        {
            ctx->wipers_one_time_counter = 0;
            lin_wipers_set_mode(WM_OFF, 0);
        }
    }
}

void a_wipe_init(void)
{
    ir_ch_init(&ir_ch[IR_CH_A], (float)IR_Channel_A_data[0]);
    ir_ch_init(&ir_ch[IR_CH_B], (float)IR_Channel_B_data[0]);
}
void a_wipe_poll_100ms(void)
{
    ir_ch_process(&ir_ch[IR_CH_A], (float)IR_Channel_A_data[0]);
    ir_ch_process(&ir_ch[IR_CH_B], (float)IR_Channel_B_data[0]);
}