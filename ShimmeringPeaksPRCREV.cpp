//g++ -Istk/include/ -Lstk/src/ -D__MACOSX_CORE__ ShimmeringPeaksPRCREV.cpp -lstk -lpthread -framework CoreAudio -framework CoreMIDI -framework CoreFoundation -o ShimmeringPeaks-PRCRev

#include "RtAudio.h"
#include <iostream>
#include "PitShift.h"
#include "PRCRev.h"
#include "BiQuad.h"
#include "SineWave.h"
using namespace stk;

//gets the effects together
PRCRev Verb;
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
BiQuad Peak_9;
BiQuad Peak_10;
BiQuad Peak_11;
BiQuad Peak_12;
BiQuad Peak_13;
BiQuad Peak_14;
BiQuad Peak_15;
BiQuad Peak_16;

//casts the temp variables for DSP processing
static StkFloat Verb_Temp1;
static StkFloat Verb_Temp2_L;
static StkFloat Verb_Temp2_R;
static StkFloat Verb_Temp3;
static StkFloat PitchShift_Filtered_L;
static StkFloat PitchShift_Filtered_R;
static StkFloat Peaks_L;
static StkFloat Peaks_R;

//creates the empty gain variables. These will be given values by the user later.
static StkFloat InGain;
static StkFloat OutGain;
static StkFloat Feedback;
static StkFloat PitchShiftMix;
static StkFloat VerbMix;
static StkFloat PeakMix;


// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
  double streamTime, RtAudioStreamStatus status, void *dataPointer )
  {
    //Create registers needed for real time input & output, and the pointer for the Sinewave to generate properly in the callback function
    SineWave *Peak_Mod_L = (SineWave *) dataPointer;
    SineWave *Peak_Mod_R = (SineWave *) dataPointer;
    Peak_Mod_R->addPhaseOffset(0.5);
    register StkFloat sample;
    register StkFloat Temp_Peak_Mod_L;
    register StkFloat Temp_Peak_Mod_R;
    register StkFloat *Insamples = (StkFloat *) inputBuffer;
    register StkFloat *samples = (StkFloat *) outputBuffer;
    //process audio when buffer frames must be filled
    for ( unsigned int i=0; i<nBufferFrames; i++ ){
      //generates modulator wave
      Temp_Peak_Mod_L = Peak_Mod_L->tick();
      Temp_Peak_Mod_R = Peak_Mod_R->tick();

      //Mixing of the various outputs to the output
      sample = PitchShiftMix * PitchShift_Filtered_L + VerbMix * Verb.lastOut(0) + PeakMix * Peaks_L;
      *samples++ = sample * OutGain;
      sample = PitchShiftMix * PitchShift_Filtered_R + VerbMix * Verb.lastOut(1) + PeakMix * Peaks_R;
      *samples++ = sample * OutGain;

      //Mixing of feedback & Input to send into the reverb
      Verb_Temp1 = 0.95 * Verb_Temp3 + InGain * 0.3 * (*Insamples++);

      //Gain check to try and avoid some clipping/add some distortion to the signal at high gains
      if (Verb_Temp1 > 0.99){
        Verb_Temp1 = (Verb_Temp1 * (0.9/Verb_Temp1));
      }

      //Start of the reverb chain
      Verb.tick( Verb_Temp1 );

      //hipassing the post reverb signal, to avoid possible low end build up
      hipass.tick(Verb.lastOut(0));
      Verb_Temp2_L = hipass.lastOut();
      hipass.tick(Verb.lastOut(1));
      Verb_Temp2_R = hipass.lastOut();

      //Pitch shifts, then highpasses, then lowpasses the audio, to avoid build up on either end of the spectrum and to avoid some self resonating
      PitchShift_L.tick(Verb_Temp2_L);
      hipass.tick(PitchShift_L.lastOut());
      lowpass.tick(hipass.lastOut());
      PitchShift_Filtered_L = lowpass.lastOut();

      //The filtered signal is then passed through the bandpass filter bank
      Peaks_L = 0.5 * Temp_Peak_Mod_L * (Peak_1.tick(PitchShift_Filtered_L) + Peak_3.tick(PitchShift_Filtered_L) + Peak_5.tick(PitchShift_Filtered_L) + Peak_7.tick(PitchShift_Filtered_L) + Peak_9.tick(PitchShift_Filtered_L) + Peak_11.tick(PitchShift_Filtered_L) + Peak_13.tick(PitchShift_Filtered_L) + Peak_15.tick(PitchShift_Filtered_L)) + (Temp_Peak_Mod_R * (Peak_2.tick(PitchShift_Filtered_L) + Peak_4.tick(PitchShift_Filtered_L) + Peak_6.tick(PitchShift_Filtered_L) + Peak_8.tick(PitchShift_Filtered_L) + Peak_10.tick(PitchShift_Filtered_L) + Peak_12.tick(PitchShift_Filtered_L) + Peak_14.tick(PitchShift_Filtered_L) + Peak_16.tick(PitchShift_Filtered_L) ) );

      //Repeat for the right side
      PitchShift_R.tick(Verb_Temp2_R);
      hipass.tick(PitchShift_R.lastOut());
      lowpass.tick(hipass.lastOut());
      PitchShift_Filtered_R = lowpass.lastOut();

      Peaks_R = 0.5 * Temp_Peak_Mod_R * (Peak_1.tick(PitchShift_Filtered_R) + Peak_3.tick(PitchShift_Filtered_R) + Peak_5.tick(PitchShift_Filtered_R) + Peak_7.tick(PitchShift_Filtered_R) + Peak_9.tick(PitchShift_Filtered_R) + Peak_11.tick(PitchShift_Filtered_R) + Peak_13.tick(PitchShift_Filtered_R) + Peak_15.tick(PitchShift_Filtered_R)) + ((Temp_Peak_Mod_L * (Peak_2.tick(PitchShift_Filtered_R) + Peak_4.tick(PitchShift_Filtered_R) + Peak_6.tick(PitchShift_Filtered_R) + Peak_8.tick(PitchShift_Filtered_R) + Peak_10.tick(PitchShift_Filtered_R) + Peak_12.tick(PitchShift_Filtered_R) + Peak_14.tick(PitchShift_Filtered_R) + Peak_16.tick(PitchShift_Filtered_R))));

      //Mix everything together into one mono signal to return back into the reverb
      Verb_Temp3 = 0.0015 * Feedback * (PitchShift_Filtered_L + PitchShift_Filtered_R + Peaks_L + Peaks_R);

    }
    return 0;
  }

  int main(int argc, char *argv[])
  {
    // Set the global sample rate before creating class instances.
    Stk::setSampleRate( 48000.0 );
    SineWave sine;
    SineWave Peak_Mod_L;
    SineWave Peak_Mod_R;
    RtAudio adac;

    //Sets the user declared values for processing

    if ( argc != 13 ) {
      std::cout << "usage: " << argv[0] << " filepath" << std::endl;
      std::cout << "    dump var for filepath" << std::endl;
      std::cout << "usage: " << argv[1] << "s: t60" << std::endl;
      std::cout << "   Verbt60 is the time for a sound to decay 60dB in the revberant system when no feedback is present, measured in seconds" << std::endl;
      std::cout << "usage: " << argv[2] << " Hz: Base Peak Frequency" << std::endl;
      std::cout << "    Peak is the frequency of the first bandpass filter in Hz" << std::endl;
      std::cout << "usage: " << argv[3] << " Peaks Spacing Constant" << std::endl;
      std::cout << " Peak Spacing constant is used to calculate the distance between each Peak, where 1. results in the in first 8 harmonics of the Base Peak Frequency" << std::endl;
      std::cout << "usage: " << argv[4] << " Hz: Mod Frequency" << std::endl;
      std::cout << "Mod is the frequency of the oscilator modulating the amplitude of the filter bank, measured in Hz" << std::endl;
      std::cout << "usage: " << argv[5] << " Left Shift Factor" << std::endl;
      std::cout << "ShiftL controls the pitch shift of the left channel, where 1. is no change, 0.5 is one octave down and 2. is one octave up" << std::endl;
      std::cout << "usage: " << argv[6] << " Right Shift Factor" << std::endl;
      std::cout << "ShiftR controls the pitch shift of the right channel, where 1. is no change, 0.5 is one octave down and 2. is one octave up" << std::endl;
      std::cout << "usage: " << argv[7] << " Input Gain" << std::endl;
      std::cout << "InGain is the gain of the mono input signal, where 1. is unity gain" << std::endl;
      std::cout << "usage: " << argv[8] << " Output Gain" << std::endl;
      std::cout << "OutGain is the gain of all output channels, where 1. is unity gain" << std::endl;
      std::cout << "usage: " << argv[9] << " Feedback" << std::endl;
      std::cout << "Feedback is the gain constant of the feedback signal, where 1. is the highest possible feedback gain for a t60 of 22 seconds, and -1. is this gain where the polarity is inverted every pass of the feedback loop" << std::endl;
      std::cout << "usage: " << argv[10] << " Pitch Shift Mix" << std::endl;
      std::cout << "PitchShiftMix is the gain of the Pitch Shifted signal in the final output mix, where 1. is unity gain" << std::endl;
      std::cout << "usage: " << argv[11] << " Verb Mix" << std::endl;
      std::cout << "VerbMix is the gain of the output of the reverb signal in the final output mix, where 1. is unity gain" << std::endl;
      std::cout << "usage: " << argv[12] << " Peak Mix" << std::endl;
      std::cout << "PeakMix is the gain of the output of the filterbank signal in the final output mix, where 1. is unity gain" << std::endl;
      return(0);
    }

    std::cout << std::endl;
    std::cout << "Settings Dump" << std::endl;
    std::cout << std::endl;
    std::cout << argv[1] << "s t60" << std::endl;
    std::cout << argv[2] << " Hz Base Peak Frequency" << std::endl;
    std::cout << argv[3] << " Peaks Spacing Constant" << std::endl;
    std::cout  << argv[4] <<" Hz Mod Frequency" << std::endl;
    std::cout << argv[5] << " Left Shift Factor" << std::endl;
    std::cout << argv[6] << " Right Shift Factor" << std::endl;
    std::cout << argv[7] << " Input Gain" << std::endl;
    std::cout << argv[8] << " Output Gain" << std::endl;
    std::cout << argv[9] << " Feedback" << std::endl;
    std::cout << argv[10] << " Pitch Shift Mix" << std::endl;
    std::cout << argv[11] << " Verb Mix" << std::endl;
    std::cout << argv[12] << " Peak Mix" << std::endl;

    static StkFloat Verbt60 = atof(argv[1]);

    if (Verbt60 <= 0){
      std::cout << " Verbt60 should be more than zero.... setting duration to 1.2 seconds" << std::endl;
      Verbt60 = 1.2;
    }

    static StkFloat Peak = atof(argv[2]);

    if (Peak <= 0){
      std::cout << "The Base Peak Frequency should be more than zero.... setting to 55 Hz" << std::endl;
      Peak = 55;
    }

    static StkFloat PeakSpacing = atof(argv[3]);

    if (PeakSpacing <= 0){
      std::cout << " Peak Spacing consant should be more than zero.... setting to 1" << std::endl;
      PeakSpacing = 1;
    }

    static StkFloat Mod = atof(argv[4]);

    if (Mod <= 0){
      std::cout << " Mod Frequency should be more than zero.... setting to 0.05 Hz" << std::endl;
      Mod = 0.05;
    }

    static StkFloat ShiftL = atof(argv[5]);

    if (ShiftL <= 0){
      std::cout << "Left Shift Factor should be more than zero.... setting 2." << std::endl;
      ShiftL = 2.;
    }
    static StkFloat ShiftR = atof(argv[6]);

    if (ShiftR <= 0){
      std::cout << "Right Shift Factor should be more than zero.... setting to 1.6" << std::endl;
      ShiftR = 1.6;
    }

    InGain = atof(argv[7]);

    if (InGain <= 0){
      std::cout << "Input Gain should be more than zero.... setting to 0.55" << std::endl;
      InGain = 0.55;
    }

    OutGain = atof(argv[8]);

    if (OutGain <= 0){
      std::cout << "Output Gain should be more than zero.... setting 0.55" << std::endl;
      Peak = 0.55;
    }

    Feedback = atof(argv[9]);

    PitchShiftMix = atof(argv[10]);

    if (PitchShiftMix < 0){
      std::cout << "PitchShiftMix should be positive.... setting to 1." << std::endl;
      PitchShiftMix = 1.;
    }

    VerbMix = atof(argv[11]);

    if (VerbMix < 0){
      std::cout << "VerbMix should be positive.... setting to 0.5" << std::endl;
      VerbMix = 0.5;
    }

    PeakMix = atof(argv[12]);

    if (PeakMix < 0){
      std::cout << "PeakMix should be positive.... setting to 0.5" << std::endl;
      PeakMix = 0.5;
    }

    //sets the variables into the needed places for audio generation

    Verb.setT60(Verbt60);
    Verb.setEffectMix(1);

    PitchShift_L.setShift(ShiftL);
    PitchShift_L.setEffectMix(1);
    PitchShift_R.setShift(ShiftR);
    PitchShift_R.setEffectMix(1);

    hipass.setNotch(60, 0.6);
    lowpass.setNotch(18000, 0.6);

    Peak_1.setResonance(Peak,0.99,1);
    Peak_2.setResonance(Peak*2*PeakSpacing,0.98,1);
    Peak_3.setResonance(Peak*3*PeakSpacing,0.97,1);
    Peak_4.setResonance(Peak*4*PeakSpacing,0.96,1);
    Peak_5.setResonance(Peak*5*PeakSpacing,0.95,1);
    Peak_6.setResonance(Peak*6*PeakSpacing,0.94,1);
    Peak_7.setResonance(Peak*7*PeakSpacing,0.93,1);
    Peak_8.setResonance(Peak*8*PeakSpacing,0.92,1);
    Peak_9.setResonance(Peak*9*PeakSpacing,0.91,1);
    Peak_10.setResonance(Peak*10*PeakSpacing,0.90,1);
    Peak_11.setResonance(Peak*11*PeakSpacing,0.88,1);
    Peak_12.setResonance(Peak*12*PeakSpacing,0.86,1);
    Peak_13.setResonance(Peak*13*PeakSpacing,0.84,1);
    Peak_14.setResonance(Peak*14*PeakSpacing,0.82,1);
    Peak_15.setResonance(Peak*15*PeakSpacing,0.80,1);
    Peak_16.setResonance(Peak*16*PeakSpacing,0.78,1);

    Peak_Mod_L.setFrequency(Mod);
    Peak_Mod_R.setFrequency(Mod);

    // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
    RtAudio::StreamParameters oParameters, iParameters;
    oParameters.deviceId = adac.getDefaultOutputDevice();
    iParameters.deviceId = adac.getDefaultInputDevice();
    unsigned int nChannels = 2;
    oParameters.nChannels = nChannels;
    iParameters.nChannels = nChannels;

    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

    unsigned int bufferFrames = RT_BUFFER_SIZE;

    try {
      adac.openStream( &oParameters, &iParameters, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&Peak_Mod_L);
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
    std::cout << "See you soon!" << std::endl;
    cleanup:
    return 0;
  }
