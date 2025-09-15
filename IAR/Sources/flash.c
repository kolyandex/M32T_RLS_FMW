#include "flash.h"
#include "intrinsics.h"
#include "derivative.h" /* include peripheral declarations */

#define BUS_CLK_MHZ(freq) ((freq) / 1000000L)

#define FCLKDIV_FDIV_STATUS() ((FTMRH_FCLKDIV & FTMRH_FCLKDIV_FDIVLD_MASK) >> FTMRH_FCLKDIV_FDIVLD_SHIFT)
#define FSTAT_CCIF_STATUS() ((FTMRH_FSTAT & FTMRH_FSTAT_CCIF_MASK) >> FTMRH_FSTAT_CCIF_SHIFT)
#define FSTAT_CCIF_EFPVIOL() ((FTMRH_FSTAT & FTMRH_FSTAT_FPVIOL_MASK) >> FTMRH_FSTAT_FPVIOL_SHIFT)
#define FSTAT_CCIF_EACCESS() ((FTMRH_FSTAT & FTMRH_FSTAT_ACCERR_MASK) >> FTMRH_FSTAT_ACCERR_SHIFT)

#define FCCOBHI_setHI_or_break(_data, _i, _len) { \
    FTMRH_FCCOBHI = (_data)[(_i)];                \
                                                  \
    (_i)++;                                       \
    (_len)--;                                     \
                                                  \
    if ((_len) == 0)                              \
    {                                             \
      FTMRH_FCCOBLO = 0xFFu;                      \
      break;                                      \
    }                                             \
  }

#define FCCOBHI_setLO_or_break(_data, _i, _len) { \
    FTMRH_FCCOBLO = (_data)[(_i)];                \
                                                  \
    (_i)++;                                       \
    (_len)--;                                     \
                                                  \
    if ((_len) == 0)                              \
    {                                             \
      break;                                      \
    }                                             \
  }

enum
{
  ERASE_VERIFY_ALL_BLOCKS = 0x01,
  ERASE_VERIFY_BLOCK = 0x02,
  ERASE_VERIFY_FLASH_SECTION = 0x03,
  READ_ONCE = 0x04,
  PROGRAM_FLASH = 0x06,
  PROGRAM_ONCE = 0x07,
  ERASE_ALL_BLOCK = 0x08,
  ERASE_BLOCK = 0x09,
  ERASE_FLASH_SECTOR = 0x0A,
  UNSECURE = 0x0B,
  VERIFY_BACKDOOR_ACCESS_KEY = 0x0C,
  SET_USER_MARGIN_LEVEL = 0x0D,
  SET_FACTORY_MARGIN_LEVEL = 0x0E,
  ERASE_VERIFY_EEPROM_SECTION = 0x10,
  PROGRAM_EEPROM = 0x11,
  ERASE_EEPROM_SECTOR = 0x12,
};

uint16_t flash_init(uint32_t freq)
{
  uint16_t err;

  do
  {
    if (FSTAT_CCIF_STATUS() == 1U)
    {
      err = FLASH_ERR_INIT_CCIF;
      break;
    }

    if ((FTMRH_FCLKDIV & FTMRH_FCLKDIV_FDIVLCK_MASK) != 0U)
    {
      err = FLASH_ERR_INIT_FDIV;
      break;
    }

    /* Clock Divide Value Set */
    FTMRH_FCLKDIV = FTMRH_FCLKDIV_FDIV(BUS_CLK_MHZ(freq) - 1);

    if (FCLKDIV_FDIV_STATUS() == 0U)
    {
      err = FLASH_ERR_INIT_FDIV;
      break;
    }

    err = FLASH_ERR_SUCCESS;
  } while (0);

  return err;
}

#ifdef IAR
static void __ramfunc flash_launch_cmd(void)
#else
static void flash_launch_cmd(void)
#endif
{
  __disable_interrupt();

  /* Start transferring the content of FCCOB to flash memory controller */
  FTMRH_FSTAT = FTMRH_FSTAT_CCIF_MASK;
  /* Upon completion, the memory controller will return FSTAT[CCIF] to 1 */
  while (!FSTAT_CCIF_STATUS()) {}

 __enable_interrupt();
}

