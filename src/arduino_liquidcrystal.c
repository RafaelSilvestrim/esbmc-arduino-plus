/**
 * arduino_liquidcrystal.c
 * ESBMC-Arduino Plus – Modelos Operacionais para LiquidCrystal LCD
 */

#include "arduino_liquidcrystal.h"
#include <esbmc.h>

int lcd_initialized = 0;
int lcd_cols = 0;
int lcd_rows = 0;
int lcd_cursor_col = 0;
int lcd_cursor_row = 0;

void LiquidCrystal_begin(int cols, int rows) {
    __ESBMC_assert(cols > 0 && cols <= LCD_MAX_COLS,
        "LiquidCrystal_begin: cols inválido");
    __ESBMC_assert(rows > 0 && rows <= LCD_MAX_ROWS,
        "LiquidCrystal_begin: rows inválido");
    lcd_cols = cols;
    lcd_rows = rows;
    lcd_initialized = 1;
}

void LiquidCrystal_clear(void) {
    __ESBMC_assert(lcd_initialized,
        "LiquidCrystal_clear: display não inicializado");
}

void LiquidCrystal_setCursor(int col, int row) {
    __ESBMC_assert(lcd_initialized,
        "LiquidCrystal_setCursor: display não inicializado");
    __ESBMC_assert(col >= 0 && col < lcd_cols,
        "LiquidCrystal_setCursor: col fora de intervalo");
    __ESBMC_assert(row >= 0 && row < lcd_rows,
        "LiquidCrystal_setCursor: row fora de intervalo");
    lcd_cursor_col = col;
    lcd_cursor_row = row;
}

void LiquidCrystal_print(const char* msg) {
    __ESBMC_assert(lcd_initialized,
        "LiquidCrystal_print: display não inicializado");
    __ESBMC_assert(msg != 0,
        "LiquidCrystal_print: msg NULL");
    // Simulação de impressão (no-op)
}
