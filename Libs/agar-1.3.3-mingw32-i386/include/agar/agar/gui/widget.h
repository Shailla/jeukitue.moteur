/*	Public domain	*/

#ifndef _AGAR_WIDGET_H_
#define _AGAR_WIDGET_H_

#include <agar/config/have_opengl.h>
#include <agar/config/have_long_double.h>

#include <agar/gui/colors.h>
#include <agar/gui/view.h>
#include <agar/gui/style.h>

#include <agar/gui/begin.h>

#define AG_SIZE_SPEC_MAX		256
#define AG_WIDGET_BINDING_NAME_MAX	16

typedef struct ag_size_req {
	int w, h;			/* Requested geometry in pixels */
} AG_SizeReq;

typedef struct ag_size_alloc {
	int w, h;			/* Allocated geometry in pixels */
	int x, y;			/* Allocated position in pixels */
} AG_SizeAlloc;

typedef struct ag_widget_class {
	struct ag_object_class _inherit;
	void (*draw)(void *);
	void (*size_request)(void *, AG_SizeReq *);
	int  (*size_allocate)(void *, const AG_SizeAlloc *);
} AG_WidgetClass;

typedef enum ag_widget_binding_type {
	AG_WIDGET_NONE,
	AG_WIDGET_BOOL,
	AG_WIDGET_UINT,
	AG_WIDGET_INT,
	AG_WIDGET_UINT8,
	AG_WIDGET_SINT8,
	AG_WIDGET_UINT16,
	AG_WIDGET_SINT16,
	AG_WIDGET_UINT32,
	AG_WIDGET_SINT32,
	AG_WIDGET_UINT64,
	AG_WIDGET_SINT64,
	AG_WIDGET_FLOAT,
	AG_WIDGET_DOUBLE,
	AG_WIDGET_LONG_DOUBLE,
	AG_WIDGET_STRING,
	AG_WIDGET_POINTER,
	AG_WIDGET_PROP,
	AG_WIDGET_FLAG,
	AG_WIDGET_FLAG8,
	AG_WIDGET_FLAG16,
	AG_WIDGET_FLAG32,
} AG_WidgetBindingType;

enum ag_widget_sizespec {
	AG_WIDGET_BAD_SPEC,	/* Parser error */
	AG_WIDGET_PIXELS,	/* Pixel count */
	AG_WIDGET_PERCENT,	/* % of available space */
	AG_WIDGET_STRINGLEN,	/* Width of given string */
	AG_WIDGET_FILL		/* Fill remaining space */
};

enum ag_widget_packing {
	AG_PACK_HORIZ,
	AG_PACK_VERT
};

typedef struct ag_flag_descr {
	Uint bitmask;			/* Bitmask */
	const char *descr;		/* Bit(s) description */
	int writeable;			/* User-editable */
} AG_FlagDescr;

typedef struct ag_widget_binding {
	char name[AG_WIDGET_BINDING_NAME_MAX];	/* Binding identifier */
	int type;				/* Real binding type */
	int vtype;				/* Virtual binding type
						   (translated PROP types) */
	AG_Mutex *mutex;			/* Mutex to acquire or NULL */
	void *p1;				/* Pointer to variable */
	union {
		char prop[AG_PROP_KEY_MAX];	/* Property name (for PROP) */
		size_t size;			/* Size (for STRING) */
		Uint32 bitmask;			/* Bitmask (for FLAG) */
	} data;
	AG_SLIST_ENTRY(ag_widget_binding) bindings;
} AG_WidgetBinding;

struct ag_popup_menu;

