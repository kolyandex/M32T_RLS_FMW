/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */
#include "lin.h"
#include "spi.h"
#include "mlx75308.h"
#include "lin_data.h"
#include "a_light.h"
/**********************************************************************************************
 * Constants and macros
 **********************************************************************************************/
// #define LED0_TOGGLE		OUTPUT_TOGGLE(PORT_C,16)
// #define LED1_TOGGLE		OUTPUT_TOGGLE(PORT_C,17)
// #define LED2_TOGGLE		OUTPUT_TOGGLE(PORT_C,18)
// #define LED3_TOGGLE		OUTPUT_TOGGLE(PORT_C,19)
//
// #define LED0_OFF		OUTPUT_CLEAR(PORT_C,16);
// #define LED1_OFF		OUTPUT_CLEAR(PORT_C,17);
// #define LED2_OFF		OUTPUT_CLEAR(PORT_C,18);
// #define LED3_OFF		OUTPUT_CLEAR(PORT_C,19);
//
// #define LED0_ON			OUTPUT_SET(PORT_C,16);
// #define LED1_ON			OUTPUT_SET(PORT_C,17);
// #define LED2_ON			OUTPUT_SET(PORT_C,18);
// #define LED3_ON			OUTPUT_SET(PORT_C,19);

#define OUTPUT 1
#define INPUT 0

#define PORT_A A
#define PORT_B A
#define PORT_C A
#define PORT_D A

#define PORT_E B
#define PORT_F B
#define PORT_G B
#define PORT_H B

#define CONFIG_PIN_AS_GPIO(port, register_number, mode) XCONFIG_PIN_AS_GPIO(port, register_number, mode)
#define XCONFIG_PIN_AS_GPIO(port, register_number, mode) (mode == 0) ? (GPIO##port##_PDDR &= ~(1 << register_number)) : (GPIO##port##_PDDR |= 1 << register_number)

#define ENABLE_INPUT(port, pin_number) XENABLE_INPUT(port, pin_number)
#define XENABLE_INPUT(port, pin_number) GPIO##port##_PIDR &= ~(1 << pin_number)

#define ENABLE_PULLUP(port, pin_number) XENABLE_PULLUP(port, pin_number)
#define XENABLE_PULLUP(port, pin_number) PORT_PUE0 |= PORT_PUE0_PT##port##PE##pin_number##_MASK

#define OUTPUT_SET(port, register_num) XOUTPUT_SET(port, register_num)
#define XOUTPUT_SET(port, register_num) GPIO##port##_PSOR |= 1 << register_num

#define OUTPUT_CLEAR(port, register_num) XOUTPUT_CLEAR(port, register_num)
#define XOUTPUT_CLEAR(port, register_num) GPIO##port##_PCOR |= 1 << register_num

#define OUTPUT_TOGGLE(port, register_num) XOUTPUT_TOGGLE(port, register_num)
#define XOUTPUT_TOGGLE(port, register_num) GPIO##port##_PTOR |= 1 << register_num

__root static const unsigned char SCFTRIM @ 0x000003FE = 0x01;
__root static const unsigned char SCTRIM @ 0x000003FF = 0x52;


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
  ICS_C3 = SCTRIM;                 /* Reference clock frequency = 39.0625 KHz*/
  ICS_C4 = ICS_C4 & 0xFE | SCFTRIM;
  while (!(ICS_S & ICS_S_LOCK_MASK))
    ;                       /* Wait for PLL lock, now running at 40 MHz (1024 * 39.0625Khz) */
  ICS_C2 |= ICS_C2_BDIV(1); /*BDIV=2, Bus clock = 20 MHz*/
  ICS_S |= ICS_S_LOCK_MASK; /* Clear Loss of lock sticky bit */
}
/***********************************************************************************************
 *
 * @brief    GPIO_Init - Initialize the pins for input/output
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void GPIO_Init()
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
 * @brief   lin_application_timer_FTM2 - Initialize the timer for LIN application
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void lin_application_timer_FTM2()
{
  SIM_SCGC |= SIM_SCGC_FTM2_MASK;  /* Enable Clock for FTM2 */
  FTM2_SC |= FTM_SC_PS(7);         /* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */
                                   /* Counter increase by one every 6.4 us */
                                   /* Enable Channle 0*/
  FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
  FTM2_C0SC |= FTM_CnSC_MSA_MASK;  /* Channel as Output compare mode */
                                   /*Select interrupt frequency*/
  FTM2_C0V = FTM_CnV_VAL(391);     /* Interrupt every 2.5ms */

  FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/

  /* Set the ICPR and ISER registers accordingly */
  NVIC_ICPR |= 1 << ((INT_FTM2 - 16) % 32);
  NVIC_ISER |= 1 << ((INT_FTM2 - 16) % 32);
}

unsigned char BatteryVoltageLin_x10;
short VehicleSpeed;
volatile static int LED_A_DATA, LED_B_DATA;
void __init_hardware();

#define TICKS_PERIOD 1000
uint32_t SystTick = 0;
void FTM2_IRQHandler()
{
  if (1 == ((FTM2_C0SC & FTM_CnSC_CHF_MASK) >> FTM_CnSC_CHF_SHIFT)) /* If the CHF of the channel is equal to 0 */
  {
    SystTick++;
    (void)FTM2_C0SC;                /* Read to clear flag */
    FTM2_C0SC ^= FTM_CnSC_CHF_MASK; /* Clear flag */
    // FTM2_C0V = FTM2_C0V + 391;      /* Refresh interrupt period */
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

void main(void)
{
  __init_hardware();
  l_u8 vector_number;
  Clk_Init();
  GPIO_Init();
  l_sys_init();
  l_ifc_init(LI0);
  ld_init();
  vector_number = INT_UART0 - 16;
  NVIC_ICPR |= 1 << (vector_number % 32);
  NVIC_ISER |= 1 << (vector_number % 32);
  lin_application_timer_FTM2();
  spi_init();
  mlx_init();
  mlx_read_ambient_light();
  a_light_init();
  PeriodsInit();
  for (;;)
  {
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
      LED_A_DATA = mlx_read_data_by_id(LED_A, 8, 1000);
      LED_B_DATA = mlx_read_data_by_id(LED_B, 8, 1000);
      mlx_read_ambient_light();
      lin_proc_data_100ms();
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
      //mlx_calibration();
    }
  }
}
