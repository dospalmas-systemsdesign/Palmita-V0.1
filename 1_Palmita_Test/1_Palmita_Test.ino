/**************************************************************
  Código: 1_Palmita_Test
  Placa: Palmita_V1
  Información: Código para probar diversas funcionalidades
  del módem SIM800L usando la librería TinyGSM
 **************************************************************/

#define TINY_GSM_MODEM_SIM800   // Selección del módem SIMCom SIM800L
#define TINY_GSM_RX_BUFFER 1024 // Establecer el búfer RX a 1 Kb para recibir datos
#define SIM800L Serial1         // Definir el puerto serial utilizado para el SIM800L

// #define DUMP_AT_COMMANDS // Habilitar para ver todos los comandos AT ejecutados (opcional)

#define TINY_GSM_DEBUG Serial // Definir la consola serial para imprimir mensajes de depuración

#define GSM_PIN "" // Configurar el PIN de la SIM si es necesario (dejar en blanco si no aplica)

// Establecer números de teléfono para probar funcionalidad de SMS y llamadas
#define NUMERO_SMS "+526681570333"     // Número al cual se enviarán los mensajes SMS
#define NUMERO_LLAMADA "+526681570333" // Número al cual se realizarán las llamadas

// Credenciales GPRS para conexión a internet
const char apn[] = "internet.itelcel.com"; // APN del operador (en este caso Telcel)
const char gprsUser[] = "webgprs";         // Usuario GPRS
const char gprsPass[] = "webgprs2003";     // Contraseña GPRS

/**************************************************************
  Información: Pinout placa palmita v1
  Descripción: Definición de los pines utilizados para la comunicación
  serial con el módem SIM800L en la placa Palmita V1
 **************************************************************/

#define PIN_RX_GSM 19 // Pin RX para la comunicación con el módem SIM800L
#define PIN_TX_GSM 23 // Pin TX para la comunicación con el módem SIM800L

/**************************************************************
  Información: Pruebas de distintas funcionalidades del módem SIM800L
  Descripción: Bandera para activar o desactivar diferentes pruebas
  de las funcionalidades del módem SIM800L
 **************************************************************/

#define TINY_GSM_TEST_GPRS true  // Prueba de conexión GPRS
#define TINY_GSM_TEST_TCP true   // Prueba de conexión TCP
#define TINY_GSM_TEST_SSL false  // Prueba de conexión SSL
#define TINY_GSM_TEST_CALL false // Prueba de realizar llamadas
#define TINY_GSM_TEST_SMS false  // Prueba de envío de SMS
#define TINY_GSM_TEST_TIME true  // Prueba de obtener la hora del servidor
#define TINY_GSM_POWERDOWN true  // Prueba de modo de bajo consumo (power down)

/**************************************************************
  Información: Detalles del servidor para probar TCP/SSL
  Descripción: Datos del servidor y recurso para las pruebas de
  conexión TCP/SSL
 **************************************************************/

const char server[] = "vsh.pp.ua";           // Servidor para la prueba de conexión
const char resource[] = "/TinyGSM/logo.txt"; // Recurso del servidor a solicitar

/**************************************************************
  Información: Cliente TinyGSM
  Descripción: Configuración del cliente TinyGSM para manejar el
  módem SIM800L, con opción para depurar los comandos AT.
 **************************************************************/

#include <TinyGsmClient.h> // Incluir la librería TinyGSM para controlar el módem SIM800L

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>               // Incluir la librería para depuración de comandos AT
StreamDebugger debugger(Serial1, Serial); // Depurador de comandos AT para verlos en el monitor serial
TinyGsm modem(debugger);                  // Inicializar el módem con el depurador
#else
TinyGsm modem(Serial1); // Inicializar el módem sin depurador
#endif

/**************************************************************
  Información: Setup
  Descripción: Inicialización del sistema. Se configuran la consola
  serial para la depuración y el módulo GSM para la comunicación.
 **************************************************************/

void setup()
{
  // Configurar la velocidad en baudios de la consola serial para depuración
  Serial.begin(115200);
  delay(10); // Pequeño retraso para asegurar la correcta inicialización

  /*
     Configurar los pines de reinicio, habilitación y encendido del módulo GSM
     Estos pines pueden variar dependiendo de la implementación de hardware,
     aquí podrían configurarse si fuera necesario.
  */

  // Configurar la velocidad en baudios del módulo GSM con configuración 8N1
  Serial1.begin(115200, SERIAL_8N1, PIN_RX_GSM, PIN_TX_GSM);
}

/**************************************************************
  Información: Loop
  Descripción: Ciclo principal donde se prueban diversas funcionalidades
  del módem, como conexión GPRS, TCP, SSL, llamadas y envío de SMS.
 **************************************************************/

