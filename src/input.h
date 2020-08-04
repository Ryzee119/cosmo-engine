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

#ifndef COSMO_ENGINE_INPUT_H
#define COSMO_ENGINE_INPUT_H

#include <SDL_keycode.h>
#include "defines.h"

typedef enum
{
    QUIT,
    PAUSED,
    CONTINUE
} input_state_enum;

typedef enum
{
    CMD_KEY_UP,
    CMD_KEY_DOWN,
    CMD_KEY_LEFT,
    CMD_KEY_RIGHT,
    CMD_KEY_JUMP,
    CMD_KEY_BOMB,
    CMD_KEY_MENU,
    CMD_KEY_OTHER
} InputCommand;

typedef enum {
    FAST_FORWARD,
    EXIT,
    NO_INPUT
} HintDialogInput;

extern uint8 bomb_key_pressed;
extern uint8 jump_key_pressed;
extern uint8 up_key_pressed;
extern uint8 down_key_pressed;
extern uint8 left_key_pressed;
extern uint8 right_key_pressed;

extern uint8 byte_2E17C;

bool input_init();
void input_shutdown();

void wait_for_time_or_key(int delay_in_game_cycles);
input_state_enum read_input();
void reset_player_control_inputs();
SDL_Keycode poll_for_key_press(bool allow_key_repeat);
void cosmo_wait(int delay);

void set_input_command_key(InputCommand command, SDL_Keycode keycode);
SDL_Keycode get_input_command_key(InputCommand command);
const char *get_command_key_string(InputCommand command);

void flush_input();

bool is_return_key(SDL_Keycode key);

HintDialogInput hint_dialog_get_input(HintDialogInput input);

#endif //COSMO_ENGINE_INPUT_H
