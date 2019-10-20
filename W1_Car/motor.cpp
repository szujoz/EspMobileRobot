#include "motor.h"

#define SPEED_MAX   (1023U)    // v = max, full speed
#define SPEED_MIN   (0U)       // v = 0, stand still

MOTOR::MOTOR(uint8_t const en, uint8_t const cp1, uint8_t const cp2, bool const deb)
{
  pinMode(en,  OUTPUT);
  pinMode(cp1, OUTPUT);
  pinMode(cp2, OUTPUT);
  
  EN  = en;
  CP1 = cp1;
  CP2 = cp2;

  enabled   = false;
  direction = CLOCKWISE;
  speed     = 0;

  debug_on = deb;

  if (debug_on == true)
  {
    Serial.print(">>MOTOR: Created (En: ");
    Serial.print(EN);
    Serial.print(" , CP1: ");
    Serial.print(CP1);
    Serial.print(" , CP2: ");
    Serial.print(CP2);
    Serial.print(")");
  }
}

void MOTOR::set_saturated_speed(uint32_t const spd)
{
  // Saturate speed.
  if (spd > SPEED_MAX)
  {
    speed = SPEED_MAX;
  }
  else if (spd < SPEED_MIN)
  {
    speed = SPEED_MIN;
  }
  else
  {
    speed = spd;
  }

  // Actuate pins.
  actuate_motor_speed();

  if (debug_on == true)
  {
    Serial.print(">>MOTOR: Speed: ");
    Serial.println(speed);
  }
}

void MOTOR::set_direction(ROTATION const dir)
{
    if (dir == CLOCKWISE)
  {
    direction = CLOCKWISE;
    actuate_motor_direction();
  }
  else if (dir == ANTI_CLOCKWISE)
  {
    direction = ANTI_CLOCKWISE;
    actuate_motor_direction();
  }
  else
  {
    // Wrong parameter.
  }

  if (debug_on == true)
  {
    Serial.print(">>MOTOR: Direction: ");
    Serial.println(direction);
  }
}

uint8_t MOTOR::get_pin_en(void)
{
  if (debug_on == true)
  {
    Serial.print(">>MOTOR: EN: ");
    Serial.println(EN);
  }
  return EN;
}

uint8_t MOTOR::get_pin_cp1(void)
{
  if (debug_on == true)
  {
    Serial.print(">>MOTOR: CP1: ");
    Serial.println(CP1);
  }
  return CP1;
}

uint8_t MOTOR::get_pin_cp2(void)
{
  if (debug_on == true)
  {
    Serial.print(">>MOTOR: CP2: ");
    Serial.println(CP2);
  }
  return CP2;
}

void MOTOR::enable(void)
{
  enabled = true;
  
  if (debug_on == true)
  {
    Serial.println(">>MOTOR: Enabled.");
  }   
}

void MOTOR::disable(void)
{
  enabled = false;
  stop();

  if (debug_on == true)
  {
    Serial.println(">>MOTOR: Disabled.");
  }
}

void MOTOR::stop(void)
{
  speed = SPEED_MIN;
  digitalWrite(EN, speed);

  if (debug_on == true)
    Serial.println(">>MOTOR: Stopped.");
}

void MOTOR::rotate(ROTATION const dir, uint32_t const spd)
{
    set_direction(dir);
    set_saturated_speed(spd);
}

void MOTOR::actuate_motor_speed(void)
{
  if (enabled == true)
  {    
    analogWrite(EN, speed);

    if (debug_on == true)
    {
      Serial.print(">>MOTOR: Rotating (dir = ");
      Serial.print(direction);
      Serial.print(", spd = ");
      Serial.print(speed);
      Serial.println(").");
    }
  }
}

void MOTOR::actuate_motor_direction(void)
{
  if (enabled == true)
  {
    if (direction == CLOCKWISE)
    {
      digitalWrite(CP1, HIGH);
      digitalWrite(CP2, LOW);
    }
    else if (direction == ANTI_CLOCKWISE)
    {
      digitalWrite(CP1, LOW);
      digitalWrite(CP2, HIGH);
    }

    if (debug_on == true)
    {
      Serial.print(">>MOTOR: Rotating (dir = ");
      Serial.print(direction);
      Serial.print(", spd = ");
      Serial.print(speed);
      Serial.println(").");
    }
  }  
}
