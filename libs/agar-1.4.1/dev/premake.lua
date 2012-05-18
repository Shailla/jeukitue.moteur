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
package.name = "ag_dev_static"
package.kind = "lib"
package.language = "c"
package.guid = "e0baea2d-5cb8-4a7a-9120-8352f36109e7"
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
package.files = {
	"dev.c",
	"browser.c",
	"screenshot.c",
	"server.c",
	"timeouts.c",
	"uniconv.c",
	"view_params.c",
	"cpuinfo.c",
	"config.c",
	"object.c",
	"classes.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_DEV_INTERNAL"})
package = newpackage()
package.name = "ag_dev"
package.kind = "dll"
package.language = "c"
package.guid = "e0baea2d-5cb8-4a7a-9120-8352f36109e7"
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
package.files = {
	"dev.c",
	"browser.c",
	"screenshot.c",
	"server.c",
	"timeouts.c",
	"uniconv.c",
	"view_params.c",
	"cpuinfo.c",
	"config.c",
	"object.c",
	"classes.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_DEV_INTERNAL"})
