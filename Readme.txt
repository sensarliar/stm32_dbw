for dbw stm32, on the plane
gaoming thinkpad t410i
D:\stm32\work\test_oled


��Ҫע��ĵط���
�����Ҫ��sim������Ҫ�����շ����õ�sim���ţ�
�˳���Ϊ�ɻ��˳���
�ɻ������ñ�����ַΪ��uint8_t INFO_ADDR[4];
����վ���õ�sim���ŵ�ַΪ��uint8_t MSG_TX_ADDR[4]��

�������ö̱���dbw uart2�Ĳ�����Ϊ9600������������
�Ժ�bdm910ģ���������Ĭ�ϲ�����9600.

����޸ĳ������Ҫ���룬�����ص������̱����豸�С�
���ź��ߵ�3�����߽������ء�sw��


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