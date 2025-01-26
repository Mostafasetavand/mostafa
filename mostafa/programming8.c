#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "user.h"

#define MAX_X 80
#define MAX_Y 40

// تعریف اجزای مختلف
#define WALL '|'
#define FLOOR '.'
#define DOOR '+'
#define CORRIDOR '#'
#define PILLAR 'O'
#define WINDOW '='
#define STAIRS '^'
#define TRAP '*'
#define SECRET_DOOR '@'
#define PASSWORD_DOOR '&'
#define KEY_ANCIENT '△'  // نماد کلید باستانی
#define PASSWORD_LENGTH 4
#define TREASURE_ROOM 'T'
#define ENCHANT_ROOM 'E'
#define NIGHTMARE_ROOM 'N'
#define REGULAR_ROOM 'R'  // اتاق معمولی

// تعداد اتاق‌ها و اندازه‌ها
#define MAX_ROOMS 10
#define MIN_ROOM_SIZE 4
#define MAX_ROOM_SIZE 10
#define MIN_DISTANCE_BETWEEN_ROOMS 3

// // ساختار اتاق
typedef struct RoomRoom {
    int x, y;  // موقعیت اتاق
    int width, height;  // اندازه اتاق
    bool visited;  // آیا بازیکن وارد اتاق شده است؟
    bool has_trap; // آیا اتاق تله دارد؟
    bool has_secret_door; // آیا اتاق در مخفی دارد؟
    bool has_password_door; // آیا اتاق در رمزدار دارد؟
    bool is_treasure_room; // آیا اتاق گنج است؟
    bool is_enchant_room; // آیا اتاق طلسم است؟
    bool is_nightmare_room; // آیا اتاق کابوس است؟
    bool is_regular_room; // آیا اتاق معمولی است؟
    int password[4];  // رمز ۴ رقمی برای در رمزدار
    int second_password[4];  // رمز دوم برای در دو رمزه
    bool door_locked;  // آیا در قفل است؟
    bool reverse_password;  // آیا رمز باید معکوس وارد شود؟
    int gold;  // طلا در اتاق
    int spell;  // طلسم در اتاق
    int ammo;  // مهمات در اتاق
    int enemies;  // تعداد دشمنان
    char theme;  // تم اتاق (مثلاً R برای اتاق معمولی)
    char color_pair;  // رنگ تم اتاق
} RoomRoom;

RoomRoom rooms[MAX_ROOMS];  
int num_rooms = 0;      
int player_health = 100; 
int player_key_ancient = 0;  // تعداد کلید باستانی که بازیکن دارد
int current_password[4]; 
int password_attempts = 0; 
int player_score = 0; // امتیاز بازیکن

 extern void generate_password() {
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        current_password[i] = rand() % 10;  // هر رقم به صورت تصادفی از 0 تا 9
    }
    mvprintw(MAX_Y - 2, 0, "New password generated: ");
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        printw("%d", current_password[i]);
    }
    refresh();
    sleep(30);  // نمایش رمز برای 30 ثانیه
    mvprintw(MAX_Y - 2, 0, "Password expired.                ");
    refresh();
}
#define COLOR_REGULAR 1
#define  COLOR_TREASURE 2
#define COLOR_ENCHANT 3
#define COLOR_NIGHTMARE 4

extern void init_colors(){
    if(has_colors()){
        start_color();
        init_pair(COLOR_REGULAR,COLOR_WHITE,COLOR_BLACK);
        init_pair(COLOR_TREASURE,COLOR_YELLOW,COLOR_BLACK);
        init_pair(COLOR_ENCHANT,COLOR_GREEN,COLOR_BLACK);
        init_pair(COLOR_NIGHTMARE,COLOR_RED,COLOR_BLACK);
    }
}

extern void generate_rooms() {
    srand(time(NULL)); 

    while (num_rooms < MAX_ROOMS) {
        int width = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);
        int height = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);
        int x = rand() % (MAX_X - width - 1);
        int y = rand() % (MAX_Y - height - 1);
        bool valid = true;

        for (int i = 0; i < num_rooms; i++) {
            RoomRoom r = rooms[i];
            if (x < r.x + r.width + MIN_DISTANCE_BETWEEN_ROOMS &&
                x + width > r.x - MIN_DISTANCE_BETWEEN_ROOMS &&
                y < r.y + r.height + MIN_DISTANCE_BETWEEN_ROOMS &&
                y + height > r.y - MIN_DISTANCE_BETWEEN_ROOMS) {
                valid = false;
                break;
            }
        }

        if (valid) {
            RoomRoom new_room = {x, y, width, height, false, false, false, false, false, false, false, false, false, false, false, false, false, 'R', 0};
            rooms[num_rooms++] = new_room;

            if (rand() % 2 == 0) {
                rooms[num_rooms - 1].is_regular_room = true;  // احتمال داشتن اتاق معمولی
            }
            if (num_rooms == MAX_ROOMS) {
                rooms[num_rooms - 1].is_treasure_room = true;  // اتاق آخر باید اتاق گنج باشد
            } else if (rand() % 3 == 0) {
                rooms[num_rooms - 1].is_enchant_room = true;  // احتمال داشتن اتاق طلسم
            } else if (rand() % 4 == 0) {
                rooms[num_rooms - 1].is_nightmare_room = true;  // احتمال داشتن اتاق کابوس
            }
            
            // اضافه کردن ویژگی‌های خاص اتاق معمولی
            if (rooms[num_rooms - 1].is_regular_room) {
                rooms[num_rooms - 1].gold = rand() % 50;  // مقداری طلا
                rooms[num_rooms - 1].spell = rand() % 5;  // مقداری طلسم
                rooms[num_rooms - 1].ammo = rand() % 10;  // مقداری مهمات
                rooms[num_rooms - 1].enemies = rand() % 3;  // تعداد دشمنان
                rooms[num_rooms - 1].theme = 'R';  // تم اتاق معمولی
                rooms[num_rooms - 1].color_pair = 1;  // رنگ پیش‌فرض برای تم معمولی
            }
        }
    }
        srand(time(NULL));  // استفاده از زمان برای تصادفی کردن
    for (int i = 0; i < 10; i++) {
        RoomRoom new_room = {rand() % (MAX_X - 10), rand() % (MAX_Y - 5), 5 + rand() % 10, 5 + rand() % 10, 'R', COLOR_REGULAR};
        rooms[num_rooms++] = new_room;
    }
}

 extern void change_room_theme(RoomRoom *room) {
    if (room->is_regular_room) {
        attron(COLOR_PAIR(room->color_pair));  // اعمال رنگ تم اتاق
        mvprintw(MAX_Y - 3, 0, "You have entered a Regular Room!");
        refresh();
        // در اینجا، باید موسیقی تغییر کند (امتیازی)
        // play_music_for_theme(room->theme);
        sleep(1);
    }
}

// بازی اصلی
