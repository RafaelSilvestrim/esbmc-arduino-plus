#include "arduino_liquidcrystal.h"

int main() {
    // ==== Caso 1: uso correto ====
    LiquidCrystal_begin(16, 2);
    LiquidCrystal_setCursor(5, 1);
    LiquidCrystal_print("Hello");
    LiquidCrystal_clear();

    // ==== Caso 2: sem inicialização (violação) ====
    // comente o bloco acima e habilite esta linha para ver o erro:
    // LiquidCrystal_setCursor(0, 0);

    // ==== Caso 3: posição inválida (violação) ====
    // habilite apenas estas linhas para ver:
    // LiquidCrystal_begin(16, 2);
    // LiquidCrystal_setCursor(20, 0);

    return 0;
}
