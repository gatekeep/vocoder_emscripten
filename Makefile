CC      = gcc
CXX     = g++
CXXFLAGS = -g -O3 -Wall -std=c++0x -I.
CFLAGS  = -g -O3 -Wall -I.
LIBS    = 
LDFLAGS = -g
AR      = ar rcs
RANLIB  = ranlib

EMCC    = emcc
EMXX    = em++
EMAR    = emar rcv
EMRANLIB= emranlib
EMFLAGS = --bind -Wl,--whole-archive

OBJECTS = \
		edac/AMBEFEC.o \
		edac/Golay24128.o \
		edac/Hamming.o \
		vocoder/imbe/aux_sub.o \
		vocoder/imbe/basic_op.o \
		vocoder/imbe/ch_decode.o \
		vocoder/imbe/ch_encode.o \
		vocoder/imbe/dc_rmv.o \
		vocoder/imbe/decode.o \
		vocoder/imbe/dsp_sub.o \
		vocoder/imbe/encode.o \
		vocoder/imbe/imbe_vocoder.o \
		vocoder/imbe/math_sub.o \
		vocoder/imbe/pe_lpf.o \
		vocoder/imbe/pitch_est.o \
		vocoder/imbe/pitch_ref.o \
		vocoder/imbe/qnt_sub.o \
		vocoder/imbe/rand_gen.o \
		vocoder/imbe/sa_decode.o \
		vocoder/imbe/sa_encode.o \
		vocoder/imbe/sa_enh.o \
		vocoder/imbe/tbls.o \
		vocoder/imbe/uv_synt.o \
		vocoder/imbe/v_synt.o \
		vocoder/imbe/v_uv_det.o \
		vocoder/ambe3600x2250.o \
		vocoder/ambe3600x2400.o \
		vocoder/ambe3600x2450.o \
		vocoder/ecc.o \
		vocoder/imbe7200x4400.o \
		vocoder/mbe.o \
		vocoder/MBEDecoder.o \
		vocoder/MBEEncoder.o \
		emscripten.o

all:    libvocoder

libvocoder: $(OBJECTS)
		$(EMXX) $(OBJECTS) $(CFLAGS) $(EMFLAGS) -o libvocoder.js

%.o: %.cpp
		$(EMXX) $(CXXFLAGS) -c -o $@ $<
%.o: %.c
		$(EMCC) $(CFLAGS) -c -o $@ $<

clean:
		$(RM) libvocoder.js libvocoder.wasm libvocoder.a *.o *.d *.bak *~ edac/*.o vocoder/imbe/*.o vocoder/*.o
