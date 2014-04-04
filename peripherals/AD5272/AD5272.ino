/*
This application is for connecting a AnalogDevices AD5272 series digital potentiometer via I2C interface.
Datasheet can be downloaded here: 
http://www.analog.com/en/digital-to-analog-converters/digital-potentiometers/ad5272/products/product.html
The AD5272 scales the max resistor value in 1024 steps which can be selected via I2C.

Resistor value can be accessed via UART with simply sending a new valid wiper position value.

On booting up the AD5272 the resitor value is set to mid-range (value 511)
change of the wiper positioin is disabled on default and needs to be enabled.

! CAUTION ! 
The 50-TP register can only be written 50.times. After this number of write access to the register the 50-TP value cannot be
updated again ! 
! CAUTION ! 

last update: 2013-09-05
version: I2C_v0.1

-------------------------------------------------------------------------------------
AVAILABLE FUNCTIONS:
-------------------------------------------------------------------------------------
void enableWiperPositionChange()
void setControlRegister(int c0, int c1, int c2)
int getControlRegister()
void setResistorValue(int inputValue)
int readRDAC()
double calcResistorValue(int maxKR)
void writeRDACto50TP()
void write50TPtoRDAC()
int readTP50content(int addr)
int readLastTP50registerAddr()
void shutdown()
void nopFunction()

int receiveI2C()
void serialEvent()
void loop()
void setup()
-------------------------------------------------------------------------------------


*/


#include <Wire.h>

int button_state = 0;
int newValue = 0;
int lastValue = 0;
int ADADD = 47;
int ADtype = 100;
int debugOutput = 1;                 // enable debug output println

void setup() {                

  pinMode(P1_0, OUTPUT);             // enable RED LED as inductor for transmission
  digitalWrite(P1_0, LOW);
  pinMode(P1_3, INPUT_PULLUP);       // enable P1.3 as input for the Button
  Wire.begin();                      // initialize I2C interface controller 
  
  Serial.begin(9600);
  delay(500);
  Serial.println(" ");
  Serial.println("-----------------------------------------------------------------");
  Serial.println("AD5272 control interface");
  Serial.println("-----------------------------------------------------------------");
  delay(100);
  Serial.println("--> Enable RDAC write access....");
  Serial.print("Configuration for ");
  Serial.print(ADtype);
  Serial.println("kOhm max resistor");
  enableWiperPositionChange();
  
}

void loop() {
  button_state = digitalRead(P1_3);       // check for Button on P1.3 pressed
  if (button_state == LOW) {   
    
    //Serial.print("Reading RDAC: D=");
    //int cInt = readRDAC();
    //Serial.print(cInt);                   
    //Serial.println(" ... DONE ");
    calcResistorValue(ADtype);
    
    //int lastRegister = readLastTP50registerAddr();
    //Serial.print("Last Register Addr.: ");
    //Serial.println(lastRegister);
    
    //int TPvalue = readTP50content(1);
    //Serial.print("TP50 value of Register 1: ");
    //Serial.println(TPvalue);
    
    //write50TPtoRDAC();
    
    delay(500);                           // required for un-polling the button
  }
}


/*
Command number 7: This command is used to unlock the RDAC register write protection
! only call this function after initialisation phase. This function sets the control register to its default value except of 
enabling the wiper position change bit.
For modification on the configuration register use >> setControlRegister << instead.
*/
void enableWiperPositionChange() {
     Wire.beginTransmission(ADADD);
     Wire.write(28);
     Wire.write(2);
     Wire.endTransmission();
}


