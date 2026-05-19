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
#define	ourUlEnh	(values[(int) enhUlColor])
#define	ourFont		(values[(int) enhFont])
#define	ourVideo	(values[(int) enhVideo])

/*
** Resolve an encoded enhValue colour to an X Pixel.
**
** The colour fields carry a packed (mode, payload) value:
**   ENH_MODE_DEFAULT  -> the widget's default fg / bg from colorPairs[0]
**   ENH_MODE_INDEXED  -> payload 1..16 picks colorPairs[1..16] directly
**                        (1..8 standard ANSI, 9..16 bright variants);
**                        payload 17..256 routes through the xterm 256-colour
**                        palette resolver (xterm colour 16..255)
**   ENH_MODE_RGB      -> direct 0xRRGGBB; wired up in a later change
**
** For foregrounds, the xterm bold-brightens-fg convention promotes an
** INDEXED 1..8 payload to its bright twin 9..16 when SGR 1 (BOLD) is set.
** SGR 2 (HALF_BRIGHT) picks the precomputed `hb` pixel on the underlying
** colorPair when one is available.  Backgrounds do not get either rule.
*/
static Pixel
_DtTermResolveFgPixel(Widget w, enhValue v, unsigned int videoFlags)
{
    DtTermWidget tw = (DtTermWidget) w;
    DtTermData td = tw->vt.td;
    int slot;

    if (ENH_COLOR_MODE(v) == ENH_MODE_INDEXED) {
	slot = (int) ENH_COLOR_PAYLOAD(v);
	if (tw->vt.boldColors && IS_BOLD(videoFlags) && slot >= 1 && slot <= 8) {
	    slot += 8;
	}
	if (slot >= 1 && slot <= 16) {
	    if (!td->colorPairs[slot].initialized) {
		(void) _DtTermColorInitializeColorPair(w, &td->colorPairs[slot]);
	    }
	    if (IS_HALF_BRIGHT(videoFlags) && td->colorPairs[slot].hbValid) {
		return td->colorPairs[slot].hb.pixel;
	    }
	    return td->colorPairs[slot].fg.pixel;
	}
	if (slot >= 17 && slot <= 256) {
	    return _DtTermResolve256Pixel(w, (unsigned int) (slot - 1));
	}
    }
    if (ENH_COLOR_MODE(v) == ENH_MODE_RGB) {
	unsigned int rgb = ENH_COLOR_PAYLOAD(v);
	return _DtTermResolveRGBPixel(w,
		(rgb >> 16) & 0xffU,
		(rgb >>  8) & 0xffU,
		 rgb        & 0xffU,
		False);
    }
    /* DEFAULT or unsupported mode -> widget default fg */
    if (!td->colorPairs[0].initialized) {
	(void) _DtTermColorInitializeColorPair(w, &td->colorPairs[0]);
    }
    if (IS_HALF_BRIGHT(videoFlags) && td->colorPairs[0].hbValid) {
	return td->colorPairs[0].hb.pixel;
    }
    return td->colorPairs[0].fg.pixel;
}

static Pixel
_DtTermResolveBgPixel(Widget w, enhValue v)
{
    DtTermWidget tw = (DtTermWidget) w;
    DtTermData td = tw->vt.td;
    int slot;

    if (ENH_COLOR_MODE(v) == ENH_MODE_INDEXED) {
	slot = (int) ENH_COLOR_PAYLOAD(v);
	if (slot >= 1 && slot <= 16) {
	    if (!td->colorPairs[slot].initialized) {
		(void) _DtTermColorInitializeColorPair(w, &td->colorPairs[slot]);
	    }
	    return td->colorPairs[slot].bg.pixel;
	}
	if (slot >= 17 && slot <= 256) {
	    return _DtTermResolve256Pixel(w, (unsigned int) (slot - 1));
	}
    }
    if (ENH_COLOR_MODE(v) == ENH_MODE_RGB) {
	unsigned int rgb = ENH_COLOR_PAYLOAD(v);
	return _DtTermResolveRGBPixel(w,
		(rgb >> 16) & 0xffU,
		(rgb >>  8) & 0xffU,
		 rgb        & 0xffU,
		True);
    }
    if (!td->colorPairs[0].initialized) {
	(void) _DtTermColorInitializeColorPair(w, &td->colorPairs[0]);
    }
    return td->colorPairs[0].bg.pixel;
}

void
_DtTermEnhProc(Widget w, enhValues values, TermEnhInfo info)
{
    DtTermWidget tw = (DtTermWidget) w;
    DtTermData td = tw->vt.td;

    info->fg = _DtTermResolveFgPixel(w, ourFgEnh, ourVideo);
    info->bg = _DtTermResolveBgPixel(w, ourBgEnh);

    /* if inverse video, swap fg and bg... */
    if (IS_INVERSE(ourVideo)) {
	Pixel tmp;

	tmp = info->fg;
	info->fg = info->bg;
	info->bg = tmp;
    }

    /*
    ** Underline colour: when SGR 58 has set ourUlEnh, resolve it via the
    ** foreground path (no bold-brighten promotion).  Otherwise fall back
    ** to info->fg so plain SGR 4 keeps its historical look.
    */
    if (ENH_IS_DEFAULT(ourUlEnh)) {
	info->ulFg = info->fg;
    } else {
	info->ulFg = _DtTermResolveFgPixel(w, ourUlEnh, 0 /* no bold */);
    }

    info->flags = (unsigned long) 0;
    if (IS_SECURE(ourVideo)) {
	info->flags |= TermENH_SECURE;
    }
    if (IS_UNDERLINE(ourVideo) || IS_LINK_ACTIVE(ourVideo)) {
	/* OSC 8 hyperlinks render as underlined even when the app
	** hasn't emitted an explicit SGR 4. */
	info->flags |= TermENH_UNDERLINE;
    }
    if (IS_DOUBLE_UNDERLINE(ourVideo)) {
	info->flags |= TermENH_DOUBLE_UNDERLINE;
    }
    if (IS_OVERLINE(ourVideo)) {
	info->flags |= TermENH_OVERLINE;
    }
    if (IS_SUPERSCRIPT(ourVideo)) {
	info->flags |= TermENH_SUPERSCRIPT;
    }
    if (IS_SUBSCRIPT(ourVideo)) {
	info->flags |= TermENH_SUBSCRIPT;
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
