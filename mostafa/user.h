#ifndef USER_H
#define USER_H
#define MAX_SONGS 5 
#define PASSWORD_LENGTH 4
#define MAX_MSG_LENGTH 100
#define MESSAGE_AREA_HEIGHT 3

typedef struct {
    char username[50];
    char password[50];
    char email[100];
} User;

typedef struct {
    char save_name[50];
    char username[50];
    int level;
    int score;
} GameSave;
int init_game();
     int player_x;
     int plater_y;
  void Generate_rooms();
  int username_exists(const char *username);
  int validate_password(const char *password);
 int validate_email(const char *email);
 void save_user(const User *user);
 void generate_random_password(char *password, int length);
 int authenticate_user(const char *username, const char *password);
 char *retrieve_password(const char *username);
 void show_create_user_menu();
 void show_login_menu();
 void move_player();
 void eat_food();
 void print_map(); 
typedef struct {
    char username[50];
    int total_score;
    int total_gold;
    int games_completed;
    time_t first_game_time; // زمان اولین بازی
} PlayerStats;

 bool validate_password1();
 void pre_game_menu();
 void new_game(const char *username);
 void continue_game(const char *username);
 void show_scoreboard(const char *username);
 void show_settings();
 void show_profile(const char *username);
 void save_settings(int difficulty);
 int load_settings();
 void show_scoreboard(const char *logged_in_user);
 void add_sample_players();
  void show_settings_menu();
 void change_difficulty(int *difficulty);
  void change_hero_color(int *hero_color);
 void simulate_game_event();
 void change_game_music(char selected_song[MAX_SONGS][50], int *selected_index);
 int main();



#endif // USER_H
