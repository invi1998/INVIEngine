project "INVIEngine"
	kind "WindowedApp"
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
		"vendor/simple_library/**.c",
		"vendor/simple_library/**.cpp",
        "vendor/simple_library/**.hpp",
	}

	defines
	{}

	includedirs
	{
		"Engine",
        "%{IncludeDir.simple_library}",
	}

	links
	{
		"d3dcompiler.lib",
		"D3D12.lib",
		"dxgi.lib",
	}

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

	filter "action:vs*"
		buildoptions "/F 16000000"
		linkoptions "/STACK:16000000"

	filter "action:gcc*"
		linkoptions "-Wl,--stack,16000000"
		linkoptions "-Wl,-z,stack-size=16000000"
		