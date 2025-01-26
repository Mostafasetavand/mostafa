#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_X 80
#define MAX_Y 40

#define PLAYER '@'
#define WALL '|'
#define FLOOR '.'
#define DOOR '+'
#define STAIRS '>'
#define TREASURE '*'
#define FOOD 'G'
#define GOLD 'T'
#define BLACK_GOLD '$'

// استفاده از یونیکد به‌طور مستقیم
#define CLUB L"⚒"
#define DAGGER L"🗡"
#define WAND L"杖"
#define ARROW L"←"
#define SWORD L"⚔"
#define HEALTH_SPELL L"❤"
#define SPEED_SPELL L"⏳"
#define DAMAGE_SPELL L"☠"

wchar_t map[MAX_X][MAX_Y];
int player_x = MAX_X / 2, player_y = MAX_Y / 2;
int food_count = 0;
int health = 100;
int gold_count = 0;
int score = 0;

// تابع برای خوردن غذا و افزایش سلامتی
 extern void eat_food() {
    if (food_count > 0) {
        food_count--;
        health += 5;
        if (health > 100) {
            health = 100; // حداکثر سلامتی 100 است
        }
    }
}

// تابع برای حرکت بازیکن
 extern void move_player(int direction) {
    int new_x = player_x;
    int new_y = player_y;

    // به‌روزرسانی موقعیت بازیکن بر اساس جهت حرکت
    if (direction == 1) {
        new_x--; // حرکت به بالا
    } else if (direction == 2) {
        new_x++; // حرکت به پایین
    } else if (direction == 3) {
        new_y--; // حرکت به چپ
    } else if (direction == 4) {
        new_y++; // حرکت به راست
    }

    // اطمینان از اینکه بازیکن در محدوده نقشه باقی بماند
    if (new_x >= 0 && new_x < MAX_X && new_y >= 0 && new_y < MAX_Y) {
        player_x = new_x;
        player_y = new_y;

        // بررسی محتویات خانه جدید
        if (map[new_x][new_y] == CLUB[0]) {
            // انجام عملیاتی که مربوط به Club است
            printf("Picked up a club!\n");
        } else if (map[new_x][new_y] == DAGGER[0]) {
            // انجام عملیاتی که مربوط به Dagger است
            printf("Picked up a dagger!\n");
        } else if (map[new_x][new_y] == WAND[0]) {
            // انجام عملیاتی که مربوط به Wand است
            printf("Picked up a wand!\n");
        } else if (map[new_x][new_y] == ARROW[0]) {
            // انجام عملیاتی که مربوط به Arrow است
            printf("Picked up an arrow!\n");
        } else if (map[new_x][new_y] == SWORD[0]) {
            // انجام عملیاتی که مربوط به Sword است
            printf("Picked up a sword!\n");
        } else if (map[new_x][new_y] == HEALTH_SPELL[0]) {
            // درمان با طلسم سلامتی
            health += 10;
            if (health > 100) {
                health = 100;
            }
            printf("Health spell used. Health is now %d.\n", health);
        } else if (map[new_x][new_y] == SPEED_SPELL[0]) {
            // استفاده از طلسم سرعت
            printf("Speed spell used.\n");
        } else if (map[new_x][new_y] == DAMAGE_SPELL[0]) {
            // استفاده از طلسم آسیب
            printf("Damage spell used.\n");
        } else if (map[new_x][new_y] == FOOD) {
            food_count++;
            printf("Food found. Food count: %d\n", food_count);
        } else if (map[new_x][new_y] == GOLD) {
            gold_count++;
            printf("Gold found. Gold count: %d\n", gold_count);
        } else if (map[new_x][new_y] == BLACK_GOLD) {
            gold_count += 5;
            printf("Black gold found! Gold count: %d\n", gold_count);
        }
    }
}

// تابع برای چاپ نقشه
 extern void print_map() {
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            wprintf(L"%lc", map[i][j]);
        }
        wprintf(L"\n");
    }
}

// تابع اصلی برای بازی
// extern int main() {
//     setlocale(LC_ALL, "");  // فعال‌سازی پشتیبانی یونیکد

//     // مقداردهی اولیه به نقشه
//     for (int i = 0; i < MAX_X; i++) {
//         for (int j = 0; j < MAX_Y; j++) {
//             if ((i == 0) &&( i == MAX_X - 1) && (j == 0) || (j == MAX_Y - 1)) {
//                 map[i][j] = WALL; // دیوار در حاشیه
//             } else {
//                 map[i][j] = FLOOR; // کف نقشه
//             }
//         }
//     }
// // قرار دادن اشیاء مختلف بر روی نقشه
//     map[10][10] = CLUB[0];
//     map[5][5] = DAGGER[0];
//     map[20][20] = WAND[0];
//     map[15][15] = SWORD[0];
//     map[30][30] = FOOD;
//     map[25][25] = GOLD;
//     map[35][35] = BLACK_GOLD;

//     // حرکت دادن بازیکن
//     move_player(1); // حرکت به بالا
//     print_map(); // چاپ نقشه
//     move_player(4); // حرکت به راست
//     print_map(); // چاپ نقشه
//     eat_food(); // خوردن غذا
//     printf("Health after eating food: %d\n", health);

//     return 0;
// }