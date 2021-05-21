#ifndef BLINK_H_
#define BLINK_H_

#ifdef TEST_MODE
static const int ledpin = 13;
#endif

void blink(void*);
void blink_stop();

#endif