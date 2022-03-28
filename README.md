# Niall's Own Delay

## Background

This project was influenced by an idea suggested by a good friend and musician, Niall. I started and procastinated on this project, but eventually started approaching it in a more serious way, treating it as a comprehensive exercise into the workflow of building a plugin, as well as an independent venture towards creating a sonically interesting effect. As of this initial documentation, I hope to bring in more features and improve the performance of the plugin in all aspects.

### Features

* A Feedback Delay Network Reverb with damping and "freeze" features.
* Adjustable Stereo Width.
* Delay with feedback and up to 1000ms timing controls.
* State Variable Filter that can be applied before or after delay processing.
* FFT based frequency shift to create recursive pitched movements with every delay.
* Frequency Modulation Post-Delay.

## System Requirements

* [CMake](https://cmake.org/download/) (>= v3.15)
* macOS 10.13+ (currently only tested with this configuration but welcoming contributers to )

## Cloning

This project uses submodules for [JUCE](https://github.com/juce-framework/JUCE), [googletest](https://github.com/google/googletest), and a fork of [react-juce](https://github.com/wccharles/react-juce), so you may wish to clone this project recursively:

```bash
git clone https://github.com/wccharles/nialls-own-delay.git --recursive
```

Alternatively, you can clone as normal and enter the following command in your local repo directory:

```bash
git submodule update --init --recursive
```

## To Configure and Build

The intial step to building this project is via the command

```bash
cmake -B [BUILD_FOLDER] (-G [GENERATOR_NAME])
```

Unless you happen to be building on an IDE generator, (or you just love the command line) you can build the project like so:

```bash
cmake --build [BUILD_FOLDER] --target <targetName> (--config Release/Debug/...)
```

## Other Notes

The CMake configuration used is loosely based around the repo for [pamplejuce](https://github.com/sudara/pamplejuce), which provides a decent template for a DevOps friendly environment with JUCE projects.
