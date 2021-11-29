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
User users[max_user] = {{"1", "2"}};

int state = 0;

int is_admin = -1; // -1 means no admin nor user logged in

void login_page();
void menu_page();
void get_info_page();
void date_setting_page();
void add_user_page();
void logout();

void add_user(char *id, char *pass);
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
        LCD_Write_String(key);
        sprintf(lcdInput, "%s%s", lcdInput, key);
    }
}
void login_page()
{
    char id[20] = "";
    char password[20] = "";

    LCD_Clear();
    LCD_Write_String("Enter your id:");
    char *id_scaned = input(line2);
    sprintf(id, "%s", id_scaned);
    LCD_Write_Line(line3, "Enter your password:");
    char *password_scaned = input(line4);
    sprintf(password, "%s", password_scaned);

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

        if (is_admin == 1)
        {
            if (menu_page_num == 0)
            {
                LCD_Write_String("1.see information");
                LCD_Write_Line(line2, "2.rotate motor");
                LCD_Write_Line(line3, "3.date&time setting");
                LCD_Write_Line(line4, "use # for next page");
            }
            else
            {
                LCD_Write_String("1.Add or Edit user");
                LCD_Write_Line(line2, "2.remove user");
                LCD_Write_Line(line3, "3.logout");
                LCD_Write_Line(line4, "use * for pre page");
            }
        }
        else if (is_admin == 0)
        {
            LCD_Write_String("1.see information");
            LCD_Write_Line(line2, "2.rotate motor");
            LCD_Write_Line(line3, "3.logout");
        }

        while (true)
        {
            char *key = key_scan();

            if (is_admin == 0)
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
                    logout();
                    menu_page_num = -1;
                    break;
                }
            }
            else if (is_admin == 1)
            {
                if (strcmp(key, "#") == 0)
                {
                    menu_page_num = 1;
                    break;
                }
                if (strcmp(key, "*") == 0)
                {
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
                        add_user_page();
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
                        logout();
                        menu_page_num = -1;
                        break;
                    }
                }
            }
        }
    }
}
void logout()
{
    LCD_Timing_Write("Logged out ...");
    is_admin = -1;
    state = 0;
}
void add_user_page()
{
    char id[20] = "";
    char password[20] = "";

    LCD_Clear();
    LCD_Write_String("Enter user id:");
    char *id_scaned = input(line2);
    sprintf(id , "%s",id_scaned);
    LCD_Write_Line(line3, "Enter user password:");
    char *password_scaned = input(line4);
    sprintf(password, "%s" , password_scaned);
    bool isEdited = false;

    LCD_Clear();
    for (int i = 0; i < users_count; i++)
    {
        if (strcmp(users[i].id, id) == 0)
        {
            strcpy(users[i].password, password);
            isEdited = true;
            LCD_Timing_Write("user updated ...");
            break;
        }
    }
    if (!isEdited)
    {
        if (users_count == max_user)
        {
            LCD_Clear();
            LCD_Write_String("The ram is Full");
            LCD_Write_Line(line2, "overwrite it?");
            LCD_Write_Line(line3, "(1=yes/0=no)");
            while (true)
            {
                char *key = key_scan();

                if (strcmp(key, "1") == 0)
                {
                    add_user(id, password);
                    LCD_Timing_Write("user added ...");
                     break;
                }
                if (strcmp(key, "0") == 0)
                {
                    LCD_Timing_Write("user dont add ...");
                     break;
                }
            }
        }
        else
        {
            add_user(id, password);
            LCD_Timing_Write("user added ...");
        }
    }
}
void add_user(char *id, char *password)
{
    if (users_count == max_user)
    {
        strcpy(users[users_count - 1].id, id);
        strcpy(users[users_count - 1].password, password);
    }
    else
    {
        strcpy(users[users_count].id, id);
        strcpy(users[users_count].password, password);
        users_count++;
    }
}