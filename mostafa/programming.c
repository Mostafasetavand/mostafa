#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "user.h"


#define FILENAME "users.txt"

     
//  extern int main() {
//     initscr();               
//     cbreak();                
//     noecho();                
//     keypad(stdscr, TRUE);    //فعال کردن گزینه مورد نیاز 

//     char username[50], password[50], email[100];
//     int choice;

//     while (1) {
//         clear();
//         mvprintw(1, 10, "Create New User");
//         mvprintw(3, 5, "1. Enter Username: ");
//         mvprintw(4, 5, "2. Enter Password: ");
//         mvprintw(5, 5, "3. Enter Email: ");
//         mvprintw(6, 5, "4. Generate Random Password");
//         mvprintw(7, 5, "5. Save and Exit");
//         mvprintw(8, 5, "6. Exit Without Saving");
//         mvprintw(10, 5, "Choose an option: ");
//         refresh();

//         choice = getch() - '0'; // خواندن ورودی عددی

//         switch (choice) {
//             case 1: {
//                 echo();
//                 mvprintw(3, 25, "               ");
//                 mvprintw(3, 25, "> ");
//                 refresh();
//                 getstr(username);
//                 noecho();
//                 if (username_exists(username)) {
//                     mvprintw(11, 5, "Username already exists!");
//                 } else {
//                     mvprintw(11, 5, "Username is available!");
//                 }
//                 refresh();
//                 getch();
//                 break;
//             }

//             case 2: {
//                 echo();
//                 mvprintw(4, 25, "               ");
//                 mvprintw(4, 25, "> ");
//                 refresh();
//                 getstr(password);
//                 noecho();
//                 if (validate_password(password)) {
//                     mvprintw(11, 5, "Password is valid!");
//                 } else {
//                     mvprintw(11, 5, "Password must be at least 7 characters, and include a number, an uppercase, and a lowercase letter.");
//                 }
//                 refresh();
//                 getch();
//                 break;
//             }
//             case 3: {
//                 echo();
//                 mvprintw(5, 25, "               ");
//                 mvprintw(5, 25, "> ");
//                 refresh();
//                 getstr(email);
//                 noecho();
//                 if (validate_email(email)) {
//                     mvprintw(11, 5, "Email is valid!");
//                 } else {
//                     mvprintw(11, 5, "Invalid email format!");
//                 }
//                 refresh();
//                 getch();
//                 break;
//             }
//             case 4: {
//                 generate_random_password(password, 12);
//                 mvprintw(4, 25, "               ");
//                 mvprintw(4, 25, "> %s", password);
//                 mvprintw(11, 5, "Random password generated.");
//                 refresh();
//                 getch();
//                 break;
//             }
//             case 5: {
//                 if (!username_exists(username) && validate_password(password) && validate_email(email)) {
//                     User new_user = {0};
//                     strcpy(new_user.username, username);
//                     strcpy(new_user.password, password);
//                     strcpy(new_user.email, email);
//                     save_user(&new_user);
//                     mvprintw(11, 5, "User saved successfully!");
//                 } else {
//                     mvprintw(11, 5, "Fix errors before saving.");
//                 }
//                 refresh();
//                 getch();
//                 break;
//             }
//             case 6:
//                 endwin();
//                 return 0;
//         }
//     }
//     endwin();
//     return 0;
// }

extern int username_exists(const char *username) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

extern int validate_password(const char *password) {
    if (strlen(password) < 7) return 0;

    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (int i = 0; password[i]; i++) {
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
        if (isdigit(password[i])) has_digit = 1;
    }

    return has_upper && has_lower && has_digit;
}

extern int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && dot - at > 1;
}

extern void save_user(const User *user) {
    FILE *file = fopen(FILENAME, "ab");
    if (file) {
        fwrite(user, sizeof(User), 1, file);
        fclose(file);
    }
}

extern void generate_random_password(char *password, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";    
    for (int i = 0; i < length - 1; i++) {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[length - 1] = '\0';
}

