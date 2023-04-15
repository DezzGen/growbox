
//#include <LiquidCrystal.h>
#include <DS1307.h>
#include <LiquidCrystalRus.h>

// Init the DS1307
DS1307 rtc(SDA, SCL);
// Init the LCD
LiquidCrystalRus lcd(13, 12, 11, 10,  9,  8);

// ПЕРЕМЕННЫЕ ДЛЯ РАСПИСАНИЯ ЛАМПЫ
int r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21,r22,r23;

// ПЕРЕМЕННЫЕ ДЛЯ РАСПИСАНИЯ РАБОТЫ USB2
int usb0,usb1,usb2,usb3,usb4,usb5,usb6,usb7,usb8,usb9,usb10,usb11,usb12,usb13,usb14,usb15,usb16,usb17,usb18,usb19,usb20,usb21,usb22,usb23;

// ПЕРЕМЕННЫЕ ДЛЯ НАСТРОЙКИ ЛАМПЫ
int mode_light, power_light, light_setting_full;


const int buttonPin = A0;
int buttonState = 0;


// КНОПКИ
const int buttonUp = 7;
const int buttonDown = 5;
const int buttonCenter = 6;

// УПРАВЛЕНИЕ ПИТАНИЕМ LCD ЧЕРЕЗ ТРАНЗИСТОРНЫЙ КЛЮЧ
const int lcd_light = 4;

//ПЕРЕМЕННАЯ ТЕМПЕРАТУРЫ
int temp;

// ПЕРЕМЕННАЯ ВЛАЖНОСТИ
int humidity_val;

//ИНИЦИАЛИЗИРУЮ ФУНКЦИИ
int polling_buttons();
void empty_display();
void raspisaniye_lamp();

// ПЕРЕМЕННАЯ КУДА ФУНКЦИЯ ЗАПИСЫВАЕТ СОСТОЯНИЕ НАЖАТОЙ КНОПКИ
int button_press;

// ПЕРЕМЕННАЯ СЧЁТЧИКА ВРЕМЕНИ ДЛЯ ТОГО ЧТО БЫ ПОГАСИТЬ ЭКРАН LCD
unsigned long last_time;

// ПЕРЕМЕННАЯ ДЛЯ ОТСЛЕЖИВАНИЕЯ ЗАЛИПАНИЯ
bool sticking = false;

// ПЕРЕМЕННАЯ ДЛЯ ОТСЛЕЖИВАНИЕЯ ОДНОКРАТНОГО ОБНОВЛЕНИЯ LCD ЭКРАНА
bool display_full = true;

// ПЕРЕМЕННЫЕ МЕНЮ
int menu, level_one;
int light_timetable, light_setting, set_time, humidity, temperature;

// ПЕРЕМЕННЫЕ ДЛЯ НАСТРОЙКИ ВРЕМЕНИ
int hour_setting, minutes_setting, second_setting, year_setting, day_setting, month_setting;

// ПОДСЧЁТ НУЖНЫХ ДНЕЙ В МЕСЯЦЕ
int max_day = 28;
  