static uint16_t erase_sector(uint8_t cmd, uint32_t addr)
{
  uint16_t err;

  do
  {
    /* FCCOB Availability Check */
    if (FSTAT_CCIF_STATUS() == 0U)
    {
      err = FLASH_ERR_BUSY;
      break;
    }

    /* Write index to specify the command code to be loaded */
    FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b000);
    FTMRH_FCCOBHI = cmd;
    /* Global address [23:16] */
    FTMRH_FCCOBLO = (uint8_t)((addr & 0xFF0000) >> 16);

    FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b001);
    /* Global address [15:8] */
    FTMRH_FCCOBHI = (uint8_t)((addr & 0xFF00) >> 8);
    /* Global address [7:0] */
    FTMRH_FCCOBLO = (uint8_t)(addr & 0xFF);

    flash_launch_cmd();

    err = FLASH_ERR_SUCCESS;

    /* Check error status */
    if (FTMRH_FSTAT & FTMRH_FSTAT_ACCERR_MASK)
    {
      err |= FLASH_ERR_ACCESS;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
    {
      err |= FLASH_ERR_PROTECTION;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
    {
      err |= FLASH_ERR_MGSTAT0;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
    {
      err |= FLASH_ERR_MGSTAT1;
    }
  } while (0);

  return err;
}

uint16_t flash_erase_sector(uint32_t addr)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment by sector */
    if (addr & (FLASH_SECTOR_SIZE - 1))
    {
      break;
    }

    /* check bounds */
    if (!(addr >= FLASH_START_ADDR && (addr + FLASH_SECTOR_SIZE) <= FLASH_END_ADDR))
    {
      break;
    }

    err = erase_sector(ERASE_FLASH_SECTOR, addr);
  } while (0);

  return err;
}

uint16_t eeprom_erase_sector(uint32_t addr)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment by sector */
    if (addr & (EEPROM_SECTOR_SIZE - 1))
    {
      break;
    }

    /* check bounds */
    if (!(addr >= EEPROM_START_ADDR && (addr + EEPROM_SECTOR_SIZE) <= EEPROM_END_ADDR))
    {
      break;
    }

    err = erase_sector(ERASE_EEPROM_SECTOR, addr);
  } while (0);

  return err;
}

static uint16_t write_bytes(uint8_t cmd, uint32_t addr, const unsigned char * data, uint16_t len)
{
  uint8_t  i = 0;
  uint16_t err;

  do
  {
    if (len == 0)
    {
      err = FLASH_ERR_INVALID_PARAM;
      break;
    }

    if (len > 8)
    {
      err = FLASH_ERR_INVALID_PARAM;
      break;
    }

    /* FCCOB Availability Check */
    if (FSTAT_CCIF_STATUS() == 0U)
    {
      err = FLASH_ERR_BUSY;
      break;
    }

    /* Write index to specify the command code to be loaded */
    FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b000);
    FTMRH_FCCOBHI = cmd;
    /* Global address [23:16] */
    FTMRH_FCCOBLO = (uint8_t)((addr & 0xFF0000) >> 16);

    FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b001);
    /* Global address [15:8] */
    FTMRH_FCCOBHI = (uint8_t)((addr & 0xFF00) >> 8);
    /* Global address [7:0] */
    FTMRH_FCCOBLO = (uint8_t)(addr & 0xFF);

    do
    {
      FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b010);
      /* Data 0 [15:8] */
      FCCOBHI_setHI_or_break(data, i, len);
      /* Data 0 [7:0] */
      FCCOBHI_setLO_or_break(data, i, len);

      FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b011);
      /* Data 1 [15:8] */
      FCCOBHI_setHI_or_break(data, i, len);
      /* Data 1 [7:0] */
      FCCOBHI_setLO_or_break(data, i, len);

      FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b100);
      /* Data 2 [15:8] */
      FCCOBHI_setHI_or_break(data, i, len);
      /* Data 2 [7:0] */
      FCCOBHI_setLO_or_break(data, i, len);

      FTMRH_FCCOBIX = FTMRH_FCCOBIX_CCOBIX(0b101);
      /* Data 3 [15:8] */
      FCCOBHI_setHI_or_break(data, i, len);
      /* Data 3 [7:0] */
      FCCOBHI_setLO_or_break(data, i, len);
    } while (0);

    flash_launch_cmd();

    err = FLASH_ERR_SUCCESS;

    /* Check error status */
    if (FTMRH_FSTAT & FTMRH_FSTAT_ACCERR_MASK)
    {
      err |= FLASH_ERR_ACCESS;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_FPVIOL_MASK)
    {
      err |= FLASH_ERR_PROTECTION;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_MGSTAT0_MASK)
    {
      err |= FLASH_ERR_MGSTAT0;
    }

    if (FTMRH_FSTAT & FTMRH_FSTAT_MGSTAT1_MASK)
    {
      err |= FLASH_ERR_MGSTAT1;
    }
  } while (0);

  return err;
}

