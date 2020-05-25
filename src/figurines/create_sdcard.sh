#!/bin/bash

#mkdir -p sd_card/01 sd_card/02 sd_card/03 tmp_src tmp_dest
#./getaudio.py ../../figurines/*/*.yaml --path_src tmp_src --path_dest tmp_dest
cp tmp_dest/*-nameandsound.mp3 sd_card/03
cp tmp_dest/*-song.mp3 sd_card/02
./create_beep.py
ffmpeg -y -i 003-beep.wav tmp_dest/003-beep.mp3
cp tmp_dest/003-beep.mp3 sd_card/01
./create_command_sounds.py
cp 00?-*.mp3 sd_card/01
