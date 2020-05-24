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

#ifndef NGAP_PATH_H
#define NGAP_PATH_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NGAP_NON_UE_SIGNALLING   0

#define ngap_event_push  amf_sctp_event_push

int ngap_open(void);
void ngap_close(void);

ogs_sock_t *ngap_server(ogs_socknode_t *node);
void ngap_recv_upcall(short when, ogs_socket_t fd, void *data);
int ngap_send(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr, uint16_t stream_no);

#if 0
int ngap_send_to_enb(
        amf_enb_t *enb, ogs_pkbuf_t *pkb, uint16_t stream_no);
int ngap_send_to_enb_ue(enb_ue_t *enb_ue, ogs_pkbuf_t *pkbuf);
int ngap_delayed_send_to_enb_ue(enb_ue_t *enb_ue,
        ogs_pkbuf_t *pkbuf, ogs_time_t duration);
int ngap_send_to_nas(enb_ue_t *enb_ue,
        NGAP_ProcedureCode_t procedureCode, NGAP_NAS_PDU_t *nasPdu);
int ngap_send_to_esm(amf_ue_t *amf_ue, ogs_pkbuf_t *esmbuf);

void ngap_send_s1_setup_response(amf_enb_t *enb);
void ngap_send_s1_setup_failure(
        amf_enb_t *enb, NGAP_Cause_PR group, long cause);

void ngap_send_initial_context_setup_request(amf_ue_t *amf_ue);
void ngap_send_ue_context_modification_request(amf_ue_t *amf_ue);
void ngap_send_ue_context_release_command(
    enb_ue_t *enb_ue, NGAP_Cause_PR group, long cause,
    uint8_t action, uint32_t delay);

void ngap_send_paging(amf_ue_t *amf_ue, NGAP_CNDomain_t cn_domain);

void ngap_send_amf_configuration_transfer(
        amf_enb_t *target_enb,
        NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer);

void ngap_send_path_switch_ack(amf_ue_t *amf_ue);

void ngap_send_handover_command(enb_ue_t *source_ue);
void ngap_send_handover_preparation_failure(
        enb_ue_t *source_ue, NGAP_Cause_t *cause);

void ngap_send_handover_request(
        amf_ue_t *amf_ue,
        amf_enb_t *target_enb,
        NGAP_ENB_UE_NGAP_ID_t *enb_ue_ngap_id,
        NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
        NGAP_HandoverType_t *handovertype,
        NGAP_Cause_t *cause,
        NGAP_Source_ToTarget_TransparentContainer_t
            *source_totarget_transparentContainer);

void ngap_send_handover_cancel_ack(enb_ue_t *source_ue);

void ngap_send_amf_status_transfer(
        enb_ue_t *target_ue,
        NGAP_ENB_StatusTransfer_TransparentContainer_t
            *enb_statustransfer_transparentContainer);
void ngap_send_error_indication(
        amf_enb_t *enb,
        NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
        NGAP_ENB_UE_NGAP_ID_t *enb_ue_ngap_id,
        NGAP_Cause_PR group, long cause);
void ngap_send_s1_reset_ack(
        amf_enb_t *enb,
        NGAP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NGAP_PATH_H */
