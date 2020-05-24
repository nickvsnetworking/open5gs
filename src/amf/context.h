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

#ifndef AMF_CONTEXT_H
#define AMF_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "ogs-sctp.h"
#include "ogs-ngap.h"
#include "ogs-nas-5gs.h"

#include "amf-sm.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_OF_SERVED_GUAMI     8
#define REGION_PER_AMF              256    /* According to spec it is 65535 */
#define SET_PER_AMF                 256    /* According to spec it is 1024 */

extern int __amf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __amf_log_domain

typedef struct served_guami_s {
    int             num_of_plmn_id;
    ogs_plmn_id_t   plmn_id[OGS_MAX_NUM_OF_PLMN_ID];

    int             num_of_amf_region_id;
    uint16_t        amf_region_id[REGION_PER_AMF];
    int             num_of_amf_set_id;
    uint16_t        amf_set_id[SET_PER_AMF];
} served_guami_t;

typedef struct amf_context_s {
    ogs_queue_t     *queue;         /* Queue for processing UPF control */
    ogs_timer_mgr_t *timer_mgr;     /* Timer Manager */
    ogs_pollset_t   *pollset;       /* Poll Set for I/O Multiplexing */

    OpenAPI_nf_type_e   nf_type;

    /* Served GUMME */
    uint8_t         max_num_of_served_guami;
    served_guami_t  served_guami[MAX_NUM_OF_SERVED_GUAMI];

    /* Served TAI */
    uint8_t         num_of_served_tai;
    struct {
        ogs_tai0_list_t list0;
        ogs_tai2_list_t list2;
    } served_tai[OGS_MAX_NUM_OF_SERVED_TAI];

    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EEA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EEA2    2
     * #define NAS_SECURITY_ALGORITHMS_128_EEA3    3 */
    uint8_t         num_of_ciphering_order;
    uint8_t         ciphering_order[OGS_MAX_NUM_OF_ALGORITHM];
    /* defined in 'nas_ies.h'
     * #define NAS_SECURITY_ALGORITHMS_EIA0        0
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    1
     * #define NAS_SECURITY_ALGORITHMS_128_EIA1    2
     * #define NAS_SECURITY_ALGORITHMS_128_EIA3    3 */
    uint8_t         num_of_integrity_order;
    uint8_t         integrity_order[OGS_MAX_NUM_OF_ALGORITHM];

    /* Network Name */    
    ogs_nas_network_name_t short_name; /* Network short name */
    ogs_nas_network_name_t full_name; /* Network Full Name */

    /* NGSetupResponse */
    uint8_t         relative_capacity;

    uint16_t        ngap_port;      /* Default NGAP Port */

    ogs_list_t      ngap_list;      /* AMF NGAP IPv4 Server List */
    ogs_list_t      ngap_list6;     /* AMF NGAP IPv6 Server List */

    ogs_list_t      gnb_list;       /* ENB S1AP Client List */
    ogs_hash_t      *gnb_addr_hash; /* hash table for GNB Address */
    ogs_hash_t      *gnb_id_hash;   /* hash table for GNB-ID */
} amf_context_t;

typedef struct amf_gnb_s {
    ogs_lnode_t     lnode;

    ogs_fsm_t       sm;         /* A state machine */

    uint32_t        gnb_id;     /* eNB_ID received from eNB */
    int             sock_type;  /* SOCK_STREAM or SOCK_SEQPACKET */
    ogs_sock_t      *sock;      /* eNB S1AP Socket */
    ogs_sockaddr_t  *addr;      /* eNB S1AP Address */
    ogs_poll_t      *poll;      /* eNB S1AP Poll */

    struct {
        bool s1_setup_success;  /* eNB S1AP Setup complete successfuly */
    } state;

    uint16_t        max_num_of_ostreams;/* SCTP Max num of outbound streams */
    uint16_t        ostream_id;         /* gnb_ostream_id generator */


    uint8_t         num_of_supported_ta_list;
    ogs_tai_t       supported_ta_list[OGS_MAX_NUM_OF_TAI*OGS_MAX_NUM_OF_BPLMN];

    ogs_list_t      gnb_ue_list;

} amf_gnb_t;

void amf_context_init(void);
void amf_context_final(void);
amf_context_t *amf_self(void);

int amf_context_parse_config(void);

amf_gnb_t *amf_gnb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr);
int amf_gnb_remove(amf_gnb_t *gnb);
int amf_gnb_remove_all(void);
amf_gnb_t *amf_gnb_find_by_addr(ogs_sockaddr_t *addr);
amf_gnb_t *amf_gnb_find_by_gnb_id(uint32_t gnb_id);
int amf_gnb_set_gnb_id(amf_gnb_t *gnb, uint32_t gnb_id);
int amf_gnb_sock_type(ogs_sock_t *sock);
bool amf_is_maximum_number_of_gnbs_reached(void);

#ifdef __cplusplus
}
#endif

#endif /* AMF_CONTEXT_H */
