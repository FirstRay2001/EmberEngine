workspace "Ember"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Ember"
    location "Ember"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Vendor/spdlog/include"
    }

    filter "configurations:*"
        buildoptions
        {
            "/utf-8"
        }

        disablewarnings { "4828" }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "EMBER_PLATFORM_WINDOWS",
            "EMEBR_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Client/")
        }

    filter "configurations:Debug"
        defines "EMBER_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        optimize "On"

project "Client"
    location "Client"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
    }

    includedirs
    {
        "Ember/Vendor/spdlog/include",
        "Ember/Source"
    }

    links
    {
        "Ember"
    }

    filter "configurations:*"
        buildoptions 
        {
            "/utf-8"
        }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "EMBER_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "EMBER_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        optimize "On"