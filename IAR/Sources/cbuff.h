#pragma once

#include <stdint.h>

typedef struct
{
  uint8_t *buff;
  uint16_t buff_size;
  uint16_t buff_mask;
  uint16_t in_idx;
  uint16_t out_idx;
} cbuff_t;

void cbuff_init(cbuff_t *const cbuff, uint8_t *const mem, const uint16_t mem_size);

void write_to_cbuff(cbuff_t *const cbuf, uint8_t *const src, const uint16_t len);

void read_from_cbuff(cbuff_t *const cbuff, uint8_t *const dst, const uint16_t len);
