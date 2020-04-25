// crtsine.cpp STK tutorial program
#include "RtAudio.h"
#include <iostream>
#include "PitShift.h"
#include "JCRev.h"
#include "BiQuad.h"
#include "SineWave.h"
using namespace stk;


JCRev Verb;
PitShift PitchShift_L;
PitShift PitchShift_R;
BiQuad hipass;
BiQuad lowpass;
BiQuad Peak_1;
BiQuad Peak_2;
BiQuad Peak_3;
BiQuad Peak_4;
BiQuad Peak_5;
BiQuad Peak_6;
BiQuad Peak_7;
BiQuad Peak_8;


static StkFloat Verb_Temp1;
static StkFloat Verb_Temp2_L;
static StkFloat Verb_Temp2_R;
static StkFloat Verb_Temp3;
static StkFloat PitchShift_Filtered_L;
static StkFloat PitchShift_Filtered_R;
static StkFloat Peaks_L;
static StkFloat Peaks_R;

static StkFloat Verbt60 = 32.2;
static StkFloat Peak = 55;
static StkFloat Mod = 2.;
static StkFloat ShiftL = 1.01;
static StkFloat ShiftR = 0.99;


// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
    SineWave *Peak_Mod_L = (SineWave *) dataPointer;
    SineWave *Peak_Mod_R = (SineWave *) dataPointer;
    register StkFloat sample;
    register StkFloat Temp_Peak_Mod_L;
    register StkFloat Temp_Peak_Mod_R;
    register StkFloat *Insamples = (StkFloat *) inputBuffer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    for ( unsigned int i=0; i<nBufferFrames; i++ ){
        
        Temp_Peak_Mod_L = Peak_Mod_L->tick();
        Temp_Peak_Mod_R = Peak_Mod_R->tick();
        
        sample = 0.5 * PitchShift_Filtered_L + 0.25 * Verb.lastOut(0) + Peaks_L;
        *samples++ = sample;
        sample = 0.5 * PitchShift_Filtered_R + 0.25 * Verb.lastOut(1) + Peaks_R;
        *samples++ = sample;
        
        Verb_Temp1 = 0.95 * Verb_Temp3 + 0.3 * (*Insamples++);
        Verb.tick( Verb_Temp1 );
        hipass.tick(Verb.lastOut(0));
        Verb_Temp2_L = hipass.lastOut();
        
        hipass.tick(Verb.lastOut(1));
        Verb_Temp2_R = hipass.lastOut();
        
        PitchShift_L.tick(Verb_Temp2_L);
        hipass.tick(PitchShift_L.lastOut());
        lowpass.tick(hipass.lastOut());
        PitchShift_Filtered_L = lowpass.lastOut();
        
        Peaks_L = 0.5 * Temp_Peak_Mod_L * (Peak_1.tick(PitchShift_Filtered_L) + Peak_3.tick(PitchShift_Filtered_L) + Peak_5.tick(PitchShift_Filtered_L) + Peak_7.tick(PitchShift_Filtered_L)) + (Temp_Peak_Mod_R * (Peak_2.tick(PitchShift_Filtered_L) + Peak_4.tick(PitchShift_Filtered_L) + Peak_6.tick(PitchShift_Filtered_L) + Peak_8.tick(PitchShift_Filtered_L) ) );
        
        PitchShift_R.tick(Verb_Temp2_R);
        hipass.tick(PitchShift_R.lastOut());
        lowpass.tick(hipass.lastOut());
        PitchShift_Filtered_R = lowpass.lastOut();
        
        Peaks_R = 0.5 * Temp_Peak_Mod_R * (Peak_1.tick(PitchShift_Filtered_R) + Peak_3.tick(PitchShift_Filtered_R) + Peak_5.tick(PitchShift_Filtered_R) + Peak_7.tick(PitchShift_Filtered_R)) + (Temp_Peak_Mod_L * (Peak_2.tick(PitchShift_Filtered_R) + Peak_4.tick(PitchShift_Filtered_R) + Peak_6.tick(PitchShift_Filtered_R) + Peak_8.tick(PitchShift_Filtered_R)));
        
        Verb_Temp3 = 0.0015 * (PitchShift_Filtered_L + PitchShift_Filtered_R + Peaks_L + Peaks_R);
        
    }
    return 0;
}
int main()
{
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 48000.0 );
    SineWave sine;
    SineWave Peak_Mod_L;
    SineWave Peak_Mod_R;
    RtAudio adac;
    
    Verb.setT60(Verbt60);
    Verb.setEffectMix(1);
    
    PitchShift_L.setShift(ShiftL);
    PitchShift_L.setEffectMix(1);
    PitchShift_R.setShift(ShiftR);
    PitchShift_R.setEffectMix(1);
    
    hipass.setNotch(60, 0.6);
    lowpass.setNotch(18000, 0.6);
    
    Peak_1.setResonance(Peak,0.99,1);
    Peak_2.setResonance(Peak*2,0.98,1);
    Peak_3.setResonance(Peak*3,0.97,1);
    Peak_4.setResonance(Peak*4,0.96,1);
    Peak_5.setResonance(Peak*5,0.95,1);
    Peak_6.setResonance(Peak*6,0.93,1);
    Peak_7.setResonance(Peak*7,0.91,1);
    Peak_8.setResonance(Peak*8,0.89,1);
    
    Peak_Mod_L.setFrequency(Mod);
    Peak_Mod_R.setFrequency(Mod);
    
    Peak_Mod_R.addPhaseOffset(0.25);
    
    
    // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
    RtAudio::StreamParameters oParameters, iParameters;
    oParameters.deviceId = adac.getDefaultOutputDevice();
    iParameters.deviceId = adac.getDefaultInputDevice();
    unsigned int nChannels = 2;
    oParameters.nChannels = nChannels;
    iParameters.nChannels = nChannels;
    ;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    try {
        adac.openStream( &oParameters, &iParameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&sine);
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
        goto cleanup;
    }
    sine.setFrequency(Mod);
    try {
        adac.startStream();
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
        goto cleanup;
    }
    // Block waiting here.
    char keyhit;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get( keyhit );
    // Shut down the output stream.
    try {
        adac.closeStream();
    }
    catch ( RtAudioError &error ) {
        error.printMessage();
    }
cleanup:
    return 0;
}
