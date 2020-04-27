<!---layout: page
title: "How?"
permalink: /how/--->

<h2> How? </h2>

A Mono audio input is fed into a mono to stereo reverb, and then pitch shifted, with independent pitch shifting of the left & right channels available. Each pitch shifted channel is then fed through 16 resonant filters in parallel. The filtered and pitch shifted audio is then mixed down to mono and fed back into the reverb feed, along with the dry audio to create the classic "shimmer" effect. The level of the bandpass filters is modulated by 2 sine waves, which can be pushed into audio range for ring modulation to be introduced to the feedback path, or run at low levels to create panning effects.

<h2> Why Use My Patches?</h2>
Shimmering Peaks can be used to achieve a wide variety of audio effects. Pitch shift values close to 1 will use the feedback loop to excite the filter bank, reinforcing & smearing spectral content relative to the reverb time and feedback gain. Higher pitch shift values will cause more traditional shimmer like effects, with filter banks working to excite other filters in the system as the signals are pitch shifted.

The nature of the mono - stereo reverb is also quite important to the character of the Shimmer Peaks, as both pitch shifted feedback signals will feed into both pitch shifters, distributed by the early & late reflection systems. Additionally, the pitch shifting algorithm extend outside of the typical one octave range, which when paired with the resonator bank, can be used to easily create effects ranging from spring reverb like to lush sub bass pads and beyond.

<h2> A closer look at the code </h2>
Lets let a step by step look at how the DSP portion code works. We'll be using the JCRev version as an Example
<br>

```
#include "RtAudio.h"
#include <iostream>
#include "PitShift.h"
#include "JCRev.h"
#include "BiQuad.h"
#include "SineWave.h"
using namespace stk;

//gets the effects together
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
static StkFloat PitchShift_Sum_L;
static StkFloat PitchShift_Sum_R;
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
```

<br>

Here the proper header files are loaded, and the STK class instances not using a data pointer are created here for use later on. We then create all the temp variables that are used in the processing loop, as well as all the gain variables that will be defined once we load the user input

<br>

```
// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
  double streamTime, RtAudioStreamStatus status, void *dataPointer)
  ```
  <br>

  Here we build our tick statement. This patch uses "callback" audio, which will call the FX script every time a new block of audio is needed. This also builds the data pointer we'll use for the modulator oscillators, to avoid a segment fault on start up.

  <br>

  ```
  //Create registers needed for real time input & output, and the pointer for the Sinewave to generate properly in the callback function
  SineWave *Peak_Mod_L = (SineWave *) dataPointer;
  SineWave *Peak_Mod_R = (SineWave *) dataPointer;
  Peak_Mod_R->addPhaseOffset(0.5);
  register StkFloat sample;
  register StkFloat Temp_Peak_Mod_L;
  register StkFloat Temp_Peak_Mod_R;
  register StkFloat *Insamples = (StkFloat *) inputBuffer;
  register StkFloat *samples = (StkFloat *) outputBuffer;
  ```

  <br>

  Here we instantiate the sine wave modulators to the pointer, add phase offset, and create the registers that will move audio from the audio card in and out of the of the fx script.

  <br>

  ```
  //process audio when buffer frames must be filled
      for ( unsigned int i=0; i<nBufferFrames; i++ ){
          //generates modulator wave
          Temp_Peak_Mod_L = Peak_Mod_L->tick();
          Temp_Peak_Mod_R = Peak_Mod_R->tick();

  ```
<br>

Now we really start processing. We start by generating the samples of the modulator waves

<br>
```
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
```
<br>

Mixing of the gain on the inputs & outputs. The mix on the outputs only affects the final outputs, so you can continue use the pitch shifters to excite the filter bank in various ways.

<br>

 The input buffer for the reverb is built here here from the feedback Temp value and the current input to the adac, as scaled by the gain argument.

<br> Some additional multiplication is done on the input buffer here to avoid peaking and self oscillation with sustained tones and long reverb times, but in a system based on feedback, this still occurs. We also do a gain check to see if the signal clips once both signals are summed, and if so, do extra signal scaling.  

<br>
```
//Start of the reverb chain
Verb.tick( Verb_Temp1 );

//hipassing the post reverb signal, to avoid possible low end build up
hipass.tick(Verb.lastOut(0));
Verb_Temp2_L = hipass.lastOut();
hipass.tick(Verb.lastOut(1));
Verb_Temp2_R = hipass.lastOut();
```
<br>

Reverb is then calculated, and some initial filtering is done, which avoids some more build up on the lower and upper ends of the spectrum

<br>
```
//Pitch shifts, then highpasses, then lowpasses the audio, to avoid build up on either end of the spectrum and to avoid some self resonating
PitchShift_L.tick(Verb_Temp2_L);
hipass.tick(PitchShift_L.lastOut());
lowpass.tick(hipass.lastOut());
PitchShift_Filtered_L = lowpass.lastOut();

```
<br>

