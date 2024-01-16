#!/usr/bin/env bash
set -euo pipefail

script_path="$(readlink -f "${0}")"
script_dir="${script_path%/*}"

cd "${script_dir}/.."

declare -a required_files=("opus_file.ogg" "vorbis_file.ogg" "wav_file.wav")
declare -a missing_files=()
for f in "${required_files[@]}"; do
    if [ ! -f "${f}" ]; then
        missing_files+=("${f}")
    fi
done
if [ "${#missing_files[@]}" -ne 0 ]; then
    echo "Required files are missing: ${missing_files[@]}"
    exit 1
fi

echo "# Examples"
. "${script_dir}/examples.txt" 2>&1 | awk -f "${script_dir}/process_bash_trace.awk" | sed 's/.\+/    &/'

echo ""
echo "# Error conditions"
set +e
. "${script_dir}/errors.txt" 2>&1 | awk -f "${script_dir}/process_bash_trace.awk" | sed 's/.\+/    &/'
