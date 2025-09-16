#include "low_power.h"
#include "lin.h"
#include "intrinsics.h"
#include "mlx75308.h"

static unsigned int last_lin_active_time = 0;

void low_power_poll(void)
{
    if (lin_lld_get_state() != SLEEP_MODE)
    {
        last_lin_active_time = SystTick;
    }
}

static void go_to_sleep(void)
{
    __disable_interrupt();
    FTM1_C0SC &= ~FTM_CnSC_CHIE_MASK;
    FTM2_C0SC &= ~FTM_CnSC_CHIE_MASK;
    (void)FTM1_C0SC;
    (void)FTM2_C0SC;
    NVIC_ICER |= 1 << ((INT_FTM1 - 16) % 32);
    NVIC_ICER |= 1 << ((INT_FTM2 - 16) % 32);
    NVIC_ICPR |= 1 << ((INT_FTM1 - 16) % 32);
    NVIC_ICPR |= 1 << ((INT_FTM2 - 16) % 32);

    mlx_sleep();
    // HERE ADD RTC INTERRUPT
    NVIC_ICPR |= 1 << ((INT_UART0 - 16) % 32);
    NVIC_ISER |= 1 << ((INT_UART0 - 16) % 32);
    UART0_S2 |= UART_S2_RXEDGIF_MASK; // RxD Pin Active Edge Interrupt Flag
    UART0_BDH |= UART_BDH_RXEDGIE_MASK;
    OUTPUT_CLEAR(PORT_B, (7 + 8)); // LIN DIS
    PMC_SPMSC1 &= ~(PMC_SPMSC1_LVDRE_MASK | PMC_SPMSC1_LVDSE_MASK | PMC_SPMSC1_LVDE_MASK);
    SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;
    __WFI();
    __enable_interrupt();
}

void wakeup(void)
{
    l_u8 vector_number;
    OUTPUT_SET(PORT_B, (7 + 8)); // LIN EN
    l_sys_init();
    l_ifc_init(LI0);
    ld_init();
    vector_number = INT_UART0 - 16;
    NVIC_ICPR |= 1 << (vector_number % 32);
    NVIC_ISER |= 1 << (vector_number % 32);
    lin_application_timer_FTM2();
    mlx_wakeup();
    init_rls_data();
}

void low_power_poll_1000ms(void)
{
    if (/*lin_goto_sleep_flg &&*/ (SystTick - last_lin_active_time > 5000))
    {
        lin_goto_sleep_flg = 0;
        go_to_sleep();
        wakeup();
    }
}