#include "car.h"
#include <string>

CAR::CAR(const uint8_t r_en, const uint8_t r_cp1, const uint8_t r_cp2,
    const uint8_t l_en, const uint8_t l_cp1, const uint8_t l_cp2)
{  
  motor_right = new MOTOR(r_en, r_cp1, r_cp2, true);
  motor_left  = new MOTOR(l_en, l_cp1, l_cp2, false);
  speed = 0;

  net = new WEBSERVER("KOCSI01","password01");

  imu = new Imu();
  imu->start(true);

  Serial.println("Car is created");
}

CAR::~CAR(void)
{
  free(motor_right);
  free(motor_left);
}

void CAR::process(void)
{
  uint8_t* rx;
  uint16_t value;
  char     hex[4];
  
  // Get inputs
  net->process();
  imu->process();
  rx = net->getRxData();

  hex[0] = rx[2];
  hex[1] = rx[3];
  hex[2] = rx[4];
  //hex[3] = rx[5];

  value = (uint32_t) strtol(hex, NULL, 16);

  // Calculate response
  
  // Actuate
  if (net->newDataReceived() == true)
  {
    Serial.print("Value: ");
    Serial.println(value);
    
    switch (rx[1])
    {
      case 48:
      { 
        if (value == 1)
        {
          Serial.println("Engine on");
          engine_on();  
        }
        else
        {
          Serial.println("Engine off");
          engine_off();
        }                                
        break; 
      }
      case 49:  { (value == 1) ? Serial.println("Left motor on") : Serial.println("Left motor off");                        break; }
      case 50:
      { 
        Serial.print("Left motor speed: ");     
        Serial.println(value);
        motor_left->set_saturated_speed(value); 
        break;
      }
      case 51:
      {
        Serial.print("Left motor direction: ");
        Serial.println(value);
        motor_left->set_direction((ROTATION)value);
        break;
      }
      case 52:  { (value == 1) ? Serial.println("Right motor on") : Serial.println("Right motor off");                      break; }
      case 53:  { Serial.print("Right motor speed: ");     Serial.println(value);  motor_right->set_saturated_speed(value); break; }
      case 54:  { Serial.print("Right motor direction: "); Serial.println(value);  motor_right->set_direction((ROTATION)value);       break; }
      case 55:  { (rx[2] == 0x00) ? Serial.println("imu ax 0")                     : Serial.println("imu ax 1");            break; }
      case 56:  { (rx[2] == 0x00) ? Serial.println("imu ay 0")                     : Serial.println("imu ax 1");            break; }
      
    }
  }
}

void CAR::engine_on(void)
{
  motor_right->enable();
  motor_left->enable(); 
}

void CAR::engine_off(void)
{
  motor_right->disable();
  motor_left->disable();
}

void CAR::stop(void)
{
  motor_right->stop();
  motor_left->stop();
}

void CAR::straight(const STRAIGHT_DIRECTION dir, const uint8_t spd)
{
  if (dir == AHEAD)
  {
    motor_right->rotate(CLOCKWISE, spd);
    motor_left->rotate(ANTI_CLOCKWISE, spd);
  }
  else if (dir == REVERSE)
  {
    motor_right->rotate(ANTI_CLOCKWISE, spd);
    motor_left->rotate(CLOCKWISE, spd);
  }
  else
  {
    // Wrong parameters.
  }
}

void CAR::rotate(const TURN_DIRECTION dir, const uint32_t spd)
{
    if (dir == TURN_DIRECTION::LEFT)
  {
    motor_right->rotate(CLOCKWISE, spd);
    motor_left->rotate(CLOCKWISE, spd);
  }
  else if (dir == TURN_DIRECTION::RIGHT)
  {
    motor_right->rotate(ANTI_CLOCKWISE, spd);
    motor_left->rotate(ANTI_CLOCKWISE, spd);
  }
  else
  {
    // Wrong parameters.
  }
}

void CAR::turn_around(void)
{
  // TODO implementation
}
