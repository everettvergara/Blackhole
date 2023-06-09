#pragma once

#include <stdexcept>
#include "SDL2/SDL.h"

namespace eg
{

    constexpr int   FPS_    = 240;
    constexpr int   MSPF_   = 1000 / FPS_;

    class video
    {

    protected:
        SDL_Window      *win_   = NULL;
        
    public:

        video()
        {
        }

        virtual ~video()
        {
            if (win_) SDL_DestroyWindow(win_);
        }

        auto create_win(const char *title, const int w, const int h)
        {
            win_    =   SDL_CreateWindow(
                            title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            w, h, SDL_WINDOW_SHOWN);

            if (win_ == NULL) 
                throw std::runtime_error("Could not create window.");
        }

        auto run()
        {
            init();

            do
            {
                auto start = SDL_GetTicks();

                if (not event()) break;
                recalc();
                update();
  
                if (auto elapsed = SDL_GetTicks() - start;
                    elapsed < MSPF_) SDL_Delay(MSPF_ - elapsed);

            } while (true);
            
        }

    protected:

        virtual auto init() -> void
        {
        }

        virtual auto recalc() -> void
        {
        }

        virtual auto update() -> void
        {
            SDL_UpdateWindowSurface(win_);
        }


        virtual auto event() -> bool
        {
            SDL_Event e; 
            while(SDL_PollEvent(&e))
                if (e.type == SDL_QUIT) return false;
        
            return true;
        }
    };

}
