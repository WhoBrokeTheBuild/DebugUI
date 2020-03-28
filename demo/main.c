#include <SDL.h>

#define DUI_IMPLEMENTATION
#include <DUI/DUI.h>

int main(int argc, char ** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * win = SDL_CreateWindow("DebugUI Demo",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600, 0);

    SDL_Renderer * ren = SDL_CreateRenderer(win, 0, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    DUI_Init(win);

    enum {
        TAB1,
        TAB2,
        TAB3,
        TAB4,
    };

    int tabIndex = TAB1;

    enum {
        INCREMENT,
        DECREMENT,
    };

    int incDecIndex = INCREMENT;

    bool autoTick = false;
    int counter = 0;

    SDL_Event evt;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) {
                running = false;
            }
        }

        DUI_Update();

        SDL_SetRenderDrawColor(ren, 0x33, 0x33, 0x33, 0xFF);
        SDL_RenderClear(ren);

        DUI_MoveCursor(8, 8);

        DUI_BeginTabBar();

        if (DUI_Tab("TAB1", TAB1, &tabIndex)) {
            DUI_MoveCursor(8, 40);
            DUI_Panel(800 - 16, 600 - 48);

            DUI_Println("TAB #1");
            DUI_Newline();

            DUI_Println("COUNTER: %d", counter);

            if (DUI_Button("TICK!") || autoTick) {
                if (incDecIndex == INCREMENT) {
                    ++counter;
                }
                else {
                    --counter;
                }
            }

            DUI_Checkbox("AUTO TICK", &autoTick);
            
            DUI_Newline();
            DUI_Newline();

            DUI_Radio("INCREMENT", INCREMENT, &incDecIndex);
            DUI_Newline();
            DUI_Newline();

            DUI_Radio("DECREMENT", DECREMENT, &incDecIndex);
            DUI_Newline();
            DUI_Newline();
        }
        
        if (DUI_Tab("TAB2", TAB2, &tabIndex)) {
            DUI_MoveCursor(8, 40);
            DUI_Panel(800 - 16, 600 - 48);

            DUI_Println("TAB #2");
            DUI_Newline();
        }
        
        if (DUI_Tab("TAB3", TAB3, &tabIndex)) {
            DUI_MoveCursor(8, 40);
            DUI_Panel(800 - 16, 600 - 48);

            DUI_Println("TAB #3");
            DUI_Newline();
        }
        
        if (DUI_Tab("TAB4", TAB4, &tabIndex)) {
            DUI_MoveCursor(8, 40);
            DUI_Panel(800 - 16, 600 - 48);

            DUI_Println("TAB #4");
            DUI_Newline();
        }

        SDL_RenderPresent(ren);
    }

    DUI_Term();

    return 0;
}