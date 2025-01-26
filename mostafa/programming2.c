#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "user.h"


#define SAVES_FILE "game_saves.txt"
#define SETTINGS_FILE "settings.txt"


// توابع برای مدیریت منوی پیش از بازی




extern void pre_game_menu(const char *username) {
    int choice;
    while (1) {
        clear();
        mvprintw(1, 10, "Pre-Game Menu");
        mvprintw(3, 5, "1. New Game");
        mvprintw(4, 5, "2. Continue Previous Game");
        mvprintw(5, 5, "3. Scoreboard");
        mvprintw(6, 5, "4. Settings");
        mvprintw(7, 5, "5. Profile");
        mvprintw(8, 5, "6. Exit");
        mvprintw(10, 5, "Choose an option: ");
        refresh();

        choice = getch() - '0';  // دریافت ورودی از کاربر

        // نمایش مقدار ورودی برای بررسی
        mvprintw(12, 5, "You pressed: %d", choice);  
        refresh();  // به روزرسانی صفحه

        switch (choice) {
            case 1:
                new_game(username);
                break;
            case 2:
                continue_game(username);
                break;
            case 3:
                show_scoreboard(username);  // ارسال پارامتر username به show_scoreboard
                break;
            case 4:
                show_settings();
                break;
            case 5:
                show_profile(username);
                break;
            case 6:
                return;  // خروج از برنامه
        }
    }
}

extern void show_Scoreboard(const char *logged_in_user) {
    clear();
    mvprintw(1, 10, "Scoreboard for user: %s", logged_in_user);  // نمایش نام کاربری
    mvprintw(3, 5, "Feature not implemented yet for user: %s", logged_in_user); // متن نمایشی برای تست
    mvprintw(5, 5, "Press any key to return to the menu.");
    refresh();
    getch();  // منتظر ورودی از کاربر برای بازگشت به منو
}

extern void new_game(const char *username) {
    int difficulty = load_settings();  // بارگذاری سطح سختی از تنظیمات
    clear();
    mvprintw(1, 10, "New Game");
    mvprintw(3, 5, "Starting a new game for user: %s", username);
    mvprintw(4, 5, "Difficulty: %d", difficulty);
    mvprintw(6, 5, "Press any key to start...");
    refresh();
    getch();

    // محیط بازی (شبیه‌سازی)
    clear();
    mvprintw(1, 10, "Playing the Game...");
    mvprintw(3, 5, "Press 's' to save and exit.");
    refresh();

    char ch;
    while ((ch = getch()) != 's') {
        // بازی ادامه دارد
    }

    // ذخیره بازی
    clear();
    mvprintw(1, 10, "Save Game");
    mvprintw(3, 5, "Enter a name for this save: ");
    char save_name[50];
    echo();
    getstr(save_name);
    noecho();

    FILE *file = fopen(SAVES_FILE, "a");
    if (file) {
        GameSave save = {0};
        strcpy(save.save_name, save_name);
        strcpy(save.username, username);
        save.level = 1; // فرضی
        save.score = 0; // فرضی
        fwrite(&save, sizeof(GameSave), 1, file);
        fclose(file);
    }

    mvprintw(5, 5, "Game saved successfully!");
    refresh();
    getch();
}

extern void continue_game(const char *username) {
    FILE *file = fopen(SAVES_FILE, "r");
    if (!file) {
        mvprintw(3, 5, "No saved games found.");
        refresh();
        getch();
        return;
    }

    clear();
    mvprintw(1, 10, "Continue Game");
    mvprintw(3, 5, "Select a saved game:");

    GameSave save;
    int count = 0, choice;
    while (fread(&save, sizeof(GameSave), 1, file)) {
        if (strcmp(save.username, username) == 0) {
            mvprintw(5 + count, 5, "%d. %s (Level: %d, Score: %d)", count + 1, save.save_name, save.level, save.score);
            count++;
        }
    }

    if (count == 0) {
        mvprintw(5, 5, "No saved games found for this user.");
        fclose(file);
        refresh();
        getch();
        return;
    }
    mvprintw(7 + count, 5, "Choose a save (1-%d): ", count);
    refresh();
    echo();
    scanw("%d", &choice);
    noecho();
    fclose(file);

    if (choice < 1 || choice > count) {
        mvprintw(9 + count, 5, "Invalid choice.");
        refresh();
        getch();
        return;
    }

    // شبیه‌سازی ادامه بازی
    clear();
    mvprintw(1, 10, "Resuming Game...");
    mvprintw(3, 5, "Loading save: %s", save.save_name);
    refresh();
    getch();
}

extern void show_settings() {
    clear();
    mvprintw(1, 10, "Settings");
    mvprintw(3, 5, "Enter difficulty level (1-3): ");
    refresh();

    int difficulty;
    echo();
    scanw("%d", &difficulty);
    noecho();

    if (difficulty < 1 || difficulty > 3) {
        mvprintw(5, 5, "Invalid difficulty level.");
    } else {
        save_settings(difficulty);
        mvprintw(5, 5, "Settings saved successfully!");
    }

    refresh();
    getch();
}

 extern void show_profile(const char *username) {
    clear();
    mvprintw(1, 10, "User Profile");
    mvprintw(3, 5, "Username: %s", username);
    mvprintw(4, 5, "Feature not fully implemented yet.");
    refresh();
    getch();
}
extern void save_settings(int difficulty) {
    FILE *file = fopen(SETTINGS_FILE, "w");
    if (file) {
        fprintf(file, "%d\n", difficulty);
        fclose(file);
    }
}

int load_settings() {
    FILE *file = fopen(SETTINGS_FILE, "r");
    if (file) {
        int difficulty;
        fscanf(file, "%d", &difficulty);
        fclose(file);
        return difficulty;
    }

    return 1; // پیش‌فرض: سطح ۱
}