# Examples

    ]$ ./opusgainfield -h
    Usage:
      opusgainfield INPUT_FILE

      Mandatory:
        INPUT_FILE             Opus file in Ogg container.

      Optional:
        -a | --add-gain GAIN   Add GAIN to the value already stored in the file.
        -s | --set-gain GAIN   Set GAIN, replacing the value stored in the file.

        -h | --help            This help message

    Without optional arguments it prints current gain of the INPUT_FILE.

    The allowed values for gain stored in the file are floating point in range [-128, 127).

    ]$ ./opusgainfield opus_file.ogg
    gain: 0 dB

    ]$ ./opusgainfield opus_file.ogg -s 5.1
    changing gain from 0 dB to 5.1 dB

    ]$ ./opusgainfield opus_file.ogg
    gain: 5.09766 dB

    ]$ opusinfo opus_file.ogg | grep gain
    	Playback gain: 5.09766 dB

    ]$ ./opusgainfield opus_file.ogg -a 10
    changing gain from 5.09766 dB to 15.0977 dB

    ]$ ./opusgainfield opus_file.ogg
    gain: 15.0977 dB

    ]$ ./opusgainfield opus_file.ogg -a -20.2
    changing gain from 15.0977 dB to -5.10234 dB

    ]$ ./opusgainfield opus_file.ogg
    gain: -5.10547 dB

    ]$ ./opusgainfield opus_file.ogg -s -3
    changing gain from -5.10547 dB to -3 dB

    ]$ ./opusgainfield opus_file.ogg
    gain: -3 dB

# Error conditions

    ]$ ./opusgainfield vorbis_file.ogg
    Doesn't look like Opus file, wrong magic string.

    ]$ ./opusgainfield wav_file.wav
    Doesn't look like OGG file, wrong magic string.

    ]$ ./opusgainfield opus_file.ogg -s 128
    New gain: 128 out of range [-128.0, 127.0).

    ]$ ./opusgainfield opus_file.ogg -a 140
    New gain: 140 out of range [-128.0, 127.0).

    ]$ ./opusgainfield opus_file.ogg -s 123not_a_number
    opusgainfield: Invalid arguments for "--add-gain" or "--set-gain".

    Usage:
      [...]

    ]$ ./opusgainfield opus_file.ogg -s
    opusgainfield: Not enough arguments for "--add-gain" or "--set-gain".

    Usage:
      [...]

    ]$ ./opusgainfield opus_file.ogg -s 10 -a 20
    opusgainfield: Only one option that modifies the gain be used.

    ]$ ./opusgainfield opus_file.ogg another_file
    opusgainfield: Only one input file can be used.

    Usage:
      [...]

    ]$ ./opusgainfield not_existing
    not_existing: No such file or directory

    ]$ ./opusgainfield -s 3
    opusgainfield: Input file is missing.

    Usage:
      [...]
