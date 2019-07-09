git clone -b '2019_U6' --single-branch --depth 1 https://github.com/intel/tbb.git

export HPCWASM_BASE_DIR=/home/manstetten/github_hpcwasm/boilerplate
# export HPCWASM_BASE_DIR=/home/manstetten/ProgramsDev/hpcwasm
export HPCWASM_BASE_DIR_EMSDK=$HPCWASM_BASE_DIR/emsdk
export EM_CONFIG=$HPCWASM_BASE_DIR_EMSDK/.emscripten
source $HPCWASM_BASE_DIR_EMSDK/emsdk_env.sh


EMCC_DEBUG=0 emmake make  extra_inc=big_iron.inc VERBOSE=1  tbb

emmake make tbb

# dump all preprocessor defines
em++ -dM -E -x c++ /dev/null | grep 32


export HPCWASM_BASE_DIR_EMSDK=/home/manstetten/Programs/emsdknew
export EM_CONFIG=/home/manstetten/.emscripten
source $HPCWASM_BASE_DIR_EMSDK/emsdk_env.sh