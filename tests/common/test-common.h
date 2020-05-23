#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-app.h"
#include "ogs-sctp.h"
#include "ogs-diameter-common.h"

#define OGS_TEST_INSIDE

#include "common/application.h"
#include "common/sctp.h"
#include "common/gtpu.h"

#undef OGS_TEST_INSIDE

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_H */
