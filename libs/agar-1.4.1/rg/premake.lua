--
-- Do not edit!
-- This file was generated from Makefile by BSDbuild 2.8.
--
-- To regenerate this file, get the latest BSDbuild release from
-- http://hypertriton.com/bsdbuild/, the latest Premake release
-- (v3 series) from http://premake.sourceforge.net/, and execute:
--
--     $ make proj
--
package = newpackage()
package.name = "ag_rg_static"
package.kind = "lib"
package.language = "c"
package.guid = "54223b00-f4c5-4b49-ab84-cd2576c7f8d6"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
tinsert(package.links,{"ag_gui_static"})
if (hdefs["HAVE_SDL"] ~= nil) then
table.insert(package.links,{"SDL"})
table.insert(package.links,{"SDLmain"})
table.insert(package.includepaths,{"$(#sdl.include)"})
table.insert(package.libpaths,{"$(#sdl.lib)"})
end
if (hdefs["HAVE_OPENGL"] ~= nil) then
table.insert(package.links,{"opengl32"})
table.insert(package.includepaths,{"$(#gl.include)"})
table.insert(package.libpaths,{"$(#gl.lib)"})
end
if (hdefs["HAVE_FREETYPE"] ~= nil) then
table.insert(package.links,{"freetype"})
table.insert(package.includepaths,{"$(#freetype.include)"})
table.insert(package.libpaths,{"$(#freetype.lib)"})
end
tinsert(package.links,{"ag_vg_static"})
package.files = {
	"tileset.c",
	"tileview.c",
	"tile.c",
	"feature.c",
	"fill.c",
	"pixmap.c",
	"prim.c",
	"animation.c",
	"texture.c",
	"texsel.c",
	"transform.c",
	"animview.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_RG_INTERNAL"})
package = newpackage()
package.name = "ag_rg"
package.kind = "dll"
package.language = "c"
package.guid = "54223b00-f4c5-4b49-ab84-cd2576c7f8d6"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
tinsert(package.links,{"ag_gui_static"})
if (hdefs["HAVE_SDL"] ~= nil) then
table.insert(package.links,{"SDL"})
table.insert(package.links,{"SDLmain"})
table.insert(package.includepaths,{"$(#sdl.include)"})
table.insert(package.libpaths,{"$(#sdl.lib)"})
end
if (hdefs["HAVE_OPENGL"] ~= nil) then
table.insert(package.links,{"opengl32"})
table.insert(package.includepaths,{"$(#gl.include)"})
table.insert(package.libpaths,{"$(#gl.lib)"})
end
if (hdefs["HAVE_FREETYPE"] ~= nil) then
table.insert(package.links,{"freetype"})
table.insert(package.includepaths,{"$(#freetype.include)"})
table.insert(package.libpaths,{"$(#freetype.lib)"})
end
tinsert(package.links,{"ag_vg_static"})
package.files = {
	"tileset.c",
	"tileview.c",
	"tile.c",
	"feature.c",
	"fill.c",
	"pixmap.c",
	"prim.c",
	"animation.c",
	"texture.c",
	"texsel.c",
	"transform.c",
	"animview.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_RG_INTERNAL"})
