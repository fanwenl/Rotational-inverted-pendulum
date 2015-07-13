#ifndef __GUI_H__
#define __GUI_H__

#include "stm32f4xx.h"

#define CENTER_X	50
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

void gui_drawIcon(u16 x, u16 y, char *name, u16 color);
void gui_drawHome(void);
u8 isInRange(u16 xPos, u16 yPos, u16 appx, u16 appy);
void side_to_side(void);
void starts_swinging(void);
void inverted_pendulum(void);
void circular_motion(void);

#endif
