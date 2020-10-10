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


namespace asplib
{
template<typename T>
class IBufferBase
{
public:
  IBufferBase(uint32_t MaxFrameLength, uint32_t MaxFrames, uint32_t Alinment=0)
  {
    if(MaxFrameLength <= 0 || MaxFrames <= 0)
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid buffer creation parameters! MaxFrameLength or MaxFrames <= 0");
    }

    m_Buffer = NULL;
    m_MaxFrameLength = MaxFrameLength;
    m_MaxFrames = MaxFrames;
    m_Alignment = Alinment;
  }

  virtual ~IBufferBase()
  {
  }

  uint32_t get_MaxElementBytes()
  {
    return m_MaxElementBytes;
  }

  uint32_t get_MaxFrameLength()
  {
    return m_MaxFrameLength;
  }

  uint32_t get_MaxFrames()
  {
    return m_MaxFrames;
  }

  uint32_t get_Alignment()
  {
    return m_Alignment;
  }

protected:
  virtual void ResetBuffer() = 0;
  virtual void Create() = 0;
  virtual void Destroy() = 0;

  T *m_Buffer;

private:
  uint32_t m_MaxElementBytes;
  uint32_t m_MaxFrameLength;
  uint32_t m_MaxFrames;
  uint32_t m_Alignment;
};
}
