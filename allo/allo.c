#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // ابتدا SDL را مقداردهی اولیه کنید
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    // مقداردهی اولیه برای SDL_mixer
    if (Mix_Init(MIX_INIT_MP3) == 0) {
        printf("Mix_Init Error: %s\n", Mix_GetError());
        return -1;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        return -1;
    }

    // بارگذاری و پخش فایل صوتی
    Mix_Music* music = Mix_LoadMUS("/home/mostafa/Music/ayjan .mp3");
    if (!music) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
        return -1;
    }

    Mix_PlayMusic(music, -1); // پخش آهنگ به طور مداوم

    // صبر کنید تا موسیقی تمام شود
    while (Mix_PlayingMusic()) {
        SDL_Delay(100);
    }

    // آزاد کردن منابع
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    return 0;
}
