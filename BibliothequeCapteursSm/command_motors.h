#ifndef COMMAND_MOTORS_H
#define COMMAND_MOTORS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Servo.h>

class command_motors
{
    public:
        command_motors();
        command_motors(int pinMot1, int pinMot2, int pinMot3, int pinMot4, int pinMotProf1, int pinMotProf2, int pinMotProf3, int pinMotProf4);
        virtual ~command_motors();

        void initialisation(int ESCpin, Servo servo);
        void get_command();
        void recvWithStartEndMarkers();
        void parseData();
        void send_manual_command(int cmd[]);
        void showParsedData();

    protected:

    private:

        int pinMot1; //Pin des moteure de direction
        int pinMot2;
        int pinMot3;
        int pinMot4;

        int pinMotProf1; //Pin des moteure de profondeur
        int pinMotProf2;
        int pinMotProf3;
        int pinMotProf4;

        Servo Mot1;
        Servo Mot2;
        Servo Mot3;
        Servo Mot4;

        Servo MotProf1;
        Servo MotProf2;
        Servo MotProf3;
        Servo MotProf4;

        const byte numChars = 12;
        char receivedChars[numChars];
        char tempChars[numChars];        // temporary array for use when parsing

        // variables to hold the parsed data
        char messageFromPC[numChars] = {0};
        int num_moteur = 0;
        int cmd_moteur = 0;
        //float floatFromPC = 0.0;
        bool verif = 1;
        boolean newData = false;

        //int cmd_dep[];
};

#endif // COMMAND_MOTORS_H
