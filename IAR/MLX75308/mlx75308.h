#pragma once
typedef enum
{
    AMB_LIGHT_0 = 1,
    AMB_LIGHT_1 = 2,
    AMB_LIGHT_2 = 4,
    LED_B = 8,
    LED_A = 0x10,
    TEMPERATURE = 0x80
} e_read_data_type;


#define TOTAL_AMB_LIGHT_CHANNELS 3



void mlx_init(void);
int mlx_read_data_by_id(e_read_data_type data, int tries, int max_val);
void mlx_read_ambient_light();
void mlx_sleep(void);
void mlx_wakeup(void);