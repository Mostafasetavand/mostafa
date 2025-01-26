#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "user.h"

// اندازه صفحه نمایش
#define MAX_X 80
#define MAX_Y 40

// تعریف نمادها
#define WALL '|'
#define FLOOR '.'
#define DOOR '+'
#define STAIRS '>'
#define TRAP '^'
#define SECRET_DOOR '?'
#define WALL_TOP_BOTTOM '_'

// تم‌ها
#define REGULAR_ROOM 'R'
#define TREASURE_ROOM 'T'
#define ENCHANT_ROOM 'E'
#define NIGHTMARE_ROOM 'N'

// رنگ‌ها
#define COLOR_REGULAR 1
#define COLOR_TREASURE 2
#define COLOR_ENCHANT 3
#define COLOR_NIGHTMARE 4
#define COLOR_TRAP 5
#define COLOR_SECRET_DOOR 6

// ساختار اتاق
typedef struct Room {
    int x, y;  // موقعیت اتاق
    int width, height;  // اندازه اتاق
    char theme;  // تم اتاق (مثلاً R برای اتاق معمولی)
    char color_pair;  // رنگ تم اتاق
} Room;

Room rooms[10];  // آرایه‌ای برای ذخیره اتاق‌ها
int num_rooms = 0;  // تعداد اتاق‌ها

// تابع برای ایجاد اتاق‌ها
 extern void generate_rooms() {
    srand(time(NULL));  // استفاده از زمان برای تصادفی کردن
    for (int i = 0; i < 10; i++) {
        Room new_room = {rand() % (MAX_X - 10), rand() % (MAX_Y - 5), 5 + rand() % 10, 5 + rand() % 10, 'R', COLOR_REGULAR};
        rooms[num_rooms++] = new_room;
    }
}

// تابع برای نمایش اتاق‌ها
 extern void display_room(Room room) {
    attron(COLOR_PAIR(room.color_pair));  // اعمال رنگ تم اتاق

    // نمایش دیوارها و کف اتاق
    for (int i = room.y; i < room.y + room.height; i++) {
        for (int j = room.x; j < room.x + room.width; j++) {
            if (i == room.y || i == room.y + room.height - 1) {
                mvaddch(i, j, WALL_TOP_BOTTOM);  // نمایش دیوار بالا و پایین
            } else if (j == room.x || j == room.x + room.width - 1) {
                mvaddch(i, j, WALL);  // نمایش دیوار چپ و راست
            } else {
                mvaddch(i, j, FLOOR);  // نمایش کف اتاق
            }
        }
    }
    
   // نمایش درها به صورت تصادفی
    int num_doors = 1 + rand() % 3;  // تعداد درها بین 1 تا 3
    for (int i = 0; i < num_doors; i++) {
         int door_x, door_y;
            // انتخاب موقعیت تصادفی برای در روی دیوارهای اتاق
        do {
           if (rand() % 2 == 0) { // در روی دیوارهای افقی
                door_y = room.y + (rand() % (room.height));
                if (rand() % 2 == 0) {
                    door_x = room.x;
                } else {
                   door_x = room.x + room.width -1;
                 }
                
           } else { //در روی دیوارهای عمودی
                door_x = room.x + (rand() % (room.width));
               if(rand() % 2 == 0){
                    door_y = room.y;
                } else {
                  door_y = room.y + room.height -1;
                }
               
            }
        } while (door_x < room.x + 1  && ( door_y < room.y+1 || door_y > room.y + room.height-2 ));
        mvaddch(door_y, door_x, DOOR); 
    }

    // نمایش پله‌ها (در اینجا به طور تصادفی)
    mvaddch(room.y + room.height - 2, room.x + room.width - 2, STAIRS);  // پله

    // نمایش تله (تصادفی)
    if (rand() % 2 == 0) {
        mvaddch(room.y + 2, room.x + 2, TRAP);  // تله در اتاق
    }

    // نمایش در مخفی (تصادفی)
    if (rand() % 2 == 0) {
        mvaddch(room.y + 3, room.x + 3, SECRET_DOOR);  // در مخفی
    }
}

 extern void setup_colors() {
    start_color();
    init_pair(COLOR_REGULAR, COLOR_WHITE, COLOR_BLACK);  // رنگ پیش‌فرض برای اتاق معمولی
    init_pair(COLOR_TREASURE, COLOR_YELLOW, COLOR_BLACK);  // رنگ اتاق گنج
    init_pair(COLOR_ENCHANT, COLOR_MAGENTA, COLOR_BLACK);  // رنگ اتاق طلسم
    init_pair(COLOR_NIGHTMARE, COLOR_RED, COLOR_BLACK);  // رنگ اتاق کابوس
    init_pair(COLOR_TRAP, COLOR_RED, COLOR_BLACK);  // رنگ تله
    init_pair(COLOR_SECRET_DOOR, COLOR_GREEN, COLOR_BLACK);  // رنگ در مخفی
}

