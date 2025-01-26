#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include "user.h"


#define SCOREBOARD_FILE "scoreboard.txt"
#define ROWS_PER_PAGE 5 // تعداد ردیف‌های قابل نمایش در هر صفحه

// typedef struct {
//     char username[50];
//     int total_score;
//     int total_gold;
//     int games_completed;
//     time_t first_game_time; }// زمان اولین بازی





void show_scoreboard(const char *logged_in_user) {
    FILE *file = fopen(SCOREBOARD_FILE, "r");
    if (!file) {
        printw("No data available.\n");
        refresh();
        getch();
        return;
    }

    PlayerStats players[100];
    int player_count = 0;

    // خواندن اطلاعات از فایل
    while (fread(&players[player_count], sizeof(PlayerStats), 1, file)) {
        player_count++;
    }
    fclose(file);

    // مرتب‌سازی کاربران بر اساس امتیاز کل
    for (int i = 0; i < player_count - 1; i++) {
        for (int j = 0; j < player_count - i - 1; j++) {
            if (players[j].total_score < players[j + 1].total_score) {
                PlayerStats temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    int current_page = 0;
    int max_pages = (player_count + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;

    while (1) {
        clear();
        mvprintw(1, 10, "Scoreboard - Page %d/%d", current_page + 1, max_pages);
        mvprintw(3, 5, "Rank | Username      | Medal  | Total Score | Total Gold | Games Completed | Experience (days)");
        mvprintw(4, 5, "---------------------------------------------------------------------------------------------");

        for (int i = current_page * ROWS_PER_PAGE; i < (current_page + 1) * ROWS_PER_PAGE && i < player_count; i++) {
            time_t now = time(NULL);
            int experience_days = (now - players[i].first_game_time) / (60 * 60 * 24);

            // تنظیم مدال‌ها
            const char *medal = "";
            if (i == 0) medal = "🏆";       // نفر اول
            else if (i == 1) medal = "🥈";  // نفر دوم
            else if (i == 2) medal = "🥉";  // نفر سوم

            // تنظیم رنگ و برجسته‌سازی
            if (i == 0) attron(COLOR_PAIR(1) | A_BOLD); // رنگ طلایی
            else if (i == 1) attron(COLOR_PAIR(2) | A_BOLD); // رنگ سفید
            else if (i == 2) attron(COLOR_PAIR(3) | A_BOLD); // رنگ آبی روشن
            else attron(COLOR_PAIR(4));

            if (strcmp(players[i].username, logged_in_user) == 0) {
                attron(A_UNDERLINE); // برجسته کردن کاربر لاگین‌شده
            }

            mvprintw(6 + (i % ROWS_PER_PAGE), 5, "%4d | %-13s | %-6s | %11d | %10d | %15d | %16d",
                     i + 1,
                     players[i].username,
                     medal,
                     players[i].total_score,
                     players[i].total_gold,
                     players[i].games_completed,
                     experience_days);

            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) | A_BOLD | A_UNDERLINE);
        }

        mvprintw(LINES - 2, 5, "Use LEFT/RIGHT to navigate pages, UP/DOWN to scroll, 'q' to quit.");

        refresh();

        int ch = getch();
        if (ch == 'q') break;
        else if (ch == KEY_LEFT && current_page > 0) current_page--;  // رفتن به صفحه قبل
        else if (ch == KEY_RIGHT && current_page < max_pages - 1) current_page++; // رفتن به صفحه بعد
    }
}

void add_sample_players() {
    FILE *file = fopen(SCOREBOARD_FILE, "w");
    if (!file) return;

    PlayerStats players[] = {
        {"Player1", 5000, 300, 10, time(NULL) - (3600 * 24 * 300)},
        {"Player2", 4500, 250, 8, time(NULL) - (3600 * 24 * 200)},
        {"Player3", 6000, 400, 15, time(NULL) - (3600 * 24 * 400)},
        {"Player4", 2000, 100, 5, time(NULL) - (3600 * 24 * 100)},
        {"Player5", 3500, 150, 7, time(NULL) - (3600 * 24 * 150)},
        {"Player6", 1500, 50, 3, time(NULL) - (3600 * 24 * 50)},
        {"Player7", 2500, 80, 4, time(NULL) - (3600 * 24 * 120)},
    };

    fwrite(players, sizeof(PlayerStats), 7, file);
    fclose(file);
}