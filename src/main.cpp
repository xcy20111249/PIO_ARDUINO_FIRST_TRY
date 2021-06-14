#include <Arduino.h>
 
#define MODEM_RST 5       //SIM800L复位引脚接在GPIO5
#define MODEM_PWRKEY 4    //SIM800L开关机引脚接在GPIO4
#define MODEM_POWER_ON 23 //SIM800L电源引脚接在GPIO23
#define MODEM_TX 27       //SIM800L串口TX引脚接在GPIO27
#define MODEM_RX 26       //SIM800L串口RX引脚接在GPIO26
 
#define SerialMon Serial //调试串口为UART0
#define SerialAT Serial1 //AT串口为UART1
 
void setup()
{
  pinMode(MODEM_POWER_ON, OUTPUT); //电源引脚
  pinMode(MODEM_PWRKEY, OUTPUT);   //开关机键引脚
 
  // 先打开SIM800L的电源
  digitalWrite(MODEM_POWER_ON, HIGH);
 
  //根据手册要求拉下PWRKEY 1秒钟以上 可以开机
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
  //开启串口
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
}
 
void loop()
{
  while (SerialAT.available())
  {
    SerialMon.write(SerialAT.read());
  }
  while (SerialMon.available())
  {
    SerialAT.write(SerialMon.read());
  }
}