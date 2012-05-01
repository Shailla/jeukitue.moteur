/*	Public domain	*/

#ifndef _AGAR_WIDGET_WINDOW_H_
#define _AGAR_WIDGET_WINDOW_H_

#include <agar/gui/widget.h>

#include <agar/gui/begin.h>

#define AG_WINDOW_CAPTION_MAX 512

struct ag_titlebar;
struct ag_icon;

enum ag_window_alignment {
	AG_WINDOW_TL,
	AG_WINDOW_TC,
	AG_WINDOW_TR,
	AG_WINDOW_ML,
	AG_WINDOW_MC,
	AG_WINDOW_MR,
	AG_WINDOW_BL,
	AG_WINDOW_BC,
	AG_WINDOW_BR,
	AG_WINDOW_ALIGNMENT_LAST
};

#define AG_WINDOW_UPPER_LEFT	AG_WINDOW_TL
#define AG_WINDOW_UPPER_CENTER	AG_WINDOW_TC
#define AG_WINDOW_UPPER_RIGHT	AG_WINDOW_TR
#define AG_WINDOW_MIDDLE_LEFT	AG_WINDOW_ML
#define AG_WINDOW_CENTER	AG_WINDOW_MC
#define AG_WINDOW_MIDDLE_RIGHT	AG_WINDOW_MR
#define AG_WINDOW_LOWER_LEFT	AG_WINDOW_BL
#define AG_WINDOW_LOWER_CENTER	AG_WINDOW_BC
#define AG_WINDOW_LOWER_RIGHT	AG_WINDOW_BR

enum ag_window_close_action {
	AG_WINDOW_HIDE,
	AG_WINDOW_DETACH,
	AG_WINDOW_NONE
};

struct ag_widget;

