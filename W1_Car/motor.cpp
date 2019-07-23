#include "motor.h"

#define SPEED_MAX   (255)    // v = max, full speed
#define SPEED_MIN   (0)     // v = 0, stand still

MOTOR::MOTOR(uint8_t const en, uint8_t const cp1, uint8_t const cp2, bool const deb)
{
  pinMode( en, OUTPUT);
  pinMode( cp1, OUTPUT);
  pinMode( cp2, OUTPUT);
  
  EN  = en;
  CP1 = cp1;
  CP2 = cp2;

  enabled   = false;
  direction = CLOCKWISE;
  speed     = 0;

  debug_on = deb;

  if (debug_on == true)
    Serial.println("Motor is created");
}

void MOTOR::set_saturated_speed(uint32_t const spd)
{
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

  if (debug_on == true)
  {
    Serial.print("Motor speed: ");
    Serial.println(speed);
  }
}

void MOTOR::set_direction(ROTATION const dir)
{
    if (dir == CLOCKWISE)
  {
    direction = CLOCKWISE;
  }
  else if (dir == ANTI_CLOCKWISE)
  {
    direction = ANTI_CLOCKWISE;
  }
  else
  {
    // Wrong parameter.
  }

  if (debug_on == true)
  {
    Serial.print("Motor direction: ");
    Serial.println(direction);
  }
}

uint8_t MOTOR::get_pin_en(void)
{
  return EN;
}

uint8_t MOTOR::get_pin_cp1(void)
{
  return CP1;
}

uint8_t MOTOR::get_pin_cp2(void)
{
  return CP2;
}

void MOTOR::enable(void)
{
  enabled = true;
  
  if (debug_on == true)
    Serial.println("Motor is enabled.");
}

void MOTOR::disable(void)
{
  enabled = false;
  stop();

  if (debug_on == true)
    Serial.println("Motor is disabled.");
}

void MOTOR::stop(void)
{
  speed = SPEED_MIN;
  digitalWrite(EN, speed);

  if (debug_on == true)
    Serial.println("Motor is stopped.");
}

void MOTOR::rotate(ROTATION const dir, uint32_t const spd)
{
  if (enabled == true)
  {
    set_direction(dir);
    set_saturated_speed(spd);

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
    
    digitalWrite(EN, speed);

    if (debug_on == true)
    {
      Serial.print("Motor is rotating (dir = ");
      Serial.print(direction);
      Serial.print(", spd = ");
      Serial.print(speed);
      Serial.println(").");
    }
  }
}