typedef struct ag_widget {
	struct ag_object obj;

	Uint flags;
#define AG_WIDGET_FOCUSABLE		0x00001 /* Can grab focus */
#define AG_WIDGET_FOCUSED		0x00002 /* Holds focus (optimization) */
#define AG_WIDGET_UNFOCUSED_MOTION	0x00004 /* All mousemotion events */
#define AG_WIDGET_UNFOCUSED_BUTTONUP	0x00008 /* All mousebuttonup events */
#define AG_WIDGET_UNFOCUSED_BUTTONDOWN	0x00010 /* All mousebuttondown events */
#define AG_WIDGET_HFILL			0x00040 /* Expand to fill width */
#define AG_WIDGET_VFILL			0x00080 /* Expand to fill height */
#define AG_WIDGET_HIDE			0x00200 /* Don't draw this widget */
#define AG_WIDGET_DISABLED		0x00400 /* Don't respond to input */
#define AG_WIDGET_CATCH_TAB		0x01000 /* Catch tab key events */
#define AG_WIDGET_PRIO_MOTION		0x02000 /* Block mousemotion events to
						  any other widget, regardless
						  of focus */
#define AG_WIDGET_UNDERSIZE		0x04000 /* Size allocation failed */
#define AG_WIDGET_NOSPACING		0x08000 /* Disable spacings around
						   widget; container-specific */
#define AG_WIDGET_UNFOCUSED_KEYDOWN	0x10000 /* All mousebuttondown events */
#define AG_WIDGET_UNFOCUSED_KEYUP	0x20000 /* All mousebuttondown events */
#define AG_WIDGET_DEBUG_RSENS		0x40000 /* Debug sensitivity rect */
#define AG_WIDGET_EXPAND		(AG_WIDGET_HFILL|AG_WIDGET_VFILL)

	int x, y;			/* Coordinates in container */
	int w, h;			/* Allocated geometry */
	AG_Rect2 rView;			/* Computed view coordinates */
	AG_Rect2 rSens;			/* Rectangle of sensitivity (i.e., to
					   cursor events), in view coords */

	AG_Style *style;		/* Style (inherited from parent) */

	AG_Surface **surfaces;		/* Registered surfaces */
	Uint *surfaceFlags;		/* Surface flags */
#define AG_WIDGET_SURFACE_NODUP	0x01	/* Don't free on destroy */
#define AG_WIDGET_SURFACE_REGEN	0x02	/* Texture needs to be regenerated */
	Uint nsurfaces;

	/* For OpenGL */
	Uint *textures;			/* Cached OpenGL textures */
	float *texcoords;		/* Cached texture coordinates */
	Uint *textureGC;		/* Textures queued for deletion */
	Uint nTextureGC;

	AG_Mutex bindings_lock;			 	/* Lock on bindings */
	AG_SLIST_HEAD(,ag_widget_binding) bindings;	/* Binding data */
	AG_SLIST_HEAD(,ag_popup_menu) menus;		/* Managed menus */
} AG_Widget;

#define AGWIDGET(wi)		((AG_Widget *)(wi))
#define AGWIDGET_OPS(wi)	((AG_WidgetClass *)OBJECT(wi)->cls)
#define AGWIDGET_SUPER_OPS(wi)	((AG_WidgetClass *)OBJECT(wi)->cls->super)

#define AGWIDGET_SURFACE(wi, ind)	AGWIDGET(wi)->surfaces[ind]
#define AGWIDGET_TEXTURE(wi, ind)	AGWIDGET(wi)->textures[ind]
#define AGWIDGET_TEXCOORD(wi, ind)	AGWIDGET(wi)->texcoords[(ind)*4]
#define AGWIDGET_SURFACE_NODUP(wi, ind) (AGWIDGET(wi)->surfaceFlags[ind] & AG_WIDGET_SURFACE_NODUP)
#define AGSTYLE(p)			 AGWIDGET(p)->style

#define AGWIDGET_FOREACH_CHILD(var, ob) AGOBJECT_FOREACH_CHILD(var, ob, ag_widget)
#define AGWIDGET_NEXT_CHILD(var) AGOBJECT_NEXT_CHILD((var),ag_widget)

#if defined(_AGAR_INTERNAL) || defined(_USE_AGAR_GUI)
#define WIDGET(wi)			AGWIDGET(wi)
#define WIDGET_OPS(wi)			AGWIDGET_OPS(wi)
#define WIDGET_SUPER_OPS(wi)		AGWIDGET_SUPER_OPS(wi)
#define WSURFACE(wi,ind)		AGWIDGET_SURFACE((wi),(ind))
#define WTEXTURE(wi,ind)		AGWIDGET_TEXTURE((wi),(ind))
#define WTEXCOORD(wi,ind)		AGWIDGET_TEXCOORD((wi),(ind))
#define WSURFACE_NODUP(wi,ind)		AGWIDGET_SURFACE_NODUP((wi),(ind))
#define STYLE(p)                        AGSTYLE(p)
#define WIDTH(p)			AGWIDGET(p)->w
#define HEIGHT(p)			AGWIDGET(p)->h
#define WIDGET_FOREACH_CHILD(var,ob)	AGWIDGET_FOREACH_CHILD(var,ob)
#define WIDGET_NEXT_CHILD(var)		AGWIDGET_NEXT_CHILD(var)
#endif

