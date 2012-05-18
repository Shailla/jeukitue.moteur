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
project.name = "configsettings"
package = newpackage()
package.name = "configsettings"
package.kind = "winexe"
package.language = "c"
package.guid = "21388f56-8e8d-4890-b4b0-b07b0bd757e2"
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
package.files = {
	"configsettings.c",
}
tinsert(package.defines,{"_USE_AGAR_STD"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/demos/configsettings"})
