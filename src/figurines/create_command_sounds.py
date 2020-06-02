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

    download_name("Lied", join(args.path_dest, "001-TextLied.mp3"))
    download_name("Geräusch", join(args.path_dest, "002-TextGeräusch.mp3"))


if __name__ == "__main__":
    main()