void setup()
{
  
//  Serial.begin(9600);
  
  // ИНИЦИАЛИЗАЦИЯ ОБЪЕКТА ЧАСОВ
  rtc.begin();
  // Set the clock to run-mode
  rtc.halt(false);
  // Setup LCD to 16x2 characters
  lcd.begin(16, 2);
  
// ЗАНОШУ В ПЕРЕМЕННЫЕ ДАННЫЕ ИЗ ПАМЯТИ DS1307
  r0=rtc.peek(0);
  r1=rtc.peek(1);
  r2=rtc.peek(2);
  r3=rtc.peek(3);
  r4=rtc.peek(4);
  r5=rtc.peek(5);
  r6=rtc.peek(6);
  r7=rtc.peek(7);
  r8=rtc.peek(8);
  r9=rtc.peek(9);
  r10=rtc.peek(10);
  r11=rtc.peek(11);
  r12=rtc.peek(12);
  r13=rtc.peek(13);
  r14=rtc.peek(14);
  r15=rtc.peek(15);
  r16=rtc.peek(16);
  r17=rtc.peek(17);
  r18=rtc.peek(18);
  r19=rtc.peek(19);
  r20=rtc.peek(20);
  r21=rtc.peek(21);
  r22=rtc.peek(22);
  r23=rtc.peek(23);


// ЗАНОШУ В ПЕРЕМЕННЫЕ ДАННЫЕ ИЗ ПАМЯТИ DS1307
  usb0=rtc.peek(27);
  usb1=rtc.peek(28);
  usb2=rtc.peek(29);
  usb3=rtc.peek(30);
  usb4=rtc.peek(31);
  usb5=rtc.peek(32);
  usb6=rtc.peek(33);
  usb7=rtc.peek(34);
  usb8=rtc.peek(35);
  usb9=rtc.peek(36);
  usb10=rtc.peek(37);
  usb11=rtc.peek(38);
  usb12=rtc.peek(39);
  usb13=rtc.peek(40);
  usb14=rtc.peek(41);
  usb15=rtc.peek(42);
  usb16=rtc.peek(43);
  usb17=rtc.peek(44);
  usb18=rtc.peek(45);
  usb19=rtc.peek(46);
  usb20=rtc.peek(47);
  usb21=rtc.peek(48);
  usb22=rtc.peek(49);
  usb23=rtc.peek(50);


// НАСТРОЙКИ ЛАМПЫ
  // ПЕРВАЯ ЦИФРА
  mode_light = rtc.peek(24);
  mode_light = (mode_light / 10) % 10;
  // ПОСЛЕДНЯЯ ЦИФРА
  power_light = rtc.peek(24);
  power_light = power_light % 10;

// ТЕМПЕРАТУРА
  temp = rtc.peek(25);
  
// ВЛАЖНОСТЬ
  humidity_val = rtc.peek(26);
  

// ОБНУЛЯЮ ПЕРЕМЕННЫЕ МЕНЮ
  menu=0;
  level_one=0;
  light_timetable=0;
  light_setting=0;
  set_time=0;
  humidity=0;
  temperature=0;

// ОБНУЛЯЮ ПЕРЕМЕННЫЕ ДЛЯ НАСТРОЙКИ ЧАСОВ
  hour_setting = 0;
  minutes_setting = 0;
  second_setting = 0;
  year_setting = 0;
  day_setting = 0;
  month_setting = 0;

// Set SQW/Out rate to 1Hz, and enable SQW
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);

// КНОПКИ   
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonCenter, INPUT);
// УПРАВЛЕНИЕ ПИТАНИЕМ LCD ЧЕРЕЗ ТРАНЗИСТОРНЫЙ КЛЮЧ
  pinMode(lcd_light, OUTPUT);

  pinMode(buttonPin, INPUT);
 
}


