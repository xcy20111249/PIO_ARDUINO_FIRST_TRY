#ifndef EPDUTILS_H_
#define EPDUTILS_H_


#include <SPI.h>
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "config_test.h"

#define gauche 0
#define milieu 1
#define droit 2

void epd_init();
int EPD_set_header (Epd epd, std::string text, int pos);
int EPD_set_header (Epd epd, std::string text, int pos, int bgc);
int EPD_set_footer (Epd epd, std::string text, int pos);
int EPD_set_footer (Epd epd, std::string text, int pos, int bgc);
void EPD_loop (void *);


#endif