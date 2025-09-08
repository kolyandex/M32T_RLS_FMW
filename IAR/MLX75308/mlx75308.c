#include "mlx75308.h"

#include "spi.h"
#include "derivative.h" /* include peripheral declarations */
#include "table_lookup.h"

extern const s_table_2d_uint16_uint32 * TBL_RawAmbientLightData[TOTAL_AMB_LIGHT_CHANNELS];

unsigned char dac_level_ir_ch_a = 128;
unsigned char dac_level_ir_ch_b = 128;

unsigned short IR_Channel_A_data[6];
unsigned short IR_Channel_B_data[6];
unsigned short Ambient_light_channel_data[TOTAL_AMB_LIGHT_CHANNELS];
unsigned int AmbientLIghtLevelsScaled[TOTAL_AMB_LIGHT_CHANNELS];
static unsigned short IR_Channel_data_SRC;

#define CS_SET (GPIOA_PCOR |= (1 << (5 + 8)))
#define CS_RESET (GPIOA_PSOR |= (1 << (5 + 8)))

static void delay_soft(int t)
{
    signed int v1;
    for (; t; --t)
    {
        v1 = 3200;
        do
            --v1;
        while (v1);
    }
}
static void wait_dr(void)
{
    while (!((GPIOA_PDIR >> 14) & 1))
        ;
}
typedef enum
{
    REG_SetAna = 0,
    REG_SetAH = 1,
    REG_SetAL = 2,
    REG_SetBH = 3,
    REG_SetBL = 4,
    REG_SetPF = 5,
    REG_Err = 6,
    REG_Rst = 7,
    REG_DCComp1 = 8,
    REG_DCComp2 = 9,
    REG_GainBuf = 10,
    REG_Calib1 = 11,
    REG_Calib2 = 12,
    REG_EnChan = 13,
    REG_Tabm = 14
} e_user_reg;
static unsigned int read_register(e_user_reg reg)
{
    unsigned int result;
    int tmp = (reg << 12) | 0x8E0000;
    // wdt_reload();
    wait_dr();
    CS_SET;
    spi_tx_rx(tmp >> 16);
    result = (spi_tx_rx((tmp << 16) >> 24) << 8) | spi_tx_rx(0);
    CS_RESET;
    if (result >> 8 == 0x8E)
        result = result;
    else
        result = 0xAA;
    return result;
}

signed int write_register(unsigned int val, e_user_reg reg)
{
    unsigned int r = (unsigned int)reg;
    unsigned int tmp = 4 * ((val >> 7) ^ (r ^ (r >> 1) ^ (r >> 2) ^ (r >> 3) ^ (val >> 6) ^ (val >> 5) ^ (val >> 4) ^ (val >> 3) ^ (val >> 2) ^ (val >> 1) ^ val) & 1 | 2 * (r & 1 ^ (r << 30 >> 31) ^ (r >> 2) & 1 ^ (r >> 3) & 1 ^ val & 1 ^ (val << 30 >> 31) ^ (val << 29 >> 31) ^ (val << 28 >> 31) ^ (val << 27 >> 31) ^ (val << 26 >> 31) ^ (val >> 6) & 1 ^ (val >> 7) ^ 1 | 2 * (r | 16 * val)));
    // wdt_reload();
    wait_dr();
    CS_SET;
    spi_tx_rx(0x87);
    spi_tx_rx(tmp << 16 >> 24);
    spi_tx_rx(tmp);
    CS_RESET;
    return 1;
}

