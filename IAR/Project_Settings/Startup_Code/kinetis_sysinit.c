#include "kinetis_sysinit.h"
#include "derivative.h"

extern void __iar_program_start(void);

void Default_Handler()
{
    while (1)
        ;
}

/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler() __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__((weak, alias("Default_Handler")));
void SVC_Handler() __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__((weak, alias("Default_Handler")));

void FTMRH_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void PMC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void IRQ_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void I2C0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART0_SCI0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART1_SCI1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void UART2_SCI2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ADC0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ACMP0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void FTM0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void FTM1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void FTM2_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ACMP1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void PIT_CH0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void PIT_CH1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void KBI0_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void KBI1_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void ICS_IRQHandler() __attribute__((weak, alias("Default_Handler")));
void WDG_IRQHandler() __attribute__((weak, alias("Default_Handler")));

#pragma language = extended
#pragma segment = "CSTACK"
typedef void (*vector_entry)(void);
typedef union
{
    vector_entry __fun;
    void *__ptr;
} intvec_elem;

__root const intvec_elem __vector_table[] @ ".intvec" =
    {
        {.__ptr = __sfe("CSTACK")}, /* Initial SP           */
        __iar_program_start,
        NMI_Handler,
        HardFault_Handler,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        SVC_Handler,
        0,
        0,
        PendSV_Handler,
        SysTick_Handler,

        /* Interrupts */
        Default_Handler,       /* Reserved interrupt 16/0 */
        Default_Handler,       /* Reserved interrupt 17/1 */
        Default_Handler,       /* Reserved interrupt 18/2 */
        Default_Handler,       /* Reserved interrupt 19/3 */
        Default_Handler,       /* Reserved interrupt 20/4 */
        FTMRH_IRQHandler,      /* Command complete and error interrupt */
        PMC_IRQHandler,        /* Low-voltage detect, low-voltage warning */
        IRQ_IRQHandler,        /* External interrupt */
        I2C0_IRQHandler,       /* Single interrupt vector for all sources */
        Default_Handler,       /* Reserved interrupt 25/9 */
        SPI0_IRQHandler,       /* Single interrupt vector for all sources */
        SPI1_IRQHandler,       /* Single interrupt vector for all sources */
        UART0_SCI0_IRQHandler, /* UART0 Status and Error interrupt */
        UART1_SCI1_IRQHandler, /* UART1 Status and Error interrupt */
        UART2_SCI2_IRQHandler, /* UART2 Status and Error interrupt */
        ADC0_IRQHandler,       /* ADC conversion complete interrupt */
        ACMP0_IRQHandler,      /* Analog comparator 0 interrupt */
        FTM0_IRQHandler,       /* Single interrupt vector for all sources */
        FTM1_IRQHandler,       /* Single interrupt vector for all sources */
        FTM2_IRQHandler,       /* Single interrupt vector for all sources */
        RTC_IRQHandler,        /* RTC overflow */
        ACMP1_IRQHandler,      /* Analog comparator 1 interrupt */
        PIT_CH0_IRQHandler,    /* PIT CH0 overflow */
        PIT_CH1_IRQHandler,    /* PIT CH1 overflow */
        KBI0_IRQHandler,       /* Keyboard interrupt0 */
        KBI1_IRQHandler,       /* Keyboard interrupt1 */
        Default_Handler,       /* Reserved interrupt 42/26 */
        ICS_IRQHandler,        /* Clock loss of lock Interrupt */
        WDG_IRQHandler,        /* WDG timeout interrupt */
        Default_Handler,       /* Reserved interrupt 45/29 */
        Default_Handler,       /* Reserved interrupt 46/30 */
        Default_Handler,       /* Reserved interrupt 47/31 */
};
#define CONFIG_1 0xffffffff
#define CONFIG_2 0xffffffff
#define CONFIG_3 0xffffffff
#define CONFIG_4 0xfffeffff
__root const unsigned int Config[] @ "FlashConfig" =
    {
        CONFIG_1,
        CONFIG_2,
        CONFIG_3,
        CONFIG_4,
};

void __init_hardware()
{
    SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
    WDOG_CNT = 0x20C5;
    WDOG_CNT = 0x28D9;
    WDOG_CS1 = WDOG_CS1_DBG_MASK | WDOG_CS1_EN_MASK | WDOG_CS1_UPDATE_MASK | WDOG_CS1_STOP_MASK; // Debug Enable, Watchdog Enable, Allow updates, Stop Enable
    WDOG_CS2 = WDOG_CS2_CLK(1);                                                                  // 1 kHz internal low-power oscillator (LPOCLK)
    WDOG_TOVALH = (15000 >> 8) & 0xFF;                                                           // 15 second wdt
    WDOG_TOVALL = (15000 >> 0) & 0xFF;
}