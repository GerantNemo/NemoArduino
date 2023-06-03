#ifndef CAPTEURS_SM_H
#define CAPTEURS_SM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HygrometerSensor.h"
#include "JY901.h"
#include "MS5837.h"
#include "TinyGPS.h"
#include "Ultrasonic.h"

class capteurs_sm2
{
    public:
        capteurs_sm2();
        capteurs_sm2(int baudrate_ss,
                    int gps_pin_RX,
                    int gps_pin_TX,
                    int hygrometer_pin_analog,
                    int ultrason_trigger_pin_digital,
                    int ultrason_echo_pin_digital,
                    int tension_pin_analog);
        virtual ~capteurs_sm2();

        void get_data();

        void get_data_GPS();
        void get_data_hygrometre();
        void get_data_debitmetre();
        void get_data_tension();
        void get_data_IMU();
        void get_data_pression();
        void get_data_ultrason();
        void get_data_temperature();

        void send_data(double data, const char* header, bool header_true);

    protected:

    private:

        //SoftwareSerial *ss;
        eHygrometerKind kind;

        HygrometerSensor *Hygrometer;
        //JY901 *Imu;
        TinyGPS *Gps;
        MS5837 *Pressure;
        Ultrasonic *Ultrason;

        int voltageSensorPin;
        float vOUT = 0.0;
        float vIN = 0.0;
        float R1 = 30000.0;
        float R2 = 7500.0;

        int value;

};

#endif // CAPTEURS_SM_H
