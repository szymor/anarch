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
 * ifnot, see <http://www.gnu.org/licenses/>.
 *
 */



// this module implements basic biquad filters 
// for more details see: http://en.wikipedia.org/wiki/Digital_biquad_filter

#include "Core/os/asplib_os.h"
#include "Core/Constants_Typedefs/asplib_Typedefs.h"
#include "Core/Constants_Typedefs/asplib_Constants.h"


namespace asplib
{
class CBiquadFactory
{
public:
    // Handle Biquads
    static ASPLIB_ERR            reset_Biquads(ASPLIB_BIQUAD_HANDLE *Biquads);
    static ASPLIB_ERR            destroy_Biquads(ASPLIB_BIQUAD_HANDLE **Biquads);
    static uint32_t              get_maxBiquads(ASPLIB_BIQUAD_HANDLE *Biquads);
    static ASPLIB_BIQUAD_HANDLE* get_Biquads(uint32_t BiquadQuantity, float SampleFrequency, ASPLIB_OPT_MODULE OptModule);
    static ASPLIB_ERR            calc_BiquadSample(ASPLIB_BIQUAD_HANDLE *Biquads, float In, float *Out);
    static ASPLIB_ERR            calc_BiquadSamples(ASPLIB_BIQUAD_HANDLE *Biquads, float *In, float *Out, uint32_t FrameSize);

    // set Biquad Parameters
    static ASPLIB_ERR            set_BiquadCoefficients(ASPLIB_BIQUAD_HANDLE *Biquads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, float C0 = 1.0f, float D0 = 0.0f);
    static ASPLIB_ERR            set_BiquadCoefficients(ASPLIB_BIQUAD_HANDLE *Biquads, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients, uint32_t BiquadIdx, float C0 = 1.0f, float D0 = 0.0f);
    // Octave EQs, 1 Octave (9 to 11 bands), 2/3 octave (15 - 17 bands),  1/2 octave (17 to 22 bands), and 1/3 octave (30 - 31 bands)
    // frequency bands are defined in ISO R 266-1997 or ANSI S1.6-1984
    static ASPLIB_ERR            set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain);
    static ASPLIB_ERR            set_constQPeakingParams(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain, uint32_t BiquadIdx);
    static ASPLIB_ERR            get_constQPeakingBiquadCoes(ASPLIB_BIQUAD_HANDLE *Biquads, float Gain, uint32_t BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
    static ASPLIB_ERR            get_constQPeakingBiquadCoes(uint32_t SampleFrequency, uint32_t MaxFreqBands, float Gain, uint32_t BiquadIdx, ASPLIB_BIQUAD_COEFFICIENTS *Coefficients);
};
}
