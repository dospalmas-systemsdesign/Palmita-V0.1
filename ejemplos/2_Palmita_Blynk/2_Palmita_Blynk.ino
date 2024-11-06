/**************************************************************
  Código: 2_Palmita_Blynk_GSM
  Placa: Palmita_V0.1
  Información: Código para integrar Palmita V0.1 a Blynk (plataforma IoT)
  mandando un valor aleatorio
 **************************************************************/

#define TINY_GSM_MODEM_SIM800   // Selección del módem SIMCom SIM800L para conexión GSM
#define TINY_GSM_RX_BUFFER 1024 // Establecer el búfer RX a 1 Kb para recibir datos del módem SIM800L
#define SIM800L Serial1         // Definir el puerto serial utilizado para la comunicación con el SIM800L

// #define DUMP_AT_COMMANDS // Habilitar para ver todos los comandos AT ejecutados en el monitor serial (opcional)

#define TINY_GSM_DEBUG Serial // Definir la consola serial para imprimir mensajes de depuración (Serial Monitor)

// Credenciales GPRS para la conexión a internet
const char apn[] = "internet.itelcel.com"; // APN del operador (en este caso Telcel)
const char gprsUser[] = "webgprs";         // Usuario GPRS para autenticación
const char gprsPass[] = "webgprs2003";     // Contraseña GPRS para autenticación

/**************************************************************
  Información: Código para la integración de Palmita V0.1 con Blynk.
  Descripción: Credenciales necesarias para
  la comunicación con la plataforma Blynk, utilizando la placa Palmita V0.1
  y el módem SIM800L.
 **************************************************************/

#define BLYNK_TEMPLATE_ID "TMPL2any69t0z"                   // Identificador del template en Blynk, necesario para asociar el dispositivo con la interfaz en la aplicación Blynk
#define BLYNK_TEMPLATE_NAME "Monitor temperatura y humedad" // Nombre del template en Blynk, utilizado para identificar el proyecto en la plataforma
#define BLYNK_AUTH_TOKEN "KQYo8vadxstAaHMwmP-DkZ_Z5rLLvQkc" // Token de autenticación Blynk, requerido para conectar el dispositivo con el servidor de Blynk

/**************************************************************
  Información: Pinout para la comunicación con el módem SIM800L.
  Descripción: Definición de los pines utilizados para la transmisión (TX)
  y recepción (RX) de datos entre la placa Palmita V0.1 y el módem SIM800L.
 **************************************************************/

#define PIN_RX_GSM 19 // Pin RX (Recepción de datos) utilizado para la comunicación con el módem SIM800L
#define PIN_TX_GSM 23 // Pin TX (Transmisión de datos) utilizado para la comunicación con el módem SIM800L

/**************************************************************
  Información: Inclusión de librerías y configuración del módem
  Descripción: Este bloque de código incluye las librerías necesarias
  para la comunicación con el módem SIM800L y la integración con Blynk.
 **************************************************************/

#include <TinyGsmClient.h>      // Incluir la librería para manejar el cliente GSM
#include <BlynkSimpleTinyGSM.h> // Incluir la librería para la integración de Blynk con TinyGSM

#ifdef DUMP_AT_COMMANDS // Verificar si se habilita la depuración de comandos AT

// Incluir la librería para depuración de comandos AT
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial); // Inicializar el depurador de comandos AT para visualizar en el monitor serial
TinyGsm modem(debugger);                  // Inicializar el módem utilizando el depurador para comandos AT
#else
TinyGsm modem(Serial1); // Inicializar el módem sin depurador (sin ver los comandos AT)
#endif

BlynkTimer timer; // Crear un objeto timer para manejar eventos temporizados en Blynk

/**************************************************************
  Información: Configuración inicial del programa.
  Descripción: Esta función se ejecuta una vez al inicio del programa.
  Se encarga de inicializar la comunicación serial y configurar
  los parámetros necesarios para la operación del módem GSM y Blynk.
 **************************************************************/
void setup()
{
  // Esperar 1 segundo antes de comenzar la inicialización
  delay(1000);

  // Inicializar la comunicación serial para el monitor serial
  Serial.begin(115200);

  // Inicializar la comunicación serial para el módem GSM utilizando los pines RX y TX
  Serial1.begin(115200, SERIAL_8N1, PIN_RX_GSM, PIN_TX_GSM);
  // Esperar 1 segundo para asegurar que el módem se inicializa correctamente
  delay(1000);

  // Inicializar la semilla para la generación de números aleatorios
  randomSeed(esp_random());

  // Configurar la función 'funcionIntervaloBlynk' para que se llame cada 5000 milisegundos (5 segundos)
  timer.setInterval(5000L, funcionIntervaloBlynk);
}

