# Orange Pi PC LCD bandwidth monitor

`orangepipc-lcd-bandwidth-monitor` is a CLI software used to display download and upload speed (in KB/s) of a network interface, on a Nokia 5110 LCD display and made specifically for Orange Pi PC. A port of a PCD8544 arduino library is included, ported to work with WiringOP and used to interface with the screen.

![IMG_20200429_153321.jpg](IMG_20200429_153321.jpg)

## Installation

Copy executable to /usr/bin and you're good to go. As for how to connect the pins to the screen, a mapping is provided by defines in main.cpp.

A precompiled binary is available in the releases page for usage on Orange Pi PC, or else you can refer to the building instructions below.


## Building

Dependencies: 

* `WiringOP` from [Xunlong's official repository](https://github.com/orangepi-xunlong/wiringOP)
* `PCD8544.h` included in this repository

```
$ git clone https://github.com/neeedle/orangepipc-lcd-bandwidth-monitor.github
$ cd orangepipc-lcd-bandwidth-monitor
$ make
```

## Usage

```$ orangepipc-lcd-bandwidth-monitor -i interface [-d interval] [-b] [-h]```

## Options

- `-i`: Interface name (eth0, tun0, etc.)
- `-d`: (Optional) Screen update delay, in milliseconds. Defaults to 1000 (1 second)
- `-b`: (Optional) Turn backlight on if present. Off by default
- `-h`: Print usage.

## Licenses

* GPLv3 for PCD8544 library
* MIT License for main.cpp
