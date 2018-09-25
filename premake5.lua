workspace "SketchUpApiCppWrapper"
	
	location ("premake5/" .. _ACTION)
	configurations { "Release", "Debug" }
	platforms { "x64" }
	architecture "x86_64"
	files "premake5.lua"
	includedirs {"include"}
	cppdialect "C++11"
	flags {"FatalWarnings", "MultiProcessorCompile", "NoPCH", "NoImplicitLink"}

	filter {"system:windows"}
		includedirs {"third-party/slapi/win/headers"}
		libdirs {"third-party/slapi/win/binaries/sketchup/x64"}
		defines {"WIN32", "_WINDOWS", "WIN32_LEAN_AND_MEAN", "NOMINMAX"}

	filter {"system:macosx"}
		includedirs {"third-party/slapi/mac/headers"}
		libdirs {"third-party/slapi/mac/SketchUpAPI.framework"}

	filter "Debug"
		defines {"_DEBUG"}
		optimize "Debug"

	filter "Release"
		defines {"NDEBUG"}
		optimize "Full"

	filter {}

	
project "SketchUpAPICppWrapper"
	kind "StaticLib"
	files { "src/SUAPI-CppWrapper/**", "include/SUAPI-CppWrapper/**" }


project "GoogleTest"
	kind "StaticLib"
	includedirs {"third-party/googletest/googletest/include", "third-party/googletest/googletest"}
	files {"third-party/googletest/googletest/src/gtest-all.cc"}
	defines {"_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"}


project "SketchUpAPITests"
	kind "ConsoleApp"
	files {"tests/**"}
	includedirs {
		"third-party/googletest/googletest/include", 
		"third-party/googletest/googletest",
		"tests"
		}
	libdirs {"%{cfg.buildtarget.directory}"}
	links {"GoogleTest", "SketchUpAPICppWrapper", "SketchUpAPI"}
	defines {"_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"}


filter {"system:windows"}
postbuildcommands {
	"{COPY} ../../third-party/slapi/win/binaries/sketchup/x64/SketchUpAPI.dll %{cfg.buildtarget.directory}",
	"{COPY} ../../third-party/slapi/win/binaries/sketchup/x64/SketchUpCommonPreferences.dll %{cfg.buildtarget.directory}"
}



