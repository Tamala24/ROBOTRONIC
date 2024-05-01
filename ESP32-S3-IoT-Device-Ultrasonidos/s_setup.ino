/**
 @file s_setup.ino
*/

#include <string>

#define SensorsUpdateInterval 1000 // 1 segundo de frecuencia de muestreo

// creating a task handle
TaskHandle_t Productor_Task, Consumidor_Task, GestorComMQTT_Task;
// creating buffer circular protegido
static Buffer_Circ Measure_buffer;
//static Buffer_Circ Message_buffer;

//void GestorComMQTT( void * parameter );
void Productor( void * parameter );
void Consumidor( void * parameter );

/**
 @brief on_setup. Añadir la configuración de pines, inicialización de variables, 
        configurar interrupciones, crear tasks, etc
 @param  ninguno
 @return ninguno
*/
void on_setup() 
{

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    // Inicializa los pines del ultrasonidos: 
    // trigger --> output  ;  echo --> input
    pinMode(TRIGGER_PIN_ULTRASONIDOS, OUTPUT);
    pinMode(ECHO_PIN_ULTRASONIDOS, INPUT);

    // Mensaje para comprobar la conexion MQTT
    String hello_msg = String("Hola Mundo! Desde dispositivo ") + deviceID;

    // Test JSON
    /*JsonDocument doc;
    doc["message"] = hello_msg;
    doc["luminosidad"] = 450;
    doc["temperatura"] = 21.5;
    String hello_msg_json;
    serializeJson(doc, hello_msg_json);
    enviarMensajePorTopic(HELLO_TOPIC, hello_msg_json);
    */

    // Configuramos interrupcion con el boton
    config_button();

    /* Create "Productor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Productor,             /* Task function. */
             "Productor",           /* name of task. */
             10000,                 /* Stack size of task */
             &Measure_buffer,       /* parameter of the task */
             1,                     /* priority of the task */
             &Productor_Task,       /* Task handle to keep track of created task */
             0);                    /* pin task to core 0 */

    /* Create "Consumidor_Task" using the xTaskCreatePinnedToCore() function */
    xTaskCreatePinnedToCore(
             Consumidor,            /* Task function. */
             "Consumidor",          /* name of task. */
             10000,                 /* Stack size of task */
             &Measure_buffer,       /* parameter of the task */
             1,                     /* priority of the task */
             &Consumidor_Task,      /* Task handle to keep track of created task */
             1);                    /* pin task to core 0 */
    
    // Create "GestorComMQTT_Task" using the xTaskCreatePinnedToCore() function 
    //xTaskCreate(
      //       GestorComMQTT,         /* Task function. */
        //     "GestorComMQTT",       /* name of task. */
          //   10000,                 /* Stack size of task */
            // &Message_buffer,       /* parameter of the task */
             //1,                     /* priority of the task */
             //&GestorComMQTT_Task);  /* Task handle to keep track of created task */
    
    delay(1000);

}

// Tarea para tomar mediciones del sensor ultrasonidos
void Productor( void * parameter )
{
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 1 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    // Leemos sensor ultrasonidos y guardamos distancia
    int cm = ping(TRIGGER_PIN_ULTRASONIDOS, ECHO_PIN_ULTRASONIDOS);
    if(put_item(cm, buff_prod) == -1)
    {
        // si el buffer esta lleno y no puedo insertar dato...
    }
    else
    {
      Serial.printf("Inserto dato %d\n", cm);
    }
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS) );
  }
    Serial.println("Finalizando tarea 1");
    vTaskDelete( NULL );
    
}

// Tarea para interpretar mediciones del sensor y tomar acciones 
void Consumidor( void * parameter )
{
  int cm;
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(&cm, buff_prod) == 0)
    {
      // Si he podido obtener dato...
      Serial.printf("saco dato %d\n", cm);
      // Asignamos mensaje segun la distancia
      String value;
      if( cm <= 25 )
      {
        value = "detectado";
        // Parar el motor dc de la cinta...
      }
      else 
      {
        value = "libre";
        // Reactivar motor dc de la cinta...
      }

      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = value;
      String ULTRASONIDOS_msg_json;
      serializeJson(doc, ULTRASONIDOS_msg_json);
      enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
    } 
 
    vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}

/*
// Tarea para gestionar comunicaciones con el broker MQTT
void GestorComMQTT( void * parameter )
{
  int cm;
  TickType_t xLastWakeTime;
  Buffer_Circ * buff_prod = (Buffer_Circ *) parameter;
  Serial.printf("Hola desde la tarea 2 en el Core %d\n", xPortGetCoreID());
  xLastWakeTime = xTaskGetTickCount();
  while (!PARAR)
  {
    if(get_item(&cm, buff_prod) == 0)
    {
      // Si he podido obtener dato...
      Serial.printf("saco dato %d\n", cm);
      // Asignamos mensaje segun la distancia
      String value;
      if( cm <= 25 )
      {
        value = "detectado";
        // Parar el motor dc de la cinta...
      }
      else 
      {
        value = "libre";
        // Reactivar motor dc de la cinta...
      }

      // Hacemos documento de json con el mensaje y enviamos por topic
      JsonDocument doc;
      doc["presencia"] = value;
      String ULTRASONIDOS_msg_json;
      serializeJson(doc, ULTRASONIDOS_msg_json);
      enviarMensajePorTopic(TOPIC_PRESENCIA, ULTRASONIDOS_msg_json);
    } 
 
    //vTaskDelayUntil( &xLastWakeTime, (SensorsUpdateInterval/ portTICK_PERIOD_MS));
    
  }
  Serial.println("Finalizando tarea 2");
  vTaskDelete( NULL );
}
*/

/*** End of file ****/