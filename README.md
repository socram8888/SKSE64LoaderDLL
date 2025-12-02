
SKSE64 Loader DLL
=================

This is a plugin for the [SSE Engine Fixes](https://www.nexusmods.com/skyrimspecialedition/mods/17230)
Preloader, which loads [SKSE64](https://www.nexusmods.com/skyrimspecialedition/mods/30379) at
runtime without using the `skse64_loader.exe` executable.

Available for download on [Nexus Mods](https://www.nexusmods.com/skyrimspecialedition/mods/165890).

Why?
----

With this plugin installed, you can start the game with SKSE64 and all its plugins loaded using
the regular `SkyrimSE.exe` or `SkyrimSELauncher.exe`.

This makes launching the game easier, as you no longer need to configure the game client to use
a custom executable, but more importantly it allows GOG Galaxy's overlay and achievements to
work, which they do not when launched via `skse64_loader.exe`
([issue](https://github.com/ianpatt/skse64/issues/54)).

To be clear: *this will not make your mods load any faster*. It's just for convenience and to fix
that GOG issue.

Compatibility
-------------

As of v1.1.0, this mod has been tested and is known to work on:

  - Special Edition v1.5.97 on Steam
  - Anniversary Edition v1.6.1170 on Steam
  - Anniversay Edition v1.6.1179 on GOG

Other version are untested, though it should also work (eg SkyrimVR). If you got it working, please
report it so I can add it to this list!

Installation
------------

  - Follow the [SKSE64](https://www.nexusmods.com/skyrimspecialedition/mods/30379) install instructions
  - Follow the [SSE Engine Fixes](https://www.nexusmods.com/skyrimspecialedition/mods/17230) install instructions
  - Download the archive
  - Unpack it on your game folder
  - Now you should have `Data\DLLPlugins\!!!!SKSE64LoaderDLL.dll`

To uninstall, simply remove that DLL.

**I'd recommend that you do not rename the file and remove the leading exclamation marks** - those
ensure the file is the first one loaded, avoiding crashes if any other DLL in the folder depends on
SKSE64.