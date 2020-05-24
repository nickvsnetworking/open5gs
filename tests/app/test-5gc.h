#ifndef TEST_5GC_H
#define TEST_5GC_H

#include "test-common.h"
#include "test-ngap.h"

#include "amf/context.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_5gc_init(void);
#define test_5gc_final ogs_mongoc_final

#ifdef __cplusplus
}
#endif

#endif /* TEST_5GC_H */
