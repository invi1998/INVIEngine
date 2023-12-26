include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "INVIEngine"
	architecture "x86_64"
	startproject "INVIEngine"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dev"
	include "INVIEngine"

group "Dependencies"
	include "vendor/premake"
	include "INVIEngine/vendor/simple_library"
	include "INVIEngine/vendor/imgui"
