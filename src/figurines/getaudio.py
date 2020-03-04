#!/usr/bin/env python3

"""
Create audio content for figurines.

Read figurine yaml file. Download sound and song from youtube.
Generate mp3 with spoken name using tts. Normalize all mp3s.
Combine name and sound into one mp3.
"""

import sys
import requests
import json

import youtube_dl
import pydub
import yaml


def main(path_to_config):
    """Create figurine mp3s."""
    config = yaml.load(open(path_to_config, 'r'), Loader=yaml.BaseLoader)

    sound = get_from_youtube(config['sound']['link'],
                             config['name'] + '-sound',
                             config['sound']['selection'])
    song = get_from_youtube(config['song']['link'],
                            config['name'] + '-song',
                            config['song']['selection'])
    name = get_name_from_tts(config['name_tts'], config['name'])
    silence = pydub.AudioSegment.silent(duration=500)

    nameandsound = name_norm + silence + sound_norm
    nameandsound.export(name_file + "-nameandsound.mp3", format="mp3")


def get_from_youtube(link, name, selection):
    """
    Download mp3 from youtube.

    Downloads a video from youtube, convert it to mp3,
    cuts it, if neccessary and saves it to disk.

    link -- youtube link
    name -- prefix of mp3 filename
    selection -- list with start end endpoint in seconds
    """
    download_from_youtube(link, name + '.mp3')
    sound = pydub.AudioSegment.from_file(name + '.mp3')
    if selection:
        sound = sound[selection[0] * 1000: selection[1] * 1000]
    sound_norm_filename = name + "-normalized.mp3"
    sound_norm = match_target_amplitude(sound, -20.0)
    sound_norm.export(sound_norm_filename, format="mp3")
    return sound_norm


def get_name_from_tts(name_tts, name):
    """
    Convert text to speech mp3.

    name_tts -- text to be spoken by the tts engine
    name -- prefix for the mp3 file
    """
    download_name(name_tts, name + '-name.mp3')
    name = pydub.AudioSegment.from_file(name + '-name.mp3')
    name_norm = match_target_amplitude(name, -20.0)
    name_norm.export(name_file + "-name-normalized.mp3", format="mp3")
    return name_norm


def download_from_youtube(link, filename):
    """
    Download mp3 from youtube and save to disk.

    link -- youtube link
    filename -- where to save mp3
    """
    ydl_opts = {
        'format': 'bestaudio/best',
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'mp3',
            'preferredquality': '192',
        }],
        'outtmpl': filename,
    }

    youtube_dl.YoutubeDL(ydl_opts).download([link])


def download_name(name, filename):
    """
    Convert text to speech.

    name -- text to convert
    filename -- where to save mp3
    """
    headers = {
        'Connection': 'keep-alive',
        'Sec-Fetch-Dest': 'empty',
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 '
                      '(KHTML, like Gecko) Chrome/80.0.3987.116 Safari/537.36',
        'Content-type': 'application/x-www-form-urlencoded',
        'Accept': '*/*',
        'Origin': 'https://ttsmp3.com',
        'Sec-Fetch-Site': 'same-origin',
        'Sec-Fetch-Mode': 'cors',
        'Referer': 'https://ttsmp3.com/text-to-speech/German/',
        'Accept-Language': 'en-US,en;q=0.9,de;q=0.8',
    }

    data = {
      'msg': name,
      'lang': 'Vicki',
      'source': 'ttsmp3'
    }

    response = requests.post('https://ttsmp3.com/makemp3_new.php',
                             headers=headers, data=data)

    url = json.loads(response.text)['URL']
    myfile = requests.get(url)
    open(filename, 'wb').write(myfile.content)


def match_target_amplitude(sound, target_dBFS):
    """
    Normalize sound to specified dBFS value.

    sound -- input AudioSegment
    target_dBFS -- target volume
    """
    change_in_dBFS = target_dBFS - sound.dBFS
    print("change_in_dBFS = ", change_in_dBFS)
    return sound.apply_gain(change_in_dBFS)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print("usage: {0} path_to_yaml_file".format(sys.argv[0]))
