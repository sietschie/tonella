#!/usr/bin/env python3

"""
Create audio content for figurines.

Read figurine yaml file. Download sound and song from youtube.
Generate mp3 with spoken name using tts. Normalize all mp3s.
Combine name and sound into one mp3.
"""

import json
import os.path
import argparse
import requests
import multiprocessing

import youtube_dl
import pydub
import yaml
import joblib
import tqdm


def create_mp3s(path_to_config, path_src=".", path_dest="."):
    """Create figurine mp3s."""
    config = yaml.load(open(path_to_config, 'r'), Loader=yaml.BaseLoader)

    prefix = "%03d-" % (int(config['id'])) + config['name']
    nameandsound_path = os.path.join(path_dest, prefix + "-nameandsound.mp3")
    if not os.path.isfile(nameandsound_path):
        name = get_name_from_tts(config['name_tts'], config['name'], path_src)
        if config['sound']['link']:
            sound = get_from_youtube(config['sound']['link'],
                                     config['name'] + '-sound',
                                     config['sound']['selection'],
                                     path_src)
            silence = pydub.AudioSegment.silent(duration=500)

            nameandsound = name + silence + sound
        else:
            nameandsound = name
        nameandsound.export(nameandsound_path)

    song_path = os.path.join(path_dest, prefix + '-song.mp3')
    if not os.path.isfile(song_path):
        song = get_from_youtube(config['song']['link'],
                                config['name'] + '-song',
                                config['song']['selection'],
                                path_src)
        song.export(song_path)

    story_path = os.path.join(path_dest, prefix + '-story.mp3')
    if not os.path.isfile(story_path):
        story = get_from_youtube(config['story']['link'],
                                 config['name'] + '-story',
                                 config['story']['selection'],
                                 path_src)
        story.export(story_path)

    metal_path = os.path.join(path_dest, prefix + '-metal.mp3')
    if not os.path.isfile(metal_path):
        metal = get_from_youtube(config['metal']['link'],
                                 config['name'] + '-metal',
                                 config['metal']['selection'],
                                 path_src)
        metal.export(metal_path)


def get_from_youtube(link, name, selection, path_src):
    """
    Download mp3 from youtube.

    Downloads a video from youtube, convert it to mp3,
    cuts it, if neccessary and saves it to disk.

    link -- youtube link
    name -- prefix of mp3 filename
    selection -- list with start end endpoint in seconds
    """
    youtube_id = link.split('=')[-1]
    filename = os.path.join(path_src, name) + "_" + youtube_id

    if not os.path.isfile(filename + ".mp3"):
        print(filename, link)
        counter = 0
        success = False
        while not success and counter < 5:
            try:
                print('try', counter, filename, link)
                download_from_youtube(link, filename)
                success = True
            except youtube_dl.utils.DownloadError as e:
                counter += 1
        if not success:
            raise Exception()
    else:
        print("%s already exists, skip downloading" % (filename + ".mp3"))

    sound = pydub.AudioSegment.from_file(filename + '.mp3')
    if selection:
        sound = sound[int(selection[0]) * 1000: int(selection[1]) * 1000]
    sound_norm = match_target_amplitude(sound, -20.0)
    return sound_norm


def get_name_from_tts(name_tts, name_file, path_src):
    """
    Convert text to speech mp3.

    name_tts -- text to be spoken by the tts engine
    name_file -- prefix for the mp3 file
    """
    filename = os.path.join(path_src, name_file + '-name.mp3')
    if not os.path.isfile(filename):
        download_name(name_tts, filename)
    else:
        print("%s already exists, skip downloading" % filename)

    name = pydub.AudioSegment.from_file(filename)
    name_norm = match_target_amplitude(name, -20.0)
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
        'outtmpl': filename + ".tmp",
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


def match_target_amplitude(sound, target_dbfs):
    """
    Normalize sound to specified dBFS value.

    sound -- input AudioSegment
    target_dBFS -- target volume
    """
    change_in_dbfs = target_dbfs - sound.dBFS
    print("change_in_dBFS = ", change_in_dbfs)
    return sound.apply_gain(change_in_dbfs)


def main():
    """Loop over all yaml files and creates mp3s."""
    parser = argparse.ArgumentParser(
                description='Create tonella mp3s ready for sd card')
    parser.add_argument('path_to_yaml_file',
                        help='yaml file with figurine config',
                        nargs="+")
    parser.add_argument('--path_src',
                        help='folder where source mp3s are cached',
                        default=".")
    parser.add_argument('--path_dest',
                        help='folder where final mp3s are saved',
                        default=".")

    args = parser.parse_args()

    for yaml_file in args.path_to_yaml_file:
        print("process %s" % yaml_file)
        create_mp3s(yaml_file, args.path_src, args.path_dest)


if __name__ == "__main__":
    main()