/*
This functions sends command #7 from the documentation to the AD5272 to set the configuration for the wiper and the TP register

c0:      50-TP program enable
         0 = 50-TP program disabled (default)
         1 = enable device for 50-TP program
         
c1:      RDAC register write protect
         0 = wiper position frozen to value in 50-TP memory (default)
         1 = allow update of wiper position through a digital interface
         
c2:      Resistor performance enable
         0 = RDAC resistor tolerance calibration enabled (default)
         1 = RDAC resistor tolerance calibration disabled
         
c3:      50-TP memory programm success bit
         0 = fuse program command unsuccessful (default)
         1 = fuse program command successful
         
control register Bitmap: 
| DB9 | DB8 | DB7 | DB6 | DB5 | DB4 | DB3 | DB2 | DB1 | DB0 |
|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
|  0  |  0  |  0  |  0  |  0  |  0  |  C3 |  C2 |  C1 |  C0 |
*/
void setControlRegister(int c0, int c1, int c2) {
  int config_value = 0;
  if(c0 == 1) {
    config_value = config_value + 1;
  }
  if(c1 == 1) {
    config_value = config_value + 2;
  }
  if(c2 == 1) {
    config_value = config_value + 4;
  }
  //if(c3 == 1) {
  //  config_value = config_value + 8;
  //}
  
  Wire.beginTransmission(ADADD);
  Wire.write(28);
  Wire.write(config_value);
  Wire.endTransmission();
}

/*
Reads the control register (command #8)

c0:      50-TP program enable
         0 = 50-TP program disabled (default)
         1 = enable device for 50-TP program
         
c1:      RDAC register write protect
         0 = wiper position frozen to value in 50-TP memory (default)
         1 = allow update of wiper position through a digital interface
         
c2:      Resistor performance enable
         0 = RDAC resistor tolerance calibration enabled (default)
         1 = RDAC resistor tolerance calibration disabled
         
c3:      50-TP memory programm success bit
         0 = fuse program command unsuccessful (default)
         1 = fuse program command successful
         
control register Bitmap: 
| DB9 | DB8 | DB7 | DB6 | DB5 | DB4 | DB3 | DB2 | DB1 | DB0 |
|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|
|  0  |  0  |  0  |  0  |  0  |  0  |  C3 |  C2 |  C1 |  C0 |
*/
int getControlRegister() {
   Wire.beginTransmission(ADADD);
   Wire.write(32);
   Wire.write(0);
   Wire.endTransmission();
   
   int cInt = receiveI2C();
   
   /*if(debugOutput == 1) {
     Serial.print("Configuration register set to: ");
     Serial.println(cInt);
   }*/
   return cInt;
}


/*
  sets a new Value into the RDAC register on the AD5272 and changes the current wiper position.
  This command is supported after enabling the wiper position change only. 
  To enable this feature the control register needs to be set via changeControlRegister() or enableWiperPositionChange() before
  setting a new Resistor value 
  
  Maximum value D=1023 
  R=(D/1024)*Rmax (for 20kOhm 1*D = 19.53Ohm)
*/
void setResistorValue(int inputValue) {
 
   if(debugOutput == 1) {
     Serial.print("-> setting up new RADC Register value .... D=");
   }
   
   // Generating sequence for given value
   if(inputValue < 255) {
     Wire.beginTransmission(ADADD);
     Wire.write(4);
     Wire.write(inputValue);
     Wire.endTransmission();
   }
   else if(inputValue < 512) {
     Wire.beginTransmission(ADADD);
     Wire.write(5);
     Wire.write((inputValue-256));
     Wire.endTransmission();
   }
   else if(inputValue < 768) {
     Wire.beginTransmission(ADADD);
     Wire.write(6);
     Wire.write((inputValue-512));
     Wire.endTransmission();
   }
   else if(inputValue <= 1024) {
     Wire.beginTransmission(ADADD);
     Wire.write(7);
     Wire.write((inputValue-768));
     Wire.endTransmission();
   }
   else {
     if(debugOutput == 1) {
       Serial.println("no valid value given ... Enter value for D in range [0 ... 1023]");
     }
   }
       
   // test command for writing the RDAC register to the lowes possible value (0)
   
   
   Serial.print(inputValue);
   if(inputValue == readRDAC()) {
     if(debugOutput == 1) {
       Serial.println("... SUCCESS");
     }
   }
   else {
     if(debugOutput == 1) {
       Serial.println("... FAILED");
     }
   }
}


