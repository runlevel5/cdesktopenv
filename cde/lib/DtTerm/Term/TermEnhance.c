/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/*                                                                      *
 * (c) Copyright 1993, 1994 Hewlett-Packard Company                     *
 * (c) Copyright 1993, 1994 International Business Machines Corp.       *
 * (c) Copyright 1993, 1994 Sun Microsystems, Inc.                      *
 * (c) Copyright 1993, 1994 Novell, Inc.                                *
 */

#include "TermHeader.h"
#include <X11/X.h>
#include "TermP.h"
#include "TermColor.h"
#include "TermBuffer.h"

#define	ourFgEnh	(values[(int) enhFgColor])
#define	ourBgEnh	(values[(int) enhBgColor])
#define	ourFont		(values[(int) enhFont])
#define	ourVideo	(values[(int) enhVideo])

/*
** Resolve an encoded enhValue colour to a slot in td->colorPairs[].
**
** The colour fields carry a packed (mode, payload) value.  The parser
** currently produces ENH_MODE_DEFAULT (-> slot 0, the widget's default
** fg / bg) and ENH_MODE_INDEXED with payload 1..8 (-> slots 1..8, the 8 ANSI
** colours).  Anything else is clamped to slot 0 so the resolver stays in
** range while the palette is extended later.
*/
static int
_DtTermResolveColorPair(enhValue v)
{
    unsigned int payload;

    if (ENH_IS_DEFAULT(v)) {
	return 0;
    }
    if (ENH_COLOR_MODE(v) == ENH_MODE_INDEXED) {
	payload = ENH_COLOR_PAYLOAD(v);
	if (payload <= 8) {
	    return (int) payload;
	}
    }
    /* mode not yet supported by the resolver -- fall back to default */
    return 0;
}

void
_DtTermEnhProc(Widget w, enhValues values, TermEnhInfo info)
{

    DtTermWidget tw = (DtTermWidget) w;
    DtTermData td = tw->vt.td;
    int fgPair = _DtTermResolveColorPair(ourFgEnh);
    int bgPair = _DtTermResolveColorPair(ourBgEnh);

    /* initialize the color pair if we need to... */
    if (!td->colorPairs[fgPair].initialized) {
	(void) _DtTermColorInitializeColorPair(w,
		&td->colorPairs[fgPair]);
    }
    if (!td->colorPairs[bgPair].initialized) {
	(void) _DtTermColorInitializeColorPair(w,
		&td->colorPairs[bgPair]);
    }

    /* take care of video enhancements...
     */
    /* half bright (picks fg color) ... */
    if (IS_HALF_BRIGHT(ourVideo) && td->colorPairs[fgPair].hbValid) {
	info->fg = td->colorPairs[fgPair].hb.pixel;
    } else {
	info->fg = td->colorPairs[fgPair].fg.pixel;
    }

    /* background is always background... */
    info->bg = td->colorPairs[bgPair].bg.pixel;

    /* if inverse video, swap fg and bg... */
    if (IS_INVERSE(ourVideo)) {
	Pixel tmp;

	tmp = info->fg;
	info->fg = info->bg;
	info->bg = tmp;
    }

    info->flags = (unsigned long) 0;
    if (IS_SECURE(ourVideo)) {
	info->flags |= TermENH_SECURE;
    }
    if (IS_UNDERLINE(ourVideo)) {
	info->flags |= TermENH_UNDERLINE;
    }

    info->font = td->renderFonts[RENDER_FONT_NORMAL].termFont;
    if (IS_BOLD(ourVideo)) {
	if (ourFont == FONT_NORMAL) {
	    if (td->renderFonts[RENDER_FONT_BOLD].termFont) {
		/* valid bold font -- use it... */
		info->font = td->renderFonts[RENDER_FONT_BOLD].termFont;
	    } else {
		/* embolden via overstrike... */
		info->flags |= TermENH_OVERSTRIKE;
	    }
	} else {
	    /* DKS: can't do bold with the linedraw font... */
	    info->font = td->renderFonts[RENDER_FONT_LINEDRAW].termFont;
	}
    } else {
	/* not bold -- default is FONT_NORMAL... */
	if (ourFont == FONT_LINEDRAW) {
	    info->font = td->renderFonts[RENDER_FONT_LINEDRAW].termFont;
	}
    }
		
    return;
}
