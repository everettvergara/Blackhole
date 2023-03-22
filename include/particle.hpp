#pragma once

#include <SDL.h>
#include "common.hpp"

namespace eg
{
    struct particle
    {
        FP r;
        int a;

        inline auto x() -> Sint
        {
            return static_cast<Sint>(SDL_cos(a) * r);
        }

        inline auto y() -> Sint
        {
            return static_cast<Sint>(SDL_sin(a) * r);
        }

    };
}

