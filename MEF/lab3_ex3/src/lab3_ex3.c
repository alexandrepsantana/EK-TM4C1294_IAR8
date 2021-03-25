#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "driverlib/systick.h"
#include "driverleds.h" // Projects/drivers

// MEF com apenas 2 estados e 1 evento temporal que alterna entre eles
// Seleção por evento

typedef enum {S000, S001, S011, S010, S110, S111, S101, S100} state_t;

volatile uint8_t Evento = 0;

void SysTick_Handler(void){
  Evento = 1;
} // SysTick_Handler

void main(void){
  static state_t Estado = S000; // estado inicial da MEF
  
  LEDInit(LED1); // Inicializa porta digital do led 1
  LEDInit(LED2); // Inicializa porta digital do led 2
  LEDInit(LED3); // Inicializa porta digital do led 3
  
  // Inicializa gongo do "relogio" (SysTick_Handler()).
  SysTickPeriodSet(24000000); // f = 1Hz para clock = 24MHz
  SysTickIntEnable();
  SysTickEnable();

  // Fica dentro do while para sempre.
  while(1) {    
    if (Evento == 0) {
      continue;
    }
    
    // Soh passa para baixo desta linha no gongo do relogio (no tempo do SysTick_Handler()).

    // Soh LED 1 (bit mais significativo).
    if (Estado == S000 || Estado == S001 || Estado == S010 || Estado == S011) {
      LEDOff(LED1);
    } else {
      LEDOn(LED1);
    }
    
    // Soh LED 2.
    if (Estado == S000 || Estado == S001 || Estado == S100 || Estado == S101) {
      LEDOff(LED2);
    } else {
      LEDOn(LED2);
    }
    
    // Soh LED 3.
    if (Estado == S000 || Estado == S010 || Estado == S110 || Estado == S100) {
      LEDOff(LED3);
    } else {
      LEDOn(LED3);
    }
    
    // Especifico o proximo Estado, abaixo.
    if (Estado == S100 /*se for o ultimo...*/) {
      Estado = S000; //... volto para o primeiro
    } else {
      Estado++; //... senao eu vou para o seguinte na lista de state_t.
    }
    
    Evento = 0;
  }
}