void loop()
{
  /**************************************************************
    Prueba: Inicializar el módem para conexión a red y GPRS
    Esta sección inicializa el módem y asegura que esté listo
    para operar. Si falla, se detiene el programa.
    **************************************************************/

  DBG("Inicializando el módem ...");
  if (!modem.restart())
  {
    DBG("Modem Init: ERROR");
    return;
  }
  DBG("Modem Init: OK");

  /**************************************************************
    Prueba: Información del módem
    Aquí se obtienen y muestran el nombre y la información del módem
    para verificar que se esté utilizando el dispositivo correcto.
    **************************************************************/
  String nombreModem = modem.getModemName();
  DBG("Nombre del módem: ", nombreModem);

  String infoModem = modem.getModemInfo();
  DBG("Info del módem: ", infoModem);

  /**************************************************************
    Prueba: Conexión a red celular
    Esta parte verifica si es necesario desbloquear la SIM,
    luego intenta conectar a la red celular.
    **************************************************************/
#if TINY_GSM_TEST_GPRS
  // Desbloquear la tarjeta SIM con un PIN si es necesario
  if (GSM_PIN && modem.getSimStatus() != 3)
  {
    modem.simUnlock(GSM_PIN);
  }
#endif

  DBG("Esperando red...");
  if (!modem.waitForNetwork(600000L, true))
  {
    DBG("Red: No Conectada");
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected())
  {
    DBG("Red: Conectada");
  }

  /**************************************************************
    Prueba: Conexión GPRS
    Se conecta a GPRS utilizando el APN, usuario y contraseña.
    También se muestra información relevante como CCID, IMEI, etc.
    **************************************************************/
#if TINY_GSM_TEST_GPRS
  DBG("Conectando a GPRS...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass))
  {
    delay(10000);
    return;
  }

  bool res = modem.isGprsConnected();
  DBG("Estado de GPRS: ", res ? "Conectado" : "No conectado");

  String ccid = modem.getSimCCID();
  DBG("CCID: ", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI: ", imei);

  String imsi = modem.getIMSI();
  DBG("IMSI: ", imsi);

  String operador = modem.getOperator();
  DBG("Operador: ", operador);

  IPAddress ipLocal = modem.localIP();
  DBG("IP local: ", ipLocal.toString());

  int calidadSenal = modem.getSignalQuality();
  DBG("Calidad de señal: ", calidadSenal);
#endif

  /**************************************************************
    Prueba: Conexión TCP
    Intenta establecer una conexión TCP con un servidor y enviar
    una solicitud HTTP GET. También lee los datos recibidos.
    **************************************************************/
#if TINY_GSM_TEST_TCP && defined TINY_GSM_MODEM_HAS_TCP
  TinyGsmClient client(modem, 0);
  const int port = 80;
  DBG("Connecting to", server);
  if (!client.connect(server, port))
  {
    DBG("... failed");
  }
  else
  {
    // Make a HTTP GET request:
    client.print(String("GET ") + resource + " HTTP/1.0\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.print("Connection: close\r\n\r\n");

    // Wait for data to arrive
    uint32_t start = millis();
    while (client.connected() && !client.available() &&
           millis() - start < 30000L)
    {
      delay(100);
    };

    // Read data
    start = millis();
    char logo[640] = {
        '\0',
    };
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
    DBG("#####  RECEIVED:", strlen(logo), "CHARACTERS");
    client.stop();
  }
#endif

  /**************************************************************
       Prueba: Conexión SSL
       Establece una conexión SSL segura al servidor especificado y
       realiza una solicitud HTTPS. Luego, procesa los datos recibidos.
       **************************************************************/

#if TINY_GSM_TEST_SSL && defined TINY_GSM_MODEM_HAS_SSL
  // TODO: Add test of adding certificcate
  TinyGsmClientSecure secureClient(modem, 1);
  const int securePort = 443;
  DBG("Connecting securely to", server);
  if (!secureClient.connect(server, securePort))
  {
    DBG("... failed");
  }
  else
  {
    // Make a HTTP GET request:
    secureClient.print(String("GET ") + resource + " HTTP/1.0\r\n");
    secureClient.print(String("Host: ") + server + "\r\n");
    secureClient.print("Connection: close\r\n\r\n");

    // Wait for data to arrive
    uint32_t startS = millis();
    while (secureClient.connected() && !secureClient.available() &&
           millis() - startS < 30000L)
    {
      delay(100);
    };

    // Read data
    startS = millis();
    char logoS[640] = {
        '\0',
    };
    int read_charsS = 0;
    while (secureClient.connected() && millis() - startS < 10000L)
    {
      while (secureClient.available())
      {
        logoS[read_charsS] = secureClient.read();
        logoS[read_charsS + 1] = '\0';
        read_charsS++;
        startS = millis();
      }
    }
    Serial.println(logoS);
    DBG("#####  RECEIVED:", strlen(logoS), "CHARACTERS");
    secureClient.stop();
  }
#endif

  /**************************************************************
      Prueba: Llamar a un número celular
      Realiza una llamada al número especificado, reproduce tonos DTMF
      y luego cuelga la llamada.
      **************************************************************/

#if TINY_GSM_TEST_CALL && defined(TINY_GSM_MODEM_HAS_CALLING) && \
    defined(NUMERO_LLAMADA)
  DBG("Llamando a: ", NUMERO_LLAMADA);

  // Realizar llamada
  bool llamadaExitosa = modem.callNumber(NUMERO_LLAMADA);
  DBG("Llamada: ", llamadaExitosa ? "OK" : "fallida");

  if (llamadaExitosa)
  {
    delay(1000L);

    // Reproducir tono DTMF 'A'
    modem.dtmfSend('A', 1000);

    // Reproducir tonos DTMF del 0 al 4
    for (char tono = '0'; tono <= '4'; tono++)
    {
      modem.dtmfSend(tono);
    }

    delay(5000);

    // Colgar la llamada
    bool colgar = modem.callHangup();
    DBG("Colgar llamada: ", colgar ? "OK" : "fallido");
  }
#endif

  /**************************************************************
     Prueba: Enviar un SMS
     Envía un mensaje SMS al número especificado.
     **************************************************************/
#if TINY_GSM_TEST_SMS && defined TINY_GSM_MODEM_HAS_SMS && defined NUMERO_SMS
  bool mensajeSMS = modem.sendSMS(NUMERO_SMS, String("Hola desde ") + imei);
  DBG("SMS: ", mensajeSMS ? "OK" : "fallido");
#endif

/**************************************************************
    Prueba: Obtener hora de la red
    Solicita y muestra la hora actual obtenida de la red celular.
    **************************************************************/
#if TINY_GSM_TEST_TIME && defined TINY_GSM_MODEM_HAS_TIME
  // Inicializar variables para almacenar la fecha y hora
  int anio = 0;          // Variable para el año
  int mes = 0;           // Variable para el mes
  int dia = 0;           // Variable para el día
  int hora = 0;          // Variable para la hora
  int minuto = 0;        // Variable para los minutos
  int segundo = 0;       // Variable para los segundos
  float zonaHoraria = 0; // Variable para la zona horaria

  // Intentar obtener la hora actual de la red hasta 5 veces
  for (int8_t i = 5; i; i--)
  {
    DBG("Solicitando hora actual de la red"); // Mensaje de depuración para indicar que se solicita la hora

    // Llamar a la función para obtener la hora de la red
    if (modem.getNetworkTime(&anio, &mes, &dia, &hora, &minuto, &segundo, &zonaHoraria))
    {
      // Si se obtiene la hora correctamente, imprimir la fecha y hora
      DBG("Fecha: " + String(anio) + "-" + String(mes) + "-" + String(dia));       // Muestra la fecha obtenida
      DBG("Hora: " + String(hora) + ":" + String(minuto) + ":" + String(segundo)); // Muestra la hora obtenida
      DBG("Zona horaria: " + String(zonaHoraria));                                 // Muestra la zona horaria
      break;                                                                       // Salir del bucle si la hora se obtiene correctamente
    }
    else
    {
      // Si no se puede obtener la hora, imprimir un mensaje y esperar 15 segundos antes de reintentar
      DBG("No se pudo obtener la hora de la red, reintentando en 15s.");
      delay(15000L); // Espera de 15 segundos antes de intentar de nuevo
    }
  }

  // Obtener la hora actual de la red como un string
  String horaRed = modem.getGSMDateTime(DATE_FULL); // Obtener la fecha y hora en formato completo
  DBG("Hora actual de la red: " + horaRed);         // Imprimir la hora actual obtenida de la red
#endif

/**************************************************************
  Prueba: Apagar el módulo
  **************************************************************/
#if TINY_GSM_POWERDOWN
#if TINY_GSM_TEST_GPRS
  // Desconectar del servicio GPRS
  modem.gprsDisconnect(); // Llama a la función para desconectar el GPRS
  delay(5000L);           // Espera 5 segundos para asegurar que la desconexión se complete

  // Verifica si el módem está desconectado de GPRS
  if (!modem.isGprsConnected())
  {
    DBG("GPRS desconectado"); // Imprime un mensaje de éxito si está desconectado
  }
  else
  {
    DBG("Desconexión GPRS fallida"); // Imprime un mensaje de error si la desconexión falla
  }
#endif

  // Intentar apagar el módem
  modem.poweroff();      // Llama a la función para apagar el módem
  DBG("Módem apagado."); // Imprime un mensaje para confirmar que el módem se apagó
#endif

  DBG("Fin de las pruebas."); // Indica que todas las pruebas han finalizado

  // Mantener el módulo activo indefinidamente
  while (true)
  {
    modem.maintain(); // Mantiene el módem en modo de bajo consumo, evitando que se apague
  }
}
