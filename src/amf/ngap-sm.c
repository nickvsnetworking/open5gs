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

#include "ogs-gtp.h"
#include "ogs-nas-eps.h"

#include "ngap-build.h"
#include "ngap-handler.h"
#include "ngap-path.h"

#include "amf-event.h"
#include "amf-timer.h"
#include "amf-sm.h"

void ngap_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);

    OGS_FSM_TRAN(s, &ngap_state_operational);
}

void ngap_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);
}

void ngap_state_operational(ogs_fsm_t *s, amf_event_t *e)
{
    amf_enb_t *enb = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    NGAP_NGAP_PDU_t *pdu = NULL;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    enb = e->enb;
    ogs_assert(enb);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVT_NGAP_MESSAGE:
        pdu = e->ngap_message;
        ogs_assert(pdu);

        switch (pdu->present) {
        case NGAP_NGAP_PDU_PR_initiatingMessage :
            initiatingMessage = pdu->choice.initiatingMessage;
            ogs_assert(initiatingMessage);

            switch (initiatingMessage->procedureCode) {
            case NGAP_ProcedureCode_id_S1Setup :
                ngap_handle_s1_setup_request(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_initialUEMessage :
                ngap_handle_initial_ue_message(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_uplinkNASTransport :
                ngap_handle_uplink_nas_transport(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_UECapabilityInfoIndication :
                ngap_handle_ue_capability_info_indication( enb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextReleaseRequest:
                ngap_handle_ue_context_release_request( enb, pdu);
                break;
            case NGAP_ProcedureCode_id_PathSwitchRequest:
                ngap_handle_path_switch_request(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_eNBConfigurationTransfer:
                pkbuf = e->pkbuf;
                ogs_assert(pkbuf);

                ngap_handle_enb_configuration_transfer(enb, pdu, pkbuf);
                break;
            case NGAP_ProcedureCode_id_HandoverPreparation:
                ngap_handle_handover_required(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverCancel:
                ngap_handle_handover_cancel(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_eNBStatusTransfer:
                ngap_handle_enb_status_transfer(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverNotification:
                ngap_handle_handover_notification(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_Reset:
                ngap_handle_s1_reset(enb, pdu);
                break;
            default:
                ogs_warn("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)initiatingMessage->procedureCode);
                break;
            }
            break;
        case NGAP_NGAP_PDU_PR_successfulOutcome :
            successfulOutcome = pdu->choice.successfulOutcome;
            ogs_assert(successfulOutcome);

            switch (successfulOutcome->procedureCode) {
            case NGAP_ProcedureCode_id_InitialContextSetup:
                ngap_handle_initial_context_setup_response(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextModification:
                ngap_handle_ue_context_modification_response(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextRelease:
                ngap_handle_ue_context_release_complete(
                        enb, pdu);
                break;
            case NGAP_ProcedureCode_id_E_RABSetup:
                ngap_handle_e_rab_setup_response(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_E_RABModify:
                break;
            case NGAP_ProcedureCode_id_E_RABRelease:
                break;
            case NGAP_ProcedureCode_id_HandoverResourceAllocation:
                ngap_handle_handover_request_ack(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_WriteReplaceWarning:
                ngap_handle_write_replace_warning_response(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_Kill:
                ngap_handle_kill_response(enb, pdu);
                break;
            default:
                ogs_warn("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)successfulOutcome->procedureCode);
                break;
            }
            break;
        case NGAP_NGAP_PDU_PR_unsuccessfulOutcome :
            unsuccessfulOutcome = pdu->choice.unsuccessfulOutcome;
            ogs_assert(unsuccessfulOutcome);

            switch (unsuccessfulOutcome->procedureCode) {
            case NGAP_ProcedureCode_id_InitialContextSetup :
                ngap_handle_initial_context_setup_failure(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextModification:
                ngap_handle_ue_context_modification_failure(enb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverResourceAllocation :
                ngap_handle_handover_failure(enb, pdu);
                break;
            default:
                ogs_warn("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)unsuccessfulOutcome->procedureCode);
                break;
            }
            break;
        default:
            ogs_warn("Not implemented(choice:%d)", pdu->present);
            break;
        }

        break;
    case AMF_EVT_NGAP_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_S1_DELAYED_SEND:
            ogs_assert(e->enb_ue);
            ogs_assert(e->pkbuf);

            ogs_expect(OGS_OK == ngap_send_to_enb_ue(e->enb_ue, e->pkbuf));
            ogs_timer_delete(e->timer);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

void ngap_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

