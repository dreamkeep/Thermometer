#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "bmp.h"
#include "usart1.h"

uint16_t BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

uint8_t ref=0;//ˢ����ʾ
uint16_t vx=15542,vy=11165;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
uint16_t chx=140,chy=146;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ

void SSD1331_Send_Byte(uint8_t chData)
{
	(chData>>0 & 0x01) ?	SET_D0_H:SET_D0_L;
	(chData>>1 & 0x01) ?	SET_D1_H:SET_D1_L;
	(chData>>2 & 0x01) ?	SET_D2_H:SET_D2_L;
	(chData>>3 & 0x01) ?	SET_D3_H:SET_D3_L;
	(chData>>4 & 0x01) ?	SET_D4_H:SET_D4_L;
	(chData>>5 & 0x01) ?	SET_D5_H:SET_D5_L;
	(chData>>6 & 0x01) ?	SET_D6_H:SET_D6_L;
	(chData>>7 & 0x01) ?	SET_D7_H:SET_D7_L;
}

/*��������LCD_CmdWrite
 *��  �ܣ���LCD������д�����
 *����ֵ����
 *��  ����uint8_t cmd�����
 */
void LCD_CmdWrite(uint8_t cmd)			   
{
	uint16_t num=0;   
	LCD_RSL;
//	delay_us(4);
//	LCD_CSL;        
//	LCD_RDH;
//	SSD1331_Send_Byte(cmd);
	num=cmd;  
	num |=((GPIOB->ODR)&0xFF00);
	(GPIOB->ODR) =num;
//	delay_us(4);	
	LCD_WRL;
//	delay_us(4);	
	LCD_WRH;    
//	LCD_CSH;
	LCD_RSH;
}

/*��������LCD_DataWrite
 *��  �ܣ���LCD������д�������
 *����ֵ����
 *��  ����uint8_t data Ҫд�������
 */
 void LCD_DataWrite(uint8_t data)		
{
	uint16_t num=0;   
	LCD_RSH; 
//	delay_us(4);
//	LCD_CSL; 
//	LCD_RDH; 
//	SSD1331_Send_Byte(data);	
	num=data;  
	num |=((GPIOB->ODR)&0xFF00);
	(GPIOB->ODR) =num;
//	delay_us(4); 
	LCD_WRL;
//	delay_us(4);    
	LCD_WRH;    
//	LCD_CSH;
}

void LCD_Data16Write(uint16_t data)		
{
	uint16_t num=0;   
	LCD_RSH; 
//	LCD_CSL; 
//	LCD_RDH; 
//	SSD1331_Send_Byte(data);	
	num=(data&0x00ff);  
	num |=((GPIOB->ODR)&0xFF00);
	(GPIOB->ODR) =num;
	LCD_WRL;
	LCD_WRH;    
//	LCD_CSH;
	
	LCD_RSH; 
//	LCD_CSL; 
//	LCD_RDH; 
//	SSD1331_Send_Byte(data);	
	num=data>>8;  
	num |=((GPIOB->ODR)&0xFF00);
	(GPIOB->ODR) =num;
	LCD_WRL;
	LCD_WRH;    
//	LCD_CSH;
}
/*/////////////////////////////////////////////////////////
void LCD_Writ_Bus(char dat)   //��������д��
{	
	uint8_t i;			  
  

	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}			
}

void LCD_DataWrite8(char da) //��������-8λ����
{	//OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da);  
	//OLED_CS_Set();
}  
 void LCD_Data16Write(int da)
{//	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	//OLED_CS_Set();
}	  
void LCD_CmdWrite(char da)	 
{	//	OLED_CS_Clr();
    OLED_DC_Clr();
	LCD_Writ_Bus(da);
	//OLED_CS_Set();
}
 void LCD_CmdWrite_DATA(int reg,int da)
{	//OLED_CS_Clr();
    LCD_CmdWrite(reg);
	LCD_Data16Write(da);
	//OLED_CS_Set();
}

*//////////////////////////////////////////////////////////

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_CmdWrite(0x2a);
	LCD_DataWrite(x1>>8);
	LCD_DataWrite(x1);
	LCD_DataWrite(x2>>8);
	LCD_DataWrite(x2);

	LCD_CmdWrite(0x2b);
	LCD_DataWrite(y1>>8);
	LCD_DataWrite(y1);
	LCD_DataWrite(y2>>8);
	LCD_DataWrite(y2);

	LCD_CmdWrite(0x2C);					 						 
}

