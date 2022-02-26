include("conanbuildinfo.premake.lua")

workspace "SgCity"
    conan_basic_setup()

    architecture "x64"
    startproject "SgCity"

    configurations
    {
        "Debug",
        "Release"
    }

    floatingpoint "Fast"
    flags "MultiProcessorCompile"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SgCity"
    location "/Dev/SgCity"
    architecture "x64"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    linkoptions
    {
        conan_exelinkflags,
        "/IGNORE:4099"
    }

    files
    {
        "/Dev/SgCity/src/**.h",
        "/Dev/SgCity/src/**.cpp"
    }

    includedirs
    {
        "/Dev/SgCity/src"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "SG_CITY_DEBUG_BUILD", "GLFW_INCLUDE_NONE" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "GLFW_INCLUDE_NONE"
        runtime "Release"
        optimize "On"
