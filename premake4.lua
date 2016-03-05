solution "hunter-game"
	configurations { "debug", "release" }

	project "hunter-game"
		kind "WindowedApp"
		language "C"
		files { "**.h", "**.c" }
		links { "m" }

		configuration "debug"
			flags { "Symbols" }
			links { "tds_debug" }
			targetname "hunter-game"

		configuration "release"
			flags { "Optimize" }
			links { "tds" }
			targetname "hunter-game-debug"