void Lcd_Init(void)
{
	ssd1331_IOInit();
	LCD_Reset();
	
	//************* Start Initial Sequence **********// 
	LCD_CmdWrite(0x36); 
	LCD_DataWrite(0x00);

	LCD_CmdWrite(0x3A); 
	LCD_DataWrite(0x05);

	LCD_CmdWrite(0xB2);
	LCD_DataWrite(0x0C);
	LCD_DataWrite(0x0C);
	LCD_DataWrite(0x00);
	LCD_DataWrite(0x33);
	LCD_DataWrite(0x33);

	LCD_CmdWrite(0xB7); 
	LCD_DataWrite(0x35);  

	LCD_CmdWrite(0xBB);
	LCD_DataWrite(0x19);

	LCD_CmdWrite(0xC0);
	LCD_DataWrite(0x2C);

	LCD_CmdWrite(0xC2);
	LCD_DataWrite(0x01);

	LCD_CmdWrite(0xC3);
	LCD_DataWrite(0x12);   

	LCD_CmdWrite(0xC4);
	LCD_DataWrite(0x20);  

	LCD_CmdWrite(0xC6); 
	LCD_DataWrite(0x0F);    

	LCD_CmdWrite(0xD0); 
	LCD_DataWrite(0xA4);
	LCD_DataWrite(0xA1);

	LCD_CmdWrite(0xE0);
	LCD_DataWrite(0xD0);
	LCD_DataWrite(0x04);
	LCD_DataWrite(0x0D);
	LCD_DataWrite(0x11);
	LCD_DataWrite(0x13);
	LCD_DataWrite(0x2B);
	LCD_DataWrite(0x3F);
	LCD_DataWrite(0x54);
	LCD_DataWrite(0x4C);
	LCD_DataWrite(0x18);
	LCD_DataWrite(0x0D);
	LCD_DataWrite(0x0B);
	LCD_DataWrite(0x1F);
	LCD_DataWrite(0x23);

	LCD_CmdWrite(0xE1);
	LCD_DataWrite(0xD0);
	LCD_DataWrite(0x04);
	LCD_DataWrite(0x0C);
	LCD_DataWrite(0x11);
	LCD_DataWrite(0x13);
	LCD_DataWrite(0x2C);
	LCD_DataWrite(0x3F);
	LCD_DataWrite(0x44);
	LCD_DataWrite(0x51);
	LCD_DataWrite(0x2F);
	LCD_DataWrite(0x1F);
	LCD_DataWrite(0x1F);
	LCD_DataWrite(0x20);
	LCD_DataWrite(0x23);

	LCD_CmdWrite(0x21); 

	LCD_CmdWrite(0x11); 
	//Delay (120); 

	LCD_CmdWrite(0x29);
	
} 

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;  	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_Data16Write(Color);	 			 
	    }

	  }
}

void LCD_Clearpart(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t Color)
{
	uint16_t i,j;  	
	Address_set(x1,y1,x2-1,y2-1);
    for(i=0;i<x2;i++)
	 {
	  for (j=0;j<y2;j++)
	   	{
        	LCD_Data16Write(Color);	 			 
	    }

	  }
}



//��ָ��λ����ʾһ������(32*33��С)
//dcolorΪ������ɫ��gbcolorΪ������ɫ
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
    Address_set(x,y,x+31,y+31); //��������      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_Data16Write(POINT_COLOR);
			} 
			else
			{
				LCD_Data16Write(BACK_COLOR);
			}   
		}
		temp++;
	 }
}
//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	LCD_Data16Write(POINT_COLOR); 	    
} 	 
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(uint16_t x,uint16_t y)
{
	LCD_Fill(x-5,y-5,x+5,y+5,POINT_COLOR);
} 
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
		LCD_Data16Write(color);//���ù��λ�� 	    
	} 					  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"

