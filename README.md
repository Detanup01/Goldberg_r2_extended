# Goldberg r2 Extended
"Extended" version of goldberg R2 uplay emulator.

Original ReadMe:
# Goldberg uplay r2 emu.

You can find a build of the emu in the release64 folder.

I took the time to reverse all the function argument names for every function. They should all be close to/exactly what the real arguments are named as. Some of the function argument types might not be 100% perfect but they should be compatible

You don't have to provide the game appid to the emu because the game provides it with the UPC_Init function. To get the appid of a game, build the debug version and look at the UPC_Init in the log.

The provided uplay_r2.ini is for the fenyx game.

I'm not sure what I labeled items are actually items. They might just be another type of DLC.
