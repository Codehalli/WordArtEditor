/*
 * LCD_display_HAL.c
 * Pra
 */

#include "LCD_display_HAL.h"
#include "edit.h"

// This function is not used in upper level functions, so it does not need to be declared in the graphics_HAL.h
void init_fonts() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void init_graphics(edit_t *edit) {
    Graphics_initContext(&edit->g_sGraphicInfo,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&edit->g_sGraphicInfo, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(&edit->g_sGraphicInfo, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&edit->g_sGraphicInfo, &g_sFontCmss12);

    init_fonts();

    Graphics_clearDisplay(&edit->g_sGraphicInfo);
}

void lcd_draw_string(edit_t * edit)
{
    Graphics_Context *g_sGraphicInfo = &edit->g_sGraphicInfo;
  Graphics_clearDisplay(g_sGraphicInfo);
  Graphics_drawString(g_sGraphicInfo, edit->edit, -1, edit->position_X,edit->position_Y, true);

}
