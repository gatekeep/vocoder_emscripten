/**
* Digital Voice Modem - Transcode Library (Emscripten)
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / Transcode Library (Emscripten)
*
*/
/*
*   Copyright (C) 2022 by Bryan Biedenkapp N2PLL
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <emscripten.h>
#include <emscripten/bind.h>

#include <vocoder/MBEDecoder.h>
#include <vocoder/MBEEncoder.h>

#include <vector>

// ---------------------------------------------------------------------------
//  Global Variables
// ---------------------------------------------------------------------------

vocoder::MBEEncoder *g_mbeEncoder = NULL;
vocoder::MBEDecoder *g_mbeDecoder = NULL;

// ---------------------------------------------------------------------------
//  Global Functions
// ---------------------------------------------------------------------------

extern "C"
wasm_export void setup()
{
    g_mbeEncoder = new vocoder::MBEEncoder(vocoder::ENCODE_DMR_AMBE);
    g_mbeDecoder = new vocoder::MBEDecoder(vocoder::DECODE_DMR_AMBE);
}

extern "C"
wasm_export void cleanup()
{
    if (g_mbeEncoder != NULL)
        delete g_mbeEncoder;
    if (g_mbeDecoder != NULL)
        delete g_mbeDecoder;
}

extern "C"
wasm_export void encode(std::vector<int> samples, std::vector<int> codeword)
{
    if (samples.size() > 160U)
        return;
    if (samples.size() < 160U)
        return;

    int16_t pcmSamples[160U];
    ::memset(pcmSamples, 0x00U, 160U);

    for (int i = 0; i < 160U; i++)
        pcmSamples[i] = (uint16_t)samples[i] & 0xFFFFU;

    uint8_t ambe[9U];
    ::memset(ambe, 0x00U, 9U);

    if (g_mbeEncoder != NULL)
        g_mbeEncoder->encode(pcmSamples, ambe);

    for (int i = 0; i < 9U; i++)
        codeword.push_back(ambe[i]);
}

extern "C"
wasm_export void decode(std::vector<int> codeword, std::vector<int> samples)
{
    if (codeword.size() > 9U)
        return;
    if (codeword.size() < 9U)
        return;

    uint8_t ambe[9U];
    ::memset(ambe, 0x00U, 9U);

    for (int i = 0; i < 9U; i++)
        ambe[i] = (uint16_t)codeword[i] & 0xFFU;

    int16_t pcmSamples[160U];
    ::memset(pcmSamples, 0x00U, 160U);

    if (g_mbeDecoder != NULL)
        g_mbeDecoder->decode(ambe, pcmSamples);

    for (int i = 0; i < 160U; i++)
        samples.push_back(pcmSamples[i]);
}

EMSCRIPTEN_BINDINGS(libvocoder) {
    emscripten::class_<vocoder::MBEDecoder>("MBEDecoder");
    emscripten::class_<vocoder::MBEEncoder>("MBEEncoder");
    emscripten::function("setup", &setup);
    emscripten::function("cleanup", &cleanup);
    emscripten::function("encode", &encode, emscripten::allow_raw_pointers());
    emscripten::function("decode", &decode, emscripten::allow_raw_pointers());
}
