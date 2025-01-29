#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

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
    int x,y;     //موقعیت مکانی اسلحه 
    char name[20]; //نام اسلحه 
    int damage;    
    int maxRange;   //حداکثر برد اسلحه (برد اسلحه )
    int count;   // تعداد موجودی 
    int isThrowable;  // آیا قابل پرتاب است یا خیر ؟
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
Monster monsters[MAX_MONSTERS];
Food foods[MAX_FOODS];
Weapon weapons[MAX_WEAPONS];
Weapon inventory[MAX_WEAPONS] = {
    {0, 0, "Mace", 5, 0, 1, 0},
    {1, 1,"Dagger", 12, 5, 10, 1},
    {2, 2,"Magic Wand", 15, 10, 8, 1},
    {3, 3,"Normal Arrow", 5, 5, 20, 1},
    {4, 4,"Sword", 10, 0, 1, 0}
};

// توابع مربوط به اسلحه
void displayInventory() {
    printf("Inventory:\n");
    for (int i = 0; i < 5; i++) {
        printf("%c) %s - Damage: %d, Range: %d, Count: %d\n",
               'a' + i, inventory[i].name, inventory[i].damage,
               inventory[i].maxRange, inventory[i].count);
    }
}

void changeWeapon(int index, Weapon* currentWeapon) {
    if (inventory[index].count > 0 || inventory[index].isThrowable == 0) {
        *currentWeapon = inventory[index];
        printf("Default weapon changed to %s.\n", currentWeapon->name);
    } else {
        printf("You cannot select this weapon (out of stock).\n");
    }
}

void useWeapon(Weapon* currentWeapon) {
    if (currentWeapon->isThrowable) {
        if (currentWeapon->count > 0) {
            currentWeapon->count--;
            printf("You used %s! Remaining: %d.\n",
                   currentWeapon->name, currentWeapon->count);
        } else {
            printf("No %s left to use!\n", currentWeapon->name);
        }
    } else {
        printf("You attacked with %s!\n", currentWeapon->name);
    }
}

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
                case 0:
                    hero.health += 20;
                    break;
                case 1:
                    hero.health += 30;
                    hero.power += 1;
                    break;
                case 2:
                    hero.health += 30;
                    hero.speed += 1;
                    break;
                case 3:
                    hero.health -= 10;
                    break;
            }
            foods[i].type = -1;
        }
    }
}

void updateFoodState() {
    for (int i = 0; i < MAX_FOODS; i++) {
        if (foods[i].type != -1 && foods[i].timeToExpire > 0) {
            foods[i].timeToExpire--;
            if (foods[i].timeToExpire == 0) {
                if (foods[i].type == 1 || foods[i].type == 2)
                    foods[i].type = 0;
                else if (foods[i].type == 0)
                    foods[i].type = 3;
            }
        }
    }
}
void recoverHealth() {
    if (hero.hunger >= 80 && hero.health < 100) {
        hero.health += 5;
    }
}

void checkWeaponCollision() {
    for (int i = 0; i < MAX_WEAPONS; i++) {
        if (weapons[i].x == hero.x && weapons[i].y == hero.y && weapons[i].count > 0) {
            weapons[i].count--;
            printf("Weapon picked up! Damage: %d\n", weapons[i].damage);
        }
    }
}

void attackMonsters() {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].isActive && distance(monsters[i].x, monsters[i].y, hero.x, hero.y) <= 1) {
            monsters[i].health -= hero.power * 10;
            if (monsters[i].health <= 0) {
                monsters[i].isActive = false;
                printf("Monster defeated!\n");
            }
        }
    }
}

// حلقه اصلی بازی
void gameLoop() {
    Weapon currentWeapon = inventory[0];
    char choice;

    while (hero.health > 0) {
        moveMonsters();
        checkFoodCollision();
        checkWeaponCollision();
        attackMonsters();
        recoverHealth();
        updateFoodState();

        printf("Health: %d | Hunger: %d\n", hero.health, hero.hunger);
        hero.hunger -= 1;
        if (hero.hunger <= 0) {
            hero.hunger = 0;
            hero.health -= 1;
        }

        if (hero.health <= 0) {
            printf("Game Over!\n");
            break;
        }

        printf("\nCurrent Weapon: %s (Damage: %d, Range: %d, Count: %d)\n",
               currentWeapon.name, currentWeapon.damage,
               currentWeapon.maxRange, currentWeapon.count);
        printf("Options:\n");
        printf("i) Show inventory\n");
        printf("w) Change weapon\n");
        printf("space) Use weapon\n");
        printf("q) Quit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'i':
                displayInventory();
                break;
            case 'w':
                printf("Select weapon (a-e): ");
                char weaponChoice;
                scanf(" %c", &weaponChoice);
                if (weaponChoice >= 'a' && weaponChoice <= 'e') {
                    changeWeapon(weaponChoice - 'a', &currentWeapon);
                } else {
                    printf("Invalid weapon choice.\n");
                }
                break;
            case ' ':
                useWeapon(&currentWeapon);
                break;
            case 'q':
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

// مقداردهی اولیه
int main() {
    srand(time(NULL));

    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i] = (Monster){rand() % 10, rand() % 10, 50, true};
    }

    for (int i = 0; i < MAX_FOODS; i++) {
        foods[i] = (Food){rand() % 10, rand() % 10, rand() % 4, rand() % 20 + 10};
    }

    for (int i = 0; i < MAX_WEAPONS; i++) {
        weapons[i] = (Weapon){rand() % 10, rand() % 10, rand() % 20 + 10, true};
    }

    gameLoop();
    return 0;
}
