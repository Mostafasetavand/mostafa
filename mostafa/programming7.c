#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>
#include <math.h>
#include "user.h"

typedef struct {
bool visited;
bool has_trap;
bool has_secret_door;
bool door_locked;
int password[PASSWORD_LENGTH];
} Room;

#define PASSWORD_LENGTH 4
#define MAX_Yq 24
#define MAX_X 80
#define MAX_ROOMS 10
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define MIN_DISTANCE_BETWEEN_ROOMS 2

// تابع برای تولید رمز معکوس
 extern void generate_reverse_password(Room *room) {
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        room->password[i] = rand() % 10;  // تولید رمز اصلی
    }
}

// تابع برای تنظیم رمز برای اتاق
extern void setup_password_for_room(Room *room) {
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        room->password[i] = rand() % 10;  // تولید رمز اصلی
    }
    room->door_locked = true;  // در ابتدا در قفل است
}

// تابع برای اعتبارسنجی رمز
extern bool validate_password1(Room *room, int input[]) {
    bool match_normal = true;
    bool match_reverse = true;
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        if (room->password[i] != input[i]) {
            match_normal = false;
        }
        if (room->password[i] != input[PASSWORD_LENGTH - i - 1]) {
            match_reverse = false;
        }
    }
    return match_normal || match_reverse;
}

// تابع برای وارد کردن رمز
extern void enter_password(Room *room) {
    int input[PASSWORD_LENGTH];
    int password_attempts = 0;
    
    while (password_attempts < 3) {
        printf("Enter the 4-digit password: ");
        
        // دریافت ورودی از کاربر
        for (int i = 0; i < PASSWORD_LENGTH; i++) {
            scanf("%d", &input[i]);
        }
        
        if (validate_password1(room, input)) {
            room->door_locked = false;  // باز کردن در
            password_attempts = 0;      // ریست کردن تلاش‌ها
            printf("Password correct! Door is now open.\n");
            break;
        } else {
            password_attempts++;
            printf("Incorrect password. You have %d attempts left.\n", 3 - password_attempts);
        }
    }
}
