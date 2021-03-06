# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

libasn1c_ngap_sources = files('''
	NGAP_NGAP-PDU.c
	NGAP_InitiatingMessage.c
	NGAP_SuccessfulOutcome.c
	NGAP_UnsuccessfulOutcome.c
	NGAP_PDUSessionResourceSetupRequest.c
	NGAP_PDUSessionResourceSetupResponse.c
	NGAP_PDUSessionResourceReleaseCommand.c
	NGAP_PDUSessionResourceReleaseResponse.c
	NGAP_PDUSessionResourceModifyRequest.c
	NGAP_PDUSessionResourceModifyResponse.c
	NGAP_PDUSessionResourceNotify.c
	NGAP_PDUSessionResourceModifyIndication.c
	NGAP_PDUSessionResourceModifyConfirm.c
	NGAP_InitialContextSetupRequest.c
	NGAP_InitialContextSetupResponse.c
	NGAP_InitialContextSetupFailure.c
	NGAP_UEContextReleaseRequest.c
	NGAP_UEContextReleaseCommand.c
	NGAP_UEContextReleaseComplete.c
	NGAP_UEContextModificationRequest.c
	NGAP_UEContextModificationResponse.c
	NGAP_UEContextModificationFailure.c
	NGAP_RRCInactiveTransitionReport.c
	NGAP_HandoverRequired.c
	NGAP_HandoverCommand.c
	NGAP_HandoverPreparationFailure.c
	NGAP_HandoverRequest.c
	NGAP_HandoverRequestAcknowledge.c
	NGAP_HandoverFailure.c
	NGAP_HandoverNotify.c
	NGAP_PathSwitchRequest.c
	NGAP_PathSwitchRequestAcknowledge.c
	NGAP_PathSwitchRequestFailure.c
	NGAP_HandoverCancel.c
	NGAP_HandoverCancelAcknowledge.c
	NGAP_UplinkRANStatusTransfer.c
	NGAP_DownlinkRANStatusTransfer.c
	NGAP_Paging.c
	NGAP_InitialUEMessage.c
	NGAP_DownlinkNASTransport.c
	NGAP_UplinkNASTransport.c
	NGAP_NASNonDeliveryIndication.c
	NGAP_RerouteNASRequest.c
	NGAP_NGSetupRequest.c
	NGAP_NGSetupResponse.c
	NGAP_NGSetupFailure.c
	NGAP_RANConfigurationUpdate.c
	NGAP_RANConfigurationUpdateAcknowledge.c
	NGAP_RANConfigurationUpdateFailure.c
	NGAP_AMFConfigurationUpdate.c
	NGAP_AMFConfigurationUpdateAcknowledge.c
	NGAP_AMFConfigurationUpdateFailure.c
	NGAP_AMFStatusIndication.c
	NGAP_NGReset.c
	NGAP_NGResetAcknowledge.c
	NGAP_ErrorIndication.c
	NGAP_OverloadStart.c
	NGAP_OverloadStop.c
	NGAP_UplinkRANConfigurationTransfer.c
	NGAP_DownlinkRANConfigurationTransfer.c
	NGAP_WriteReplaceWarningRequest.c
	NGAP_WriteReplaceWarningResponse.c
	NGAP_PWSCancelRequest.c
	NGAP_PWSCancelResponse.c
	NGAP_PWSRestartIndication.c
	NGAP_PWSFailureIndication.c
	NGAP_DownlinkUEAssociatedNRPPaTransport.c
	NGAP_UplinkUEAssociatedNRPPaTransport.c
	NGAP_DownlinkNonUEAssociatedNRPPaTransport.c
	NGAP_UplinkNonUEAssociatedNRPPaTransport.c
	NGAP_TraceStart.c
	NGAP_TraceFailureIndication.c
	NGAP_DeactivateTrace.c
	NGAP_CellTrafficTrace.c
	NGAP_LocationReportingControl.c
	NGAP_LocationReportingFailureIndication.c
	NGAP_LocationReport.c
	NGAP_UETNLABindingReleaseRequest.c
	NGAP_UERadioCapabilityInfoIndication.c
	NGAP_UERadioCapabilityCheckRequest.c
	NGAP_UERadioCapabilityCheckResponse.c
	NGAP_PrivateMessage.c
	NGAP_SecondaryRATDataUsageReport.c
	NGAP_UplinkRIMInformationTransfer.c
	NGAP_DownlinkRIMInformationTransfer.c
	NGAP_AdditionalDLUPTNLInformationForHOList.c
	NGAP_AdditionalDLUPTNLInformationForHOItem.c
	NGAP_AdditionalQosFlowInformation.c
	NGAP_AllocationAndRetentionPriority.c
	NGAP_AllowedNSSAI.c
	NGAP_AllowedNSSAI-Item.c
	NGAP_AllowedTACs.c
	NGAP_AMFName.c
	NGAP_AMFPagingTarget.c
	NGAP_AMFPointer.c
	NGAP_AMFRegionID.c
	NGAP_AMFSetID.c
	NGAP_AMF-TNLAssociationSetupList.c
	NGAP_AMF-TNLAssociationSetupItem.c
	NGAP_AMF-TNLAssociationToAddList.c
	NGAP_AMF-TNLAssociationToAddItem.c
	NGAP_AMF-TNLAssociationToRemoveList.c
	NGAP_AMF-TNLAssociationToRemoveItem.c
	NGAP_AMF-TNLAssociationToUpdateList.c
	NGAP_AMF-TNLAssociationToUpdateItem.c
	NGAP_AMF-UE-NGAP-ID.c
	NGAP_AreaOfInterest.c
	NGAP_AreaOfInterestCellList.c
	NGAP_AreaOfInterestCellItem.c
	NGAP_AreaOfInterestList.c
	NGAP_AreaOfInterestItem.c
	NGAP_AreaOfInterestRANNodeList.c
	NGAP_AreaOfInterestRANNodeItem.c
	NGAP_AreaOfInterestTAIList.c
	NGAP_AreaOfInterestTAIItem.c
	NGAP_AssistanceDataForPaging.c
	NGAP_AssistanceDataForRecommendedCells.c
	NGAP_AssociatedQosFlowList.c
	NGAP_AssociatedQosFlowItem.c
	NGAP_AveragingWindow.c
	NGAP_BitRate.c
	NGAP_BroadcastCancelledAreaList.c
	NGAP_BroadcastCompletedAreaList.c
	NGAP_BroadcastPLMNList.c
	NGAP_BroadcastPLMNItem.c
	NGAP_CancelAllWarningMessages.c
	NGAP_CancelledCellsInEAI-EUTRA.c
	NGAP_CancelledCellsInEAI-EUTRA-Item.c
	NGAP_CancelledCellsInEAI-NR.c
	NGAP_CancelledCellsInEAI-NR-Item.c
	NGAP_CancelledCellsInTAI-EUTRA.c
	NGAP_CancelledCellsInTAI-EUTRA-Item.c
	NGAP_CancelledCellsInTAI-NR.c
	NGAP_CancelledCellsInTAI-NR-Item.c
	NGAP_Cause.c
	NGAP_CauseMisc.c
	NGAP_CauseNas.c
	NGAP_CauseProtocol.c
	NGAP_CauseRadioNetwork.c
	NGAP_CauseTransport.c
	NGAP_CellIDBroadcastEUTRA.c
	NGAP_CellIDBroadcastEUTRA-Item.c
	NGAP_CellIDBroadcastNR.c
	NGAP_CellIDBroadcastNR-Item.c
	NGAP_CellIDCancelledEUTRA.c
	NGAP_CellIDCancelledEUTRA-Item.c
	NGAP_CellIDCancelledNR.c
	NGAP_CellIDCancelledNR-Item.c
	NGAP_CellIDListForRestart.c
	NGAP_CellSize.c
	NGAP_CellType.c
	NGAP_CNAssistedRANTuning.c
	NGAP_CNTypeRestrictionsForEquivalent.c
	NGAP_CNTypeRestrictionsForEquivalentItem.c
	NGAP_CNTypeRestrictionsForServing.c
	NGAP_CommonNetworkInstance.c
	NGAP_CompletedCellsInEAI-EUTRA.c
	NGAP_CompletedCellsInEAI-EUTRA-Item.c
	NGAP_CompletedCellsInEAI-NR.c
	NGAP_CompletedCellsInEAI-NR-Item.c
	NGAP_CompletedCellsInTAI-EUTRA.c
	NGAP_CompletedCellsInTAI-EUTRA-Item.c
	NGAP_CompletedCellsInTAI-NR.c
	NGAP_CompletedCellsInTAI-NR-Item.c
	NGAP_ConcurrentWarningMessageInd.c
	NGAP_ConfidentialityProtectionIndication.c
	NGAP_ConfidentialityProtectionResult.c
	NGAP_CoreNetworkAssistanceInformationForInactive.c
	NGAP_COUNTValueForPDCP-SN12.c
	NGAP_COUNTValueForPDCP-SN18.c
	NGAP_CPTransportLayerInformation.c
	NGAP_CriticalityDiagnostics.c
	NGAP_CriticalityDiagnostics-IE-List.c
	NGAP_CriticalityDiagnostics-IE-Item.c
	NGAP_DataCodingScheme.c
	NGAP_DataForwardingAccepted.c
	NGAP_DataForwardingNotPossible.c
	NGAP_DataForwardingResponseDRBList.c
	NGAP_DataForwardingResponseDRBItem.c
	NGAP_DataForwardingResponseERABList.c
	NGAP_DataForwardingResponseERABListItem.c
	NGAP_DelayCritical.c
	NGAP_DLForwarding.c
	NGAP_DL-NGU-TNLInformationReused.c
	NGAP_DirectForwardingPathAvailability.c
	NGAP_DRB-ID.c
	NGAP_DRBsSubjectToStatusTransferList.c
	NGAP_DRBsSubjectToStatusTransferItem.c
	NGAP_DRBStatusDL.c
	NGAP_DRBStatusDL12.c
	NGAP_DRBStatusDL18.c
	NGAP_DRBStatusUL.c
	NGAP_DRBStatusUL12.c
	NGAP_DRBStatusUL18.c
	NGAP_DRBsToQosFlowsMappingList.c
	NGAP_DRBsToQosFlowsMappingItem.c
	NGAP_Dynamic5QIDescriptor.c
	NGAP_EmergencyAreaID.c
	NGAP_EmergencyAreaIDBroadcastEUTRA.c
	NGAP_EmergencyAreaIDBroadcastEUTRA-Item.c
	NGAP_EmergencyAreaIDBroadcastNR.c
	NGAP_EmergencyAreaIDBroadcastNR-Item.c
	NGAP_EmergencyAreaIDCancelledEUTRA.c
	NGAP_EmergencyAreaIDCancelledEUTRA-Item.c
	NGAP_EmergencyAreaIDCancelledNR.c
	NGAP_EmergencyAreaIDCancelledNR-Item.c
	NGAP_EmergencyAreaIDList.c
	NGAP_EmergencyAreaIDListForRestart.c
	NGAP_EmergencyFallbackIndicator.c
	NGAP_EmergencyFallbackRequestIndicator.c
	NGAP_EmergencyServiceTargetCN.c
	NGAP_EN-DCSONConfigurationTransfer.c
	NGAP_EndpointIPAddressAndPort.c
	NGAP_EquivalentPLMNs.c
	NGAP_EPS-TAC.c
	NGAP_EPS-TAI.c
	NGAP_E-RAB-ID.c
	NGAP_E-RABInformationList.c
	NGAP_E-RABInformationItem.c
	NGAP_EUTRACellIdentity.c
	NGAP_EUTRA-CGI.c
	NGAP_EUTRA-CGIList.c
	NGAP_EUTRA-CGIListForWarning.c
	NGAP_EUTRAencryptionAlgorithms.c
	NGAP_EUTRAintegrityProtectionAlgorithms.c
	NGAP_EventType.c
	NGAP_ExpectedActivityPeriod.c
	NGAP_ExpectedHOInterval.c
	NGAP_ExpectedIdlePeriod.c
	NGAP_ExpectedUEActivityBehaviour.c
	NGAP_ExpectedUEBehaviour.c
	NGAP_ExpectedUEMobility.c
	NGAP_ExpectedUEMovingTrajectory.c
	NGAP_ExpectedUEMovingTrajectoryItem.c
	NGAP_ExtendedRATRestrictionInformation.c
	NGAP_ExtendedRNC-ID.c
	NGAP_FiveG-S-TMSI.c
	NGAP_FiveG-TMSI.c
	NGAP_FiveQI.c
	NGAP_ForbiddenAreaInformation.c
	NGAP_ForbiddenAreaInformation-Item.c
	NGAP_ForbiddenTACs.c
	NGAP_GBR-QosInformation.c
	NGAP_GlobalGNB-ID.c
	NGAP_GlobalN3IWF-ID.c
	NGAP_GlobalNgENB-ID.c
	NGAP_GlobalRANNodeID.c
	NGAP_GNB-ID.c
	NGAP_GTP-TEID.c
	NGAP_GTPTunnel.c
	NGAP_GUAMI.c
	NGAP_GUAMIType.c
	NGAP_HandoverCommandTransfer.c
	NGAP_HandoverFlag.c
	NGAP_HandoverPreparationUnsuccessfulTransfer.c
	NGAP_HandoverRequestAcknowledgeTransfer.c
	NGAP_HandoverRequiredTransfer.c
	NGAP_HandoverResourceAllocationUnsuccessfulTransfer.c
	NGAP_HandoverType.c
	NGAP_IMSVoiceSupportIndicator.c
	NGAP_IndexToRFSP.c
	NGAP_InfoOnRecommendedCellsAndRANNodesForPaging.c
	NGAP_IntegrityProtectionIndication.c
	NGAP_IntegrityProtectionResult.c
	NGAP_IntendedNumberOfPagingAttempts.c
	NGAP_InterfacesToTrace.c
	NGAP_LAC.c
	NGAP_LAI.c
	NGAP_LastVisitedCellInformation.c
	NGAP_LastVisitedCellItem.c
	NGAP_LastVisitedEUTRANCellInformation.c
	NGAP_LastVisitedGERANCellInformation.c
	NGAP_LastVisitedNGRANCellInformation.c
	NGAP_LastVisitedUTRANCellInformation.c
	NGAP_LocationReportingAdditionalInfo.c
	NGAP_LocationReportingReferenceID.c
	NGAP_LocationReportingRequestType.c
	NGAP_MaskedIMEISV.c
	NGAP_MaximumDataBurstVolume.c
	NGAP_MessageIdentifier.c
	NGAP_MaximumIntegrityProtectedDataRate.c
	NGAP_MICOModeIndication.c
	NGAP_MobilityRestrictionList.c
	NGAP_N3IWF-ID.c
	NGAP_NAS-PDU.c
	NGAP_NASSecurityParametersFromNGRAN.c
	NGAP_NetworkInstance.c
	NGAP_NewSecurityContextInd.c
	NGAP_NextHopChainingCount.c
	NGAP_NextPagingAreaScope.c
	NGAP_NgENB-ID.c
	NGAP_NGRAN-CGI.c
	NGAP_NGRAN-TNLAssociationToRemoveList.c
	NGAP_NGRAN-TNLAssociationToRemoveItem.c
	NGAP_NGRANTraceID.c
	NGAP_NonDynamic5QIDescriptor.c
	NGAP_NotAllowedTACs.c
	NGAP_NotificationCause.c
	NGAP_NotificationControl.c
	NGAP_NRCellIdentity.c
	NGAP_NR-CGI.c
	NGAP_NR-CGIList.c
	NGAP_NR-CGIListForWarning.c
	NGAP_NRencryptionAlgorithms.c
	NGAP_NRintegrityProtectionAlgorithms.c
	NGAP_NRPPa-PDU.c
	NGAP_NumberOfBroadcasts.c
	NGAP_NumberOfBroadcastsRequested.c
	NGAP_OverloadAction.c
	NGAP_OverloadResponse.c
	NGAP_OverloadStartNSSAIList.c
	NGAP_OverloadStartNSSAIItem.c
	NGAP_PacketDelayBudget.c
	NGAP_PacketErrorRate.c
	NGAP_PacketLossRate.c
	NGAP_PagingAttemptInformation.c
	NGAP_PagingAttemptCount.c
	NGAP_PagingDRX.c
	NGAP_PagingOrigin.c
	NGAP_PagingPriority.c
	NGAP_PathSwitchRequestAcknowledgeTransfer.c
	NGAP_PathSwitchRequestSetupFailedTransfer.c
	NGAP_PathSwitchRequestTransfer.c
	NGAP_PathSwitchRequestUnsuccessfulTransfer.c
	NGAP_PDUSessionAggregateMaximumBitRate.c
	NGAP_PDUSessionID.c
	NGAP_PDUSessionResourceAdmittedList.c
	NGAP_PDUSessionResourceAdmittedItem.c
	NGAP_PDUSessionResourceFailedToModifyListModCfm.c
	NGAP_PDUSessionResourceFailedToModifyItemModCfm.c
	NGAP_PDUSessionResourceFailedToModifyListModRes.c
	NGAP_PDUSessionResourceFailedToModifyItemModRes.c
	NGAP_PDUSessionResourceFailedToSetupListCxtFail.c
	NGAP_PDUSessionResourceFailedToSetupItemCxtFail.c
	NGAP_PDUSessionResourceFailedToSetupListCxtRes.c
	NGAP_PDUSessionResourceFailedToSetupItemCxtRes.c
	NGAP_PDUSessionResourceFailedToSetupListHOAck.c
	NGAP_PDUSessionResourceFailedToSetupItemHOAck.c
	NGAP_PDUSessionResourceFailedToSetupListPSReq.c
	NGAP_PDUSessionResourceFailedToSetupItemPSReq.c
	NGAP_PDUSessionResourceFailedToSetupListSURes.c
	NGAP_PDUSessionResourceFailedToSetupItemSURes.c
	NGAP_PDUSessionResourceHandoverList.c
	NGAP_PDUSessionResourceHandoverItem.c
	NGAP_PDUSessionResourceInformationList.c
	NGAP_PDUSessionResourceInformationItem.c
	NGAP_PDUSessionResourceListCxtRelCpl.c
	NGAP_PDUSessionResourceItemCxtRelCpl.c
	NGAP_PDUSessionResourceListCxtRelReq.c
	NGAP_PDUSessionResourceItemCxtRelReq.c
	NGAP_PDUSessionResourceListHORqd.c
	NGAP_PDUSessionResourceItemHORqd.c
	NGAP_PDUSessionResourceModifyConfirmTransfer.c
	NGAP_PDUSessionResourceModifyIndicationUnsuccessfulTransfer.c
	NGAP_PDUSessionResourceModifyRequestTransfer.c
	NGAP_PDUSessionResourceModifyResponseTransfer.c
	NGAP_PDUSessionResourceModifyIndicationTransfer.c
	NGAP_PDUSessionResourceModifyListModCfm.c
	NGAP_PDUSessionResourceModifyItemModCfm.c
	NGAP_PDUSessionResourceModifyListModInd.c
	NGAP_PDUSessionResourceModifyItemModInd.c
	NGAP_PDUSessionResourceModifyListModReq.c
	NGAP_PDUSessionResourceModifyItemModReq.c
	NGAP_PDUSessionResourceModifyListModRes.c
	NGAP_PDUSessionResourceModifyItemModRes.c
	NGAP_PDUSessionResourceModifyUnsuccessfulTransfer.c
	NGAP_PDUSessionResourceNotifyList.c
	NGAP_PDUSessionResourceNotifyItem.c
	NGAP_PDUSessionResourceNotifyReleasedTransfer.c
	NGAP_PDUSessionResourceNotifyTransfer.c
	NGAP_PDUSessionResourceReleaseCommandTransfer.c
	NGAP_PDUSessionResourceReleasedListNot.c
	NGAP_PDUSessionResourceReleasedItemNot.c
	NGAP_PDUSessionResourceReleasedListPSAck.c
	NGAP_PDUSessionResourceReleasedItemPSAck.c
	NGAP_PDUSessionResourceReleasedListPSFail.c
	NGAP_PDUSessionResourceReleasedItemPSFail.c
	NGAP_PDUSessionResourceReleasedListRelRes.c
	NGAP_PDUSessionResourceReleasedItemRelRes.c
	NGAP_PDUSessionResourceReleaseResponseTransfer.c
	NGAP_PDUSessionResourceSecondaryRATUsageList.c
	NGAP_PDUSessionResourceSecondaryRATUsageItem.c
	NGAP_PDUSessionResourceSetupListCxtReq.c
	NGAP_PDUSessionResourceSetupItemCxtReq.c
	NGAP_PDUSessionResourceSetupListCxtRes.c
	NGAP_PDUSessionResourceSetupItemCxtRes.c
	NGAP_PDUSessionResourceSetupListHOReq.c
	NGAP_PDUSessionResourceSetupItemHOReq.c
	NGAP_PDUSessionResourceSetupListSUReq.c
	NGAP_PDUSessionResourceSetupItemSUReq.c
	NGAP_PDUSessionResourceSetupListSURes.c
	NGAP_PDUSessionResourceSetupItemSURes.c
	NGAP_PDUSessionResourceSetupRequestTransfer.c
	NGAP_PDUSessionResourceSetupResponseTransfer.c
	NGAP_PDUSessionResourceSetupUnsuccessfulTransfer.c
	NGAP_PDUSessionResourceSwitchedList.c
	NGAP_PDUSessionResourceSwitchedItem.c
	NGAP_PDUSessionResourceToBeSwitchedDLList.c
	NGAP_PDUSessionResourceToBeSwitchedDLItem.c
	NGAP_PDUSessionResourceToReleaseListHOCmd.c
	NGAP_PDUSessionResourceToReleaseItemHOCmd.c
	NGAP_PDUSessionResourceToReleaseListRelCmd.c
	NGAP_PDUSessionResourceToReleaseItemRelCmd.c
	NGAP_PDUSessionType.c
	NGAP_PDUSessionUsageReport.c
	NGAP_PeriodicRegistrationUpdateTimer.c
	NGAP_PLMNIdentity.c
	NGAP_PLMNSupportList.c
	NGAP_PLMNSupportItem.c
	NGAP_PortNumber.c
	NGAP_Pre-emptionCapability.c
	NGAP_Pre-emptionVulnerability.c
	NGAP_PriorityLevelARP.c
	NGAP_PriorityLevelQos.c
	NGAP_PWSFailedCellIDList.c
	NGAP_QosCharacteristics.c
	NGAP_QosFlowAcceptedList.c
	NGAP_QosFlowAcceptedItem.c
	NGAP_QosFlowAddOrModifyRequestList.c
	NGAP_QosFlowAddOrModifyRequestItem.c
	NGAP_QosFlowAddOrModifyResponseList.c
	NGAP_QosFlowAddOrModifyResponseItem.c
	NGAP_QosFlowIdentifier.c
	NGAP_QosFlowInformationList.c
	NGAP_QosFlowInformationItem.c
	NGAP_QosFlowLevelQosParameters.c
	NGAP_QosMonitoringRequest.c
	NGAP_QosFlowListWithCause.c
	NGAP_QosFlowWithCauseItem.c
	NGAP_QosFlowModifyConfirmList.c
	NGAP_QosFlowModifyConfirmItem.c
	NGAP_QosFlowNotifyList.c
	NGAP_QosFlowNotifyItem.c
	NGAP_QosFlowPerTNLInformation.c
	NGAP_QosFlowPerTNLInformationList.c
	NGAP_QosFlowPerTNLInformationItem.c
	NGAP_QosFlowSetupRequestList.c
	NGAP_QosFlowSetupRequestItem.c
	NGAP_QosFlowListWithDataForwarding.c
	NGAP_QosFlowItemWithDataForwarding.c
	NGAP_QosFlowToBeForwardedList.c
	NGAP_QosFlowToBeForwardedItem.c
	NGAP_QoSFlowsUsageReportList.c
	NGAP_QoSFlowsUsageReport-Item.c
	NGAP_RANNodeName.c
	NGAP_RANPagingPriority.c
	NGAP_RANStatusTransfer-TransparentContainer.c
	NGAP_RAN-UE-NGAP-ID.c
	NGAP_RAT-Information.c
	NGAP_RATRestrictions.c
	NGAP_RATRestrictions-Item.c
	NGAP_RATRestrictionInformation.c
	NGAP_RecommendedCellsForPaging.c
	NGAP_RecommendedCellList.c
	NGAP_RecommendedCellItem.c
	NGAP_RecommendedRANNodesForPaging.c
	NGAP_RecommendedRANNodeList.c
	NGAP_RecommendedRANNodeItem.c
	NGAP_RedirectionVoiceFallback.c
	NGAP_ReflectiveQosAttribute.c
	NGAP_RelativeAMFCapacity.c
	NGAP_ReportArea.c
	NGAP_RepetitionPeriod.c
	NGAP_ResetAll.c
	NGAP_ResetType.c
	NGAP_RNC-ID.c
	NGAP_RoutingID.c
	NGAP_RRCContainer.c
	NGAP_RRCEstablishmentCause.c
	NGAP_RRCInactiveTransitionReportRequest.c
	NGAP_RRCState.c
	NGAP_RIMInformationTransfer.c
	NGAP_RIMInformation.c
	NGAP_GNBSetID.c
	NGAP_SCTP-TLAs.c
	NGAP_SD.c
	NGAP_SecondaryRATUsageInformation.c
	NGAP_SecondaryRATDataUsageReportTransfer.c
	NGAP_SecurityContext.c
	NGAP_SecurityIndication.c
	NGAP_SecurityKey.c
	NGAP_SecurityResult.c
	NGAP_SerialNumber.c
	NGAP_ServedGUAMIList.c
	NGAP_ServedGUAMIItem.c
	NGAP_ServiceAreaInformation.c
	NGAP_ServiceAreaInformation-Item.c
	NGAP_SgNB-UE-X2AP-ID.c
	NGAP_SliceOverloadList.c
	NGAP_SliceOverloadItem.c
	NGAP_SliceSupportList.c
	NGAP_SliceSupportItem.c
	NGAP_S-NSSAI.c
	NGAP_SONConfigurationTransfer.c
	NGAP_SONInformation.c
	NGAP_SONInformationReply.c
	NGAP_SONInformationRequest.c
	NGAP_SourceNGRANNode-ToTargetNGRANNode-TransparentContainer.c
	NGAP_SourceOfUEActivityBehaviourInformation.c
	NGAP_SourceRANNodeID.c
	NGAP_SourceToTarget-TransparentContainer.c
	NGAP_SourceToTarget-AMFInformationReroute.c
	NGAP_SRVCCOperationPossible.c
	NGAP_ConfiguredNSSAI.c
	NGAP_RejectedNSSAIinPLMN.c
	NGAP_RejectedNSSAIinTA.c
	NGAP_SST.c
	NGAP_SupportedTAList.c
	NGAP_SupportedTAItem.c
	NGAP_TAC.c
	NGAP_TAI.c
	NGAP_TAIBroadcastEUTRA.c
	NGAP_TAIBroadcastEUTRA-Item.c
	NGAP_TAIBroadcastNR.c
	NGAP_TAIBroadcastNR-Item.c
	NGAP_TAICancelledEUTRA.c
	NGAP_TAICancelledEUTRA-Item.c
	NGAP_TAICancelledNR.c
	NGAP_TAICancelledNR-Item.c
	NGAP_TAIListForInactive.c
	NGAP_TAIListForInactiveItem.c
	NGAP_TAIListForPaging.c
	NGAP_TAIListForPagingItem.c
	NGAP_TAIListForRestart.c
	NGAP_TAIListForWarning.c
	NGAP_TargeteNB-ID.c
	NGAP_TargetID.c
	NGAP_TargetNGRANNode-ToSourceNGRANNode-TransparentContainer.c
	NGAP_TargetRANNodeID.c
	NGAP_TargetRNC-ID.c
	NGAP_TargetToSource-TransparentContainer.c
	NGAP_TimerApproachForGUAMIRemoval.c
	NGAP_TimeStamp.c
	NGAP_TimeToWait.c
	NGAP_TimeUEStayedInCell.c
	NGAP_TimeUEStayedInCellEnhancedGranularity.c
	NGAP_TNLAddressWeightFactor.c
	NGAP_TNLAssociationList.c
	NGAP_TNLAssociationItem.c
	NGAP_TNLAssociationUsage.c
	NGAP_TraceActivation.c
	NGAP_TraceDepth.c
	NGAP_TrafficLoadReductionIndication.c
	NGAP_TransportLayerAddress.c
	NGAP_TypeOfError.c
	NGAP_UEAggregateMaximumBitRate.c
	NGAP_UE-associatedLogicalNG-connectionList.c
	NGAP_UE-associatedLogicalNG-connectionItem.c
	NGAP_UEContextRequest.c
	NGAP_UEHistoryInformation.c
	NGAP_UEIdentityIndexValue.c
	NGAP_UE-NGAP-IDs.c
	NGAP_UE-NGAP-ID-pair.c
	NGAP_UEPagingIdentity.c
	NGAP_UEPresence.c
	NGAP_UEPresenceInAreaOfInterestList.c
	NGAP_UEPresenceInAreaOfInterestItem.c
	NGAP_UERadioCapability.c
	NGAP_UERadioCapabilityForPaging.c
	NGAP_UERadioCapabilityForPagingOfNR.c
	NGAP_UERadioCapabilityForPagingOfEUTRA.c
	NGAP_UERetentionInformation.c
	NGAP_UESecurityCapabilities.c
	NGAP_UL-NGU-UP-TNLModifyList.c
	NGAP_UL-NGU-UP-TNLModifyItem.c
	NGAP_UnavailableGUAMIList.c
	NGAP_UnavailableGUAMIItem.c
	NGAP_ULForwarding.c
	NGAP_UPTransportLayerInformation.c
	NGAP_UPTransportLayerInformationList.c
	NGAP_UPTransportLayerInformationItem.c
	NGAP_UPTransportLayerInformationPairList.c
	NGAP_UPTransportLayerInformationPairItem.c
	NGAP_UserLocationInformation.c
	NGAP_UserLocationInformationEUTRA.c
	NGAP_UserLocationInformationN3IWF.c
	NGAP_UserLocationInformationNR.c
	NGAP_UserPlaneSecurityInformation.c
	NGAP_VolumeTimedReportList.c
	NGAP_VolumeTimedReport-Item.c
	NGAP_WarningAreaCoordinates.c
	NGAP_WarningAreaList.c
	NGAP_WarningMessageContents.c
	NGAP_WarningSecurityInfo.c
	NGAP_WarningType.c
	NGAP_XnExtTLAs.c
	NGAP_XnExtTLA-Item.c
	NGAP_XnGTP-TLAs.c
	NGAP_XnTLAs.c
	NGAP_XnTNLConfigurationInfo.c
	NGAP_Criticality.c
	NGAP_Presence.c
	NGAP_PrivateIE-ID.c
	NGAP_ProcedureCode.c
	NGAP_ProtocolExtensionID.c
	NGAP_ProtocolIE-ID.c
	NGAP_TriggeringMessage.c
	NGAP_ProtocolIE-Container.c
	NGAP_ProtocolIE-SingleContainer.c
	NGAP_ProtocolIE-Field.c
	NGAP_ProtocolIE-ContainerList.c
	NGAP_ProtocolExtensionContainer.c
	NGAP_ProtocolExtensionField.c
	NGAP_PrivateIE-Container.c
	NGAP_PrivateIE-Field.c
'''.split())

libasn1c_ngap_inc = include_directories('.')

libasn1c_ngap = library('ogsasn1c-ngap',
    sources : libasn1c_ngap_sources,
    version : libogslib_version,
    c_args : libasn1c_common_cc_flags,
    include_directories : libasn1c_ngap_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep],
    install : true)

libasn1c_ngap_dep = declare_dependency(
    link_with : libasn1c_ngap,
    include_directories : libasn1c_ngap_inc,
    dependencies : [libasn1c_common_dep, libasn1c_util_dep])
