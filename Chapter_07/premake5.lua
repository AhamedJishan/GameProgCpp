workspace "Chapter_07"
    architecture "x86_64"
    configurations {"Debug", "Release"}

    project "Chapter_07"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"

        targetdir "bin"
        objdir "bin-int"

        files {
            "src/**.h",
            "src/**.cpp",
            "src/**.hpp"
        }

        includedirs {
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/inc",
            "../_dependencies/include/",
            "src/"
        }

        libdirs {
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/lib/x64",
            "../_dependencies/lib/",
        }

        links {
            "SDL2",
            "SDL2main",
            "opengl32",
            "glew32",
            "assimp-vc143-mtd",
            "fmodL_vc",
            "fmodstudioL_vc"
        }

        filter "configurations:Debug"
            symbols "On"
            runtime "Debug"

        filter "configurations:Release"
            symbols "Off"
            optimize "full"