The signals are then pitch shifted and filtered again to eliminate any new unwanted frequencies. In the Quad models, the pitch shifted signals are summed here as well

<br>
```
//The filtered signal is then passed through the bandpass filter bank
Peaks_L = 0.5 * Temp_Peak_Mod_L * (Peak_1.tick(PitchShift_Filtered_L) + Peak_3.tick(PitchShift_Filtered_L) + Peak_5.tick(PitchShift_Filtered_L) + Peak_7.tick(PitchShift_Filtered_L) + Peak_9.tick(PitchShift_Filtered_L) + Peak_11.tick(PitchShift_Filtered_L) + Peak_13.tick(PitchShift_Filtered_L) + Peak_15.tick(PitchShift_Filtered_L)) + (Temp_Peak_Mod_R * (Peak_2.tick(PitchShift_Filtered_L) + Peak_4.tick(PitchShift_Filtered_L) + Peak_6.tick(PitchShift_Filtered_L) + Peak_8.tick(PitchShift_Filtered_L) + Peak_10.tick(PitchShift_Filtered_L) + Peak_12.tick(PitchShift_Filtered_L) + Peak_14.tick(PitchShift_Filtered_L) + Peak_16.tick(PitchShift_Filtered_L) ) );

```
<br>

We then put the left pitch shifted signal through the bandpass filter bank, modulate the resulting gains with the mod oscillator, and then sum them together. In the next step we repeat the above filtering steps for the right side, but this time we use the mod oscillators in inverse order. This creates the effect of spreading the modulation of the Odd & Even filter banks across the stereo field, which creates a phantom image that moves towards the center as the two oscillators meet at their zero crossings.

<br>
```
//Repeat for the right side
PitchShift_R.tick(Verb_Temp2_R);
hipass.tick(PitchShift_R.lastOut());
lowpass.tick(hipass.lastOut());
PitchShift_Filtered_R = lowpass.lastOut();

Peaks_R = 0.5 * Temp_Peak_Mod_R * (Peak_1.tick(PitchShift_Filtered_R) + Peak_3.tick(PitchShift_Filtered_R) + Peak_5.tick(PitchShift_Filtered_R) + Peak_7.tick(PitchShift_Filtered_R) + Peak_9.tick(PitchShift_Filtered_R) + Peak_11.tick(PitchShift_Filtered_R) + Peak_13.tick(PitchShift_Filtered_R) + Peak_15.tick(PitchShift_Filtered_R)) + ((Temp_Peak_Mod_L * (Peak_2.tick(PitchShift_Filtered_R) + Peak_4.tick(PitchShift_Filtered_R) + Peak_6.tick(PitchShift_Filtered_R) + Peak_8.tick(PitchShift_Filtered_R) + Peak_10.tick(PitchShift_Filtered_R) + Peak_12.tick(PitchShift_Filtered_R) + Peak_14.tick(PitchShift_Filtered_R) + Peak_16.tick(PitchShift_Filtered_R))));

```
<br>

Then we mix everything together into the needed mono signal to sent back into the reverberators. The Feedback constant can be a negative value, so you can also use this step to invert the phase of the signal before it's fed back into the reverberator, added another level of modulation depth if desired.

<br>
```
//Mix everything together into one mono signal to return back into the reverb
Verb_Temp3 = 0.0015 * Feedback * (PitchShift_Filtered_L + PitchShift_Filtered_R + Peaks_L + Peaks_R);

```
<br>

  <h2> Challenges </h2>

  The biggest challenge was building for the callback engine, which I had not done in STK before, as most of my experience so far had been with the blocking algorithm. This made implementing some things, such as oscillators, more challenging than I had initially expected, but I am still quite satisfied with the result.

  Finding good gain settings to act as a reference required a bit of trial and error, and I'm still not satisfied with fixed resonance for each filter, but I haven't found a satisfying way to do this setup without too many user defined variables.

  <h2> For the Future</h2>

  Further modulation could easily be expanded by replacing the basic sine tone to modular wave table bank, as well as adding frequency and/or resonance modulation to the filter bank as well. Traditional shimmer verbs also add incorporate a delay line, which could add further modulation possibilities - chorus, further pitch shifting & comb filtering, reverse etc. However I've chosen to focus instead on implementing this algorithm with a few different reverb classes, which I've found was the most rewarding way to easily give a wide variety of reverb characters while keeping the controls constant between designs. The frequency and notch depth of the High pass & Low pass filters could have been user definable as well, but I'd like to limit the amount of user definable variables needed for use of the program.

  <a href="https://kaseypocius.github.io/MUMT-307-ShimmeringPeaks/about"> Back to the About</a>
