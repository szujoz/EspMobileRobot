#include "car.h"
#include <FS.h>

CAR* car;
int tim = 0;

void setup()
{
  Serial.begin(115200);

  for(uint8_t t = 5; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  SPIFFS.begin();

  Serial.println("Boot has finished...");
  car = new CAR(D8,D7,D6,D3,D0,D5);
}


void loop() {
  car->process();
  //demo();

  //Serial.println(analogRead(A0));
}

void demo(void)
{
  int cntr = millis();

  if ((cntr - tim) == 10000)
  {
    cntr++;
    
    Serial.println("10th second RIGHT");
    //digitalWrite(D8, HIGH);
    //digitalWrite(D6, HIGH);
    car->engine_on();
    car->rotate(TURN_DIRECTION::RIGHT, 1000);
  }
  if ((cntr - tim) == 12000)
  {
    cntr++;
    
    Serial.println("12th second");
    //digitalWrite(D8, LOW);
    //digitalWrite(D6, LOW);
    car->stop();
  }
  if ((cntr - tim) == 20000)
  {
    cntr++;
    
    Serial.println("20th second Left");
    //digitalWrite(D8, HIGH);
    //digitalWrite(D7, HIGH);
    car->rotate(TURN_DIRECTION::LEFT, 700);  
  }
  if ((cntr - tim) >= 22000)
  {
    tim = cntr;
    
    Serial.println("22th second");
    //digitalWrite(D8, LOW);
    //digitalWrite(D7, LOW);
    car->engine_off();
  }
}
