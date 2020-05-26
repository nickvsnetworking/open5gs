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

#if 0
#include "amf-kdf.h"
#endif
#include "ngap-handler.h"
#if 0
#include "5gmm-handler.h"
#include "5gmm-build.h"
#include "5gsm-handler.h"
#include "nas-path.h"
#endif
#include "nas-security.h"
#include "ngap-path.h"
#if 0
#include "sgsap-types.h"
#include "sgsap-path.h"
#include "amf-path.h"
#endif
#include "amf-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __5gmm_log_domain

void gmm_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);

    OGS_FSM_TRAN(s, &gmm_state_de_registered);
}

void gmm_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);
}

static void common_register_state(ogs_fsm_t *s, amf_event_t *e);

void gmm_state_de_registered(ogs_fsm_t *s, amf_event_t *e)
{
#if 0
    amf_ue_t *amf_ue = NULL;
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(amf_ue);
        CLEAR_MME_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        break;
    }
#endif

    common_register_state(s, e);
}

void gmm_state_registered(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    common_register_state(s, e);
}

static void common_register_state(ogs_fsm_t *s, amf_event_t *e)
{
#if 0
    int rv;

    amf_ue_t *amf_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    S1AP_ProcedureCode_t procedureCode;
    
    ogs_assert(e);
        
    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return;
    case OGS_FSM_EXIT_SIG:
        return;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        enb_ue = amf_ue->enb_ue;
        ogs_assert(enb_ue);

        if (message->5gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("[EMM] Service request");
            rv = 5gmm_handle_service_request(
                    amf_ue, &message->5gmm.service_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("[EMM] Service request : Unknown UE");
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
                ogs_warn("No Session Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);
            return;
        }

        switch (message->5gmm.h.message_type) {
        case OGS_NAS_EPS_IDENTITY_RESPONSE:
            ogs_debug("[EMM] Identity response");
            CLEAR_MME_UE_TIMER(amf_ue->t3470);

            rv = 5gmm_handle_identity_response(amf_ue,
                    &message->5gmm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(amf_ue)) {
                ogs_error("No IMSI");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_debug("[EMM] Attach request[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_attach_request(
                    amf_ue, &message->5gmm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("[EMM] Tracking area update request");
            rv = 5gmm_handle_tau_request(
                    amf_ue, &message->5gmm.tracking_area_update_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("[EMM] TAU request : Unknown UE");
                nas_eps_send_tau_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_debug("[EMM] Tracking area update complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            return;
        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST:
            ogs_debug("[EMM] Extended service request");
            rv = 5gmm_handle_extended_service_request(
                    amf_ue, &message->5gmm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!MME_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("[EMM] Extended Service request : Unknown UE");
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->5gmm.5gmm_status.5gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_detach_request(
                    amf_ue, &message->5gmm.detach_request_from_ue);
            if (rv != OGS_OK)
            {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (MME_P_TMSI_IS_AVAILABLE(amf_ue)) {
                sgsap_send_detach_indication(amf_ue);
            } else {
                amf_send_delete_session_or_detach(amf_ue);
            }

            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            return;
        case OGS_NAS_EPS_UPLINK_NAS_TRANSPORT:
            ogs_debug("[EMM] Uplink NAS Transport");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            if (MME_SGSAP_IS_CONNECTED(amf_ue)) {
                sgsap_send_uplink_unitdata(amf_ue, &message->5gmm.
                        uplink_nas_transport.nas_message_container);
            } else {
                S1AP_MME_UE_S1AP_ID_t MME_UE_S1AP_ID;
                S1AP_ENB_UE_S1AP_ID_t ENB_UE_S1AP_ID;

                ogs_warn("No connection of MSC/VLR");
                MME_UE_S1AP_ID = enb_ue->amf_ue_ngap_id;
                ENB_UE_S1AP_ID = enb_ue->enb_ue_ngap_id;

                ngap_send_error_indication(enb_ue->enb, 
                        &MME_UE_S1AP_ID, &ENB_UE_S1AP_ID,
                        S1AP_Cause_PR_transport,
                        S1AP_CauseTransport_transport_resource_unavailable);
            }

            return;
        default:
            ogs_warn("Unknown message[%d]", message->5gmm.h.message_type);
            return;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3413:
            if (amf_ue->t3413.retry_count >=
                    amf_timer_cfg(MME_TIMER_T3413)->max_count) {
                /* Paging failed */
                ogs_warn("[EMM] Paging to IMSI[%s] failed. Stop paging",
                        amf_ue->imsi_bcd);
                CLEAR_MME_UE_TIMER(amf_ue->t3413);

                if (CS_CALL_SERVICE_INDICATOR(amf_ue) ||
                    SMS_SERVICE_INDICATOR(amf_ue)) {
                    sgsap_send_ue_unreachable(amf_ue,
                            SGSAP_SGS_CAUSE_UE_UNREACHABLE);
                }
    
                CLEAR_SERVICE_INDICATOR(amf_ue);
            } else {
                amf_ue->t3413.retry_count++;
                /*
                 * If t3413 is timeout, the saved pkbuf is used.
                 * We don't have to set CNDomain.
                 * So, we just set CNDomain to 0
                 */
                ngap_send_paging(amf_ue, 0);
            }
            break;
        case MME_TIMER_T3470:
            if (amf_ue->t3470.retry_count >=
                    amf_timer_cfg(MME_TIMER_T3470)->max_count) {
                ogs_warn("[EMM] Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                CLEAR_MME_UE_TIMER(amf_ue->t3470);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3470.retry_count++;
                nas_eps_send_identity_request(amf_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        return;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        return;
    }

    if (!MME_UE_HAVE_IMSI(amf_ue)) {
        CLEAR_MME_UE_TIMER(amf_ue->t3470);
        nas_eps_send_identity_request(amf_ue);
        return;
    }

    enb_ue = amf_ue->enb_ue;
    ogs_assert(enb_ue);

    switch (amf_ue->nas_eps.type) {
    case MME_EPS_TYPE_ATTACH_REQUEST:
        if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            rv = nas_eps_send_5gmm_to_5gsm(amf_ue,
                    &amf_ue->pdn_connectivity_request);
            if (rv != OGS_OK) {
                ogs_error("nas_eps_send_5gmm_to_5gsm() failed");
                nas_eps_send_attach_reject(amf_ue,
                    EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
            } else {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            }
        } else {
            if (SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
                amf_gtp_send_delete_all_sessions(amf_ue);
            } else {
                amf_s6a_send_air(amf_ue, NULL);
            }
            OGS_FSM_TRAN(s, &gmm_state_authentication);
        }
        break;
    case MME_EPS_TYPE_TAU_REQUEST:
        procedureCode = e->ngap_code;

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_eps_send_tau_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_authentication);
            break;
        }

        if (procedureCode == S1AP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Iniital UE Message");
            if (amf_ue->nas_eps.update.active_flag) {
                nas_eps_send_tau_accept(amf_ue,
                        S1AP_ProcedureCode_id_InitialContextSetup);
            } else {
                nas_eps_send_tau_accept(amf_ue,
                        S1AP_ProcedureCode_id_downlinkNASTransport);
                amf_send_release_access_bearer_or_ue_context_release(enb_ue);
            }
        } else if (procedureCode == S1AP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            nas_eps_send_tau_accept(amf_ue,
                    S1AP_ProcedureCode_id_downlinkNASTransport);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }
        break;
    case MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST:
        procedureCode = e->ngap_code;

        if (!MME_P_TMSI_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No P-TMSI : UE[%s]", amf_ue->imsi_bcd);
            nas_eps_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_eps_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
            nas_eps_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }

        if (procedureCode == S1AP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Initial UE Message");
            if (amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                ogs_debug("    MO-CSFB-INDICATION[%d]",
                        amf_ue->nas_eps.service.value);
                sgsap_send_mo_csfb_indication(amf_ue);
            } else if (amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                ogs_debug("    SERVICE_REQUEST[%d]",
                        amf_ue->nas_eps.service.value);
                sgsap_send_service_request(amf_ue, SGSAP_EMM_IDLE_MODE);
            } else {
                ogs_warn(" Unknown CSFB Service Type[%d]",
                        amf_ue->nas_eps.service.value);
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);

        } else if (procedureCode == S1AP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            if (amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE ||
                amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE) {
                ogs_debug("    MO-CSFB-INDICATION[%d]",
                        amf_ue->nas_eps.service.value);
                sgsap_send_mo_csfb_indication(amf_ue);
            } else if (amf_ue->nas_eps.service.value ==
                    OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE) {
                ogs_debug("    SERVICE_REQUEST[%d]",
                        amf_ue->nas_eps.service.value);
                sgsap_send_service_request(amf_ue, SGSAP_EMM_CONNECTED_MODE);
            } else {
                ogs_warn(" Unknown CSFB Service Type[%d]",
                        amf_ue->nas_eps.service.value);
                nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_ue_context_modification_request(amf_ue);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }

        break;
    default:
        ogs_fatal("Invalid NAS-EPS[%d]", amf_ue->nas_eps.type);
        break;
    }
#endif
}

#if 0
void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);
    
    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->5gmm.h.message_type) {
        case OGS_NAS_EPS_AUTHENTICATION_RESPONSE:
        {
            ogs_nas_eps_authentication_response_t *authentication_response =
                &message->5gmm.authentication_response;
            ogs_nas_authentication_response_parameter_t
                *authentication_response_parameter =
                    &authentication_response->
                        authentication_response_parameter;

            ogs_debug("[EMM] Authentication response");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(amf_ue->t3460);

            if (authentication_response_parameter->length == 0 ||
                memcmp(authentication_response_parameter->res,
                amf_ue->xres,
                authentication_response_parameter->length) != 0) {
                ogs_log_hexdump(OGS_LOG_WARN,
                        authentication_response_parameter->res,
                        authentication_response_parameter->length);
                ogs_log_hexdump(OGS_LOG_WARN,
                        amf_ue->xres, OGS_MAX_RES_LEN);
                nas_eps_send_authentication_reject(amf_ue);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_security_mode);
            }

            break;
        }
        case OGS_NAS_EPS_AUTHENTICATION_FAILURE:
        {
            ogs_nas_eps_authentication_failure_t *authentication_failure =
                &message->5gmm.authentication_failure;
            ogs_nas_authentication_failure_parameter_t
                *authentication_failure_parameter = 
                    &authentication_failure->
                        authentication_failure_parameter;

            ogs_debug("[EMM] Authentication failure");
            ogs_debug("    IMSI[%s] EMM_CAUSE[%d]", amf_ue->imsi_bcd,
                    authentication_failure->5gmm_cause);

            CLEAR_MME_UE_TIMER(amf_ue->t3460);

            switch (authentication_failure->5gmm_cause) {
            case EMM_CAUSE_MAC_FAILURE:
                ogs_warn("Authentication failure(MAC failure)");
                break;
            case EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE:
                ogs_error("Authentication failure"
                        "(Non-EPS authentication unacceptable)");
                break;
            case EMM_CAUSE_SYNCH_FAILURE:
                ogs_warn("Authentication failure(Synch failure)");
                amf_s6a_send_air(amf_ue,
                        authentication_failure_parameter);
                return;
            default:
                ogs_error("Unknown EMM_CAUSE{%d] in Authentication"
                        " failure",
                        authentication_failure->5gmm_cause);
                break;
            }

            nas_eps_send_authentication_reject(amf_ue);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

            break;
        }
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_attach_request(
                    amf_ue, &message->5gmm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->5gmm.5gmm_status.5gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_detach_request(
                    amf_ue, &message->5gmm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_detach(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->5gmm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (amf_ue->t3460.retry_count >=
                    amf_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("[EMM] Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        amf_ue->imsi_bcd);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

                nas_eps_send_authentication_reject(amf_ue);
            } else {
                amf_ue->t3460.retry_count++;
                nas_eps_send_authentication_request(amf_ue, NULL);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_security_mode(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_MME_UE_TIMER(amf_ue->t3460);
        nas_eps_send_security_mode_command(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        if (message->5gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("[EMM] Service request");
            nas_eps_send_service_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }

        switch (message->5gmm.h.message_type) {
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
            ogs_debug("[EMM] Security mode complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            CLEAR_MME_UE_TIMER(amf_ue->t3460);

            /* Now, We will check the MAC in the NAS message*/
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("Security-mode : No Integrity Protected in IMSI[%s]",
                        amf_ue->imsi_bcd);

                nas_eps_send_attach_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_eps_send_attach_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            rv = 5gmm_handle_security_mode_complete(
                    amf_ue, &message->5gmm.security_mode_complete);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_security_mode_complete() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_kdf_enb(amf_ue->kasme, amf_ue->ul_count.i32, 
                    amf_ue->kenb);
            amf_kdf_nh(amf_ue->kasme, amf_ue->kenb, amf_ue->nh);
            amf_ue->nhcc = 1;

            amf_s6a_send_ulr(amf_ue);
            if (amf_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            } else if (amf_ue->nas_eps.type ==
                    MME_EPS_TYPE_SERVICE_REQUEST ||
                    amf_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
            } else {
                ogs_fatal("Invalid OGS_NAS_EPS[%d]", amf_ue->nas_eps.type);
            }
            break;
        case OGS_NAS_EPS_SECURITY_MODE_REJECT:
            ogs_warn("[EMM] Security mode reject : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->5gmm.security_mode_reject.5gmm_cause);
            CLEAR_MME_UE_TIMER(amf_ue->t3460);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_attach_request(
                    amf_ue, &message->5gmm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("[EMM] Tracking area update request");
            nas_eps_send_tau_reject(amf_ue,
                EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->5gmm.5gmm_status.5gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_detach_request(
                    amf_ue, &message->5gmm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_send_delete_session_or_detach(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->5gmm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        case MME_TIMER_T3460:
            if (amf_ue->t3460.retry_count >=
                    amf_timer_cfg(MME_TIMER_T3460)->max_count) {
                ogs_warn("[EMM] Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        amf_ue->imsi_bcd);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

                nas_eps_send_attach_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            } else {
                amf_ue->t3460.retry_count++;
                nas_eps_send_security_mode_command(amf_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_initial_context_setup(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_eps_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_EMM_MESSAGE:
        message = e->nas_message;
        ogs_assert(message);

        switch (message->5gmm.h.message_type) {
        case OGS_NAS_EPS_ATTACH_COMPLETE:
            ogs_debug("[EMM] Attach complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            rv = 5gmm_handle_attach_complete(
                    amf_ue, &message->5gmm.attach_complete);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_complete() failed "
                        "in gmm_state_initial_context_setup");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }
            if (MME_P_TMSI_IS_AVAILABLE(amf_ue))
                sgsap_send_tmsi_reallocation_complete(amf_ue);

            OGS_FSM_TRAN(s, &gmm_state_registered);
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST:
            ogs_warn("[EMM] Attach request[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_attach_request(
                    amf_ue, &message->5gmm.attach_request);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_gtp_send_delete_all_sessions(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_EPS_EMM_STATUS:
            ogs_warn("[EMM] EMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->5gmm.5gmm_status.5gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_EPS_DETACH_REQUEST:
            ogs_debug("[EMM] Detach request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = 5gmm_handle_detach_request(
                    amf_ue, &message->5gmm.detach_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("5gmm_handle_attach_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_detach(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", 
                    message->5gmm.h.message_type);
            break;
        }
        break;
    case MME_EVT_EMM_TIMER:
        switch (e->timer_id) {
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

void gmm_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(e);
    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(amf_ue);
        CLEAR_MME_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}
#endif
