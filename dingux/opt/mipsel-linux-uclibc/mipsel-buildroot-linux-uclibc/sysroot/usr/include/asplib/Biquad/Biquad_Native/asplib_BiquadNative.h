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



// this module implements basic biquad filters 
// for more details see: http://en.wikipedia.org/wiki/Digital_biquad_filter

#include "Core/Constants_Typedefs/asplib_Constants.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"

#include "Biquad/Interfaces/asplib_IBiquad.h"

namespace asplib
{
class CBiquad_Native : public IBiquad<float>
{
public:
    // use this constructor to create a biquad filter with coefficients
    CBiquad_Native(uint32_t Amount, float SampleFrequency);
    //CBiquad_Native(float A0, float A1, float A2, float B1, float B2);
    ~CBiquad_Native();

    // Set all past values (y[k-1] & y[k-2]) to zero.
    virtual void resetState();

    virtual ASPLIB_ERR updateCoefficients(ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float D0);
    virtual ASPLIB_ERR updateCoefficients(ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float D0, uint32_t BiquadIdx);

    // calculate one output sample with the following difference equation
    // y[k] = a0*x[k] + a1*x[k-1] + a2*x[k-2] - (b1*y[k-1] + b2*y[k-2])
    virtual float calcSample(float In);
    // calculate output samples with the following difference equation
    // y[k] = a0*x[k] + a1*x[k-1] + a2*x[k-2] - (b1*y[k-1] + b2*y[k-2])
    virtual ASPLIB_ERR calcSamples(float *In, float *Out, uint32_t N);
};
};