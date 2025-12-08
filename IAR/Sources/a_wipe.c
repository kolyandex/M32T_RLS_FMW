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
    int rain_long_time_average_val;
    int rain_immediately_average_val_difference;
    int min_val_for_last_10_sec;
    int max_val_for_last_10_sec;
    int val_for_last_10_sec_counter;
    int diff_val_for_last_10_sec;
    int abs_diff; // tmp
    cbuff_t cbuff;
    unsigned short cbuff_buff[8];
    int min_val_for_cbuff;
    int max_val_for_cbuff;

    cbuff_t wio_cbuff;
    unsigned short wio_cbuff_buff[32];
    s_sma_flt wio_max_average;
    int wio_avg_max_val;
    int wio_max_diff_abs;
} s_ir_ch_ctx;

static s_ir_ch_ctx ir_ch[IR_CH_TOTAL];
static unsigned char washer_last_op_counter = 0;
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
    BeforeSleepIrData[IR_CH_A] = IR_Channel_A_data;
    BeforeSleepIrData[IR_CH_B] = IR_Channel_B_data;
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
        SMA_InitFlt(&ctx->rain_long_time_average, val, (3 * 15 * 10)); // ~3 minutes
        cbuff_init(&ctx->cbuff, (uint8_t *)ctx->cbuff_buff, sizeof(ctx->cbuff_buff));
        cbuff_init(&ctx->wio_cbuff, (uint8_t *)ctx->wio_cbuff_buff, sizeof(ctx->wio_cbuff_buff));
        SMA_InitFlt(&ctx->wio_max_average, val, 1 * 15 * 10); // ~1 minute
        for (int i = 0; i < SIZEOF_ARR(ctx->cbuff_buff); i++)
        {
            ctx->cbuff_buff[i] = (unsigned short)val;
        }
        for (int i = 0; i < SIZEOF_ARR(ctx->wio_cbuff_buff); i++)
        {
            ctx->wio_cbuff_buff[i] = (unsigned short)val;
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
        if (ctx->val_for_last_10_sec_counter >= 300)
        {
            ctx->val_for_last_10_sec_counter = 0;
            ctx->diff_val_for_last_10_sec = ctx->max_val_for_last_10_sec - ctx->min_val_for_last_10_sec;
            if (ctx->diff_val_for_last_10_sec <= 30)
            {
                avg_counts += 100;
            }
        }
        if (WipersInOperationNow)
        {
            unsigned short cbuff_val = (unsigned short)val;
            write_to_cbuff(&ctx->wio_cbuff, (uint8_t *)&cbuff_val, sizeof(cbuff_val));
            ctx->wio_max_diff_abs = 0;
        }
        else
        {
            int wio_max_val = 0;
            int wio_min_val = INT32_MAX;
            for (int i = 0; i < SIZEOF_ARR(ctx->wio_cbuff_buff); i++)
            {
                if (ctx->wio_cbuff_buff[i] == 0)
                {
                    continue;
                }
                if (ctx->wio_cbuff_buff[i] > wio_max_val)
                {
                    wio_max_val = ctx->wio_cbuff_buff[i];
                }
                if (ctx->wio_cbuff_buff[i] < wio_min_val)
                {
                    wio_min_val = ctx->wio_cbuff_buff[i];
                }
            }
            memset(ctx->wio_cbuff_buff, 0, sizeof(ctx->wio_cbuff_buff));
            if ((wio_max_val != 0) && (wio_min_val != INT32_MAX))
            {
                ctx->wio_max_diff_abs = abs(wio_max_val - wio_min_val);
                ctx->wio_avg_max_val = (int)SMA_GetFlt(&ctx->wio_max_average, (float)wio_max_val, 1);
                if (wio_max_val > ctx->rain_long_time_average_val)
                {
                    SMA_GetFlt(&ctx->rain_long_time_average, (float)wio_max_val, 1);
                }
            }
        }

        if (ctx->rain_long_time_average_val == 0)
        {
            ctx->rain_long_time_average_val = (int)val;
        }
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
        if (WipersInOperationNow == false)
        {
            write_to_cbuff(&ctx->cbuff, (uint8_t *)&cbuff_val, sizeof(cbuff_val));
        }

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

        if (ctx->max_val_for_cbuff > ctx->wio_avg_max_val)
        {
            ctx->wio_avg_max_val = (int)SMA_GetFlt(&ctx->wio_max_average, (float)ctx->max_val_for_cbuff, 1);
        }
        // ctx->abs_diff = ctx->wio_avg_max_val - ctx->min_val_for_cbuff;
        ctx->abs_diff = ctx->rain_long_time_average_val - ctx->min_val_for_cbuff;
    }
}

static s_sma_flt val_diff_sma;

void a_wipe_init(void)
{
    ir_ch_init(&ir_ch[IR_CH_A], (float)IR_Channel_A_data);
    ir_ch_init(&ir_ch[IR_CH_B], (float)IR_Channel_B_data);
    SMA_InitFlt(&val_diff_sma, 0.f, 2 * 15 * 10); // ~2 minute
}

static unsigned char dousing_detection_counter = 0;
static int avg_diff = 0;
static const int a_wipe_thresold_divider[AUTO_WIPERS_THRESOLD_MODES_COUNT] =
    {50, 40, 30, 20};
void a_wipe_poll_100ms(void)
{
    switch (wkup_step)
    {
    case WKUP_IDLE:
    {
        RtcInterruptFare = false;
        if (WasherLongPressed)
        {
            WasherLongPressed = false;
            washer_last_op_counter = 80;
        }
        if (washer_last_op_counter)
        {
            washer_last_op_counter--;
        }
        else
        {
            ir_ch_process(&ir_ch[IR_CH_A], (float)IR_Channel_A_data);
            ir_ch_process(&ir_ch[IR_CH_B], (float)IR_Channel_B_data);
            int max_diff = ir_ch[IR_CH_B].abs_diff;
            if (ir_ch[IR_CH_A].abs_diff > ir_ch[IR_CH_B].abs_diff)
            {
                max_diff = ir_ch[IR_CH_A].abs_diff;
            }
            // Dousing detection
            if ((ir_ch[IR_CH_A].abs_diff > 6000) && (ir_ch[IR_CH_B].abs_diff > 6000))
            {
                if (dousing_detection_counter < 10)
                {
                    dousing_detection_counter++;
                }
                else
                {
                    val_diff_sma.val = (float)(a_wipe_thresold_divider[AutoWipersThresold] * 100);
                }
            }
            else
            {
                dousing_detection_counter = 0;
            }
            if (avg_diff < max_diff)
            {
                avg_diff = (int)SMA_GetFlt(&val_diff_sma, (float)max_diff, 2);
            }
            else
            {
                avg_diff = (int)SMA_GetFlt(&val_diff_sma, (float)max_diff, 2);
            }

            if (avg_diff > 500)
            {
                RainDetectedCloseWindowsRequest = true;
            }
            lin_wipers_enable(avg_diff / a_wipe_thresold_divider[AutoWipersThresold]);
        }
        break;
    }
    case WKUP_CHK_COND:
    {
        if (
            (abs((int)BeforeSleepIrData[IR_CH_A] - (int)IR_Channel_A_data) > 500) ||
            (abs((int)BeforeSleepIrData[IR_CH_B] - (int)IR_Channel_B_data) > 500))
        {
            // init_lin();
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