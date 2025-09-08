#include "sma.h"
#include "stdint.h"

void SMA_InitFlt(s_sma_flt *ctx, float initial_val, unsigned int counts)
{
    ctx->val = initial_val;
    ctx->counts = counts;
    ctx->counter = 0;
}
float SMA_GetFlt(s_sma_flt *ctx, float val)
{
    float diff = val - ctx->val;
    if (ctx->counter < UINT32_MAX)
    {
        ctx->counter++;
    }
    if (ctx->counts != 0)
    {
        //if (ctx->counter >= ctx->counts)
        {
            ctx->counter = ctx->counts;
        }
    }
    if (ctx->counter == 0)
    {
        ctx->counter = 1;
    }
    ctx->val += diff / (float)ctx->counter;
    return ctx->val;
}