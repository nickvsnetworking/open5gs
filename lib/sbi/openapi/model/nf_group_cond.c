
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_group_cond.h"

char *OpenAPI_nf_typenf_group_cond_ToString(OpenAPI_nf_group_cond_nf_type_e nf_type)
{
    const char *nf_typeArray[] =  { "NULL", "UDM", "AUSF", "UDR", "PCF" };
    size_t sizeofArray = sizeof(nf_typeArray) / sizeof(nf_typeArray[0]);
    if (nf_type < sizeofArray)
        return (char *)nf_typeArray[nf_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nf_group_cond_nf_type_e OpenAPI_nf_typenf_group_cond_FromString(char* nf_type)
{
    int stringToReturn = 0;
    const char *nf_typeArray[] =  { "NULL", "UDM", "AUSF", "UDR", "PCF" };
    size_t sizeofArray = sizeof(nf_typeArray) / sizeof(nf_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nf_type, nf_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_nf_group_cond_t *OpenAPI_nf_group_cond_create(
    OpenAPI_nf_group_cond_nf_type_e nf_type,
    char *nf_group_id
    )
{
    OpenAPI_nf_group_cond_t *nf_group_cond_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_group_cond_t));
    if (!nf_group_cond_local_var) {
        return NULL;
    }
    nf_group_cond_local_var->nf_type = nf_type;
    nf_group_cond_local_var->nf_group_id = nf_group_id;

    return nf_group_cond_local_var;
}

void OpenAPI_nf_group_cond_free(OpenAPI_nf_group_cond_t *nf_group_cond)
{
    if (NULL == nf_group_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_group_cond->nf_group_id);
    ogs_free(nf_group_cond);
}

cJSON *OpenAPI_nf_group_cond_convertToJSON(OpenAPI_nf_group_cond_t *nf_group_cond)
{
    cJSON *item = NULL;

    if (nf_group_cond == NULL) {
        ogs_error("OpenAPI_nf_group_cond_convertToJSON() failed [NfGroupCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_group_cond->nf_type) {
        ogs_error("OpenAPI_nf_group_cond_convertToJSON() failed [nf_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_typenf_group_cond_ToString(nf_group_cond->nf_type)) == NULL) {
        ogs_error("OpenAPI_nf_group_cond_convertToJSON() failed [nf_type]");
        goto end;
    }

    if (!nf_group_cond->nf_group_id) {
        ogs_error("OpenAPI_nf_group_cond_convertToJSON() failed [nf_group_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfGroupId", nf_group_cond->nf_group_id) == NULL) {
        ogs_error("OpenAPI_nf_group_cond_convertToJSON() failed [nf_group_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_nf_group_cond_t *OpenAPI_nf_group_cond_parseFromJSON(cJSON *nf_group_condJSON)
{
    OpenAPI_nf_group_cond_t *nf_group_cond_local_var = NULL;
    cJSON *nf_type = cJSON_GetObjectItemCaseSensitive(nf_group_condJSON, "nfType");
    if (!nf_type) {
        ogs_error("OpenAPI_nf_group_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }

    OpenAPI_nf_group_cond_nf_type_e nf_typeVariable;

    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_nf_group_cond_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_typenf_group_cond_FromString(nf_type->valuestring);

    cJSON *nf_group_id = cJSON_GetObjectItemCaseSensitive(nf_group_condJSON, "nfGroupId");
    if (!nf_group_id) {
        ogs_error("OpenAPI_nf_group_cond_parseFromJSON() failed [nf_group_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_group_id)) {
        ogs_error("OpenAPI_nf_group_cond_parseFromJSON() failed [nf_group_id]");
        goto end;
    }

    nf_group_cond_local_var = OpenAPI_nf_group_cond_create (
        nf_typeVariable,
        ogs_strdup(nf_group_id->valuestring)
        );

    return nf_group_cond_local_var;
end:
    return NULL;
}

