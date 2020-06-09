#!/usr/bin/env python3

"""Create audio files for mode change feedback."""

import argparse
from os.path import join
from getaudio import download_name


def main():
    """Create two mp3s to announce current mode."""
    parser = argparse.ArgumentParser(
                description='Create command sounds and save to mp3 file.')
    parser.add_argument('--path_dest',
                        help='folder where final mp3s are saved',
                        default=".")

    args = parser.parse_args()

    path_song = join(args.path_dest, "001-TextLieder.mp3")
    download_name("Lieder", path_song)

    path_sound = join(args.path_dest, "002-TextGeräusche.mp3")
    download_name("Namen und Geräusche", path_sound)

    path_story = join(args.path_dest, "005-TextGeschichten.mp3")
    download_name("Geschichten", path_story)

    path_metal = join(args.path_dest, "006-TextMetal.mp3")
    download_name("Metall", path_metal)


if __name__ == "__main__":
    main()
