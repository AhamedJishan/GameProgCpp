workspace "Chapter_06"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    project "Chapter_06"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"

        targetdir "bin/"
        objdir "bin-int/"

        files {
            "src/**.h",
            "src/**.cpp",
            "src/**.hpp"
        }

        includedirs {
            "../_dependencies/include",
            "src"
        }

        libdirs {
            "../_dependencies/lib"
        }

        links {
            "SDL2",
            "SDL2main",
            "opengl32",
            "glew32",
            "assimp-vc143-mtd"
        }

        filter "configurations:Debug"
            symbols "On"
            runtime "Debug"

        filter "configurations:Release"
                symbols "Off"
                optimize "full"
