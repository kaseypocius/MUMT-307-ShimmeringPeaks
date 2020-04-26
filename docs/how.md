<!---layout: page
title: "How?"
permalink: /how/--->

<h2> How? </h2>

A Mono audio input is fed into a mono to stereo reverb, and then pitch shifted, with independent pitch shifting of the left & right channels available. Each pitch shifted channel is then fed through 8 resonant filters in parallel. The filtered and pitch shifted audio is then mixed down to mono and fed back into the reverb feed, along with the dry audio to create the classic "shimmer" effect. The level of the bandpass filters is modulated by 2 sine waves, which can be pushed into audio range for ring modulation to be introduced to the feedback path, or run at low levels to create panning effects.

<h2> Why Use My Patches?</h2>
Shimmering Peaks can be used to achieve a wide variety of audio effects. Pitch shift values close to 1 will use the feedback loop to excite the filter bank, reinforcing & smearing spectral content relative to the reverb time and feedback gain. Higher pitch shift values will cause more traditional shimmer like effects, with filter banks working to excite other filters in the system as the signals are pitch shifted.

The nature of the mono - stereo reverb is also quite important to the character of the Shimmer Peaks, as both pitch shifted feedback signals will feed into both pitch shifters, distributed by the early & late reflection systems.

<h2> For the Future</h2>

The Arduino could be replaced by a wireless model, and a network enabled model with the <a href="https://github.com/CNMAT/OSC">CNMAT OSC Library </a> would allow for all the OSC formatting to be done on the Arduino itself, removing the need for the max patch. More robust sensors may be needed, but as stated previously, I seem to have lucked out with my own sensors. Further updates to the patches will include flags and triggers for certain states it detects in the water (ie rolling, coming out of boil, transients in the hydrophone), that would be available to the user for mapping, though as stated in the why page, I have yet to find a good musical implementation of these data sets. Many of those previously mentioned mappings would also increase dependency on the hydrophone's audio data, as my current calibration protocols prioritize smooth musical ramps, which highlight the highs and lows of the sensor set and can be easily transported from room to room over instead of assigning events to exact degree/humidity values.

 <a href="https://kaseypocius.github.io/MUMT306-MagicMappedKettle/about"> Back to the About</a>
