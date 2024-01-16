#!/usr/bin/env bash
set -euo pipefail

script_path="$(readlink -f "${0}")"
script_dir="${script_path%/*}"

cd "${script_dir}/.."

sox -r44.1k -n -b16 "wav_file.wav" synth 10 sin 220 norm -10
opusenc "wav_file.wav" "opus_file.ogg"
oggenc "wav_file.wav" -o "vorbis_file.ogg"
