# EQ4MOC

Equalizer for **Music On Console (MOC)** developed in **C++** using a terminal-based interface powered by **ncurses**.

EQ4MOC combines the classic UNIX philosophy with a configurable and persistent user experience, allowing users to manage equalizer profiles directly from the Linux terminal.

---

## Features

* Terminal interface built with ncurses.
* Integration with Music On Console (MOC).
* Lightweight and fast equalizer.
* Save and load presets.
* Custom preset editing.
* Persistent configuration support.
* Color theme support.
* Low resource consumption.
* Portable and minimalist application.
* Fully developed in C++.

---

## Screenshots

![Screenshot_20241205_014534](https://github.com/user-attachments/assets/5a6c3d18-4c08-4520-b281-ed44bff7ee8b)

![Screenshot_20241205_014534](https://github.com/user-attachments/assets/7da86642-fa2e-44b9-9d71-04a2e831d9d8)

---

## Technologies Used

* C++
* ncurses
* Linux
* Music On Console (MOC)

---

## Functionality

### Persistent Presets

Equalizer profiles can be saved and automatically reloaded, allowing users to reuse custom audio configurations for different music styles.

### Theme System

The interface supports multiple color themes to customize the terminal experience.

### Retro UNIX-Style Interface

Inspired by classic console applications:

* fast,
* efficient,
* direct,
* and free from unnecessary dependencies.

---

## Requirements

* Linux
* g++
* ncurses
* meson (optional)
* Music On Console (MOC)

---

## Compilation

### Compiling with Meson

If you have Meson installed, you can build EQ4MOC with

```
meson build
cd build
ninja
ninja install
```

### Compiling with provided `makefile`

You will need normal build tooling installed such as make and g++.
You will also need the ncurses development libraries.

     apt install libncurses-dev

 Normal compilation in /usr/local:

    make

    sudo make install

### Precompiled binaries

If you are using a Debian-like distro, you can try using these precompiled binaries
(both were generated with checkinstall):

[https://github.com/canuconde/EQ-for-MOC/releases/download/v1.0/eq4moc_1.1-1_amd64.deb](https://github.com/canuconde/EQ-for-MOC/releases/download/v1.0/eq4moc_1.1.1-1_amd64.deb)

https://github.com/canuconde/EQ-for-MOC/releases/download/v1.0/eq4moc_1.0-1_i386.deb

---

## Running

```bash id="uw04h"
./eq4moc
```

---

## Project Philosophy

EQ4MOC follows the spirit of classic Linux software:

* small and specialized tools,
* keyboard-driven workflow,
* fast execution,
* local persistence,
* and minimal complexity.

It avoids heavy frameworks and unnecessary modern UI layers.

---

## Possible Future Improvements

* Real-time spectrum visualizer.
* Import/export preset system.
* Compatibility with additional audio players.
* External configuration files.
* User-customizable themes.
* Configurable keyboard shortcuts.

---

## Project Goals

This project was created to:

* practice terminal UI programming in C++,
* explore ncurses,
* experiment with audio applications,
* and develop real tools for Linux environments.

---

## License

This project is distributed under the GNU General Public License v3.0 (GPLv3).

This means the software can be:

* freely used,
* modified,
* redistributed,
* and improved,
  as long as derivative works remain under the same GPLv3 license.

For more information:
[https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html)
