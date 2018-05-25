# NOTE: Assertions have been autogenerated by utils/update_mca_test_checks.py
# RUN: llvm-mca -mtriple=x86_64-unknown-unknown -mcpu=znver1 -instruction-tables < %s | FileCheck %s

extrq       %xmm0, %xmm2
extrq       $22, $2, %xmm2

insertq     %xmm0, %xmm2
insertq     $22, $22, %xmm0, %xmm2

movntsd     %xmm0, (%rax)
movntss     %xmm0, (%rax)

# CHECK:      Instruction Info:
# CHECK-NEXT: [1]: #uOps
# CHECK-NEXT: [2]: Latency
# CHECK-NEXT: [3]: RThroughput
# CHECK-NEXT: [4]: MayLoad
# CHECK-NEXT: [5]: MayStore
# CHECK-NEXT: [6]: HasSideEffects

# CHECK:      [1]    [2]    [3]    [4]    [5]    [6]    Instructions:
# CHECK-NEXT:  1      2     1.00                        extrq	%xmm0, %xmm2
# CHECK-NEXT:  1      2     1.00                        extrq	$22, $2, %xmm2
# CHECK-NEXT:  1      4     1.00                        insertq	%xmm0, %xmm2
# CHECK-NEXT:  1      4     1.00                        insertq	$22, $22, %xmm0, %xmm2
# CHECK-NEXT:  1      8     1.00           *            movntsd	%xmm0, (%rax)
# CHECK-NEXT:  1      8     1.00           *            movntss	%xmm0, (%rax)

# CHECK:      Resources:
# CHECK-NEXT: [0]   - ZnAGU0
# CHECK-NEXT: [1]   - ZnAGU1
# CHECK-NEXT: [2]   - ZnALU0
# CHECK-NEXT: [3]   - ZnALU1
# CHECK-NEXT: [4]   - ZnALU2
# CHECK-NEXT: [5]   - ZnALU3
# CHECK-NEXT: [6]   - ZnDivider
# CHECK-NEXT: [7]   - ZnFPU0
# CHECK-NEXT: [8]   - ZnFPU1
# CHECK-NEXT: [9]   - ZnFPU2
# CHECK-NEXT: [10]  - ZnFPU3
# CHECK-NEXT: [11]  - ZnMultiplier

# CHECK:      Resource pressure per iteration:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]
# CHECK-NEXT: 1.00   1.00    -      -      -      -      -     1.00   3.00   5.00   1.00    -

# CHECK:      Resource pressure by instruction:
# CHECK-NEXT: [0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   Instructions:
# CHECK-NEXT:  -      -      -      -      -      -      -      -     0.50   1.50    -      -     extrq	%xmm0, %xmm2
# CHECK-NEXT:  -      -      -      -      -      -      -      -     0.50   1.50    -      -     extrq	$22, $2, %xmm2
# CHECK-NEXT:  -      -      -      -      -      -      -     0.50   1.00    -     0.50    -     insertq	%xmm0, %xmm2
# CHECK-NEXT:  -      -      -      -      -      -      -     0.50   1.00    -     0.50    -     insertq	$22, $22, %xmm0, %xmm2
# CHECK-NEXT: 0.50   0.50    -      -      -      -      -      -      -     1.00    -      -     movntsd	%xmm0, (%rax)
# CHECK-NEXT: 0.50   0.50    -      -      -      -      -      -      -     1.00    -      -     movntss	%xmm0, (%rax)

