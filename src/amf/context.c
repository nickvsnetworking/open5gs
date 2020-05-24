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

#include "ngap-path.h"

static amf_context_t self;

int __amf_log_domain;

static OGS_POOL(amf_gnb_pool, amf_gnb_t);

static int context_initialized = 0;

void amf_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize AMF context */
    memset(&self, 0, sizeof(amf_context_t));

    ogs_log_install_domain(&__amf_log_domain, "amf", ogs_core()->log.level);

    ogs_list_init(&self.ngap_list);
    ogs_list_init(&self.ngap_list6);

    /* Allocate TWICE the pool to check if maximum number of gNBs is reached */
    ogs_pool_init(&amf_gnb_pool, ogs_config()->max.gnb*2);

    ogs_list_init(&self.gnb_list);
    self.gnb_addr_hash = ogs_hash_make();
    self.gnb_id_hash = ogs_hash_make();

    context_initialized = 1;
}

void amf_context_final(void)
{
    ogs_assert(context_initialized == 1);

    amf_gnb_remove_all();

    ogs_assert(self.gnb_addr_hash);
    ogs_hash_destroy(self.gnb_addr_hash);
    ogs_assert(self.gnb_id_hash);
    ogs_hash_destroy(self.gnb_id_hash);

    ogs_pool_final(&amf_gnb_pool);

    context_initialized = 0;
}

amf_context_t *amf_self(void)
{
    return &self;
}

static int amf_context_prepare(void)
{
    self.nf_type = OpenAPI_nf_type_AMF;

    self.relative_capacity = 0xff;

    self.ngap_port = OGS_NGAP_SCTP_PORT;

    return OGS_OK;
}

