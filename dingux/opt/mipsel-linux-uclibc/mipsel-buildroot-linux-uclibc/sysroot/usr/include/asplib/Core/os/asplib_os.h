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



// --- TARGET_WINDOWS ---------------------------------------------------------
#if defined(TARGET_WINDOWS) || defined(MSVC) || defined(_WIN32) || defined(_WIN64)
  #if !defined(TARGET_WINDOWS)
    #define TARGET_WINDOWS
  #endif
  #include "Core/os/windows/windows_definitions.h"
// --- TARGET_LINUX -----------------------------------------------------------
#elif defined(TARGET_LINUX) || defined(__gnu_linux__) || defined(__linux__)
  #if !defined(TARGET_LINUX)
    #define TARGET_LINUX
  #endif
  #include "Core/os/linux/linux_definitions.h"
// --- TARGET_ANDROID ---------------------------------------------------------
#elif defined(TARGET_ANDROID)       // TODO: add platform compiler flags
  #if !defined(TARGET_ANDROID)
    #define TARGET_ANDROID
  #endif
  #include "Core/os/android/android_definitions.h"
// --- TARGET_FREEBSD ---------------------------------------------------------
#elif defined(TARGET_FREEBSD)       // TODO: add platform compiler flags
  #if !defined(TARGET_FREEBSD)
    #define TARGET_FREEBSD
  #endif
  #include "Core/os/freebsd/freebsd_definitions.h"
// --- TARGET_DARWIN_OSX ------------------------------------------------------
#elif defined(TARGET_DARWIN_OSX)    // TODO: add platform compiler flags  
  #if !defined(TARGET_DARWIN_OSX)
    #define TARGET_DARWIN_OSX
  #endif
  #include "Core/os/darwin_osx/darwin_osx_definitions.h"
// --- TARGET_DARWIN_IOS ------------------------------------------------------
#elif defined(TARGET_DARWIN_IOS)    // TODO: add platform compiler flags
  #if !defined(TARGET_DARWIN_IOS)
    #define TARGET_DARWIN_IOS
  #endif
  #include "Core/os/darwin_ios/darwin_ios_definitions.h"
// --- TARGET_RASPBERRY_PI ----------------------------------------------------
#elif defined(TARGET_RASPBERRY_PI)  // TODO: add platform compiler flags
  #if !defined(TARGET_RASPBERRY_PI)
    #define TARGET_RASPBERRY_PI
  #endif
  #include "Core/os/raspberry_pi/raspberry_pi_definitions.h"
#else
  #error "Unsupported operating system!"
#endif