/**************************************************************
  Información: Función principal del programa.
  Descripción: Esta función se ejecuta continuamente en un bucle.
  Se encarga de verificar la conexión del módem, conectarse a la red
  y gestionar la conexión con la plataforma Blynk.
 **************************************************************/
void loop()
{
  // Verificar si el módem responde a los comandos AT
  if (modem.testAT())
  {
    // Verificar si no está conectado a la red
    if (!modem.isNetworkConnected())
    {
      DBG("[Modem]: Conectado RED . . .");
      // Esperar a que el módem se conecte a la red
      if (modem.waitForNetwork(10000L, true))
      {
        DBG("[Modem]: RED OK");
        // Obtener y mostrar el operador de la red
        DBG("[Modem] Operador: ", modem.getOperator());
        DBG("[Modem]: Conectado GPRS ", String(apn) + ". . .");
      }
      else
      {
        DBG("[Modem]: RED Error"); // Mensaje de error en la conexión de red
      }
    }

    // Si está conectado a la red pero no a GPRS
    if (modem.isNetworkConnected() && !modem.isGprsConnected())
    {
      // Intentar conectar a GPRS con las credenciales proporcionadas
      if (modem.gprsConnect(apn, gprsUser, gprsPass))
      {
        DBG("[Modem]: GPRS OK"); // Mensaje de éxito en la conexión GPRS
      }
      else
      {
        DBG("[Modem]: GPRS Error"); // Mensaje de error en la conexión GPRS
      }
    }

    // Si está conectado a la red y GPRS, y Blynk está conectado
    if (modem.isNetworkConnected() && modem.isGprsConnected() && Blynk.connected())
    {
      Blynk.run(); // Ejecutar la función de Blynk para manejar la conexión
      timer.run(); // Iniciar el temporizador de Blynk
    }
    else
    {
      // Si está conectado a la red y GPRS pero no a Blynk
      if (modem.isNetworkConnected() && modem.isGprsConnected())
      {
        DBG("[BLYNK]: Configurando dispositivo Token ", BLYNK_AUTH_TOKEN);
        // Configurar el dispositivo Blynk con el token de autenticación
        Blynk.config(modem, BLYNK_AUTH_TOKEN);
        DBG("[BLYNK]: Conectando . . . ");

        // Intentar conectar a Blynk con un tiempo de espera de 5000 ms
        if (Blynk.connect(5000))
        {
          DBG("[BLYNK]: Conectado a Blynk . . .");
        }
        else
        {
          DBG("[BLYNK]: Error al conectar . . .");
        }
      }
    }
  }
  else
  {
    DBG("[Modem]: AT ERROR"); // Mensaje de error en los comandos AT
  }
}

/**************************************************************
  Información: Función para enviar datos aleatorios a Blynk.
  Descripción: Esta función se ejecuta en intervalos regulares
  para generar un número aleatorio y enviarlo a la plataforma Blynk
  a través del pin virtual V0. También imprime el dato en el monitor
  serial para fines de depuración.
 **************************************************************/
void funcionIntervaloBlynk() // Definición de la función que envía datos a Blynk
{
  // Imprimir una línea en blanco para separar las salidas en el monitor serial
  Serial.println();
  // Imprimir un separador visual
  Serial.println("--------------------------------------------------");
  Serial.println("               ENVIANDO DATOS A BLYNK             "); // Mensaje indicando que se están enviando datos
  Serial.println("--------------------------------------------------");

  // Generar un número aleatorio entre 0 y 9999
  unsigned long dato = random(0, 10000);

  // Imprimir el dato aleatorio generado en el monitor serial
  Serial.println("Dato Aleatorio: " + String(dato));

  // Enviar el dato aleatorio a Blynk, utilizando el pin virtual V0
  Blynk.virtualWrite(V0, dato);
}

/**************************************************************
  Información: Función para depurar mensajes.
  Descripción: Esta función se utiliza para imprimir mensajes
  de depuración en el monitor serial.
 **************************************************************/
void DBG(const String &message, const String &value = "")
{
  if (value != "")
  {
    Serial.println(message + value);
  }
  else
  {
    Serial.println(message);
  }
}