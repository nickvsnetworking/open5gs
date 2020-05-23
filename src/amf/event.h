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

#ifndef AMF_EVENT_H
#define AMF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_server_s ogs_sbi_server_t;
typedef struct ogs_sbi_session_s ogs_sbi_session_t;
typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef enum {
    AMF_EVT_BASE = OGS_FSM_USER_SIG,

    AMF_EVT_SBI_SERVER,
    AMF_EVT_SBI_CLIENT,
    AMF_EVT_SBI_TIMER,

    AMF_EVT_TOP,

} amf_event_e;

typedef struct amf_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;

    struct {
        /* OGS_EVT_SBI_SERVER */
        ogs_sbi_request_t *request;
        ogs_sbi_session_t *session;
        ogs_sbi_server_t *server;

        /* OGS_EVT_SBI_CLIENT */
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

} amf_event_t;

void amf_event_init(void);
void amf_event_final(void);

amf_event_t *amf_event_new(amf_event_e id);
void amf_event_free(amf_event_t *e);

const char *amf_event_get_name(amf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* AMF_EVENT_H */
