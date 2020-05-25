/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-ngap.h"

void ogs_ngap_uint32_to_GNB_ID(uint32_t gnb_id, NGAP_GNB_ID_t *gNB_ID)
{
    BIT_STRING_t *bit_string = NULL;

    ogs_assert(gNB_ID);

    bit_string = &gNB_ID->choice.gNB_ID;
    ogs_assert(bit_string);

    gNB_ID->present = NGAP_GNB_ID_PR_gNB_ID;
    bit_string->size = 3;
    bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));

    bit_string->buf[0] = gnb_id >> 16;
    bit_string->buf[1] = gnb_id >> 8;
    bit_string->buf[2] = (gnb_id & 0xff);
}

void ogs_ngap_GNB_ID_to_uint32(NGAP_GNB_ID_t *gNB_ID, uint32_t *gnb_id)
{
    uint8_t *buf = NULL;

    ogs_assert(gnb_id);
    ogs_assert(gNB_ID);

    buf = gNB_ID->choice.gNB_ID.buf;
    ogs_assert(buf);

    *gnb_id = (buf[0] << 16) + (buf[1] << 8) + buf[2];
}
