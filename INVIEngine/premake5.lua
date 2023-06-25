project "INVIEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- pchheader "hzpch.h"
	-- pchsource "src/hzpch.cpp"
	

	files
	{
		"Engine/**.h",
		"Engine/**.cpp",
		"vendor/simple_library/**.h",
		"vendor/simple_library/**.cpp",
	}

	defines
	{}

	includedirs
	{
		"Engine",
        "%{IncludeDir.simple_library}",
	}

	links
	{}

	filter "system:windows"
		systemversion "latest"

		defines
		{}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"




