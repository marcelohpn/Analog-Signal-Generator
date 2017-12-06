/*
    __  ___ ___     ____   ______ ______ __    ____     __  __ ____   _   __                                __         
   /  |/  //   |   / __ \ / ____// ____// /   / __ \   / / / // __ \ / | / /    _____ ____   ____ ___      / /_   _____
  / /|_/ // /| |  / /_/ // /    / __/  / /   / / / /  / /_/ // /_/ //  |/ /    / ___// __ \ / __ `__ \    / __ \ / ___/
 / /  / // ___ | / _, _// /___ / /___ / /___/ /_/ /  / __  // ____// /|  /  _ / /__ / /_/ // / / / / /_  / /_/ // /    
/_/  /_//_/  |_|/_/ |_| \____//_____//_____/\____/  /_/ /_//_/    /_/ |_/  (_)\___/ \____//_/ /_/ /_/(_)/_.___//_/     

Programa escrito por Marcelo Perbeils
Contato: marcelopfigueira@gmail.com

HONDA ELD SIGNAL GENERATOR

Emula uma saída analogica utilizando o FAST PWM de 62500hz e um capacitor.

Criado pois o swap de J30A4 precisa do sinal do ELD para não acender erro da injecao e meu VTI EG6 não possui ELD.

10K trimpot para escolher a voltagem de saída.
- 5v
- ground
- A3

220uf eletrolitic capacitor (pode ser um valor maior)
- D6
- ground

ELD output is D6
                                                                                                                      
*/


void setup() {
  Serial.begin(9600);
  
  // Set pin 6's PWM frequency to 62500 Hz (62500/1 = 62500)
  // Note that the base frequency for pins 5 and 6 is 62500 Hz
  setPwmFrequency(6, 1);
  
  pinMode(6, OUTPUT);
  pinMode(A3, INPUT);
  pinMode(13, OUTPUT);
     
  analogWrite(6, 220);

}

void loop() {
  delay(50000); //a alteração da frequencia (timer0) muda o calculo do delay
  int readValue = analogRead(A3);
  readValue = map(readValue, 0, 1023, 0, 255);
  readValue = constrain(readValue, 80, 230); //limita a voltagem de saída entre aprox. 1.5v a 4.5v
  analogWrite(6, readValue);
  digitalWrite(13, HIGH); //led on 
  delay(350000); //a alteração da frequencia (timer0) muda o calculo do delay
  digitalWrite(13, LOW); //led off
  Serial.print("Voltagem do ELD: ");
  Serial.println((float)readValue*5/255, 1);

}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
