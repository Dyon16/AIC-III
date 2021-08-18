#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA

LiquidCrystal_I2C lcd(0x3f,16,2);

#define CURRENT_CAL 18.40 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
const int pinIN2 = 6;
const int pinBotao = 5;

int estado_botao = 0;
int estado = 0;

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA

void setup()
{  
  Serial.begin(9600); //INICIALIZA A SERIAL
  lcd.init();
  
  pinMode(pinIN2, OUTPUT);
  pinMode(pinBotao, INPUT_PULLUP);
  digitalWrite(pinIN2, HIGH);
  emon1.current(pinoSensor, CURRENT_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
}

void loop()
{ 
  if (digitalRead(pinBotao) == HIGH and estado == 0)
  {
    estado = 1;
    digitalWrite(pinIN2, LOW);
  }
  else if (digitalRead(pinBotao) == HIGH and estado == 1)
  {
    estado = 0;
    digitalWrite(pinIN2, HIGH);
  }
  
  emon1.calcVI(17, 100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double currentDraw = emon1.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  double power = 127 * currentDraw;

  lcd.setBacklight(HIGH);
  
  lcd.setCursor(0,0);
  lcd.print("Corrente: ");
  lcd.print(currentDraw);
  lcd.print("A");

  lcd.setCursor(0,1);
  lcd.print("Potencia: ");
  lcd.print(power);
  lcd.print("W");

  digitalWrite(pinIN2, HIGH);

  delay(1000);
}
