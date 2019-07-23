#include "car.h"

CAR::CAR(const uint8_t r_en, const uint8_t r_cp1, const uint8_t r_cp2,
    const uint8_t l_en, const uint8_t l_cp1, const uint8_t l_cp2)
{  
  motor_right = new MOTOR(r_en, r_cp1, r_cp2, false);
  motor_left  = new MOTOR(l_en, l_cp1, l_cp2, false);
  speed = 0;

  net = new WEBSERVER("KOCSI01","password01");

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
  
  // Get inputs
  net->process();
  rx = net->getRxData();

  // Calculate response
  
  // Actuate
  if (net->newDataReceived() == true)
  {
    switch (rx[1])
    {
      case 0x00:  { (rx[2] == 0x00) ? Serial.println("Engine off")                   : Serial.println("Engine on");                    break; }
      case 0x01:  { (rx[2] == 0x00) ? digitalWrite(motor_left->get_pin_en(),    LOW) : digitalWrite(motor_left->get_pin_en(),    HIGH);  break; }
      case 0x02:  { (rx[2] == 0x00) ? digitalWrite(motor_left->get_pin_cp1(),   LOW) : digitalWrite(motor_left->get_pin_cp1(),   HIGH);  break; }
      case 0x03:  { (rx[2] == 0x00) ? digitalWrite(motor_left->get_pin_cp2(),   LOW) : digitalWrite(motor_left->get_pin_cp2(),   HIGH);  break; }
      case 0x04:  { (rx[2] == 0x00) ? digitalWrite(motor_right->get_pin_en(),   LOW) : digitalWrite(motor_right->get_pin_en(),   HIGH);  break; }
      case 0x05:  { (rx[2] == 0x00) ? digitalWrite(motor_right->get_pin_cp1(),  LOW) : digitalWrite(motor_right->get_pin_cp1(),  HIGH);  break; }
      case 0x06:  { (rx[2] == 0x00) ? digitalWrite(motor_right->get_pin_cp2(),  LOW) : digitalWrite(motor_right->get_pin_cp2(),  HIGH);  break; }
      case 0x07:  { (rx[2] == 0x00) ? Serial.println("imu ax 0")                     : Serial.println("imu ax 1");                     break; }
      case 0x08:  { (rx[2] == 0x00) ? Serial.println("imu ay 0")                     : Serial.println("imu ax 1");                     break; }
      
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
