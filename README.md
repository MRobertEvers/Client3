# RuneScape 2 revision #225 (18 May 2004) C99 port
Portable single-threaded C client for early RS2, the last update before a new cache format and ondemand protocol.

Compatible with [2004Scape](https://github.com/2004Scape/Server), the most accurate runescape remake

Features:
- should work on any 32 bit system with 64 MB of RAM on lowmem, networking and a (read-only) filesystem.
- webassembly build to avoid javascript code being optimized out by the browser.
- WIP ports for most game consoles from 1998 until 2013! Ones with incomplete input are set up to auto-connect.
- optional [config.ini](config.ini.example) file to change client behaviour. Create an empty config.ini to avoid passing cli args.

See [docs](/docs) for more info and media.

## disclaimer
During limited playtesting the client seems to be stable, but it could crash at any time! Run sanitizer or tcc debug backtrace build to share errors

See [known issues](#known-issues) before reporting an issue.

## quickstart for windows
All you need to build for 32 bit windows is included:
* tinycc (C compiler, built with `TCC_C=..\tcc.c` env var and some removed libs for smaller distribution)
* libeay32.dll for faster rsa https://gnuwin32.sourceforge.net/packages/openssl.htm
* all 32 bit SDL dlls, only SDL1 works prior to windows XP and is always 32 bit unlike the others

To build simply run `build.bat` in cmd to get the client.exe, it depends on the cache + SDL.dll, libeay32.dll, and optionally .sf2 soundfont/config.ini

`build.bat -h` shows options, EG `-v 1|2|3` sets SDL version and `-c tcc|gcc|emcc` for system tcc, gcc, emcc

Both tcc and old mingw-gcc can target windows 9x with SDL 1. This (unofficial) release doesn't require msys install: https://github.com/fsb4000/gcc-for-Windows98/releases. mingw-gcc 11 optimizations seem to only be slightly faster than tcc though.

type `::perf` command ingame to see fps and lrucache size

### Troubleshooting:
- If the client fails to start it most likely means you are using a SDL dll for the wrong architecture. Delete them and it should copy during next build
- If the server changes cache it has to be manually updated in cache/ directory for now (still happens if new mapdata is found). Crc is disabled for maps and archives for time being.

### Changing between worlds (aka nodeid) using config.ini:
1. You first have to put a `#` in front of all lines that contain the fields below to ignore them
```
# nodeid = 1
# portoff = 0
# server = localhost
# http_port = 80
```
2. Then remove the `#` from the grouped fields of the world you want to access, some worlds don't need all fields and use the defaults.

## known issues
```
server cache changes would require manual cache update in client for now, it isn't supposed to change but as of right now there's an issue with client map crcs being changed when server maps get updated (also the cache has some interface changes rn for quest tab and another one) enable crc again after fixes, search "wait authentic cache". Maybe also use emscripten indexeddb api to store data file?

figure out rsaenc bug(s), i'm thinking there are multiple issues (chance of failing login), RSA_BIGINT can still fail due to wrong enc length, but the others fail due to other reasons too... Also fix connecting on desktop to servers with higher than 512 bit rsa, need bigger result array but not for web?

Recompile is needed to change between different RSA key lengths, RSA_BUF_LEN needs to be set at compile time because it's needed for stack allocated arrays and BN_ARRAY_SIZE define. Can just heap allocate and leave tiny-bignum at 512 bits rsa only to fix this?

no midi fading, old js code for IE: https://github.com/2004Scape/Server/blob/61bf21fb3755c14b5cf6d47c9d974dee5783beda/view/javaclient.ejs new ts code: https://github.com/2004Scape/Client2/commit/92e74f1f134ea82e48dd608dcca3422777a7a986 (client-ts has more some fade fixes)

locs like fires have no animations as pushLocs is disabled for now, it constantly allocates memory due to always calling model_copy_faces in loctype which requires a different approach. The leaks get worse if the dynamic model cache can't fit all sequences (animations) of the models in an area, disable the allocator to see origins.

wordfilter isn't ported yet, so you will see your own swear words but others don't as it gets filtered by the server still.

the game uses 3 titles: "RuneScape - the massive online adventure game by Jagex Ltd" (website), "RuneScape Game" (html) and "Jagex" (jar), maybe show the first as it was most commonly seen

some bits from signlink missing (uid, reporterror, findcachedir, openurl, opensocket etc, move map loading to cacheload?

remove the refcounting from model/pix24/lrucache for components and do smth else (kept to avoid leak spam rn) as components get assigned models from packets which are put into lrucaches, so global component doesn't own the memory anymore

there are a few more small memleaks to work out, but they shouldn't become a problem outside of very memory constrained environments. Also make sure playground doesn't leak anymore after attempting to fix this.
```

## non issues (expected or unfixable)
Windows Firefox "lag" is due to setTimeout being broken with wasm as using the firefox profiler increases fps. Asyncify is not the problem here I've had it happen in wasm without emscripten, https://github.com/lesleyrs/web-gbc?tab=readme-ov-file#limitations

The "fix" is to use `emscripten_set_main_loop_arg` with 0 fps which calls requestAnimationFrame instead. The function to be called should be the contents of the while loop in gameshell_run (delete the while loop itself) which will get you full FPS. If it still lags close devtools and refresh page. Downsides are that if the tab goes inactive it will speed up when returning, it doesn't seem fixable the same way as in Client2 since they still use setTimeout. Also Firefox won't have the benefit of the websocket never timing out anymore.

emrun causes extra batch job message on windows sigint, can swap it for `py -m http.server` or so to avoid it

## Java client
The 2004 jar is stored for comparisons, run with EG: `java -cp bin/runescape.jar client 10 0 highmem members` but:
- there is no audio, it saves audio files for the browser to play which is no longer applicable
- right clicking breaks past java 8
- window insets on modern systems are causing the sides of the game to be cut off slightly
- outside of windows it saves the cache to `/tmp` so every reboot you may have to redownload it
- it only connects to localhost if it's not running as applet
- server http port needs to be set to 80 (2004scape on linux defaults to 8888 right now to avoid sudo)
- TODO confirm: to connect to local java servers on WSL from Windows you might need to add `-Djava.net.preferIPv6Addresses=true` when running client

## Platforms and Compilers
TODO:
```
macos, bsds, android https://wiki.libsdl.org/SDL2/Android
+ add default helvetica-like system ttf font in gameshell_draw_string for each
```

### Windows 95 to Windows 11
build.bat(32 bit): tcc (included), mingw-gcc, emcc

run.ps1: cl, clang, tcc, mingw-gcc, emcc

You might want the updated [PowerShell](https://github.com/PowerShell/PowerShell) for run.ps1

NOTE: currently bignum lib isn't working with tcc on windows and gives [invalid memory access](https://lists.nongnu.org/archive/html/tinycc-devel/2024-12/msg00020.html) so we use openssl

TODO:
```
confirm win9x work still (with old openssl or mingw-gcc with libtom), maybe add screenshot to /docs
do we assume windows 95 has -lws2_32, otherwise re-add -lwsock32 and remove -DMODERN_POSIX in batch file
make win9x compatible batch file (no delayed expansion?) right now needs to build from more modern system
clean up ps1 script so it doesn't need to be modified
```

### Linux GNU or musl
Makefile: gcc, clang, tcc, mingw-gcc, emcc

If tcc from your package manager isn't working you should build latest [tcc](https://github.com/TinyCC/tinycc) from source

### Web (Emscripten)
install [emsdk](#tools)
run `emmake make`/`make CC=emcc` or `build.bat -c emcc` for windows

Linux wasm/js output seems to be quite a bit smaller than on Windows

If you pass args in the html file the ip address and http port are from the URL itself.

If not passing args make sure to set http_port to 8888 on linux (or whatever it's configured as in server).

```
TODO: possibly target wasm directly with clang instead of emscripten, but then we don't have a libc at all
TODO: emscripten wasm on firefox has memleaks related to midi, gets cleaned up by pressing GC in about:memory but why does this happen? Chromium based browsers are ok. Happens on both SDL2 and SDL3.
TODO: auto-generated js by emscripten is blocking default browser shortcuts why exactly
```

### Consoles
If files aren't built into rom, you have to move the `cache/`, `Roboto/`, `config.ini` and optionally `SCC1_Florestan.sf2` to sdcard.

```
TODO: add different client entrypoints based on screen resolution? or move coordinates to defines.h (error prone) needs experimenting to see what works.
TODO: check dnslookup for consoles: vita, psp, dreamcast, nds
```

### Nintendo consoles (devkitPro)
Install [devkitpro](#tools) with (wii/3ds/wiiu/switch)-dev package and run `make -f (wii/3ds/wiiu/switch).mk -j$(nproc) -B`.

Wii U and Switch also need the (wiiu/switch)-sdl2 package.

If you own a console and want to improve a port look at rsc-c for reference: https://github.com/2003scape/rsc-c

#### NDS
Necessary files are built into rom.
```
with 32 mb ram gba expansion pak? + 4MB system ram
original game will probably run at 1 fps if it works at all
would need major changes to run smoothly, need separate client entrypoint for it
```

#### Wii
in dolphin emulator you can find the sdcard path in `options>configuration>wii>sd card` settings and after moving the files there you have to click `Convert Folder to File Now` to format it.

Controls: wiimote IR pointer works as mouse, A for left click, B for right click, Dpad works as arrow keys, 1 for control, home button to exit.

```
TODO: need 2 framebuffers to a show cursor on screen without flickering or you won't see where you are aiming (dolphin mouse is slightly offset)
TODO: lowmem is recommended, highmem only barely fits in wii's memory and crashes quite fast. Might become better if the last small leaks are fixed.
TODO: virtual keyboard to type, for now set user and pass in config.ini, audio, add game offset on real HW?, support usb keyboard and mouse on wii
```

#### 3DS
in citra emulator click `file>open citra folder` for sdmc dir https://citra-emulator.com/wiki/user-directory/

Controls: Touch to left click, L + touch to right click, Dpad for arrow keys, X for control

```
TODO: undo touch changes in 3ds.c depending how it works on real hardware (vita seems to work fine on hw)
TODO: The "New" 3ds/2ds line for higher cpu clock rate does not seem to make much difference in citra?
```

#### Wii U
in cemu emulator click `file>open mlc folder`, go 1 directory up to see sdcard dir

```
TODO: Touch input not working yet, might be fixed by last wiiu-sdl2 commit.
NOTE: there's weird issues: highmem seems to not start due to tinysoundfont tsf_load failing, and libtom encryption fails (tiny-bignum is ok)
```

#### Switch
in suyu emulator (yuzu fork) click `file->open suyu folder` for sdmc dir

### Sony PSP
Install [pspdev](#tools) and run `make -f psp.mk -j$(nproc) -B`.

ppsspp emulator loads relative dir as memstick, so the filesystem works automatically. Also you should probably enable printf logging with `settings>tools>developer tools>logging channels>printf` to EG verbose

Controls: move cursor with analog stick, O for left click, X for right click, /\ for control, Dpad as arrow keys

Works on real hardware but requires at least model 2000 due to the full 32 MB not being accessible, only lowmem fits into memory so we force lowmem in custom.c

```
TODO: Model 1000 has the same CPU just less memory, might be worth trying to make it work in kernel mode for 28MB BUT is it safe to do so?
TODO: Could enable audio in lowmem for 2000+ models if there's enough memory remaining?
```

### Sony PS Vita
Install [vitasdk](#tools) and run `make -f vita.mk -j$(nproc) -B`. Add `SDL=0` to use vita.c

on vita3k emulator to avoid installing the .vpk each change just copy the eboot.bin, and non-blocking networking causes connect fail on windows same as 3ds.

Controls: touch as mouse, X for right click, /\ for control, Dpad as arrow keys

```
TODO: draw game at offset to center it, and offset touch input based on that.
TODO: touch input on the back, confirm touch works correctly
TODO: update sce_sys assets
TODO: allow setting sdl version to 3 in makefile when it works
TODO: check if vita.c native touch fixes minimap offset, and make it playable
```

### Sega Dreamcast
Install [kallistios and mkdcdisc](#tools) and run `make -f dreamcast.mk -j$(nproc) -B`. Necessary files are built into rom.

To try on real hardware you'd need the 32 MB ram expansion mod, which seems involved and maybe less compatible with some other games

Flycast seems to be the best emulator, supports ram expansion and networking. For mouse and keyboard you have to set the physical device ports to dreamcast device port.

Not fully playable due to limited RAM, you can only load areas that use 2 MB of space in lrucaches (wildy etc)

Controls: Mouse/arrow keys. The SDL1 mouse seems to be overly sensitive and the mouse in samples is offset, maybe change to controller

```
NOTE: local servers don't work on emulator? only live works
TODO: fopen path was changed due to the mkdcdisc tool adding dots to files without extension https://gitlab.com/simulant/mkdcdisc/-/issues/14
TODO: check if it's making use of vram currently, dreamcast vram is quite big (8mb)
TODO: instead of INIT_DEFAULT choose the flags we want to use
```

### Microsoft Xbox
Install [nxdk](#tools) and run `make -f xbox.mk -j$(nproc) -B`. Necessary files are built into rom.

Controls: TODO none yet, it just automatically logs in

```
NOTE: local servers don't work on emulator? only live works
NOTE: default.xbe stays around in rom dir when it's junk for other consoles that embed that directory. Can remove it after building.
TODO: fopen had to be separated due to the need for backwards slashes, also there's no chdir equivalent?
```

## libraries
* [micro-bunzip](https://landley.net/code/) | https://landley.net/code/bunzip-4.1.c
* [isaac](https://burtleburtle.net/bob/rand/isaacafa.html) | https://burtleburtle.net/bob/c/readable.c
* [tiny-bignum-c](https://github.com/kokke/tiny-bignum-c)
* [ini](https://github.com/rxi/ini)
* [TinySoundFont](https://github.com/schellingb/TinySoundFont) - with fix for attack1.mid by skipping RIFF header
* [stb_image and stb_truetype](https://github.com/nothings/stb)

## optional libraries
* [OpenSSL](https://github.com/openssl/openssl) | https://wiki.openssl.org/index.php/Binaries

32 bit libeay32.dll from: [gnuwin32](https://gnuwin32.sourceforge.net/packages/openssl.htm)

emcc libcrypto.a from: [get-openssl-wasm.sh](get-openssl-wasm.sh)

* [LibTomMath](https://github.com/libtom/libtommath) | https://github.com/libtom/libtommath/releases/latest

libtommath releases include a gen.pl script to generate a single mpi.c file from the whole source

* [SDL-1.2](https://github.com/libsdl-org/SDL-1.2) | [SDL-2/SDL-3](https://github.com/libsdl-org/SDL) | https://libsdl.org/release/

Using prebuilt SDL releases but removed tests dir, removed dotfiles from mingw SDL1 and fixes in VC SDL1 for tcc.
Latest SDL1 already contains the tcc fix but they don't make new releases for it.

## tools
* [tcc](https://github.com/TinyCC/tinycc) | https://bellard.org/tcc/
* [emsdk](https://github.com/emscripten-core/emsdk) | https://emscripten.org/docs/getting_started/downloads.html
* [devkitpro](https://github.com/devkitPro) | https://devkitpro.org/
* [pspdev](https://github.com/pspdev/pspdev) | https://pspdev.github.io/
* [vitasdk](https://github.com/vitasdk/vdpm) | https://vitasdk.org/
* [kallistios](https://github.com/KallistiOS/KallistiOS) | [mkdcdisc](https://gitlab.com/simulant/mkdcdisc)
* [nxdk](https://github.com/XboxDev/nxdk)
