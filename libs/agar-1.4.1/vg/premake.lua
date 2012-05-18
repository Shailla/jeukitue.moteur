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
package.name = "ag_vg_static"
package.kind = "lib"
package.language = "c"
package.guid = "6d009edb-c66a-46e8-a1dd-b6f653db6327"
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
	"vg.c",
	"vg_circle.c",
	"vg_arc.c",
	"vg_line.c",
	"vg_ortho.c",
	"vg_point.c",
	"vg_snap.c",
	"vg_tables.c",
	"vg_text.c",
	"vg_polygon.c",
	"vg_view.c",
	"vg_tool.c",
	"vg_circle_tool.c",
	"vg_line_tool.c",
	"vg_point_tool.c",
	"vg_proximity_tool.c",
	"vg_text_tool.c",
	"vg_arc_tool.c",
	"vg_polygon_tool.c",
	"vg_select_tool.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_VG_INTERNAL"})
package = newpackage()
package.name = "ag_vg"
package.kind = "dll"
package.language = "c"
package.guid = "6d009edb-c66a-46e8-a1dd-b6f653db6327"
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
	"vg.c",
	"vg_circle.c",
	"vg_arc.c",
	"vg_line.c",
	"vg_ortho.c",
	"vg_point.c",
	"vg_snap.c",
	"vg_tables.c",
	"vg_text.c",
	"vg_polygon.c",
	"vg_view.c",
	"vg_tool.c",
	"vg_circle_tool.c",
	"vg_line_tool.c",
	"vg_point_tool.c",
	"vg_proximity_tool.c",
	"vg_text_tool.c",
	"vg_arc_tool.c",
	"vg_polygon_tool.c",
	"vg_select_tool.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_VG_INTERNAL"})
