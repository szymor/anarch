#pragma once

/*
 * Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
 * https://github.com/AchimTuran/asplib
 *
 * This file is part of asplib (Achim's Signal Processing LIBrary)
 *
 * asplib (Achim's Signal Processing LIBrary) is free software:
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * asplib (Achim's Signal Processing LIBrary) is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#if defined(TARGET_ANDROID)
  #ifndef __ASPLIB_FUNCTION__
    #define __ASPLIB_FUNCTION__ __func__
  #endif
  #ifndef __ASPLIB_FILE__
    #define __ASPLIB_FILE__ __FILE__
  #endif
  #ifndef __ASPLIB_LINE__
    #define __ASPLIB_LINE__ __LINE__
  #endif

  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>
  #include <math.h>

  // data typedefs
  //  typedef unsigned char   uint8_t;
  //  typedef char            int8_t;
  //  typedef unsigned short  uint16_t;
  //  typedef short           int16_t;
  //  typedef unsigned int    uint32_t;
  //  typedef int             int32_t;
  //  typedef unsigned long   uin64_t;
  //  typedef long            int64_t;

  // path typedefs
  #define ASPLIB_PATH_SEPERATOR '/'
#else
  #error "Configuring TARGET_ANDROID failed!"
#endif
