
Debian
====================
This directory contains files used to package glynod/glyno-qt
for Debian-based Linux systems. If you compile glynod/glyno-qt yourself, there are some useful files here.

## glyno: URI support ##


glyno-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install glyno-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your glyno-qt binary to `/usr/bin`
and the `../../share/pixmaps/glyno128.png` to `/usr/share/pixmaps`

glyno-qt.protocol (KDE)

