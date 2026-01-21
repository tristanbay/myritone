# Myritone

A microtuning file format based off of [Scala files](https://www.huygens-fokker.org/scala/scl_format.html), but updated for integration in modern DAWs:
* Directly express equal divisions of the octave with a\b = a/b of an octave
* Express equal divisions of other just intervals with a\b\<c/d> = a/b of interval c/d
* Use intervals with numerators and denominators that have up to 20 decimal digits (a maximum of 2^64 - 1, or about 18.4 quintillion)
* Multi-channel support (retune the notes for multiple MIDI channels differently at the same time)
* Note names
* Note colors (2^24 or about 16.8 million different possible colors)

Even chunkier just intervals and no more pesky decimal cents for EDOs, Bohlen-Pierce, and more! This additional syntax was taken from [Scale Workshop](https://scaleworkshop.plainsound.org).

Myritone was designed so that the Scala format would be forwards-compatible with it. It's also similar to a subset of the more powerful [SonicWeave language](https://github.com/xenharmonic-devs/sonic-weave).

The program in this repository is a command line tool that reads a scale file in the Myritone format into an internal data structure and prints out the contents of that data structure.

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
- Enter the command `./build/myritone <input file (Myritone format)>`
- A test scale file is included as an input file example

Not tested on Windows