//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
    uint8_t temp;
    uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_Data16Write(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}  

void LCD_ShowChar22(uint16_t x,uint16_t y,uint8_t num,uint8_t Size,uint8_t mode)
{
    uint8_t temp;
    uint8_t t;
    uint16_t pos;
    uint16_t posnum=(Size*Size/2)/8;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;  
    
    if(x>LCD_W-(Size/2)||y>LCD_H-Size)return;	    
	//���ô���		   
//	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+Size/2-1,y+Size-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{ 
			temp=asc2_12864[(uint32_t)num*posnum+pos];		 //
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_Data16Write(POINT_COLOR);	
				temp>>=1; 
				x++;
			}
			x=x0;
			y++;
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{
		    temp=asc2_12864[(uint32_t)num*posnum+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
} 

void LCD_ShowChar33(uint16_t x,uint16_t y,uint8_t num,uint8_t Size,uint8_t mode)
{
    uint8_t temp;
    uint8_t t;
    uint16_t pos;
    uint16_t posnum=(Size*Size/2)/8;
	uint16_t x0=x;
	uint16_t x1=0;
	uint16_t y1=0;
	uint16_t colortemp=POINT_COLOR;  
    
    if(x>LCD_W-(Size/2)||y>LCD_H-Size)return;	    
	//���ô���		   
//	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+Size/2-1,y+Size-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{ 
			temp=char4824[(uint32_t)num*posnum+pos];		 //
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_Data16Write(POINT_COLOR);	
				temp>>=1; 
				x++;
			}
			x=x0;
			y++;
			
		}	
	}
	else//���ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{
		    temp=char4824[(uint32_t)num*posnum+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+x1,y+y1);//��һ���� 
				x1++;		
				if(x1>(Size/2-1))
				{
					x1=0;
					y1++;
				}
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
} 

void LCD_ShowChar44(uint16_t x,uint16_t y,uint8_t num,uint8_t Size,uint8_t mode)
{
    uint8_t temp;
    uint8_t t;
    uint16_t pos;
    uint16_t posnum=(Size*Size/2)/8;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;  
    
    if(x>LCD_W-(Size/2)||y>LCD_H-Size)return;	    
	//���ô���		   
//	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+Size/2-1,y+Size-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{ 
			temp=asc2_3216[(uint32_t)num*posnum+pos];		 //
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_Data16Write(POINT_COLOR);	
				temp>>=1; 
				x++;
			}
			x=x0;
			y++;
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<posnum;pos++)
		{
		    temp=asc2_3216[(uint32_t)num*posnum+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

//m^n����
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
}

void LCD_ShowNum22(uint16_t x,uint16_t y,uint32_t num,uint8_t Size,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar22(x+Size/2*t,y,' ',Size,0);
//				continue;
//			}else enshow=1; 
//		 	 
//		}
	 	LCD_ShowChar22(x+Size/2*t,y,temp,Size,0); 
	}
} 
void LCD_ShowNum33(uint16_t x,uint16_t y,uint32_t num,uint8_t Size,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar33(x+Size/2*t,y,' ',Size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar33(x+Size/2*t,y,temp,Size,0); 
	}
} 

//��ʾ2������
//x,y:�������
//num:��ֵ(0~99);	 
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len)
{         	
	uint8_t t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}

void ssd1331_IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOB, ENABLE );

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC|GPIO_Mode_AF,ENABLE );
    RCC_LSEConfig( RCC_LSE_OFF ); /* �ر��ⲿ����ʱ��,PC14+PC15����������ͨIO*/

    GPIO_InitStructure.GPIO_Pin = RES_Pin |WR_Pin ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, RES_Pin | WR_Pin);
	
	GPIO_InitStructure.GPIO_Pin = DC_Pin ;  				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,DC_Pin);
	
	GPIO_InitStructure.GPIO_Pin = LED_Pin ;  				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin ;  				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,D0_Pin|D1_Pin|D2_Pin|D3_Pin|D4_Pin|D5_Pin|D6_Pin|D7_Pin);
}

/*��������LCD_Reset
 *��  �ܣ���λTFT��
 *��  ������
 *����ֵ����
 */
