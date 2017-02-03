#!/usr/bin/python
import gi
gi.require_version('Gtk', '3.0')
gi.require_version('GtkSource', '3.0')
from gi.repository import Gtk
from gi.repository import GtkSource
from gi.repository import Pango
from gi.repository import Gdk

import clang
from clang import cindex

import sys
import subprocess
from threading import Thread

window = Gtk.Window()

toplevel_box     = Gtk.HBox(homogeneous=True, spacing=10)
left_box         = Gtk.VBox()
menu_box         = Gtk.HBox()
load_button      = Gtk.Button("load")
analyze_button   = Gtk.Button("analyze")
hide_button      = Gtk.Button("hide")
source_frame     = Gtk.Frame()
source_box       = Gtk.ScrolledWindow()
sourcecode       = GtkSource.View()
result_box       = Gtk.ScrolledWindow()
result_inner_box = Gtk.VBox()
message_window1  = Gtk.Label()
code_window1     = GtkSource.View()
message_window2  = Gtk.Label()
code_window2     = GtkSource.View()
center_bar       = Gtk.Label()

#sourcecode.set_show_line_numbers(True)
sourcecode.modify_font(Pango.FontDescription("Monospace 10"))
load_button.modify_font(Pango.FontDescription("Sans 10"))
analyze_button.modify_font(Pango.FontDescription("Sans 10"))
hide_button.modify_font(Pango.FontDescription("Sans 10"))
language_manager = GtkSource.LanguageManager()
sourcecode.get_buffer().set_language(language_manager.get_language("cpp"))
sourcecode.get_buffer().set_highlight_syntax(True)
message_window1.set_xalign(0.0)
message_window1.set_yalign(0.0)
message_window2.set_xalign(0.0)
message_window2.set_yalign(0.0)
result_box.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.ALWAYS)
code_window1.get_buffer().set_language(language_manager.get_language("llvm"))
code_window1.get_buffer().set_highlight_syntax(True)
code_window2.get_buffer().set_language(language_manager.get_language("llvm"))
code_window2.get_buffer().set_highlight_syntax(True)
message_window1.modify_font(Pango.FontDescription("Monospace 10"))
code_window1.modify_font(Pango.FontDescription("Monospace 8"))
message_window2.modify_font(Pango.FontDescription("Monospace 10"))
code_window2.modify_font(Pango.FontDescription("Monospace 8"))
code_window1.set_editable(False)
code_window2.set_editable(False)

window.add(toplevel_box)
toplevel_box.pack_start(left_box, True, True, 0)
toplevel_box.pack_start(result_box, True, True, 0)
left_box.pack_start(menu_box, False, True, 0)
menu_box.pack_start(load_button, True, True, 0)
menu_box.pack_start(analyze_button, True, True, 0)
menu_box.pack_start(hide_button, True, True, 0)
left_box.pack_start(source_frame, True, True, 0)
source_frame.add(source_box)
source_box.add(sourcecode)
result_box.add(result_inner_box)
result_inner_box.pack_start(message_window1, False, False, 5)
result_inner_box.pack_start(code_window1, False, False, 5)
result_inner_box.pack_start(message_window2, False, False, 5)
result_inner_box.pack_start(code_window2, False, False, 5)


PROJECT_DIRECTORY  = "/".join(sys.argv[0].split("/")[:-1])+"/"
CLANGPP_EXECUTABLE = PROJECT_DIRECTORY+"clang++"
WRAPPER_SCRIPT     = PROJECT_DIRECTORY+"constraints-wrapper.py"
TEMP_DIRECTORY     = ""
SOURCE_FILENAME    = TEMP_DIRECTORY+"input.cc"
LLVM_FILENAME      = TEMP_DIRECTORY+"input.ll"
TARGET_FILENAME    = TEMP_DIRECTORY+"output.ll"

callback_id = None

def on_open_clicked(widget):

    dialog = Gtk.FileChooserDialog("Please choose a file", window, Gtk.FileChooserAction.OPEN,
                                   (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL, Gtk.STOCK_OPEN, Gtk.ResponseType.OK))

    response = dialog.run()
    if response == Gtk.ResponseType.OK:
        selected_file = dialog.get_filename()
        with open(selected_file, 'r') as f:
            data = f.read()
            sourcecode.get_buffer().set_text(data)
    elif response == Gtk.ResponseType.CANCEL:
        dialog.destroy()

    dialog.destroy()

