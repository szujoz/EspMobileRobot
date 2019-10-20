#include "imu.h"

Imu::Imu()
{
  imu = new MPU9250(Wire,0x68);
  status = 0;
}

Imu::~Imu()
{
  free(imu);
}

void Imu::getVectorAccel(float* const accel)
{
  *accel = *acc;
}

void Imu::getVectorGyro (float* const gyro)
{
  *gyro = *gyr;
}

void Imu::getVectorMagn (float* const magn)
{
  *magn = *mag;
}

void Imu::start(bool const debugON)
{
  status = imu->begin();
  
  if (status < 0) 
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    //while(1) {}
  }
  // setting the accelerometer full scale range to +/-8G 
  imu->setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  imu->setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  imu->setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  
  // setting SRD to 19 for a 50 Hz update rate
  imu->setSrd(19);
};

void Imu::process()
{
    // read the sensor
  imu->readSensor();

  // Save the results
  acc[0] = imu->getAccelX_mss();
  acc[1] = imu->getAccelY_mss();
  acc[2] = imu->getAccelZ_mss();

  gyr[0] = imu->getGyroX_rads();
  gyr[1] = imu->getGyroY_rads();
  gyr[2] = imu->getGyroZ_rads();

  mag[0] = imu->getMagX_uT();
  mag[1] = imu->getMagX_uT();
  mag[2] = imu->getMagX_uT();

  tempr  = imu->getTemperature_C();
  //Serial.println(imu->getTemperature_C(),6);
}
