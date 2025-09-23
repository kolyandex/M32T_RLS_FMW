#pragma once
#include "derivative.h"

#define AUTO_WIPERS_THRESOLD_MODES_COUNT 4

void a_wipe_sleep(void);
void a_wipe_rtc_wakeup(void);
void a_wipe_init(void);
void a_wipe_poll_100ms(void);