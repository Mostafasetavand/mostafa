#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "user.h" 


#define MAX_X 80
#define MAX_YC 40

#define WALL '|'
#define FLOOR '.'
#define DOOR '+'
#define CORRIDOR '#'
#define PILLAR 'O'
#define WINDOW '='
#define STAIRS '^'

#define MAX_ROOMS 10
#define MIN_ROOM_SIZE 4
#define MAX_ROOM_SIZE 10
#define MIN_DISTANCE_BETWEEN_ROOMS 3  // حداقل فاصله بین اتاق‌ها

// ساختار اتاق
typedef struct Room1 {
    int x, y;  // موقعیت اتاق
    int width, height;  // اندازه اتاق
    bool visited;  // آیا بازیکن وارد اتاق شده است؟
} Room1;

Room1 rooms[MAX_ROOMS];  // آرایه‌ای برای ذخیره اتاق‌ها
int number_rooms = 0;      // تعداد اتاق‌ها

extern void draw_wall(int x, int y) {
    mvaddch(y, x, WALL);
}

extern void draw_floor(int x, int y) {
    mvaddch(y, x, FLOOR);
}

extern void draw_door(int x, int y) {
    mvaddch(y, x, DOOR);
}

// تابع برای رسم راهرو
extern void draw_corridor(int x, int y) {
    mvaddch(y, x, CORRIDOR);
}

// تابع برای رسم ستون
extern void draw_pillar(int x, int y) {
    mvaddch(y, x, PILLAR);
}

extern void draw_window(int x, int y) {
    mvaddch(y, x, WINDOW);
}

extern void draw_stairs(int x, int y) {
    mvaddch(y, x, STAIRS);
}

 extern void draw_map() {
    for (int i = 0; i < number_rooms; i++) {
        Room1 r = rooms[i];
        for (int x = r.x; x < r.x + r.width; x++) {
            for (int y = r.y; y < r.y + r.height; y++) {
                if (x == r.x || x == r.x + r.width - 1) {
                    draw_wall(x, y); 
                } else if (y == r.y || y == r.y + r.height - 1) {
                    draw_wall(x, y); 
                } else {
                    draw_floor(x, y); 
                }
            }
        }
        draw_door(r.x + r.width / 2, r.y + r.height - 1);
    }

    draw_stairs(rooms[0].x + rooms[0].width / 2, rooms[0].y + rooms[0].height - 2);

    // اتصال اتاق‌ها به هم با راهرو
    for (int i = 0; i < number_rooms - 1; i++) {
        Room1 r1 = rooms[i];
        Room1 r2 = rooms[i + 1];

        // اتصال اتاق‌ها با راهرو پیچ در پیچ
        for (int x = r1.x + r1.width / 2; x <= r2.x + r2.width / 2; x++) {
            draw_corridor(x, r1.y + r1.height);
        }
    }
}

// تابع برای تولید اتاق‌ها با اندازه تصادفی
extern void Generate_rooms() {
    srand(time(NULL));  // استفاده از زمان برای تصادفی کردن)(بسیار مهم )

    while (number_rooms < MAX_ROOMS) {
        int width = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);
        int height = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);

        // تولید موقعیت تصادفی
        int x = rand() % (MAX_X - width - 1);
        int y = rand() % (MAX_YC - height - 1);

        bool valid = true;
        
        for (int i = 0; i < number_rooms; i++) {
            Room1 r = rooms[i];
            if (x < r.x + r.width + MIN_DISTANCE_BETWEEN_ROOMS &&
                x + width > r.x - MIN_DISTANCE_BETWEEN_ROOMS &&
                y < r.y + r.height + MIN_DISTANCE_BETWEEN_ROOMS &&
                y + height > r.y - MIN_DISTANCE_BETWEEN_ROOMS) {
                valid = false;  
                break;
            }
        }

        if (valid) {
            rooms[number_rooms].x = x;
            rooms[number_rooms].y = y;
            rooms[number_rooms].width = width;
            rooms[number_rooms].height = height;
            rooms[number_rooms].visited = false;  // اتاق تازه ساخته‌شده هنوز بازدید نشده است
            number_rooms++;
        }
    }
}

// تابع برای رسم اتاق‌های قابل مشاهده
extern void draw_visible_rooms() {
    for (int i = 0; i < number_rooms; i++) {
        if (rooms[i].visited) {
            // فقط اتاق‌هایی که بازدید شده‌اند نمایش داده می‌شوند
            for (int x = rooms[i].x; x < rooms[i].x + rooms[i].width; x++) {
                for (int y = rooms[i].y; y < rooms[i].y + rooms[i].height; y++) {
                    draw_floor(x, y);
                }
            }
        }
    }
}

// تابع برای بازدید اتاق‌ها
extern void visit_room(int x, int y) {
    for (int i = 0; i < number_rooms; i++) {
        if (x >= rooms[i].x && x < rooms[i].x + rooms[i].width &&
            y >= rooms[i].y && y < rooms[i].y + rooms[i].height) {
            rooms[i].visited = true;
            break;
        }
    }
}

