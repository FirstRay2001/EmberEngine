workspace "Ember"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ember/Vendor/glfw/include"
IncludeDir["Glad"] = "Ember/Vendor/glad/include"
IncludeDir["ImGui"] = "Ember/Vendor/imgui"

include "Ember/Vendor/glfw"
include "Ember/Vendor/glad"
include "Ember/Vendor/imgui"

project "Ember"
    location "Ember"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "emberpch.h"
    pchsource "Ember/Source/emberpch.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
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
        defines 
        {
            "EMBER_DEBUG",
            "EMBER_ENABLE_ASSERTS"
        }
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        buildoptions "/MD"
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

        disablewarnings { "4828" }

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
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        buildoptions "/MD"
        optimize "On"