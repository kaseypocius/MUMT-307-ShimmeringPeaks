<!---layout: page
title: "Why?"
permalink: /why/--->

<h1> What's Shimmer? </h1>
According to VST manufacturer Valhalla DSP, the term <a href="https://valhalladsp.com/2010/05/11/enolanois-shimmer-sound-how-it-is-made/">Shimmer Reverb </a>often refers to a technique developed by Brian Eno & Daniel Lanois where the tail of a reverberant signal is fed through a +1 octave pitch shifter (or a tape machine at double speed) and then fed back into the reverb system, first heard on Eno & Lanois' collaborative album <a href="https://www.youtube.com/watch?v=v2DRWVHPRxs"><i>Apollo</i></a> and made popular by their production work for U2

<h2> Why Shimmer? </h2>
I'm a big fan of Brian Eno & Dan Lanois' work, and have been experimenting a lot in my own artistic process with feedback in realtime pitch shifting systems, particularly after the remaster of Apollo over the summer of 2019. I have continued to find combinations of pitch shifting & feedback loops continue to produce artistically satisfying results, especially in real time environments, and while I have plenty of plugins at my disposal that implement this with delay lines, my collection of shimmer reverbs, particulars those aimed at more abstract sound design is lacking. Often, to create the effects I'm after, I have to string together several plugins together, with a send & receive loop between audio devices to bypass feedback control in DAWs like Reaper. Shimmer reverb is also something I had played around with building in VCV Rack using NYSTHI's implementation of the <a href="https://library.vcvrack.com/NYSTHI/STKPitchShifter"> STKPitchshifters </a> for slightly more stable real time use, so I already had a good idea of the signal path going in, and just really had the challenge of implementing this with STK, which I am still relatively new to.

<h2> Why Bandpass Filters? </h2>
During the creation process of my work <a href="https://sonus.ca/oeuvre/46674/Feedback_Cycles_Kasey_Pocius">Feedback Cycles</a>, I had experimented with using filtered feedback signals with the <a href="http://www.doepfer.de/a100_man/A199_man.pdf"> Doepfer A-199 Spring Reverb module </a> and was incredible pleased with the result, but felt limited after awhile with using just one filter, and mixing multiple filters together would suffer due to analog noise build up. While working on how to expand the initial shimmer algorithm I was reminded of this process by a video by <a href="https://www.youtube.com/watch?v=Zp7DKJaQs9o"> Hainbach </a> about using filters with analog mixing boards to create resonant feedback, and wanted to try bringing this into the digital text based realm.

<h2> Why these Reverb Algorithms? </h2>
This started as a reverb focused for use with the synthesizers and still is largely focused on this, so my first choice was the JCRev, based on Chowning's focus on FM synthesis and my existing familiarity with the algorithm. I noted that both the NRev & PCRev both use the same syntax as the JCRev, so implementing them into the script was pretty easy, and allowed for a wide variety of reverb characters while keeping the controls constant across all of the versions. 

<h2> What sounds didn't make the cut? </h2>


<h4> Some other artistic inspirations worth mentioning</h4>
The following VSTs and hardware modules have heavily inspired the design here

<h6>VSTs</h6>
<a href ="https://valhalladsp.com/shop/reverb/valhalla-shimmer/"> Valhalla Shimmer</a>
<a href="https://www.youtube.com/watch?v=UE57APMMi7M">Soundtoys Crystalizer</a> VST version of some of the pitch shifting delay patches on the Eventide h3000
<a href="https://www.pluginboutique.com/product/2-Effects/53-Multi-Effect-/5226-Excalibur"> Exponential Audio Excalibur </a> Multi Effects VST, which has spurred my interest in the combination of Feedback systems with Ring Mod & Pitch Shifters

<h6>Hardware</h6>
<a href ="https://www.youtube.com/watch?v=9Svm0Ku6cAY"> Shimmer Cartridge for Tiptop Audio Z-DSP </a> - the quad version of Shimmering Peaks is inspired by the Quad Mode on this version of the Valhalla shimmer algorithm  
<a href="https://youtu.be/PS_aj2jZgyM?t=1215">Erica Synths Black Hole DSP 2</a> Another eurorack module with a shimmer implementation  

Hainbach
Dan Lanois

 <a href="https://kaseypocius.github.io/MUMT306-MagicMappedKettle/about"> Back to the About</a>
