#pragma once

typedef struct
{
    float val;
    unsigned int counts;
    unsigned int counter;
}s_sma_flt;

void SMA_InitFlt(s_sma_flt * ctx, float initial_val, unsigned int counts);
float SMA_GetFlt(s_sma_flt *ctx, float val, int counts);
void SMA_SetFlt(s_sma_flt *ctx, float val);