#ifndef _OLED_H_
#define _OLED_H_

/***
 *SPIӲ������SSD1306 OLED 
 *ģ����4��SPI�ӿ� Ƭѡ�Ѿ�Ӳ���õ�
 *D/C���ߴ����������� ���ʹ�����������
 *SDA:�������� SCL:ʱ������
 ****/
//#define     RCC_APB2Periph_OLED_PORT        RCC_APB2Periph_GPIOA
//#define  OLED_DC      PEout(0)
//#define  OLED_RST     PEout(1)
//#define  OLED_PORT    GPIOA

//#define  OLED_SCL_PIN					GPIO_Pin_5
//#define	 OLED_SDA_PIN					GPIO_Pin_7 

#define     XLevelL		    0x00
#define     XLevelH		    0x10
#define     XLevel		    ((XLevelH&0x0F)*16+XLevelL)
#define     Max_Column	    128
#define     Max_Row		    64
#define	    Brightness	    0xCF
#define     X_WIDTH         128
#define     Y_WIDTH         64
#define		Page			8

void OLED_WB(uint8_t data);
/*********************OLEDд����************************************/ 
void OLED_WrDat(uint8_t data);
/*********************OLEDд����************************************/
void OLED_WrCmd(uint8_t cmd);
/*********************�趨����************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y);
/*********************OLEDȫ������************************************/
void OLED_Fill(unsigned char bmp_dat);
/*********************OLED��λ************************************/
void OLED_CLS(void);
/*********************OLED��ʼ��************************************/
void OLED_Init(void);
/***************function ��ʾ6*8һ���׼ASCII�ַ��� ��ʾ������(x,y),yΪҳ��Χ0~7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
/*******************function ��ʾ8*16һ���׼ASCII�ַ��� ��ʾ����(x,y),yΪҳ��Χ****************/
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
/*****************��������:��ʾ16*16���� ��ʾ����(x,y),yΪҳ��Χ****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned char N);
/*****************��������:��ʾ32*32���� ��ʾ������(x,y),yΪҳ��Χ****************************/
void OLED_P32x32Ch(unsigned char x,unsigned char y,unsigned char N);

/***********��������:��ʾBMPͼƬ128*64��ʼ����(x,y),x�ķ�ΧΪ0-127��yΪҳ��Χ*****************/
//void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,unsigned char const BMP[]);
void Draw_BMP(unsigned char const BMP[], uint8(* prdtrImg)[80]);
void OLED_Drawcamera(uint8 BMP[][80]);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
#endif
