/**************************************************************
**  Yeu cau: Chuong trinh do dien ap DC tu 0 - 50VDC         **
**  Thuc hien: Thi Minh Nhut - Email: thiminhnhut@gmail.com  ** 
**  Thoi gian: Ngay 22 thang 10 nam 2016                     **
**************************************************************/

#include<16f887.h>
#include<def_887.h>
#device *=16 ADC = 10                //Su dung bo ADC 10 bit
#fuses HS,NOWDT, NOPROTECT, NOLVP
#use delay(clock=20000000)          //Tan so thach anh 20MHz

#define votl_adc 0                  //Dien ap dua vao chan analog RA0

// Khai bao chan cua IC 74HC595
#define Clock PIN_E0 //Chan SH_CP
#define Data PIN_E1  //Chan SD
#define Latch PIN_E2 //Chan ST_CP

//Cac ham con su dung trong chuong trinh
void Display_LED7SEG(float volt);               // Hien thi gia tri dien ap len 4 LED 7 doan
void add_byte_74hc595(unsigned char data_byte);    //Ghi 1 byte vao IC
void output_byte_74hc595();                   //Cho phep xuat du lieu trong ra cac chan Q0 - Q7

//Khai bao ma code cho LED 7 doan co Anode chung
//Cac so tu 0 - 9
unsigned char code7seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

//Cac so tu 0. - 9.
unsigned char code7segdp[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

//Vi tri cac LED 1 2 3 4
unsigned char digits[] = {0x01, 0x02, 0x04, 0x08};

//Chuong trinh chinh
void main(){
    float voltage;
    int16 adc;

    TRISE = 0x00;                    //Tat ca cac chan o PORT E la chan xuat (output)
    
    Setup_ADC(ADC_Clock_Internal);
    Set_ADC_Channel(votl_adc);           //Cai dat chan doc gia tri ADC
    delay_us(10);
    
    while (True){
        adc = READ_ADC();             //Doc ADC tu chan A0
        voltage = ((float) adc*50/1023); //Chuyen gia tri ADC ve gia tri dien ap
        Display_LED7SEG(voltage);
    }
}


// Ham hien thi gia tri dien ap do duoc len 4 LED 7 doan
void Display_LED7SEG(float volt){
   int a,b,c,d;
   // Hien thi gia tri dien ap do duoc
   int32 voltint;
   
   voltint = volt*100;             //Chuyen so thuc ve so nguyen de tach so
   
   a = voltint / 1000;            // Lay so o vi tri thu nhat
   b = (voltint%1000)/100;         // Lay so o vi tri thu hai
   c = (voltint%100)/10;         // Lay so o vi tri thu ba
   d = (voltint%100)%10;         // Lay so o vi tri thu tu
   
   if (a != 0){
     add_byte_74hc595(digits[0]);
     add_byte_74hc595(Code7seg[a]);
     output_byte_74hc595();
   }
                
   add_byte_74hc595(digits[1]);
   add_byte_74hc595(Code7segdp[b]);
   output_byte_74hc595();
                    
   add_byte_74hc595(digits[2]);
   add_byte_74hc595(Code7seg[c]);
   output_byte_74hc595();
                
   add_byte_74hc595(digits[3]);
   add_byte_74hc595(Code7seg[d]);
   output_byte_74hc595(); 
}

void add_byte_74hc595(unsigned char data_byte){   //Ghi 1 byte vao IC
    int i;
    output_low(Clock);                //Muc thap
    for (i=0; i<8; i++){
        //Lay ra tung bit de ghi vao chip
        if ((data_byte & 0x80) == 0){    //So sanh bit
            output_low(Data);          //Ghi vao IC bit 0
        }
        else{
            output_high(Data);          //Ghi vao IC bit 1 
        }
        
        data_byte = data_byte << 1;    //Bo di bit da duoc o tren
        output_high(Clock);          //Muc cao
        //Ghi duoc 1 bit vao IC
        
        //Tiep tuc ghi cac bit tiep theo
        output_low(Clock); //Muc thap
    }
}

void output_byte_74hc595(){            //Cho phep xuat du lieu trong ra cac chan Q0 - Q7
    output_low(Latch);                //Muc thap
    delay_us(5);
    output_high(Latch);             //Muc cao
    //Cho phep xuat du lieu
}

