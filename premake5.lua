workspace "tinyc"
    configurations { "Debug", "Release" }
    language "C"
    architecture "x86_64"

project "tinyc"
    kind "ConsoleApp"

    language "C"
    cdialect "C99"
    usestandardpreprocessor "On"

    targetdir "bin/%{cfg.buildcfg}"
    targetname "tinyc"

    files { "src/**.h", "src/**.c" }

    includedirs { "src" }

    warnings ("High")
