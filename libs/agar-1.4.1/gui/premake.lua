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
package.name = "ag_gui_static"
package.kind = "lib"
package.language = "c"
package.guid = "71a13512-13e7-44a6-ba69-e868296cdc34"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
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
	"gui.c",
	"widget.c",
	"window.c",
	"style.c",
	"style_default.c",
	"iconmgr.c",
	"colors.c",
	"cursors.c",
	"ttf.c",
	"text.c",
	"keymap_ascii.c",
	"keymap_utf8.c",
	"keymap_latin1.c",
	"keymap_compose.c",
	"keysyms.c",
	"editable.c",
	"box.c",
	"label.c",
	"button.c",
	"checkbox.c",
	"textbox.c",
	"radio.c",
	"fixed_plotter.c",
	"tlist.c",
	"scrollbar.c",
	"spinbutton.c",
	"titlebar.c",
	"toolbar.c",
	"fspinbutton.c",
	"combo.c",
	"ucombo.c",
	"units.c",
	"nlunits.c",
	"hsvpal.c",
	"mspinbutton.c",
	"mfspinbutton.c",
	"statusbar.c",
	"menu.c",
	"menu_view.c",
	"treetbl.c",
	"separator.c",
	"notebook.c",
	"pixmap.c",
	"file_dlg.c",
	"objsel.c",
	"fixed.c",
	"numerical.c",
	"table.c",
	"glview.c",
	"mpane.c",
	"pane.c",
	"console.c",
	"graph.c",
	"socket.c",
	"icon.c",
	"progress_bar.c",
	"slider.c",
	"text_cache.c",
	"load_surface.c",
	"load_color.c",
	"load_xcf.c",
	"file_selector.c",
	"scrollview.c",
	"font_selector.c",
	"time_sdl.c",
	"debugger.c",
	"surface.c",
	"perfmon.c",
	"widget_legacy.c",
	"global_keys.c",
	"input_device.c",
	"mouse.c",
	"keyboard.c",
	"packedpixel.c",
	"load_bmp.c",
	"load_jpg.c",
	"load_png.c",
	"drv.c",
	"drv_sw.c",
	"drv_mw.c",
	"drv_gl_common.c",
	"drv_sdl_common.c",
	"drv_glx.c",
	"drv_sdlfb.c",
	"drv_sdlgl.c",
	"drv_wgl.c",
	"dir_dlg.c",
	"anim.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_GUI_INTERNAL"})
package = newpackage()
package.name = "ag_gui"
package.kind = "dll"
package.language = "c"
package.guid = "71a13512-13e7-44a6-ba69-e868296cdc34"
dofile("../configure.lua")
tinsert(package.links,{"ag_core_static"})
if (hdefs["HAVE_PTHREADS"] ~= nil) then
table.insert(package.links,{"pthreadVC2"})
table.insert(package.includepaths,{"$(#pthreads.include)"})
table.insert(package.libpaths,{"$(#pthreads.lib)"})
end
tinsert(package.links,{"winmm"})
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
	"gui.c",
	"widget.c",
	"window.c",
	"style.c",
	"style_default.c",
	"iconmgr.c",
	"colors.c",
	"cursors.c",
	"ttf.c",
	"text.c",
	"keymap_ascii.c",
	"keymap_utf8.c",
	"keymap_latin1.c",
	"keymap_compose.c",
	"keysyms.c",
	"editable.c",
	"box.c",
	"label.c",
	"button.c",
	"checkbox.c",
	"textbox.c",
	"radio.c",
	"fixed_plotter.c",
	"tlist.c",
	"scrollbar.c",
	"spinbutton.c",
	"titlebar.c",
	"toolbar.c",
	"fspinbutton.c",
	"combo.c",
	"ucombo.c",
	"units.c",
	"nlunits.c",
	"hsvpal.c",
	"mspinbutton.c",
	"mfspinbutton.c",
	"statusbar.c",
	"menu.c",
	"menu_view.c",
	"treetbl.c",
	"separator.c",
	"notebook.c",
	"pixmap.c",
	"file_dlg.c",
	"objsel.c",
	"fixed.c",
	"numerical.c",
	"table.c",
	"glview.c",
	"mpane.c",
	"pane.c",
	"console.c",
	"graph.c",
	"socket.c",
	"icon.c",
	"progress_bar.c",
	"slider.c",
	"text_cache.c",
	"load_surface.c",
	"load_color.c",
	"load_xcf.c",
	"file_selector.c",
	"scrollview.c",
	"font_selector.c",
	"time_sdl.c",
	"debugger.c",
	"surface.c",
	"perfmon.c",
	"widget_legacy.c",
	"global_keys.c",
	"input_device.c",
	"mouse.c",
	"keyboard.c",
	"packedpixel.c",
	"load_bmp.c",
	"load_jpg.c",
	"load_png.c",
	"drv.c",
	"drv_sw.c",
	"drv_mw.c",
	"drv_gl_common.c",
	"drv_sdl_common.c",
	"drv_glx.c",
	"drv_sdlfb.c",
	"drv_sdlgl.c",
	"drv_wgl.c",
	"dir_dlg.c",
	"anim.c",
}
tinsert(package.defines,{"_AGAR_INTERNAL"})
tinsert(package.defines,{"_BSD_SOURCE"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include"})
tinsert(package.includepaths,{"/home/vedge/src/agar-1.4.1/include/agar"})
tinsert(package.defines,{"_AGAR_GUI_INTERNAL"})
