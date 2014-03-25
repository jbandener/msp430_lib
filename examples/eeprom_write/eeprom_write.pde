/*
* Source code for Processing (processing.org) for sending 8Bit values via UART to the MSP430
* eeprom.c stored in this folder is the counter part on the MSP430 to receive the bytes and
* write them to an EEPROM via I2C interface.
*/


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

char[] edid_content = {0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x4d, 0x29, 0x35, 0x91, 0x01};

void setup() {
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 9600);
  print(portName);
  //myPort.write(0x56);
  
  for (int i = 0; i < 256; i = i+1) {
    myPort.write(edid_content[i]);
    //myPort.write(0x00);
    delay(50);
    //millis(100);
  }
}

