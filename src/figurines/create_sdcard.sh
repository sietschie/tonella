#!/bin/bash
set -e

# prepare folders
mkdir -p sd_card/01 sd_card/02 sd_card/03 sd_card/04 sd_card/05 tmp_src tmp_dest

#prepare python environment
python3 -m venv .venv
source .venv/bin/activate
pip3 install pydub requests youtube-dl pyyaml scipy numpy joblib tqdm ipdb

./getaudio.py ../../figurines/*/*.yaml --path_src tmp_src --path_dest tmp_dest
cp tmp_dest/*-nameandsound.mp3 sd_card/03
cp tmp_dest/*-song.mp3 sd_card/02
cp tmp_dest/*-story.mp3 sd_card/04
cp tmp_dest/*-metal.mp3 sd_card/05
./create_beep.py --path_dest tmp_dest
ffmpeg -y -i tmp_dest/003-beep.wav sd_card/01/003-beep.mp3
ffmpeg -y -i tmp_dest/004-beeps.wav sd_card/01/004-beeps.mp3
./create_command_sounds.py --path_dest sd_card/01
