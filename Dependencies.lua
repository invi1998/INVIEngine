
-- INVIEngine Dependencies

IncludeDir = {}
IncludeDir["simple_library"] = "%{wks.location}/INVIEngine/vendor/simple_library/include"
IncludeDir["FBX_SDK"] = "%{wks.location}/INVIEngine/vendor/FBX_SDK/include"
IncludeDir["imgui"] = "%{wks.location}/INVIEngine/vendor/imgui"

LibraryDir = {}
LibraryDir["FBX_SDK"] = "%{wks.location}/INVIEngine/vendor/FBX_SDK/lib/vs2022/x64/%{cfg.buildcfg}"

Library = {}
Library["FBX_SDK"] = "%{LibraryDir.FBX_SDK}/libfbxsdk.lib"
Library["FBX_SDK_MD"] = "%{LibraryDir.FBX_SDK}/libfbxsdk-md.lib"
Library["FBX_SDK_MT"] = "%{LibraryDir.FBX_SDK}/libfbxsdk-mt.lib"


Binaries = {}
Binaries["FBX_SDK_Debug"] = "%{wks.location}/INVIEngine/vendor/FBX_SDK/lib/vs2022/x64/debug/libfbxsdk.dll"
Binaries["FBX_SDK_Release"] = "%{wks.location}/INVIEngine/vendor/FBX_SDK/lib/vs2022/x64/release/libfbxsdk.dll"
