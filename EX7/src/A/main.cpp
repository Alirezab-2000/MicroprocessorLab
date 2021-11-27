#include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include "lcd.h"
#include "keypad.h"

// {
// state introduction
// state = 0 ---> login page
// state = 1 ---> menu page
// state = 11 ---> stepper motor page
// state = 12 ---> get information page
// state = 13 ---> Date setting page
// state = 14 ---> user setting page
// state = 15 ---> logOut
// }
char lcdInput[20] = "";

const int max_user = 2;
int users_count = 1;

struct User
{
    char id[20];
    char password[20];
};
User users[max_user] = {{"1", "1"}};

int state = 0;

int is_admin = -1; // -1 means no admin nor user logged in

void login_page();
void menu_page();
void get_info_page();
void date_setting_page();
void user_setting_page();

char *input(int line);

int main()
{
    char *key = "";

    DDRB = 0xFF;
    DDRD = 0xE0;
    DDRC = 0xF8;
    DDRA = 0xFF;

    keypad_init();
    init_LCD();

    _delay_ms(10);

    LCD_cmd(0x0E); // display on, cursor blinking

    _delay_ms(10);

    LCD_Write_String("Hi.use # to submit");
    LCD_Write_Line(line2 , "use * to back");
    _delay_ms(3000);

    while (1)
    {
        switch (state)
        {
        case 0:
            login_page();
            break;
        }
    }
    return 0;
}

char *input(int line)
{
    LCD_cmd(line);
    sprintf(lcdInput , "%s", "\0");
    
    while (true)
    {
        char* key = key_scan();

        if (strcmp(key, "#") == 0)
        {
            return lcdInput;
        }
        // if (strcmp(key, "*") == 0)
        // {
        //     state = state / 10;
        // }
        LCD_Write_String(key);
        sprintf(lcdInput , "%s%s" , lcdInput, key);
    }
}
void login_page()
{
    LCD_Clear();
    LCD_Write_String("Enter your id:");
    char* id = input(line2);
    LCD_Write_Line(line3, "Enter your password:");
    char* password = input(line4);

    for (int i = 0; i < users_count; i++)
    {
        if (strcmp(users[i].id, id) == 0 && strcmp(users[i].password, password) == 0)
        {
            PORTA = 1 << PA1;
            LCD_Timing_Write("entered successfully...");
            PORTA = 0x00;
            return;
        }
    }
    PORTA = 1 << PA0;
    LCD_Timing_Write("Access denied...");
    PORTA = 0x00;
}