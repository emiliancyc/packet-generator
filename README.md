# packet-generator
This is repository for packet-generator source code and files.

# HOW TO

1. Use 'git clone' to download repository to your computer or download it from wrepository website as zip archive.
2. If you downloaded zip file, unpack it.
3. Change directory (cd) to qt/build-packet-generator-Desktop-Debug/
4. Run with sudo: sudo ./packet-generator.
5. If program doesn't run and terminal says about lacking Qt libraries, download qt5-default package, which includes all dynamic libraries needed for compile, run and change code with QtCreator.

Source files are stored in /qt/packet-generator.
Precise list of needed libraries (including Qt):
libQt5Widget.so.5
libQt5Gui.so.5
libQt5Core.so.5
libpthread.so.0
libstdc++.so.6
libgcc_s.so.1
libc.so.6

# Known problems:
1. If program crashes with Segmentation fault error, you;ve probably forgot about sudo priviledges. Sockets require root priviledges.
SOLUTION: Re-run with sudo or as root.

2. ./packet-generator: error while loading shared libraries: libQt5Widgets.so.5: cannot open shared object file: No such file or directory
SOLUTION: Download and install qt5-default package.
