
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atom.h"

OpenAPI_atom_t *OpenAPI_atom_create(
    char *attr,
    char *value,
    int negative
    )
{
    OpenAPI_atom_t *atom_local_var = OpenAPI_malloc(sizeof(OpenAPI_atom_t));
    if (!atom_local_var) {
        return NULL;
    }
    atom_local_var->attr = attr;
    atom_local_var->value = value;
    atom_local_var->negative = negative;

    return atom_local_var;
}

void OpenAPI_atom_free(OpenAPI_atom_t *atom)
{
    if (NULL == atom) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(atom->attr);
    ogs_free(atom->value);
    ogs_free(atom);
}

cJSON *OpenAPI_atom_convertToJSON(OpenAPI_atom_t *atom)
{
    cJSON *item = NULL;

    if (atom == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [Atom]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!atom->attr) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [attr]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "attr", atom->attr) == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [attr]");
        goto end;
    }

    if (!atom->value) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [value]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "value", atom->value) == NULL) {
        ogs_error("OpenAPI_atom_convertToJSON() failed [value]");
        goto end;
    }

    if (atom->negative) {
        if (cJSON_AddBoolToObject(item, "negative", atom->negative) == NULL) {
            ogs_error("OpenAPI_atom_convertToJSON() failed [negative]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_atom_t *OpenAPI_atom_parseFromJSON(cJSON *atomJSON)
{
    OpenAPI_atom_t *atom_local_var = NULL;
    cJSON *attr = cJSON_GetObjectItemCaseSensitive(atomJSON, "attr");
    if (!attr) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [attr]");
        goto end;
    }


    if (!cJSON_IsString(attr)) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [attr]");
        goto end;
    }

    cJSON *value = cJSON_GetObjectItemCaseSensitive(atomJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [value]");
        goto end;
    }


    if (!cJSON_IsString(value)) {
        ogs_error("OpenAPI_atom_parseFromJSON() failed [value]");
        goto end;
    }

    cJSON *negative = cJSON_GetObjectItemCaseSensitive(atomJSON, "negative");

    if (negative) {
        if (!cJSON_IsBool(negative)) {
            ogs_error("OpenAPI_atom_parseFromJSON() failed [negative]");
            goto end;
        }
    }

    atom_local_var = OpenAPI_atom_create (
        ogs_strdup(attr->valuestring),
        ogs_strdup(value->valuestring),
        negative ? negative->valueint : 0
        );

    return atom_local_var;
end:
    return NULL;
}

