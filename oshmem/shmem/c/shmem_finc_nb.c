/*
 * Copyright (c) 2021      NVIDIA Corporation.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
#include "oshmem_config.h"

#include "oshmem/constants.h"
#include "oshmem/include/shmem.h"
#include "oshmem/include/shmemx.h"

#include "oshmem/runtime/runtime.h"

#include "oshmem/op/op.h"
#include "oshmem/mca/atomic/atomic.h"

/*
 * These routines perform a nonblocking fetch-and-increment operation.
 * The fetch and increment routines retrieve the value at address target on PE pe, and update
 * target with the result of incrementing the retrieved value by one. The operation must be
 * completed without the possibility of another process updating target between the time of
 * the fetch and the update.
 */
#define DO_SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(ctx, type_name, type, fetch, target, pe, out_value) do { \
        int rc = OSHMEM_SUCCESS;                                    \
        size_t size;                                                \
        type value = 1;                                             \
                                                                    \
        RUNTIME_CHECK_INIT();                                       \
        RUNTIME_CHECK_PE(pe);                                       \
        RUNTIME_CHECK_ADDR(target);                                 \
                                                                    \
        size = sizeof(out_value);                                   \
        rc = MCA_ATOMIC_CALL(fadd_nb(                               \
            ctx,                                                    \
            fetch,                                                  \
            (void*)target,                                          \
            (void*)&out_value,                                      \
            value,                                                  \
            size,                                                   \
            pe));                                                   \
        RUNTIME_CHECK_RC(rc);                                       \
    } while (0)

#define SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(type_name, type, prefix)            \
    void prefix##_ctx##type_name##_atomic_fetch_inc_nbi(shmem_ctx_t ctx, type *fetch, type *target, int pe) \
    {                                                                           \
        type out_value;                                                         \
        DO_SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(ctx, type_name, type, fetch, target, \
                                  pe, out_value);                               \
        return ;                                                                \
    }

#define SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(type_name, type, prefix)                \
    void prefix##type_name##_atomic_fetch_inc_nbi(type *fetch, type *target, int pe) \
    {                                                                           \
        type out_value;                                                         \
        DO_SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(oshmem_ctx_default, type_name,       \
                                  type, fetch, target, pe, out_value);          \
        return ;                                                                \
    }

#if OSHMEM_PROFILING
#include "oshmem/include/pshmem.h"

/* Nonblocking Atomic Fetch and Increment */
#pragma weak shmem_ctx_int_atomic_fetch_inc_nbi               = pshmem_ctx_int_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_long_atomic_fetch_inc_nbi              = pshmem_ctx_long_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_longlong_atomic_fetch_inc_nbi          = pshmem_ctx_longlong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_inc_nbi              = pshmem_ctx_uint_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_inc_nbi             = pshmem_ctx_ulong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_inc_nbi         = pshmem_ctx_ulonglong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_inc_nbi             = pshmem_ctx_int32_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_inc_nbi             = pshmem_ctx_int64_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_inc_nbi            = pshmem_ctx_uint32_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_inc_nbi            = pshmem_ctx_uint64_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_size_atomic_fetch_inc_nbi              = pshmem_ctx_size_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_inc_nbi           = pshmem_ctx_ptrdiff_atomic_fetch_inc_nbi

#pragma weak shmem_int_atomic_fetch_inc_nbi        			  = pshmem_int_atomic_fetch_inc_nbi
#pragma weak shmem_long_atomic_fetch_inc_nbi       			  = pshmem_long_atomic_fetch_inc_nbi
#pragma weak shmem_longlong_atomic_fetch_inc_nbi   			  = pshmem_longlong_atomic_fetch_inc_nbi
#pragma weak shmem_uint_atomic_fetch_inc_nbi       			  = pshmem_uint_atomic_fetch_inc_nbi
#pragma weak shmem_ulong_atomic_fetch_inc_nbi      			  = pshmem_ulong_atomic_fetch_inc_nbi
#pragma weak shmem_ulonglong_atomic_fetch_inc_nbi  			  = pshmem_ulonglong_atomic_fetch_inc_nbi
#pragma weak shmem_int32_atomic_fetch_inc_nbi      			  = pshmem_int32_atomic_fetch_inc_nbi
#pragma weak shmem_int64_atomic_fetch_inc_nbi      			  = pshmem_int64_atomic_fetch_inc_nbi
#pragma weak shmem_uint32_atomic_fetch_inc_nbi     			  = pshmem_uint32_atomic_fetch_inc_nbi
#pragma weak shmem_uint64_atomic_fetch_inc_nbi     			  = pshmem_uint64_atomic_fetch_inc_nbi
#pragma weak shmem_size_atomic_fetch_inc_nbi       			  = pshmem_size_atomic_fetch_inc_nbi
#pragma weak shmem_ptrdiff_atomic_fetch_inc_nbi    			  = pshmem_ptrdiff_atomic_fetch_inc_nbi

#include "oshmem/shmem/c/profile/defines.h"
#endif

SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_int, int, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_long, long, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_longlong, long long, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_uint, unsigned int, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_ulong, unsigned long, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_ulonglong, unsigned long long, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_int32, int32_t, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_int64, int64_t, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_uint32, uint32_t, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_uint64, uint64_t, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_size, size_t, shmem)
SHMEM_CTX_TYPE_ATOMIC_FETCH_INC_NBI(_ptrdiff, ptrdiff_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_int, int, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_long, long, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_longlong, long long, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_uint, unsigned int, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_ulong, unsigned long, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_ulonglong, unsigned long long, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_int32, int32_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_int64, int64_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_uint32, uint32_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_uint64, uint64_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_size, size_t, shmem)
SHMEM_TYPE_ATOMIC_FETCH_INC_NBI(_ptrdiff, ptrdiff_t, shmem)
