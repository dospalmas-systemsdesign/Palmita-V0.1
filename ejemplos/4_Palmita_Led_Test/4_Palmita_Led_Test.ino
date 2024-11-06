
/**************************************************************
  Código: 4_Palmita_Led_Test
  Placa: Palmita_V0.1
  Información: Este código configura y controla el LED RGB que
  incluye la placa Palmita V0.1 alternando entre los colores rojo,
  verde, azul y apagado.
 **************************************************************/

/**************************************************************
  Información: Control de un LED utilizando la biblioteca FastLED
  Descripción: Este código configura y controla un único LED RGB
               en una tira WS2812, alternando entre los colores
               rojo, verde, azul y apagado.
 **************************************************************/

#include <FastLED.h> // Archivo de cabecera para la biblioteca FastLED

/**************************************************************
  Información: Configuración del LED
  Descripción: Define el número de LEDs y el pin de datos
               utilizado para la comunicación con el LED.
 **************************************************************/
#define NUM_LEDS 1 // Número de LEDs presentes en la tira (solo 1 LED)
#define DATA_PIN 4 // Pin digital del Arduino conectado al LED

CRGB leds[NUM_LEDS]; // Array para almacenar el único LED

/**************************************************************
Información: Configuración inicial del LED
Descripción: Configura el único LED WS2812 en el pin DATA_PIN y
prepara el sistema para la comunicación con el LED.
**************************************************************/

void setup()
{

  // Configura el LED WS2812 en el pin DATA_PIN
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

/**************************************************************
Información: Ciclo de control del color del LED
Descripción: Cambia el color del LED entre rojo, verde, azul y
apagado, manteniendo cada color por 1 segundo.
**************************************************************/

void loop()
{

  // Enciende el LED en rojo
  leds[0] = CRGB::Red; // Asigna el color rojo al LED
  FastLED.show();      // Actualiza el LED para mostrar el nuevo color
  delay(1000);         // Mantener encendido por 1 segundo

  // Enciende el LED en verde
  leds[0] = CRGB::Green; // Asigna el color verde al LED
  FastLED.show();        // Actualiza el LED
  delay(1000);           // Mantener encendido por 1 segundo

  // Enciende el LED en azul
  leds[0] = CRGB::Blue; // Asigna el color azul al LED
  FastLED.show();       // Actualiza el LED
  delay(1000);          // Mantener encendido por 1 segundo

  // Apagar el LED
  leds[0] = CRGB::Black;        // Asigna el color negro (apagado) al LED
  FastLED.show();               // Actualiza el LED
  Serial.println(F("LED OFF")); // Imprime en el monitor serie que el LED está apagado
  delay(1000);                  // Mantener apagado por 1 segundo
}