void loop(){
    button_press = polling_buttons();

// НАСТРОЙКА ВРЕМЕНИ
    if( set_time > 0 ){
        switch (set_time) {
// НАСТРОЙКА ЧАСОВ
          case 1:
            lcd.setCursor(0, 0);
            lcd.print("  Введите часы:  ");
    
            if( button_press == 11 ){ 
              if( hour_setting == 23 ){
                hour_setting = 0;
                lcd.setCursor(0, 1);
                lcd.print("                ");
              }else{
                hour_setting = hour_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( hour_setting == 0 ){
                hour_setting = 23;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                hour_setting = hour_setting - 1;
              }
            }
  
            if( button_press == 12 ){
              set_time = 2;
            }
  
            
            lcd.setCursor(0, 1);
            lcd.print("Ч""\x13");
            lcd.print(hour_setting);
            break;
// НАСТРОЙКА МИНУТ
          case 2:
           lcd.setCursor(0, 0);
            lcd.print("Введите минуты: ");
    
            if( button_press == 11 ){ 
              if( minutes_setting == 59 ){
                minutes_setting = 0;
                lcd.setCursor(0, 1);
                lcd.print("                ");
              }else{
                minutes_setting = minutes_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( minutes_setting == 0 ){
                minutes_setting = 59;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                minutes_setting = minutes_setting - 1;
              }
            }
  
            if( button_press == 12 ){
              set_time = 3;
            }
  
            
            lcd.setCursor(0, 1);
            lcd.print("Ч""\x13");
            lcd.print(hour_setting);
            lcd.print("  ");
            lcd.print("М""\x13");
            lcd.print(minutes_setting);
            break;
// НАСТРОЙКА СЕКУНД
          case 3:
            lcd.setCursor(0, 0);
            lcd.print("Введите секунды:");
    
            if( button_press == 11 ){ 
              if( second_setting == 59 ){
                second_setting = 0;
              }else{
                second_setting = second_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( second_setting == 0 ){
                second_setting = 59;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                second_setting = second_setting - 1;
              }
            }
            
            lcd.setCursor(0, 1);
            lcd.print("Ч""\x13");
            lcd.print(hour_setting);
            lcd.print("  ");
            lcd.print("М""\x13");
            lcd.print(minutes_setting);
            lcd.print("  ");
            lcd.print("C""\x13");
            lcd.print(second_setting);
            lcd.print(" ");

            if( button_press == 12 ){
              lcd.setCursor(0, 1);
              lcd.print("                ");
              set_time = 4;
            }
            break;
// НАСТРОЙКА МЕСЯЦА
          case 4:
            lcd.setCursor(0, 0);
            lcd.print(" Введите месяц: ");

            if( button_press == 11 ){ 
              if( month_setting == 12 ){
                lcd.setCursor(0, 1);
                lcd.print("                ");
                month_setting = 1;
              }else{
                month_setting = month_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( month_setting < 2 ){
                month_setting = 12;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                month_setting = month_setting - 1;
              }
            }

            if( button_press == 12 ){
              set_time = 5;
            }
            
            lcd.setCursor(0, 1);
            lcd.print("М""\x13");
            lcd.print(month_setting);
            break;
          case 5:

            lcd.setCursor(0, 0);
            lcd.print("  Введите день: ");

            if( month_setting == 2 ){
              max_day = 28;
            }else if( month_setting == 1 || month_setting == 3 || month_setting == 5 || month_setting == 7 || month_setting == 8 || month_setting == 10 || month_setting == 12){
              max_day = 31;
            }else if(  month_setting == 4 || month_setting == 6 || month_setting == 9 || month_setting == 11 ){
              max_day = 30;
            }


            if( button_press == 11 ){
              lcd.setCursor(0, 1);
              lcd.print("                ");
              if( day_setting == max_day ){
                day_setting = 1;
              }else{
                day_setting = day_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( day_setting < 2 ){
                day_setting = max_day;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                day_setting = day_setting - 1;
              }
            }

            if( button_press == 12 ){
              set_time = 6;
            }
            
            lcd.setCursor(0, 1);
            lcd.print("М""\x13");
            lcd.print(month_setting);
            lcd.print("  ");
            lcd.print("Д""\x13");
            lcd.print(day_setting);
            lcd.print("  ");
            break;
          case 6:

            lcd.setCursor(0, 0);
            lcd.print("  Введите год: ");


            if( button_press == 11 ){ 
              if( year_setting == 99 ){
                year_setting = 0;
              }else{
                year_setting = year_setting + 1;
              }
            }
    
            if( button_press == 13 ){
              if( year_setting < 2 ){
                year_setting = 99;
              }else{
                lcd.setCursor(0, 1);
                lcd.print("                ");
                year_setting = year_setting - 1;
              }
            }
            
            lcd.setCursor(0, 1);
            lcd.print("М""\x13");
            lcd.print(month_setting);
            lcd.print("  ");
            lcd.print("Д""\x13");
            lcd.print(day_setting);
            lcd.print("  ");
            lcd.print("Г""\x13");
            lcd.print(year_setting);
            lcd.print("  ");
            
// СОХРАНЯЮ ДАННЫЕ О ВРЕМЕНИ В DS1307
            if( button_press == 12 ){
              empty_display();
              set_time = 7;
            }

            break;
          case 7:
            lcd.setCursor(0, 0);
            lcd.print("\x13"" Не сохранять ");
            lcd.setCursor(0, 1);
            lcd.print("  Сохранить");

            if( button_press == 11 || button_press == 13 ){ 
              empty_display();
              set_time = 8;
            }

            if( button_press == 12 ){
              empty_display();
              set_time = 0;
            }
          break;
          case 8:
            lcd.setCursor(0, 0);
            lcd.print("  Не сохранять ");
            lcd.setCursor(0, 1);
            lcd.print("\x13"" Сохранить");

            if( button_press == 11 || button_press == 13 ){
              empty_display();
              set_time = 7;
            }

            if( button_press == 12 ){ 
              empty_display();
              rtc.setTime(hour_setting, minutes_setting, second_setting);
              rtc.setDate(day_setting, month_setting, year_setting);
              set_time = 0;
            }
          break;             
          
          default:
            set_time = 0;
        }
    }
    
/*************************************************************/
// НАСТРОЙКА РАСПИСАНИЯ ЛАМПЫ    
    else if( light_timetable > 0 ){
        switch (light_timetable) {
          case 1:
            lcd.setCursor(0, 0);
            lcd.print("С 00:00 ПО 00:59");
            raspisaniye_lamp(r0);
          break;
            case 2:
            lcd.setCursor(0, 0);
            lcd.print("С 01:00 ПО 01:59");
            raspisaniye_lamp(r1);
          break;
          case 3:
            lcd.setCursor(0, 0);
            lcd.print("С 02:00 ПО 02:59");
            raspisaniye_lamp(r2);
          break;
          case 4:
            lcd.setCursor(0, 0);
            lcd.print("С 03:00 ПО 03:59");
            raspisaniye_lamp(r3);
          break;
          case 5:
            lcd.setCursor(0, 0);
            lcd.print("С 04:00 ПО 04:59");
            raspisaniye_lamp(r4);
          break;
          case 6:
            lcd.setCursor(0, 0);
            lcd.print("С 05:00 ПО 05:59");
            raspisaniye_lamp(r5);
          break;
          case 7:
            lcd.setCursor(0, 0);
            lcd.print("С 06:00 ПО 06:59");
            raspisaniye_lamp(r6);
          break;
          case 8:
            lcd.setCursor(0, 0);
            lcd.print("С 07:00 ПО 07:59");
            raspisaniye_lamp(r7);
          break;
          case 9:
            lcd.setCursor(0, 0);
            lcd.print("С 08:00 ПО 08:59");
            raspisaniye_lamp(r8);
          break;
          case 10:
            lcd.setCursor(0, 0);
            lcd.print("С 09:00 ПО 09:59");
            raspisaniye_lamp(r9);
          break;
          case 11:
            lcd.setCursor(0, 0);
            lcd.print("С 10:00 ПО 10:59");
            raspisaniye_lamp(r10);
          break;
          case 12:
            lcd.setCursor(0, 0);
            lcd.print("С 11:00 ПО 11:59");
            raspisaniye_lamp(r11);
          break;
          case 13:
            lcd.setCursor(0, 0);
            lcd.print("С 12:00 ПО 12:59");
            raspisaniye_lamp(r12);
          break;
          case 14:
            lcd.setCursor(0, 0);
            lcd.print("С 13:00 ПО 13:59");
            raspisaniye_lamp(r13);
          break;
          case 15:
            lcd.setCursor(0, 0);
            lcd.print("С 14:00 ПО 14:59");
            raspisaniye_lamp(r14);
          break;
          case 16:
            lcd.setCursor(0, 0);
            lcd.print("С 15:00 ПО 15:59");
            raspisaniye_lamp(r15);
          break;
          case 17:
            lcd.setCursor(0, 0);
            lcd.print("С 16:00 ПО 16:59");
            raspisaniye_lamp(r16);
          break;
          case 18:
            lcd.setCursor(0, 0);
            lcd.print("С 17:00 ПО 17:59");
            raspisaniye_lamp(r17);
          break;
          case 19:
            lcd.setCursor(0, 0);
            lcd.print("С 18:00 ПО 18:59");
            raspisaniye_lamp(r18);
          break;
          case 20:
            lcd.setCursor(0, 0);
            lcd.print("С 19:00 ПО 19:59");          
            raspisaniye_lamp(r19);
          break;
          case 21:
            lcd.setCursor(0, 0);
            lcd.print("С 20:00 ПО 20:59");
            raspisaniye_lamp(r20);
          break;
          case 22:
            lcd.setCursor(0, 0);
            lcd.print("С 21:00 ПО 21:59");
            raspisaniye_lamp(r21);
          break;
          case 23:
            lcd.setCursor(0, 0);
            lcd.print("С 22:00 ПО 22:59");
            raspisaniye_lamp(r22);
          break;
          case 24:
            lcd.setCursor(0, 0);
            lcd.print("С 23:00 ПО 23:59");
            raspisaniye_lamp(r23);
          break;
          case 25:
            lcd.setCursor(0, 0);
            lcd.print("\x7e"" Выйти назад");
            lcd.setCursor(0, 1);
            lcd.print("                ");
          break;
          default:
            light_timetable = 0;
        }

        if( button_press == 11 ){
          empty_display();
          if( light_timetable == 1 ){
            light_timetable = 25;
          }else{
            light_timetable = light_timetable - 1;
          }
        }

        if( button_press == 13 ){
          empty_display();
          if( light_timetable == 25 ){
            light_timetable = 1;
          }else{
            light_timetable = light_timetable + 1;
          }
        }

        if( button_press == 12 ){ 
          if( light_timetable == 1 ){
            if( r0 == 0 ){
              rtc.poke(0, 1);
              r0 = 1;
            }else{
              rtc.poke(0, 0);
              r0 = 0;
            }
          }else if(  light_timetable == 2 ){
            if( r1 == 0 ){
              rtc.poke(1, 1);
              r1 = 1;
            }else{
              rtc.poke(1, 0);
              r1 = 0;
            }
          }else if(  light_timetable == 3 ){
            if( r2 == 0 ){
              rtc.poke(2, 1);
              r2 = 1;
            }else{
              rtc.poke(2, 0);
              r2 = 0;
            }
          }else if(  light_timetable == 4 ){
            if( r3 == 0 ){
              rtc.poke(3, 1);
              r3 = 1;
            }else{
              rtc.poke(3, 0);
              r3 = 0;
            }
          }else if(  light_timetable == 5 ){
            if( r4 == 0 ){
              rtc.poke(4, 1);
              r4 = 1;
            }else{
              rtc.poke(4, 0);
              r4 = 0;
            }
          }else if(  light_timetable == 6 ){
            if( r5 == 0 ){
              rtc.poke(5, 1);
              r5 = 1;
            }else{
              rtc.poke(5, 0);
              r5 = 0;
            }
          }else if(  light_timetable == 7 ){
            if( r6 == 0 ){
              rtc.poke(6, 1);
              r6 = 1;
            }else{
              rtc.poke(6, 0);
              r6 = 0;
            }
          }else if(  light_timetable == 8 ){
            if( r7 == 0 ){
              rtc.poke(7, 1);
              r7 = 1;
            }else{
              rtc.poke(7, 0);
              r7 = 0;
            }
          }else if(  light_timetable == 9 ){
            if( r8 == 0 ){
              rtc.poke(8, 1);
              r8 = 1;
            }else{
              rtc.poke(8, 0);
              r8 = 0;
            }
          }else if(  light_timetable == 10 ){
            if( r9 == 0 ){
              rtc.poke(9, 1);
              r9 = 1;
            }else{
              rtc.poke(9, 0);
              r9 = 0;
            }
          }else if(  light_timetable == 11 ){
            if( r10 == 0 ){
              r10 = 1;
              rtc.poke(10, 1);
            }else{
              rtc.poke(10, 0);
              r10 = 0;
            }
          }else if(  light_timetable == 12 ){
            if( r11 == 0 ){
              rtc.poke(11, 1);
              r11 = 1;
            }else{
              rtc.poke(11, 0);
              r11 = 0;
            }
          }else if(  light_timetable == 13 ){
            if( r12 == 0 ){
              rtc.poke(12, 1);
              r12 = 1;
            }else{
              rtc.poke(12, 0);
              r12 = 0;
            }
          }else if(  light_timetable == 14 ){
            if( r13 == 0 ){
              rtc.poke(13, 1);
              r13 = 1;
            }else{
              rtc.poke(13, 0);
              r13 = 0;
            }
          }else if(  light_timetable == 15 ){
            if( r14 == 0 ){
              rtc.poke(14, 1);
              r14 = 1;
            }else{
              rtc.poke(14, 0);
              r14 = 0;
            }
          }else if(  light_timetable == 16 ){
            if( r15 == 0 ){
              rtc.poke(15, 1);
              r15 = 1;
            }else{
              rtc.poke(15, 0);
              r15 = 0;
            }
          }else if(  light_timetable == 17 ){
            if( r16 == 0 ){
              rtc.poke(16, 1);
              r16 = 1;
            }else{
              rtc.poke(16, 0);
              r16 = 0;
            }
          }else if(  light_timetable == 18 ){
            if( r17 == 0 ){
              rtc.poke(17, 1);
              r17 = 1;
            }else{
              rtc.poke(17, 0);
              r17 = 0;
            }
          }else if(  light_timetable == 19 ){
            if( r18 == 0 ){
              rtc.poke(18, 1);
              r18 = 1;
            }else{
              rtc.poke(18, 0);
              r18 = 0;
            }
          }else if(  light_timetable == 20 ){
            if( r19 == 0 ){
              rtc.poke(19, 1);
              r19 = 1;
            }else{
              rtc.poke(19, 0);
              r19 = 0;
            }
          }else if(  light_timetable == 21 ){
            if( r20 == 0 ){
              rtc.poke(20, 1);
              r20 = 1;
            }else{
              rtc.poke(20, 0);
              r20 = 0;
            }
          }else if(  light_timetable == 22 ){
            if( r21 == 0 ){
              rtc.poke(21, 1);
              r21 = 1;
            }else{
              rtc.poke(21, 0);
              r21 = 0;
            }
          }else if(  light_timetable == 23 ){
            if( r22 == 0 ){
              rtc.poke(22, 1);
              r22 = 1;
            }else{
              rtc.poke(22, 0);
              r22 = 0;
            }
          }else if(  light_timetable == 24 ){
            if( r23 == 0 ){
              rtc.poke(23, 1);
              r23 = 1;
            }else{
              rtc.poke(23, 0);
              r23 = 0;
            }
          }else if(  light_timetable == 25 ){
            empty_display();
            light_timetable = 0;
            menu = 0;
          }
        }
        

/*************************************************************/
//НАСТРОЙКА РЕЖИМОВ РАБОТЫ ЛАМПЫ
      }else if( light_setting > 0 ){
          switch (light_setting) {
            case 1:
              lcd.setCursor(0, 0);
              lcd.print("  Введите режим:  ");
              lcd.setCursor(0, 1);
              lcd.print("MODE""\x13");
              lcd.print(mode_light);

              if( button_press == 11 ){
                if( mode_light == 3 ){
                  mode_light = 1;
                }else{
                  mode_light = mode_light + 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("MODE""\x13");
                lcd.print(mode_light);
              }
            
            if( button_press == 13 ){
                if( mode_light == 1 ){
                  mode_light = 3;
                }else{
                  mode_light = mode_light - 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("MODE""\x13");
                lcd.print(mode_light);
              }

  
            if( button_press == 12 ){
              empty_display();
              light_setting = 2;
            }
            break;
            case 2:
              lcd.setCursor(0, 0);
              lcd.print("Введите мощность:");
                lcd.setCursor(0, 1);
                lcd.print("MODE""\x13");
                lcd.print(mode_light);
                lcd.print("  ");
                lcd.print("POWER""\x13");
                lcd.print(power_light);
              if( button_press == 11 ){ 
                if( power_light == 8 ){
                  power_light = 1;
                }else{
                  power_light = power_light + 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("MODE""\x13");
                lcd.print(mode_light);
                lcd.print("  ");
                lcd.print("POWER""\x13");
                lcd.print(power_light);
              }
            
            if( button_press == 13 ){
              if( power_light == 1 ){
                  power_light = 8;
                }else{
                  power_light = power_light - 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("MODE""\x13");
                lcd.print(mode_light);
                lcd.print("  ");
                lcd.print("POWER""\x13");
                lcd.print(power_light);
            }
  
            if( button_press == 12 ){
              empty_display();
              light_setting_full = power_light  + mode_light* 10;
              rtc.poke(24, light_setting_full);
              light_setting = 0;
              menu = 0;
            }
            break;
          default:
            light_setting = 0;
          }

/*************************************************************/
      }else if( humidity > 0 ){
        switch (humidity) {
          case 1:
            if( button_press == 11 ){
                if( humidity_val > 95 ){
                  humidity_val = 0;
                }else{
                  humidity_val = humidity_val + 5;
                }
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( humidity_val );
              }
              
              if( button_press == 13 ){
                if( humidity_val < 2 ){
                  humidity_val = 95;
                }else{
                  humidity_val = humidity_val - 5;
                }
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( humidity_val );
              }
    
              if( button_press == 12 ){
                humidity = 2;
              }
          break;
          case 2:
            lcd.setCursor(0, 0);
            lcd.print("\x13"" Не сохранить  ");
            lcd.setCursor(0, 1);
            lcd.print("  Сохранить");
            
            if( button_press == 11 || button_press == 13 ){ 
              empty_display();
              humidity = 3;
            }
            
            if( button_press == 12 ){
              empty_display();
              humidity = 0;
            }
          break;
          case 3:
            lcd.setCursor(0, 0);
            lcd.print("  Не сохранить");
            lcd.setCursor(0, 1);
            lcd.print("\x13"" Сохранить ");
            
            if( button_press == 11 || button_press == 13 ){ 
              empty_display();
              humidity = 2;
            }
            
            if( button_press == 12 ){
              empty_display();
              rtc.poke(26, humidity_val);
              humidity = 0;
            }
           break;
           default:
            humidity = 0;
        }
/*************************************************************/
      }else if( temperature > 0 ){
          switch (temperature) {
            case 1:
              if( button_press == 11 ){
                if( temp > 44 ){
                  temp = 1;
                }else{
                  temp = temp + 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( temp );
              }
              
              if( button_press == 13 ){
                if( temp < 2 ){
                  temp = 45;
                }else{
                  temp = temp - 1;
                }
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( temp );
              }
    
              if( button_press == 12 ){
                temperature = 2;
              }
            break;
            case 2:
              lcd.setCursor(0, 0);
              lcd.print("\x13"" Не сохранить  ");
              lcd.setCursor(0, 1);
              lcd.print("  Сохранить");
              
              if( button_press == 11 || button_press == 13 ){ 
                empty_display();
                temperature = 3;
              }
              
              if( button_press == 12 ){
                empty_display();
                temperature = 0;
              }
            break;
            case 3:
              lcd.setCursor(0, 0);
              lcd.print("  Не сохранить");
              lcd.setCursor(0, 1);
              lcd.print("\x13"" Сохранить ");
              
              if( button_press == 11 || button_press == 13 ){ 
                empty_display();
                temperature = 2;
              }
              
              if( button_press == 12 ){
                empty_display();
                rtc.poke(25, temp);
                temperature = 0;
              }
              
            break;
          default:
            temperature = 0;
          
          }
          
/*************************************************************/
      }else{
          // НАЧАЛЬНЫЙ ЭКРАН
          if( menu == 0){
            lcd.setCursor(4, 0);
            lcd.print(rtc.getTimeStr());
        
            lcd.setCursor(0, 1);
            lcd.print("Тут куча текста!");
        
            if( button_press == 12 ){
              menu = 1;
            }
          // ПЕРВЫЙ УРОВЕНЬ МЕНЮ
          }else if(menu == 1){ 
        
            // BEGIN 1 => 0
            if(level_one == 0){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("\x7e""    НАЗАД      ");
              if( button_press == 12 ){
                menu = 0;
              }
            }
            // END 1 => 0
        
            // BEGIN 1 => 1
            if(level_one == 1){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("Расписание лампы");
              if( button_press == 12 ){
                light_timetable = 1;
              }
            }
            // END 1 => 1
            
            // BEGIN 1 => 2
            if(level_one == 2){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("Настройки лампы");
              if( button_press == 12 ){
                empty_display();
                light_setting = 1;
              }
            }
            // END 1 => 2
            
            // BEGIN 1 => 3
            if(level_one == 3){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("Часы/Время      ");
              if( button_press == 12 ){
                empty_display();
                set_time = 1;
              }
            }
            // END 1 => 3
        
            // BEGIN 1 => 4
            if(level_one == 4){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("\xee""Влажность       ");
              if( button_press == 12 ){
                humidity = 1;
                lcd.setCursor(0, 0);
                lcd.print( "On USB if vlaj >" );
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( humidity_val );
              }
            }
            // END 1 => 4
            
            // BEGIN 1 => 5
            if(level_one == 5){
              lcd.setCursor(4, 0);
              lcd.print(rtc.getTimeStr());
              lcd.setCursor(0, 1);
              lcd.print("\x7f""Температура     ");
              if( button_press == 12 ){
                temperature = 1;
                lcd.setCursor(0, 0);
                lcd.print( "On USB if temp >" );
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print( temp );
              }
            }
            // END 1 => 5
        
            if( button_press == 11 ){
              empty_display();
              if(level_one == 5){
                level_one = 0;
              }else{
                level_one = level_one + 1;
              }
            }
         
            if( button_press == 13 ){
              empty_display();
              if(level_one == 0){
                level_one = 5;
              }else{
                level_one = level_one - 1;
              }
            }    
         } 
      }

}


// ФУНКЦИЯ ПРОВЕРКИ РАСПИСАНИЯ
void raspisaniye_lamp( int time_work ){
  lcd.setCursor(0, 1);
  if( time_work == 0 ){
    lcd.print("   \x13""Выключена""\x14  ");   
  }else{
    lcd.print("   \x13""Включена""\x14   ");
  }
}

// ФУНКЦИЯ ДЛЯ ОБНУЛЕНИЯ ЭКРАНА
void empty_display(){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}



// up 210
//action 144
//down 382

// ФУНКЦИЯ ДЛЯ ОТЛОВА НАЖАТИЯ НА КНОПКИ
int polling_buttons(){
  
 buttonState = analogRead(buttonPin);

// Serial.println( buttonState );

 if ( 170 < buttonState && buttonState < 300 ) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 11;
      }else{
        return 0;
      }
    }else if ( 300 < buttonState && buttonState < 450 ) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 13;        
      }else{
        return 0;
      }
    }else if ( 100 < buttonState && buttonState < 170 ) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 12;
      }else{
        return 0;
      }
    }else{
      // ОБНУЛЯЮ ПЕРЕМЕННУЮ СЧЁТЧИКА ЗАЛИПАНИЙ
      sticking = false;
    }

    if( millis() - last_time > 15000 ){
      digitalWrite(lcd_light, LOW);
        menu=0;
        level_one=0;
        light_timetable=0;
        light_setting=0;
        set_time=0;
        humidity=0;
        temperature=0;
        if( display_full == true ){
            display_full = false;
            empty_display();
        }
    }else{
      digitalWrite(lcd_light, HIGH);
    }


    
   
    /*
    if (digitalRead(buttonUp)) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 11;
      }else{
        return 0;
      }
    }else if (digitalRead(buttonDown)) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 13;        
      }else{
        return 0;
      }
    }else if (digitalRead(buttonCenter)) {
      if( sticking == false ){
        sticking = true;
        last_time = millis();
        display_full = true;
        return 12;
      }else{
        return 0;
      }
    }else{
      // ОБНУЛЯЮ ПЕРЕМЕННУЮ СЧЁТЧИКА ЗАЛИПАНИЙ
      sticking = false;
    }

    if( millis() - last_time > 15000 ){
      digitalWrite(lcd_light, LOW);
        menu=0;
        level_one=0;
        light_timetable=0;
        light_setting=0;
        set_time=0;
        humidity=0;
        temperature=0;
        if( display_full == true ){
            display_full = false;
            empty_display();
        }
    }else{
      digitalWrite(lcd_light, HIGH);
    }
    */
}
