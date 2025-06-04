workspace "Chapter_10"
    configurations {"Debuh", "Release"}
    architecture "x86_64"

    project "Chapter_10"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"

        targetdir "bin/"
        objdir "bin-int/"

        files {
            "src/**.h",
            "src/**.hpp",
            "src/**.cpp"
        }

        includedirs {
            "../_dependencies/include/",
            "src/",
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
            "fmodL_vc",
            "fmodstudioL_vc",
            "assimp-vc143-mtd"
        }

        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
            
        filter "configurations:Release"
            runtime "Release"
            symbols "off"
            optimize "full"