#!/usr/bin/env python3

'''
Read figurine yaml file. Download sound and song from youtube.
Generate mp3 with spoken name using tts. Normalize all mp3s.
Combine name and sound into one mp3.
'''

import sys
import requests
import json

import youtube_dl
import pydub
import yaml


def main(path_to_config):
    config = yaml.load(open(path_to_config,'r'), Loader=yaml.BaseLoader)

    sound = get_from_youtube(config['sound']['link'], config['name'], 'sound', config['sound']['selection'])
    song = get_from_youtube(config['song']['link'], config['name'], 'song', config['song']['selection'])
    name = get_name_from_tts(config['name_tts'], config['name'])

    (name_norm + pydub.AudioSegment.silent(duration=500) + sound_norm).export(name_file + "-nameandsound.mp3", format="mp3")


def get_from_youtube(link, name, suffix, selection):
    download_from_youtube(link, name + '-' + suffix + '.mp3')
    sound = pydub.AudioSegment.from_file(name + '-' + suffix + '.mp3')
    if selection:
        sound = sound[selection[0] * 1000: selection[1] * 1000]
    sound_norm = match_target_amplitude(sound, -20.0)
    sound_norm.export(name_file + "-" + suffix + "-normalized.mp3", format="mp3")
    return sound_norm

def get_name_from_tts(name_tts, name):
    download_name(name_tts, name + '-name.mp3')
    name = pydub.AudioSegment.from_file(name + '-name.mp3')
    name_norm = match_target_amplitude(name, -20.0)
    name_norm.export(name_file + "-name-normalized.mp3", format="mp3")
    return name_norm

def download_from_youtube(link, filename):
    ydl_opts = {
        'format': 'bestaudio/best',
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'mp3',
            'preferredquality': '192',
        }],
        'outtmpl':filename,
    }

    youtube_dl.YoutubeDL(ydl_opts).download([link])

def download_name(name, filename):
    headers = {
        'Connection': 'keep-alive',
        'Sec-Fetch-Dest': 'empty',
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.116 Safari/537.36',
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

    response = requests.post('https://ttsmp3.com/makemp3_new.php', headers=headers, data=data)

    url = json.loads(response.text)['URL']
    myfile = requests.get(url)
    open(filename, 'wb').write(myfile.content)

def match_target_amplitude(sound, target_dBFS):
    change_in_dBFS = target_dBFS - sound.dBFS
    print("change_in_dBFS = ", change_in_dBFS)
    return sound.apply_gain(change_in_dBFS)


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print("usage: {0} path_to_yaml_file".format(sys.argv[0]))
