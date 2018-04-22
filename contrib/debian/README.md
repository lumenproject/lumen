
Debian
====================
This directory contains files used to package lumend/lumen-qt
for Debian-based Linux systems. If you compile lumend/lumen-qt yourself, there are some useful files here.

## lumen: URI support ##


lumen-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install lumen-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your lumen-qt binary to `/usr/bin`
and the `../../share/pixmaps/lumen128.png` to `/usr/share/pixmaps`

lumen-qt.protocol (KDE)

