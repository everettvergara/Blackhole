#pragma once

#include <memory>
#include <vector>
#include <tuple>

#include "common.hpp"
#include "video.hpp"
#include "pal.hpp"
#include "particle.hpp"

namespace eg
{
    class video_anim : public video 
    {
    
    private:

        std::vector<particle> particles_;
        std::vector<Uint32> pal_;
        Uint N_;
        FP a_red_, r_red_;

        std::vector<FP> sine_, cosine_;

    public:
        
        video_anim(Uint N = 50000, FP angle_red = 0.005, FP rad_red = 0.5)
            : N_(N), a_red_(angle_red), r_red_(rad_red)
        {
        }

    protected:

        auto init() -> void override
        {
            auto surface = SDL_GetWindowSurface(win_);
            pal_ = get_palette_gradient(surface->format, {  {0, SDL_MapRGBA(surface->format, 255, 255, 255, 255)},
                                                            {50, SDL_MapRGBA(surface->format, 123, 207, 224, 255)},
                                                            {100, SDL_MapRGBA(surface->format, 28, 71, 152, 255)},
                                                            {150, SDL_MapRGBA(surface->format, 34, 15, 49, 255)},
                                                            {200, SDL_MapRGBA(surface->format, 94, 15, 66, 255)},
                                                            {surface->h, SDL_MapRGBA(surface->format, 206, 29, 109, 255)},
                                                            {surface->w, SDL_MapRGBA(surface->format, 0, 0, 0, 0)},
                                                        }).value();



            SDL_FillRect(surface, NULL, 0);
            
            particles_.reserve(N_);
            auto AN = static_cast<size_t>(surface->w / 0.005) + 1;
            sine_.reserve(AN);
            cosine_.reserve(AN);
            for (auto i = 0.0, inc = 2.0 * M_PI / AN; i < 2 * M_PI; i += inc)
            {
                sine_.emplace_back(SDL_sin(i));
                cosine_.emplace_back(SDL_sin(i));
            }


            FP r = 0.0;
            static constexpr FP r_inc = 0.5;
            static constexpr FP r_dec = -r_inc * 0.991;
            FP rc = 0.0;

            FP a = 0.0;
            static constexpr FP a_inc = 0.005;
            static constexpr FP a_dec = -a_inc;
            FP ac = 0.0;
            static constexpr FP aj = 1.0 / 35.0;

            for (decltype(N_) i = 0; i < N_; ++i)
            {
                auto action = rand() % 5;    
                if (action == 0) rc = r_inc;
                if (action == 1) rc = r_dec;
                if (action == 2) ac = a_inc;
                if (action == 3) ac = a_dec;

                r += rc;
                if (r < 0) r = 0;

                a += ac;
                auto ar = r * aj;

                particles_.emplace_back(particle{.r = r, .a = a + ar});
            }

        }

        auto event() -> bool override
        {
            SDL_Event e; 
            while(SDL_PollEvent(&e))
            { 
                switch (e.type)
                {
                    case SDL_QUIT: 
                        return false;

                }
            } 

            return true;
        }

        auto recalc() -> void override
        {
            auto surface = SDL_GetWindowSurface(win_);
            auto pixels = static_cast<Uint32 *>(surface->pixels);
            auto bound = pixels + (surface->w * surface->h);

            auto cx = surface->w >> 1;
            auto cy = surface->h >> 1;
            auto center = (pixels + cx) + (cy * surface->w);
            auto mr = surface->h >> 1;

            for (auto &p : particles_)
            {
                auto data = (center + p.x()) + (surface->w * p.y());
                if (data >= pixels and data < bound) *data = 0;
            }

            for (auto &p : particles_)
            {

                p.a -= a_red_;
                p.r -= r_red_;
                if (p.r < 0) p.r = mr;

                auto data = (center + p.x()) + (surface->w * p.y());
                if (data >= pixels and data < bound) *data = pal_.at(static_cast<int>(p.r));
            }
        }
    };
}
