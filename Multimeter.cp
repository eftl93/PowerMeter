#line 1 "G:/SeniorDesign/Multimeter/Multimeter.c"
char duty_cycle;
sbit LCD_RS at RC1_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RD3_bit;
sbit LCD_D5 at RD2_bit;
sbit LCD_D6 at RD0_bit;
sbit LCD_D7 at RC3_bit;

sbit LCD_RS_Direction at TRISC1_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD3_bit;
sbit LCD_D5_Direction at TRISD2_bit;
sbit LCD_D6_Direction at TRISD0_bit;
sbit LCD_D7_Direction at TRISC3_bit;

unsigned int k;
unsigned int j;
char txt1[]= "Eder Towers";
char msg1[]= " VDC        ";
char msg2[]= " ADC        ";
char msg3[]= " Watts     ";
char msg4[]= " TI Innovation ";
char *volt= "00.00";
char *cur= "00.00";
char *powe= "00.00";
unsigned int volt_value;
unsigned int cur_value;
unsigned int voltage;
unsigned int current;
unsigned long power;
unsigned long corrected_current;
unsigned long corrected_voltage;
char display_status=0;

void IV_display();
void P_display();
void owner();
void brightness();


void main() {
OSCCON=0b01111001;
CM2CON0=0;
CM1CON0=0;
TRISA=0xFF;
TRISB=0xFF;
PWM1_Init(5000);
PWM1_Start();
CCP2CON=0;
duty_cycle=80;
PWM1_Set_Duty(duty_cycle);
k=0;
j=0;
 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,3,txt1);
 delay_ms(1000);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Cmd(_LCD_CLEAR);

while(1){
if(~RA7_bit)
{
delay_ms(300);
if(display_status<2)
{
display_status++;
}
else if(display_status==2)
{
display_status=0;
}
}

if(~RA6_bit)
{
brightness();
}

if(display_status==0)
{
k++;
while(k==0xFFFF)
{
IV_display();
k=0;
}
}

else if(display_status==1)
{
j++;
while(j==0xFFFF)
{
P_display();
j=0;
}
}

else if(display_status==2)
{
owner();
}

}
}



void IV_display()
{
volt_value = ADC_Read(0);
cur_value = ADC_Read(1);


voltage=volt_value * (3000/1023.00);
current=cur_value * (3000/1023.00);
if(voltage<12.11)
{
corrected_voltage=1.0235*(voltage);
}
else if(voltage>12.11)
{
corrected_voltage=1.0235*(voltage)+(12.11);
}

if(current<5.49)
{
corrected_current=.8408*(current);
}
else if(current>5.49)
{
corrected_current=.8408*(current)+(5.46);
}

volt[0] = corrected_voltage/1000 + 48;
volt[1] = (corrected_voltage/100)%10 + 48;
volt[3] = (corrected_voltage/10)%10 + 48;
volt[4] = corrected_voltage%10 + 48;

cur[0] = corrected_current/1000 + 48;
cur[1] = (corrected_current/100)%10 + 48;
cur[3] = (corrected_current/10)%10 + 48;
cur[4] = corrected_current%10 + 48;
Lcd_Out(1,6,msg1);
Lcd_Out(2,6,msg2);
Lcd_Out(1,1,volt);
Lcd_Out(2,1,cur);
Delay_ms(100);
}

void P_display()
{
volt_value = ADC_Read(0);
cur_value = ADC_Read(1);

voltage=volt_value * (3000/1023.00);
current=cur_value * (3000/1023.00);
if(voltage<12.11)
{
corrected_voltage=1.0235*(voltage);
}
else if(voltage>12.11)
{
corrected_voltage=1.0235*(voltage)+(12.11);
}

if(current<5.49)
{
corrected_current=.8408*(current);
}
else if(current>5.49)
{
corrected_current=.8408*(current)+(5.46);
}
power=(corrected_current)*(corrected_voltage);
powe[0] = power/100000 + 48;
powe[1] = (power/10000)%10 + 48;
powe[3] = (power/1000)%10 + 48;
powe[4] = (power/100)%10 + 48;

Lcd_Out(1,6,msg3);
Lcd_Out(1,1,powe);
Lcd_out(2,1,"            ");
delay_ms(100);
}

void owner()
{
Lcd_Out(1,1,txt1);
Lcd_Out(2,1,msg4);
}

void brightness()
{
delay_ms(300);
if(duty_cycle>230)
{
duty_cycle=0;
}
else{
duty_cycle=duty_cycle+25;
}
PWM1_Set_Duty(duty_cycle);
}
