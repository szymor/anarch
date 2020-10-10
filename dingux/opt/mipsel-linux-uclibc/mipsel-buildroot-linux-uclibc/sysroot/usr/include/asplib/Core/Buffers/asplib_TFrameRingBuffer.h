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

#include "Core/Buffers/asplib_TBaseFrameBuffer.h"


namespace asplib
{
template<typename T>
class TFrameRingBuffer : public TBaseFrameBuffer<T>
{
public:
  TFrameRingBuffer(uint32_t MaxFrameLength, uint32_t MaxFrames, uint32_t Alignment = 0) :
    TBaseFrameBuffer<T>(MaxFrameLength, MaxFrames, Alignment)
  {
    this->m_IsEmpty = true;
    this->m_CurrentFrame = this->get_MaxFrames();
  }

  inline T* get_Frame(uint32_t Frame)
  {
    if (Frame >= this->get_MaxFrames() || this->m_IsEmpty)
    {
      return NULL;
    }

    int32_t tempFrame = this->m_CurrentFrame - (int32_t)Frame;
    if(tempFrame < 0)
    {
      tempFrame = this->m_CurrentFrame + this->get_MaxFrames() - (int32_t)Frame;
    }

    return this->m_Buffer + tempFrame*this->get_MaxFrameLength();
  }

  inline T* get_NextFrame()
  {
    this->m_CurrentFrame++;
    if (this->m_CurrentFrame >= this->m_MaxFrames)
    {
      this->m_CurrentFrame = 0;
    }

    if (this->m_IsEmpty)
    {
      this->m_IsEmpty = false;
    }

    return this->m_Buffer + this->m_CurrentFrame*this->m_MaxFrameLength;
  }

private:
  bool      m_IsEmpty;
};
}
