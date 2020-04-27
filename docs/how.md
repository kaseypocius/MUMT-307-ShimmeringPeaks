<!---layout: page
title: "How?"
permalink: /how/--->

<h2> How? </h2>

A Mono audio input is fed into a mono to stereo reverb, and then pitch shifted, with independent pitch shifting of the left & right channels available. Each pitch shifted channel is then fed through 8 resonant filters in parallel. The filtered and pitch shifted audio is then mixed down to mono and fed back into the reverb feed, along with the dry audio to create the classic "shimmer" effect. The level of the bandpass filters is modulated by 2 sine waves, which can be pushed into audio range for ring modulation to be introduced to the feedback path, or run at low levels to create panning effects.

<h2> Why Use My Patches?</h2>
Shimmering Peaks can be used to achieve a wide variety of audio effects. Pitch shift values close to 1 will use the feedback loop to excite the filter bank, reinforcing & smearing spectral content relative to the reverb time and feedback gain. Higher pitch shift values will cause more traditional shimmer like effects, with filter banks working to excite other filters in the system as the signals are pitch shifted.

The nature of the mono - stereo reverb is also quite important to the character of the Shimmer Peaks, as both pitch shifted feedback signals will feed into both pitch shifters, distributed by the early & late reflection systems. Additionally, the pitch shifting algorithm extend outside of the typical one octave range, which when paired with the resonator bank, can be used to easily create effects ranging from spring reverb like to lush sub bass pads and beyond.

<h2> A closer look at the code </h2>

<h2> Challenges </h2>

<h2> For the Future</h2>

Further modulation could easily be expanded by replacing the basic sine tone to modular wave table bank, as well as adding frequency or resonance modulation to the filter bank as well. Traditional shimmer verbs also add incorporate a delay line, which could add further modulation possibilities - chorus, further pitch shifting & comb filtering, reverse etc. However I've chosen to focus instead on implementing this algorithm with a few reverbs, which I've found is my personal favorite way of experimenting with changes in comb filter combinations here.

 <a href="https://kaseypocius.github.io/MUMT306-MagicMappedKettle/about"> Back to the About</a>
