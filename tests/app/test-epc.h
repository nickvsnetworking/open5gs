#ifndef TEST_EPC_H
#define TEST_EPC_H

#include "ogs-sctp.h"
#include "ogs-dbi.h"
#include "mme/mme-context.h"

#include "test-common.h"
#include "test-packet.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_epc_init(void);
#define test_epc_run test_app_run
#define test_epc_final ogs_mongoc_final

#ifdef __cplusplus
}
#endif

#endif /* TEST_EPC_H */
