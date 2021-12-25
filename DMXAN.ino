#include <TeensyDMX.h>

namespace teensydmx = ::qindesign::teensydmx;

// Create the DMX sender on Serial1.
teensydmx::Sender dmxTx{Serial1};

// Data for 3 channels.
uint8_t data[3]{0x44, 0x88, 0xcc};

char commands[4];
char temp;
int channel;
int value;
int dmxSpeed;
int channelToChase;
int channelToChaseValue;
bool runChase;

void setup() {
  // put your setup code here, to run once:

  pinMode(1, OUTPUT_OPENDRAIN);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWriteFast(LED_BUILTIN, HIGH);

  Serial.begin(9600); 

  /*while(!Serial){
    // wait for serial monitor connect
  }*/

  Serial.println("Starting DMX Test");
  delay(1000);

  dmxSpeed = 1000;
  
  commands[0] = 0x00;
  commands[1] = 0x00;
  commands[2] = 0x00;
  commands[3] = 0x00;
  channel = 0;
  channelToChaseValue = 0;
  channelToChase = 1;
  runChase = false;

  dmxTx.setRefreshRate(44);
  
  // Set channels 10-12 to the 3 values in 'data'
  // Call this after setting up the channel contents if you want to
  // guarantee that the values are used as the initial contents;
  // transmission doesn't start until after begin() is called.
  // If it doesn't matter, begin() can be called before setting the
  // channel contents.
  dmxTx.begin();

}

 

void loop() {

  for(;;){

    uint DMXchannel = 1;

  if(runChase){

    //Serial.println("in chase loop");
    delay(dmxSpeed);
    dmxTx.set(channelToChase, channelToChaseValue);
    channelToChaseValue++;
    
    if(channelToChaseValue == 256){
      channelToChaseValue = 0;
    }
  }
  
  
  
  
  int byteSize = Serial.readBytes(commands, 4);
  
  if(byteSize > 0){

    //Serial.println("checking command");
    
    switch(commands[0]){

        case 'C':
        DMXchannel = commands[1] + commands[2];
        if( DMXchannel > 512 ){
          Serial.println("BAD");
          break;
        }
        else{
          dmxTx.set(DMXchannel, commands[3]); // (channel, value)
          Serial.println("K");
          break;
        }
        break;

        case 'R':
        if(commands[1] > 0){
          runChase = true;
          channelToChase = commands[1];
          Serial.println("K");
        }
        else{
          Serial.println("BAD");
        }
        
        break;

        case 'X':
        runChase = false;
        channelToChaseValue = 0;
        Serial.println("K");
        break;

        case 'S':
        dmxSpeed = commands[1];
        dmxSpeed *= 10;
        Serial.print("speed have been set to ");
        Serial.print(dmxSpeed);
        Serial.println("ms");
        break;
        
        case 'T':
        Serial.println("test response from Teensy 4.1");
        break;
      
        case 'P':
        Serial.println("pong");
        break;

        case 0x0A:
        break;

        case 'B':
        for(channel = 1; channel < 513; channel++){

          dmxTx.set(channel, 0);  // (channel, value)
        
        }
        Serial.println("K");
        break;
      
        default:
        Serial.println("BAD");
        break;
      }
    }

  }


}
