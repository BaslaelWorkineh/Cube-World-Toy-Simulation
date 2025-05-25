#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <algorithm>

const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 200;

const int STICKMAN_HEIGHT = 12;
const int STICKMAN_WIDTH = 12;
const int PIXEL_SIZE = 3;

enum AnimState {
    IDLE_NORMAL = 0,
    IDLE_BLINK = 1,
    SWAY_LEFT = 2,
    SWAY_RIGHT = 3,
    FOOT_TAP = 4
};

int current_frame = 0;
Uint32 last_frame_time = 0;
int animation_counter = 0;
AnimState current_state = IDLE_NORMAL;

const int NUM_ANIM_FRAMES = 4;
int current_anim_frame = 0;
Uint32 last_anim_time = 0;
int animation_intervals[NUM_ANIM_FRAMES] = {2200, 120, 400, 400};

int stickman_frames[NUM_ANIM_FRAMES][STICKMAN_HEIGHT][STICKMAN_WIDTH];

// int stickman_patterns[5][STICKMAN_HEIGHT][STICKMAN_WIDTH] = {
//     {
//         {0,0,1,1,1,1,0,0},
//         {1,1,1,0,0,1,1,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,0,1,1,0,0,0},
//         {1,0,1,1,1,1,0,1},
//         {1,1,1,1,1,1,1,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,1,1,0,0,1,1,0}
//     },
//     {
//         {0,0,1,1,1,1,0,0},
//         {1,0,1,1,1,1,0,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,0,1,1,0,0,0},
//         {1,0,1,1,1,1,0,1},
//         {1,1,1,1,1,1,1,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,1,1,0,0,1,1,0}
//     },
//     {
//         {1,1,1,1,1,1,0,0},
//         {1,1,0,0,1,1,1,0},
//         {0,1,1,1,1,1,0,0},
//         {0,0,1,1,0,0,0,1},
//         {1,1,1,1,1,1,0,0},
//         {1,1,1,1,1,1,1,0},
//         {0,1,1,1,1,1,0,0},
//         {0,1,1,1,1,1,0,0},
//         {0,1,0,0,1,0,0,0},
//         {0,1,0,0,1,0,0,0},
//         {0,1,0,0,1,0,0,0},
//         {1,1,0,0,1,1,0,0}
//     },
//     {
//         {0,0,1,1,1,1,1,1},
//         {0,1,1,1,0,0,1,1},
//         {0,0,1,1,1,1,1,0},
//         {1,0,0,0,1,1,0,0},
//         {0,0,1,1,1,1,1,1},
//         {0,0,1,1,1,1,1,1},
//         {0,0,1,1,1,1,1,0},
//         {0,0,1,1,1,1,1,0},
//         {0,0,0,1,0,0,1,0},
//         {0,0,0,1,0,0,1,0},
//         {0,0,0,1,0,0,1,0},
//         {0,0,1,1,0,0,1,1}
//     },
//     {
//         {0,0,1,1,1,1,0,0},
//         {1,1,1,0,0,1,1,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,0,1,1,0,0,0},
//         {1,0,1,1,1,1,0,1},
//         {1,1,1,1,1,1,1,1},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,1,1,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,0,1,0,0,1,0,0},
//         {0,1,0,0,0,1,1,0}
//     }
// };

int anim_sequence[] = {IDLE_NORMAL, IDLE_BLINK, IDLE_NORMAL, SWAY_LEFT, IDLE_NORMAL, SWAY_RIGHT, IDLE_NORMAL, FOOT_TAP};
int anim_sequence_len = sizeof(anim_sequence) / sizeof(anim_sequence[0]);
int anim_seq_idx = 0;

void drawPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 9, 9, 9, 255);
    SDL_Rect pixel = {x, y, PIXEL_SIZE, PIXEL_SIZE};
    SDL_RenderFillRect(renderer, &pixel);
}