#define AG_WidgetFocused(wi)	(AGWIDGET(wi)->flags&AG_WIDGET_FOCUSED)
#define AG_WidgetDisabled(wi)	(AGWIDGET(wi)->flags&AG_WIDGET_DISABLED)
#define AG_WidgetEnabled(wi)	((AGWIDGET(wi)->flags&AG_WIDGET_DISABLED)==0)

struct ag_window;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agWidgetClass;
extern DECLSPEC int agKbdDelay;
extern DECLSPEC int agKbdRepeat;
extern DECLSPEC int agMouseDblclickDelay;
extern DECLSPEC int agMouseSpinDelay;
extern DECLSPEC int agMouseSpinIval;
extern DECLSPEC void AG_WidgetDraw(void *);
extern DECLSPEC void AG_WidgetSizeReq(void *, AG_SizeReq *);
extern DECLSPEC int AG_WidgetSizeAlloc(void *, AG_SizeAlloc *);
extern DECLSPEC void AG_WidgetSetFocusable(void *, int);
extern DECLSPEC void AG_WidgetFocus(void *);
extern DECLSPEC void AG_WidgetUnfocus(void *);
extern DECLSPEC AG_Widget *AG_WidgetFindFocused(void *);
extern DECLSPEC void *AG_WidgetFindPoint(const char *, int, int);
extern DECLSPEC void *AG_WidgetFindRect(const char *, int, int, int, int);
extern DECLSPEC void AG_WidgetUpdateCoords(void *, int, int);
extern DECLSPEC struct ag_window *AG_ParentWindow(void *);
#define AG_WidgetParentWindow(w) AG_ParentWindow(w)
extern DECLSPEC int AG_WidgetMapSurface(void *, AG_Surface *);
extern DECLSPEC int AG_WidgetMapSurfaceNODUP(void *, AG_Surface *);
extern DECLSPEC void AG_WidgetReplaceSurface(void *, int, AG_Surface *);
extern DECLSPEC void AG_WidgetReplaceSurfaceNODUP(void *, int, AG_Surface *);
extern DECLSPEC void AG_WidgetUpdateSurface(void *, int);
#define AG_WidgetUnmapSurface(w, n) AG_WidgetReplaceSurface((w),(n),NULL)
#ifdef HAVE_OPENGL
# define AG_WidgetUpdateSurface(wid,name) do { AGWIDGET(wid)->surfaceFlags[(name)] |= AG_WIDGET_SURFACE_REGEN; } while (0)
#else
# define AG_WidgetUpdateSurface(wid,name)
#endif
extern DECLSPEC void AG_WidgetBlit(void *, AG_Surface *, int, int);
extern DECLSPEC void AG_WidgetBlitFrom(void *, void *, int, AG_Rect *, int, int);
#define AG_WidgetBlitSurface(p,n,x,y) AG_WidgetBlitFrom((p),(p),(n),NULL,(x),(y))
#ifdef HAVE_OPENGL
extern DECLSPEC void AG_WidgetBlitGL(void *, AG_Surface *, float, float);
extern DECLSPEC void AG_WidgetBlitSurfaceGL(void *, int, float, float);
extern DECLSPEC void AG_WidgetBlitSurfaceFlippedGL(void *, int, float, float);
extern DECLSPEC void AG_WidgetPutPixel32_GL(void *, int, int, Uint32);
extern DECLSPEC void AG_WidgetPutPixelRGB_GL(void *, int, int, Uint8, Uint8, Uint8);
extern DECLSPEC void AG_WidgetFreeResourcesGL(AG_Widget *);
extern DECLSPEC void AG_WidgetRegenResourcesGL(AG_Widget *);
#endif
extern DECLSPEC void AG_PushClipRect(void *, AG_Rect);
extern DECLSPEC void AG_PopClipRect(void);
extern DECLSPEC void AG_SetCursor(int);
extern DECLSPEC void AG_UnsetCursor(void);
#define AG_WidgetPutPixel AG_WidgetPutPixel32
#define AG_WidgetBlendPixel AG_WidgetBlendPixelRGBA
extern DECLSPEC void AG_WidgetBlendPixelRGBA(void *, int, int, Uint8 [4], enum ag_blend_func);
extern DECLSPEC int AG_WidgetSensitive(void *, int, int);
extern DECLSPEC void AG_WidgetMouseMotion(struct ag_window *, AG_Widget *, int, int, int, int, int);
extern DECLSPEC void AG_WidgetMouseButtonUp(struct ag_window *, AG_Widget *, int, int, int);
extern DECLSPEC int AG_WidgetMouseButtonDown(struct ag_window *, AG_Widget *, int, int, int);
extern DECLSPEC void AG_WidgetUnfocusedKeyUp(AG_Widget *, int, int, int);
extern DECLSPEC void AG_WidgetUnfocusedKeyDown(AG_Widget *, int, int, int);
extern DECLSPEC AG_WidgetBinding *AG_WidgetBind(void *, const char *, AG_WidgetBindingType, ...);
extern DECLSPEC AG_WidgetBinding *AG_WidgetBindMp(void *, const char *, AG_Mutex *, AG_WidgetBindingType, ...);
extern DECLSPEC AG_WidgetBinding *AG_WidgetGetBinding(void *, const char *, ...);
extern DECLSPEC void AG_WidgetBindingChanged(AG_WidgetBinding *);
extern DECLSPEC int AG_WidgetCopyBinding(void *, const char *, AG_WidgetBinding *);
#define AG_WidgetBindBool(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_BOOL,(p))
#define AG_WidgetBindInt(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_INT,(p))
#define AG_WidgetBindUint(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_UINT,(p))
#define AG_WidgetBindUint8(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_UINT8,(p))
#define AG_WidgetBindSint8(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_SINT8,(p))
#define AG_WidgetBindUint16(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_UINT16,(p))
#define AG_WidgetBindSint16(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_SINT16,(p))
#define AG_WidgetBindUint32(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_UINT32,(p))
#define AG_WidgetBindSint32(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_SINT32,(p))
#define AG_WidgetBindUint64(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_UINT64,(p))
#define AG_WidgetBindSint64(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_SINT64,(p))
#define AG_WidgetBindFloat(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_FLOAT,(p))
#define AG_WidgetBindDouble(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_DOUBLE,(p))
#define AG_WidgetBindLongDouble(w,b,p) AG_WidgetBind((w),(b), AG_WIDGET_LONG_DOUBLE,(p))
#define AG_WidgetBindPointer(w,b,p) AG_WidgetBind((w),(b),AG_WIDGET_POINTER,(p))
#define AG_WidgetBindProp(w,b,o,k) AG_WidgetBind((w),(b),AG_WIDGET_PROP,(o),(k))
#define AG_WidgetBindString(w,b,p,len) AG_WidgetBind((w),(b),AG_WIDGET_STRING, (p),(len))
#define AG_WidgetBindFlag(w,b,p,mask) AG_WidgetBind((w),(b),AG_WIDGET_FLAG, (p),(mask))
#define AG_WidgetBindFlag8(w,b,p,mask) AG_WidgetBind((w),(b),AG_WIDGET_FLAG8, (p),(mask))
#define AG_WidgetBindFlag16(w,b,p,mask) AG_WidgetBind((w),(b),AG_WIDGET_FLAG16, (p),(mask))
#define AG_WidgetBindFlag32(w,b,p,mask) AG_WidgetBind((w),(b),AG_WIDGET_FLAG32, (p),(mask))
extern DECLSPEC enum ag_widget_sizespec AG_WidgetParseSizeSpec(const char *, int *);
extern DECLSPEC int AG_WidgetScrollDelta(Uint32 *);
extern DECLSPEC void *AG_WidgetFind(AG_Display *, const char *);
extern DECLSPEC void AG_WidgetShownRecursive(void *);
extern DECLSPEC void AG_WidgetHiddenRecursive(void *);
extern DECLSPEC void AG_WidgetSetString(void *, const char *, const char *);
extern DECLSPEC size_t AG_WidgetCopyString(void *, const char *, char *, size_t);
extern DECLSPEC void AG_WidgetInheritDraw(void *);
extern DECLSPEC void AG_WidgetInheritSizeRequest(void *, AG_SizeReq *);
extern DECLSPEC int AG_WidgetInheritSizeAllocate(void *, const AG_SizeAlloc *);
/*
 * Inlines
 */
