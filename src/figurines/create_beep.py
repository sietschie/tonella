#!/usr/bin/env python3

"""Create beep sound effect for volume change feedback."""

import argparse
import os.path
from scipy.io.wavfile import write
from scipy.signal import convolve
import scipy.signal
import numpy as np

from numpy import linspace, sin, pi, int16


# tone synthesis
def note_sine(freq, len, amp=1, rate=44100):
    """Create sine wave."""
    t = linspace(0, len, len*rate)
    data = sin(2*pi*freq*t)*amp
    return data.astype(int16)


def note_square(freq, len, amp=1, rate=44100):
    """Create square wave."""
    t = linspace(0, len, int(len*rate))
    data = sin(2*pi*freq*t)*amp
    data[data > 0] = amp
    data[data <= 0] = -amp

    return data.astype(int16)


def envelope(tone, attack, decay, sustain, rate=44100):
    """Apply envelope to sound."""
    a = linspace(0, 1, int(attack*rate))
    d = linspace(1, sustain, int(decay*rate))
    s = linspace(sustain, sustain, len(tone) - len(a) - len(d))

    return (np.concatenate([a, d, s]) * tone).astype('int16')


def create_beep(duration):
    """Create beep sound."""
    tone = note_square(440, duration, amp=20000)

    gaussian = scipy.signal.windows.gaussian(50, 3)
    gaussian_norm = gaussian / np.sum(gaussian)
    tone_lowpass = convolve(tone, gaussian_norm)

    tone_enveloped = envelope(tone_lowpass, 0.05, 0.3, 0.0)

    return tone_enveloped


def main():
    """Create beep sounds and save to wav file."""
    parser = argparse.ArgumentParser(
                description='Create beep sounds and save to wav file.')
    parser.add_argument('--path_dest',
                        help='folder where final wavs are saved',
                        default=".")

    args = parser.parse_args()

    beep1 = create_beep(0.5)
    beep2 = create_beep(1.0)
    beeps = np.concatenate([beep2] + [beep1]*30)

    write(os.path.join(args.path_dest, '003-beep.wav'), 44100, beep1)
    write(os.path.join(args.path_dest, '004-beeps.wav'), 44100, beeps)


if __name__ == "__main__":
    main()
