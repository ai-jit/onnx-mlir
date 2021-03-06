//===-------------- ONNXMLIROpt.cpp - Optimization Driver -----------------===//
//
// Copyright 2019-2020 The IBM Research Authors.
//
// =============================================================================
//
//
//
//===----------------------------------------------------------------------===//

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/ToolOutputFile.h>
#include <mlir/InitAllDialects.h>
#include <mlir/Pass/Pass.h>
#include <mlir/Pass/PassManager.h>
#include <mlir/Support/FileUtilities.h>
#include <mlir/Support/MlirOptMain.h>

#include "src/Dialect/Krnl/KrnlOps.hpp"
#include "src/Dialect/ONNX/ONNXOps.hpp"
#include "src/Pass/Passes.hpp"

using namespace onnx_mlir;

static llvm::cl::opt<std::string> input_filename(llvm::cl::Positional,
                                                 llvm::cl::desc("<input file>"),
                                                 llvm::cl::init("-"));

static llvm::cl::opt<std::string>
    output_filename("o", llvm::cl::desc("Output filename"),
                    llvm::cl::value_desc("filename"), llvm::cl::init("-"));

static llvm::cl::opt<bool> split_input_file(
    "split-input-file",
    llvm::cl::desc("Split the input file into pieces and process each "
                   "chunk independently"),
    llvm::cl::init(false));

static llvm::cl::opt<bool> verify_diagnostics(
    "verify-diagnostics",
    llvm::cl::desc("Check that emitted diagnostics match "
                   "expected-* lines on the corresponding line"),
    llvm::cl::init(false));

static llvm::cl::opt<bool> verify_passes(
    "verify-each",
    llvm::cl::desc("Run the verifier after each transformation pass"),
    llvm::cl::init(true));

int main(int argc, char **argv) {
  mlir::registerDialect<mlir::AffineOpsDialect>();
  mlir::registerDialect<mlir::LLVM::LLVMDialect>();
  mlir::registerDialect<mlir::loop::LoopOpsDialect>();
  mlir::registerDialect<mlir::StandardOpsDialect>();
  llvm::InitLLVM y(argc, argv);

  mlir::registerDialect<mlir::ONNXOpsDialect>();
  mlir::registerDialect<mlir::KrnlOpsDialect>();

  // Register any pass manager command line options.
  mlir::registerPassManagerCLOptions();
  mlir::PassPipelineCLParser passPipeline("", "Compiler passes to run");
  llvm::cl::ParseCommandLineOptions(argc, argv,
                                    "ONNX MLIR modular optimizer driver\n");

  // Set up the input file.
  std::string error_message;
  auto file = mlir::openInputFile(input_filename, &error_message);
  assert(file);

  auto output = mlir::openOutputFile(output_filename, &error_message);
  assert(output);

  return failed(mlir::MlirOptMain(output->os(), std::move(file), passPipeline,
                                  split_input_file, verify_diagnostics,
                                  verify_passes));
}
