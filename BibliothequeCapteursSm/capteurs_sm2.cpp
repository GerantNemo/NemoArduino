#include "capteurs_sm2.h"

char headerH[] = "H";
char headerT[] = "T";
char headerI[] = "I";
char headerP[] = "P";
char headerU[] = "U";

capteurs_sm2::capteurs_sm2()
{
    //ctor
}

capteurs_sm2::capteurs_sm2(int baudrate_ss,
                         int gps_pin_RX,
                         int gps_pin_TX,
                         int hygrometer_pin_analog,
                         int ultrason_trigger_pin_digital,
                         int ultrason_echo_pin_digital,
                         int tension_pin_analog)
{
    Wire.begin();

    voltageSensorPin = tension_pin_analog;
    vOUT = 0.0;
    vIN = 0.0;
    R1 = 30000.0;
    R2 = 7500.0;

    this->kind = ANALOG;

    //this->ss = new SoftwareSerial(gps_pin_RX, gps_pin_TX); //pinRX, pinTX (serial)

    this->Hygrometer = new HygrometerSensor(kind, hygrometer_pin_analog); //entree analogique
    //this->Imu = new JY901(); //I2C : SDA, SCL
    this->Gps = new TinyGPS();
    this->Pressure = new MS5837(); //I2C : SDA, SCL
    this->Ultrason = new Ultrasonic(ultrason_trigger_pin_digital, ultrason_echo_pin_digital);

    //ss->begin(baudrate_ss);

    while (!Pressure->init())
    {
        Serial.println("Init failed!");
        Serial.println("Are SDA/SCL connected correctly?");
        Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
        Serial.println("\n\n\n");
        delay(5000);
    }

    Pressure->setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

    //Imu->StartIIC();
    JY901.StartIIC();

}

capteurs_sm2::~capteurs_sm2()
{
    //dtor
    //delete ss;
    //ss = NULL;
    delete Hygrometer;
    Hygrometer = NULL;
    //delete Imu;
    //Imu = NULL;
    delete Gps;
    Gps = NULL;
    delete Pressure;
    Pressure = NULL;
    delete Ultrason;
    Ultrason = NULL;
}

void capteurs_sm2::get_data()
{
    //vector<float> data;
    void get_data_IMU(); //17
    void get_data_pression();//3
    void get_data_ultrason(); //1
    void get_data_tension(); //1
    void get_data_hygrometre(); //1
    void get_data_temperature(); //0
    void get_data_GPS(); //0
    void get_data_debitmetre(); //0

    /*data_tab[0] = millis();
    for(int i=1; i<24; i++)
    {
        data_tab[i] = data[i];
    }*/
}

void capteurs_sm2::get_data_GPS()
{

}

void capteurs_sm2::get_data_hygrometre() //longueur : 1
{
    //const char headerH = "H";
    send_data(Hygrometer->readHumidityValue(), headerH, 1);
}

void capteurs_sm2::get_data_debitmetre() // longueur : 0
{

}

void capteurs_sm2::get_data_tension() // longueur : 1
{

    value = analogRead(voltageSensorPin);
    vOUT = (value * 5.0) / 1024.0;
    vIN = vOUT / (R2/(R1+R2));

    //const char headerT = "T";
    send_data(vIN, headerT, 1);
}

void capteurs_sm2::get_data_IMU() // longueur : 17
{
    //const char headerI = "I";
    
    JY901.GetAcc();
    send_data((double)JY901.stcAcc.a[0]/32768*16, headerI, 1);
    send_data((double)JY901.stcAcc.a[1]/32768*16, headerI, 0);
    send_data((double)JY901.stcAcc.a[2]/32768*16, headerI, 0);

    JY901.GetGyro();
    send_data((double)JY901.stcGyro.w[0]/32768*2000, headerI, 0);
    send_data((double)JY901.stcGyro.w[1]/32768*2000, headerI, 0);
    send_data((double)JY901.stcGyro.w[2]/32768*2000, headerI, 0);

    JY901.GetAngle();
    send_data((double)JY901.stcAngle.Angle[0]/32768*180, headerI, 0);
    send_data((double)JY901.stcAngle.Angle[1]/32768*180, headerI, 0);
    send_data((double)JY901.stcAngle.Angle[2]/32768*180, headerI, 0);

    JY901.GetMag();
    send_data((double)JY901.stcMag.h[0], headerI, 0);
    send_data((double)JY901.stcMag.h[1], headerI, 0);
    send_data((double)JY901.stcMag.h[2], headerI, 0);

    JY901.GetPress();
    send_data((double)JY901.stcPress.lPressure, headerI, 0);

    JY901.GetQuaternion();
    send_data((double)JY901.stcQuater.q0, headerI, 0);
    send_data((double)JY901.stcQuater.q1, headerI, 0);
    send_data((double)JY901.stcQuater.q2, headerI, 0);
    send_data((double)JY901.stcQuater.q3, headerI, 0);
}

void capteurs_sm2::get_data_pression() //longueur : 3
{
    //const char headerP = "P";
    Pressure->read();
    send_data(Pressure->pressure(), headerP, 1);
    send_data(Pressure->temperature(), headerP, 0);
    send_data(Pressure->depth(), headerP, 0);
}

void capteurs_sm2::get_data_ultrason() // longueur : 1
{
    //const char headerU = "U";
    send_data(Ultrason->read() * 0.01, headerU, 1);//sortie en cm par defaut
}

void capteurs_sm2::get_data_temperature() // longueur : 0
{

}

void capteurs_sm2::send_data(double data, const char* header, bool header_true)
{
    union{
        double myDouble;
        unsigned char myChars[sizeof(double)];
    } data_byte;

    data_byte.myDouble = data;

    if (header_true == 1)
        Serial.write(header);
    for (int i = 0; i < sizeof(double); i++)
    {
        Serial.write(data_byte.myChars[i]);
    }
}


