#!/usr/bin/env python3

"""Create audio files for mode change feedback."""

from getaudio import download_name


def main():
    """Create two mp3s to announce current mode."""
    download_name("Lied", "001-TextLied.mp3")
    download_name("Geräusch", "002-TextGeräusch.mp3")


if __name__ == "__main__":
    main()
