#pragma once

/* Copyright (C) 2014-2015 Achim Turan, Achim.Turan@o2online.de
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

#include "Core/Exceptions/asplib_StringException.h"

#include "Core/Buffers/asplib_TBufferBase.h"


namespace asplib
{
template<typename T>
class ITBuffer : public TBufferBase<T>
{
public:
  ITBuffer(uint32_t MaxFrameLength, uint32_t MaxFrames, uint32_t Alignment=0) :
    TBufferBase<T>(MaxFrameLength, MaxFrames, Alignment)
  {
  }

  virtual uint32_t read(T *Data, uint32_t SamplesToRead) = 0;
  virtual uint32_t write(T *Data, uint32_t SamplesToWrite) = 0;
};
}
