# GTFO Mumble Plugin

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
  - [Manual Installation](#manual-installation)
  - [Automatic Installation](#automatic-installation)
- [Configuration](#configuration)
- [Usage](#usage)
  - [In-Game](#in-game)
  - [Troubleshooting](#troubleshooting)
- [Development](#development)
  - [Building from Source](#building-from-source)
  - [Contributing](#contributing)
- [Changelog](#changelog)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Overview

Brief description of what this plugin does, the problem it solves, and why it is useful for GTFO players.
This is a simple Mumble Plugin which provides positional audio for the game GTFO (https://gtfothegame.com/).
I'm a big fan of this game and I thought that adding this feature would be a great addition to the game and make it more immersive when playing with friends.

---

## Features

- Position Based Audio Scaled to Meters
- Proper Height Scaling
- Accurate Eye Direction

---

## Requirements

- Mumble 1.4+ (Tested on Mumble 1.4.230)
- Legal Copy of GTFO (Tested on Steam Build)
- Windows 10+ (Tested on Windows 10 22H2)

---

## Installation

### Manual Installation

1. Downloaded the latest release from the [Releases] page. Or build from source using CMake.
2. Drag and drop into the Mumble Plugins folder. (Default: `C:\Program Files\Mumble\client\plugins`)
3. Find 'GTFO' in the Mumble Plugins list and enable it.
4. Enable 'Link to Game and Transmit Position' and 'PA' in the Mumble Plugins settings.
5. Start GTFO and enjoy!

### Automatic Installation
1. Download the latest gtfo.mumble_plugin file from the [Releases] page.
2. Double click the gtfo.mumble_plugin file or use the 'Install Plugin' button in the Mumble Plugins settings.
3. Find 'GTFO' in the Mumble Plugins list and enable it.
---

## Configuration

To enjoy a more 'immersive' experience, tinker with the Positional Audio settings in the 'Audio Output' tab of the Mumble settings.
I find the following settings to be the most immersive:

[+] Headphones

Minimum Distance: 5.0m

Maximum Distance: 30.0m

Minimum Volume: 0%

---

## Usage

### In-Game

- No additional steps are required to use this plugin in-game.

### Troubleshooting

If you are having issues with the plugin, please check the following:

Ensure the game is not running with Administrative permissions, or run Mumble as Admin.

---

## Development

### Building from Source

1. Clone the repository
2. Coinfigure the build using CMake. Example: `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S <source_dir> -B <build_dir> -G "Visual Studio 17 2022" -T host=x64 -A x64`
3. Build the project using CMake. Example: `cmake --build <build_dir> --config Release --target ALL_BUILD`

### Contributing

If you would like to contribute please open a pull request with your changes.

Make sure to explain what problem you are solving and how you are solving it.

I will do my best to review and merge pull requests in a timely manner.

---

## Changelog

- v1.0.0
  - Initial Release
- v1.0.1
  - Add mumble manifest installation

---

## License

Find the license for this project [here](LICENSE).

---

## Acknowledgments

Thanks to GreenCheese333 for helping me test :D

---

