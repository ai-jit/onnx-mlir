# ONNX MLIR
The Open Neural Network Exchange implementation in MLIR.

[![CircleCI](https://circleci.com/gh/onnx/onnx-mlir/tree/master.svg?style=svg)](https://circleci.com/gh/onnx/onnx-mlir/tree/master)

## Prerequisites

```
gcc >= 6.4
libprotoc >= 3.11.0
cmake >= 3.15.4

protobuffer:
sudo apt-get install  libprotobuf-dev protobuf-compiler
libxml2
llvm:
sudo apt-get install Z3
sudo apt-get install ocaml

```

## Installation

Firstly, install MLIR (as a part of LLVM-Project):

[same-as-file]: <> (utils/install-mlir.sh)
``` bash
git clone https://github.com/llvm/llvm-project.git
# Check out a specific branch that is known to work with ONNX MLIR.
cd llvm-project && git checkout 076475713c236081a3247a53e9dbab9043c3eac2 && cd ..
mkdir llvm-project/build
cd llvm-project/build
cmake -G Ninja ../llvm \
   -DLLVM_ENABLE_PROJECTS=mlir \
   -DLLVM_BUILD_EXAMPLES=ON \
   -DLLVM_TARGETS_TO_BUILD="host" \
   -DCMAKE_BUILD_TYPE=Release \
   -DLLVM_ENABLE_ASSERTIONS=ON \
   -DLLVM_ENABLE_RTTI=ON

cmake --build . --target -- ${MAKEFLAGS}
====================================================================================================
[2275/2331] Building CXX object exampl...ports.dir/LLJITWithLazyReexports.cpp.o
FAILED: examples/LLJITExamples/LLJITWithLazyReexports/CMakeFiles/LLJITWithLazyReexports.dir/LLJITWithLazyReexports.cpp.o 
/usr/bin/c++  -DBUILD_EXAMPLES -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -Iexamples/LLJITExamples/LLJITWithLazyReexports -I/home/leon/Cambricon/code/mlir/llvm-project/llvm/examples/LLJITExamples/LLJITWithLazyReexports -I/home/leon/Cambricon/anaconda3/include/libxml2 -Iinclude -I/home/leon/Cambricon/code/mlir/llvm-project/llvm/include -fPIC -fvisibility-inlines-hidden -Werror=date-time -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wno-missing-field-initializers -pedantic -Wno-long-long -Wimplicit-fallthrough -Wno-maybe-uninitialized -Wno-noexcept-type -Wdelete-non-virtual-dtor -Wno-comment -fdiagnostics-color -ffunction-sections -fdata-sections -O3     -fno-exceptions -UNDEBUG -std=c++14 -MD -MT examples/LLJITExamples/LLJITWithLazyReexports/CMakeFiles/LLJITWithLazyReexports.dir/LLJITWithLazyReexports.cpp.o -MF examples/LLJITExamples/LLJITWithLazyReexports/CMakeFiles/LLJITWithLazyReexports.dir/LLJITWithLazyReexports.cpp.o.d -o examples/LLJITExamples/LLJITWithLazyReexports/CMakeFiles/LLJITWithLazyReexports.dir/LLJITWithLazyReexports.cpp.o -c /home/leon/Cambricon/code/mlir/llvm-project/llvm/examples/LLJITExamples/LLJITWithLazyReexports/LLJITWithLazyReexports.cpp
/home/leon/Cambricon/code/mlir/llvm-project/llvm/examples/LLJITExamples/LLJITWithLazyReexports/LLJITWithLazyReexports.cpp: In lambda function:
/home/leon/Cambricon/code/mlir/llvm-project/llvm/examples/LLJITExamples/LLJITWithLazyReexports/LLJITWithLazyReexports.cpp:110:16: error: could not convert ‘TSM’ from ‘llvm::orc::ThreadSafeModule’ to ‘llvm::Expected<llvm::orc::ThreadSafeModule>’
         return TSM;
                ^~~
[2276/2331] Building CXX object exampl...LJITWithCustomObjectLinkingLayer.cpp.o
ninja: build stopped: subcommand failed.

====================================================================================================
cmake --build . --target check-mlir
```

Two environment variables need to be set:
- LLVM_PROJ_SRC should point to the llvm-project src directory (e.g., llvm-project/).
- LLVM_PROJ_BUILD should point to the llvm-project build directory (e.g., llvm-project/build).

To build ONNX-MLIR, use the following command:

[same-as-file]: <> ({"ref": "utils/install-onnx-mlir.sh", "skip-doc": 2})
```
git clone --recursive git@github.com:onnx/onnx-mlir.git

# Export environment variables pointing to LLVM-Projects.
export LLVM_PROJ_SRC=$(pwd)/llvm-project/
export LLVM_PROJ_BUILD=$(pwd)/llvm-project/build

mkdir onnx-mlir/build && cd onnx-mlir/build
cmake ..
cmake --build . --target onnx-mlir

====================================================================================================
/usr/bin/ld: /home/leon/Cambricon/code/mlir/llvm-project/build/lib/libLLVMSupport.a(Compression.cpp.o): undefined reference to symbol 'crc32'
/usr/lib/x86_64-linux-gnu/libz.so: error adding symbols: DSO missing from command line
collect2: error: ld returned 1 exit status

====================================================================================================
# Run FileCheck tests:
export LIT_OPTS=-v
cmake --build . --target check-mlir-lit
```

After the above commands succeed, an `onnx-mlir` executable should appear in the `bin` directory. 

## Using ONNX MLIR

The usage of `onnx-mlir` is as such:
```
OVERVIEW: ONNX MLIR modular optimizer driver

USAGE: onnx-mlir [options] <input file>

OPTIONS:

Generic Options:

  --help        - Display available options (--help-hidden for more)
  --help-list   - Display list of available options (--help-list-hidden for more)
  --version     - Display the version of this program

ONNX MLIR Options:
These are frontend options.

  Choose target to emit:
      --EmitONNXIR - Ingest ONNX and emit corresponding ONNX dialect.
      --EmitMLIR   - Lower model to MLIR built-in transformation dialect.
      --EmitLLVMIR - Lower model to LLVM IR (LLVM dialect).
      --EmitLLVMBC - Lower model to LLVM IR and emit (to file) LLVM bitcode for model.
```

## Example

For example, to lower an ONNX model (e.g., add.onnx) to ONNX dialect, use the following command:
```
./onnx-mlir --EmitONNXIR add.onnx
```
The output should look like:
```
module {
  func @main_graph(%arg0: tensor<10x10x10xf32>, %arg1: tensor<10x10x10xf32>) -> tensor<10x10x10xf32> {
    %0 = "onnx.Add"(%arg0, %arg1) : (tensor<10x10x10xf32>, tensor<10x10x10xf32>) -> tensor<10x10x10xf32>
    return %0 : tensor<10x10x10xf32>
  }
}
```

## Troubleshooting

If the latest LLVM project fails to work due to the latest changes to the MLIR subproject please consider using a slightly older version of LLVM. One such version, which we use, can be found [here](https://github.com/clang-ykt/llvm-project).
