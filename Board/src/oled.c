/**
*   @file    oled.c
*   @brief   oled����
*   @author  ������
*   @version v1.0
*   @date    2017-3-2

�ӿڶ���
cs----GND
D0----PTA15
D1----PTA16
DC----PTB11
RES---PTB10
**/

#include "include.h"
#include "MK60_port.h"
#include "oled.h"
#include "common.h"
#include "oled_table.h"
#include "stdint.h"

#define OLED_RES(X)    X?gpio_set(PTA24,1):gpio_set(PTA24,0)   //XΪ1ʱ��Ӧ��GPIO�˿�����ߵ�ƽ��XΪ0ʱ��Ӧ��GPIO�˿�����͵�ƽ

#define OLED_DC(X)     X?gpio_set(PTA28,1):gpio_set(PTA28,0)

#define OLED_Order  0//����д����
#define OLED_Data   1  //����д����


void OLED_Refresh_Gram(void);

uint8_t OLED_GRAM[128][8];

uint8 OLED_camera[8][80];

void OLED_WB(uint8_t data)
{
    uint8 buff[1];
    buff[0] = data;
    spi_mosi(SPI0,SPI0_PCS0_PIN,buff,0,1);
    dwt_delay_us(2);
}

/**********һ���ֽ�����д��**************/
void OLED_WrDat(uint8_t data)
{
    OLED_DC(1);
    OLED_WB(data);
}
/*************һ��ָ��д��**************/
void OLED_WrCmd(uint8_t cmd)
{
    OLED_DC(0);
    OLED_WB(cmd);
}
/************������ʾλ��**************/
void OLED_Set_Pos(uint8_t x,uint8_t y)
{
    /*page addressing mode*/
    OLED_WrCmd(0xb0+y); /*set page start address*/
    
    OLED_WrCmd(((x&0xf0)>>4)|0x10);  /*set higher nibble of the column address*/
    OLED_WrCmd((x&0x0f)|0x01);  /*set lower nibble of the column addresss*/
}

