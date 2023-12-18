project "INVIEngine"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "EngineMinimal.h"
	pchsource "Engine/EngineMinimal.cpp"
	

	files
	{
		"Engine/**.h",
		"Engine/**.cpp",
		-- "vendor/simple_library/**.h",
		-- "vendor/simple_library/**.c",
		-- "vendor/simple_library/**.cpp",
        -- "vendor/simple_library/**.hpp",
	}

	defines
	{}

	includedirs
	{
		"Engine",
        "%{IncludeDir.simple_library}",
		"%{IncludeDir.FBX_SDK}",
	}

	links
	{
		"d3dcompiler.lib",
		"D3D12.lib",
		"dxgi.lib",
		"simple_library",
		"%{Library.FBX_SDK}",
		"%{Library.FBX_SDK_MD}",
		"%{Library.FBX_SDK_MT}",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CLOSE_DISABLE_DEBUG_INFOR",
			"USE_PIX",
			"DBG",
			"_DEBUG",
			"PROFILE",
			"PROFILE_BUILD",
		}

	filter "configurations:Debug"
		defines
		{
			"HZ_DEBUG"
		}
		runtime "Debug"
		symbols "on"

		postbuildcommands 
		{
			'{COPY} "%{Binaries.FBX_SDK_Debug}" "%{cfg.targetdir}"',
		}
		
		-- staticruntime "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} "%{Binaries.FBX_SDK_Release}" "%{cfg.targetdir}"',
		}

		-- staticruntime "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

		postbuildcommands 
		{
			'{COPY} "%{Binaries.FBX_SDK_Release}" "%{cfg.targetdir}"',
		}

		-- staticruntime "On"

	filter "action:vs*"
		buildoptions "/F 16000000"
		linkoptions "/STACK:16000000"

	filter "action:gcc*"
		linkoptions "-Wl,--stack,16000000"
		linkoptions "-Wl,-z,stack-size=16000000"
