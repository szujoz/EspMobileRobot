#include "MPU9250.h"

class Imu
{
  private:
    MPU9250 *imu;
    int     status;
    float   acc[3];
    float   gyr[3];
    float   mag[3];
    float   tempr;

  public:
    Imu();
    ~Imu();

    void getVectorAccel(float* const acc);
    void getVectorGyro (float* const gyro);
    void getVectorMagn (float* const magn);
      
    void start(bool const debugON);
    void process();
};
