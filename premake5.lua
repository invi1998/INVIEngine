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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dev"
	include "vendor/premake"
	include "INVIEngine"
