/*
 * Copyright 2009 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Authors:
 *      "Daniel Kopecek" <dkopecek@redhat.com>
 */

#pragma once
#ifndef _SEXP_OUTPUT_H
#define _SEXP_OUTPUT_H

#include <stddef.h>
#include <stdint.h>
#include "public/sexp-types.h"
#include "public/sexp-output.h"
#include "../../../common/util.h"
#include "generic/strbuf.h"


struct SEXP_ostate {
        SEXP_t        *s_exp;   /* ref to the S-exp that we are writing */
        uint32_t      *p_stack; /* list position stack */
        uint8_t       *s_part;  /* S-exp part */
        size_t        *s_pos;   /* S-exp position */
};


#endif /* _SEXP_OUTPUT_H */
