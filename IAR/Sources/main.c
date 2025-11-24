#include "derivative.h" /* include peripheral declarations */
#include "lin.h"
#include "spi.h"
#include "mlx75308.h"
#include "lin_data.h"
#include "a_light.h"
#include "a_wipe.h"
#include "flash.h"
#include "low_power.h"
#include "rtc.h"
#include "intrinsics.h"
#include "kinetis_sysinit.h"
#include "eeprom_data.h"

#define TICKS_PERIOD 1000
uint32_t SystTick = 0;

#define IS_CAR_RLS 0
#if (IS_CAR_RLS == 1)
__root static const unsigned char SCFTRIM @0x000003FE = 0x01;
__root static const unsigned char SCTRIM @0x000003FF = 0x52;
#else
__root static const unsigned char SCFTRIM @0x000003FE = 0x00;
__root static const unsigned char SCTRIM @0x000003FF = 0x56;
#endif

void wdt_reload(void)
{
  __disable_interrupt();
  WDOG_CNT = 0x02A6; // write the 1st refresh word
  WDOG_CNT = 0x80B4; // write the 2nd refresh word to refresh counter
  __enable_interrupt();
}

void WDG_IRQHandler()
{
  WDOG_CS2 |= WDOG_CS2_FLG_MASK;
}
/***********************************************************************************************
 *
 * @brief    CLK_Init - Initialize the clocks to run at 20 MHz from the 10Mhz external XTAL
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void Clk_Init()
{
  ICS_C1 |= ICS_C1_IRCLKEN_MASK; /* Enable the internal reference clock*/
  ICS_C3 = SCTRIM;               /* Reference clock frequency = 39.0625 KHz*/
  ICS_C4 = ICS_C4 & 0xFE | SCFTRIM;
  while (!(ICS_S & ICS_S_LOCK_MASK))
    ;                       /* Wait for PLL lock, now running at 40 MHz (1024 * 39.0625Khz) */
  ICS_C2 |= ICS_C2_BDIV(1); /*BDIV=2, Bus clock = 20 MHz*/
  ICS_S |= ICS_S_LOCK_MASK; /* Clear Loss of lock sticky bit */
}
/***********************************************************************************************
 *
 * @brief    gpio_init - Initialize the pins for input/output
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void gpio_init()
{
  //
  CONFIG_PIN_AS_GPIO(PORT_B, (7 + 8), OUTPUT); // PTB7 - LIN EN
  OUTPUT_SET(PORT_B, (7 + 8));
  //
  CONFIG_PIN_AS_GPIO(PORT_B, (6 + 8), INPUT); // PTB6 - MLX DR
  ENABLE_INPUT(PORT_B, (6 + 8));
  //
  CONFIG_PIN_AS_GPIO(PORT_D, (3 + 24), INPUT); // PTD3- MLX MR
  ENABLE_INPUT(PORT_D, (3 + 24));
  //
  CONFIG_PIN_AS_GPIO(PORT_A, 6, OUTPUT); // PTA6 - MLX WT
  OUTPUT_SET(PORT_A, 6);
  //
  CONFIG_PIN_AS_GPIO(PORT_A, 7, OUTPUT); // PTA7 - MLX WKUP
  OUTPUT_SET(PORT_A, 7);
  PORT_PUEL |= (1 << 7);
  //
  CONFIG_PIN_AS_GPIO(PORT_B, (5 + 8), OUTPUT); // PTB5 - MLX CS
  OUTPUT_SET(PORT_B, (5 + 8));
  PORT_PUEL |= (1 << (5 + 8));
  //
  SIM_SOPT &= ~SIM_SOPT_NMIE_MASK;
  //
  CONFIG_PIN_AS_GPIO(PORT_A, 2, INPUT); // NOT USED
  ENABLE_INPUT(PORT_A, 2);
  CONFIG_PIN_AS_GPIO(PORT_A, 3, INPUT); // NOT USED
  ENABLE_INPUT(PORT_A, 3);
  CONFIG_PIN_AS_GPIO(PORT_A, 0, INPUT); // NOT USED
  ENABLE_INPUT(PORT_A, 0);
  CONFIG_PIN_AS_GPIO(PORT_A, 1, OUTPUT); // NOT USED
  OUTPUT_SET(PORT_A, 1);
  CONFIG_PIN_AS_GPIO(PORT_C, (0 + 16), INPUT); // NOT USED
  ENABLE_INPUT(PORT_C, (0 + 16));
  CONFIG_PIN_AS_GPIO(PORT_C, (1 + 16), INPUT); // NOT USED
  ENABLE_INPUT(PORT_C, (1 + 16));
  CONFIG_PIN_AS_GPIO(PORT_C, (2 + 16), INPUT); // NOT USED
  ENABLE_INPUT(PORT_C, (2 + 16));
  CONFIG_PIN_AS_GPIO(PORT_C, (5 + 16), INPUT); // NOT USED
  ENABLE_INPUT(PORT_C, (5 + 16));
  CONFIG_PIN_AS_GPIO(PORT_C, (6 + 16), INPUT); // NOT USED
  ENABLE_INPUT(PORT_C, (6 + 16));
  CONFIG_PIN_AS_GPIO(PORT_C, (7 + 16), OUTPUT); // NOT USED
  OUTPUT_SET(PORT_C, (7 + 16));
  CONFIG_PIN_AS_GPIO(PORT_D, (0 + 24), INPUT); // NOT USED
  ENABLE_INPUT(PORT_D, (0 + 24));
  CONFIG_PIN_AS_GPIO(PORT_D, (1 + 24), INPUT); // NOT USED
  ENABLE_INPUT(PORT_D, (1 + 24));
  CONFIG_PIN_AS_GPIO(PORT_D, (2 + 24), INPUT); // NOT USED
  ENABLE_INPUT(PORT_D, (2 + 24));
}
/***********************************************************************************************
 *
 * @brief   init_systick_timer - Initialize the timer for SystTick 1ms counter
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void init_systick_timer()
{
  SIM_SCGC |= SIM_SCGC_FTM2_MASK;                                             /* Enable Clock for FTM2 */
  FTM2_SC |= FTM_SC_PS(7);                                                    /* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */
                                                                              /* Counter increase by one every 6.4 us */
                                                                              /* Enable Channle 0*/
  FTM2_C0SC |= FTM_CnSC_CHIE_MASK;                                            /* Enable channel 0 interrupt */
  FTM2_C0SC |= FTM_CnSC_MSA_MASK;                                             /* Channel as Output compare mode */
                                                                              /*Select interrupt frequency*/
  FTM2_C0V = (uint32_t)((MCU_BUS_FREQ / 1000000.0) * (TICKS_PERIOD / 128.0)); /* Interrupt every 1ms */
  FTM2_SC |= FTM_SC_CLKS(1);                                                  /*FTM2 use system clock*/
  /* Set the ICPR and ISER registers accordingly */
  __CLEAR_PENDING_INTERRUPT(INT_FTM2);
  __ENABLE_INTERRUPT(INT_FTM2);
}