void regs_proc(int a, int val)
{
    unsigned int tmp;
    e_user_reg r;
    switch (a)
    {
    case 1:
        // wdt_reload();
        tmp = read_register(REG_SetAna) & 0x0F;
        if (val)
        {
            tmp |= (val << 4); // Tdemx
        }
        write_register(tmp, REG_SetAna);
        break;
    case 2:
        // wdt_reload();
        tmp = read_register(REG_SetAna) & 0xFE;
        if (val)
        {
            tmp |= 0x01; // Unity_Gain
        }
        write_register(tmp, REG_SetAna);
        break;
    case 3:
    case 4:
        if (a == 3)
            r = REG_SetAL;
        else
            r = REG_SetBL;
        tmp = read_register(r) & 0x1F;
        if (val)
        {
            tmp |= (val << 5); // GAIN_ADJ_AA
        }
        write_register(tmp, r);
        break;
    case 5:
    case 6:
        if (a == 5)
            r = REG_SetAL;
        else
            r = REG_SetBL;
        tmp = read_register(r) & 0xE3;
        if (val)
        {
            tmp |= ((val << 2) & 0x1C); // BW_ADJ_AA
        }
        write_register(tmp, r);
        break;
    case 7:
    case 8:
        if (a == 7)
            r = REG_SetAL;
        else
            r = REG_SetBL;
        tmp = read_register(r) & 0xFC;
        if (val)
        {
            tmp |= (val & 0x03); // BW_SEL_LP
        }
        write_register(tmp, r);
        break;
    case 9:
        // wdt_reload();
        tmp = read_register(REG_SetPF) & 0x0F;
        if (val)
        {
            tmp |= (val << 4); // NPx
        }
        write_register(tmp, REG_SetPF);
        break;
    case 10:
        tmp = read_register(REG_SetPF) & 0xF8;
        if (val)
        {
            tmp |= (val & 0x07); // RPFx
        }
        write_register(tmp, REG_SetPF);
        break;
    case 11:
    case 12:
        if (a == 11)
            r = REG_SetAH;
        else
            r = REG_SetBH;
        write_register(val, r);
        break;
    case 14:
        // wdt_reload();
        tmp = read_register(REG_SetPF) & 0xF7;
        if (val)
        {
            tmp |= 0x08; // EN_DCCOMP
        }
        write_register(tmp, REG_SetPF);
        break;
    case 15:
        // wdt_reload();
        tmp = read_register(REG_Rst) & 0x0F;
        if (val)
        {
            tmp |= (val << 4); // DC_COMP_IC1
        }
        write_register(tmp, REG_Rst);
        break;
    case 16:
        // wdt_reload();
        tmp = read_register(REG_DCComp1) & 0xF0;
        if (val)
        {
            tmp |= (val & 0x0F); // DC_COMP_IC2
        }
        write_register(tmp, REG_DCComp1);
        break;
    case 17:
        // wdt_reload();
        tmp = read_register(REG_DCComp2) & 0x0F;
        if (val)
        {
            tmp |= (val << 4); // DC_COMP_IC3
        }
        write_register(tmp, REG_DCComp2);
        break;
    case 18:
        // wdt_reload();
        tmp = read_register(REG_DCComp2) & 0xF0;
        if (val)
        {
            tmp |= (val & 0x0F); // DC_COMP_IC4
        }
        write_register(tmp, REG_DCComp2);
        break;
    case 19:
        // wdt_reload();
        tmp = read_register(REG_Tabm) & 0x0F;
        if (val)
        {
            tmp |= (val << 4); // DC_COMP_IC5
        }
        write_register(tmp, REG_Tabm);
        break;
    case 20:
        // wdt_reload();
        tmp = read_register(REG_SetAna) & 0xF9;
        if (val)
        {
            tmp |= ((val << 1) & 0x06); // Tdc_pulse
        }
        write_register(tmp, REG_SetAna);
        break;
    default:
        return;
    }
}

unsigned short mlx_read_buffer[32];
unsigned char is_mlx_calibrated = 0;
int read_data_by_type(int type)
{
    int i = 0;
    signed int len = 0;

    mlx_read_buffer[3] = 0;
    if (type & 0x80)
        len = 1;
    if (type & 0x10)
        len += 3;
    if (type & 8)
        len += 3;
    if (type & 4)
        ++len;
    if (type & 2)
        ++len;
    if (type & 1)
        ++len;
    // wdt_reload();
    wait_dr();
    CS_SET;
    int tmp = (spi_tx_rx(0xC3) << 8) | spi_tx_rx(0);
    mlx_read_buffer[0] = ((tmp >> 8) & 0xFF);
    mlx_read_buffer[0] |= ((tmp << 8) & 0xFF00);
    if (len << 24)
    {
        do
        {
            mlx_read_buffer[i + 1] = (spi_tx_rx(0) << 8) | spi_tx_rx(0);
            i++;
        } while (i < len);
    }
    spi_tx_rx(0);
    CS_RESET;
    return 0;
}
void start_measurements(unsigned int a1)
{
    unsigned int v1 = a1 & 0xFFFF; // r4
    if ((a1 & 0xF000) == 0xD000)
        v1 = (a1 << 24 >> 31) ^ ((a1 << 24 >> 30) ^ (a1 << 24 >> 29) ^ (a1 << 24 >> 28) ^ (a1 << 24 >> 27) ^ (a1 << 24 >> 26) ^ (a1 << 24 >> 25)) & 1 | a1;
    // wdt_reload();
    wait_dr();
    CS_SET;
    spi_tx_rx(v1 >> 8);
    spi_tx_rx(v1 >> 0);
    CS_RESET;
}

