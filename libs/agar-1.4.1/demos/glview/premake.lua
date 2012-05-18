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
project.name = "glview"
package = newpackage()
package.name = "glview"
package.kind = "winexe"
package.language = "c"
package.guid = "B22C8324-AB8E-417e-9459-9FA0698D45D7"
dofile("../../configure.lua")
table.insert(package.links,{"ag_core"})
table.insert(package.links,{"ag_core"})
table.insert(package.links,{"SDL"})
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
end
tinsert(package.links,{"winmm"})
table.insert(package.links,{"ag_gui"})
table.insert(package.links,{"SDL"})
table.insert(package.links,{"opengl32"})
table.insert(package.links,{"freetype"})
if (hdefs["HAVE_SDL"] ~= nil) then
table.insert(package.links,{"SDL"})
table.insert(package.links,{"SDLmain"})
end
if (hdefs["HAVE_OPENGL"] ~= nil) then
table.insert(package.links,{"opengl32"})
end
if (hdefs["HAVE_FREETYPE"] ~= nil) then
table.insert(package.links,{"freetype"})
end
if (hdefs["HAVE_OPENGL"] ~= nil) then
table.insert(package.links,{"opengl32"})
end
if (hdefs["HAVE_GLU"] ~= nil) then
table.insert(package.links,{"glu32"})
end
package.files = {
	"glview.c",
}
tinsert(package.includepaths,{"."})
