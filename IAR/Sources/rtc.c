#include "rtc.h"

bool RtcInterruptFare = false;

void rtc_init(void)
{
    SIM_SCGC |= SIM_SCGC_RTC_MASK;
    RTC_SC |= RTC_SC_RTCLKS(1); // Real-time clock source is 1 kHz (LPOCLK)
    RTC_SC |= RTC_SC_RTCPS(6);  // Real-Time Clock Prescaler Select if RTCLKS = x1, it is 100
    RTC_MOD = RTC_MOD_MOD(140);
    __CLEAR_PENDING_INTERRUPT(INT_RTC);
    __ENABLE_INTERRUPT(INT_RTC);
    rtc_set_interrupt(false);
    RtcInterruptFare = false;
}

void rtc_set_interrupt(bool enable)
{
    if (enable)
    {
        RTC_SC |= RTC_SC_RTIE_MASK;
    }
    else
    {
        RTC_SC &= ~RTC_SC_RTIE_MASK;
    }
}

void RTC_IRQHandler(void)
{
    RTC_SC |= RTC_SC_RTIF_MASK;
    RtcInterruptFare = true;
}