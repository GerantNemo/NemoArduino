#include <Ultrasonic.h>
#include <command_motors.h>

command_motors Command(2, 3, 4, 5, 6, 7, 8, 9);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Command.get_command();

}
