#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include <libretro.h>
#include <retro_inline.h>

#ifndef HAVE_NO_LANGEXTRA
#include "libretro_core_options_intl.h"
#endif

#if defined(_3DS) && defined(HAVE_DYNAREC)
#include "3ds/3ds_utils.h"
#endif

/*
 ********************************
 * VERSION: 1.3
 ********************************
 *
 * - 1.3: Move translations to libretro_core_options_intl.h
 *        - libretro_core_options_intl.h includes BOM and utf-8
 *          fix for MSVC 2010-2013
 *        - Added HAVE_NO_LANGEXTRA flag to disable translations
 *          on platforms/compilers without BOM support
 * - 1.2: Use core options v1 interface when
 *        RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION is >= 1
 *        (previously required RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION == 1)
 * - 1.1: Support generation of core options v0 retro_core_option_value
 *        arrays containing options with a single value
 * - 1.0: First commit
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 ********************************
 * Core Option Definitions
 ********************************
*/

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

struct retro_core_option_definition option_defs_us[] = {
   {
      "gpsp_bios",
      "BIOS",
      "选择要使用的 BIOS 映像。官方 BIOS 必须由用户提供。使用非官方（或内置）BIOS 可能会导致与某些游戏不兼容的问题。最佳效果是通过官方任天堂 BIOS 实现。",
      {
         { "auto",      "自动选择" },
         { "builtin",   "内置BIOS" },
         { "official",  "原始BIOS" },
         { NULL, NULL },
      },
      "auto"
   },
   {
      "gpsp_boot_mode",
      "Boot模式",
      "选择在游戏前启动BIOS与否。这两种模式下没有太大的区别。",
      {
         { "game",      "启动游戏" },
         { "bios",      "启动BIOS" },
         { NULL, NULL },
      },
      "game"
   },
#if defined(HAVE_DYNAREC)
   {
      "gpsp_drc",
      "动态重新编译",
      "动态重新编译CPU指令到本地指令。大幅提升性能，但可能会降低准确性。",
      {
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "enabled"
   },
#endif
   {
      "gpsp_sprlim",
      "移除精灵限制",
      "删除原始每扫描行一个精灵的硬件限制。某些游戏可能依赖于硬件限制以正确渲染（例如隐藏某些精灵）。",
      {
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "gpsp_rtc",
      "RTC支持",
      "设置仿真卡带的 RTC 支持。自动检测使用与大多数商业标题兼容的 ROM 数据库。在使用自制软件或 ROM 补丁时，可能需要强制启用 RTC。",
      {
         { "auto",     "自动" },
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "auto"
   },
   {
      "gpsp_serial",
      "连接线缆",
      "配置串行（连接线）连接。自动支持大多数已知游戏。",
      {
         { "auto",     "自动" },
         { "disabled", "关闭" },
         { "rfu",      "GBA 无线适配器"},
         { NULL, NULL },
      },
      "auto"
   },
   {
      "gpsp_rumble",
      "震动支持",
      "设置模拟卡带的振动支持。自动检测使用与大多数商业标题兼容的 ROM 数据库。您可能希望在使用支持振动的自制软件或 ROM 修改时强制启用振动。如果您不喜欢，还可以强制禁用它。",
      {
         { "auto", "自动" },
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "auto"
   },
   {
      "gpsp_frameskip",
      "跳帧",
      "跳过帧以避免音频缓冲区运行不足（爆裂声）。以牺牲视觉平滑度为代价提高性能。当前端建议时，'自动'会跳过帧。 '自动 (阈值)'利用'跳帧阈值 (%)'设置。 '固定间隔'利用'跳帧间隔'设置。",
      {
         { "disabled",       "关闭" },
         { "auto",           "自动" },
         { "auto_threshold", "自动 (阈值)" },
         { "fixed_interval", "固定间隔" },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "gpsp_frameskip_threshold",
      "跳帧阈值(%)",
      "当将'跳帧'设置为'自动 (阈值)'时，指定音频缓冲区占用阈值的百分比，低于该值将跳过帧。值越高，通过更频繁地丢弃帧来降低刺耳的可能性，从而降低爆裂风险。",
      {
         { "15", NULL },
         { "18", NULL },
         { "21", NULL },
         { "24", NULL },
         { "27", NULL },
         { "30", NULL },
         { "33", NULL },
         { "36", NULL },
         { "39", NULL },
         { "42", NULL },
         { "45", NULL },
         { "48", NULL },
         { "51", NULL },
         { "54", NULL },
         { "57", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "33"
   },
   {
      "gpsp_frameskip_interval",
      "跳帧间隔",
      "当将'跳帧'设置为'固定间隔'时，此处设置的值是在渲染一个帧后省略的帧数 - 即'0'表示 60fps，'1'表示 30fps，'2'表示 15fps 等。",
      {
         { "0",  NULL },
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "4",  NULL },
         { "5",  NULL },
         { "6",  NULL },
         { "7",  NULL },
         { "8",  NULL },
         { "9",  NULL },
         { "10", NULL },
         { NULL, NULL },
      },
      "1"
   },
   {
      "gpsp_color_correction",
      "色彩校正",
      "调整输出颜色以匹配真实 GBA 硬件的显示。",
      {
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "gpsp_frame_mixing",
      "帧间混合",
      "模拟 LCD 重影效果，通过执行当前帧与上一帧的 50:50 混合。在播放利用 LCD 重影实现透明效果的游戏（如 F-Zero、Boktai 系列等）时，正确运行所需的条件。",
      {
         { "enabled",  "开启" },
         { "disabled", "关闭" },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "gpsp_turbo_period",
      "连发间隔",
      "指定按下 Turbo A/B 按钮时的重复间隔（以帧为单位）。",
      {
         { "1",   NULL },
         { "2",   NULL },
         { "3",   NULL },
         { "4",   NULL },
         { "5",   NULL },
         { "6",   NULL },
         { "7",   NULL },
         { "8",   NULL },
         { "9",   NULL },
         { "10",  NULL },
         { "11",  NULL },
         { "12",  NULL },
         { "13",  NULL },
         { "14",  NULL },
         { "15",  NULL },
         { "16",  NULL },
         { "17",  NULL },
         { "18",  NULL },
         { "19",  NULL },
         { "20",  NULL },
         { "21",  NULL },
         { "22",  NULL },
         { "23",  NULL },
         { "24",  NULL },
         { "25",  NULL },
         { "26",  NULL },
         { "27",  NULL },
         { "28",  NULL },
         { "29",  NULL },
         { "30",  NULL },
         { "31",  NULL },
         { "32",  NULL },
         { "33",  NULL },
         { "34",  NULL },
         { "35",  NULL },
         { "36",  NULL },
         { "37",  NULL },
         { "38",  NULL },
         { "39",  NULL },
         { "40",  NULL },
         { "41",  NULL },
         { "42",  NULL },
         { "43",  NULL },
         { "44",  NULL },
         { "45",  NULL },
         { "46",  NULL },
         { "47",  NULL },
         { "48",  NULL },
         { "49",  NULL },
         { "50",  NULL },
         { "51",  NULL },
         { "52",  NULL },
         { "53",  NULL },
         { "54",  NULL },
         { "55",  NULL },
         { "56",  NULL },
         { "57",  NULL },
         { "58",  NULL },
         { "59",  NULL },
         { "60",  NULL },
         { "61",  NULL },
         { "62",  NULL },
         { "63",  NULL },
         { "64",  NULL },
         { "65",  NULL },
         { "66",  NULL },
         { "67",  NULL },
         { "68",  NULL },
         { "69",  NULL },
         { "70",  NULL },
         { "71",  NULL },
         { "72",  NULL },
         { "73",  NULL },
         { "74",  NULL },
         { "75",  NULL },
         { "76",  NULL },
         { "77",  NULL },
         { "78",  NULL },
         { "79",  NULL },
         { "80",  NULL },
         { "81",  NULL },
         { "82",  NULL },
         { "83",  NULL },
         { "84",  NULL },
         { "85",  NULL },
         { "86",  NULL },
         { "87",  NULL },
         { "88",  NULL },
         { "89",  NULL },
         { "90",  NULL },
         { "91",  NULL },
         { "92",  NULL },
         { "93",  NULL },
         { "94",  NULL },
         { "95",  NULL },
         { "96",  NULL },
         { "97",  NULL },
         { "98",  NULL },
         { "99",  NULL },
         { "100", NULL },
         { "101", NULL },
         { "102", NULL },
         { "103", NULL },
         { "104", NULL },
         { "105", NULL },
         { "106", NULL },
         { "107", NULL },
         { "108", NULL },
         { "109", NULL },
         { "110", NULL },
         { "111", NULL },
         { "112", NULL },
         { "113", NULL },
         { "114", NULL },
         { "115", NULL },
         { "116", NULL },
         { "117", NULL },
         { "118", NULL },
         { "119", NULL },
         { "120", NULL },
         { NULL, NULL },
      },
      "4"
   },
   { NULL, NULL, NULL, {{0}}, NULL },
};

/*
 ********************************
 * Language Mapping
 ********************************
*/

#ifndef HAVE_NO_LANGEXTRA
struct retro_core_option_definition *option_defs_intl[RETRO_LANGUAGE_LAST] = {
   option_defs_us, /* RETRO_LANGUAGE_ENGLISH */
   NULL,           /* RETRO_LANGUAGE_JAPANESE */
   NULL,           /* RETRO_LANGUAGE_FRENCH */
   NULL,           /* RETRO_LANGUAGE_SPANISH */
   NULL,           /* RETRO_LANGUAGE_GERMAN */
   NULL,           /* RETRO_LANGUAGE_ITALIAN */
   NULL,           /* RETRO_LANGUAGE_DUTCH */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   NULL,           /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   NULL,           /* RETRO_LANGUAGE_RUSSIAN */
   NULL,           /* RETRO_LANGUAGE_KOREAN */
   NULL,           /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   NULL,           /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   NULL,           /* RETRO_LANGUAGE_ESPERANTO */
   NULL,           /* RETRO_LANGUAGE_POLISH */
   NULL,           /* RETRO_LANGUAGE_VIETNAMESE */
   NULL,           /* RETRO_LANGUAGE_ARABIC */
   NULL,           /* RETRO_LANGUAGE_GREEK */
   NULL,           /* RETRO_LANGUAGE_TURKISH */
   NULL,           /* RETRO_LANGUAGE_SLOVAK */
   NULL,           /* RETRO_LANGUAGE_PERSIAN */
   NULL,           /* RETRO_LANGUAGE_HEBREW */
   NULL,           /* RETRO_LANGUAGE_ASTURIAN */
};
#endif

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should be called as early as possible - ideally inside
 * retro_set_environment(), and no later than retro_load_game()
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb)
{
   unsigned version = 0;

   if (!environ_cb)
      return;

#if defined(_3DS) && (HAVE_DYNAREC)
   if(!__ctr_svchax)
   {
      /* Critical error - dynarec is force
       * disabled, so remove 'gpsp_drc' option */
      option_defs_us[2].key           = NULL;
      option_defs_us[2].desc          = NULL;
      option_defs_us[2].info          = NULL;
      option_defs_us[2].default_value = NULL;
   }
#endif

   if (environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version) && (version >= 1))
   {
#ifndef HAVE_NO_LANGEXTRA
      struct retro_core_options_intl core_options_intl;
      unsigned language = 0;

      core_options_intl.us    = option_defs_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = option_defs_intl[language];

      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_intl);
#else
      environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, &option_defs_us);
#endif
   }
   else
   {
      size_t i;
      size_t num_options               = 0;
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine number of options */
      for (;;)
      {
         if (!option_defs_us[num_options].key)
            break;
         num_options++;
      }

      /* Allocate arrays */
      variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
      values_buf = (char **)calloc(num_options, sizeof(char *));

      if (!variables || !values_buf)
         goto error;

      /* Copy parameters from option_defs_us array */
      for (i = 0; i < num_options; i++)
      {
         const char *key                        = option_defs_us[i].key;
         const char *desc                       = option_defs_us[i].desc;
         const char *default_value              = option_defs_us[i].default_value;
         struct retro_core_option_value *values = option_defs_us[i].values;
         size_t buf_len                         = 3;
         size_t default_index                   = 0;

         values_buf[i] = NULL;

         if (desc)
         {
            size_t num_values = 0;

            /* Determine number of values */
            for (;;)
            {
               if (!values[num_values].value)
                  break;

               /* Check if this is the default value */
               if (default_value)
                  if (strcmp(values[num_values].value, default_value) == 0)
                     default_index = num_values;

               buf_len += strlen(values[num_values].value);
               num_values++;
            }

            /* Build values string */
            if (num_values > 0)
            {
               size_t j;

               buf_len += num_values - 1;
               buf_len += strlen(desc);

               values_buf[i] = (char *)calloc(buf_len, sizeof(char));
               if (!values_buf[i])
                  goto error;

               strcpy(values_buf[i], desc);
               strcat(values_buf[i], "; ");

               /* Default value goes first */
               strcat(values_buf[i], values[default_index].value);

               /* Add remaining values */
               for (j = 0; j < num_values; j++)
               {
                  if (j != default_index)
                  {
                     strcat(values_buf[i], "|");
                     strcat(values_buf[i], values[j].value);
                  }
               }
            }
         }

         variables[i].key   = key;
         variables[i].value = values_buf[i];
      }

      /* Set variables */
      environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

error:

      /* Clean up */
      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif
