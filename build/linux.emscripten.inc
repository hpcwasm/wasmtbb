# Copyright (c) 2005-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

$(info ********************* using linux.emscripten.inc)

CPLUS ?= clang++
CONLY ?= clang
COMPILE_ONLY = -c -MMD
PREPROC_ONLY = -E -x c++
INCLUDE_KEY = -I
DEFINE_KEY = -D
OUTPUT_KEY = -o #
OUTPUTOBJ_KEY = -o #
PIC_KEY = 
WARNING_AS_ERROR_KEY = -Werror
WARNING_KEY = -Wall -Wextra
TEST_WARNING_KEY = -Wshadow -Wcast-qual -Woverloaded-virtual -Wnon-virtual-dtor
WARNING_SUPPRESS = -Wno-parentheses -Wno-non-virtual-dtor -Wno-dangling-else
DYLIB_KEY = -shared
EXPORT_KEY = -Wl,--version-script,
LIBDL = -ldl

LIB_LINK_FLAGS = $(DYLIB_KEY) -Wl,-soname=$(BUILDING_LIBRARY)
LIBS += -lpthread -lrt
LINK_FLAGS = -Wl,-rpath-link=. -rdynamic
C_FLAGS = $(CPLUS_FLAGS)



ifeq ($(cfg), release)
        CPLUS_FLAGS = $(ITT_NOTIFY) -g -O2 -DUSE_PTHREAD
endif
ifeq ($(cfg), debug)
        CPLUS_FLAGS = -DTBB_USE_DEBUG $(ITT_NOTIFY) -g -O0 -DUSE_PTHREAD
endif

ifneq (,$(stdlib))
    CPLUS_FLAGS    += -stdlib=$(stdlib)
    LIB_LINK_FLAGS += -stdlib=$(stdlib)
endif

ifneq (,$(gcc_version))
    # TODO: do not assume that GCC minor and patchlevel versions are always single-digit.
    CPLUS_FLAGS += -DTBB_USE_GLIBCXX_VERSION=$(subst .,0,$(gcc_version))
endif

CPLUS_FLAGS += -DBUILD_WASM 
CPLUS_FLAGS += -D__was32__
CPLUS_FLAGS += -D__emscripten__
CPLUS_FLAGS += -D__linux__=0
# -s WASM_OBJECT_FILES=1
# CPLUS_FLAGS += -s USE_PTHREADS=1 -s USE_ZLIB=1 -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s NO_FILESYSTEM=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=0 -s PTHREAD_POOL_SIZE=2 -s TOTAL_MEMORY=536870912 -s WASM_MEM_MAX=536870912
CPLUS_FLAGS += ${WASM_BUILD_OPTIONS_TBB}
CPLUS_FLAGS += -D__TBB_TASK_GROUP_CONTEXT=1
CPLUS_FLAGS += -DTBB_USE_EXCEPTIONS=0
#CPLUS_FLAGS += -D__TBB_DYNAMIC_LOAD_ENABLED=0
#CPLUS_FLAGS += -D__TBB_SOURCE_DIRECTLY_INCLUDED=0

TBB_ASM.OBJ=
MALLOC_ASM.OBJ=

ifeq (wasm32,$(arch))
    CPLUS_FLAGS += -m32 
    LIB_LINK_FLAGS += -m32
endif

$(info ********************* using CPLUS_FLAGS $(CPLUS_FLAGS))
$(info ********************* using LIB_LINK_FLAGS $(LIB_LINK_FLAGS))

#------------------------------------------------------------------------------
# Setting assembler data.
#------------------------------------------------------------------------------
ASM = as
ifeq (wasm32,$(arch))
    ASM_FLAGS += --32
endif
ifeq ($(cfg),debug)
    ASM_FLAGS += -g
endif

ASSEMBLY_SOURCE=$(arch)-gas
#------------------------------------------------------------------------------
# End of setting assembler data.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Setting tbbmalloc data.
#------------------------------------------------------------------------------

M_CPLUS_FLAGS = $(CPLUS_FLAGS) -fno-rtti -fno-exceptions

#------------------------------------------------------------------------------
# End of setting tbbmalloc data.
#------------------------------------------------------------------------------
