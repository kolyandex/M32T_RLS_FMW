#include "low_power.h"
#include "lin.h"
#include "intrinsics.h"
#include "mlx75308.h"
#include "rtc.h"
#include "a_wipe.h"
#include "eeprom_data.h"

unsigned int LinLastActiveTime = 0;
static bool we_need_to_check_rain = false;

static void go_to_sleep(void)
{
    eeprom_data_save();
    wdt_reload();
    __disable_interrupt();
    FTM1_C0SC &= ~FTM_CnSC_CHIE_MASK;
    FTM2_C0SC &= ~FTM_CnSC_CHIE_MASK;
    (void)FTM1_C0SC;
    (void)FTM2_C0SC;
    __DISABLE_INTERRUPT(INT_FTM1);
    __DISABLE_INTERRUPT(INT_FTM2);
    __CLEAR_PENDING_INTERRUPT(INT_FTM1);
    __CLEAR_PENDING_INTERRUPT(INT_FTM2);
    a_wipe_sleep();
    mlx_sleep();
    if (AllWindowsAreClosed || (IgnState != IGN_OFF))
    {
        we_need_to_check_rain = false;
    }
    else
    {
        we_need_to_check_rain = true;
    }
    rtc_set_interrupt(true);
    __CLEAR_PENDING_INTERRUPT(INT_UART0);
    __ENABLE_INTERRUPT(INT_UART0);
    __CLEAR_PENDING_INTERRUPT(INT_RTC);
    __ENABLE_INTERRUPT(INT_RTC);
    UART0_S2 |= UART_S2_RXEDGIF_MASK; // RxD Pin Active Edge Interrupt Flag
    UART0_BDH |= UART_BDH_RXEDGIE_MASK;
    OUTPUT_CLEAR(PORT_B, (7 + 8)); // LIN DIS
    do
    {
        RtcInterruptFare = false;
        WDOG_CNT = 0x02A6; // write the 1st refresh word
        WDOG_CNT = 0x80B4; // write the 2nd refresh word to refresh counter
        __disable_interrupt();
        PMC_SPMSC1 &= ~(PMC_SPMSC1_LVDRE_MASK | PMC_SPMSC1_LVDSE_MASK | PMC_SPMSC1_LVDE_MASK);
        SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
        __WFI();
        __enable_interrupt();
        // wakeup_event();
    } while ((RtcInterruptFare == true) && (we_need_to_check_rain == false));
}
void init_lin(void)
{
    OUTPUT_SET(PORT_B, (7 + 8)); // LIN EN
    l_sys_init();
    l_ifc_init(LI0);
    ld_init();
    __CLEAR_PENDING_INTERRUPT(INT_UART0);
    __ENABLE_INTERRUPT(INT_UART0);
}

void wakeup(void)
{
    OUTPUT_SET(PORT_B, (7 + 8)); // LIN EN
    l_sys_init();                // Only timer init
    if (RtcInterruptFare == false)
    {
        // Wakeup from LIN. This is "kostyl" because in some cases we
        // can't receive 0x11 LIN frame, if it goes after damaged 0x55 frame. THIS IS AUTOMOTIVE, BLEAT'
        // #warning Check what's wrong later with logic analyzer - Checked: Chery BCM sucks
        l_u8_wr_LI0_BCM_AllWindowsClosedFlag(0);
        // init_lin();
    }
    else
    {
        rtc_set_interrupt(false);
        a_wipe_rtc_wakeup();
    }
    init_systick_timer();
    mlx_wakeup();
    init_rls_data();
}

void low_power_poll(void)
{
    if (lin_lld_get_state() != SLEEP_MODE)
    {
        LinLastActiveTime = SystTick;
    }
    if (/*lin_goto_sleep_flg &&*/ (SystTick - LinLastActiveTime > 5000) && !RtcInterruptFare)
    {
        lin_goto_sleep_flg = 0;
        go_to_sleep();
        wakeup();
        wakeup_event();
    }
}