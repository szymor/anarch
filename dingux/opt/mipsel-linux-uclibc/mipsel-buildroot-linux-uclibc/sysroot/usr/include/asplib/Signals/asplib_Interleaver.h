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

template<typename T>
bool fmtc_NonInterleaved_TO_Interleaved(T *InSamples, T *OutSamples, uint16_t MaxChannels, uint32_t MaxSamples)
{
  if(!InSamples || !OutSamples || !MaxSamples || !MaxChannels)
  {
    return false;
  }

  for(uint16_t ch=0; ch < MaxChannels; ch++)
  {
    for(uint32_t ii = 0; ii < MaxSamples; ii++)
    {
      OutSamples[ii*MaxChannels + ch] = InSamples[ch*MaxSamples + ii];
    }
  }

  return true;
}

template<typename T>
bool fmtc_Interleaved_TO_NonInterleaved(T *InSamples, T *OutSamples, uint16_t MaxChannels, uint32_t MaxSamples)
{
  if(!InSamples || !OutSamples || !MaxSamples || !MaxChannels)
  {
    return false;
  }

  for(uint32_t ii = 0; ii < MaxSamples; ii++)
  {
    for(uint16_t ch=0; ch < MaxChannels; ch++)
    {
      OutSamples[ch*MaxSamples + ii] = InSamples[ii*MaxChannels + ch];
    }
  }

  return true;
}
