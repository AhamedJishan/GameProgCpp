workspace "Chapter_12"
    architecture "x86_64"
    configurations {"Debug", "Release"}

    project "Chapter_12"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "bin-int/%{cfg.buildcfg}"

        files {
            "src/**.h",
            "src/**.hpp",
            "src/**.cpp",
            "src/**.c"
        }

        includedirs {
            "src/",
            "../_dependencies/include/",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/inc"
        }

        libdirs {
            "../_dependencies/lib/",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64",
            "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/lib/x64"
        }

        links {
            "opengl32",
            "glew32",
            "SDL2",
            "SDL2main",
            "SDL2_ttf",
            "fmodL_vc",
            "fmodstudioL_vc",
            "assimp-vc143-mtd"
        }

        filter "configurations:Debug"
            runtime "Debug"
            defines "DEBUG"
            symbols "on"
            
        filter "configurations:Release"
            runtime "Release"
            defines "NDEBUG"
            symbols "off"
            optimize "full"