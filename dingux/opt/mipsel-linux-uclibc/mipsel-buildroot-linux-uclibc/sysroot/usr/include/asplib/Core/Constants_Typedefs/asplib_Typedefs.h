#pragma once

/*
 * Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
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
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */



#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_ExtendedStructIDs.hpp"


// error definitions
typedef enum
{
  // asplib fft error codes
  // 199 ... -130
  ASPLIB_ERR_FFT_FMT_CONVERTER_CONFIGURE_FAILED = -133,
  ASPLIB_ERR_FFT_CONFIGURE_FAILED               = -132,
  ASPLIB_ERR_FFT_INVALID_INTERNAL_FRAME_SIZE    = -131,
  ASPLIB_ERR_FFT_INVALID_OPTIONS_STRUCT         = -130,
  
  // TAutoFactory error codes
  // -129 ... -100
  ASPLIB_ERR_UNKNOWN_FACTORY_PRODUCT = -100,

  // General asplib error codes 
  // -99 ... -1
  ASPLIB_ERR_INVALID_INPUT    = -3,
  ASPLIB_ERR_UNKNOWN_ID       = -2,
  ASPLIB_ERR_NOT_IMPLEMENTED  = -1,

  ASPLIB_ERR_NO_ERROR = 0

  // asplib warning codes
}ASPLIB_ERR;


typedef enum 
{
  ASPLIB_OPT_MIN = -1,
  ASPLIB_OPT_NATIVE,
  //reserved for future versions
  ASPLIB_OPT_SSE,
  ASPLIB_OPT_SSE2,
  ASPLIB_OPT_SSE3,
  ASPLIB_OPT_SSE4,
  ASPLIB_OPT_AVX,
  ASPLIB_OPT_AVX2,
  ASPLIB_OPT_CUDA,
  ASPLIB_OPT_OPENCL,
  ASPLIB_OPT_ARM_VFP,
  ASLPIB_OPT_NEON,
  ASPLIB_OPT_MAX
}ASPLIB_OPT_MODULE;

typedef struct
{
  ASPLIB_OPT_MODULE  optModule;
  void               *Biquads;
}ASPLIB_BIQUAD_HANDLE;

typedef struct
{
  float a0;
  float a1;
  float a2;
  float b1;
  float b2;
}ASPLIB_BIQUAD_COEFFICIENTS;

typedef struct
{
  float fc;
  float fs;
  float Q;
  float Gain;
}ASPLIB_CONST_Q_PEAKING_PARAM;
