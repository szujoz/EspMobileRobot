#include <Arduino.h>

enum ROTATION
{
  CLOCKWISE        = 0,
  ANTI_CLOCKWISE
};

class MOTOR
{
  private:
    // Hardware connections: enable, control pin 1, control pin 2.
    uint8_t   EN;
    uint8_t   CP1;
    uint8_t   CP2;

    // Motor operational properties.
    bool      enabled;
    ROTATION  direction;
    uint8_t   speed;

    // Debug
    bool      debug_on;
  
  public:
    MOTOR(uint8_t const en, uint8_t const cp1, uint8_t const cp2, bool const deb);

    void    set_saturated_speed(uint32_t const spd);
    void    set_direction(ROTATION const dir);
    uint8_t get_pin_en(void);
    uint8_t get_pin_cp1(void);
    uint8_t get_pin_cp2(void);

    void    enable(void);
    void    disable(void);
    void    stop(void);
    void    rotate(ROTATION const dir, uint32_t const spd);
};
