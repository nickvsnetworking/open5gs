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

#include "ngap-handler.h"
#include "gmm-handler.h"
#include "gmm-build.h"
#if 0
#include "gsm-handler.h"
#endif
#include "nas-path.h"
#include "nas-security.h"
#include "kdf.h"
#include "ngap-path.h"
#if 0
#include "sgsap-types.h"
#include "sgsap-path.h"
#include "amf-path.h"
#endif
#include "amf-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

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
    amf_ue_t *amf_ue = NULL;
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_SERVICE_INDICATOR(amf_ue);
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        break;
    }

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
    int rv;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    ogs_nas_5gs_message_t *message = NULL;
#if 0
    NGAP_ProcedureCode_t procedureCode;
#endif
    
    ogs_assert(e);
        
    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return;
    case OGS_FSM_EXIT_SIG:
        return;
    case AMF_EVT_5GMM_MESSAGE:
        message = e->nas.message;
        ogs_assert(message);

        ran_ue = amf_ue->ran_ue;
        ogs_assert(ran_ue);

#if 0
        if (message->gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            rv = gmm_handle_service_request(
                    amf_ue, &message->gmm.service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("Service request : Unknown UE");
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
                ogs_warn("No Session Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);
            return;
        }
#endif

        switch (message->gmm.h.message_type) {
#if 0
        case OGS_NAS_5GS_IDENTITY_RESPONSE:
            ogs_debug("Identity response");
            CLEAR_AMF_UE_TIMER(amf_ue->t3470);

            rv = gmm_handle_identity_response(amf_ue,
                    &message->gmm.identity_response);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_identity_response() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_error("No IMSI");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            break;
#endif
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_debug("Registration request[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_registration_request(
                    amf_ue, &message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }
            break;
#if 0
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            rv = gmm_handle_tau_request(
                    amf_ue, &message->gmm.tracking_area_update_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_tau_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("TAU request : Unknown UE");
                nas_5gs_send_tau_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_COMPLETE:
            ogs_debug("Tracking area update complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            return;
        case OGS_NAS_5GS_EXTENDED_SERVICE_REQUEST:
            ogs_debug("Extended service request");
            rv = gmm_handle_extended_service_request(
                    amf_ue, &message->gmm.extended_service_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_extended_service_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (!AMF_UE_HAVE_IMSI(amf_ue)) {
                ogs_warn("Extended Service request : Unknown UE");
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            break;
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("5GMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK)
            {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            if (AMF_P_TMSI_IS_AVAILABLE(amf_ue)) {
                sgsap_send_deregistration_indication(amf_ue);
            } else {
                amf_send_delete_session_or_deregistration(amf_ue);
            }

            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            return;
        case OGS_NAS_5GS_UPLINK_NAS_TRANSPORT:
            ogs_debug("Uplink NAS Transport");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            if (AMF_SGSAP_IS_CONNECTED(amf_ue)) {
                sgsap_send_uplink_unitdata(amf_ue, &message->gmm.
                        uplink_nas_transport.nas_message_container);
            } else {
                NGAP_AMF_UE_NGAP_ID_t AMF_UE_NGAP_ID;
                NGAP_ENB_UE_NGAP_ID_t ENB_UE_NGAP_ID;

                ogs_warn("No connection of MSC/VLR");
                AMF_UE_NGAP_ID = ran_ue->amf_ue_ngap_id;
                ENB_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

                ngap_send_error_indication(ran_ue->gnb, 
                        &AMF_UE_NGAP_ID, &ENB_UE_NGAP_ID,
                        NGAP_Cause_PR_transport,
                        NGAP_CauseTransport_transport_resource_unavailable);
            }

            return;
#endif
        default:
            ogs_warn("Unknown message[%d]", message->gmm.h.message_type);
            return;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
#if 0
        switch (e->timer_id) {
        case AMF_TIMER_T3413:
            if (amf_ue->t3413.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3413)->max_count) {
                /* Paging failed */
                ogs_warn("Paging to IMSI[%s] failed. Stop paging",
                        amf_ue->imsi_bcd);
                CLEAR_AMF_UE_TIMER(amf_ue->t3413);

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
        case AMF_TIMER_T3470:
            if (amf_ue->t3470.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3470)->max_count) {
                ogs_warn("Retransmission of Identity-Request failed. "
                        "Stop retransmission");
                CLEAR_AMF_UE_TIMER(amf_ue->t3470);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                amf_ue->t3470.retry_count++;
                nas_5gs_send_identity_request(amf_ue);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
#endif
        return;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        return;
    }

#if 0
    if (!AMF_UE_HAVE_IMSI(amf_ue)) {
        CLEAR_AMF_UE_TIMER(amf_ue->t3470);
        nas_5gs_send_identity_request(amf_ue);
        return;
    }
#endif

    ran_ue = amf_ue->ran_ue;
    ogs_assert(ran_ue);

    switch (amf_ue->nas.type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
#if 0
        if (SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            amf_gtp_send_delete_all_sessions(amf_ue);
        } else {
            amf_s6a_send_air(amf_ue, NULL);
        }
#else
        ogs_fatal("send auth");
#endif
        OGS_FSM_TRAN(s, &gmm_state_authentication);
        break;
#if 0
    case AMF_EPS_TYPE_TAU_REQUEST:
        procedureCode = e->ngap_code;

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_tau_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_authentication);
            break;
        }

        if (procedureCode == NGAP_ProcedureCode_id_initialUEMessage) {
            ogs_debug("    Iniital UE Message");
            if (amf_ue->nas_eps.update.active_flag) {
                nas_5gs_send_tau_accept(amf_ue,
                        NGAP_ProcedureCode_id_InitialContextSetup);
            } else {
                nas_5gs_send_tau_accept(amf_ue,
                        NGAP_ProcedureCode_id_downlinkNASTransport);
                amf_send_release_access_bearer_or_ue_context_release(ran_ue);
            }
        } else if (procedureCode == NGAP_ProcedureCode_id_uplinkNASTransport) {
            ogs_debug("    Uplink NAS Transport");
            nas_5gs_send_tau_accept(amf_ue,
                    NGAP_ProcedureCode_id_downlinkNASTransport);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }
        break;
    case AMF_EPS_TYPE_EXTENDED_SERVICE_REQUEST:
        procedureCode = e->ngap_code;

        if (!AMF_P_TMSI_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No P-TMSI : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SESSION_CONTEXT_IS_AVAILABLE(amf_ue)) {
            ogs_warn("No PDN Connection : UE[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, gmm_state_exception);
            break;
        }

        if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
            ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
            nas_5gs_send_service_reject(amf_ue,
                EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }

        if (procedureCode == NGAP_ProcedureCode_id_initialUEMessage) {
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
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_initial_context_setup_request(amf_ue);

        } else if (procedureCode == NGAP_ProcedureCode_id_uplinkNASTransport) {
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
                nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                return;
            }

            ngap_send_ue_context_modification_request(amf_ue);
        } else {
            ogs_fatal("Invalid Procedure Code[%d]", (int)procedureCode);
        }

        break;
#endif
    default:
        ogs_fatal("Invalid NAS-5GS[%d]", amf_ue->nas.type);
        break;
    }
}

void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_message_t *message = NULL;

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
    case AMF_EVT_5GMM_MESSAGE:
        message = e->nas.message;
        ogs_assert(message);

        switch (message->gmm.h.message_type) {
        case OGS_NAS_5GS_AUTHENTICATION_RESPONSE:
        {
#if 0
            ogs_nas_eps_authentication_response_t *authentication_response =
                &message->gmm.authentication_response;
            ogs_nas_authentication_response_parameter_t
                *authentication_response_parameter =
                    &authentication_response->
                        authentication_response_parameter;

            ogs_debug("Authentication response");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            CLEAR_AMF_UE_TIMER(amf_ue->t3460);

            if (authentication_response_parameter->length == 0 ||
                memcmp(authentication_response_parameter->res,
                amf_ue->xres,
                authentication_response_parameter->length) != 0) {
                ogs_log_hexdump(OGS_LOG_WARN,
                        authentication_response_parameter->res,
                        authentication_response_parameter->length);
                ogs_log_hexdump(OGS_LOG_WARN,
                        amf_ue->xres, OGS_MAX_RES_LEN);
                nas_5gs_send_authentication_reject(amf_ue);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
            } else {
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_security_mode);
            }
#endif

            break;
        }
        case OGS_NAS_5GS_AUTHENTICATION_FAILURE:
        {
#if 0
            ogs_nas_eps_authentication_failure_t *authentication_failure =
                &message->gmm.authentication_failure;
            ogs_nas_authentication_failure_parameter_t
                *authentication_failure_parameter = 
                    &authentication_failure->
                        authentication_failure_parameter;

            ogs_debug("Authentication failure");
            ogs_debug("    IMSI[%s] EMM_CAUSE[%d]", amf_ue->imsi_bcd,
                    authentication_failure->gmm_cause);

            CLEAR_AMF_UE_TIMER(amf_ue->t3460);

            switch (authentication_failure->gmm_cause) {
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
                        authentication_failure->gmm_cause);
                break;
            }

            nas_5gs_send_authentication_reject(amf_ue);
            OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);
#endif

            break;
        }
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_registration_request(
                    amf_ue, &message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

#if 0
            amf_s6a_send_air(amf_ue, NULL);
#endif
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("5GMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
#if 0
            rv = gmm_handle_deregistration_request(
                    amf_ue, &message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
#endif
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
#if 0
        switch (e->timer_id) {
        case AMF_TIMER_T3460:
            if (amf_ue->t3460.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        amf_ue->imsi_bcd);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

                nas_5gs_send_authentication_reject(amf_ue);
            } else {
                amf_ue->t3460.retry_count++;
                nas_5gs_send_authentication_request(amf_ue, NULL);
            }
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
#endif
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}

#if 0
void gmm_state_security_mode(ogs_fsm_t *s, amf_event_t *e)
{
    int rv;
    amf_ue_t *amf_ue = NULL;
    ogs_nas_5gs_message_t *message = NULL;
    ogs_nas_security_header_type_t h;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    amf_ue = e->amf_ue;
    ogs_assert(amf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        CLEAR_AMF_UE_TIMER(amf_ue->t3460);
        nas_5gs_send_security_mode_command(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVT_5GMM_MESSAGE:
        message = e->nas.message;
        ogs_assert(message);

        if (message->gmm.h.security_header_type
                == OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
            ogs_debug("Service request");
            nas_5gs_send_service_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            return;
        }

        switch (message->gmm.h.message_type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
            ogs_debug("Security mode complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            CLEAR_AMF_UE_TIMER(amf_ue->t3460);

            /* Now, We will check the MAC in the NAS message*/
            h.type = e->nas_type;
            if (h.integrity_protected == 0) {
                ogs_error("Security-mode : No Integrity Protected in IMSI[%s]",
                        amf_ue->imsi_bcd);

                nas_5gs_send_registration_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
                ogs_warn("No Security Context : IMSI[%s]", amf_ue->imsi_bcd);
                nas_5gs_send_registration_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                OGS_FSM_TRAN(s, &gmm_state_exception);
                break;
            }

            rv = gmm_handle_security_mode_complete(
                    amf_ue, &message->gmm.security_mode_complete);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_security_mode_complete() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_kdf_gnb(amf_ue->kasme, amf_ue->ul_count.i32, 
                    amf_ue->kgnb);
            amf_kdf_nh(amf_ue->kasme, amf_ue->kgnb, amf_ue->nh);
            amf_ue->nhcc = 1;

            amf_s6a_send_ulr(amf_ue);
            if (amf_ue->nas_eps.type == AMF_EPS_TYPE_REGISTRATION_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_initial_context_setup);
            } else if (amf_ue->nas_eps.type ==
                    AMF_EPS_TYPE_SERVICE_REQUEST ||
                    amf_ue->nas_eps.type == AMF_EPS_TYPE_TAU_REQUEST) {
                OGS_FSM_TRAN(s, &gmm_state_registered);
            } else {
                ogs_fatal("Invalid OGS_NAS_5GS[%d]", amf_ue->nas_eps.type);
            }
            break;
        case OGS_NAS_5GS_SECURITY_MODE_REJECT:
            ogs_warn("Security mode reject : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->gmm.security_mode_reject.gmm_cause);
            CLEAR_AMF_UE_TIMER(amf_ue->t3460);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_registration_request(
                    amf_ue, &message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_s6a_send_air(amf_ue, NULL);
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_5GS_TRACKING_AREA_UPDATE_REQUEST:
            ogs_debug("Tracking area update request");
            nas_5gs_send_tau_reject(amf_ue,
                EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("5GMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
        switch (e->timer_id) {
        case AMF_TIMER_T3460:
            if (amf_ue->t3460.retry_count >=
                    amf_timer_cfg(AMF_TIMER_T3460)->max_count) {
                ogs_warn("Retransmission of IMSI[%s] failed. "
                        "Stop retransmission",
                        amf_ue->imsi_bcd);
                OGS_FSM_TRAN(&amf_ue->sm, &gmm_state_exception);

                nas_5gs_send_registration_reject(amf_ue,
                    EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED,
                    ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
            } else {
                amf_ue->t3460.retry_count++;
                nas_5gs_send_security_mode_command(amf_ue);
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
    ogs_nas_5gs_message_t *message = NULL;

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
    case AMF_EVT_5GMM_MESSAGE:
        message = e->nas.message;
        ogs_assert(message);

        switch (message->gmm.h.message_type) {
        case OGS_NAS_5GS_REGISTRATION_COMPLETE:
            ogs_debug("Registration complete");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);

            rv = gmm_handle_registration_complete(
                    amf_ue, &message->gmm.registration_complete);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_complete() failed "
                        "in gmm_state_initial_context_setup");
                OGS_FSM_TRAN(s, gmm_state_exception);
                break;
            }
            if (AMF_P_TMSI_IS_AVAILABLE(amf_ue))
                sgsap_send_tmsi_reallocation_complete(amf_ue);

            OGS_FSM_TRAN(s, &gmm_state_registered);
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST:
            ogs_warn("Registration request[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_registration_request(
                    amf_ue, &message->gmm.registration_request);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_gtp_send_delete_all_sessions(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_authentication);
            break;
        case OGS_NAS_5GS_5GMM_STATUS:
            ogs_warn("5GMM STATUS : IMSI[%s] Cause[%d]",
                    amf_ue->imsi_bcd,
                    message->gmm.gmm_status.gmm_cause);
            OGS_FSM_TRAN(s, &gmm_state_exception);
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
            ogs_debug("Deregistration request");
            ogs_debug("    IMSI[%s]", amf_ue->imsi_bcd);
            rv = gmm_handle_deregistration_request(
                    amf_ue, &message->gmm.deregistration_request_from_ue);
            if (rv != OGS_OK) {
                ogs_error("gmm_handle_registration_request() failed");
                OGS_FSM_TRAN(s, gmm_state_exception);
                return;
            }

            amf_send_delete_session_or_deregistration(amf_ue);
            OGS_FSM_TRAN(s, &gmm_state_de_registered);
            break;
        default:
            ogs_warn("Unknown message[%d]", 
                    message->gmm.h.message_type);
            break;
        }
        break;
    case AMF_EVT_5GMM_TIMER:
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
#endif

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
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event[%s]", amf_event_get_name(e));
        break;
    }
}
