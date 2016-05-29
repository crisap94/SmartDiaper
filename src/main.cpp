#define DEBUGGIN

#include <ESP8266WiFi.h>
#include "RainSensor.h"
#include "fsr.h"

#define HUMEDO 1
#define SECO 0

#define OK 1
#define WRONG 0

const char *ssid = "TORIBIO***";
const char *password = "11TO23ry62";

void wetFunc();
void dryFunc();
void sendData();
bool checkPosition();

#define DIGITAL_PIN_RAINDROPS_SENSOR  5
#define ANALOG_PIN A0
#define MAX_ANALOG_OSCILATION 75
#define POSITION_TIME 4000

uint32_t previusPositiontime = 0;
uint16_t maxRange = 0;
uint16_t previusMaxRange = 0;
uint16_t minRange = 0;
uint16_t previusMinRange = 0;


WiFiClient client;
RainSensor rainSensor(DIGITAL_PIN_RAINDROPS_SENSOR, &wetFunc, &dryFunc);
Fsr FSR(ANALOG_PIN);

/*******************************************
 * Variables para enviar al servidor
 *********************************************/
/**
 * HUMEDAD
 */
uint8_t estado_parametro_sensado1 = OK;
uint8_t id_tipo_sensado1 = 1;
uint8_t parametro_sensado1 = 0;

/**
 * PRESION
 */
uint8_t estado_parametro_sensado2 = OK;
uint8_t id_tipo_sensado2 = 2;
uint8_t parametro_sensado2 = 0;
/*********************************************/


void setup() {
  Serial.begin(9600);
  rainSensor.begin();

  // Serial.println("Conectando a: ");
  // Serial.println(ssid);
  // WiFi.begin(ssid, password);
  //
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  //
  // Serial.println("Conectado a Red WI-FI");
  // Serial.println(ssid);
  // Serial.print("La Ip de la placa es: ");
  // Serial.println(WiFi.localIP());
}


void loop() {
  checkPosition();
  rainSensor.sense();
  // Serial.println("Presion : " + String(FSR.getValue()) + "\tAgua : " + String(rainSensor.getDigitalState()));

}

bool checkPosition(){
  uint16 value = FSR.getValue();
  if(maxRange != previusMaxRange || minRange != previusMinRange){
    if (value - MAX_ANALOG_OSCILATION < 0) {
      minRange = 0;
      previusMinRange = minRange;
      maxRange = value + MAX_ANALOG_OSCILATION;
      previusMaxRange = value + MAX_ANALOG_OSCILATION;
    }else if(value + MAX_ANALOG_OSCILATION > 1023){
      maxRange = 1023;
      previusMaxRange = maxRange;
      minRange = value - MAX_ANALOG_OSCILATION;
      previusMinRange = value - MAX_ANALOG_OSCILATION;
    }else{
      maxRange = value + MAX_ANALOG_OSCILATION;
      previusMaxRange = value + MAX_ANALOG_OSCILATION;
      minRange = value - MAX_ANALOG_OSCILATION;
      previusMinRange = value - MAX_ANALOG_OSCILATION;
    }
  }

  if( value < minRange || value > maxRange){
    previusMaxRange++;
    previusMinRange++;
    previusPositiontime = millis();
    #ifdef DEBUGGIN
      Serial.println("El Paciente se movio..");
    #endif
  }

  if(value > minRange && value < maxRange && millis() - previusPositiontime > POSITION_TIME){
    parametro_sensado2 = value;
    #ifdef DEBUGGIN
      Serial.println("Enviado dato de posicion estatica...");
    #endif
    // sendData();
    previusPositiontime = millis();
  }
}

void wetFunc(){
  parametro_sensado1 = HUMEDO;
  #ifdef DEBUGGIN
    Serial.println("El Paciente se Orino");
  #endif
  // sendData();
}
void dryFunc(){
  parametro_sensado1 = SECO;
  #ifdef DEBUGGIN
    Serial.println("El Pañal Fue cambiado");
  #endif
  // sendData();
}

void sendData() {
  if (client.connect("monitor.grupoesoluciones.com", 80) > 0) {

    client.print("GET "
                 "http://monitor.grupoesoluciones.com/"
                 "controlador/ReceptorDatosPaciente.php?");
    client.print("&cantidad=");
    client.print("2");
    client.print("&estado_parametro_sensado1=");
    client.print(estado_parametro_sensado1);
    client.print("&id_tipo_sensado1=");
    client.print(id_tipo_sensado1);
    client.print("&parametro_sensado1=");
    client.print(parametro_sensado1);
    client.print("&estado_parametro_sensado2=");
    client.print(estado_parametro_sensado2);
    client.print("&id_tipo_sensado2=");
    client.print(id_tipo_sensado2);
    client.print("&parametro_sensado2=");
    client.print(parametro_sensado2);
    client.print("&identificacion_paciente=");
    client.print("65690298");
    client.print(" HTTP/1.0");
    client.println();
    client.println("User-Agent: monitor.grupoesoluciones.com");
    client.println();

    #ifdef DEBUGGIN
      Serial.print("DATOS ENVIADOS..!!");
    #endif

  }else{
    #ifdef DEBUGGIN
      Serial.println("Connection Failed...!!");
    #endif
  }
}
