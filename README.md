# Myritone

A microtuning file format based off of [Scala files](https://www.huygens-fokker.org/scala/scl_format.html), but with a few changes:
* Directly express equal divisions of the octave with a\b = a/b of an octave
* Express equal divisions of other just intervals with a\b\<c/d> = a/b of interval c/d
* Use intervals with numerators and denominators that have up to 20 decimal digits (a maximum of 2^64 - 1, or about 18.4 quintillion)

Even chunkier just intervals and no more pesky decimal cents for EDOs, Bohlen-Pierce, and more! This additional syntax was taken from [Scale Workshop](https://scaleworkshop.plainsound.org).

Myritone was designed so that the Scala format would be forwards-compatible with it. It's also similar to a subset of the more powerful [SonicWeave language](https://github.com/xenharmonic-devs/sonic-weave).

The program in this repository is a command line tool that reads in a scale in the Myritone format and converts it to Scala. My goal is to add more output formats for this program, like AnaMark .tun, Korg 'logue, MTS SysEx, and so on.

The name "Myritone" comes from the words "myriad" and "tone", hence the meaning, "a myriad of tones."

## Building

On Linux:
- Clone this repository with "git clone \<this repository>"
- Add folder to root directory called "build"
- With the terminal in the root directory of the cloned repository, type "make"

On macOS and other Unix-like OSes:
- likely the same or similar as Linux, but I don't have a Mac

On Windows:
- I don't know about Makefile on Windows
- Perhaps use your C compiler of choice and manually compile in cmd or PowerShell with a command or compile with Visual Studio or VSCode

## Running

On Linux and (likely) macOS, etc.:
- Enter the command `./build/myritone -i <input_file.file_extension> [type-dependent options (more info below)] -o <output_file.file_extension>`
    - `-i` is required
    - `-o` is optional (Myritone program prints to terminal if not given)
    - Valid file extensions and their options when used for the output file:
        - `ascl` (Ableton Live scale)
            - `-p` (reference pitch in Hz) and `-n` (reference MIDI note number)
        - `myri` (Myritone)
        - `scl` (Scala)
        - `swi` (SonicWeave Interchange)
        - `syx` (SysEx MTS)
            - `-p` (reference pitch in Hz), `-n` (reference MIDI note number), and `-I` (preset index)
        - `tun` (AnaMark v1 or v2)
            - `-p` (reference pitch in Hz) `-n` (reference MIDI note number), and `-V` (version 1 or 2)
- A test scale file is included as an input file example

Not tested on Windows