void LCD_Reset(void)
{
  //************* Reset LCD Driver ****************// 
    LCD_RSTH;    
    delay_ms(1);  // delay_ms 1ms 
    LCD_RSTL;  
    delay_ms(10); // delay_ms 10ms               // This delay time is necessary 
    LCD_RSTH;    
    delay_ms(120); // delay_ms 120 ms   	  
}

void showimage(void) //��ʾ40*40ͼƬ
{
  	int i,j,k; 
	//k:��ʾ�У�j��ʾ��

	for(k=2;k<6;k++)
	{
	   	for(j=0;j<6;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//��������
			Address_set(40,40,40+39,40+39);		//��������
		    for(i=0;i<1600;i++)
			 { 	
				 		
			  	 LCD_DataWrite(image[i*2+1]);	 
				 LCD_DataWrite(image[i*2]);				
			 }	
		 }
	}
	ref=0;				
}

void showbattery(void) //��ʾ60*24ͼƬ
{
	LCD_Fill(180,10,220,13,WHITE);
	LCD_Fill(180,30,220,33,WHITE);
	LCD_Fill(180,10,183,30,WHITE);
	LCD_Fill(220,10,223,33,WHITE);
	LCD_Fill(223,18,226,24,WHITE);
	
	
}



void showbuzzer(void)
{
    uint8_t temp;
    uint8_t t;
    uint16_t pos;
	
    uint8_t x=10;
    uint16_t y=10;
	uint16_t x0=x;
	
	uint16_t colortemp=POINT_COLOR;
	
	uint8_t l=34;
	uint8_t w=31;
	uint16_t charnum=155;
    
	//���ô���		   
	Address_set(x,y,x+l-1,y+w-1);      //���ù��λ�� 
	for(pos=0;pos<charnum;pos++)
	{ 
		temp=buzzer[pos];		 //
		for(t=0;t<8;t++)
		{                 
			if(temp&0x01)POINT_COLOR=colortemp;
			else POINT_COLOR=BACK_COLOR;
			LCD_Data16Write(POINT_COLOR);	
			temp>>=1; 
			x++;
			if(x-x0==l)
			{
				x=x0;
				y++;
				break;
			}
		}
	}	
}


void batteryupdate(uint16_t ADvalue)
{
	uint8_t per;
//	��ص�ѹ2.4-3.3V,��ѹ��1/3.3->4000/3.3		
	if(ADvalue>1230)			
	{
		per=10;
	}
	else if(ADvalue>1208)			
	{
		per=9;
	}
	else if(ADvalue>1175)			
	{
		per=8;
	}
	else if(ADvalue>1142)			
	{
		per=7;
	}
	else if(ADvalue>1109)			
	{
		per=6;
	}
	else if(ADvalue>1076)			
	{
		per=5;
	}
	else if(ADvalue>1043)			
	{
		per=4;
	}
	else if(ADvalue>1010)			
	{
		per=3;
	}
	else if(ADvalue>977)			
	{
		per=2;
	}
	else if(ADvalue>944)			
	{
		per=1;
	}
	else
	{
		per=0;
	}
	LCD_Clearpart(183,13,187+33,30,BLACK);
	if(per<2)
	{
		LCD_Fill(185,15,185+3+(per*3),28,RED);
	}
	else
	{
		LCD_Fill(185,15,185+3+(per*3),28,GREEN);
	}
		

}

void tempupdate(uint16_t TobjADvalue)
{
	uint8_t i,j;
	
	POINT_COLOR=GREEN;
	LCD_Clearpart(0,56,240,184,BLACK);
	i=TobjADvalue/10;
	j=TobjADvalue%10;
	LCD_ShowNum22(0,56,i,128,2);	//88
	Draw_Circle(135,154,4);			//.
	Draw_Circle(135,154,3);			//.
	Draw_Circle(135,154,2);			//.
	Draw_Circle(135,154,1);			//.
	Draw_Circle(135,154,0);			//.
	LCD_ShowNum22(140,56,j,128,1);	//8
	Draw_Circle(208,88,4);			//.
	Draw_Circle(208,88,3);			//.
	LCD_ShowChar33(214,78,0,48,0);	//C
}






























