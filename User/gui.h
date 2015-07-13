#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>

#define CENTER_X	60
#define CENTER_Y	150

#define OFF_X	40
#define OFF_Y	70

#define APP1_X	(CENTER_X)
#define APP1_Y	(CENTER_Y-2*OFF_Y)

#define APP2_X	(CENTER_X)
#define APP2_Y	(CENTER_Y-OFF_Y)

#define APP3_X	(CENTER_X)
#define APP3_Y	(CENTER_Y)

#define APP4_X	(CENTER_X)
#define APP4_Y	(CENTER_Y+1*OFF_Y)

void gui_drawIcon(uint16_t x, uint16_t y, char *name, uint16_t color);
void gui_drawHome(void);
uint8_t isInRange(uint16_t xPos, uint16_t yPos, uint16_t appx, uint16_t appy);
void side_to_side(void);
void starts_swinging(void);
void inverted_pendulum(void);
void circular_motion(void);

#endif
