#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MONSTERS 10
#define MAX_FOODS 10
#define MAX_WEAPONS 5



typedef struct 
{
 int x,y,health;
 bool alive ;
}Monster;



typedef struct 
{
    int x,y,type ,value;

}Food;

// تعریف ساختار برای سلاح‌ها
typedef struct {
    char name[20];
    int damage;
    int maxRange; // برای نشان دادن برد سلاح
    int count;    // تعداد موجودی
    int isThrowable; // آیا پرتاب‌شدنی است؟ (1 بله، 0 خیر)
} Weapon;


Monster monsters [MAX_MONSTERS];
Food foods[MAX_FOODS];
Weapon Weapons [MAX_WEAPONS];
Weapon inventory[MAX_WEAPONS];


// تعریف موجودی
Weapon inventory[] = {
    {"Mace", 5, 0, 1, 0},
    {"Dagger", 12, 5, 10, 1},
    {"Magic Wand", 15, 10, 8, 1},
    {"Normal Arrow", 5, 5, 20, 1},
    {"Sword", 10, 0, 1, 0}
};

// تابع برای نمایش موجودی
void displayInventory() {
    printf("Inventory:\n");
    for (int i = 0; i < 5; i++) {
        printf("%c) %s - Damage: %d, Range: %d, Count: %d\n",
               'a' + i, inventory[i].name, inventory[i].damage,
               inventory[i].maxRange, inventory[i].count);
    }
}

// تابع برای تغییر سلاح پیش‌فرض
void changeWeapon(int index, Weapon* currentWeapon) {
    if (inventory[index].count > 0 || inventory[index].isThrowable == 0) {
        *currentWeapon = inventory[index];
        printf("Default weapon changed to %s.\n", currentWeapon->name);
    } else {
        printf("You cannot select this weapon (out of stock).\n");
    }
}

// تابع برای پرتاب یا استفاده از سلاح
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

int main() {
    // مقداردهی اولیه
    srand(time(NULL));

    for (int i = 0; i < MAX_MONSTERS; i++) {
        monsters[i] = (Monster){rand() % 10, rand() % 10, 50, true};
    }

    for (int i = 0; i < MAX_FOODS; i++) {
        foods[i] = (Food){rand() % 10, rand() % 10, rand() % 4, rand() % 20 + 10};
    }

    for (int i = 0; i < MAX_WEAPONS; i++) {
        Weapons[i] = (Weapon){rand() % 10, rand() % 10, rand() % 20 + 10, false};
    }

    Weapon currentWeapon = inventory[0]; // سلاح پیش‌فرض: گرز
    char choice;
    int running = 1;

    while (running) {
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
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    printf("Game Over.\n");
    return 0;
}