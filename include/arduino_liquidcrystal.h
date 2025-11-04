/**
 * arduino_liquidcrystal.h
 * ESBMC-Arduino Plus – Modelos Operacionais para LiquidCrystal LCD
 */

#ifndef ARDUINO_LIQUIDCRYSTAL_H
#define ARDUINO_LIQUIDCRYSTAL_H

#define LCD_MAX_COLS 20
#define LCD_MAX_ROWS 4

extern int lcd_initialized;
extern int lcd_cols;
extern int lcd_rows;
extern int lcd_cursor_col;
extern int lcd_cursor_row;

/**
 * Inicializa o display LCD com colunas e linhas.
 * Pré-condições: 
 *    cols > 0 && cols <= LCD_MAX_COLS, 
 *    rows > 0 && rows <= LCD_MAX_ROWS.
 */
void LiquidCrystal_begin(int cols, int rows);

/**
 * Limpa o display. 
 * Pré-condição: display inicializado.
 */
void LiquidCrystal_clear(void);

/**
 * Posiciona o cursor em (col, row). 
 * Pré-condição: display inicializado e posição válida.
 */
void LiquidCrystal_setCursor(int col, int row);

/**
 * Imprime uma string no display. 
 * Pré-condição: display inicializado, msg != NULL.
 */
void LiquidCrystal_print(const char* msg);

#endif // ARDUINO_LIQUIDCRYSTAL_H
