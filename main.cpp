#include <iostream>

extern "C" {
#include <SDL2/SDL.h>
}

int main() {
    SDL_Init(SDL_INIT_VIDEO); // SDL_INIT_EVERYTHING

    // 创建SDL窗口
    SDL_Window *window = SDL_CreateWindow("SDL2 Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!window) {
        SDL_Log("Failed to create window!");
        exit(1);
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_Log("Failed to create renderer!");
        exit(1);
    }

    // 指定渲染颜色
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // 清空当前窗口的颜色，避免和要渲染的颜色混在一起
    // 类似于OpenGL的渲染操作（先清空后渲染）：
    //      glClearColor()
    //      glClear()
    //      glDrawArrays()
    SDL_RenderClear(renderer);
    // 执行渲染操作，更新窗口
    SDL_RenderPresent(renderer);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_TARGET, 600, 450);
    if (!texture) {
        SDL_Log("Create texture failed!");
        exit(1);
    }

    bool quit = false;
    SDL_Event event;
    do {
        //SDL_WaitEvent(&event);
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                SDL_Log("Event type is %d [SDL_QUIT]", event.type);
                break;
            default:
                SDL_Log("Event type is %d", event.type);
                break;
        }

        SDL_Rect rect;
        rect.w = 30;
        rect.h = 30;
        rect.x = rand() % 600;
        rect.y = rand() % 450;

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);

    } while (!quit);
    //SDL_Delay(3000);

    // 释放资源
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