def on_analyze_clicked(widget):

    analyze_button.disconnect(callback_id);

    source_code = sourcecode.get_buffer().get_text(sourcecode.get_buffer().get_bounds()[0],
                                                   sourcecode.get_buffer().get_bounds()[1], False)

    with open(SOURCE_FILENAME, 'w') as temp_file:
        temp_file.write(source_code+"\n")

    message_window1.set_text("")
    code_window1.get_buffer().set_text("")
    message_window2.set_text("Running clang and SMT solver.")
    code_window2.get_buffer().set_text("")
    message_window2.queue_draw()

    process = subprocess.Popen([WRAPPER_SCRIPT, CLANGPP_EXECUTABLE, "-std=c++14",  "-S", "-emit-llvm",
                                "-o", TARGET_FILENAME, "-gline-tables-only", SOURCE_FILENAME],
                                cwd = "temp", stderr=subprocess.PIPE)

    Thread(target=wait_thread, args=(process,source_code)).start()

def wait_thread(process, source_code):

    global callback_id
    process.wait()

    stderr_result = "\n".join([line.decode("utf8").rstrip() for line in process.stderr])

    Gdk.threads_enter()
    if stderr_result:

        message_window1.set_text(stderr_result)
        code_window1.get_buffer().set_text("")
        message_window2.set_text("")
        code_window2.get_buffer().set_text("")

    else:

        stdout_result = "".join([line.decode("utf8") for line in open("temp/replace-report.txt")])

        loops_with_reductions = [part.split("END LOOP\n")[0] for part in stdout_result.split("BEGIN LOOP\n")[1:]]

        parsed_loops = []

        for loop in loops_with_reductions:
            scalars  = [part.split("END SCALAR REDUCTION\n")[0] for part in loop.split("BEGIN SCALAR REDUCTION\n")[1:]]
            histos   = [part.split("END HISTOGRAM\n")[0]        for part in loop.split("BEGIN HISTOGRAM\n")[1:]]
            stencils = [part.split("END STENCIL\n")[0]          for part in loop.split("BEGIN STENCIL\n")[1:]]
            operator = loop.split("BEGIN OPERATOR\n")[1]

            operator = "\n".join([line.split('   ')[0] for line in operator.split('\n')])
            operator = "\n".join([line.split('!')[0] for line in operator.split('\n')])
            operator = "\n".join([line.split('#')[0] for line in operator.split('\n')])
            operator = "\n".join([line.split(', align')[0] for line in operator.split('\n')])
            operator = operator.replace(".000000e+00", ".0")
            operator = operator.replace("[ ", "[")
            operator = operator.replace(" ]", "]")
            operator = operator.replace("{ ", "{")
            operator = operator.replace(" \n", "\n")
            operator = operator.replace(" }", "}")
            operator = operator.replace(") {", "){")
            operator = operator.replace("%1 =", "%1  =")
            operator = operator.replace("%2 =", "%2  =")
            operator = operator.replace("%3 =", "%3  =")
            operator = operator.replace("%4 =", "%4  =")
            operator = operator.replace("%5 =", "%5  =")
            operator = operator.replace("%6 =", "%6  =")
            operator = operator.replace("%7 =", "%7  =")
            operator = operator.replace("%8 =", "%8  =")
            operator = operator.replace("%9 =", "%9  =")
            operator = operator.replace("\n  ", "\n")
            operator = operator.replace("getelementptr", "GEP")
            operator = operator.replace("<badref>", "@op")

            reduction_type = operator.split("@op({")[1].split("}")[0];
            operator = operator.replace("{"+reduction_type+"}", "%out_t")

            scalars    = [eval(str(scalar))    for scalar    in scalars]
            histos     = [eval(str(histo))     for histo     in histos]
            stencils   = [eval(str(stencil))   for stencil   in stencils]

            parsed_loops.append({"scalars":scalars, "histos":histos, "stencils":stencils, "operator":operator})

        for loop in parsed_loops:
            debug_pos_begin     = None
            debug_pos_successor = None
            if len(loop["scalars"]) > 0:
                debug_pos_begin     = loop["scalars"][0]["begin"]
                debug_pos_successor = loop["scalars"][0]["successor"]
            elif len(loop["histos"]) > 0:
                debug_pos_begin     = loop["histos"][0]["begin"]
                debug_pos_successor = loop["histos"][0]["successor"]
            elif len(loop["stencils"]) > 0:
                debug_pos_begin     = loop["stencils"][0]["loop1.begin"]
                debug_pos_successor = loop["stencils"][0]["loop1.successor"]

            has_hit = False

            with open(LLVM_FILENAME, 'r') as temp_file:
                for line in temp_file:
                    if line.rstrip() == debug_pos_begin:
                        has_hit = True
                    if has_hit and "!" in line:
                        debug_pos_begin = line.rstrip()
                        break

            has_hit = False

            with open(LLVM_FILENAME, 'r') as temp_file:
                for line in temp_file:
                    if line.rstrip() == debug_pos_successor:
                        has_hit = True
                    if has_hit and "!" in line:
                        debug_pos_successor = line.rstrip()
                        break

            debug_pos_begin     = debug_pos_begin.split("!")[-1]
            debug_pos_successor = debug_pos_successor.split("!")[-1]

            line_nr_begin     = None
            line_nr_successor = None

            with open(LLVM_FILENAME, 'r') as temp_file:
                for line in temp_file:
                    if line[:1] == "!":
                        if line[1:][:len(debug_pos_begin)] == debug_pos_begin:
                            line_nr_begin = line.split('(')[1].split(')')[0]
                        if line[1:][:len(debug_pos_successor)] == debug_pos_successor:
                            line_nr_successor = line.split('(')[1].split(')')[0]

            loop["begin"] = int(line_nr_begin.split("line: ")[1].split(",")[0])
            loop["end"]   = int(line_nr_successor.split("line: ")[1].split(",")[0]) - 1
            while source_code.split("\n")[loop["end"]-1] == "":
                loop["end"] = loop["end"] - 1

        for loop in parsed_loops:

            message_window1.set_text("Found reduction in lines "+str(loop["begin"])+" - "+str(loop["end"])+"\n"+
                                     "with the following reduction type")

            code_window1.get_buffer().set_text("%out_t = type {"+reduction_type+"}")

            scalar_types = [scalar["old_value"  ].split("phi "  )[1].split(" ")[0] for scalar in loop["scalars"]]
            histo_types  = [histo ["store_instr"].split("store ")[1].split(" ")[0] for histo  in loop["histos"]]

            if len(loop["scalars"]) > 0:
                iter_begin = loop["scalars"][0]["iter_begin"].lstrip()
                iter_end   = loop["scalars"][0]["iter_end"].lstrip()
                iter_step  = loop["scalars"][0]["iter_step"].lstrip()

            if len(loop["histos"]) > 0:
                iter_begin = loop["histos"][0]["iter_begin"].lstrip()
                iter_end   = loop["histos"][0]["iter_end"].lstrip()
                iter_step  = loop["histos"][0]["iter_step"].lstrip()

            message_window2.set_text("and reduction operator")

            code_window2.get_buffer().set_text(loop["operator"].strip())

        if len(parsed_loops) == 0:
            message_window1.set_text("")
            code_window1.get_buffer().set_text("")
            message_window2.set_text("No reductions were identified.")
            code_window2.get_buffer().set_text("")

    result_box.queue_draw()
    Gdk.threads_leave()
    callback_id = analyze_button.connect("clicked", on_analyze_clicked)

def on_hide_clicked(widget):
    message_window1.set_text("")
    code_window1.get_buffer().set_text("")
    message_window2.set_text("")
    code_window2.get_buffer().set_text("")
    toplevel_box.show_all()

Gdk.threads_init()
load_button.connect("clicked", on_open_clicked)
callback_id = analyze_button.connect("clicked", on_analyze_clicked)
hide_button.connect("clicked", on_hide_clicked)
window.connect("delete-event", Gtk.main_quit)
window.show_all()
Gtk.main()
