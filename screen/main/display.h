#ifndef DISPLAY_H
#define DISPLAY_H

#include "driver/i2c.h"

void init_display(void);
TickType_t test_display(char text[]);
TickType_t display_textarea(char *strings[], int num_strings);
TickType_t display_floatarea(float floats[], int num_floats);
TickType_t display_textline(char text[]);

#endif /* DISPLAY_H */