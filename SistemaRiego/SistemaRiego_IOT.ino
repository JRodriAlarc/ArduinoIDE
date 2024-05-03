//Se Añaden las Librerias para el LCD
#include <Wire.h> //Librería para la Interfaz I2C
#include <LiquidCrystal_I2C.h>  //Libreria para la Pantalla LCD

//Se Añaden las Librerias para el Sensor de Temperatura
#include <DHT.h>
#include <DHT_U.h>

//Constante del Sistema de Riego
const int LEDpin = 10;   //Constante que indica el Pin de conexión del Led del Sistema de Riego 

/*	Para el Sistema de Riego:
1 dia equivale a 1 minuto = 60000ms
1 minuto equivale a 1 segundo = 1000ms
*/
const long onDuration = 3500; //Constante de Tiempo de Encendido del LED en milisegundos
const long offDuration = 15000; //Constante de Tiempo de Apagado del LED en milisegundos
int LEDState = HIGH;  //Variable que almacena el estado Actual del LED
long rememberTime = 0;  //Variable que almacenará el Tiempo de Inicio en milisegundos

//Variables del Sensor de Movimiento
int pirState = LOW; //Variable que almacena el estado Actual del Sensor
const int sensorPir = 8; //Constante que indica el Pin de conexión del sensor 
int val = 0; //Variable auxiliar que almacena la información del Sensor

//Sistema de Iluminacion activado por Movimiento
const int ledIluminacion = 13; //Constante que indica el Pin de conexión del Led de Iluminación 

//Sistema de Alerta por Temperatura
const int tempAlta = 5; //Constante que indica el Pin de conexión del Led de Temperatura Alta 
const int tempBaja = 4; //Constante que indica el Pin de conexión del Led de Temperatura Baja 

//Variables del Sensor de Temperatura
const int sensor = 2; //Constante que indica el Pin de conexión del Sensor
int Temperatura;   //Variable que almacena la temperatura registrada por el sensor 

//Varibles de Ventilador
const int cooler = A0; //Constante que indica el Pin de conexión del Ventilador

//Se crea un objeto llamado "dht" con el número de pin de conexión del sensor y se indica el modelo
DHT dht(sensor, DHT11);

//Se crea un objeto llamado "lcd" con la dirección del LCD y se indican las columnas y filas del mismo
LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {
  //Se inicializan de las variables
  Serial.begin(9600);   //Se inicializa el Puerto Serial 
  dht.begin();  //Se inicializa el sensor de Temperatura
  
  lcd.init(); //Se inicializa el LCD
  lcd.backlight();  //Se Enciende la luz de fondo del LCD.
  
  //Se inicializan sensor de Movimiento 
  pinMode(ledIluminacion, OUTPUT);  //Se inicializa el Led activado por Movimiento
  pinMode(sensorPir, INPUT);  

  //Se inicializan los Leds de Alerta de Temperatura
  pinMode(tempAlta, OUTPUT);  
  pinMode(tempBaja, OUTPUT);

  //Se inicializan los Leds del Sistema de Riego
  pinMode(LEDpin,OUTPUT);
  digitalWrite(LEDpin,LEDState);  //Se inicializa según su estado
}

void loop() {

  lcd.clear(); //Se limpia el contenido del LCD
  Temperatura = dht.readTemperature();  //Se almacena la temperatura ambiente en la variable

  //Se Muestra La Temperatuar en el LCD.
  lcd.setCursor(0, 0);  //Se indica la posición de las letras
  lcd.print("Temperatura: "); //Se muestra la cadena de carácteres
  lcd.setCursor(0, 1);  //Se indica la nueva posición del Mensaje
  lcd.print(Temperatura); //Se imprime la temperatura almacenada en la variable
  lcd.println(" *C"); //Se concatenan los caracteres para indicar que se trata de grados Centigrados
  delay(500); //Se actualiza la Información de la Temperatura cada 500 milisegundos

  if(Temperatura >= 25) { //Se Verifica si la Temperatura es Superior 24°C
    Serial.print("Temperatura ALTA: "); //Se Muestra la Alerta en el Monitor Serial
    Serial.print(Temperatura);  //Se Muestra la Temperatura actual en el Monitor Serial
    Serial.println("°C"); //Se concatenan los caracteres para indicar que se trata de grados Centigrados
    digitalWrite(tempBaja,LOW); //Se Apaga el Led indicador de temperatura Estandar
    digitalWrite(tempAlta,HIGH);  //Se Enciende el Led de Alerta de Temperatura Alta
    analogWrite(cooler, 500); //Si la temperatura alcanza los 25°C Se enciende el Ventilador
  }
  
  else {  //Si no sucede la condición
    Serial.print("Temperatura NORMAL: "); //Se Muestra la Alerta en el Monitor Serial
    Serial.print(Temperatura);  //Se Muestra la Temperatura actual en el Monitor Serial
    Serial.println("°C"); //Se concatenan los caracteres para indicar que se trata de grados Centigrados
    digitalWrite(tempAlta,LOW); //Se Apaga el Led de Alerta de Temperatura Alta
    digitalWrite(tempBaja,HIGH);  //Se Enciende el Led indicador de Temperatura Estandar  
    analogWrite(cooler, 0); //Si no se superan los 24°C se mantiene apagado el Ventilador
  }

  val = digitalRead(sensorPir);	//Se lee y almacena el valor del sensor PIR HC-SR501 del pin digital 8
	
	if(val == HIGH) { //Se Verifica si esta activado el sensor de Movimiento
		digitalWrite(ledIluminacion, HIGH);	//Se enciende el Led del sistema de iluminación por Movimiento
		if(sensorPir == LOW) { //Se verifica si previamente estaba inactivo el sensor de Movimiento
			Serial.println(" Invernadero Activo	");	//Se muestra en el puerto Serial el Estado del Invernadero
			pirState = HIGH;	//Se Invierte el estado del Sensor a Encendido
		}
	} 
  
  else { 	//Si no sucede la condición
		digitalWrite(ledIluminacion, LOW); //Se apaga el Led del sistema de iluminación por movimiento
		if(pirState == HIGH) { //Se verifica si previamente estaba Activo el Sensor de Movimiento
			Serial.println(" Invernadero Inactivo	");	//Se muestra en el puerto Serial el Estado del Invernadero
			pirState = LOW;		//Se Invierte el estado del Sensor a Apagado
		}
	}
  
	/*	Para el Sistema de Riego:
	1 dia equivale a 1 minuto = 60000ms
	1 minuto equivale a 1 segundo = 1000ms
	*/

  if( LEDState == HIGH ) {  //Se verifica si el estado del LED esta Activo
    if( (millis() - rememberTime) >= onDuration){ //Se comprueba si el tiempo Actual MENOS el tiempo de Inicio es SUPERIOR al tiempo de ENCENDIDO del LED
    LEDState = LOW; //Se Invierte el estado del LED
    rememberTime = millis();  //Se actualiza el Tiempo Inicial mediante la función millis()
    }
  } 
  
  else  {   //Si no sucede la condición
    if( (millis() - rememberTime) >= offDuration){  //Se comprueba si el tiempo Actual MENOS el tiempo de Inicio es SUPERIOR o IGUAL al tiempo de APAGADO del LED  
    LEDState = HIGH;  //Se Invierte el estado del LED
    rememberTime = millis();  //Se actualiza el Tiempo Inicial mediante la función millis()
    }
 }

  //LED encendido por medio de la función millis()
  digitalWrite(LEDpin,LEDState); //Según el estado del LED, se Encenderá o se Apagará
}