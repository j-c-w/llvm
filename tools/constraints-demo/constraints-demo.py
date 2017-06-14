#!/usr/bin/python
import gi
import re
import sys
import time
import threading
import subprocess
gi.require_version('Gdk', '3.0')
gi.require_version('Gtk', '3.0')
gi.require_version('GtkSource', '3.0')
from gi.repository import Gdk
from gi.repository import Gtk
from gi.repository import Pango
from gi.repository import GtkSource

BINARY_DIRECTORY   = "/".join(sys.argv[0].split("/")[:-1])+"/"
CLANGPP_EXECUTABLE = BINARY_DIRECTORY+"clang++"
WRAPPER_SCRIPT     = BINARY_DIRECTORY+"constraints-wrapper.py"
SOURCE_FILENAME    = "input.cc"
LLVM_FILENAME      = "input.ll"
TARGET_FILENAME    = "output.ll"

window          = Gtk.Window()
toplevel_box    = Gtk.HBox(homogeneous=True, spacing=5)
left_box        = Gtk.VBox()
menu_box        = Gtk.HBox()
load_button     = Gtk.Button("load")
flatana_button  = Gtk.Button("analyze")
analyze_button  = Gtk.Button("analyze pretty")
hide_button     = Gtk.Button("hide")
source_frame    = Gtk.Frame()
source_box      = Gtk.ScrolledWindow()
sourcecode      = GtkSource.View()
result_box      = Gtk.ScrolledWindow()
result_inner    = Gtk.VBox()
message_window1 = Gtk.Label()
code_window1    = GtkSource.View()
message_window2 = Gtk.Label()
code_window2    = GtkSource.View()
message_window3 = Gtk.Label()
code_window3    = GtkSource.View()
center_bar      = Gtk.Label()

window.      add(toplevel_box)
source_frame.add(source_box)
source_box.  add(sourcecode)
result_box.  add(result_inner)

toplevel_box.pack_start(left_box,        True,  True,  0)
toplevel_box.pack_start(result_box,      True,  True,  0)
left_box.    pack_start(menu_box,        False, True,  0)
menu_box.    pack_start(load_button,     True,  True,  0)
menu_box.    pack_start(flatana_button,  True,  True,  0)
menu_box.    pack_start(analyze_button,  True,  True,  0)
menu_box.    pack_start(hide_button,     True,  True,  0)
left_box.    pack_start(source_frame,    True,  True,  0)
result_inner.pack_start(message_window1, False, False, 5)
result_inner.pack_start(code_window1,    False, False, 5)
result_inner.pack_start(message_window2, False, False, 5)
result_inner.pack_start(code_window2,    False, False, 5)
result_inner.pack_start(message_window3, False, False, 5)
result_inner.pack_start(code_window3,    False, False, 5)

sourcecode.     modify_font(Pango.FontDescription( "Monospace 8"))
load_button.    modify_font(Pango.FontDescription(     "Sans 10"))
flatana_button. modify_font(Pango.FontDescription(     "Sans 10"))
analyze_button. modify_font(Pango.FontDescription(     "Sans 10"))
hide_button.    modify_font(Pango.FontDescription(     "Sans 10"))
code_window1.   modify_font(Pango.FontDescription( "Monospace 8"))
code_window2.   modify_font(Pango.FontDescription( "Monospace 8"))
code_window3.   modify_font(Pango.FontDescription( "Monospace 8"))

result_box.set_policy(Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.ALWAYS)

message_window1.set_xalign(0.0)
message_window1.set_yalign(0.0)
message_window2.set_xalign(0.0)
message_window2.set_yalign(0.0)
message_window3.set_xalign(0.0)
message_window3.set_yalign(0.0)

language_manager = GtkSource.LanguageManager()

sourcecode.  get_buffer().set_language(language_manager.get_language("cpp"))
code_window1.get_buffer().set_language(language_manager.get_language("cpp"))
code_window2.get_buffer().set_language(language_manager.get_language("cpp"))
code_window3.get_buffer().set_language(language_manager.get_language("cpp"))

sourcecode.  get_buffer().set_highlight_syntax(True)
code_window1.get_buffer().set_highlight_syntax(True)
code_window2.get_buffer().set_highlight_syntax(True)
code_window3.get_buffer().set_highlight_syntax(True)

code_window1.set_editable(False)
code_window2.set_editable(False)
code_window3.set_editable(False)

already_running_analysis      = False
already_running_analysis_lock = threading.Lock()