#ifdef THREADS

static __inline__ void
AG_WidgetLockBinding(AG_WidgetBinding *bind)
{
	if (bind->mutex != NULL)
		AG_MutexLock(bind->mutex);
}

static __inline__ void
AG_WidgetUnlockBinding(AG_WidgetBinding *bind)
{
	if (bind->mutex != NULL)
		AG_MutexUnlock(bind->mutex);
}
#else
#define AG_WidgetLockBinding(b)
#define AG_WidgetUnlockBinding(b)
#endif 

static __inline__ void
AG_WidgetEnable(void *p)
{
	AG_ObjectLock(p);
	if (AGWIDGET(p)->flags & AG_WIDGET_DISABLED) {
		AGWIDGET(p)->flags &= ~(AG_WIDGET_DISABLED);
		AG_PostEvent(NULL, p, "widget-enabled", NULL);
	}
	AG_ObjectUnlock(p);
}

static __inline__ void
AG_WidgetDisable(void *p)
{
	AG_ObjectLock(p);
	if (!(AGWIDGET(p)->flags & AG_WIDGET_DISABLED)) {
		AGWIDGET(p)->flags |= AG_WIDGET_DISABLED;
		AG_PostEvent(NULL, p, "widget-disabled", NULL);
	}
	AG_ObjectUnlock(p);
}

