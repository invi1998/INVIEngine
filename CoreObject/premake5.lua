project "CoreObject"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "CoreObjectPch.h"
	pchsource "src/CoreObjectPch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	defines
	{}

	includedirs
	{
		"src",
		"%{IncludeDir.simple_library}",
		"%{IncludeDir.FBX_SDK}",
		"%{IncludeDir.imgui}",
	}

	links
	{
		"imgui",
		"simple_library",
	}


	filter "configurations:Debug"
		defines
		{
			"HZ_DEBUG"
		}
		runtime "Debug"
		symbols "on"
		
		-- staticruntime "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

		-- staticruntime "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

    filter "action:vs*"
		buildoptions "/F 16000000"
		linkoptions "/STACK:16000000"

	filter "action:gcc*"
		linkoptions "-Wl,--stack,16000000"
		linkoptions "-Wl,-z,stack-size=16000000"

