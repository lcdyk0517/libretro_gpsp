/* gameplaySP
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "common.h"

bool libretro_supports_bitmasks    = false;
bool libretro_supports_ff_override = false;
bool libretro_ff_enabled           = false;
bool libretro_ff_enabled_prev      = false;

static u32 old_key = 0;
static retro_input_state_t input_state_cb;

void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

extern void set_fastforward_override(bool fastforward);

static void trigger_key(u32 key)
{
  u32 p1_cnt = io_registers[REG_P1CNT];

  if((p1_cnt >> 14) & 0x01)
  {
    u32 key_intersection = (p1_cnt & key) & 0x3FF;

    if(p1_cnt >> 15)
    {
      if(key_intersection == (p1_cnt & 0x3FF))
        raise_interrupt(IRQ_KEYPAD);
    }
    else
    {
      if(key_intersection)
        raise_interrupt(IRQ_KEYPAD);
    }
  }
}

u32 update_input(void)
{
   unsigned i;
   uint32_t new_key = 0;

   if (!input_state_cb)
      return 0;

   if (libretro_supports_bitmasks)
   {
      int16_t ret = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);

      for (i = 0; i < sizeof(btn_map) / sizeof(map); i++)
         new_key |= (ret & (1 << btn_map[i].retropad)) ? btn_map[i].gba : 0;

      libretro_ff_enabled = libretro_supports_ff_override &&
            (ret & (1 << RETRO_DEVICE_ID_JOYPAD_R2));
   }
   else
   {
      for (i = 0; i < sizeof(btn_map) / sizeof(map); i++)
         new_key |= input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, btn_map[i].retropad) ? btn_map[i].gba : 0;

       libretro_ff_enabled = libretro_supports_ff_override &&
            input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2);
   }

   if ((new_key | old_key) != old_key)
      trigger_key(new_key);

   old_key = new_key;
   io_registers[REG_P1] = (~old_key) & 0x3FF;

   /* Handle fast forward button */
   if (libretro_ff_enabled != libretro_ff_enabled_prev)
   {
      set_fastforward_override(libretro_ff_enabled);
      libretro_ff_enabled_prev = libretro_ff_enabled;
   }

   return 0;
}

#define input_savestate_builder(type)   \
void input_##type##_savestate(void)     \
{                                       \
  state_mem_##type##_variable(old_key); \
}

input_savestate_builder(read)
input_savestate_builder(write)
