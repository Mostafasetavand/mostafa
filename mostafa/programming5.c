#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "user.h"


#define MAX_MSG_LENGTH 100
#define MESSAGE_AREA_HEIGHT 3 // تعداد خطوطی که برای نمایش پیام‌ها در نظر گرفته می‌شود

// پیام‌ها
char message[MAX_MSG_LENGTH];

// تابع برای نمایش پیام‌ها
extern void display_message(const char *msg) {
    strncpy(message, msg, MAX_MSG_LENGTH - 1);
    message[MAX_MSG_LENGTH - 1] = '\0'; // اطمینان از اینکه پیام به درستی تمام می‌شود
}

// تابع برای نمایش نوار پیام‌ها
extern void show_message_area() {
    // تنظیم رنگ
    attron(COLOR_PAIR(5));
    mvprintw(LINES - MESSAGE_AREA_HEIGHT, 0, "-------------------------------------------");
    mvprintw(LINES - MESSAGE_AREA_HEIGHT + 1, 0, " Message Area: ");
    mvprintw(LINES - MESSAGE_AREA_HEIGHT + 2, 0, "-------------------------------------------");
    
    // پیام را نمایش می‌دهیم
    mvprintw(LINES - MESSAGE_AREA_HEIGHT + 2, 14, "%s", message);
    
    // رنگ و حالت‌ها را از بین می‌بریم
    attroff(COLOR_PAIR(5));
    refresh();
}

// تابع برای شبیه‌سازی یک رویداد در بازی
extern void simulate_game_event() {
    // شبیه‌سازی رویدادهای مختلف در بازی
    display_message("You hit the enemy!");
    show_message_area();
    napms(2000);  // نمایش پیام به مدت 2 ثانیه
    
    display_message("You received damage from the enemy!");
    show_message_area();
    napms(2000);
    
    display_message("You picked up a weapon!");
    show_message_area();
    napms(2000);
    
    display_message("You shot your weapon!");
    show_message_area();
    napms(2000);
}