/*************д��������**************/
void OLED_Fill(unsigned char bmp_dat)
{
    unsigned char y,x;
    OLED_WrCmd(0x20); //-set page addressing mode (0x00/0x01/0x02)
    OLED_WrCmd(0x00); 
    OLED_WrCmd(0x21); //-set Column address
    OLED_WrCmd(0x00); 
    OLED_WrCmd(0x7f); 
    OLED_WrCmd(0x22); //-set Page Address
    OLED_WrCmd(0x00);
    OLED_WrCmd(0x07);
    dwt_delay_ms(1);
    for(y = 0; y < Page; y++)
    {
        for(x = 0;x < X_WIDTH; x++)
        {
            OLED_WrDat(bmp_dat);
        }
    }

}
/*************��������************/
void OLED_CLS(void)
{
    unsigned char y,x;
    for(y = 0;y < 8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x = 0;x < X_WIDTH;x++)
        {
            OLED_WrDat(0);
        }
        dwt_delay_ms(200);
    }
}
/************OLED��ʼ��**********/
void OLED_Init(void)
{
    gpio_init(PTA24,GPO,0);
    gpio_init(PTA28,GPO,0);
//    gpio_init(PTA14,GPO,0);
    spi_init(SPI0,SPI0_PCS0_PIN,MASTER,500*1000*1000);
    OLED_RES(0);
    dwt_delay_ms(50);
    OLED_RES(1);
    dwt_delay_ms(50);
    OLED_WrCmd(0XAE);//--turn off oled panel
    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3f);//--1/64 duty
    OLED_WrCmd(0xd3);//--set display offset Shift Mapping RAM counter(0x00-0x3f)
    OLED_WrCmd(0x00);//-not offset
    OLED_WrCmd(0x40);//--set start line address set mapping ram display line (0x00-0x3f)
    OLED_WrCmd(0xa0);//--set seg/column mapping  0xa0���ҷ���0xa1����
    OLED_WrCmd(0xc0);//--set com/row scan direction
    OLED_WrCmd(0xda);//--set com pins hardware configuration
    OLED_WrCmd(0x12);//
    OLED_WrCmd(0x81);//--set contrast control register
    OLED_WrCmd(0xcf);// set seg output current brightness
    OLED_WrCmd(0xa4);// disable entire display on (0xa4/0xa5)
    OLED_WrCmd(0xa6);// disable inverse display on
    OLED_WrCmd(0xd5);//--set dispaly clock divide ratio/oscillator frequecy
    OLED_WrCmd(0x80);//----set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    OLED_WrCmd(0x14);//--set(0x10) disable
    OLED_WrCmd(0xaf);//--turn on oled panel

    OLED_WrCmd(0xd9);//--set pre-charge period
    OLED_WrCmd(0xf8);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

    OLED_WrCmd(0xdb);//--set vcomh
    OLED_WrCmd(0x40);//Set VCOM Deselect Level

    OLED_Fill(0x00);  //��ʼ����
}
/*
* function ��ʾ6*8һ���׼ASCII�ַ��� ��ʾ������(x,y),yΪҳ��Χ0~7
*/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
    unsigned char c = 0,i = 0,j = 0;
    while(ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>126){x = 0;y++;}
        OLED_Set_Pos(x,y);
        for(i = 0;i < 6;i++)
        OLED_WrDat(F6x8[c][i]);
        x+=6;
        j++;
    }
}
/*
* function ��ʾ8*16һ���׼ASCII�ַ��� ��ʾ����(x,y),yΪҳ��Χ
*/
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
    unsigned char c = 0,i = 0,j = 0;
    while(ch[j]!='\0')
    {
        c = ch[j]-32;
        if(x>120){x=0;y++;}
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
        OLED_WrDat(F8X16[c*16+i]);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
        OLED_WrDat(F8X16[c*16+i+8]);
        x+=8;
        j++;
     }
}
/*****************��������:��ʾ16*16���� ��ʾ����(x,y),yΪҳ��Χ****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;  //ÿ����ģ����ʼ��ַ  	
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)  //             
	{
		OLED_WrDat(F16x16[adder]);	
		adder += 1;
	}      	 //�ϰ���
	OLED_Set_Pos(x,y + 1); 
	for(wm = 0;wm < 16;wm++) //         
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  //�°���
}
/*****************��������:��ʾ32*32���� ��ʾ������(x,y),yΪҳ��Χ****************************/
void OLED_P32x32Ch(unsigned char x,unsigned char y,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=128*N;  //  	
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 32;wm++)  //             
	{
		OLED_WrDat(F32x32[adder]);	
		adder += 1;
	}      
	OLED_Set_Pos(x,y + 1); 
	for(wm = 0;wm < 32;wm++) //         
	{
		OLED_WrDat(F32x32[adder]);
		adder += 1;
	} 
		OLED_Set_Pos(x,y + 2); 
	for(wm = 0;wm < 32;wm++) //         
	{
		OLED_WrDat(F32x32[adder]);
		adder += 1;
	} 
		OLED_Set_Pos(x,y + 3); 
	for(wm = 0;wm < 32;wm++) //         
	{
		OLED_WrDat(F32x32[adder]);
		adder += 1;
	} 			  	
}
/***********��������:��ʾBMPͼƬ128*64��ʼ����(x,y),x�ķ�ΧΪ0-127��yΪҳ��Χ*****************/
//void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1,unsigned char y1,unsigned char const BMP[])
void Draw_BMP(unsigned char const BMP[], uint8(* prdtrImg)[80])
{ 	
 unsigned int i,j;//l=0;
 int k = -1;
 unsigned char x,y;
  for(i=63;i>3;i--)
    for(j=0;j<80;j++)
    {
      if(j%8 == 0) 
      {
        k++;
      }
      if((BMP[k]&(0x80>>(j%8))) == (0x80>>(j%8)))
      {
        OLED_DrawPoint(j, i, 0);
      //
       prdtrImg[63-i][j] = 1;
      }
      else
      {
        OLED_DrawPoint(j, i, 1);
        prdtrImg[63-i][j] = 0;
      //OLED_Refresh_Gram();
      }
    }
  OLED_Refresh_Gram();       
} 
/////////
void OLED_Drawcamera(uint8 BMP[][80])
{
    uint8_t i,j,h;
    
    for(j = 0; j < 80; j++)
    {
        int k = -1;
        for(i = 0;i < 60; i++)
        {
            if(i%8 == 0)
            {
                k++;
            }
            //h=(i^2)/(-20)+i*2+45;
           // if(OLED_camera[k][j] == 0)
            OLED_camera[k][j] |= ((BMP[i][j]&0x01)<<(i%8));
           // if(OLED_camera[k][j])
          //  OLED_camera[k][j] &= ((~BMP[i][j]&0x01)<<(7-i%8));
        }
    }
    OLED_Refresh_Gram();
}
////////////////

void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WrCmd (0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WrCmd (0x00);      //������ʾλ�á��е͵�ַ
		OLED_WrCmd (0x10);      //������ʾλ�á��иߵ�ַ   
//		for(n=0;n<128;n++)OLED_WrDat(OLED_GRAM[n][i]); 
//////////////////////////////////////////////
               for(n=0;n<80;n++)
                {
                    OLED_WrDat(OLED_camera[i][n]); 
                    OLED_camera[i][n]=0;
                }
///////////////////////////////////////////////                
	}   
}    