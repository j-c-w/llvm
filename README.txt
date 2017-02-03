LLVM Constraint Solving Demo
============================

The demo GUI as shown at CGO'17 can be accessed by running
"${BUILD_DIR}/bin/constraint-demo.py" after building with cmake.
This requires clang to be integrated in the source tree by
running "git clone http://llvm.org/git/clang" in "tools/clang"
before building.


Low Level Virtual Machine (LLVM)
================================

This directory and its subdirectories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and runtime environments.

LLVM is open source software. You may freely distribute it under the terms of
the license agreement found in LICENSE.txt.

Please see the documentation provided in docs/ for further
assistance with LLVM, and in particular docs/GettingStarted.rst for getting
started with LLVM and docs/README.txt for an overview of LLVM's
documentation setup.

If you are writing a package for LLVM, see docs/Packaging.rst for our
suggestions.
