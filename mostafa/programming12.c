#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <math.h>
#include <ctype.h>
#include "user.h"

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

// نمادهای یونیکد
#define CLUB L"⚒"
#define DAGGER L"🗡"
#define WAND L"杖"
#define ARROW L"←"
#define SWORD L"⚔"
#define HEALTH_SPELL L"❤"
#define SPEED_SPELL L"⏳"
#define DAMAGE_SPELL L"☠"

#define MOVE_UP    8
#define MOVE_DOWN  2
#define MOVE_LEFT  4
#define MOVE_RIGHT 6
#define MOVE_UP_LEFT    7
#define MOVE_UP_RIGHT   9
#define MOVE_DOWN_LEFT  1
#define MOVE_DOWN_RIGHT 3

int player_x = MAX_X / 2, player_y = MAX_Y / 2;
int current_floor = 0;
wchar_t map[MAX_X][MAX_Y] = {{0}};  // تغییر از char به wchar_t
int food_count = 0;
int health = 100;
int hunger = 0;
int move_mode = 0;
int gold_count = 0;
int score = 0;

#define MAX_WEAPONS 10
#define MAX_SPELLS 10
char* weapons[MAX_WEAPONS];
char* spells[MAX_SPELLS];
int weapon_count = 0;
int spell_count = 0;
char* default_weapon = "Club";  // سلاح پیش‌فرض گرز

// ذخیره‌سازی
void save_game(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        mvprintw(0, 0, "Error saving game!");
        return;
    }

    fprintf(file, "%d %d\n", player_x, player_y);
    fprintf(file, "%d\n", current_floor);
    fprintf(file, "%d %d\n", gold_count, food_count);
    fprintf(file, "%d\n", health);
    fprintf(file, "%d\n", hunger);
    fprintf(file, "%d\n", weapon_count);
    for (int i = 0; i < weapon_count; i++) {
        fprintf(file, "%s\n", weapons[i]);
    }
    fprintf(file, "%d\n", spell_count);
    for (int i = 0; i < spell_count; i++) {
        fprintf(file, "%s\n", spells[i]);
    }
    fclose(file);
}

extern void load_game(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        mvprintw(0, 0, "Error loading game!");
        return;
    }

    fscanf(file, "%d %d\n", &player_x, &player_y);
    fscanf(file, "%d\n", &current_floor);
    fscanf(file, "%d %d\n", &gold_count, &food_count);
    fscanf(file, "%d\n", &health);
    fscanf(file, "%d\n", &hunger);
    
    fscanf(file, "%d\n", &weapon_count);
    for (int i = 0; i < weapon_count; i++) {
        weapons[i] = malloc(100 * sizeof(char));
        fscanf(file, "%s\n", weapons[i]);
    }
    
    fscanf(file, "%d\n", &spell_count);
    for (int i = 0; i < spell_count; i++) {
        spells[i] = malloc(100 * sizeof(char));
        fscanf(file, "%s\n", spells[i]);
    }

    fclose(file);
}

extern void draw_room() {
    for (int i = 0; i < MAX_Y; i++) {
        for (int j = 0; j < MAX_X; j++) {
            if ((i == 0)||(j == 0 ) || (i == MAX_X - 1) || (j == MAX_Y - 1)) {
                mvaddch(i, j, WALL);
            } else {
                mvaddch(i, j, FLOOR);
            }

            if (rand() % 100 < 2 && map[j][i] == 0 && food_count < 5) {
                map[j][i] = FOOD;
                food_count++;
            }

            if (rand() % 100 < 3 && map[j][i] == 0) {
                map[j][i] = GOLD;
            } else if (rand() % 100 < 1 && map[j][i] == 0) {
                map[j][i] = BLACK_GOLD;
            }

            if (rand() % 100 < 5 && map[j][i] == 0) {
                int item_type = rand() % 8;
                switch (item_type) {
                    case 0: map[j][i] = L'⚒'; break;  // CLUB
                    case 1: map[j][i] = L'🗡'; break;  // DAGGER
                    case 2: map[j][i] = L'杖'; break;  // WAND
                    case 3: map[j][i] = L'←'; break;  // ARROW
                    case 4: map[j][i] = L'⚔'; break;  // SWORD
                    case 5: map[j][i] = L'❤'; break;  // HEALTH_SPELL
                    case 6: map[j][i] = L'⏳'; break;  // SPEED_SPELL
                    case 7: map[j][i] = L'☠'; break;  // DAMAGE_SPELL
                }
            }
        }
    }
mvaddch(player_y, player_x, PLAYER);
    mvprintw(MAX_Y - 1, 0, "Health: %d  Food: %d  Hunger: %d  Gold: %d  Score: %d", health, food_count, hunger, gold_count, score);
    refresh();
}

