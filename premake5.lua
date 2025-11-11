workspace "Ember"
    architecture "x64"
    startproject "Client"

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
IncludeDir["glm"] = "Ember/Vendor/glm"

group "Dependencies"
    include "Ember/Vendor/glfw"
    include "Ember/Vendor/glad"
    include "Ember/Vendor/imgui"
group ""


project "Ember"
    location "Ember"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "emberpch.h"
    pchsource "Ember/Source/emberpch.cpp"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.hpp",
        "%{prj.name}/Vendor/glm/glm/**.inl",
        "%{prj.name}/Vendor/stb_image/**.h",
        "%{prj.name}/Vendor/stb_image/**.cpp"

    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "configurations:*"
        buildoptions
        {
            "/utf-8"
        }

        disablewarnings { "4828" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EMBER_PLATFORM_WINDOWS",
            "EMEBR_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines 
        {
            "EMBER_DEBUG"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        runtime "Release"
        optimize "on"

project "Ember-Editor"
    location "Ember-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "Ember/Vendor",
        "Ember/Source",
        "%{IncludeDir.glm}"
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
        systemversion "latest"

        defines
        {
            "EMBER_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "EMBER_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EMBER_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EMBER_DIST"
        runtime "Release"
        optimize "on"

    project "Client"
        location "Client"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
    
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
            "Ember/Vendor",
            "Ember/Source",
            "%{IncludeDir.glm}"
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
            systemversion "latest"
    
            defines
            {
                "EMBER_PLATFORM_WINDOWS"
            }
    
        filter "configurations:Debug"
            defines "EMBER_DEBUG"
            runtime "Debug"
            symbols "on"
    
        filter "configurations:Release"
            defines "EMBER_RELEASE"
            runtime "Release"
            optimize "on"
    
        filter "configurations:Dist"
            defines "EMBER_DIST"
            runtime "Release"
            optimize "on"