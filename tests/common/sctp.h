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

#ifndef TEST_COMMON_SCTP_H
#define TEST_COMMON_SCTP_H

#ifdef __cplusplus
extern "C" {
#endif

extern ogs_sockaddr_t ogs_test_sctp_last_addr;

ogs_socknode_t *testsctp_server(const char *ipstr, int port);
ogs_socknode_t *testsctp_client(const char *ipstr, int port);
ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type);

#define testgnb_ngap_client(x) testsctp_client(x, OGS_NGAP_SCTP_PORT)
#define testgnb_ngap_read(x) testsctp_read(x, 0);
#if 0
int testgnb_ngap_send(ogs_socknode_t *node, ogs_pkbuf_t *sendbuf);
#endif
#define testgnb_ngap_close ogs_socknode_free

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_SCTP_H */