int read_data(int a1)
{
    int v1; // r4
    int v2; // r1
            // 0x01, 0x02, 0x04 - ambient light sensors
            // 0x08 - LED B
            // 0x10 - LED A
            // 0x80 - temperature
    v1 = a1;
    v2 = a1 & 0x80; // temperature measurement
    if (a1 << 29)
        v2 |= 0x40u; // three ambient light channels
    if (a1 & 0x10)
        v2 |= 0x34u; // LED A
    if (a1 & 8)
        v2 |= 0x2Au; // LED B
    start_measurements(v2 | 0xD000);
    // wdt_reload();
    delay_soft(3);
    return read_data_by_type(v1);
}
typedef struct
{
    unsigned short dc_meas_before;
    unsigned short rain_burst_meas;
    unsigned short dc_meas_after;
} s_ir_ch;
static s_ir_ch ir_ch_a, ir_ch_b;
unsigned short dc_meas_diff_ch_a[2];
unsigned short dc_meas_diff_ch_b[2];
unsigned short dc_meas_ch_a_prev[2];
unsigned short dc_meas_ch_b_prev[2];
int mlx_read_data_by_id(e_read_data_type data, int probes, int max_diff)
{
    unsigned short diff = 0;
    unsigned int threshold = 250;

    int sum_buf[3] = {0, 0, 0};
    for (int i = 0; i < probes; i++)
    {
        // wdt_reload();
        read_data(data);
        diff = (mlx_read_buffer[3] >= mlx_read_buffer[1]) ? (mlx_read_buffer[3] - mlx_read_buffer[1]) : (mlx_read_buffer[1] - mlx_read_buffer[3]);
        if (is_mlx_calibrated)
        {
            if (diff >= max_diff)
                return 0;
        }
        if (mlx_read_buffer[2] <= 34000)
            return 0;
        sum_buf[0] += mlx_read_buffer[1];
        sum_buf[2] += mlx_read_buffer[3];
        sum_buf[1] += mlx_read_buffer[2];
    }
    if (data == LED_A)
    {
        ir_ch_a.dc_meas_before = sum_buf[0] / probes;
        ir_ch_a.rain_burst_meas = sum_buf[1] / probes;
        ir_ch_a.dc_meas_after = sum_buf[2] / probes;
        if (dc_meas_ch_a_prev[0] < ir_ch_a.dc_meas_before)
            dc_meas_diff_ch_a[0] = ir_ch_a.dc_meas_before - dc_meas_ch_a_prev[0];
        else
            dc_meas_diff_ch_a[0] = dc_meas_ch_a_prev[0] - ir_ch_a.dc_meas_before;
        if (dc_meas_ch_a_prev[1] < ir_ch_a.dc_meas_after)
            dc_meas_diff_ch_a[1] = ir_ch_a.dc_meas_after - dc_meas_ch_a_prev[1];
        else
            dc_meas_diff_ch_a[1] = dc_meas_ch_a_prev[1] - ir_ch_a.dc_meas_after;
        dc_meas_ch_a_prev[0] = ir_ch_a.dc_meas_before;
        dc_meas_ch_a_prev[1] = ir_ch_a.dc_meas_after;
        if (threshold >= dc_meas_diff_ch_a[0] && threshold >= dc_meas_diff_ch_a[1])
        {
            IR_Channel_A_data[5] = IR_Channel_A_data[4];
            IR_Channel_A_data[4] = IR_Channel_A_data[3];
            IR_Channel_A_data[3] = IR_Channel_A_data[2];
            IR_Channel_A_data[2] = IR_Channel_A_data[1];
            IR_Channel_A_data[1] = IR_Channel_A_data[0];
            IR_Channel_A_data[0] = ir_ch_a.rain_burst_meas;
            return ir_ch_a.rain_burst_meas;
        }
    }
    else if (data == LED_B)
    {
        ir_ch_b.dc_meas_before = sum_buf[0] / probes;
        ir_ch_b.rain_burst_meas = sum_buf[1] / probes;
        ir_ch_b.dc_meas_after = sum_buf[2] / probes;
        dc_meas_diff_ch_b[0] = (dc_meas_ch_b_prev[0] < ir_ch_b.dc_meas_before) ? (ir_ch_b.dc_meas_before - dc_meas_ch_b_prev[0]) : (dc_meas_ch_b_prev[0] - ir_ch_b.dc_meas_before);
        dc_meas_diff_ch_b[1] = (dc_meas_ch_b_prev[1] < ir_ch_b.dc_meas_after) ? (ir_ch_b.dc_meas_after - dc_meas_ch_b_prev[1]) : (dc_meas_ch_b_prev[1] - ir_ch_b.dc_meas_after);
        dc_meas_ch_b_prev[0] = ir_ch_b.dc_meas_before;
        dc_meas_ch_b_prev[1] = ir_ch_b.dc_meas_after;
        if (threshold >= dc_meas_diff_ch_b[0] && threshold >= dc_meas_diff_ch_b[1])
        {
            IR_Channel_B_data[5] = IR_Channel_B_data[4];
            IR_Channel_B_data[4] = IR_Channel_B_data[3];
            IR_Channel_B_data[3] = IR_Channel_B_data[2];
            IR_Channel_B_data[2] = IR_Channel_B_data[1];
            IR_Channel_B_data[1] = IR_Channel_B_data[0];
            IR_Channel_B_data[0] = ir_ch_b.rain_burst_meas;
            return ir_ch_b.rain_burst_meas;
        }
    }
    else
    {
        return sum_buf[1] / probes;
    }
    return 0;
}