extern void move_player(int direction) {
    int new_x = player_x;
    int new_y = player_y;

    switch (direction) {
        case MOVE_UP: new_y--; break;
        case MOVE_DOWN: new_y++; break;
        case MOVE_LEFT: new_x--; break;
        case MOVE_RIGHT: new_x++; break;
        case MOVE_UP_LEFT: new_x--; new_y--; break;
        case MOVE_UP_RIGHT: new_x++; new_y--; break;
        case MOVE_DOWN_LEFT: new_x--; new_y++; break;
        case MOVE_DOWN_RIGHT: new_x++; new_y++; break;
        default: break;
    }

    if (map[new_y][new_x] != WALL) {
        player_x = new_x;
        player_y = new_y;

        if (map[new_y][new_x] == GOLD) {
            gold_count += 10;
            score += 10;
            map[new_y][new_x] = 0;
            mvprintw(MAX_Y - 2, 0, "You picked up 10 gold!");
        } else if (map[new_y][new_x] == BLACK_GOLD) {
            gold_count += 50;
            score += 50;
            map[new_y][new_x] = 0;
            mvprintw(MAX_Y - 2, 0, "You picked up 50 black gold!");
        }
    }
}

extern void print_map() {
    // کد چاپ نقشه
    draw_room();
}

extern void eat_food() {
    // کد خوردن غذا و افزایش سلامتی
    if (food_count > 0) {
        health += 10;
        hunger -= 5;
        food_count--;
        mvprintw(MAX_Y - 2, 0, "You ate food! Health: %d, Hunger: %d", health, hunger);
    }
}

extern int main() {
    int choice;
    char username[50], password[50], email[100];
    setlocale(LC_CTYPE, "");  // برای پشتیبانی از یونیکد
    
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);

    srand(time(NULL));

    while (1) {
        clear();
        mvprintw(1, 10, "Main Menu");
        mvprintw(3, 5, "1. Create New User");
        mvprintw(4, 5, "2. Start Game");
        mvprintw(5, 5, "3. Exit");
        mvprintw(7, 5, "Choose an option: ");
        refresh();

        choice = getch() - '0'; // خواندن ورودی عددی

        switch (choice) {
            case 1: {
                // بخش ایجاد کاربر جدید
                echo();
                mvprintw(3, 25, "               ");
                mvprintw(3, 25, "> ");
                refresh();
                getstr(username);
                noecho();

                mvprintw(4, 25, "               ");
                mvprintw(4, 25, "> ");
                refresh();
                getstr(password);
                noecho();

                mvprintw(5, 25, "               ");
                mvprintw(5, 25, "> ");
                refresh();
                getstr(email);
                noecho();

                if (username_exists(username)) {
                    mvprintw(6, 5, "Username already exists!");
                } else if (!validate_password(password)) {
                    mvprintw(6, 5, "Invalid password!");
                } else if (!validate_email(email)) {
                    mvprintw(6, 5, "Invalid email format!");
                } else {
                    User new_user = {0};
                    strcpy(new_user.username, username);
                    strcpy(new_user.password, password);
                    strcpy(new_user.email, email);
                    save_user(&new_user);
                    mvprintw(6, 5, "User saved successfully!");
                }
                refresh();
                getch();
                break;
            }
            case 2: {
                // بخش شروع بازی
                clear();
                setlocale(LC_CTYPE, ""); // پشتیبانی از یونیکد
                init_game();
                while (1) {
                    print_map();
                    move_player(MOVE_UP);  // به طور پیش‌فرض حرکت بالا
                    getch();
                }
                break;
            }
            case 3:
                endwin();
                return 0;
        }
    }

    endwin();
    return 0;
}
