for dbw stm32, on the plane
gaoming thinkpad t410i
D:\stm32\work\test_oled


需要注意的地方：
如果需要换sim卡，需要配置收发所用的sim卡号：
此程序为飞机端程序。
飞机端配置本机地址为：uint8_t INFO_ADDR[4];
地面站配置的sim卡号地址为：uint8_t MSG_TX_ADDR[4]；

现在配置短报文dbw uart2的波特率为9600。测试正常。
以后bdm910模块出厂后都用默认波特率9600.

如果修改程序后，需要编译，并下载到北斗短报文设备中。
用信号线的3条短线进行下载。sw口


--------------------------- Drives/USART/dbw_func.c ---------------------------
index 915d9f4..54effd0 100644
@@ -13,11 +13,13 @@ int16_t hott_msg_len = 0;
 //uint8_t MSG_TO_SEND[43]="WWW.UCORTEX.COM---gaoming,gaoming,nihao ma";
 uint8_t INFO_HEAD[6]="$TXSQ";   //5B
 uint8_t INFO_LEN[2]={0x33,0x34};  //2B
-uint8_t INFO_ADDR[4]={0x04,0xE6,0x10};//321040  addr 3B
+//uint8_t INFO_ADDR[4]={0x04,0xE6,0x10};//321040  addr 3B benji
+uint8_t INFO_ADDR[4]={0x02,0xF5,0x48};//193864,benji
 //uint8_t MSG_TX_FLAG=0B01000110;     //1B
 uint8_t MSG_TX_FLAG=0x46;     //1B
 //uint8_t MSG_TX_ADDR[4]={0x04,0xE6,0x10};//321040  TX addr   //3B
-uint8_t MSG_TX_ADDR[4]={0x04,0x73,0xEE};//291822  TX addr   //3B
+//uint8_t MSG_TX_ADDR[4]={0x04,0x73,0xEE};//291822  TX addr   //3B
+uint8_t MSG_TX_ADDR[4]={0x02,0xF5,0x4C};//193868
 uint8_t MSG_TX_LEN[2]={0x00,0x00};      //2B
 uint8_t MSG_TX_ACK=0x00;               //1B
 uint8_t MSG_TX[211]="gaoming";    //1680bit maxium   NON-MIL 628BIT  78B 