typedef struct ag_window {
	struct ag_widget wid;

	Uint flags;
#define AG_WINDOW_MODAL		0x000001 /* Place in foreground */
#define AG_WINDOW_MAXIMIZED	0x000002 /* Window is maximized */
#define AG_WINDOW_MINIMIZED	0x000004 /* Window is minimized */
#define AG_WINDOW_KEEPABOVE	0x000008 /* Keep window above */
#define AG_WINDOW_KEEPBELOW	0x000010 /* Keep window below */
#define AG_WINDOW_DENYFOCUS	0x000020 /* Widgets cannot gain focus */
#define AG_WINDOW_NOTITLE	0x000040 /* Disable the titlebar */
#define AG_WINDOW_NOBORDERS	0x000080 /* Disable the window borders */
#define AG_WINDOW_NOHRESIZE	0x000100 /* Disable horizontal resize */
#define AG_WINDOW_NOVRESIZE	0x000200 /* Disable vertical resize */
#define AG_WINDOW_NOCLOSE	0x000400 /* Disable close button */
#define AG_WINDOW_NOMINIMIZE	0x000800 /* Disable minimize button */
#define AG_WINDOW_NOMAXIMIZE	0x001000 /* Disable maximize button */
#define AG_WINDOW_CASCADE	0x002000 /* For AG_WindowSetPosition() */
#define AG_WINDOW_MINSIZEPCT	0x004000 /* Set minimum size in % */
#define AG_WINDOW_NOBACKGROUND	0x008000 /* Don't fill the background */
#define AG_WINDOW_NOUPDATERECT	0x010000 /* Unused */
#define AG_WINDOW_FOCUSONATTACH	0x020000 /* Automatic focus on attach */
#define AG_WINDOW_HMAXIMIZE	0x040000 /* Keep maximized horizontally */
#define AG_WINDOW_VMAXIMIZE	0x080000 /* Keep maximized vertically */
#define AG_WINDOW_NOMOVE	0x100000 /* Disallow movement of window */
#define AG_WINDOW_NOCLIPPING	0x200000 /* Don't set a clipping rectangle over the window area */
#define AG_WINDOW_MODKEYEVENTS	0x400000 /* Generate key{up,down} events for
                                            keypresses on modifier keys */

#define AG_WINDOW_NORESIZE	(AG_WINDOW_NOHRESIZE|AG_WINDOW_NOVRESIZE)
#define AG_WINDOW_NOBUTTONS (AG_WINDOW_NOCLOSE|AG_WINDOW_NOMINIMIZE| AG_WINDOW_NOMAXIMIZE)
#define AG_WINDOW_PLAIN		(AG_WINDOW_NOTITLE|AG_WINDOW_NOBORDERS)

	char caption[AG_WINDOW_CAPTION_MAX];	/* Window caption */
	int visible;				/* Window is visible */

	struct ag_titlebar *tbar;		/* Titlebar (if any) */
	enum ag_window_alignment alignment;	/* Initial position */
	int spacing;				/* Default spacing (px) */
	int tPad, bPad, lPad, rPad;		/* Window padding (px) */
	int wReq, hReq;				/* Requested geometry (px) */
	int wMin, hMin;				/* Minimum geometry (px) */
	int wBorderBot;				/* Bottom border size (px) */
	int wBorderSide;			/* Side border size (px) */
	int wResizeCtrl;			/* Resize controls size (px) */
	AG_Rect rSaved;				/* Saved geometry */
	int minPct;				/* For MINSIZEPCT */

	AG_TAILQ_HEAD(,ag_window) subwins;	/* Sub-windows */
	AG_TAILQ_ENTRY(ag_window) windows;	/* Active window list */
	AG_TAILQ_ENTRY(ag_window) swins;	/* Sub-window list */
	AG_TAILQ_ENTRY(ag_window) detach;	/* Zombie window list */

	struct ag_icon *icon;			/* Window icon */
	AG_Rect r;				/* View area */
	int nFocused;				/* Widgets in focus chain */
} AG_Window;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agWindowClass;
extern DECLSPEC int agWindowIconWidth;
extern DECLSPEC int agWindowIconHeight;
extern DECLSPEC void AG_InitWindowSystem(void);
extern DECLSPEC void AG_DestroyWindowSystem(void);
extern DECLSPEC AG_Window *AG_WindowNew(Uint);
extern DECLSPEC AG_Window *AG_WindowNewNamed(Uint, const char *, ...) FORMAT_ATTRIBUTE(printf, 2, 3);
extern DECLSPEC void AG_WindowSetCaption(AG_Window *, const char *, ...) FORMAT_ATTRIBUTE(printf, 2, 3) NONNULL_ATTRIBUTE(2);
extern DECLSPEC void AG_WindowUpdateCaption(AG_Window *);
#define AG_WindowSetIcon(win,su) AG_IconSetSurface((win)->icon,(su))
#define AG_WindowSetIconNODUP(win,su) AG_IconSetSurfaceNODUP((win)->icon,(su))
extern DECLSPEC void AG_WindowSetSpacing(AG_Window *, int);
extern DECLSPEC void AG_WindowSetPadding(AG_Window *, int, int, int, int);
#define AG_WindowSetPaddingLeft(w,p) AG_WindowSetPadding((w),(p),-1,-1,-1)
#define AG_WindowSetPaddingRight(w,p) AG_WindowSetPadding((w),-1,(p),-1,-1)
#define AG_WindowSetPaddingTop(w,p) AG_WindowSetPadding((w),-1,-1,(p),-1)
#define AG_WindowSetPaddingBottom(w,p) AG_WindowSetPadding((w),-1,-1,-1,(p))
extern DECLSPEC void AG_WindowSetSideBorders(AG_Window *, int);
extern DECLSPEC void AG_WindowSetBottomBorder(AG_Window *, int);
extern DECLSPEC void AG_WindowSetPosition(AG_Window *, enum ag_window_alignment, int);
extern DECLSPEC void AG_WindowSetCloseAction(AG_Window *, enum ag_window_close_action);
extern DECLSPEC void AG_WindowSetMinSize(AG_Window *, int, int);
extern DECLSPEC void AG_WindowSetMinSizePct(AG_Window *, int);
extern DECLSPEC int AG_WindowSetGeometryRect(AG_Window *, AG_Rect, int);
extern DECLSPEC int AG_WindowSetGeometryAligned(AG_Window *, enum ag_window_alignment, int, int);
extern DECLSPEC int AG_WindowSetGeometryAlignedPct(AG_Window *, enum ag_window_alignment, int, int);
#define AG_WindowSetGeometry(win,x,y,w,h) AG_WindowSetGeometryRect((win),AG_RECT((x),(y),(w),(h)),0)
#define AG_WindowSetGeometryBounded(win,x,y,w,h) AG_WindowSetGeometryRect((win),AG_RECT((x),(y),(w),(h)),1)
#define AG_WindowSetGeometryMax(win) AG_WindowSetGeometry((win),0,0,agView->w,agView->h)
extern DECLSPEC void AG_WindowSaveGeometry(AG_Window *);
extern DECLSPEC int AG_WindowRestoreGeometry(AG_Window *);
extern DECLSPEC void AG_WindowMaximize(AG_Window *);
extern DECLSPEC void AG_WindowUnmaximize(AG_Window *);
extern DECLSPEC void AG_WindowMinimize(AG_Window *);
extern DECLSPEC void AG_WindowUnminimize(AG_Window *);
extern DECLSPEC void AG_WindowAttach(AG_Window *, AG_Window *);
extern DECLSPEC void AG_WindowDetach(AG_Window *, AG_Window *);
extern DECLSPEC void AG_WindowShow(AG_Window *);
extern DECLSPEC void AG_WindowHide(AG_Window *);
extern DECLSPEC void AG_WindowSetVisibility(AG_Window *, int);
extern DECLSPEC int AG_WindowEvent(SDL_Event *);
extern DECLSPEC void AG_WindowResize(AG_Window *);
extern DECLSPEC int AG_WindowIsSurrounded(AG_Window *);
extern DECLSPEC void AG_WindowFocus(AG_Window *);
extern DECLSPEC int AG_WindowFocusAtPos(int, int);
extern DECLSPEC int AG_WindowFocusNamed(const char *);
extern DECLSPEC void AG_WindowCycleFocus(AG_Window *, int);
extern DECLSPEC void AG_WindowDetachGenEv(AG_Event *);
extern DECLSPEC void AG_WindowHideGenEv(AG_Event *);
extern DECLSPEC void AG_WindowShowGenEv(AG_Event *);
extern DECLSPEC void AG_WindowCloseGenEv(AG_Event *);
#define AGWINDOW(win) ((AG_Window *)(win))
#define AGWINDETACH(win) AG_WindowDetachGenEv, "%p", (win)
#define AGWINHIDE(win) AG_WindowHideGenEv, "%p", (win)
#define AGWINCLOSE(win) AG_WindowCloseGenEv, "%p", (win)
/*
 * Render a window to the display (must be enclosed between calls to
 * AG_BeginRendering() and AG_EndRendering()).
 * The View VFS and Window object must be locked.
 */

