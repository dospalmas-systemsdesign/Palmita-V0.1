/**************************************************************
  Código: 3__Palmita_RS485
  Placa: Palmita_V1
  Información: Código para probar la interfaz RS485 disponible
  en la placa Palmita V1, en el ejemplo usamos el sensor XY-MD02.
  Este código se utiliza para leer la temperatura y humedad de un
  dispositivo Modbus RTU utilizando el protocolo RS485
 **************************************************************/

/**************************************************************
  Información: Pinout para la comunicación de la interfaz RS485
  Descripción: Configuración de pines para comunicación RS485
               utilizando un módulo MAX485 conectado a un ESP32.
 **************************************************************/

#define PIN_RX_RS485 16 // Definición del pin RX para la comunicación RS485
#define PIN_TX_RS485 17 // Definición del pin TX para la comunicación RS485
#define DE_RE_PIN 5     // Definición del pin para controlar la dirección de transmisión
 /**************************************************************
    Información: Inicialización de la comunicación serie
    Descripción: Configura la comunicación serie con la computadora
                 y el módulo RS485, estableciendo los pines y modos.
  **************************************************************/
void setup()
{
  // Inicializa la comunicación serie con la computadora a 115200 baudios
  Serial.begin(115200);

  // Inicializa la comunicación serie RS485 a 9600 baudios,
  // configurando los parámetros de datos (8 bits, sin paridad, 1 bit de parada)
  Serial1.begin(9600, SERIAL_8N1, PIN_RX_RS485, PIN_TX_RS485);

  // Configura el pin DE_RE como salida para controlar la dirección de transmisión
  pinMode(DE_RE_PIN, OUTPUT);

  // Establece el pin DE_RE en LOW para activar el modo de recepción
  digitalWrite(DE_RE_PIN, LOW); // LOW para modo recepción
}

  /**************************************************************
    Información: Lectura de datos del sensor
    Descripción: Envía solicitudes de lectura de temperatura y
                 humedad a través de Modbus RTU, y procesa las respuestas.
  **************************************************************/
 
void loop()
{
  // ----------- Lectura de temperatura (0x0001) -----------

  // Cambia al modo transmisión para enviar la solicitud
  digitalWrite(DE_RE_PIN, HIGH);

  // Solicitud Modbus RTU para leer el input register 0x0001 (temperatura)
  byte solicitudTemperatura[] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x60, 0x0A};
  Serial1.write(solicitudTemperatura, sizeof(solicitudTemperatura));

  delay(10); // Espera breve para que se envíe la solicitud

  // Cambia al modo recepción para recibir la respuesta
  digitalWrite(DE_RE_PIN, LOW);

  // Lee la respuesta para temperatura
  if (Serial1.available())
  {
    byte respuesta[7]; // Espera 7 bytes de respuesta
    int indice = 0;
    while (Serial1.available())
    {
      respuesta[indice++] = Serial1.read();
      if (indice >= 7)
        break;
    }

    // Verifica la respuesta y extrae la temperatura
    if (indice == 7 && respuesta[1] == 0x04)
    { // Función 0x04 confirmada
      int temperatura = (respuesta[3] << 8) | respuesta[4];
      Serial.print("Temperatura: ");
      Serial.print(temperatura / 10);
      Serial.println(" °C");
    }
    else
    {
      Serial.println("Error en la respuesta de temperatura");
    }
  }

  delay(500); // Pausa breve entre peticiones

  // ----------- Lectura de humedad (0x0002) -----------

  // Cambia al modo transmisión para enviar la solicitud
  digitalWrite(DE_RE_PIN, HIGH);

  // Solicitud Modbus RTU para leer el input register 0x0002 (humedad)
  byte solicitudHumedad[] = {0x01, 0x04, 0x00, 0x02, 0x00, 0x01, 0x90, 0x0A};
  Serial1.write(solicitudHumedad, sizeof(solicitudHumedad));

  delay(10); // Espera breve para que se envíe la solicitud

  // Cambia al modo recepción para recibir la respuesta
  digitalWrite(DE_RE_PIN, LOW);

  // Lee la respuesta para humedad
  if (Serial1.available())
  {
    byte respuesta[7]; // Espera 7 bytes de respuesta
    int indice = 0;
    while (Serial1.available())
    {
      respuesta[indice++] = Serial1.read();
      if (indice >= 7)
        break;
    }

    // Verifica la respuesta y extrae la humedad
    if (indice == 7 && respuesta[1] == 0x04)
    { // Función 0x04 confirmada
      int humedad = (respuesta[3] << 8) | respuesta[4];
      Serial.print("Humedad: ");
      Serial.print(humedad / 10);
      Serial.println(" %");
    }
    else
    {
      Serial.println("Error en la respuesta de humedad");
    }
  }

  delay(1000); // Espera un segundo antes de la siguiente lectura completa
}