static __inline__ int
AG_WidgetArea(void *p, int x, int y)
{
	AG_Widget *wid = AGWIDGET(p);

	return (x > wid->rView.x1 && y > wid->rView.y1 &&
	        x < wid->rView.x2 && y < wid->rView.y2);
}

static __inline__ int
AG_WidgetRelativeArea(void *p, int x, int y)
{
	AG_Widget *wid = AGWIDGET(p);

	return (x >= 0 &&
	        y >= 0 &&
	        x < wid->w &&
		y < wid->h);
}

static __inline__ void
AG_WidgetPutPixel32(void *p, int wx, int wy, Uint32 color)
{
	AG_Widget *wid = AGWIDGET(p);
	int vx = wid->rView.x1 + wx;
	int vy = wid->rView.y1 + wy;

#ifdef HAVE_OPENGL
	if (agView->opengl) {
		AG_WidgetPutPixel32_GL(p, vx,vy, color);
	} else
#endif
	if (!AG_CLIPPED_PIXEL(agView->v, vx,vy))
		AG_PUT_PIXEL2(agView->v, vx,vy, color);
}

static __inline__ void
AG_WidgetPutPixelRGB(void *p, int wx, int wy, Uint8 r, Uint8 g, Uint8 b)
{
	AG_Widget *wid = AGWIDGET(p);
	int vx = wid->rView.x1 + wx;
	int vy = wid->rView.y1 + wy;
	
#ifdef HAVE_OPENGL
	if (agView->opengl) {
		AG_WidgetPutPixelRGB_GL(p, vx,vy, r,g,b);
	} else
#endif
	if (!AG_CLIPPED_PIXEL(agView->v, vx, vy))
		AG_PUT_PIXEL2(agView->v, vx,vy, AG_MapRGB(agVideoFmt,r,g,b));
}

static __inline__ void
AG_WidgetBlendPixel32(void *p, int wx, int wy, Uint32 pixel, AG_BlendFn fn)
{
	Uint8 c[4];

	AG_GetRGBA(pixel, agSurfaceFmt, &c[0],&c[1],&c[2],&c[3]);
	AG_WidgetBlendPixelRGBA(p, wx,wy, c, fn);
}
#define AG_WidgetBool AG_WidgetInt

