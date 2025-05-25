#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <string>

const int WIN_W = 200, WIN_H = 200;
const int NUM_WINDOWS = 2;
const int DOCK_THRESHOLD = 30; 

struct WindowInfo {
    SDL_Window* win;
    SDL_Renderer* ren;
    int id;
};

void get_window_pos(SDL_Window* win, int &x, int &y) {
    SDL_GetWindowPosition(win, &x, &y);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    WindowInfo windows[NUM_WINDOWS];
    int start_x[NUM_WINDOWS] = {100, 350};
    int start_y[NUM_WINDOWS] = {100, 100};

    for (int i = 0; i < NUM_WINDOWS; ++i) {
        windows[i].win = SDL_CreateWindow(
            (std::string("Cube ") + std::to_string(i+1)).c_str(),
            start_x[i], start_y[i], WIN_W, WIN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        windows[i].ren = SDL_CreateRenderer(windows[i].win, -1, SDL_RENDERER_ACCELERATED);
        windows[i].id = SDL_GetWindowID(windows[i].win);
    }

    bool quit = false;
    SDL_Event e;
    bool docked = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;
        }
        int x0, y0, x1, y1;
        get_window_pos(windows[0].win, x0, y0);
        get_window_pos(windows[1].win, x1, y1);
        int dx = std::abs((x0 + WIN_W/2) - (x1 + WIN_W/2));
        int dy = std::abs((y0 + WIN_H/2) - (y1 + WIN_H/2));
        docked = (dx < DOCK_THRESHOLD && dy < DOCK_THRESHOLD);

        for (int i = 0; i < NUM_WINDOWS; ++i) {
            SDL_SetRenderDrawColor(windows[i].ren, 30, 30, 30, 255);
            SDL_RenderClear(windows[i].ren);
            if (docked) {
                SDL_SetRenderDrawColor(windows[i].ren, 0, 200, 0, 255); 
            } else {
                SDL_SetRenderDrawColor(windows[i].ren, 100, 100, 100, 255);
            }
            SDL_Rect border = {10, 10, WIN_W-20, WIN_H-20};
            SDL_RenderDrawRect(windows[i].ren, &border);
            SDL_RenderPresent(windows[i].ren);
        }
        SDL_Delay(16);
    }
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        SDL_DestroyRenderer(windows[i].ren);
        SDL_DestroyWindow(windows[i].win);
    }
    SDL_Quit();
    return 0;
}
