Modify 36413-e40.txt to 36413-e40.asn
===========================================
user@host ~/Documents/git/nextgnb/lib/asn1c/support/s1ap-r14.4.0$ \
    diff 36413-e40.txt 36413-e40.asn

Use velichkov git's fork for asn1c
===========================================
user@host ~/Documents/git/my$ \
    git clone https://github.com/velichkov/asn1c.git
user@host ~/Documents/git/my$ \
    git checkout s1ap
user@host Documents/git/my/asn1c$ \
    autoreconf -fi;./configure;make -j4

ASN.1 encoder/decoder
===========================================
user@host ~/Documents/git/open5gs/lib/asn1c/s1ap$ \
    ASN1C_PREFIX=S1AP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    ../support/s1ap-r14.4.0/36413-e40.asn

    ASN1C_PREFIX=NGAP_ ../../../../my/asn1c/asn1c/asn1c -pdu=all \
    -fcompound-names -findirect-choice -fno-include-deps \
    ../support/ngap-r16.1.0/38413-g10.asn

Fix NGAP_ProtocolExtensionField.c
===========================================
diff --git a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
index 26da1b34..940db758 100644
--- a/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
+++ b/lib/asn1c/ngap/NGAP_ProtocolExtensionField.c
@@ -27671,7 +27671,11 @@ static asn_TYPE_member_t asn_MBR_NGAP_extensionValue_396[] = {
                0,
                &asn_DEF_OCTET_STRING,
                0,
+#if 0 /* modified by acetcom */
                { &asn_OER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_17, &asn_PER_memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constr_17,  memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_396 },
+#else
+               { 0, 0,  memb_NGAP_OCTET_STRING_CONTAINING_PDUSessionResourceReleaseResponseTransfer__constraint_396 },
+#endif
                0, 0, /* No default value */
                "OCTET STRING (CONTAINING PDUSessionResourceReleaseResponseTransfer)"
                },

Update Files
===========================================
user@host ~/Documents/git/open5gs/lib/s1ap/s1ap/asn1c$ \
    diff Makefile.am Makefile.am.libasncodec

user@host ~/Documents/git/open5gs/lib/s1ap/asn1c$ \
    git diff asn_internal.h

Remove unnecessary files
===========================================
user@host ~/Documents/git/open5gs/lib/s1ap/s1ap/asn1c$ \
    rm -f S1AP_E-RAB-IE-ContainerPairList.c \
    S1AP_ProtocolError-IE-ContainerList.c \
    S1AP_ProtocolIE-ContainerPair.c S1AP_ProtocolIE-FieldPair.c \
    S1AP_ProtocolIE-ContainerPairList.c

user@host ~/Documents/git/open5gs/lib/s1ap/asn1c$ \
    rm -f Makefile.am.asn1convert Makefile.am.libasncodec

user@host ~/Documents/git/open5gs/lib/s1ap/asn1c$ \
    rm -f converter-example.mk converter-example.c pdu_collection.c

