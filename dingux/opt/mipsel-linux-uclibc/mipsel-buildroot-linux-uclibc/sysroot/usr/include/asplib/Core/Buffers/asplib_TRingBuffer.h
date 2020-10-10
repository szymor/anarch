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

#include "Core/Buffers/asplib_TBaseBuffer.h"


namespace asplib
{
template<typename T>
class TRingBuffer : public TBaseBuffer<T>
{
public:
  TRingBuffer(uint32_t MaxSize, uint32_t Delay=0, uint32_t Alignment=0) :
    TBaseBuffer<T>(MaxSize, 1, Alignment = 0)
  {
    if(Delay >= this->get_MaxSize())
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid input! Delay >= MaxSize!");
    }
    this->m_WriteIdx = 0;
    this->m_CurrentStoredSamples = 0;
    this->m_Delay = Delay;
    if(this->m_Delay > 0)
    {
      this->m_ReadIdx = this->get_MaxSize() - this->m_Delay;
    }
    else
    {
      this->m_ReadIdx = 0;
    }

    // Read & Write pointers
    this->m_WritePtr = NULL;
    this->m_ReadPtr = NULL;
  }

  // reads SamplesToRead samples from internal buffer and stores it in OutData
  uint32_t read(T *Data, uint32_t SamplesToRead)
  {
    if(!Data)
    {
      return 0;
    }

    int32_t samplesRead = 0;
    if (SamplesToRead <= this->m_CurrentStoredSamples)
    {
      samplesRead = SamplesToRead;
    }
    else
    {
      samplesRead = this->m_CurrentStoredSamples;
    }

    int32_t samplesToBoundary = (int32_t)this->get_MaxSize() - this->m_ReadIdx - samplesRead;
    if(samplesToBoundary >= 0)
    { // read data from m_Buffer without crossing the boundary boerder
      memcpy(Data, this->m_Buffer + this->m_ReadIdx, sizeof(T)*samplesRead);
      if(samplesToBoundary != 0)
      {
        this->m_ReadIdx += samplesRead;
      }
      else
      { // set index to buffer start
        this->m_ReadIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = samplesRead + samplesToBoundary;
      memcpy(Data, this->m_Buffer + this->m_ReadIdx, sizeof(T)*copySamples);

      // store the rest of the samples
      int32_t leftSamples = samplesRead - copySamples;
      memcpy(Data + copySamples, this->m_Buffer, sizeof(T)*leftSamples);
      this->m_ReadIdx = leftSamples;
    }

    // read samples from ring buffer
    this->m_CurrentStoredSamples -= samplesRead;

    return samplesRead;
  }

  // writes SamplesToWrite from to InData the internal buffer
  uint32_t write(T *InData, uint32_t SamplesToWrite)
  {
    if(!InData)
    {
      throw ASPLIB_STRING_EXCEPTION_HANDLER("Invalid Pointer or SamplesToWrite < 0!");
    }

    int32_t samplesWrite = 0;
    // calculate samples that can be written without crossing m_Buffer boundary
    int32_t freeSamples = (int32_t)this->get_MaxSize() - this->m_CurrentStoredSamples;
    if((int32_t)SamplesToWrite <= freeSamples)
    {
      samplesWrite = SamplesToWrite;
    }
    else
    {
      samplesWrite = freeSamples;
    }

    int32_t samplesToBoundary = (int32_t)this->get_MaxSize() - this->m_WriteIdx - samplesWrite;
    if(samplesToBoundary >= 0)
    { // write data without crossing the buffer boundary
      memcpy(this->m_Buffer + this->m_WriteIdx, InData, sizeof(T)*samplesWrite);
      if(samplesToBoundary != 0)
      {
        this->m_WriteIdx += samplesWrite;
      }
      else
      { // set m_WriteIdx to buffer start
        this->m_WriteIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = samplesWrite + samplesToBoundary;
      memcpy(this->m_Buffer + this->m_WriteIdx, InData, sizeof(T)*copySamples);

      // store the rest of the samples
      int32_t leftSamples = samplesWrite - copySamples;
      memcpy(this->m_Buffer, InData + copySamples, sizeof(T)*leftSamples);
      this->m_WriteIdx = leftSamples;
    }

    // write samples to ring buffer
    this->m_CurrentStoredSamples += samplesWrite;

    return samplesWrite;
  }

  // Set pointers to start of m_Buffer
  void reset_Pointers()
  {
    this->m_WriteIdx = 0;
    this->m_ReadIdx = 0;

    this->m_WritePtr  = this->m_Buffer;
    this->m_ReadPtr   = this->m_Buffer;

    this->m_CurrentStoredSamples = 0;
  }

  int32_t write_Zeros(uint32_t Zeros)
  {
    int zerosWrite = 0;
    // calculate samples m_Buffer boundary
    int freeSamples = this->m_MaxSize - this->m_CurrentStoredSamples;
    if(Zeros <= freeSamples)
    {
      zerosWrite = Zeros;
    }
    else
    {
      zerosWrite = freeSamples;
    }

    int32_t samplesToBoundary = (int32_t)this->get_MaxSize() - this->m_WriteIdx - zerosWrite;
    if(samplesToBoundary >= 0)
    { // write 0 without crossing boundary
      memset(this->m_WritePtr, 0, sizeof(T)*zerosWrite);
      if(samplesToBoundary != 0)
      {
        m_WriteIdx += zerosWrite;
      }
      else
      {
        this->m_WriteIdx = 0;
      }
    }
    else
    {
      // samplesToBoundary < 0 --> add operation results to subtraction
      int32_t copySamples = zerosWrite + samplesToBoundary;
      memcpy(this->m_WritePtr, 0, sizeof(T)*copySamples);

      // write zeros to m_WriteIdx
      this->m_WriteIdx = zerosWrite - copySamples;
      memset(this->m_WritePtr + copySamples, 0, sizeof(T)*this->m_WriteIdx);
    }

    this->m_CurrentStoredSamples += zerosWrite;
    this->m_WritePtr = this->m_Buffer + this->m_WriteIdx;

    return zerosWrite;
  }

  // get maximum ring buffer size
  inline uint32_t get_MaxSize()    { return this->get_MaxFrameLength(); }


  // returns the current free space of the ring buffer
  inline uint32_t get_FreeSamples()   { return this->get_MaxSize() - this->m_CurrentStoredSamples; }
  // get current stored samples
  inline uint32_t get_StoredSamples() { return this->m_CurrentStoredSamples; }

private:
  volatile uint32_t m_CurrentStoredSamples;
  volatile uint32_t m_WriteIdx;
  volatile uint32_t m_ReadIdx;

  volatile T *m_WritePtr;
  volatile T *m_ReadPtr;
  uint32_t    m_Delay;
};
}
