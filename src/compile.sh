#!/bin/sh

DESTDIR="../Need For Speed II SE"

i686-w64-mingw32-gcc dsetup.c -O2 -s -shared -Wl,--kill-at -DNDEBUG -o "$DESTDIR/dsnfs.dll"

i686-w64-mingw32-gcc ddraw.c -O2 -s -shared -Wl,--kill-at -lSDL2 -DNDEBUG -DDIRECTDRAW_VERSION=0x500 -o "$DESTDIR/ddnfs.dll"

yasm -f win32 --prefix=_ eacsnd.asm
i686-w64-mingw32-gcc eacsnd.c eacsnd.obj -O2 -s -shared -Wl,--kill-at -Wl,--enable-stdcall-fixup -lSDL2 -DNDEBUG -o "$DESTDIR/eacnfs.dll"
rm -f eacsnd.obj

bspatch "$DESTDIR/NFS2SEN.EXE" "$DESTDIR/nfs2sen-patched.exe" nfs2sen-delta.bsdiff
