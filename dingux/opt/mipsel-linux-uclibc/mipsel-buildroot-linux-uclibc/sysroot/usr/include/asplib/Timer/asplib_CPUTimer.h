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



#include "Core/os/asplib_os.h"
#if defined(TARGET_WINDOWS)
  #include <windows.h>
  #include <time.h>
#elif defined(TARGET_LINUX)
  #include <time.h>
#endif

namespace asplib
{
class CCPUTimer
{
public:
  CCPUTimer();
  ~CCPUTimer();

  void start_Timer();
  void stop_Timer();
  bool is_TimerRunning();
  void pause_Timer();
  double get_ElapsedTime();

  void print_ElapsedTime();

private:
#if defined(TARGET_WINDOWS)
  LARGE_INTEGER m_frequency;
  LARGE_INTEGER m_t1;
  LARGE_INTEGER m_t2;
  double m_elapsedTime;
#else
  timespec m_startTime;
  timespec m_endTime;
#endif

  volatile bool m_activeTimer;
  bool m_completeMeasure;
};
}
