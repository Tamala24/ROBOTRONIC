/**
 * @file  Config.h
 * @brief Archivo de configuración del sistema
 */

#ifndef CONFIG_H
#define CONFIG_H

// COMM BAUDS
#define BAUDS 115200

#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // Niveles en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE

// DEVICE
#define DEVICE_GIIROB_PR2_ID      "17" //"giirobpr2_00"

// WIFI
#define NET_SSID                  "UPV-PSK"
#define NET_PASSWD                "giirob-pr2-2023"

// MQTT
#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "giirob"    // Descomentar esta línea (y la siguiente) para que se conecte al broker MQTT usando usuario y contraseña
#define MQTT_PASSWORD             "UPV2024"

// TOPICS
/**** HELLO_TOPIC ****/
#define HELLO_TOPIC               "A1" 
/**** TOPIC_QR : manda si detecta QR distinto ****/
#define TOPIC_QR                  "A1/sensor/camara/qr/cinta/cajas"
/**** TOPIC_PARADA_EMERGENCIA : manda o recibe "PARAR" si se ha pulsado el botón de emergencia ****/
#define TOPIC_PARADA_EMERGENCIA   "A1/sensor/boton/emergencia/cinta/cajas"

// DEFINICION DE CONSTANTES
#define true 1                       ///< Valor booleano verdadero
#define false 0                      ///< Valor booleano falso
#define SensorsUpdateInterval 1000   ///< Frecuencia de muestreo en milisegundos (1 segundo)


// PINES
/**** IO ****/
#define LED_ROJO                  2  // Pin led rojo  
#define LED_VERDE                 3  // Pin led verde  

/**** INTERRUPCION BOTON ****/
#define PIN_BUTTON 20

#endif // CONFIG_H

/*** End of file ****/