void mlx_read_ambient_light()
{
    int sum_array[3] = {0, 0, 0};
    for (int i = 0; i < 2; i++)
    {
        start_measurements(0xD040u); // M5: setting this bit high enables the read-out of the three ambient light channels
        // wdt_reload();
        delay_soft(3);
        mlx_read_buffer[3] = 0;
        // wdt_reload();
        wait_dr();
        CS_SET;
        mlx_read_buffer[0] = spi_tx_rx(0xC3) & 0xFF;
        mlx_read_buffer[0] |= (spi_tx_rx(0) << 8);
        mlx_read_buffer[1] = (spi_tx_rx(0) << 8) | spi_tx_rx(0);
        mlx_read_buffer[2] = (spi_tx_rx(0) << 8) | spi_tx_rx(0);
        mlx_read_buffer[3] = (spi_tx_rx(0) << 8) | spi_tx_rx(0);
        spi_tx_rx(0);
        CS_RESET;
        sum_array[0] += mlx_read_buffer[1];
        sum_array[1] += mlx_read_buffer[2];
        sum_array[2] += mlx_read_buffer[3];
    }
    for (int i = 0; i < TOTAL_AMB_LIGHT_CHANNELS; i++)
    {
        Ambient_light_channel_data[i] = sum_array[i] >> 1;
        AmbientLIghtLevelsScaled[i] = LookupTable2d_uint16_uint32(Ambient_light_channel_data[i], TBL_RawAmbientLightData[i]);
    }
}

