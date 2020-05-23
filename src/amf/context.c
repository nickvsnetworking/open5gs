/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-dbi.h"
#include "context.h"

static amf_context_t self;

int __amf_log_domain;

static int context_initialized = 0;

void amf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize AMF context */
    memset(&self, 0, sizeof(amf_context_t));

    ogs_log_install_domain(&__amf_log_domain, "amf", ogs_core()->log.level);

    context_initialized = 1;
}

void amf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    context_initialized = 0;
}

amf_context_t *amf_self(void)
{
    return &self;
}

static int amf_context_prepare(void)
{
    self.nf_type = OpenAPI_nf_type_AMF;
    return OGS_OK;
}

static int amf_context_validation(void)
{
    return OGS_OK;
}

int amf_context_parse_config(void)
{
    int rv;

    rv = amf_context_prepare();
    if (rv != OGS_OK) return rv;

    rv = amf_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}
