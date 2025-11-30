
SKSE64 Loader DLL
=================

This is a plugin for the [SSE Engine Fixes](https://www.nexusmods.com/skyrimspecialedition/mods/17230)
Preloader, which loads [SKSE64](https://www.nexusmods.com/skyrimspecialedition/mods/30379) at
runtime without using the `skse64_loader.exe` executable.

Why?
----

With this plugin installed, you can start the game with SKSE64 and all its plugins loaded using
the regular `SkyrimSE.exe` or `SkyrimSELauncher.exe`.

This makes launching the game easier, as you no longer need to customize the game client to use
a custom executable, but more importantly it allows GOG Galaxy's overlay and achievements to
work, which they do not when launched via `skse64_loader.exe`.

Installation
------------

  - Download the archive
  - Unpack it on your game folder
  - Now you should have `Data\DLLPlugins\SKSE64LoaderDLL.dll`

To uninstall, simply remove that DLL.
