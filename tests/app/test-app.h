#ifndef TEST_APP_H
#define TEST_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-app.h"
#include "ogs-diameter-common.h"
#include "core/abts.h"

void test_app_run(int argc, const char *const argv[],
        const char *name, void (*init)(const char * const argv[]));
void test_child_terminate(void);
ogs_thread_t *test_child_create(const char *name, const char *const argv[]);

#ifdef __cplusplus
}
#endif

#endif /* TEST_APP_H */