void updateAnimation() {
    Uint32 current_time = SDL_GetTicks();
    
    int frame_duration = 150;
    switch(current_state) {
        case IDLE_NORMAL:
            frame_duration = 2000 + (rand() % 1000);
            break;
        case IDLE_BLINK:
            frame_duration = 150;
            break;
        case SWAY_LEFT:
        case SWAY_RIGHT:
            frame_duration = 800;
            break;
        case FOOT_TAP:
            frame_duration = 300;
            break;
    }
    
    if (current_time - last_frame_time > frame_duration) {
        animation_counter++;
        last_frame_time = current_time;
        
        switch(current_state) {
            case IDLE_NORMAL:
                if (animation_counter % 8 == 0) {
                    current_state = IDLE_BLINK;
                } else if (animation_counter % 12 == 0) {
                    current_state = SWAY_LEFT;
                } else if (animation_counter % 15 == 0) {
                    current_state = FOOT_TAP;
                }
                break;
                
            case IDLE_BLINK:
                current_state = IDLE_NORMAL;
                break;
                
            case SWAY_LEFT:
                current_state = SWAY_RIGHT;
                break;
                
            case SWAY_RIGHT:
                current_state = IDLE_NORMAL;
                break;
                
            case FOOT_TAP:
                if (animation_counter % 2 == 0) {
                    current_state = IDLE_NORMAL;
                }
                break;
        }
    }
}

void initialize_stickman_frames() {
    int normal_frame[STICKMAN_HEIGHT][STICKMAN_WIDTH] = {
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,0,0,1,1,0,0,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {1,1,1,1,1,1,1,1,1,1},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,0,1,0,0,1,0,0,0},
        {0,0,1,1,0,0,1,1,0,0}
    };
    int blink_frame[STICKMAN_HEIGHT][STICKMAN_WIDTH];
    for (int i = 0; i < STICKMAN_HEIGHT; ++i)
        for (int j = 0; j < STICKMAN_WIDTH; ++j)
            blink_frame[i][j] = normal_frame[i][j];
    int blink_eye_row[] = {0,0,0,0,1,1,1,0};
    for (int j = 0; j < STICKMAN_WIDTH; ++j)
        blink_frame[1][j] = blink_eye_row[j];
    int sway_left[STICKMAN_HEIGHT][STICKMAN_WIDTH] = {0};
    for (int i = 0; i < STICKMAN_HEIGHT; ++i)
        for (int j = 0; j < STICKMAN_WIDTH-1; ++j)
            sway_left[i][j+1] = normal_frame[i][j];
    int sway_right[STICKMAN_HEIGHT][STICKMAN_WIDTH] = {0};
    for (int i = 0; i < STICKMAN_HEIGHT; ++i)
        for (int j = 1; j < STICKMAN_WIDTH; ++j)
            sway_right[i][j-1] = normal_frame[i][j];
    for (int i = 0; i < STICKMAN_HEIGHT; ++i) {
        for (int j = 0; j < STICKMAN_WIDTH; ++j) {
            stickman_frames[0][i][j] = normal_frame[i][j];
            stickman_frames[1][i][j] = blink_frame[i][j];
            stickman_frames[2][i][j] = sway_left[i][j];
            stickman_frames[3][i][j] = sway_right[i][j];
        }
    }
}

const int NUM_WINDOWS = 2;
const int DOCK_THRESHOLD = 220;

struct WindowInfo {
    SDL_Window* win;
    SDL_Renderer* ren;
    int id;
};

