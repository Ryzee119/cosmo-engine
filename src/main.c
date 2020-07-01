/*
 *  Cosmo-Engine
 *  Copyright Eric Fry (c) 2017. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <SDL.h>
#include "sound/audio.h"
#include "sound/music.h"
#include "game.h"
#include "map.h"
#include "dialog.h"
#include "video.h"
#include "status.h"
#include "config.h"
#include "high_scores.h"
#include "demo.h"
#include "b800.h"
#include "input.h"

#ifdef NXDK
#include <windows.h>
#include <nxdk/mount.h>
#include <hal/video.h>
#include <hal/debug.h>
#endif

int cleanup_and_exit();

int main(int argc, char *argv[]) {
    #ifdef NXDK
    /* Initialise Xbox video */
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    /* Setup save directories for Xbox */
    nxMountDrive('E', "\\Device\\Harddisk0\\Partition1\\");
    CreateDirectoryA("E:\\UDATA", NULL);
    CreateDirectoryA("E:\\UDATA\\Cosmo", NULL);
    CreateDirectoryA("E:\\UDATA\\Cosmo\\CosmoData", NULL);

    FILE* fp;
    fp = fopen("E:\\UDATA\\Cosmo\\TitleMeta.xbx", "wb");
    fprintf(fp, "TitleName=Cosmos Cosmic Adventure\r\n");
    fclose(fp);
    fp = fopen("E:\\UDATA\\Cosmo\\CosmoData\\SaveMeta.xbx", "wb");
    fprintf(fp, "Name=Cosmo Data\r\n");
    fclose(fp);

    /* Let's make a quick and simple menu to select Episode */
    char episode[] = "-ep1";
    SDL_Init(SDL_INIT_GAMECONTROLLER);
    SDL_GameController *gamepad = SDL_GameControllerOpen(0);
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (gamepad == NULL)
    {
        debugPrint("No compatible controller connected.\n\n");
        debugPrint("Restart Xbox and try again\n\n");
        while (1);
    }
    debugPrint("Cosmo's Cosmic Adventure\n\n");
    debugPrint("Press A to Start Episode 1\n\n");
    debugPrint("Press B to Start Episode 2\n\n");
    debugPrint("Press X to Start Episode 3\n\n");
    debugPrint("https://github.com/Ryzee119/cosmo-engine\n\n");
    while (1)
    {
        SDL_GameControllerUpdate();
        if (SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_A))
        {
            strcpy(episode,"-ep1");
            break;
        }
        if (SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_B))
        {
            strcpy(episode,"-ep2");
            break;
        }
        if (SDL_GameControllerGetButton(gamepad, SDL_CONTROLLER_BUTTON_X))
        {
            strcpy(episode,"-ep3");
            break;
        }
        Sleep(50);
    }
    debugClearScreen();
    SDL_GameControllerClose(0);
    SDL_Quit();

    /* Inject args */
    argc = 9;
    argv = malloc(sizeof(char *) * argc);
    for (int i = 0; i < argc; i++)
        argv[i] = malloc(256);
    strcpy(argv[1], episode);
    strcpy(argv[2], "-savedir"); strcpy(argv[3], "E:\\UDATA\\Cosmo\\CosmoData");
    strcpy(argv[4], "-gamedir"); strcpy(argv[5], "D:\\game_data");
    strcpy(argv[6], "-datadir"); strcpy(argv[7], "D:\\game_data");
    strcpy(argv[8], "-q");
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    #endif

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("argh!!");
    }

    load_config_from_command_line(argc, argv);

    #ifdef NXDK
    for (int i = 0; i < argc; i ++)
        free(argv[i]);
    free(argv);
    #endif

    video_init();
    audio_init();
    input_init();
    game_init();

    video_fill_screen_with_black();

    if(!is_quick_start())
    {
        a_game_by_dialog();
        game_play_mode = main_menu();
    }
    else
    {
        set_initial_game_state();
        game_play_mode = PLAY_GAME;
    }

    while(game_play_mode != QUIT_GAME)
    {
        load_level(current_level);

        if(game_play_mode == PLAY_DEMO)
        {
            load_demo();
        }

        game_loop();
        stop_music();
        if(game_play_mode == PLAY_GAME)
        {
            show_high_scores();
        }
        game_play_mode = main_menu();
    }

    stop_music();
    display_exit_text();

    return cleanup_and_exit();
}

int cleanup_and_exit()
{
    write_config_file();
    config_cleanup();
    video_shutdown();
    audio_shutdown();
    input_shutdown();
    SDL_Quit();

    return 0;
}
