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
 * FOR A PARTICprotectedULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with asplib (Achim's Signal Processing LIBrary).
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



#include "Core/os/asplib_os.h"

#include "Core/Exceptions/asplib_StringException.h"

#if defined(TARGET_WINDOWS) || defined(TARGET_LINUX)
  #include <xmmintrin.h>
#endif

namespace asplib
{
template<typename T>
class TBaseBuffer
{
public:
  TBaseBuffer(uint32_t MaxFrameLength, uint32_t MaxFrames, uint32_t Alignment=0)
  {
    if (MaxFrameLength <= 0 || MaxFrames <= 0)
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid buffer creation parameters! MaxFrameLength or MaxFrames <= 0");
    }

    this->m_Buffer = NULL;
    this->m_MaxFrameLength = MaxFrameLength;
    this->m_MaxFrames = MaxFrames;
    this->m_Alignment = Alignment;

    this->Create();
  }

  virtual ~TBaseBuffer()
  {
    this->Destroy();
  }

  uint32_t get_MaxElementBytes()
  {
    return this->m_MaxElementBytes;
  }

  uint32_t get_MaxFrameLength()
  {
    return this->m_MaxFrameLength;
  }

  uint32_t get_MaxFrames()
  {
    return this->m_MaxFrames;
  }

  uint32_t get_Alignment()
  {
    return this->m_Alignment;
  }

  inline void ResetBuffer()
  {
    memset(this->m_Buffer, 0, sizeof(T)*this->get_MaxFrameLength()*this->get_MaxFrames());
  }


private: // private member methods
  inline void Create()
  {
    if (this->get_Alignment() > 0)
    {
#if defined(TARGET_WINDOWS)
      m_Buffer = (T*)_aligned_malloc(sizeof(T)*this->get_MaxFrameLength()*this->get_MaxFrames(), (size_t)this->get_Alignment());
#elif defined(TARGET_LINUX)
      m_Buffer = (T*)_mm_malloc(sizeof(T)*this->get_MaxFrameLength()*this->get_MaxFrames(), (size_t)this->get_Alignment());
#else
      //#warning "Unsupported platform for memory alignment! Falling back to new."
      this->m_Buffer = new T[this->get_MaxFrameLength()*this->get_MaxFrames()];
#endif
    }
    else
    {
      this->m_Buffer = new T[this->get_MaxFrameLength()*this->get_MaxFrames()];
    }

    if (!this->m_Buffer)
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Couldn't create buffer! Not enough free dynamic memory?");
    }

    ResetBuffer();
  }

  inline void Destroy()
  {
    if (this->m_Buffer)
    {
      if (this->get_Alignment() > 0)
      {
  #if defined(TARGET_WINDOWS)
        _aligned_free(this->m_Buffer);
  #elif defined(TARGET_LINUX)
        _mm_free(this->m_Buffer);
  #else
        //#warning "Unsupported platform for memory alignment! Falling back to delete."
        delete [] this->m_Buffer;
  #endif
        }
        else
        {
          delete [] this->m_Buffer;
        }

      this->m_Buffer = NULL;
    }
  }


protected: // protected members
  T *m_Buffer;


private: // private members
  uint32_t m_MaxElementBytes;
  uint32_t m_MaxFrameLength;
  uint32_t m_MaxFrames;
  uint32_t m_Alignment;
};
}
