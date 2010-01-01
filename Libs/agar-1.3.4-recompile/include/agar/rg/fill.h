/*	Public domain	*/

#ifndef _AGAR_RG_FILL_H_
#define _AGAR_RG_FILL_H_
#include <agar/rg/begin.h>

struct ag_window;
struct ag_toolbar;

enum rg_fill_type {
	FILL_SOLID,
	FILL_HGRADIENT,
	FILL_VGRADIENT,
	FILL_CGRADIENT,
	FILL_PATTERN
};

struct rg_fill_feature {
	RG_Feature ft;
	enum rg_fill_type type;
	Uint8 alpha;
	union {
		struct {
			Uint32 c;
		} solid;
		struct {
			Uint32 c1;
			Uint32 c2;
		} gradient;
		struct {
			int texid;
			int tex_xoffs;
			int tex_yoffs;
		} pattern;
	} args;
#ifndef _AGAR_RG_PUBLIC_H_
#define f_solid args.solid
#define f_gradient args.gradient
#define f_pattern args.pattern
#endif
};

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC void RG_FillInit(void *, RG_Tileset *, int);
extern DECLSPEC int RG_FillLoad(void *, AG_DataSource *);
extern DECLSPEC void RG_FillSave(void *, AG_DataSource *);
extern DECLSPEC void RG_FillApply(void *, RG_Tile *, int, int);
extern DECLSPEC void RG_FillMenu(void *, AG_MenuItem *);
extern DECLSPEC struct ag_window *RG_FillEdit(void *, RG_Tileview *);
extern DECLSPEC struct ag_toolbar *RG_FillToolbar(void *, RG_Tileview *);
__END_DECLS
/* Close generated block */

#include <agar/rg/close.h>
#endif /* _AGAR_RG_FILL_H_ */
