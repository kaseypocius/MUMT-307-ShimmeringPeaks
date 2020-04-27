<!---layout: page
title: "How?"
permalink: /how/--->

<h2> How? </h2>

A Mono audio input is fed into a mono to stereo reverb, and then pitch shifted, with independent pitch shifting of the left & right channels available. Each pitch shifted channel is then fed through 16 resonant filters in parallel. The filtered and pitch shifted audio is then mixed down to mono and fed back into the reverb feed, along with the dry audio to create the classic "shimmer" effect. The level of the bandpass filters is modulated by 2 sine waves, which can be pushed into audio range for ring modulation to be introduced to the feedback path, or run at low levels to create panning effects.

<h2> Why Use My Patches?</h2>
Shimmering Peaks can be used to achieve a wide variety of audio effects. Pitch shift values close to 1 will use the feedback loop to excite the filter bank, reinforcing & smearing spectral content relative to the reverb time and feedback gain. Higher pitch shift values will cause more traditional shimmer like effects, with filter banks working to excite other filters in the system as the signals are pitch shifted.

The nature of the mono - stereo reverb is also quite important to the character of the Shimmer Peaks, as both pitch shifted feedback signals will feed into both pitch shifters, distributed by the early & late reflection systems. Additionally, the pitch shifting algorithm extend outside of the typical one octave range, which when paired with the resonator bank, can be used to easily create effects ranging from spring reverb like to lush sub bass pads and beyond.

<h2> A closer look at the code </h2>
Lets let a step by step look at how the code works. We'll be using the JCRev version as an Example

<pre><code>#include "RtAudio.h"
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
</code></pre>

Here the proper header files are loaded, and the STK class instances not using a data pointer are created here for use later on. We then create all the temp variables that are used in the processing loop, as well as all the gain variables that will be defined once we load the user input

<h2> Challenges </h2>

<h2> For the Future</h2>

Further modulation could easily be expanded by replacing the basic sine tone to modular wave table bank, as well as adding frequency or resonance modulation to the filter bank as well. Traditional shimmer verbs also add incorporate a delay line, which could add further modulation possibilities - chorus, further pitch shifting & comb filtering, reverse etc. However I've chosen to focus instead on implementing this algorithm with a few different reverb classes, which I've found was the most rewarding way to easily give a wide variety of reverb characters while keeping the controls constant between designs.

 <a href="https://kaseypocius.github.io/MUMT-307-ShimmeringPeaks/about"> Back to the About</a>
