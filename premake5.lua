workspace "TextureCreator"
	configurations { "Debug", "Release" }
	location "build"
	cppdialect "C++17"

include "ktech/ktech/"

project "texturecreator"
	kind "ConsoleApp"
	language "C++"
	targetdir "%{wks.location}/bin"
	objdir "%{wks.location}/obj/%{prj.name}"
	targetname "%{prj.name}"

	links { "KTechLibrary" }

	files { "*.cpp", "*.hpp", "ui/*.cpp", "ui/*.hpp" }

	filter "configurations:Debug"
		symbols "On"