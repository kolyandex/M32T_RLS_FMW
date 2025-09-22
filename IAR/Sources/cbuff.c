#include "cbuff.h"

void cbuff_init(cbuff_t *const cbuff, uint8_t *const mem, const uint16_t mem_size)
{
  if (mem_size < 4 || !((mem_size != 0) && !(mem_size & (mem_size - 1))))
  {
    for (;;)
      ;
  }
  cbuff->buff = mem;
  cbuff->buff_size = mem_size;
  cbuff->buff_mask = mem_size - 1;
  cbuff->in_idx = 0;
  cbuff->out_idx = 0;
}

void write_to_cbuff(cbuff_t *const cbuff, uint8_t *const src, const uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
  {
    cbuff->buff[cbuff->in_idx] = src[i];
    cbuff->in_idx++;
    cbuff->in_idx &= cbuff->buff_mask;
  }
}
void read_from_cbuff(cbuff_t *const cbuff, uint8_t *const dst, const uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
  {
    dst[i] = cbuff->buff[cbuff->out_idx];
    cbuff->out_idx++;
    cbuff->out_idx &= cbuff->buff_mask;
  }
}
