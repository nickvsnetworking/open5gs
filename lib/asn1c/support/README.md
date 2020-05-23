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

