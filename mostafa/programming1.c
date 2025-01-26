#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include "user.h"
#include "programming.c"


//#define FILENAME "users.txt"




extern void show_create_user_menu() {
    char username[50], password[50], email[100];
    int choice;

    while (1) {
        clear();
        mvprintw(1, 10, "Create New User");
        mvprintw(3, 5, "1. Enter Username: ");
        mvprintw(4, 5, "2. Enter Password: ");
        mvprintw(5, 5, "3. Enter Email: ");
        mvprintw(6, 5, "4. Generate Random Password");
        mvprintw(7, 5, "5. Save and Exit");
        mvprintw(8, 5, "6. Exit Without Saving");
        mvprintw(10, 5, "Choose an option: ");
        refresh();

        choice = getch() - '0';

        switch (choice) {
            case 1: {
                echo();
                mvprintw(3, 25, "               ");
                mvprintw(3, 25, "> ");
                refresh();
                getstr(username);
                noecho();
                if (username_exists(username)) {
                    mvprintw(11, 5, "Username already exists!");
                } else {
                    mvprintw(11, 5, "Username is available!");
                }
                refresh();
                getch();
                break;
            }
            case 2: {
                echo();
                mvprintw(4, 25, "               ");
                mvprintw(4, 25, "> ");
                refresh();
                getstr(password);
                noecho();
                if (validate_password(password)) {
                    mvprintw(11, 5, "Password is valid!");
                } else {
                    mvprintw(11, 5, "Password must be at least 7 characters, and include a number, an uppercase, and a lowercase letter.");
                }
                refresh();
                getch();
                break;
            }
            case 3: {
                echo();
                mvprintw(5, 25, "               ");
                mvprintw(5, 25, "> ");
                refresh();
                getstr(email);
                noecho();
                if (validate_email(email)) {
                    mvprintw(11, 5, "Email is valid!");
                } else {
                    mvprintw(11, 5, "Invalid email format!");
                }
                refresh();
                getch();
                break;
            }
            case 4: {
                generate_random_password(password, 12);
                mvprintw(4, 25, "               ");
                mvprintw(4, 25, "> %s", password);
                mvprintw(11, 5, "Random password generated.");
                refresh();
                getch();
                break;
            }
            case 5: {
                if (!username_exists(username) && validate_password(password) && validate_email(email)) {
                    User new_user = {0};
                    strcpy(new_user.username, username);
                    strcpy(new_user.password, password);
                    strcpy(new_user.email, email);
                    save_user(&new_user);
                    mvprintw(11, 5, "User saved successfully!");
                } else {
                    mvprintw(11, 5, "Fix errors before saving.");
                }
                refresh();
                getch();
                return;
            }
            case 6:
                return;
        }
    }
}

extern void show_login_menu() {
    char username[50], password[50];
    int choice;

    while (1) {
        clear();
        mvprintw(1, 10, "Login Menu");
        mvprintw(3, 5, "1. Login with Username and Password");
        mvprintw(4, 5, "2. Login as Guest");
        mvprintw(5, 5, "3. Forgot Password");
        mvprintw(6, 5, "4. Return to Main Menu");
        mvprintw(8, 5, "Choose an option: ");
        refresh();

        choice = getch() - '0';

        switch (choice) {
            case 1: {
                echo();
                mvprintw(3, 35, "                    ");
                mvprintw(3, 35, "> Username: ");
                refresh();
                getstr(username);

                mvprintw(4, 35, "                    ");
                mvprintw(4, 35, "> Password: ");
                refresh();
                getstr(password);
                noecho();

                if (authenticate_user(username, password)) {
                    mvprintw(11, 5, "Login successful! Welcome %s.", username);
                } else {
                    mvprintw(11, 5, "Invalid username or password.");
                }
                refresh();
                getch();
                break;
            }
            case 2: {
                mvprintw(11, 5, "Logged in as Guest.");
                refresh();
                getch();
                return;
            }
            case 3: {
                echo();
                mvprintw(5, 35, "                    ");
                mvprintw(5, 35, "> Enter Username: ");
                refresh();
                getstr(username);
                noecho();

                char *retrieved_password = retrieve_password(username);
                if (retrieved_password) {
                    mvprintw(11, 5, "Your password: %s", retrieved_password);
                    free(retrieved_password);
                } else {
                    mvprintw(11, 5, "Username not found.");
                }
                refresh();
                getch();
                break;
            }
            case 4:
                return;
        }
    }
}

extern int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

extern char *retrieve_password(const char *username) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return NULL;

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            fclose(file);
            char *password = malloc(strlen(user.password) + 1);
            strcpy(password, user.password);
            return password;
        }

    }


    fclose(file);
    return NULL;
}