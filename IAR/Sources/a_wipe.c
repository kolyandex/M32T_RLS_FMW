#include "a_wipe.h"
#include "sma.h"
#include "stdlib.h"
#include "string.h"
#include "cbuff.h"
#include "low_power.h"
#include "lin.h"

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
    int abs_diff; // tmp
    cbuff_t cbuff;
    unsigned short cbuff_buff[16];
    int min_val_for_cbuff;
    int max_val_for_cbuff;
} s_ir_ch_ctx;

static s_ir_ch_ctx ir_ch[IR_CH_TOTAL];
static unsigned char washer_last_op_counter = 0;
static const int a_wipe_thresold[AUTO_WIPERS_THRESOLD_MODES_COUNT][4] =
    {
        {400, 2000, 5000, 10000},
        {300, 1500, 4000, 7000},
        {200, 1000, 3000, 5000},
        {100, 500, 2000, 4000},
};

typedef enum
{
    WKUP_IDLE,
    WKUP_CHK_COND,
    WKUP_SEND_BREAK,
    WKUP_WAIT_BUS_ACTIVITY,
    WKUP_SEND_CLOSE_WINDOWS_FLAG
} e_wkup_step;
unsigned short BeforeSleepIrData[IR_CH_TOTAL] = {0, 0};
static bool before_sleep_data_present = false;
static e_wkup_step wkup_step = WKUP_IDLE;
static unsigned char wkup_time_counter = 0;
static unsigned int last_lin_active_time = 0;

void a_wipe_sleep(void)
{
    BeforeSleepIrData[IR_CH_A] = IR_Channel_A_data[0];
    BeforeSleepIrData[IR_CH_B] = IR_Channel_B_data[0];
    before_sleep_data_present = true;
}
void a_wipe_rtc_wakeup(void)
{
    if (before_sleep_data_present)
    {
        before_sleep_data_present = false;
        wkup_step = WKUP_CHK_COND;
    }
    else
    {
        wkup_step = WKUP_IDLE;
    }
}

