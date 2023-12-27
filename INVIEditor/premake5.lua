project "INVIEditor"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		-- "%{wks.location}/INVIEngine/vendor/simple_library",
		"%{wks.location}/INVIEngine/Engine",
		"%{wks.location}/INVIEngine/vendor",
		"%{IncludeDir.simple_library}",
		"%{IncludeDir.FBX_SDK}",
		"%{IncludeDir.imgui}",
	}

    links
    {
        -- "INVIEngine"
    }

    filter "system:windows"
        systemversion "latest"

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

