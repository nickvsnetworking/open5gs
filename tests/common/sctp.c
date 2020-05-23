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

#include "test-common.h"

ogs_socknode_t *testsctp_server(const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, OGS_SGSAP_SCTP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_server(SOCK_SEQPACKET, node);
    ogs_assert(node->sock);

    return node;
}

ogs_socknode_t *testsctp_client(const char *ipstr)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, ipstr, OGS_S1AP_SCTP_PORT, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);
    ogs_socknode_nodelay(node, true);

    ogs_sctp_client(SOCK_STREAM, node);
    ogs_assert(node->sock);

    return node;
}

ogs_sockaddr_t ogs_test_sctp_last_addr;

ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type)
{
    int size;
    ogs_pkbuf_t *recvbuf = NULL;

    ogs_assert(node);
    ogs_assert(node->sock);

    recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    size = ogs_sctp_recvdata(node->sock, recvbuf->data, OGS_MAX_SDU_LEN,
            type == 1 ? &ogs_test_sctp_last_addr : NULL, NULL);
    if (size <= 0) {
        ogs_error("sgsap_recv() failed");
        return NULL;
    }

    ogs_pkbuf_trim(recvbuf, size);
    return recvbuf;;
}