static __inline__ void
AG_WindowDraw(AG_Window *win)
{
	if (!win->visible)
		return;

	AG_WidgetDraw(win);
	if (!agView->opengl)
		AG_ViewUpdateFB(&AGWIDGET(win)->rView);
}
/*
 * Return the currently focused window.
 * The View VFS must be locked.
 */

static __inline__ AG_Window *
AG_WindowFindFocused(void)
{
	return AG_TAILQ_LAST(&agView->windows,ag_windowq);
}
/*
 * Evaluate whether the given window is holding focus.
 * The View VFS must be locked.
 */

static __inline__ int
AG_WindowIsFocused(AG_Window *win)
{
	return (AG_TAILQ_LAST(&agView->windows,ag_windowq) == (win));
}
/*
 * Return the effective focus state of a widget.
 * The Widget and View VFS must be locked.
 */

static __inline__ int
AG_WidgetIsFocused(void *p)
{
	AG_Widget *wid = (AG_Widget *)p;
	return ((AGWIDGET(p)->flags & AG_WIDGET_FOCUSED) &&
                (wid->window == NULL || AG_WindowIsFocused(wid->window)));
}
/*
 * Recompute the coordinates and geometries of all widgets attached to the
 * window. This is used following AG_ObjectAttach() and AG_ObjectDetach()
 * calls made in event context, or direct modifications to the x,y,w,h
 * fields of the Widget structure.
 *
 * The View VFS and Window must be locked.
 */

static __inline__ void
AG_WindowUpdate(AG_Window *win)
{
	AG_SizeAlloc a;
	
	if (win == NULL) {
		return;
	}
	if (AGWIDGET(win)->x != -1 && AGWIDGET(win)->y != -1) {
		a.x = AGWIDGET(win)->x;
		a.y = AGWIDGET(win)->y;
		a.w = AGWIDGET(win)->w;
		a.h = AGWIDGET(win)->h;
		AG_WidgetSizeAlloc(win, &a);
	}
	AG_WidgetUpdateCoords(win, AGWIDGET(win)->x, AGWIDGET(win)->y);
}
/*
 * Return visibility status of window.
 * The View VFS and Window object must be locked.
 */

static __inline__ int
AG_WindowIsVisible(AG_Window *win)
{
	return (win->visible);
}
/*
 * Test whether a window is currently selected for a given WM operation.
 * The View VFS must be locked.
 */

static __inline__ int
AG_WindowSelectedWM(AG_Window *win, enum ag_wm_operation op)
{
	return (agView->winSelected == win && agView->winop == op);
}
/*
 * Return a pointer to a widget's parent window.
 * The View VFS must be locked.
 */

static __inline__ AG_Window *
AG_ParentWindow(void *obj)
{
	return (AGWIDGET(obj)->window);
}

static __inline__ void
AG_WidgetSetPosition(void *wid, int x, int y)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->x = x;
	AGWIDGET(wid)->y = y;
	AG_WidgetUpdate(wid);
	AG_ObjectUnlock(wid);
}

static __inline__ void
AG_WidgetSetSize(void *wid, int w, int h)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->w = w;
	AGWIDGET(wid)->h = h;
	AG_WidgetUpdate(wid);
	AG_ObjectUnlock(wid);
}

static __inline__ void
AG_WidgetSetGeometry(void *wid, AG_Rect r)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->x = r.x;
	AGWIDGET(wid)->y = r.y;
	AGWIDGET(wid)->w = r.w;
	AGWIDGET(wid)->h = r.h;
	AG_WidgetUpdate(wid);
	AG_ObjectUnlock(wid);
}
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_WINDOW_H_ */