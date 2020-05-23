#ifndef TEST_5GC_H
#define TEST_5GC_H

#include "test-common.h"

#include "ogs-ngap.h"
#include "ogs-nas-5gs.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_5gc_init(void);
#define test_5gc_run test_app_run
#define test_5gc_final ogs_mongoc_final

#ifdef __cplusplus
}
#endif

#endif /* TEST_5GC_H */
