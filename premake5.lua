workspace "KTech"
	configurations { "Debug", "Release" }
	location "build"

include "ktech"

project "texturecreator"
	kind "ConsoleApp"
	language "C++"
	targetdir "%{wks.location}/bin"
	objdir "%{wks.location}/obj/%{prj.name}"
	targetname "%{prj.name}"

	links { "KTechLibrary" }

	files { "*.cpp", "*.hpp" }

	filter "configurations:Debug"
		symbols "On"