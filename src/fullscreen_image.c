//
// Created by Eric Fry on 22/12/2017.
//

#include <assert.h>
#include <files/file.h>
#include <sound/music.h>
#include "fullscreen_image.h"
#include "video.h"
#include "game.h"

static const char filename_tbl[][13] = {
        "PRETITLE.MNI",
        "TITLE1.MNI",
        "CREDIT.MNI",
        "BONUS.MNI",
        "END1.MNI",
        "ONEMOMNT.MNI"
};

#define NUM_PIXELS (320 * 200)

uint8 *load_image(const char *filename)
{
    uint8 *pixels = (uint8 *)malloc(320*200);

    File image;
    open_file(filename, &image);

    for(int c=0;c<4;c++) {

        for (int i = 0; i < NUM_PIXELS / 8; i++) {
            uint8 data = file_read1(&image);
            for (int j = 0; j < 8; j++) {
                if(c==0)
                {
                    pixels[i * 8 + j] = (data >> (7 - j)) & 1;
                }
                else
                {
                    pixels[i * 8 + j] |= ((data >> (7 - j)) & 1) << c;
                }
            }
        }
    }
    file_close(&image);

    return pixels;
}

void display_fullscreen_image(uint8 image_number)
{
    assert(image_number < 6);

    if(image_number != 1 && image_number != 2)
    {
        stop_music();
    }

    fade_to_black_speed_3();

    uint8 *image = load_image(filename_tbl[image_number]);
    video_draw_fullscreen_image(image);
    free(image);

    fade_in_from_black_with_delay_3();

}