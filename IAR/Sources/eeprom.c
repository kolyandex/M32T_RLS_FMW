#include "eeprom.h"
#include "string.h"

#define EEPROM_START_ADDR 0x10000000
#define EEPROM_END_ADDR 0x100000FF

void eeprom_init(int freq)
{
    if (!(FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_CCIF_MASK))
        return;
    if ((FTMRH_BASE_PTR->FCLKDIV & FTMRH_FCLKDIV_FDIVLCK_MASK) == 0)
    {
        FTMRH_BASE_PTR->FCLKDIV = (((freq / 1000000) - 1) & FTMRH_FCLKDIV_FDIV_MASK) | (FTMRH_BASE_PTR->FCLKDIV & (FTMRH_FCLKDIV_FDIVLD_MASK | FTMRH_FCLKDIV_FDIVLCK_MASK));
    }
}

static int eeprom_erase_sector(int addr)
{
    int result = 0;
    FTMRH_BASE_PTR->FSTAT = FTMRH_FSTAT_FPVIOL_MASK | FTMRH_FSTAT_ACCERR_MASK;
    FTMRH_BASE_PTR->FCCOBIX = 0;
    FTMRH_BASE_PTR->FCCOBHI = 0x12; // Erase EEPROM Sector
    FTMRH_BASE_PTR->FCCOBLO = addr >> 16;
    FTMRH_BASE_PTR->FCCOBIX = 1;
    FTMRH_BASE_PTR->FCCOBHI = addr >> 8;
    FTMRH_BASE_PTR->FCCOBLO = addr >> 0;
    MCM_BASE_PTR->PLACR |= MCM_PLACR_ESFC_MASK; // Enable Stalling Flash Controller
    FTMRH_BASE_PTR->FSTAT = FTMRH_FSTAT_CCIF_MASK;
    while (!(FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_CCIF_MASK))
        ;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
        result = FTMRH_FSTAT_ACCERR_MASK;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
        result |= FTMRH_FSTAT_FPVIOL_MASK;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_MGSTAT_MASK)
        result |= FTMRH_FSTAT_MGSTAT_MASK;
    return result;
}

static int eeprom_program_2_byte(int addr, unsigned char *data)
{
    int result = 0;
    FTMRH_BASE_PTR->FSTAT = FTMRH_FSTAT_FPVIOL_MASK | FTMRH_FSTAT_ACCERR_MASK;
    FTMRH_BASE_PTR->FCCOBIX = 0;
    FTMRH_BASE_PTR->FCCOBHI = 0x11;
    FTMRH_BASE_PTR->FCCOBLO = addr >> 16;
    FTMRH_BASE_PTR->FCCOBIX = 1;
    FTMRH_BASE_PTR->FCCOBHI = addr >> 8;
    FTMRH_BASE_PTR->FCCOBLO = addr >> 0;
    FTMRH_BASE_PTR->FCCOBIX = 2;
    FTMRH_BASE_PTR->FCCOBLO = data[0];
    FTMRH_BASE_PTR->FCCOBIX = 3;
    FTMRH_BASE_PTR->FCCOBLO = data[1];
    MCM_BASE_PTR->PLACR |= MCM_PLACR_ESFC_MASK; // Enable Stalling Flash Controller
    FTMRH_BASE_PTR->FSTAT = FTMRH_FSTAT_CCIF_MASK;
    while (!(FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_CCIF_MASK))
        ;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_ACCERR_MASK)
        result = FTMRH_FSTAT_ACCERR_MASK;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
        result |= FTMRH_FSTAT_FPVIOL_MASK;
    if (FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_MGSTAT_MASK)
        result |= FTMRH_FSTAT_MGSTAT_MASK;
    return result;
}

int eeprom_read(int addr, unsigned char *data, int len)
{
    if ((len == 0) || (len & 1) || (addr & 1) || (addr < EEPROM_START_ADDR) || ((addr + len) > (EEPROM_END_ADDR + 1)))
    {
        return -1;
    }

    for (int a = addr; a < addr + len; a += 2, data += 2)
    {
        while (!(FTMRH_BASE_PTR->FSTAT & FTMRH_FSTAT_CCIF_MASK))
            ;
        data[0] = *(unsigned char *)(a + 0);
        data[1] = *(unsigned char *)(a + 1);
    }
    return 0;
}
int eeprom_write(int addr, unsigned char *data, int len)
{
    int res = 0;
    if ((len == 0) || (len & 1) || (addr & 1) || (addr < EEPROM_START_ADDR) || ((addr + len) > (EEPROM_END_ADDR + 1)))
    {
        res = -1;
    }
    else
    {
        for (int a = addr; a < addr + len; a += 2, data += 2)
        {
            res = eeprom_erase_sector(a);
            if (res)
            {
                break;
            }
            res = eeprom_program_2_byte(a, data);
            if (res)
            {
                break;
            }
        }
    }
    return res;
}