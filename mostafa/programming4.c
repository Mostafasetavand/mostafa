#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h> // برای تابع sleep (برای تأخیر)
#include "user.h" // اطمینان حاصل کنید که این فایل وجود دارد
#define MAX_SONGS 5 // تعداد آهنگ‌های موجود

void play_music(char *song_path); // اعلام تابع پخش موسیقی

void show_settings_menu() {
    int difficulty = 1; // پیش‌فرض: متوسط (1: آسان، 2: متوسط، 3: سخت)
    int hero_color = 2; // پیش‌فرض: سبز (1: قرمز، 2: سبز، 3: آبی، 4: زرد)
    int selected_song_index = 0; // پیش‌فرض: اولین آهنگ

    // لیست آهنگ‌ها و مسیرهای آنها
    char songs[MAX_SONGS][50] = {
        "Song 1 - Ayjan",
        "Song 2 - Cat",
        "Song 3 - Hossen",
        "Song 4 - Re",
        "Song 5 - Ali"
    };
    char song_paths[MAX_SONGS][100] = {
        "/home/mostafa/Music/ayjan.mp3",
        "/home/mostafa/Music/cat.mp3",
        "/home/mostafa/Music/hossen.mp3",
        "/home/mostafa/Music/re.mp3",
        "/home/mostafa/Music/ali.mp3"
    };

    while (1) {
        clear();
        mvprintw(2, 10, "Settings Menu");
        mvprintw(4, 5, "1. Change Game Difficulty (Current: %s)", 
                 difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");
        mvprintw(5, 5, "2. Change Hero Color (Current: %s)", 
                 hero_color == 1 ? "Red" : hero_color == 2 ? "Green" : hero_color == 3 ? "Blue" : "Yellow");
        mvprintw(6, 5, "3. Select Background Music (Current: %s)", songs[selected_song_index]);
        mvprintw(8, 5, "4. Back to Main Menu");

        refresh();

        int ch = getch();
        if (ch == '1') {
            change_difficulty(&difficulty);
        } else if (ch == '2') {
            change_hero_color(&hero_color);
        } else if (ch == '3') {
            change_game_music(songs, &selected_song_index);
            play_music(song_paths[selected_song_index]); // پخش آهنگ انتخاب شده
        } else if (ch == '4') {
            break; // بازگشت به منوی اصلی
        }
    }
}

extern void change_difficulty(int *difficulty) {
    clear();
    mvprintw(2, 10, "Select Game Difficulty:");
    mvprintw(4, 5, "1. Easy");
    mvprintw(5, 5, "2. Medium");
    mvprintw(6, 5, "3. Hard");
    refresh();

    int ch = getch();
    if (ch == '1') *difficulty = 1;
    else if (ch == '2') *difficulty = 2;
    else if (ch == '3') *difficulty = 3;
}

extern void change_hero_color(int *hero_color) {
    clear();
    mvprintw(2, 10, "Select Hero Color:");
    mvprintw(4, 5, "1. Red");
    mvprintw(5, 5, "2. Green");
    mvprintw(6, 5, "3. Blue");
    mvprintw(7, 5, "4. Yellow");
    refresh();

    int ch = getch();
    if (ch == '1') *hero_color = 1;
    else if (ch == '2') *hero_color = 2;
    else if (ch == '3') *hero_color = 3;
    else if (ch == '4') *hero_color = 4;
}

extern void change_game_music(char selected_song[MAX_SONGS][50], int *selected_index) {
    int song_index = 0;

    while (1) {
        clear();
        mvprintw(2, 10, "Select Background Music:");
        for (int i = 0; i < MAX_SONGS; i++) {
            if (i == song_index) {
                attron(A_REVERSE); // انتخاب گزینه برجسته
            }
            mvprintw(4 + i, 5, "%d. %s", i + 1, selected_song[i]);
            if (i == song_index) {
                attroff(A_REVERSE);
            }
        }
        mvprintw(10, 5, "Use UP/DOWN to navigate, ENTER to select, 'q' to cancel.");
        refresh();
        int ch = getch();
        if (ch == KEY_UP && song_index > 0) {
            song_index--;
        } else if (ch == KEY_DOWN && song_index < MAX_SONGS - 1) {
            song_index++;
        } else if (ch == '\n') { // انتخاب آهنگ
            *selected_index = song_index;
            break;
        } else if (ch == 'q') {
            break; // لغو انتخاب
        }
    }
}

extern void play_music(char *song_path) {
    char command[200];
    snprintf(command, sizeof(command), "mpg123 -q %s &", song_path); // استفاده از mpg123 برای پخش موسیقی
    system(command);
}