/*
    Copyright (c) 2005-2019 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#if !defined(__TBB_machine_H) || defined(__TBB_machine_linux_ia32_H)
#error Do not #include this internal file directly; use public TBB headers instead.
#endif

#define __TBB_machine_linux_ia32_H

#include <stdint.h>
#include <emscripten.h>
#include <emscripten/threading.h>

#define __TBB_WORDSIZE 4
#define __TBB_ENDIANNESS __TBB_ENDIAN_LITTLE

#define __TBB_compiler_fence() emscripten_atomic_fence()
#define __TBB_control_consistency_helper() __TBB_compiler_fence()
#define __TBB_acquire_consistency_helper() __TBB_compiler_fence()
#define __TBB_release_consistency_helper() __TBB_compiler_fence()
#define __TBB_full_memory_fence()          __TBB_compiler_fence()

#if __TBB_ICC_ASM_VOLATILE_BROKEN
#define __TBB_VOLATILE
#else
#define __TBB_VOLATILE volatile
#endif

#define __TBB_MACHINE_DEFINE_ATOMICS(S,B,T)                                                         \
inline T __TBB_machine_cmpswp##S( volatile void *ptr, T value, T comparand ) {                    \
    return emscripten_atomic_cas_u##B(const_cast<void *>(ptr),comparand,value);     \
}                                                                                                 \
inline T __TBB_machine_fetchadd##S( volatile void *ptr, T value ) {                               \
    return emscripten_atomic_add_u##B(const_cast<void *>(ptr),value);                       \
}                                                                                                    \
inline T __TBB_machine_fetchstore##S( volatile void *ptr, T value ) {                               \
    return emscripten_atomic_exchange_u##B(const_cast<void *>(ptr),value);                       \
}

__TBB_MACHINE_DEFINE_ATOMICS(1,8,int8_t)
__TBB_MACHINE_DEFINE_ATOMICS(2,16,int16_t)
__TBB_MACHINE_DEFINE_ATOMICS(4,32,int32_t)
__TBB_MACHINE_DEFINE_ATOMICS(8,64,int64_t)


static inline void __TBB_machine_or( volatile void *ptr, uintptr_t addend ) {
    // __sync_fetch_and_or(reinterpret_cast<volatile uintptr_t *>(ptr),addend);
    emscripten_atomic_or_u32(const_cast<void *>(ptr),addend);
}

static inline void __TBB_machine_and( volatile void *ptr, uintptr_t addend ) {
    // __sync_fetch_and_and(reinterpret_cast<volatile uintptr_t *>(ptr),addend);
    emscripten_atomic_and_u32(const_cast<void  *>(ptr),addend);
}

#ifdef __TBBMALLOC_BUILD
#error "__TBBMALLOC_BUILD"
#endif

// Machine specific atomic operations
#define __TBB_AtomicOR(P,V) __TBB_machine_or(P,V)
#define __TBB_AtomicAND(P,V) __TBB_machine_and(P,V)

#define __TBB_USE_GENERIC_DWORD_LOAD_STORE                  1
#define __TBB_USE_GENERIC_DWORD_FETCH_ADD                   1
#define __TBB_USE_GENERIC_DWORD_FETCH_STORE                 1
#define __TBB_USE_FETCHSTORE_AS_FULL_FENCED_STORE           1
#define __TBB_USE_GENERIC_HALF_FENCED_LOAD_STORE            1
#define __TBB_USE_GENERIC_RELAXED_LOAD_STORE                1
#define __TBB_USE_GENERIC_SEQUENTIAL_CONSISTENCY_LOAD_STORE 1

