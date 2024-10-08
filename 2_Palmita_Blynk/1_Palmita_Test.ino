/**************************************************************
  Ejemplo: Palmita Testing
  Consulta Información: -------
 **************************************************************/

// PINOUT PALMITA
#define RX_GSM 19
#define TX_GSM 23

// Selección del Modem SIMCom SIM800L
#define TINY_GSM_MODEM_SIM800

// Ver todos los comandos AT, si lo desea
// #define DUMP_AT_COMMANDS

// Definir la consola serial para imprimir depuraciones, si es necesario
#define TINY_GSM_DEBUG Serial

/*
   Pruebas habilitadas
*/
#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_TCP true
#define TINY_GSM_TEST_SSL false
#define TINY_GSM_TEST_CALL false
#define TINY_GSM_TEST_SMS false
#define TINY_GSM_TEST_TIME true
#define TINY_GSM_POWERDOWN true

// Configurar el PIN GSM, si lo hubiera
#define GSM_PIN ""

// Establecer números de teléfono, si deseas probar SMS y llamadas
#define SMS_TARGET "+529999999999"
#define CALL_TARGET "+529999999999"

// Tus credenciales GPRS
const char apn[] = "internet.itelcel.com";
const char gprsUser[] = "webgprs";
const char gprsPass[] = "webgprs2003";

// Detalles del servidor para probar TCP/SSL
const char server[] = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(Serial1);
#endif

void setup()
{
  // Configurar la velocidad en baudios de la consola
  Serial.begin(115200);
  delay(10);

  // Configurar los pines de reinicio, habilitación y encendido del módulo GSM
  // Configurar la velocidad en baudios del módulo GSM
  Serial1.begin(115200, SERIAL_8N1, RX_GSM, TX_GSM);
}

void loop()
{
  /**************************************************************
    Prueba: Comandos AT del módem
    **************************************************************/
  Serial.println("Probando Modem Test AT ...");
  if (!modem.testAT())
  {
    Serial.println("Error en Modem Test AT");
    return;
  }
  Serial.println("Modem Test AT OK");

  /**************************************************************
    Prueba: Inicializar el módem para conexión a red y GPRS
    **************************************************************/
  Serial.println("Inicializando el módem ...");
  if (!modem.init())
  {
    Serial.println("Error al inicializar el módem");
    return;
  }
  Serial.println("Modem Init OK");

  /**************************************************************
    Prueba: Información del módem
    **************************************************************/
  String name = modem.getModemName();
  Serial.println("Nombre del módem: " + name);

  String modemInfo = modem.getModemInfo();
  Serial.println("Info del módem: " + modemInfo);

  /**************************************************************
    Prueba: Conexión a red celular
    **************************************************************/
#if TINY_GSM_TEST_GPRS
  // Desbloquear la tarjeta SIM con un PIN si es necesario
  if (GSM_PIN && modem.getSimStatus() != 3)
  {
    modem.simUnlock(GSM_PIN);
  }
#endif

  Serial.println("Esperando red...");
  if (!modem.waitForNetwork(600000L, true))
  {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected())
  {
    Serial.println("Red conectada");
  }

  /**************************************************************
    Prueba: Conexión GPRS
    **************************************************************/
#if TINY_GSM_TEST_GPRS
  Serial.println("Conectando a GPRS...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass))
  {
    delay(10000);
    return;
  }

  bool res = modem.isGprsConnected();
  Serial.println("Estado de GPRS: " + String(res ? "conectado" : "no conectado"));

  String ccid = modem.getSimCCID();
  Serial.println("CCID: " + ccid);

  String imei = modem.getIMEI();
  Serial.println("IMEI: " + imei);

  String imsi = modem.getIMSI();
  Serial.println("IMSI: " + imsi);

  String cop = modem.getOperator();
  Serial.println("Operador: " + cop);

  IPAddress local = modem.localIP();
  Serial.println("IP local: " + local.toString());

  int csq = modem.getSignalQuality();
  Serial.println("Calidad de señal: " + String(csq));
#endif

  /**************************************************************
    Prueba: Conexión TCP
    **************************************************************/
#if TINY_GSM_TEST_TCP && defined TINY_GSM_MODEM_HAS_TCP
  TinyGsmClient client(modem, 0);
  const int port = 80;
  Serial.println("Conectando a " + String(server));
  if (!client.connect(server, port))
  {
    Serial.println("... conexión fallida");
  }
  else
  {
    // Realizar una solicitud HTTP GET
    client.print(String("GET ") + resource + " HTTP/1.0\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.print("Connection: close\r\n\r\n");

    // Esperar a que lleguen los datos
    uint32_t start = millis();
    while (client.connected() && !client.available() && millis() - start < 30000L)
    {
      delay(100);
    }

    // Leer los datos
    start = millis();
    char logo[640] = {'\0'};
    int read_chars = 0;
    while (client.connected() && millis() - start < 10000L)
    {
      while (client.available())
      {
        logo[read_chars] = client.read();
        logo[read_chars + 1] = '\0';
        read_chars++;
        start = millis();
      }
    }
    Serial.println(logo);
    Serial.println("#####  RECIBIDO: " + String(strlen(logo)) + " CARACTERES");
    client.stop();
  }
#endif

  /**************************************************************
    Prueba: Obtener hora de la red
    **************************************************************/
#if TINY_GSM_TEST_TIME && defined TINY_GSM_MODEM_HAS_TIME
  int year3 = 0;
  int month3 = 0;
  int day3 = 0;
  int hour3 = 0;
  int min3 = 0;
  int sec3 = 0;
  float timezone = 0;
  for (int8_t i = 5; i; i--)
  {
    Serial.println("Solicitando hora actual de la red");
    if (modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3, &timezone))
    {
      Serial.println("Año: " + String(year3) + "\tMes: " + String(month3) + "\tDía: " + String(day3));
      Serial.println("Hora: " + String(hour3) + "\tMinuto: " + String(min3) + "\tSegundo: " + String(sec3));
      Serial.println("Zona horaria: " + String(timezone));
      break;
    }
    else
    {
      Serial.println("No se pudo obtener la hora de la red, reintentando en 15s.");
      delay(15000L);
    }
  }
  Serial.println("Recuperando la hora como cadena");
  String time = modem.getGSMDateTime(DATE_FULL);
  Serial.println("Hora actual de la red: " + time);
#endif

  /**************************************************************
    Prueba: Apagar el módulo
    **************************************************************/
#if TINY_GSM_POWERDOWN
#if TINY_GSM_TEST_GPRS
  modem.gprsDisconnect();
  delay(5000L);
  if (!modem.isGprsConnected())
  {
    Serial.println("GPRS desconectado");
  }
  else
  {
    Serial.println("Desconexión GPRS fallida");
  }
#endif

  // Intentar apagar el módem
  modem.poweroff();
  Serial.println("Módem apagado.");
#endif

  Serial.println("Fin de las pruebas.");

  // Mantener el módulo activo
  while (true)
  {
    modem.maintain();
  }
}
