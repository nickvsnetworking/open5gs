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

#include "test-ngap.h"

int testngap_build_setup_req(
        ogs_pkbuf_t **pkbuf, uint32_t gnb_id,
        int tac, uint16_t mcc, uint16_t mnc, uint16_t mnc_len)
{
    ogs_plmn_id_t plmn_id;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    struct NGAP_GlobalGNB_ID *globalGNB_ID = NULL;
    NGAP_RANNodeName_t *RANNodeName = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_SupportedTAItem_t *SupportedTAItem = NULL;
    NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
    NGAP_PagingDRX_t *PagingDRX = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_NGSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_NGSetupRequest;

    NGSetupRequest = &initiatingMessage->value.choice.NGSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GlobalRANNodeID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

    GlobalRANNodeID = &ie->value.choice.GlobalRANNodeID;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SupportedTAList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_SupportedTAList;

    SupportedTAList = &ie->value.choice.SupportedTAList;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);
    
    ie->id = NGAP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    ogs_plmn_id_build(&plmn_id, mcc, mnc, mnc_len);

    globalGNB_ID = CALLOC(1, sizeof(*globalGNB_ID));
    GlobalRANNodeID->choice.globalGNB_ID = globalGNB_ID;

    ogs_ngap_uint32_to_GNB_ID(gnb_id, &globalGNB_ID->gNB_ID);
    ogs_asn_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, &globalGNB_ID->pLMNIdentity);

    SupportedTAItem = (NGAP_SupportedTAItem_t *)
        CALLOC(1, sizeof(NGAP_SupportedTAItem_t));
    ogs_asn_uint16_to_OCTET_STRING(tac, &SupportedTAItem->tAC);
    pLMNIdentity = (NGAP_PLMNIdentity_t *)
        CALLOC(1, sizeof(NGAP_PLMNIdentity_t));
    ogs_asn_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, pLMNIdentity);
    ASN_SEQUENCE_ADD(&SupportedTAItem->broadcastPLMNList.list, pLMNIdentity);

    ASN_SEQUENCE_ADD(&SupportedTAList->list, SupportedTAItem);

    *PagingDRX = NGAP_PagingDRX_v64;

    *pkbuf = ogs_ngap_encode(&pdu);
    if (*pkbuf == NULL) {
        ogs_error("ogs_ngap_encode() failed");
        return OGS_ERROR;
    }
    return OGS_OK;
}