static void ir_ch_init(s_ir_ch_ctx *ctx, float val)
{
    if (ctx)
    {
        memset(ctx, 0x00, sizeof(*ctx));
        SMA_InitFlt(&ctx->rain_long_time_average, val, (3 * 60 * 10)); // 3 minutes
        SMA_InitFlt(&ctx->rain_short_time_average, val, (3 * 10));     // 3 sec
        cbuff_init(&ctx->cbuff, (uint8_t *)ctx->cbuff_buff, sizeof(ctx->cbuff_buff));
        for (int i = 0; i < SIZEOF_ARR(ctx->cbuff_buff); i++)
        {
            ctx->cbuff_buff[i] = (unsigned short)val;
        }
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
    int avg_counts = 0;
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
            if (ctx->diff_val_for_last_10_sec <= 20)
            {
                avg_counts += 100;
            }
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

        unsigned short cbuff_val = (unsigned short)val;
        write_to_cbuff(&ctx->cbuff, (uint8_t *)&cbuff_val, sizeof(cbuff_val));

        ctx->max_val_for_cbuff = INT32_MIN;
        ctx->min_val_for_cbuff = INT32_MAX;

        for (int i = 0; i < SIZEOF_ARR(ctx->cbuff_buff); i++)
        {
            if (ctx->cbuff_buff[i] > ctx->max_val_for_cbuff)
            {
                ctx->max_val_for_cbuff = ctx->cbuff_buff[i];
            }
            if (ctx->cbuff_buff[i] < ctx->min_val_for_cbuff)
            {
                ctx->min_val_for_cbuff = ctx->cbuff_buff[i];
            }
        }

        ctx->abs_diff = abs(ctx->max_val_for_cbuff - ctx->min_val_for_cbuff);
        // ctx->abs_diff = abs(ctx->rain_long_time_average_val - ctx->rain_short_time_average_val);
        int const *th = a_wipe_thresold[AutoWipersThresold];
        RainDetectedCloseWindowsRequest = true;
        if (ctx->abs_diff > th[0] && ctx->abs_diff < th[1])
        {
            ctx->wipers_one_time_counter = 0;
            if (WipersInOperationNow == false)
            {
                lin_wipers_set_mode(WM_1_TIME, 0);
            }
        }
        else if (ctx->abs_diff >= th[1] && ctx->abs_diff < th[2])
        {
            ctx->wipers_one_time_counter = 0;
            int time = (ctx->abs_diff - th[1]) * 100 / (th[2] - th[1]);
            time = (50 * time / 100) + 10;
            lin_wipers_set_mode(WM_SLOW, time);
        }
        else if (ctx->abs_diff >= th[2])
        {
            int time = (ctx->abs_diff - th[2]) * 100 / (th[3] - th[2]);
            time = (50 * time / 100) + 10;
            ctx->wipers_one_time_counter = 0;
            lin_wipers_set_mode(WM_FAST, time);
        }
        else
        {
            if (abs(ctx->rain_long_time_average_val_difference) > 1000)
            {
                if (ctx->wipers_one_time_counter == 0)
                {
                    lin_wipers_set_mode(WM_1_TIME, 0);
                }
                ctx->wipers_one_time_counter++;
                if (ctx->wipers_one_time_counter >= 100)
                {
                    ctx->wipers_one_time_counter = 0;
                }
            }
            else
            {
                RainDetectedCloseWindowsRequest = false;
                ctx->wipers_one_time_counter = 0;
                lin_wipers_set_mode(WM_OFF, 0);
            }
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
    switch (wkup_step)
    {
    case WKUP_IDLE:
    {
        RtcInterruptFare = false;
        if (WasherInOperationNow)
        {
            washer_last_op_counter = 80;
        }
        if (washer_last_op_counter)
        {
            washer_last_op_counter--;
        }
        else
        {
            ir_ch_process(&ir_ch[IR_CH_A], (float)IR_Channel_A_data[0]);
            ir_ch_process(&ir_ch[IR_CH_B], (float)IR_Channel_B_data[0]);
        }
        break;
    }
    case WKUP_CHK_COND:
    {
        if (
            (abs((int)BeforeSleepIrData[IR_CH_A] - (int)IR_Channel_A_data[0]) > 500) ||
            (abs((int)BeforeSleepIrData[IR_CH_B] - (int)IR_Channel_B_data[0]) > 500))
        {
            init_lin();
            wkup_step = WKUP_SEND_BREAK;
        }
        else
        {
            wkup_step = WKUP_IDLE;
        }
        break;
    }
    case WKUP_SEND_BREAK:
    {
        UART0_C2 |= UART_C2_SBK_MASK;
        UART0_C2 &= ~UART_C2_SBK_MASK;
        wkup_step = WKUP_WAIT_BUS_ACTIVITY;
        wkup_time_counter = 0;
        last_lin_active_time = LinLastActiveTime;
        break;
    }
    case WKUP_WAIT_BUS_ACTIVITY:
    {
        if (last_lin_active_time != LinLastActiveTime)
        {
            RainDetectedCloseWindowsRequest = true;
            wkup_step = WKUP_SEND_CLOSE_WINDOWS_FLAG;
            wkup_time_counter = 0;
        }
        else
        {
            wkup_time_counter++;
            if (wkup_time_counter > 20)
            {
                wkup_step = WKUP_IDLE;
            }
        }
        break;
    }
    case WKUP_SEND_CLOSE_WINDOWS_FLAG:
    {
        wkup_time_counter++;
        if ((wkup_time_counter > 100) || (AllWindowsAreClosed))
        {
            l_u8_wr_LI0_BCM_AllWindowsClosedFlag(1);
            RainDetectedCloseWindowsRequest = false;
            wkup_step = WKUP_IDLE;
        }
        break;
    }
    }
}