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
package.name = "ag_core_static"
package.kind = "lib"
package.language = "c"
package.guid = "9d6bb497-1d2a-4ac0-9f7b-7be68b9077e8"
dofile("../configure.lua")
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
package.files = {
	"variable.c",
	"config.c",
	"core.c",
	"error.c",
	"event.c",
	"object.c",
	"prop.c",
	"rcs.c",
	"timeout.c",
	"class.c",
	"cpuinfo.c",
	"load_den.c",
	"data_source.c",
	"load_string.c",
	"load_version.c",
	"snprintf.c",
	"vsnprintf.c",
	"vasprintf.c",
	"net_client.c",
	"net_command.c",
	"net_fgetln.c",
	"net_server.c",
	"dir.c",
	"md5.c",
	"sha1.c",
	"rmd160.c",
	"file.c",
	"string_compat.c",
	"dso.c",
	"tree.c",
	"time.c",
	"time_dummy.c",
	"time_gettimeofday.c",
	"time_win32.c",
	"time_condwait.c",
	"db.c",
	"dbobject.c",
	"tbl.c",
	"getopt.c",
	"exec.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_CORE_INTERNAL"})
package = newpackage()
package.name = "ag_core"
package.kind = "dll"
package.language = "c"
package.guid = "9d6bb497-1d2a-4ac0-9f7b-7be68b9077e8"
dofile("../configure.lua")
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
package.files = {
	"variable.c",
	"config.c",
	"core.c",
	"error.c",
	"event.c",
	"object.c",
	"prop.c",
	"rcs.c",
	"timeout.c",
	"class.c",
	"cpuinfo.c",
	"load_den.c",
	"data_source.c",
	"load_string.c",
	"load_version.c",
	"snprintf.c",
	"vsnprintf.c",
	"vasprintf.c",
	"net_client.c",
	"net_command.c",
	"net_fgetln.c",
	"net_server.c",
	"dir.c",
	"md5.c",
	"sha1.c",
	"rmd160.c",
	"file.c",
	"string_compat.c",
	"dso.c",
	"tree.c",
	"time.c",
	"time_dummy.c",
	"time_gettimeofday.c",
	"time_win32.c",
	"time_condwait.c",
	"db.c",
	"dbobject.c",
	"tbl.c",
	"getopt.c",
	"exec.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_CORE_INTERNAL"})