uint16_t flash_write(uint32_t addr, const unsigned char *data, uint16_t len)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment */
    if (addr & (FLASH_ALIGNMENT - 1))
    {
      break;
    }

    /* check bounds */
    if (!(addr >= FLASH_START_ADDR && (addr + len) <= FLASH_END_ADDR))
    {
      break;
    }

    for (uint32_t a = addr; a < (addr + len); a += 8u, len -= 8u)
    {
      if ((err = write_bytes(PROGRAM_FLASH, a, data + (addr - a), (len > 8u ? 8u : len))) != FLASH_ERR_SUCCESS)
      {
        break;
      }
    }
  } while (0);

  return err;
}

uint16_t flash_program(uint32_t addr, const unsigned char *data, uint16_t len)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment by sector */
    if (addr & (FLASH_SECTOR_SIZE - 1))
    {
      break;
    }

    if (len == 0u)
    {
      break;
    }

    /* check bounds */
    if (!(addr >= FLASH_START_ADDR && (addr + len) <= FLASH_END_ADDR))
    {
      break;
    }

    for (uint32_t a = addr; a < (addr + len); a += FLASH_SECTOR_SIZE, len -= FLASH_SECTOR_SIZE)
    {
      if ((err = flash_erase_sector(a)) != FLASH_ERR_SUCCESS)
      {
        break;
      }

      if ((err = flash_write(a, data + (addr - a), (len > FLASH_SECTOR_SIZE ? FLASH_SECTOR_SIZE : len))) != FLASH_ERR_SUCCESS)
      {
        break;
      }
    }
  } while (0);

  return err;
}

uint16_t eeprom_program(uint32_t addr, const unsigned char *data, uint16_t len)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment by sector */
    if (addr & (EEPROM_SECTOR_SIZE - 1))
    {
      break;
    }

    if (len == 0u)
    {
      break;
    }

    /* check bounds */
    if (!(addr >= EEPROM_START_ADDR && (addr + len) <= EEPROM_END_ADDR))
    {
      break;
    }

    for (uint32_t a = addr; a < (addr + len); a += EEPROM_SECTOR_SIZE, len -= EEPROM_SECTOR_SIZE)
    {
      if ((err = eeprom_erase_sector(a)) != FLASH_ERR_SUCCESS)
      {
        break;
      }

      if ((err = write_bytes(PROGRAM_EEPROM, a, data + (addr - a), (len > EEPROM_SECTOR_SIZE ? EEPROM_SECTOR_SIZE : len))) != FLASH_ERR_SUCCESS)
      {
        break;
      }
    }
  } while (0);

  return err;
}

uint16_t eeprom_read(uint32_t addr, unsigned char *data, uint16_t len)
{
  uint16_t err = FLASH_ERR_INVALID_PARAM;

  do
  {
    /* check alignment by sector */
    if (addr & (EEPROM_SECTOR_SIZE - 1))
    {
      break;
    }

    if (len == 0u)
    {
      break;
    }

    /* check bounds */
    if (!(addr >= EEPROM_START_ADDR && (addr + len) <= EEPROM_END_ADDR))
    {
      break;
    }

    for (uint32_t a = addr; a < (addr + len); a += EEPROM_SECTOR_SIZE, len -= EEPROM_SECTOR_SIZE)
    {
      while (!(FTMRH_FSTAT & FTMRH_FSTAT_CCIF_MASK))
        ;

      data[0] = *(unsigned char *)(a + 0);
      data[1] = *(unsigned char *)(a + 1);
    }
  } while (0);

  return err;
}