int readRDAC() {   // command 1 from the Datasheet
  /*
  Reads the current setting of the RDAC register on the AD5272
  */
  Wire.beginTransmission(ADADD);
  Wire.write(8);
  Wire.write(0);
  Wire.endTransmission();
  int cInt = receiveI2C();
  return cInt;
}


/*
Calculates the approximate resistor value. The calculated values may differ from the real ones and need to be verified first
*/
double calcResistorValue(int maxKR) {
  int wiperPosition = readRDAC();
  double valueR = ( maxKR / 1024.00 )* wiperPosition;
  
  /*if (debugOutput == 1) {
    Serial.print("approx resistor value: ");
    Serial.print(valueR);
    Serial.println("kOhm");
  }*/
  return valueR;  
}

/*
command #3: store wiper Setting:
Stores the current RDAC setting to the 50-TP register
*/
void writeRDACto50TP() {
   Wire.beginTransmission(ADADD);
   Wire.write(12);
   Wire.write(0);
   Wire.endTransmission();
}


/*
command #4: software reset: 
function loads the last 50-TP value to the RDAC register
*/
void write50TPtoRDAC() {
   Wire.beginTransmission(ADADD);
   Wire.write(16);
   Wire.write(0);
   Wire.endTransmission(); 
}


/*
Reads contents of 50-TP from the SDO output in the next frame
*/
int readTP50content(int numRegister) {
   Wire.beginTransmission(ADADD);
   Wire.write(20);
   Wire.write(numRegister-1);
   Wire.endTransmission();
   
   int cInt = receiveI2C();
   return cInt;
}


/*
Reads the address of the last written TP-50 register
*/
int readLastTP50registerAddr() {
   Wire.beginTransmission(ADADD);
   Wire.write(24);
   Wire.write(0);
   Wire.endTransmission(); 
   
   int cInt = receiveI2C();
   return cInt;
}


/*
function for software shutdown of the AD2572
*/
void shutdown() {
  Wire.beginTransmission(ADADD);
  Wire.write(36);
  Wire.write(0);
  Wire.endTransmission(); 
}

/*
NOP function
*/
void nopFunction() {
  Wire.beginTransmission(ADADD);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();  
}


/*
this function receives 2 Bytes from the I2C interface and returns the result
*/
int receiveI2C() {
   Wire.beginTransmission(ADADD);
   Wire.requestFrom(ADADD, 2);
   int cInt = 0;
   int counter = 0;
   while(Wire.available())    // slave may send less than requested
    { 
     int c = Wire.read();    // receive a byte as character
     counter = counter + 1;
     if(counter == 1) {
       cInt = c*256;
     }
     if(counter != 1) {
       cInt = cInt+c;
     }
    }
   Wire.endTransmission();
   return cInt;
}


/*
calculates the wiper setting according to a given resistor value
*/
int getWiperFromResistance(int resistance) {
  int D = resistance *1024/ADtype;
  setResistorValue(D);
  return D;
}

