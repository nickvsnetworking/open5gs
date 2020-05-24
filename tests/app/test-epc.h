#ifndef TEST_EPC_H
#define TEST_EPC_H

#include "test-packet.h"

#include "mme/mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_epc_init(void);
#define test_epc_final ogs_mongoc_final

#ifdef __cplusplus
}
#endif

#endif /* TEST_EPC_H */