void FTM2_IRQHandler()
{
  if (FTM2_C0SC & FTM_CnSC_CHF_MASK) /* If the CHF of the channel is equal to 0 */
  {
    SystTick++;
    (void)FTM2_C0SC;                /* Read to clear flag */
    FTM2_C0SC ^= FTM_CnSC_CHF_MASK; /* Clear flag */
    FTM2_C0V += (uint32_t)((MCU_BUS_FREQ / 1000000.0) * (TICKS_PERIOD / 128.0));
  }
}

typedef enum
{
  PERIOD_10MS = 0x01,
  PERIOD_50MS = 0x02,
  PERIOD_100MS = 0x04,
  PERIOD_200MS = 0x08,
  PERIOD_500MS = 0x10,
  PERIOD_1000MS = 0x20,
  PEROID_ALL = 0xFF
} e_period;

typedef enum
{
  PERIOD_10MS_IDX,
  PERIOD_50MS_IDX,
  PERIOD_100MS_IDX,
  PERIOD_200MS_IDX,
  PERIOD_500MS_IDX,
  PERIOD_1000MS_IDX,

  PERIOD_COUNT,
} e_period_idx;
static uint32_t period_counters[PERIOD_COUNT];
static e_period period = PEROID_ALL;
static void PeriodsInit(void)
{
  uint32_t ticks = SystTick;
  period_counters[PERIOD_10MS_IDX] = ticks + 10;
  period_counters[PERIOD_50MS_IDX] = ticks + 50;
  period_counters[PERIOD_100MS_IDX] = ticks + 100;
  period_counters[PERIOD_200MS_IDX] = ticks + 200;
  period_counters[PERIOD_500MS_IDX] = ticks + 500;
  period_counters[PERIOD_1000MS_IDX] = ticks + 1000;
}
static void PeriodsPoll(void)
{
  uint32_t ticks = SystTick;
  if (period_counters[PERIOD_10MS_IDX] <= ticks)
  {
    period_counters[PERIOD_10MS_IDX] = ticks + 10;
    period |= PERIOD_10MS;
  }
  if (period_counters[PERIOD_50MS_IDX] <= ticks)
  {
    period_counters[PERIOD_50MS_IDX] = ticks + 50;
    period |= PERIOD_50MS;
  }
  if (period_counters[PERIOD_100MS_IDX] <= ticks)
  {
    period_counters[PERIOD_100MS_IDX] = ticks + 100;
    period |= PERIOD_100MS;
  }
  if (period_counters[PERIOD_200MS_IDX] <= ticks)
  {
    period_counters[PERIOD_200MS_IDX] = ticks + 200;
    period |= PERIOD_200MS;
  }
  if (period_counters[PERIOD_500MS_IDX] <= ticks)
  {
    period_counters[PERIOD_500MS_IDX] = ticks + 500;
    period |= PERIOD_500MS;
  }
  if (period_counters[PERIOD_1000MS_IDX] <= ticks)
  {
    period_counters[PERIOD_1000MS_IDX] = ticks + 1000;
    period |= PERIOD_1000MS;
  }
}

