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
// state = 2 ---> stepper motor page
// state = 3 ---> get information page
// state = 4 ---> Date setting page
// state = 5 ---> Add or Edit user page
// state = 6 ---> remove user
// state = 7 ---> logOut
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
    LCD_Write_Line(line2, "use * to back");
    _delay_ms(3000);

    while (1)
    {
        switch (state)
        {
        case 0:
            login_page();
            break;

        case 1:
            menu_page();
            break;
        }
    }
    return 0;
}

char *input(int line)
{
    LCD_cmd(line);
    sprintf(lcdInput, "%s", "\0");

    while (true)
    {
        char *key = key_scan();

        if (strcmp(key, "#") == 0)
        {
            return lcdInput;
        }
        // if (strcmp(key, "*") == 0)
        // {
        //     state = state / 10;
        // }
        LCD_Write_String(key);
        sprintf(lcdInput, "%s%s", lcdInput, key);
    }
}
void login_page()
{
    LCD_Clear();
    LCD_Write_String("Enter your id:");
    char *id = input(line2);
    LCD_Write_Line(line3, "Enter your password:");
    char *password = input(line4);

    for (int i = 0; i < users_count; i++)
    {
        if (strcmp(users[i].id, id) == 0 && strcmp(users[i].password, password) == 0)
        {
            PORTA = 1 << PA1;
            if (i == 0)
            {
                is_admin = 1;
            }
            else
            {
                is_admin = 0;
            }
            state = 1;
            LCD_Timing_Write("successfully logined...");
            PORTA = 0x00;
            return;
        }
    }
    PORTA = 1 << PA0;
    LCD_Timing_Write("Access denied...");
    PORTA = 0x00;
}
void menu_page()
{
    int menu_page_num = 0;

    while (menu_page_num != -1)
    {
        LCD_Clear();
        if (menu_page_num == 0)
        {
            LCD_Write_String("1.see information");
            LCD_Write_Line(line2, "2.date&time setting");
            LCD_Write_Line(line3, "3.rotate motor");
            LCD_Write_Line(line4, "use # for next page");
        }
        else
        {
            LCD_Write_String("1.Add or Edit user");
            LCD_Write_Line(line2, "2.remove user");
            LCD_Write_Line(line3, "3.logout");
            LCD_Write_Line(line4, "use * for pre page");
        }

        while (true)
        {
            char *key = key_scan();
            if(strcmp(key, "#") == 0){
                menu_page_num = 1;
                break;
            }
            if(strcmp(key, "*") == 0){
                menu_page_num = 0;
                break;
            }

            if (menu_page_num == 0)
            {
                if (strcmp(key, "1") == 0)
                {
                    state = 3;
                    menu_page_num = -1;
                    break;
                }
                else if (strcmp(key, "2") == 0)
                {
                    state = 4;
                    menu_page_num = -1;
                    break;
                }
                else if (strcmp(key, "3") == 0)
                {
                    state = 2;
                    menu_page_num = -1;
                    break;
                }
            }
            else
            {
                if (strcmp(key, "1") == 0)
                {
                    state = 5;
                    menu_page_num = -1;
                    break;
                }
                else if (strcmp(key, "2") == 0)
                {
                    state = 6;
                    menu_page_num = -1;
                    break;
                }
                else if (strcmp(key, "3") == 0)
                {
                    state = 7;
                    menu_page_num = -1;
                    break;
                }
            }
        }
    }
}