static int amf_context_validation(void)
{
    if (ogs_list_first(&self.ngap_list) == NULL &&
        ogs_list_first(&self.ngap_list6) == NULL) {
        ogs_error("No amf.ngap in '%s'", ogs_config()->file);
        return OGS_RETRY;
    }

    if (self.max_num_of_served_guami == 0) {
        ogs_error("No amf.guami in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }

    if (self.served_guami[0].num_of_plmn_id == 0) {
        ogs_error("No amf.guami.plmn_id in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }

#if 0
    if (self.served_guami[0].num_of_amf_gid == 0) {
        ogs_error("No amf.guami.amf_gid in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }

    if (self.served_guami[0].num_of_amf_code == 0) {
        ogs_error("No amf.guami.amf_code in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }
#endif

    if (self.num_of_served_tai == 0) {
        ogs_error("No amf.tai in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }

    if (self.served_tai[0].list0.tai[0].num == 0 &&
        self.served_tai[0].list2.num == 0) {
        ogs_error("No amf.tai.plmn_id|tac in '%s'", ogs_config()->file);
        return OGS_ERROR;
    }

    if (self.num_of_integrity_order == 0) {
        ogs_error("No amf.security.integrity_order in '%s'",
                ogs_config()->file);
        return OGS_ERROR;
    }
    if (self.num_of_ciphering_order == 0) {
        ogs_error("no amf.security.ciphering_order in '%s'",
                ogs_config()->file);
        return OGS_ERROR;
    }


    return OGS_OK;
}

int amf_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_config()->document;
    ogs_assert(document);

    rv = amf_context_prepare();
    if (rv != OGS_OK) return rv;

    rv = amf_context_validation();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "amf")) {
            ogs_yaml_iter_t amf_iter;
            ogs_yaml_iter_recurse(&root_iter, &amf_iter);
            while (ogs_yaml_iter_next(&amf_iter)) {
                const char *amf_key = ogs_yaml_iter_key(&amf_iter);
                ogs_assert(amf_key);
                if (!strcmp(amf_key, "relative_capacity")) {
                    const char *v = ogs_yaml_iter_value(&amf_iter);
                    if (v) self.relative_capacity = atoi(v);
                } else if (!strcmp(amf_key, "ngap")) {
                    ogs_yaml_iter_t ngap_array, ngap_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &ngap_array);
                    do {
                        int family = AF_UNSPEC;
                        int i, num = 0;
                        const char *hostname[OGS_MAX_NUM_OF_HOSTNAME];
                        uint16_t port = self.ngap_port;
                        const char *dev = NULL;
                        ogs_sockaddr_t *addr = NULL;

                        if (ogs_yaml_iter_type(&ngap_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&ngap_iter, &ngap_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&ngap_array))
                                break;
                            ogs_yaml_iter_recurse(&ngap_array, &ngap_iter);
                        } else if (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&ngap_iter)) {
                            const char *ngap_key =
                                ogs_yaml_iter_key(&ngap_iter);
                            ogs_assert(ngap_key);
                            if (!strcmp(ngap_key, "family")) {
                                const char *v = ogs_yaml_iter_value(&ngap_iter);
                                if (v) family = atoi(v);
                                if (family != AF_UNSPEC &&
                                    family != AF_INET && family != AF_INET6) {
                                    ogs_warn("Ignore family(%d) : "
                                        "AF_UNSPEC(%d), "
                                        "AF_INET(%d), AF_INET6(%d) ", 
                                        family, AF_UNSPEC, AF_INET, AF_INET6);
                                    family = AF_UNSPEC;
                                }
                            } else if (!strcmp(ngap_key, "addr") ||
                                    !strcmp(ngap_key, "name")) {
                                ogs_yaml_iter_t hostname_iter;
                                ogs_yaml_iter_recurse(
                                        &ngap_iter, &hostname_iter);
                                ogs_assert(ogs_yaml_iter_type(&hostname_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    if (ogs_yaml_iter_type(&hostname_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&hostname_iter))
                                            break;
                                    }

                                    ogs_assert(num <= OGS_MAX_NUM_OF_HOSTNAME);
                                    hostname[num++] = 
                                        ogs_yaml_iter_value(&hostname_iter);
                                } while (
                                    ogs_yaml_iter_type(&hostname_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(ngap_key, "port")) {
                                const char *v = ogs_yaml_iter_value(&ngap_iter);
                                if (v) port = atoi(v);
                            } else if (!strcmp(ngap_key, "dev")) {
                                dev = ogs_yaml_iter_value(&ngap_iter);
                            } else
                                ogs_warn("unknown key `%s`", ngap_key);
                        }

                        addr = NULL;
                        for (i = 0; i < num; i++) {
                            rv = ogs_addaddrinfo(&addr,
                                    family, hostname[i], port, 0);
                            ogs_assert(rv == OGS_OK);
                        }

                        if (addr) {
                            if (ogs_config()->parameter.no_ipv4 == 0)
                                ogs_socknode_add(
                                        &self.ngap_list, AF_INET, addr);
                            if (ogs_config()->parameter.no_ipv6 == 0)
                                ogs_socknode_add(
                                        &self.ngap_list6, AF_INET6, addr);
                            ogs_freeaddrinfo(addr);
                        }

                        if (dev) {
                            rv = ogs_socknode_probe(
                                    ogs_config()->parameter.no_ipv4 ?
                                        NULL : &self.ngap_list,
                                    ogs_config()->parameter.no_ipv6 ?
                                        NULL : &self.ngap_list6,
                                    dev, port);
                            ogs_assert(rv == OGS_OK);
                        }

                    } while (ogs_yaml_iter_type(&ngap_array) ==
                            YAML_SEQUENCE_NODE);

                    if (ogs_list_first(&self.ngap_list) == NULL &&
                        ogs_list_first(&self.ngap_list6) == NULL) {
                        rv = ogs_socknode_probe(
                                ogs_config()->parameter.no_ipv4 ?
                                    NULL : &self.ngap_list,
                                ogs_config()->parameter.no_ipv6 ?
                                    NULL : &self.ngap_list6,
                                NULL, self.ngap_port);
                        ogs_assert(rv == OGS_OK);
                    }
                } else if (!strcmp(amf_key, "guami")) {
                    ogs_yaml_iter_t guami_array, guami_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &guami_array);
                    do {
                        served_guami_t *guami = NULL;
                        ogs_assert(self.max_num_of_served_guami <=
                                MAX_NUM_OF_SERVED_GUAMI);
                        guami = &self.served_guami[
                            self.max_num_of_served_guami];
                        ogs_assert(guami);

                        if (ogs_yaml_iter_type(&guami_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&guami_iter, &guami_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&guami_array))
                                break;
                            ogs_yaml_iter_recurse(&guami_array,
                                    &guami_iter);
                        } else if (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&guami_iter)) {
                            const char *guami_key =
                                ogs_yaml_iter_key(&guami_iter);
                            ogs_assert(guami_key);
                            if (!strcmp(guami_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_array, plmn_id_iter;
                                ogs_yaml_iter_recurse(&guami_iter,
                                        &plmn_id_array);
                                do {
                                    ogs_plmn_id_t *plmn_id = NULL;
                                    const char *mcc = NULL, *mnc = NULL;
                                    ogs_assert(guami->num_of_plmn_id <=
                                            OGS_MAX_NUM_OF_PLMN_ID);
                                    plmn_id = &guami->plmn_id[
                                        guami->num_of_plmn_id];
                                    ogs_assert(plmn_id);

                                    if (ogs_yaml_iter_type(&plmn_id_array) ==
                                            YAML_MAPPING_NODE) {
                                        memcpy(&plmn_id_iter, &plmn_id_array,
                                                sizeof(ogs_yaml_iter_t));
                                    } else if (ogs_yaml_iter_type(
                                                &plmn_id_array) ==
                                                YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&plmn_id_array))
                                            break;
                                        ogs_yaml_iter_recurse(&plmn_id_array,
                                                &plmn_id_iter);
                                    } else if (ogs_yaml_iter_type(
                                                &plmn_id_array) ==
                                                YAML_SCALAR_NODE) {
                                        break;
                                    } else
                                        ogs_assert_if_reached();

                                    while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                        const char *plmn_id_key =
                                            ogs_yaml_iter_key(&plmn_id_iter);
                                        ogs_assert(plmn_id_key);
                                        if (!strcmp(plmn_id_key, "mcc"))
                                        {
                                            mcc = ogs_yaml_iter_value(
                                                    &plmn_id_iter);
                                        } else if (!strcmp(
                                                    plmn_id_key, "mnc")) {
                                            mnc = ogs_yaml_iter_value(
                                                    &plmn_id_iter);
                                        }
                                    }

                                    if (mcc && mnc) {
                                        ogs_plmn_id_build(plmn_id,
                                            atoi(mcc), atoi(mnc), strlen(mnc));
                                        guami->num_of_plmn_id++;
                                    }

                                } while (ogs_yaml_iter_type(&plmn_id_array) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(guami_key, "amf_region_id")) {
                                ogs_yaml_iter_t amf_region_id_iter;
                                ogs_yaml_iter_recurse(
                                    &guami_iter, &amf_region_id_iter);
                                ogs_assert(ogs_yaml_iter_type(
                                    &amf_region_id_iter) != YAML_MAPPING_NODE);

                                do {
                                    uint16_t *amf_region_id = NULL;
                                    const char *v = NULL;

                                    ogs_assert(guami->num_of_amf_region_id <=
                                            REGION_PER_AMF);
                                    amf_region_id = &guami->amf_region_id[
                                        guami->num_of_amf_region_id];
                                    ogs_assert(amf_region_id);

                                    if (ogs_yaml_iter_type(
                                            &amf_region_id_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(
                                                &amf_region_id_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(
                                            &amf_region_id_iter);
                                    if (v) {
                                        *amf_region_id = atoi(v);
                                        guami->num_of_amf_region_id++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&amf_region_id_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else if (!strcmp(guami_key, "amf_set_id")) {
                                ogs_yaml_iter_t amf_set_id_iter;
                                ogs_yaml_iter_recurse(&guami_iter,
                                        &amf_set_id_iter);
                                ogs_assert(ogs_yaml_iter_type(
                                        &amf_set_id_iter) != YAML_MAPPING_NODE);

                                do {
                                    uint16_t *amf_set_id = NULL;
                                    const char *v = NULL;

                                    ogs_assert(guami->num_of_amf_set_id <=
                                            SET_PER_AMF);
                                    amf_set_id = &guami->amf_set_id[
                                        guami->num_of_amf_set_id];
                                    ogs_assert(amf_set_id);

                                    if (ogs_yaml_iter_type(&amf_set_id_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(
                                                    &amf_set_id_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&amf_set_id_iter);
                                    if (v) {
                                        *amf_set_id = atoi(v);
                                        guami->num_of_amf_set_id++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&amf_set_id_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", guami_key);
                        }

                        if (guami->num_of_plmn_id &&
                            guami->num_of_amf_region_id &&
                            guami->num_of_amf_set_id) {
                            self.max_num_of_served_guami++;
                        } else {
                            ogs_warn("Ignore guami : "
                                    "plmn_id(%d), amf_region_id(%d),"
                                    "amf_set_id(%d)",
                                guami->num_of_plmn_id,
                                guami->num_of_amf_region_id,
                                guami->num_of_amf_set_id);
                            guami->num_of_plmn_id = 0;
                            guami->num_of_amf_region_id = 0;
                            guami->num_of_amf_set_id = 0;
                        }
                    } while (ogs_yaml_iter_type(&guami_array) ==
                            YAML_SEQUENCE_NODE);
                } else if (!strcmp(amf_key, "tai")) {
                    int num_of_list0 = 0;
                    ogs_tai0_list_t *list0 = NULL;
                    ogs_tai2_list_t *list2 = NULL;

                    ogs_assert(self.num_of_served_tai <=
                            OGS_MAX_NUM_OF_SERVED_TAI);
                    list0 = &self.served_tai[self.num_of_served_tai].list0;
                    ogs_assert(list0);
                    list2 = &self.served_tai[self.num_of_served_tai].list2;
                    ogs_assert(list2);

                    ogs_yaml_iter_t tai_array, tai_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &tai_array);
                    do {
                        const char *mcc = NULL, *mnc = NULL;
                        uint16_t tac[OGS_MAX_NUM_OF_TAI];
                        int num_of_tac = 0;

                        if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_MAPPING_NODE) {
                            memcpy(&tai_iter, &tai_array,
                                    sizeof(ogs_yaml_iter_t));
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE) {
                            if (!ogs_yaml_iter_next(&tai_array))
                                break;
                            ogs_yaml_iter_recurse(&tai_array,
                                    &tai_iter);
                        } else if (ogs_yaml_iter_type(&tai_array) ==
                                YAML_SCALAR_NODE) {
                            break;
                        } else
                            ogs_assert_if_reached();

                        while (ogs_yaml_iter_next(&tai_iter)) {
                            const char *tai_key = ogs_yaml_iter_key(&tai_iter);
                            ogs_assert(tai_key);
                            if (!strcmp(tai_key, "plmn_id")) {
                                ogs_yaml_iter_t plmn_id_iter;

                                ogs_yaml_iter_recurse(&tai_iter, &plmn_id_iter);
                                while (ogs_yaml_iter_next(&plmn_id_iter)) {
                                    const char *plmn_id_key =
                                        ogs_yaml_iter_key(&plmn_id_iter);
                                    ogs_assert(plmn_id_key);
                                    if (!strcmp(plmn_id_key, "mcc")) {
                                        mcc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    } else if (!strcmp(plmn_id_key, "mnc")) {
                                        mnc = ogs_yaml_iter_value(
                                                &plmn_id_iter);
                                    }
                                }
                            } else if (!strcmp(tai_key, "tac")) {
                                ogs_yaml_iter_t tac_iter;
                                ogs_yaml_iter_recurse(&tai_iter, &tac_iter);
                                ogs_assert(ogs_yaml_iter_type(&tac_iter) !=
                                    YAML_MAPPING_NODE);

                                do {
                                    const char *v = NULL;

                                    ogs_assert(num_of_tac <=
                                            OGS_MAX_NUM_OF_TAI);
                                    if (ogs_yaml_iter_type(&tac_iter) ==
                                            YAML_SEQUENCE_NODE) {
                                        if (!ogs_yaml_iter_next(&tac_iter))
                                            break;
                                    }

                                    v = ogs_yaml_iter_value(&tac_iter);
                                    if (v) {
                                        tac[num_of_tac] = atoi(v);
                                        num_of_tac++;
                                    }
                                } while (
                                    ogs_yaml_iter_type(&tac_iter) ==
                                        YAML_SEQUENCE_NODE);
                            } else
                                ogs_warn("unknown key `%s`", tai_key);
                        }

                        if (mcc && mnc && num_of_tac) {
                            if (num_of_tac == 1) {
                                ogs_plmn_id_build(
                                    &list2->tai[list2->num].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                list2->tai[list2->num].tac = tac[0];

                                list2->num++;
                                if (list2->num > 1)
                                    list2->type = OGS_TAI2_TYPE;
                                else
                                    list2->type = OGS_TAI1_TYPE;
                            } else if (num_of_tac > 1) {
                                int i;
                                ogs_plmn_id_build(
                                    &list0->tai[num_of_list0].plmn_id,
                                    atoi(mcc), atoi(mnc), strlen(mnc));
                                for (i = 0; i < num_of_tac; i++) {
                                    list0->tai[num_of_list0].tac[i] = tac[i];
                                }

                                list0->tai[num_of_list0].num = num_of_tac;
                                list0->tai[num_of_list0].type = OGS_TAI0_TYPE;

                                num_of_list0++;
                            }
                        } else {
                            ogs_warn("Ignore tai : mcc(%p), mnc(%p), "
                                    "num_of_tac(%d)", mcc, mnc, num_of_tac);
                        }
                    } while (ogs_yaml_iter_type(&tai_array) ==
                            YAML_SEQUENCE_NODE);

                    if (list2->num || num_of_list0) {
                        self.num_of_served_tai++;
                    }
                } else if (!strcmp(amf_key, "security")) {
                    ogs_yaml_iter_t security_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &security_iter);
                    while (ogs_yaml_iter_next(&security_iter)) {
                        const char *security_key =
                            ogs_yaml_iter_key(&security_iter);
                        ogs_assert(security_key);
                        if (!strcmp(security_key, "integrity_order")) {
                            ogs_yaml_iter_t integrity_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &integrity_order_iter);
                            ogs_assert(ogs_yaml_iter_type(
                                        &integrity_order_iter) !=
                                YAML_MAPPING_NODE);

                            do {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&integrity_order_iter) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(
                                                &integrity_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&integrity_order_iter);
                                if (v) {
                                    int integrity_index = 
                                        self.num_of_integrity_order;
                                    if (strcmp(v, "EIA0") == 0) {
                                        self.integrity_order[integrity_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_EIA0;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "EIA1") == 0) {
                                        self.integrity_order[integrity_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EIA1;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "EIA2") == 0) {
                                        self.integrity_order[integrity_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EIA2;
                                        self.num_of_integrity_order++;
                                    } else if (strcmp(v, "EIA3") == 0) {
                                        self.integrity_order[integrity_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EIA3;
                                        self.num_of_integrity_order++;
                                    }
                                }
                            } while (
                                ogs_yaml_iter_type(&integrity_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        } else if (!strcmp(security_key, "ciphering_order")) {
                            ogs_yaml_iter_t ciphering_order_iter;
                            ogs_yaml_iter_recurse(&security_iter,
                                    &ciphering_order_iter);
                            ogs_assert(ogs_yaml_iter_type(
                                &ciphering_order_iter) != YAML_MAPPING_NODE);

                            do {
                                const char *v = NULL;

                                if (ogs_yaml_iter_type(&ciphering_order_iter) ==
                                        YAML_SEQUENCE_NODE) {
                                    if (!ogs_yaml_iter_next(
                                                &ciphering_order_iter))
                                        break;
                                }

                                v = ogs_yaml_iter_value(&ciphering_order_iter);
                                if (v) {
                                    int ciphering_index = 
                                        self.num_of_ciphering_order;
                                    if (strcmp(v, "EEA0") == 0) {
                                        self.ciphering_order[ciphering_index] = 
                                            OGS_NAS_SECURITY_ALGORITHMS_EEA0;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "EEA1") == 0) {
                                        self.ciphering_order[ciphering_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EEA1;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "EEA2") == 0) {
                                        self.ciphering_order[ciphering_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EEA2;
                                        self.num_of_ciphering_order++;
                                    } else if (strcmp(v, "EEA3") == 0) {
                                        self.ciphering_order[ciphering_index] = 
                                        OGS_NAS_SECURITY_ALGORITHMS_128_EEA3;
                                        self.num_of_ciphering_order++;
                                    }
                                }
                            } while (
                                ogs_yaml_iter_type(&ciphering_order_iter) ==
                                    YAML_SEQUENCE_NODE);
                        }
                    }
                } else if (!strcmp(amf_key, "network_name")) {
                    ogs_yaml_iter_t network_name_iter;
                    ogs_yaml_iter_recurse(&amf_iter, &network_name_iter);

                    while (ogs_yaml_iter_next(&network_name_iter)) {
                        const char *network_name_key =
                        ogs_yaml_iter_key(&network_name_iter);
                        ogs_assert(network_name_key);
                        if (!strcmp(network_name_key, "full")) {  
                            ogs_nas_network_name_t *network_full_name =
                                &self.full_name;
                            const char *c_network_name =
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
                            for (i = 0;i<size;i++) {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_full_name->name[i*2] = 0;
                                network_full_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                            network_full_name->length = size*2+1;
                            network_full_name->coding_scheme = 1;
                        } else if (!strcmp(network_name_key, "short")) {
                            ogs_nas_network_name_t *network_short_name =
                                &self.short_name;
                            const char *c_network_name =
                                ogs_yaml_iter_value(&network_name_iter);
                            uint8_t size = strlen(c_network_name);
                            uint8_t i;
                            for (i = 0;i<size;i++) {
                                /* Workaround to convert the ASCII to USC-2 */
                                network_short_name->name[i*2] = 0;
                                network_short_name->name[(i*2)+1] =
                                    c_network_name[i];

                            }
                            network_short_name->length = size*2+1;
                            network_short_name->coding_scheme = 1;
                        }
                    }
                } else if (!strcmp(amf_key, "sbi")) {
                    /* handle config in sbi library */
                } else
                    ogs_warn("unknown key `%s`", amf_key);
            }
        }
    }

    return OGS_OK;
}

amf_gnb_t *amf_gnb_add(ogs_sock_t *sock, ogs_sockaddr_t *addr)
{
    amf_gnb_t *gnb = NULL;
    amf_event_t e;

    ogs_assert(sock);
    ogs_assert(addr);

    ogs_pool_alloc(&amf_gnb_pool, &gnb);
    ogs_assert(gnb);
    memset(gnb, 0, sizeof *gnb);

    gnb->sock = sock;
    gnb->addr = addr;
    gnb->sock_type = amf_gnb_sock_type(gnb->sock);

    gnb->max_num_of_ostreams = DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS;
    gnb->ostream_id = 0;
    if (ogs_config()->sockopt.sctp.max_num_of_ostreams) {
        gnb->max_num_of_ostreams =
            ogs_config()->sockopt.sctp.max_num_of_ostreams;
        ogs_info("[ENB] max_num_of_ostreams : %d", gnb->max_num_of_ostreams);
    }

    ogs_list_init(&gnb->gnb_ue_list);

    if (gnb->sock_type == SOCK_STREAM) {
        gnb->poll = ogs_pollset_add(amf_self()->pollset,
            OGS_POLLIN, sock->fd, ngap_recv_upcall, sock);
        ogs_assert(gnb->poll);
    }

    ogs_hash_set(self.gnb_addr_hash, gnb->addr, sizeof(ogs_sockaddr_t), gnb);

    e.gnb = gnb;
    e.id = 0;
    ogs_fsm_create(&gnb->sm, ngap_state_initial, ngap_state_final);
    ogs_fsm_init(&gnb->sm, &e);

    ogs_list_add(&self.gnb_list, gnb);

    return gnb;
}

int amf_gnb_remove(amf_gnb_t *gnb)
{
    amf_event_t e;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_list_remove(&self.gnb_list, gnb);

    e.gnb = gnb;
    ogs_fsm_fini(&gnb->sm, &e);
    ogs_fsm_delete(&gnb->sm);

    ogs_hash_set(self.gnb_addr_hash, gnb->addr, sizeof(ogs_sockaddr_t), NULL);
    ogs_hash_set(self.gnb_id_hash, &gnb->gnb_id, sizeof(gnb->gnb_id), NULL);

#if 0
    gnb_ue_remove_in_gnb(gnb);
#endif

    if (gnb->sock_type == SOCK_STREAM) {
        ogs_pollset_remove(gnb->poll);
        ogs_sctp_destroy(gnb->sock);
    }

    ogs_free(gnb->addr);

    ogs_pool_free(&amf_gnb_pool, gnb);

    return OGS_OK;
}

int amf_gnb_remove_all()
{
    amf_gnb_t *gnb = NULL, *next_gnb = NULL;

    ogs_list_for_each_safe(&self.gnb_list, next_gnb, gnb)
        amf_gnb_remove(gnb);

    return OGS_OK;
}

amf_gnb_t *amf_gnb_find_by_addr(ogs_sockaddr_t *addr)
{
    ogs_assert(addr);
    return (amf_gnb_t *)ogs_hash_get(self.gnb_addr_hash,
            addr, sizeof(ogs_sockaddr_t));

    return NULL;
}

amf_gnb_t *amf_gnb_find_by_gnb_id(uint32_t gnb_id)
{
    return (amf_gnb_t *)ogs_hash_get(self.gnb_id_hash, &gnb_id, sizeof(gnb_id));
}

int amf_gnb_set_gnb_id(amf_gnb_t *gnb, uint32_t gnb_id)
{
    ogs_assert(gnb);

    gnb->gnb_id = gnb_id;
    ogs_hash_set(self.gnb_id_hash, &gnb->gnb_id, sizeof(gnb->gnb_id), gnb);

    return OGS_OK;
}

int amf_gnb_sock_type(ogs_sock_t *sock)
{
    ogs_socknode_t *snode = NULL;

    ogs_assert(sock);

    ogs_list_for_each(&amf_self()->ngap_list, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    ogs_list_for_each(&amf_self()->ngap_list6, snode)
        if (snode->sock == sock) return SOCK_SEQPACKET;

    return SOCK_STREAM;
}