def run_through_clang_format(code):

    process = subprocess.Popen(["clang-format", "-style={BasedOnStyle: llvm,ColumnLimit: 45}"],
                               stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    process.stdin.write(code.encode("utf8"))
    process.stdin.close()

    return "".join([line.decode("utf8") for line in process.stdout])


def map_IR_to_C_line(instruction):
    has_hit = 0
    with open(LLVM_FILENAME, 'r') as temp_file:
        for line in temp_file:
            if has_hit == 0 and line.strip() == instruction:
                has_hit = 1
            if has_hit == 1 and "!" in line:
                instruction = line.strip().split("!")[-1]
                has_hit = 2
            if has_hit == 2 and line[:1] == "!" and line[1:][:len(instruction)] == instruction:
                return int(line.split('(')[1].split(')')[0].split("line: ")[1].split(",")[0])

    return None

def set_output(part1 = "", part2 = "", part3 = "", part4 = "", part5 = "", part6 = "", downscale = 0):

    message_window1.modify_font(Pango.FontDescription("Monospace "+str(10-downscale)))
    message_window1.set_text(part1)
    code_window1.get_buffer().set_text(part2)
    message_window2.modify_font(Pango.FontDescription("Monospace "+str(10-downscale)))
    message_window2.set_text(part3)
    code_window2.get_buffer().set_text(part4)
    message_window3.set_text(part5)
    message_window3.modify_font(Pango.FontDescription("Monospace "+str(10-downscale)))
    code_window3.get_buffer().set_text(part6)

    if part2: code_window1.show()
    else:     code_window1.hide()
    if part4: code_window2.show()
    else:     code_window2.hide()
    if part6: code_window3.show()
    else:     code_window3.hide()

def on_open_clicked(widget):
    dialog = Gtk.FileChooserDialog("Please choose a file", window, Gtk.FileChooserAction.OPEN,
                                   (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL, Gtk.STOCK_OPEN, Gtk.ResponseType.OK))
    response = dialog.run()
    if response == Gtk.ResponseType.OK:
        selected_file = dialog.get_filename()
        with open(selected_file, 'r') as f:
            data = f.read()
            sourcecode.get_buffer().set_text(run_through_clang_format(data))
    dialog.destroy()

def on_analyze_clicked(widget):
    global already_running_analysis, already_running_analysis_lock

    source_code = sourcecode.get_buffer().get_text(sourcecode.get_buffer().get_bounds()[0],
                                                   sourcecode.get_buffer().get_bounds()[1], False)
    with open(SOURCE_FILENAME, 'w') as temp_file:
        temp_file.write(source_code+"\n")
    
    already_running_analysis_lock.acquire()
    if not already_running_analysis:
        set_output("Running clang and SMT solver.")
        threading.Thread(target=wait_thread, args=(source_code,)).start()
    already_running_analysis = True
    already_running_analysis_lock.release()


def on_flatana_clicked(widget):
    global already_running_analysis, already_running_analysis_lock

    source_code = sourcecode.get_buffer().get_text(sourcecode.get_buffer().get_bounds()[0],
                                                   sourcecode.get_buffer().get_bounds()[1], False)
    with open(SOURCE_FILENAME, 'w') as temp_file:
        temp_file.write(source_code+"\n")
    
    already_running_analysis_lock.acquire()
    if not already_running_analysis:
        set_output("Running clang and SMT solver.")
        threading.Thread(target=flatwait_thread, args=(source_code,)).start()
    already_running_analysis = True
    already_running_analysis_lock.release()

def extract_type_from_llmv_value(code):
    possible_tokens = [t for t in code.split(",")[0].split(" = ")[-1].split(" ") if t != ""]

    if   "double*" in possible_tokens: return "double*"
    elif "float*"  in possible_tokens: return "float*"
    elif "i32*"    in possible_tokens: return "int*"
    elif "i64*"    in possible_tokens: return "long*"
    elif "double"  in possible_tokens: return "double"
    elif "float"   in possible_tokens: return "float"
    elif "i32"     in possible_tokens: return "int"
    elif "i64"     in possible_tokens: return "long"
    else:                              return "<type>"

def generate_reduction(solutions):

    iter_type  = ""
    iter_begin = ""
    iter_step  = ""
    iter_end   = ""

    for solution in solutions:
        iter_type = extract_type_from_llmv_value(solution["iterator"])
        if len(solution["iter_begin"].strip().split(' ')) == 2:
            iter_begin = "="+solution["iter_begin"].strip().split(' ')[1]
        if len(solution["iter_step"].strip().split(' ')) == 2:
            iter_step = "+="+solution["iter_step"].strip().split(' ')[1]
        if len(solution["iter_end"].strip().split(' ')) == 2:
            iter_end = "="+solution["iter_end"].strip().split(' ')[1]

    if not iter_end:
        iter_begin = ""
    if iter_step == "+=1" or not iter_step:
        iter_step = "++"

    part1 = ("void reduce_par(out_t* out")
    part2 = (                          ",\n"
            +"                "+iter_type+" begin"+iter_begin+", "+iter_type+" end"+iter_end+")\n"
            +"{\n"
            +" if(/*...*/) { //sequential execution\n"
            +"  for("+iter_type+" i = begin; i < end; i"+iter_step+") {\n"
            +"   op(out")
    part3 = (         "); }\n"
            +" }\n"
            +" else {        //divide and conquer\n"
            +"  "+iter_type+"  mid = begin+(end-begin)/2;\n"
            +"  out_t tmp = init();\n"
            +"  reduce_par(out")
    part4 = (                ",begin,mid);\n"
            +"  reduce_par(tmp")
    part5 = (                ", mid, end);\n"
            +"  merge(out, &tmp);\n"
            +" }\n"
            +"}")
  
    access_base_pointers = []
    accessess            = []

    for solution in solutions:
        for read in solution["read"]:
            try:
                baseptr_idx = access_base_pointers.index(read["base_pointer"])
            except ValueError:
                baseptr_idx = len(access_base_pointers)
                access_base_pointers.append(read["base_pointer"])
                part1 += ", "+extract_type_from_llmv_value(read["base_pointer"])+" in"+str(baseptr_idx)
                part3 += ",in"+str(baseptr_idx)
                part4 += ",in"+str(baseptr_idx)

            try:
                access_idx = accessess.index(read["access_pointer"])
            except ValueError:
                access_idx = len(accessess)
                accessess.append(read["access_pointer"])

                access_pattern = "i"
                if "addend" in read["index_add"][0]:
                    access_pattern = "("+access_pattern+"+"+read["index_add"][0]["addend"].split(" ")[-1]+")"
                if "multiplier" in read["stride_mul"][0]:
                    access_pattern = read["stride_mul"][0]["multiplier"].split(" ")[-1]+"*"+access_pattern
                if "addend" in read["offset_add"]:
                    access_pattern = access_pattern+"+"+read["offset_add"]["addend"].split(" ")[-1]
                part2 += ",in"+str(baseptr_idx)+"["+access_pattern+"]"

    return part1+part2+part3+part4+part5

def beautify_operator(operator):
    declarations = re.findall("  (int|double|float|long) ([a-zA-Z0-9_]+);", operator)

    for datatype, name in declarations:
        operator = re.sub("  "+datatype+" "+name+";\n((?:  [^\n]*;\n)*)  "+name+" =", "\\1  "+datatype+" "+name+" =", operator)

    variables = re.findall("  (int|double|float|long) ([a-zA-Z0-9_]+) ", operator)

    labels_present  = re.findall("([a-zA-Z0-9_]+):\n", operator)

    for label in labels_present:
        operator = re.sub("  goto "+label+";\n"+label, label, operator)

    for label in labels_present:
        operator = re.sub("goto "+label+";([^\n]*)\n"+label, ";\\1\n"+label, operator)

    labels_targeted = re.findall("goto ([a-zA-Z0-9_]+);", operator)

    for label in labels_present:
        if label not in labels_targeted:
            operator = re.sub(label+":\n", "", operator)

    for label in labels_present:
        operator = re.sub("; else goto "+label+";\n((?:  [^\n]*;\n)*)"+label, "{\n\\1  }\n"+label, operator)

    labels_targeted = re.findall("goto ([a-zA-Z0-9_]+);", operator)

    for label in labels_present:
        if label not in labels_targeted:
            operator = re.sub(label+":\n", "", operator)

    operator = re.sub("\[\(long\)\(int\)", "[(int)", operator)
    operator = re.sub("  return ;\n\}", "}", operator)

    return operator

def generate_gemm_call(solutions):
    code = ""
    for solution in solutions:

        lda = solution["loop"][0]["iter_step"].split(" ")[-1]
        ldb = solution["loop"][1]["iter_step"].split(" ")[-1]
        ldc = solution["loop"][2]["iter_step"].split(" ")[-1]

        code += ( "cblas_sgemm(CBLAS_ORDER, CBLAS_TRANSPOSE, CBLAS_TRANSPOSE, "
                + solution["loop"][0]["iter_end"].split(" ")[-1]+", "
                + solution["loop"][1]["iter_end"].split(" ")[-1]+", "
                + solution["loop"][2]["iter_end"].split(" ")[-1]+", "
                + "1.0, A, "+lda+", B, "+ldb+", 0.0, C, "+ldc+");" )

    return code

def wait_thread(source_code):
    global already_running_analysis, already_running_analysis_lock

    process = subprocess.Popen([WRAPPER_SCRIPT, CLANGPP_EXECUTABLE, "-std=c++14",  "-S", "-emit-llvm",
                                "-o", TARGET_FILENAME, "-gline-tables-only", SOURCE_FILENAME], stderr=subprocess.PIPE)
    process.wait()

    stderr_result = process.stderr.read().decode("utf8")

    Gdk.threads_enter()
    if stderr_result:
        set_output(stderr_result)

    else:
        stdout_result = open("replace-report.txt").read().decode("utf8")

        found_suitable_loop = False

        for loop in [p.split("END LOOP\n")[0] for p in stdout_result.split("BEGIN LOOP\n")[1:]]:
            scalars  = [eval(p.split("END SCALAR REDUCTION\n")[0])       for p in loop.split("BEGIN SCALAR REDUCTION\n")[1:]]
            histos   = [eval(p.split("END HISTOGRAM\n")[0])              for p in loop.split("BEGIN HISTOGRAM\n")[1:]]
            matrix   = [eval(p.split("END MATRIX MULTIPLICATIONS\n")[0]) for p in loop.split("BEGIN MATRIX MULTIPLICATIONS\n")[1:]]
            stencils = [eval(p.split("END STENCIL\n")[0])                for p in loop.split("BEGIN STENCIL\n")[1:]]

            found_suitable_loop = True

            if len(stencils):

                line_begin = map_IR_to_C_line(stencils[0]["begin"])
                line_end   = map_IR_to_C_line(stencils[0]["successor"]) - 1

                if line_begin >= len(source_code.split("\n")):
                    line_begin = None
                if line_end >= len(source_code.split("\n")):
                    line_end = None

                set_output("Found stencil kernel in lines "+str(line_begin)+" - "+str(line_end)+":")

            elif len(matrix):

                line_begin = map_IR_to_C_line(matrix[0]["begin"])
                line_end   = map_IR_to_C_line(matrix[0]["successor"]) - 1

                if line_begin >= len(source_code.split("\n")):
                    line_begin = None
                if line_end >= len(source_code.split("\n")):
                    line_end = None

                set_output("Found matrix multiplication in lines "+str(line_begin)+" - "+str(line_end)+":",
                           run_through_clang_format(generate_gemm_call(matrix)))

            elif len(scalars+histos):

                operator       = loop.split("BEGIN OPERATOR\n")[1]
                reduction_type = operator.split("op(")[1].split("* acc")[0];
                operator       = operator.replace(reduction_type, "out_t")

                line_begin = map_IR_to_C_line((scalars+histos)[0]["begin"])
                line_end   = map_IR_to_C_line((scalars+histos)[0]["successor"]) - 1

                if line_begin >= len(source_code.split("\n")):
                    line_begin = None
                if line_end >= len(source_code.split("\n")):
                    line_end = None

                while line_end > 0 and source_code.split("\n")[line_end-1] == "":
                    line_end = line_end - 1

                set_output("Found reduction in lines "+str(line_begin)+" - "+str(line_end)+":",
                           "typedef "+run_through_clang_format(reduction_type)+" out_t;",
                           "Assuming it is associative,\nthe reduction can be rewritten:",
                           run_through_clang_format(generate_reduction(scalars+histos))+" ",
                           "This uses the reduction operator:",
                           run_through_clang_format(beautify_operator(operator))+" ")

            else:
            
                found_suitable_loop = False
                continue
            break

        if not found_suitable_loop:
            set_output("No idioms were identified.")

    Gdk.threads_leave()
    already_running_analysis_lock.acquire()
    already_running_analysis = False
    already_running_analysis_lock.release()


def flatwait_thread(source_code):
    global already_running_analysis, already_running_analysis_lock

    process = subprocess.Popen([WRAPPER_SCRIPT, CLANGPP_EXECUTABLE, "-std=c++14",  "-S", "-emit-llvm",
                                "-o", TARGET_FILENAME, "-gline-tables-only", SOURCE_FILENAME], stderr=subprocess.PIPE)
    process.wait()

    stderr_result = process.stderr.read().decode("utf8")

    Gdk.threads_enter()
    if stderr_result:
        set_output(stderr_result)

    else:
        stdout_result = open("replace-report.txt").read().decode("utf8")

        if stdout_result:
            set_output(stdout_result, "", "", "", "", "", 5)
        else:
            set_output("No idioms were identified.")

    Gdk.threads_leave()
    already_running_analysis_lock.acquire()
    already_running_analysis = False
    already_running_analysis_lock.release()

def on_hide_clicked(widget):
    set_output()

window.        connect("delete-event", Gtk.main_quit)
load_button.   connect("clicked",      on_open_clicked)
analyze_button.connect("clicked",      on_analyze_clicked)
flatana_button.connect("clicked",      on_flatana_clicked)
hide_button.   connect("clicked",      on_hide_clicked)

Gdk.threads_init()
window.show_all()
set_output()

Gtk.main()
