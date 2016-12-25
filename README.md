# hunter-game
This beautful Markdown README does not mean anything yet.
This game is in development and is barely playable.

hunter-game uses tds-engine, which can be found on my GitHub profile.

## compile

To compile the game, you must have premake4 installed.
Make sure that you have compiled tds-engine and installed it.
Execute `premake4 gmake` to generate the makefile.
Execute `make` to compile the game in debug mode.
Execute `make config=release` to compile the game in release mode.

## config

hunter uses Lua for configuration.

hunter.lua contains the local game configuration:
	`save` is an integer variable designating the save index to use.

tds.lua contains the engine configuration:
	`width` is an integer variable designating the width of the target display [default: 640]

	`height` is an integer variable designating the height of the target display [default: 480]

	`fullscreen` is a boolean variable designating whether the game should be fullscreen [default: false]

	`verticalsync` is an integer variable designating the vertical sync interval (1 -> normal vsync, 2 -> vsync at 1/2 FPS, etc) [default: 0]

	`msaa` is an integer variable designating the number of samples for MSAA [default: 0]

