#!/usr/bin/python3
import sys
import subprocess

# This script can simulate the behaviour of both clang and clang++, the first command line argument should specify.
if len(sys.argv) >= 2 and sys.argv[1].split("/")[-1] in ["clang", "clang++"]:
    BINARY_CLANG = sys.argv[1]
    BINARY_OPT   = "/".join(sys.argv[1].split("/")[:-1]+["opt"])
else:
    raise BaseException("The first command line argument has to be a path to either a clang or clang++ executable.")

# In this step we extract all the command line options relevant for compilation without optimisation.
first_phase_arguments = []
source_file_indizes   = set()
extract_next_argument = False
ignore_next_argument  = False
for n,arg in enumerate(sys.argv[2:]):
    if extract_next_argument:
        extract_next_argument = False
        first_phase_arguments += [arg]
    elif ignore_next_argument:
        ignore_next_argument = False
    elif arg in {"-D", "-I"}:
        first_phase_arguments += [arg]
        extract_next_argument = True
    elif arg[:2] in {'-g', '-D', '-W', '-I'}:
        first_phase_arguments += [arg]
    elif arg[:4] == '-std':
        first_phase_arguments += [arg]
    elif arg[:1] != "-" and arg.split('.')[-1] in {"c", "h", "cc", "hh", "cpp", "hpp"}:
        source_file_indizes |= {n+2}

# This generates the compiler passes necessary to effectively run the idiom detection algorithm.
process1 = subprocess.Popen([BINARY_OPT, "-O2", "-disable-output", "-debug-pass=Arguments"],
                            stdin=subprocess.PIPE, stderr=subprocess.PIPE)

# No input, only care about debug information of passes.
process1.stdin.close()

# These optimisation mess with the detection algorithm.
OPTIMIZATION_BLACKLIST = {'instcombine', 'loop-rotate', 'loop-unroll', 'slp-vectorizer'}

# Filter the O2 optimisation passes to only leave the desired ones.
OPTIMIZATION_PASSES = [name for line in process1.stderr
                                     if line.decode("utf8")[:17] == "Pass Arguments:  "
                            for name in line.decode("utf8")[17:].rstrip().split(' ')
                                     if name[:1] == "-" and name[1:] not in OPTIMIZATION_BLACKLIST]

# These passes are run before the detection pass as a way of normalising the LLVM IR code.
# Clearly not all of tehm are necessary, but who cares.
NORMALIZATION_PASSES  = ( OPTIMIZATION_PASSES + ["-research-flatten"]
                        + OPTIMIZATION_PASSES + ["-research-preprocessor"]
                        + OPTIMIZATION_PASSES + ["-simplifycfg"] )

# Run the tool for each C/C++ input file.
for idx in source_file_indizes:

    # This format for the modified_filename guarantees that object file outputs have the right name.
    original_filename = sys.argv[idx]
    modified_filename = ".".join(original_filename.split('.')[:-1])+".ll"
    sys.argv[idx]     = modified_filename

    # Compile to LLVM assembly without optimisations, write to stdout.
    process2 = subprocess.Popen([BINARY_CLANG, "-S", "-emit-llvm", "-o", "-", original_filename]
                                + first_phase_arguments,
                                stdout = subprocess.PIPE)

    # Strip away the stupid noinline attributes.
    process3 = subprocess.Popen(["python", '/'.join((sys.argv[0].split('/')[:-1]))+"/remove_function_attr.py"],
                                 stdin = process2.stdout,
                                 stdout = subprocess.PIPE)

    # Optimize LLVM assembly output by clang using modified version of opt.
    process4 = subprocess.Popen([BINARY_OPT, "-S", "-o", modified_filename, "-", "-always-inline"]
                                + NORMALIZATION_PASSES  + ["-research-replacer"],
                                stdin = process3.stdout)

    process4.wait()

# Run the original command line but with the C/C++ input files replaced with llvm files.
subprocess.call(sys.argv[1:])
