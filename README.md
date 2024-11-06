# Palmita-V0.1

**Palmita** es un dispositivo IoT diseñado para entusiastas del hardware. Con este proyecto, podrás crear soluciones inteligentes utilizando sensores y otros componentes electrónicos integrados.

## Características

-Conectividad GSM: Permite la transmisión de datos a múltiples plataformas a través de redes móviles GSM, lo que facilita su uso en ubicaciones remotas sin acceso a Wi-Fi.

-Integración con RS485: Soporte para el protocolo RS485, permitiendo comunicación estable y de largo alcance entre la placa y múltiples dispositivos, ideal para aplicaciones industriales y sensores específicos.

-Modularidad: Diseño modular que permite agregar o quitar módulos según las necesidades del proyecto, facilitando la personalización y ampliación de funcionalidades.

-Terminal Block para Conexión de Sensores: Terminales de conexión que simplifican la instalación y cambio de sensores, ofreciendo facilidad para proyectos en los que se necesite una conexión rápida y segura.

-Soporte de Ejemplos Prácticos: Incluye una serie de ejemplos de código y guías para ayudar a los usuarios a entender rápidamente cómo enviar datos, configurar RS485, y gestionar la conectividad GSM.

-Compatibilidad Multiplataforma: Preparada para conectarse con plataformas populares de IoT como Blynk y Ubidots permitiendo una amplia gama de aplicaciones.

-Diseño para Entusiastas y Makers: Con una estructura amigable y documentación accesible, permite a los desarrolladores de todos los niveles experimentar y prototipar rápidamente sus ideas.

## Librerías a utilizar

### TinyGsmClient
- **Uso**: Gestiona la conectividad GSM, permitiendo enviar y recibir datos a través de redes móviles. Esta biblioteca facilita el uso de módulos GSM como SIM800 y SIM900, simplificando la conexión a internet y la comunicación con servidores externos.

### StreamDebugger
- **Uso**: Proporciona herramientas de depuración para rastrear y visualizar datos enviados o recibidos por el módulo GSM. Es útil para diagnosticar problemas de conexión o transmisión, ayudando a optimizar la comunicación y detectar posibles fallos en la configuración de red.

### BlynkSimpleTinyGsm
- **Uso**: Facilita la integración con la plataforma Blynk a través de GSM. Esta biblioteca permite enviar datos y recibir comandos desde la aplicación Blynk, proporcionando una interfaz amigable para monitorear y controlar el dispositivo IoT desde un smartphone.

### FastLED
- **Uso**: Controla tiras de LEDs y otros tipos de LEDs RGB, ofreciendo efectos visuales y control de colores. Se puede utilizar para proporcionar feedback visual, como mostrar el estado de la conexión GSM, los datos de sensores o alertas específicas.

## Pinout

## Pinout

A continuación se presenta el pinout de la placa IoT:

### Vista General del Pinout

![Vista General del Pinout](./Imagenes/pinout.png.png)

### Detalles del Pinout

- **Referencia GSM**
  ![Referencia GSM](./Imagenes/referencias_gsm.png.png)

- **Referencia RS485**
  ![Referencia RS485](./Imagenes/referencias_485.png.png)

- **Referencia Jumpers**
  ![Referencia Jumpers](./Imagenes/referencias_jumper.png.png)

- **Referencia Led**
  ![Referencia Led](./Imagenes/referencias_led.png.png)


## Ejemplos

En la carpeta **ejemplos**, encontrarás una colección de ejemplos que muestran las capacidades de **Palmita-V0.1**. Estos ejemplos están diseñados para ayudarte a comenzar rápidamente y a entender cómo integrar y utilizar los diferentes sensores y componentes con el dispositivo.

### Contenido de la carpeta:

- **1_Palmita_Test**: Código para probar diversas funcionalidades del módem SIM800L usando la librería TinyGSM.
- **2_Palmita_Blynk**: Código para integrar Palmita V0.1 a Blynk (plataforma IoT) mandando un valor aleatorio
- **3_Palmita_RS485**: Código para probar la interfaz RS485 disponible en la placa Palmita V0.1, en el ejemplo usamos el sensor XY-MD02. Este código se utiliza para leer la temperatura y humedad de un dispositivo Modbus RTU
- **4_Palmita_Led_Test**: Este código configura y controla el LED RGB que incluye la placa Palmita V0.1 alternando entre los colores rojo, verde, azul y apagado.

## Instalación

Sigue estos pasos para empezar a utilizar **Palmita-V0.1**:

1. Clona este repositorio:
   ```bash
   git clone https://github.com/tu-usuario/palmita-V0.1.git