void mlx_calibration(void)
{
    IR_Channel_data_SRC = 0;
    for (int i = 0; i < 20; i++)
    {
        IR_Channel_data_SRC = mlx_read_data_by_id(LED_A, 1, 1000);
        if (IR_Channel_data_SRC)
        {
            break;
        }
    }
    if (IR_Channel_data_SRC < 44500)
    {
        for (int i = 0; i < 254; i++)
        {
            regs_proc(11, ++dac_level_ir_ch_a);
            delay_soft(5);
            IR_Channel_data_SRC = mlx_read_data_by_id(LED_A, 3, 1000);
            if (((IR_Channel_data_SRC - 44500) <= 1000) || (dac_level_ir_ch_a >= 240))
                break;
        }
        if (dac_level_ir_ch_a > 240)
        {
            dac_level_ir_ch_a = 240;
        }
        regs_proc(11, dac_level_ir_ch_a);
    }
    else if (IR_Channel_data_SRC >= 45500)
    {
        for (int i = 0; i < 254; i++)
        {
            regs_proc(11, --dac_level_ir_ch_a);
            delay_soft(5);
            IR_Channel_data_SRC = mlx_read_data_by_id(LED_A, 3, 1000);
            if (((IR_Channel_data_SRC - 44500) <= 1000) || (dac_level_ir_ch_a <= 16))
                break;
        }
        if (dac_level_ir_ch_a < 16)
        {
            dac_level_ir_ch_a = 16;
        }
        regs_proc(11, dac_level_ir_ch_a);
    }
    IR_Channel_A_data[0] = IR_Channel_data_SRC;
    IR_Channel_A_data[1] = IR_Channel_data_SRC;
    IR_Channel_A_data[2] = IR_Channel_data_SRC;
    IR_Channel_A_data[3] = IR_Channel_data_SRC;
    IR_Channel_A_data[4] = IR_Channel_data_SRC;
    IR_Channel_A_data[5] = IR_Channel_data_SRC;

    IR_Channel_data_SRC = 0;
    for (int i = 0; i < 20; i++)
    {
        IR_Channel_data_SRC = mlx_read_data_by_id(LED_B, 1, 1000);
        if (IR_Channel_data_SRC)
        {
            break;
        }
    }
    if (IR_Channel_data_SRC < 44500)
    {
        for (int i = 0; i < 254; i++)
        {
            regs_proc(12, ++dac_level_ir_ch_b);
            delay_soft(5);
            IR_Channel_data_SRC = mlx_read_data_by_id(LED_B, 3, 1000);
            if (((IR_Channel_data_SRC - 44500) <= 1000) || (dac_level_ir_ch_b >= 240))
                break;
        }
        if (dac_level_ir_ch_b > 240)
        {
            dac_level_ir_ch_b = 240;
        }
        regs_proc(12, dac_level_ir_ch_b);
    }
    else if (IR_Channel_data_SRC >= 45500)
    {
        for (int i = 0; i < 254; i++)
        {
            regs_proc(12, --dac_level_ir_ch_b);
            delay_soft(5);
            IR_Channel_data_SRC = mlx_read_data_by_id(LED_B, 3, 1000);
            if (((IR_Channel_data_SRC - 44500) <= 1000) || (dac_level_ir_ch_b <= 16))
                break;
        }
        if (dac_level_ir_ch_b < 16)
        {
            dac_level_ir_ch_b = 16;
        }
        regs_proc(12, dac_level_ir_ch_b);
    }
    IR_Channel_B_data[0] = IR_Channel_data_SRC;
    IR_Channel_B_data[1] = IR_Channel_data_SRC;
    IR_Channel_B_data[2] = IR_Channel_data_SRC;
    IR_Channel_B_data[3] = IR_Channel_data_SRC;
    IR_Channel_B_data[4] = IR_Channel_data_SRC;
    IR_Channel_B_data[5] = IR_Channel_data_SRC;

    is_mlx_calibrated = 1;
}
void mlx_init(void)
{
    wait_dr();
    CS_SET;
    spi_tx_rx(0xF0); // Chip reset
    spi_tx_rx(0);
    CS_RESET;
    wait_dr();
    delay_soft(2);
    wait_dr();
    CS_SET;
    spi_tx_rx(0x87); // Write Register
    spi_tx_rx(0xFF);
    spi_tx_rx(0xD4);
    CS_RESET;
    delay_soft(5);

    regs_proc(9, 11);
    regs_proc(1, 2);
    regs_proc(20, 2);
    regs_proc(5, 0);
    regs_proc(6, 0);
    regs_proc(7, 3);
    regs_proc(11, /*212*/ dac_level_ir_ch_a);
    regs_proc(3, 0);
    regs_proc(8, 3);
    regs_proc(12, /*197*/ dac_level_ir_ch_b);
    regs_proc(4, 0);
    regs_proc(10, 4);
    regs_proc(15, 3);
    regs_proc(16, 5);
    regs_proc(17, 4);
    regs_proc(18, 2);
    regs_proc(19, 1);
    regs_proc(14, 1);

    IR_Channel_A_data[0] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[0] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    IR_Channel_A_data[1] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[1] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    IR_Channel_A_data[2] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[2] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    IR_Channel_A_data[3] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[3] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    IR_Channel_A_data[4] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[4] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    IR_Channel_A_data[5] = mlx_read_data_by_id(LED_A, 3u, 0x3E8u);
    IR_Channel_B_data[5] = mlx_read_data_by_id(LED_B, 3u, 0x3E8u);
    if (is_mlx_calibrated == 0)
    {
        for (int i = 0; i < 128; i++)
        {
            mlx_calibration();
        }
    }
}