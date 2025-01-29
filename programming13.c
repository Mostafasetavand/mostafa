#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

#define MAX_MONSTERS 10
#define MAX_FOODS 10
#define MAX_WEAPONS 5

// ساختار هیولا
typedef struct {
    int x, y;       // موقعیت هیولا
    int health;     // جان هیولا
    bool isActive;  // آیا هیولا فعال است؟
} Monster;

// ساختار غذا
typedef struct {
    int x, y;       // موقعیت غذا
    int type;       // نوع غذا: 0 = معمولی، 1 = اعلا، 2 = جادویی، 3 = فاسد
    int timeToExpire; // زمان انقضای غذا
} Food;

// ساختار اسلحه
typedef struct {
    int x, y;       // موقعیت اسلحه
    int damage;     // میزان آسیب
    bool isPickedUp; // آیا برداشته شده؟
} Weapon;

// ساختار قهرمان
typedef struct {
    int x, y;       // موقعیت قهرمان
    int health;     // جان قهرمان
    int hunger;     // میزان گرسنگی
    int power;      // قدرت
    int speed;      // سرعت
} Hero;

// متغیرهای جهانی
Hero hero = {5, 5, 100, 100, 1, 1}; // مقدار اولیه قهرمان
extern Monster monsters[MAX_MONSTERS];
extern Food foods[MAX_FOODS];
Weapon weapons[MAX_WEAPONS];

// توابع کمکی
int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// حرکت هیولاها
void moveMonsters() {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].isActive && distance(monsters[i].x, monsters[i].y, hero.x, hero.y) <= 5) {
            if (monsters[i].x < hero.x) monsters[i].x++;
            else if (monsters[i].x > hero.x) monsters[i].x--;
            
            if (monsters[i].y < hero.y) monsters[i].y++;
            else if (monsters[i].y > hero.y) monsters[i].y--;
        }
    }
}

// بررسی برخورد قهرمان با غذاها
void checkFoodCollision() {
    for (int i = 0; i < MAX_FOODS; i++) {
        if (foods[i].x == hero.x && foods[i].y == hero.y) {
            switch (foods[i].type) {
                case 0: // معمولی
                    hero.health += 20;
                    break;
                case 1: // اعلا
                    hero.health += 30;
                    hero.power += 1;
                    break;
                case 2: // جادویی
                    hero.health += 30;
                    hero.speed += 1;
                    break;
                case 3: // فاسد
                    hero.health -= 10;
                    break;
            }
            foods[i].type = -1; // غذا مصرف شد
        }
    }
}

// بررسی انقضای غذاها
void updateFoodState() {
    for (int i = 0; i < MAX_FOODS; i++) {
        if (foods[i].type != -1 && foods[i].timeToExpire > 0) {
            foods[i].timeToExpire--;
            if (foods[i].timeToExpire == 0) {
                if (foods[i].type == 1 || foods[i].type == 2)
                    foods[i].type = 0; // اعلا و جادویی تبدیل به معمولی
                else if (foods[i].type == 0)
                    foods[i].type = 3; // معمولی تبدیل به فاسد
            }
        }
    }
}

// بازیابی سلامتی
void recoverHealth() {
    if (hero.hunger >= 80 && hero.health < 100) {
        hero.health += 5;
    }
}

// بررسی برخورد قهرمان با اسلحه‌ها
void checkWeaponCollision() {
    for (int i = 0; i < MAX_WEAPONS; i++) {
        if (weapons[i].x == hero.x && weapons[i].y == hero.y && !weapons[i].isPickedUp) {
            weapons[i].isPickedUp = true;
            printf("  The gun was removed! damage: %d\n", weapons[i].damage);
        }
    }
}

// حمله به هیولاها
void attackMonsters() {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].isActive && distance(monsters[i].x, monsters[i].y, hero.x, hero.y) <= 1) {
            monsters[i].health -= hero.power * 10; // آسیب بر اساس قدرت
            if (monsters[i].health <= 0) {
                monsters[i].isActive = false;
                printf("  Tge monster was gone!\n");
            }
        }
    }
}

// حلقه اصلی بازی
void gameLoop() {
    while (hero.health > 0) {
        moveMonsters();
        checkFoodCollision();
        checkWeaponCollision();
        attackMonsters();
        recoverHealth();
        updateFoodState();

        // نمایش وضعیت قهرمان
        printf("heath: %3d | hunger: %3d\n", hero.health, hero.hunger);
// شبیه‌سازی گرسنگی
        hero.hunger -= 1;
        if (hero.hunger <= 0) {
            hero.hunger = 0;
            hero.health -= 1; // گرسنگی باعث کاهش سلامتی می‌شود
        }

        // پایان بازی
        if (hero.health <= 0) {
            printf(" You are lose! practice makes  perfect\n");
            break;
        }
    }
}

// int main() {
//     // مقداردهی اولیه
//     srand(time(NULL));

//     for (int i = 0; i < MAX_MONSTERS; i++) {
//         monsters[i] = (Monster){rand() % 10, rand() % 10, 50, true};
//     }

//     for (int i = 0; i < MAX_FOODS; i++) {
//         foods[i] = (Food){rand() % 10, rand() % 10, rand() % 4, rand() % 20 + 10};
//     }

//     for (int i = 0; i < MAX_WEAPONS; i++) {
//         weapons[i] = (Weapon){rand() % 10, rand() % 10, rand() % 20 + 10, false};
//     }

//     // شروع حلقه بازی
//     gameLoop();
//     return 0;
// }