static __inline__ int
AG_WidgetInt(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	int *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (int **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Uint
AG_WidgetUint(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Uint *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Uint **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Uint8
AG_WidgetUint8(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Uint8 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Uint8 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Sint8
AG_WidgetSint8(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Sint8 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Sint8 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Uint16
AG_WidgetUint16(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Uint16 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Uint16 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Sint16
AG_WidgetSint16(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Sint16 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Sint16 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Uint32
AG_WidgetUint32(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Uint32 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Uint32 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Sint32
AG_WidgetSint32(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Sint32 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Sint32 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}
#ifdef HAVE_64BIT

static __inline__ Uint64
AG_WidgetUint64(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Uint64 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Uint64 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ Sint64
AG_WidgetSint64(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	Sint64 *i, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (Sint64 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *i;
	AG_WidgetUnlockBinding(b);
	return (rv);
}
#endif 

static __inline__ float
AG_WidgetFloat(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	float *f, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (float **)&f)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *f;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ double
AG_WidgetDouble(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	double *d, rv;

	if ((b = AG_WidgetGetBinding(wid, name, (double **)&d)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *d;
	AG_WidgetUnlockBinding(b);
	return (rv);
}

static __inline__ char *
AG_WidgetString(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	char *s, *sd;

	if ((b = AG_WidgetGetBinding(wid, name, (char **)&s)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	sd = AG_Strdup(s);
	AG_WidgetUnlockBinding(b);
	return (sd);
}

static __inline__ void *
AG_WidgetPointer(void *wid, const char *name)
{
	AG_WidgetBinding *b;
	void **p, *rv;

	if ((b = AG_WidgetGetBinding(wid, name, (void ***)&p)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	rv = *p;
	AG_WidgetUnlockBinding(b);
	return (p);
}
#define AG_WidgetSetBool AG_WidgetSetInt

static __inline__ void
AG_WidgetSetInt(void *wid, const char *name, int ni)
{
	AG_WidgetBinding *binding;
	int *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (int **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetUint(void *wid, const char *name, Uint ni)
{
	AG_WidgetBinding *binding;
	Uint *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Uint **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetUint8(void *wid, const char *name, Uint8 ni)
{
	AG_WidgetBinding *binding;
	Uint8 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Uint8 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetSint8(void *wid, const char *name, Sint8 ni)
{
	AG_WidgetBinding *binding;
	Sint8 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Sint8 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetUint16(void *wid, const char *name, Uint16 ni)
{
	AG_WidgetBinding *binding;
	Uint16 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Uint16 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetSint16(void *wid, const char *name, Sint16 ni)
{
	AG_WidgetBinding *binding;
	Sint16 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Sint16 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetUint32(void *wid, const char *name, Uint32 ni)
{
	AG_WidgetBinding *binding;
	Uint32 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Uint32 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetSint32(void *wid, const char *name, Sint32 ni)
{
	AG_WidgetBinding *binding;
	Sint32 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Sint32 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}
#ifdef HAVE_64BIT

static __inline__ void
AG_WidgetSetUint64(void *wid, const char *name, Uint64 ni)
{
	AG_WidgetBinding *binding;
	Uint64 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Uint64 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetSint64(void *wid, const char *name, Sint64 ni)
{
	AG_WidgetBinding *binding;
	Sint64 *i;

	if ((binding = AG_WidgetGetBinding(wid, name, (Sint64 **)&i)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*i != ni) {
		*i = ni;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}
#endif 

static __inline__ void
AG_WidgetSetFloat(void *wid, const char *name, float nf)
{
	AG_WidgetBinding *binding;
	float *f;

	if ((binding = AG_WidgetGetBinding(wid, name, (float **)&f)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*f != nf) {
		*f = nf;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_WidgetSetDouble(void *wid, const char *name, double nd)
{
	AG_WidgetBinding *binding;
	double *d;

	if ((binding = AG_WidgetGetBinding(wid, name, (double **)&d)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*d != nd) {
		*d = nd;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}
#ifdef HAVE_LONG_DOUBLE

static __inline__ void
AG_WidgetSetLongDouble(void *wid, const char *name, long double nd)
{
	AG_WidgetBinding *binding;
	long double *d;

	if ((binding = AG_WidgetGetBinding(wid, name,
	    (long double **)&d)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*d != nd) {
		*d = nd;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}
#endif

static __inline__ void
AG_WidgetSetPointer(void *wid, const char *name, void *np)
{
	AG_WidgetBinding *binding;
	void **p;

	if ((binding = AG_WidgetGetBinding(wid, name, (void ***)&p)) == NULL) {
		AG_FatalError("%s", AG_GetError());
	}
	if (*p != np) {
		*p = np;
		AG_WidgetBindingChanged(binding);
	}
	AG_WidgetUnlockBinding(binding);
}

static __inline__ void
AG_Expand(void *wid)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->flags |= AG_WIDGET_EXPAND;
	AG_ObjectUnlock(wid);
}

static __inline__ void
AG_ExpandHoriz(void *wid)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->flags |= AG_WIDGET_HFILL;
	AG_ObjectUnlock(wid);
}

static __inline__ void
AG_ExpandVert(void *wid)
{
	AG_ObjectLock(wid);
	AGWIDGET(wid)->flags |= AG_WIDGET_VFILL;
	AG_ObjectUnlock(wid);
}
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_H_ */