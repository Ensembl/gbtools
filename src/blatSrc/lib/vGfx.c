/*
 * Copyright [2018-2021] EMBL-European Bioinformatics Institute
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* vGfx - interface to polymorphic graphic object
 * that currently can either be a memory buffer or
 * a postScript file. */

/* Copyright (C) 2011 The Regents of the University of California 
 * See README in this or parent directory for licensing information. */

#include "common.h"
#include "vGfx.h"



/* Most of the implementation of this is in macros in vGfx.h. */

void vgClose(struct vGfx **pVg)
/* Close down virtual graphics object, and finish writing it to file. */
{
struct vGfx *vg = *pVg;
if (vg != NULL)
    {
    vg->close(&vg->data);
    freez(pVg);
    }
}

struct vGfx *vgHalfInit(int width, int height)
/* Close down virtual graphics object, and finish writing it to file. */
{
struct vGfx *vg;
AllocVar(vg);
vg->width = width;
vg->height = height;
return vg;
}

int vgFindRgb(struct vGfx *vg, struct rgbColor *rgb)
/* Find color index corresponding to rgb color. */
{
return vgFindColorIx(vg, rgb->r, rgb->g, rgb->b);
}

Color vgContrastingColor(struct vGfx *vg, int backgroundIx)
/* Return black or white whichever would be more visible over
 * background. */
{
struct rgbColor c = vgColorIxToRgb(vg, backgroundIx);
int val = (int)c.r + c.g + c.g + c.b;
if (val > 512)
    return MG_BLACK;
else
    return MG_WHITE;
}