// static char test_write_string[] = "TEST EEPROM DATA STRING";
// static char test_write_string[] = "ANOTHER TEST EEPROM STR";
// static char test_read_string[sizeof(test_write_string)];

void init_rls_data(void)
{
  mlx_read_ambient_light();
  mlx_read_data_by_id(LED_A, 8, 1000);
  mlx_read_data_by_id(LED_B, 8, 1000);
  a_light_init();
  //a_wipe_init();
}

unsigned int SIM_SRSID_val = 0;
__no_init s_diag_data_var reset_counter;
__no_init s_diag_data_var reset_pin_counter; //counts TJA1028 reset
__no_init s_diag_data_var wdt_reset_counter;
__no_init s_diag_data_var wakeup_counter;

static void check_diag_data(s_diag_data_var *dat)
{
  if (dat->val != (dat->xor_val ^ UINT32_MAX))
  {
    dat->val = 0;
    dat->xor_val = dat->val ^ UINT32_MAX;
  }
}
void inc_diag_data(s_diag_data_var *dat)
{
  check_diag_data(dat);
  dat->val++;
  dat->xor_val = dat->val ^ UINT32_MAX;
}
unsigned int get_diag_data(s_diag_data_var *dat)
{
  check_diag_data(dat);
  return dat->val;
}

void wakeup_event(void)
{
  inc_diag_data(&wakeup_counter);
}

void main(void)
{
  __disable_interrupt();
  get_diag_data(&reset_pin_counter);
  get_diag_data(&wdt_reset_counter);
  get_diag_data(&wakeup_counter);

  SIM_SRSID_val = SIM_SRSID;
  inc_diag_data(&reset_counter);
  if (SIM_SRSID_val & SIM_SRSID_WDOG_MASK)
  {
    inc_diag_data(&wdt_reset_counter);
  }
  if (SIM_SRSID_val & SIM_SRSID_PIN_MASK)
  {
    inc_diag_data(&reset_pin_counter);
  }
  __init_hardware();

  Clk_Init();

  flash_init(MCU_BUS_FREQ);

  // eeprom_read(0x10000080, test_read_string, sizeof(test_read_string) & ~1);
  // eeprom_program(0x10000080, test_write_string, sizeof(test_write_string) & ~1);
  eeprom_data_init();
  rtc_init();
  gpio_init();
  spi_init();
  init_lin();
  init_systick_timer();
  mlx_init();
  init_rls_data();
  a_wipe_init();
  PeriodsInit();
  __enable_interrupt();
  for (;;)
  {
    wdt_reload();
    low_power_poll();
    PeriodsPoll();
    if (period & PERIOD_10MS)
    {
      period &= ~PERIOD_10MS;
    }
    if (period & PERIOD_50MS)
    {
      period &= ~PERIOD_50MS;
    }
    if (period & PERIOD_100MS)
    {
      mlx_read_data_by_id(LED_A, 8, 1000);
      mlx_read_data_by_id(LED_B, 8, 1000);
      mlx_read_ambient_light();
      lin_proc_data_100ms();
      a_wipe_poll_100ms();
      period &= ~PERIOD_100MS;
    }
    if (period & PERIOD_200MS)
    {
      period &= ~PERIOD_200MS;
    }
    if (period & PERIOD_500MS)
    {
      a_light_poll_500ms();
      period &= ~PERIOD_500MS;
    }
    if (period & PERIOD_1000MS)
    {
      period &= ~PERIOD_1000MS;
      // mlx_calibration();
    }
  }
}
