project "simple_library"
	kind "StaticLib"
    language "C"
	staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "src/**.h",
		"src/**.cpp",
        "src/**.c",
		
		"include/**.h",

        -- "include/simple_library/simple_library.h",
        -- "include/simple_library/simple_library_macro.h",

        -- "src/third_party_library/tinyxml/tinystr.h",
        -- "src/third_party_library/tinyxml/tinyxml.h",

        -- "src/**.h",
        -- "src/**.cpp",
        -- "src/**.c",

        -- "src/private/**.h",
        -- "private/**.cpp",
        -- "private/**.c",
        -- "private/**.hpp",

        -- "public/**.h",
        -- "public/**.cpp",
        -- "public/**.c",
        -- "public/**.hpp",

        -- "third_party_library/**.h",
        -- "third_party_library/**.cpp",
        -- "third_party_library/**.c",
        -- "third_party_library/**.hpp",
	}

    includedirs
	{
		"include"
	}

    filter "system:linux"
        pic "On"

        systemversion "latest"
        
        files
        {}

        defines
        {}

    filter "system:windows"
        systemversion "latest"

        files
        {}

        defines 
        {}

        links
        {}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"
        symbols "off"

