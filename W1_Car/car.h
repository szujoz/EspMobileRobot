#include "motor.h"
#include "imu.h"
#include "webserver.h"

enum STRAIGHT_DIRECTION
{
  AHEAD   = 0,
  REVERSE
};

enum TURN_DIRECTION
{
  RIGHT = 0,
  LEFT
};

class CAR
{
  private:
    MOTOR*     motor_right;
    MOTOR*     motor_left;
    // net
    WEBSERVER* net;
    //imu
    Imu*       imu;
    uint8_t    speed;

    uint32_t   convert_Hex2Dec (char* const arr);
    
  public:
    CAR(const uint8_t r_en, const uint8_t r_cp1, const uint8_t r_cp2,
        const uint8_t l_en, const uint8_t l_cp1, const uint8_t l_cp2);
    ~CAR(void);

    void process(void);
  
    void engine_on(void);
    void engine_off(void);
    void stop(void);
    void straight(const STRAIGHT_DIRECTION dir, const uint8_t spd);
    void rotate(const TURN_DIRECTION dir, const uint32_t spd);
    void turn_around(void);
};
