solution "hunter-game"
	configurations { "debug", "release" }

	project "hunter-game"
		kind "WindowedApp"
		language "C"
		files { "**.h", "**.c" }
		includedirs { "/usr/include/freetype2" }
		links { "m" }

		configuration "debug"
			flags { "Symbols" }
			links { "tds_debug" }
			defines { "TDS_MEMORY_DEBUG" }
			targetname "hunter-game-debug"

		configuration "release"
			flags { "Optimize" }
			links { "tds" }
			targetname "hunter-game"
