#pragma once
#include "core/Data.h"

class IMUData : public Data {
public:
    double x_acc, y_acc, z_acc;
    double pitch, roll, yaw;
    double x_gyro, y_gyro, z_gyro;
    double x_mag, y_mag, z_mag;
    
    IMUData(double ts, double xa, double ya, double za,
            double p, double r, double y,
            double xg, double yg, double zg,
            double xm, double ym, double zm)
        : Data(ts), x_acc(xa), y_acc(ya), z_acc(za),
          pitch(p), roll(r), yaw(y),
          x_gyro(xg), y_gyro(yg), z_gyro(zg),
          x_mag(xm), y_mag(ym), z_mag(zm) {}
    
    std::string toString() const override;
};