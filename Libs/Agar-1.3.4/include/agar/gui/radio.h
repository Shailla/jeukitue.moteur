/*	Public domain	*/

#ifndef _AGAR_WIDGET_RADIO_H_
#define _AGAR_WIDGET_RADIO_H_

#include <agar/gui/widget.h>

#include <agar/gui/begin.h>

typedef struct ag_radio_item {
	char text[128];
	int surface;
	SDLKey hotkey;
} AG_RadioItem;

typedef struct ag_radio {
	struct ag_widget wid;
	Uint flags;
#define AG_RADIO_HFILL	0x01
#define AG_RADIO_VFILL	0x02
#define AG_RADIO_EXPAND (AG_RADIO_HFILL|AG_RADIO_VFILL)
	int value;		/* Default value binding */
	AG_RadioItem *items;
	int nItems;
	int selitem;		/* Index of selected item */
	int max_w;		/* Width of widest label */
	int oversel;		/* Overlapping selection */
	int xPadding, yPadding;
	int xSpacing, ySpacing;
	int radius;		/* Control radius in pixels */
	AG_Rect r;		/* View area */
} AG_Radio;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC AG_WidgetClass agRadioClass;
extern DECLSPEC AG_Radio *AG_RadioNew(void *, Uint, const char *[]);
extern DECLSPEC AG_Radio *AG_RadioNewFn(void *, Uint, const char **, AG_EventFn, const char *, ...);
extern DECLSPEC AG_Radio *AG_RadioNewInt(void *, Uint, const char **, int *);
extern DECLSPEC AG_Radio *AG_RadioNewUint(void *, Uint, const char **, Uint *);
extern DECLSPEC void AG_RadioItemsFromArray(AG_Radio *, const char **);
extern DECLSPEC int AG_RadioAddItem(AG_Radio *, const char *, ...);
extern DECLSPEC int AG_RadioAddItemHK(AG_Radio *, SDLKey, const char *, ...);
extern DECLSPEC void AG_RadioClearItems(AG_Radio *);
__END_DECLS
/* Close generated block */

#include <agar/gui/close.h>
#endif /* _AGAR_WIDGET_RADIO_H_ */
