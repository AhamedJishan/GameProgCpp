workspace "Chapter_13"
    configurations {"Debug", "Release"}
    architecture "x86_64"

    project "Chapter_13"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "bin-int/%{cfg.buildcfg}"

        files {
            "src/**.h",
            "src/**.c",
            "src/**.hpp",
            "src/**.cpp"
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
            "SDL2main"
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