void get_window_pos(SDL_Window* win, int &x, int &y) {
    SDL_GetWindowPosition(win, &x, &y);
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    WindowInfo windows[NUM_WINDOWS];
    int start_x[NUM_WINDOWS] = {100, 350};
    int start_y[NUM_WINDOWS] = {100, 100};

    for (int i = 0; i < NUM_WINDOWS; ++i) {
        windows[i].win = SDL_CreateWindow(
            (std::string("Stickman Cube ") + std::to_string(i+1)).c_str(),
            start_x[i], start_y[i], SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        windows[i].ren = SDL_CreateRenderer(windows[i].win, -1, SDL_RENDERER_ACCELERATED);
        windows[i].id = SDL_GetWindowID(windows[i].win);
    }

    last_frame_time = SDL_GetTicks();
    int stickman_x = (SCREEN_WIDTH - (STICKMAN_WIDTH * PIXEL_SIZE)) / 2;
    int stickman_y = (SCREEN_HEIGHT - (STICKMAN_HEIGHT * PIXEL_SIZE)) / 2 + 10;
    int room_margin = 15;
    int room_x = room_margin;
    int room_y = room_margin;
    int room_w = SCREEN_WIDTH - (2 * room_margin);
    int room_h = SCREEN_HEIGHT - (2 * room_margin);

    bool quit = false;
    SDL_Event e;
    initialize_stickman_frames();

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;
        }
        int x0, y0, x1, y1;
        get_window_pos(windows[0].win, x0, y0);
        get_window_pos(windows[1].win, x1, y1);
        int dx = std::abs((x0 + SCREEN_WIDTH/2) - (x1 + SCREEN_WIDTH/2));
        int dy = std::abs((y0 + SCREEN_HEIGHT/2) - (y1 + SCREEN_HEIGHT/2));
        bool docked = (dx < DOCK_THRESHOLD && dy < DOCK_THRESHOLD);

        static bool was_docked = false;
        static float slide_progress = 0.0f;
        const float SLIDE_SPEED = 0.04f;

        int left_idx = (x0 < x1) ? 0 : 1;
        int right_idx = (x0 < x1) ? 1 : 0;

        if (docked) {
            if (!was_docked && slide_progress < 1.0f) {
                slide_progress = 0.0f;
            }
            slide_progress = std::min(1.0f, slide_progress + SLIDE_SPEED);
        } else {
            slide_progress = 0.0f;
        }
        was_docked = docked;

        updateAnimation();
        for (int w = 0; w < NUM_WINDOWS; ++w) {
            SDL_SetRenderDrawColor(windows[w].ren, 142, 154, 143, 255);
            SDL_RenderClear(windows[w].ren);
            if (docked) {
                SDL_SetRenderDrawColor(windows[w].ren, 0, 200, 0, 255);
            } else {
                SDL_SetRenderDrawColor(windows[w].ren, 80, 80, 80, 255);
            }
            SDL_Rect room_outline = {room_x, room_y, room_w, room_h};
            SDL_RenderDrawRect(windows[w].ren, &room_outline);
            SDL_Rect inner_room = {room_x + 3, room_y + 3, room_w - 6, room_h - 6};
            SDL_RenderDrawRect(windows[w].ren, &inner_room);
            SDL_SetRenderDrawColor(windows[w].ren, 60, 60, 60, 255);
            int floor_y = stickman_y + (STICKMAN_HEIGHT * PIXEL_SIZE);
            SDL_RenderDrawLine(windows[w].ren, room_x + 5, floor_y, room_x + room_w - 5, floor_y);
            if (docked) {
                for (int i = 0; i < STICKMAN_HEIGHT; i++) {
                    for (int j = 0; j < STICKMAN_WIDTH; j++) {
                        if (stickman_frames[current_anim_frame][i][j] == 1) {
                            drawPixel(windows[w].ren, stickman_x + (j * PIXEL_SIZE), stickman_y + (i * PIXEL_SIZE));
                        }
                    }
                }
                int slide_distance = std::abs(x1 - x0);
                int slide_offset = (int)(slide_distance * slide_progress);
                int slide_dir = (left_idx == w) ? 1 : -1;
                if (slide_progress > 0.0f && slide_progress < 1.0f) {
                    int moving_x;
                    if (w == left_idx) {
                        moving_x = stickman_x + slide_offset;
                    } else {
                        moving_x = stickman_x - slide_distance + slide_offset;
                    }
                    for (int i = 0; i < STICKMAN_HEIGHT; i++) {
                        for (int j = 0; j < STICKMAN_WIDTH; j++) {
                            if (stickman_frames[current_anim_frame][i][j] == 1) {
                                drawPixel(windows[w].ren, moving_x + (j * PIXEL_SIZE), stickman_y + (i * PIXEL_SIZE));
                            }
                        }
                    }
                }
            } else {
                for (int i = 0; i < STICKMAN_HEIGHT; i++) {
                    for (int j = 0; j < STICKMAN_WIDTH; j++) {
                        if (stickman_frames[current_anim_frame][i][j] == 1) {
                            drawPixel(windows[w].ren, stickman_x + (j * PIXEL_SIZE), stickman_y + (i * PIXEL_SIZE));
                        }
                    }
                }
            }
            SDL_RenderPresent(windows[w].ren);
        }
        SDL_Delay(50);
    }
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        SDL_DestroyRenderer(windows[i].ren);
        SDL_DestroyWindow(windows[i].win);
    }
    SDL_Quit();
    return 0;
}
