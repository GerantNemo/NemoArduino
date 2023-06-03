#include "command_motors.h"

int cmd_dep[8] =  {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};

command_motors::command_motors()
{
    //ctor
}

command_motors::command_motors(int pinMot1, int pinMot2, int pinMot3, int pinMot4, int pinMotProf1, int pinMotProf2, int pinMotProf3, int pinMotProf4)
{
    this->pinMot1 = pinMot1; //Pin des moteure de direction
    this->pinMot2 = pinMot2;
    this->pinMot3 = pinMot3;
    this->pinMot4 = pinMot4;

    this->pinMotProf1 = pinMotProf1; //Pin des moteure de profondeur
    this->pinMotProf2 = pinMotProf2;
    this->pinMotProf3 = pinMotProf3;
    this->pinMotProf4 = pinMotProf4;

    initialisation(this->pinMot1, this->Mot1);
    initialisation(this->pinMot2, this->Mot2);
    initialisation(this->pinMot3, this->Mot3);
    initialisation(this->pinMot4, this->Mot4);

    initialisation(this->pinMotProf1, this->MotProf1);
    initialisation(this->pinMotProf2, this->MotProf2);
    initialisation(this->pinMotProf3, this->MotProf3);
    initialisation(this->pinMotProf4, this->MotProf4);

    delay(7000);

    //cmd_dep[8] =  {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};
    //send_manual_command(cmd_dep);

    //send_manual_command({1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500});

    //delay(7000);
}

command_motors::~command_motors()
{
    //dtor
}

void command_motors::initialisation(int ESCpin, Servo servo)
{
    servo.attach(ESCpin);
    servo.writeMicroseconds(1500);//Arret du moteur
    //delay(7000); //Attente de 7 secondes
    digitalWrite(ESCpin, LOW);
}

void command_motors::get_command()
{
    recvWithStartEndMarkers();
    if (newData == true) 
    {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
}

void command_motors::recvWithStartEndMarkers() 
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) 
                {
                    ndx = numChars - 1;
                }
            }
            else 
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) 
        {
            recvInProgress = true;
        }
    }
}

void command_motors::parseData() 
{      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

    verif = 1;

    strtokIndx = strtok(NULL, ",");
    num_moteur = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    cmd_moteur = atoi(strtokIndx);

    if(messageFromPC!="M" || num_moteur<0 || num_moteur>7 || cmd_moteur < 1100 || cmd_moteur > 1900)
    {
        verif = 0;
    }

    if(verif==1)
    {
        if(num_moteur==0)
            Mot1.writeMicroseconds(cmd_moteur);
        if(num_moteur==1)
            Mot2.writeMicroseconds(cmd_moteur);
        if(num_moteur==2)
            Mot3.writeMicroseconds(cmd_moteur);
        if(num_moteur==3)
            Mot4.writeMicroseconds(cmd_moteur);
        if(num_moteur==4)
            MotProf1.writeMicroseconds(cmd_moteur);
        if(num_moteur==5)
            MotProf2.writeMicroseconds(cmd_moteur);
        if(num_moteur==6)
            MotProf3.writeMicroseconds(cmd_moteur);
        if(num_moteur==7)
            MotProf4.writeMicroseconds(cmd_moteur);
    }

    if(messageFromPC=="U")
    {
        Mot1.writeMicroseconds(1500);
        Mot2.writeMicroseconds(1500);
        Mot3.writeMicroseconds(1500);
        Mot4.writeMicroseconds(1500);

        MotProf1.writeMicroseconds(1500);
        MotProf2.writeMicroseconds(1500);
        MotProf3.writeMicroseconds(1500);
        MotProf4.writeMicroseconds(1500);
    }

}

void command_motors::showParsedData() 
{
    Serial.print("Message ");
    Serial.print(messageFromPC);
    Serial.print("    Numero moteur: ");
    Serial.print(num_moteur);
    Serial.print("    Commande obtenue: ");
    Serial.print(cmd_moteur);
    Serial.print("    Validite: ");
    Serial.print(verif);

}

void command_motors::send_manual_command(int cmd[])
{
    Mot1.writeMicroseconds(cmd[0]);
    Mot2.writeMicroseconds(cmd[1]);
    Mot3.writeMicroseconds(cmd[2]);
    Mot4.writeMicroseconds(cmd[3]);

    MotProf1.writeMicroseconds(cmd[4]);
    MotProf2.writeMicroseconds(cmd[5]);
    MotProf3.writeMicroseconds(cmd[6]);
    MotProf4.writeMicroseconds(cmd[7]);
}
