# audio_streamer
An audio streamer built with c++, tcp socket and alsa.
This program makes use of low level code to move bytes between machines and play them 
on a sound card.

## alsa
Advanced linux sound architecture. This means this code will only run on Linux, but we're the 
linux lions, we roar with linux.

## running the code
This is made with cmake, and there are some run.sh scripts to quickly start up each program. You will need to have
the alsa library installed, the "-dev" version via apt-get.
Also, this code is making use of some c++ 20 features, so it's a good idea to also check the g++ version you're 
using if there are any errors.
