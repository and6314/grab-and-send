#include <GSM.h>
#include <RCSwitch.h>

#define PINNUMBER ""  //здесь записать пин код, если пин кода нет, оставить пустым

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;
RCSwitch mySwitch = RCSwitch();
char telNum="+79315354472"; //номер телефона по которому будет отправлено сообщение
static const char* bin2tristate(const char* bin);
static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySwitch.enableReceive(0);
  boolean notConnected = true;
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if (mySwitch.available()) {
    int decimal = mySwitch.getReceivedValue();
    int lenght = mySwitch.getReceivedBitlength();
    const char* b = dec2binWzerofill(decimal, lenght);
    b = strcat(b,"\n");
    b = strcat(b,bin2tristate( b));
    Serial.println( b ); 
    sms.beginSMS(telNum);
    sms.print(b);
    sms.endSMS();
    mySwitch.resetAvailable();
  }

}


static const char* bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}



