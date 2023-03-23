#include <iostream>

#include "video_init.hpp"
#include "video_anim.hpp"

using eg::video_init;
using eg::video_quit;
// using eg::font_init;
// using eg::font_quit;
using eg::video_anim;

auto main(int, char *[]) -> int
{
    try
    {
        video_init();
        // font_init();

        auto animation = video_anim();
        animation.create_win("Blackhole", 1024, 768);
        animation.run();

        // font_quit();
        video_quit();

    } catch(const std::exception &e)

    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}