/*
UART input handling 
!! NEEDS TO BE UPDATED FOR RECEIVING COMMANDS !! 
*/
void serialEvent() {
  char inCommand[50] = "";
  int counter = 0 ;
  int commandID = 0;
  int preCommand = 0;
  int cInt = 0;
  int inValue = 0;
  int c0 = 0;
  int c1 = 0;
  int c2 = 0;
  int c3 = 0;
  
  while (Serial.available()) {
      //delay_change(Serial.parseInt());
      inCommand[counter] = Serial.read();
      counter = counter + 1;
      /*
      newValue = Serial.parseInt();
      if(newValue < 1024 && newValue >= 0) {
        setResistorValue(newValue);
        lastValue = newValue;
      }
      else {
        newValue = lastValue;
      }
      */
      
      //preCommand = Serial.peek();
      //if(preCommand == '?') {
      //  commandID = 100;
      //}
      //else {
         commandID = Serial.parseInt();
      //}
      
      switch(commandID) {
        /*case 0:
          if(debugOutput == 1) {
            Serial.println("CMD0 --> NOP");
          }
          // void nopFunction();     // do nothing or call nopFunction. To avoid unnecesarry traffic > uncomment function call
          break;
          */
        case 1:
          newValue = Serial.parseInt();
          setResistorValue(newValue);
          if(debugOutput == 1) {
            Serial.print("CMD1 --> Setting Wiper to position: ");
            Serial.println(newValue);
            Serial.print(" >>> approx. ");
            Serial.print(calcResistorValue(ADtype));
            Serial.println("kOhm");
          }
         break;
         
        case 2:
          cInt = readRDAC();
          if(debugOutput == 1) {
            Serial.print("CMD2 --> ");
            Serial.print("Reading RDAC: D=");
          }
          Serial.println(cInt);
          if(debugOutput == 1) {
            Serial.print(" >>> approx. ");
            Serial.print(calcResistorValue(ADtype));
            Serial.println("kOhm");
          }    
         break;
         
        case 3:
          writeRDACto50TP();
          if(debugOutput == 1) {
            Serial.println("CMD3 --> Storing Wiper setting to TP50 next register");
          }
         break;
         
        case 4:
          write50TPtoRDAC();
          if(debugOutput == 1) {
              Serial.println("CMD4 --> Software reset: last50tTP to RDAC");
              Serial.print(" >>> approx. ");
              Serial.print(calcResistorValue(ADtype));
              Serial.println("kOhm");
          }
         break;
         
        case 5:
          inValue = Serial.parseInt();
          cInt = readTP50content(inValue);
          if(debugOutput == 1) {
              Serial.print("CMD5 --> read content of 50TP register: REG: #");
              Serial.print(inValue);
              Serial.print("  VALUE: ");
          }
          Serial.println(cInt);
         break;
         
        case 6:
          cInt = readLastTP50registerAddr();
          if(debugOutput == 1) {
              Serial.print("CMD6 --> read addr. of last 50TP register: ADDR: ");
          }
          Serial.println(cInt);
         break;
         
        case 7:
           cInt = Serial.parseInt();
           if(cInt <= 15) {
            while (cInt > 0) {
                if(cInt >= 8) {
                  c3 = 1;
                  cInt = cInt - 8;
                }
                else if(cInt >= 4) {
                  c2 = 1;
                  cInt = cInt - 4; 
                }
                else if(cInt >= 2) {
                  c1 = 1;
                  cInt = cInt - 2;
                }
                else if(cInt >= 1) {
                  c0 = 1;
                  cInt = cInt - 1;
                }
            }
          }
           setControlRegister(c0, c1, c2);
           if(debugOutput == 1) {
              Serial.println("CMD7 --> Write new value to the control register: ");
              if(c0 == 0) {
                Serial.println("C0= 0  50-TP program disabled (default)");
              }
              if(c0 == 1) {
                  Serial.println("C0= 1  enable device for 50-TP program");
              }
              if(c1 == 0) {
                Serial.println("C1= 0  wiper position frozen to value in 50-TP memory (default)");
              }
              if(c1 == 1) {
                  Serial.println("C1= 1  allow update of wiper position through a digital interface");
              }
              if(c2 == 0) {
                Serial.println("C2= 0  RDAC resistor tolerance calibration enabled (default)");
              }
              if(c2 == 1) {
                  Serial.println("C2= 1  RDAC resistor tolerance calibration disabled");
              }
              if(c3 == 0) {
                Serial.println("C3= 0  fuse program command unsuccessful (default)");
              }
              if(c3 == 1) {
                  Serial.println("C3= 1  fuse program command successful");
              }  
           }
         break;
         
        case 8:
          cInt = getControlRegister();
          Serial.println(cInt);
          if(cInt <= 15) {
            while (cInt > 0) {
                if(cInt >= 8) {
                  c3 = 1;
                  cInt = cInt - 8;
                }
                else if(cInt >= 4) {
                  c2 = 1;
                  cInt = cInt - 4; 
                }
                else if(cInt >= 2) {
                  c1 = 1;
                  cInt = cInt - 2;
                }
                else if(cInt >= 1) {
                  c0 = 1;
                  cInt = cInt - 1;
                }
            }
          }
          if(debugOutput == 1) {
              Serial.println("CMD8 --> Read the control register");
              if(c0 == 0) {
                Serial.println("C0= 0  50-TP program disabled (default)");
              }
              if(c0 == 1) {
                  Serial.println("C0= 1  enable device for 50-TP program");
              }
              if(c1 == 0) {
                Serial.println("C1= 0  wiper position frozen to value in 50-TP memory (default)");
              }
              if(c1 == 1) {
                  Serial.println("C1= 1  allow update of wiper position through a digital interface");
              }
              if(c2 == 0) {
                Serial.println("C2= 0  RDAC resistor tolerance calibration enabled (default)");
              }
              if(c2 == 1) {
                  Serial.println("C2= 1  RDAC resistor tolerance calibration disabled");
              }
              if(c3 == 0) {
                Serial.println("C3= 0  fuse program command unsuccessful (default)");
              }
              if(c3 == 1) {
                  Serial.println("C3= 1  fuse program command successful");
              }
          }
          
         break;
         
        case 9:
          shutdown();
          if(debugOutput == 1) {
              Serial.println("CMD9 --> Software Shutdown");
          }
         break; 
        
        case 10:
          inValue = Serial.parseInt();
          cInt = readTP50content(inValue);
          setResistorValue(cInt);
          if(debugOutput == 1) {
              Serial.println("CMD10 --> _read 50TP register and write data to RDAC");
              Serial.print("50TP value: ");
              Serial.println(cInt);
          }
          break;
          
          case 11:
            inValue = Serial.parseInt();
            ADtype = inValue;
            if(debugOutput == 1) {
              Serial.println("CMD11 --> change MAX R configuration");
              Serial.print("new max R value: ");
              Serial.println(ADtype);
            }
            break;
          
          case 12:
            inValue = Serial.parseInt();
            if (inValue == 0) {
              debugOutput = 0;
            }
            else {
              debugOutput = 1;
            }
            //if(debugOutput == 1) {
              Serial.println("CMD12 --> change debug_output");
              Serial.print("debugOutput: ");
              Serial.println(debugOutput);
            //}
            break;
            
            case 13: 
              inValue = Serial.parseInt();
              getWiperFromResistance(inValue);
              if(debugOutput == 1) {
                Serial.println("CMD13 --> set Wiper to a pecific resistance in kOhm");
                Serial.print("Wiper position set to: ");
                Serial.println(inValue);
              }
              
              
           
           default:
           //Serial.println("CMD0            --> NOP");
           Serial.println("CMD1  <INTEGER> --> Setting Wiper to position");
           Serial.println("CMD2            --> Reading RDAC ");
           Serial.println("CMD3            --> Storing Wiper setting to TP50 next register");
           Serial.println("CMD4            --> Software reset");
           Serial.println("CMD5  <INTEGER> --> read content of 50TP register");
           Serial.println("CMD6            --> read addr. of last 50TP register");
           Serial.println("CMD7  <INTEGER> --> Write new value to the control register");
           Serial.println("CMD8            --> Read the control register");
           Serial.println("CMD9  <INTEGER> --> Software Shutdown");
           Serial.println("CMD10 <INTEGER> --> _read 50TP register and write data to RDAC");
           Serial.println("CMD11 <INTEGER> --> change MAX R configuration");
           Serial.println("CMD12 <INTEGER> --> change debug_output");
           Serial.println("CMD13 <INTEGER> --> set Wiper to a pecific resistance in kOhm");
      }
  }
  //Serial.print(inCommand);
  
}


