import numpy as np
import pandas as pd

from bokeh.io import curdoc
from bokeh.plotting import figure
from bokeh.layouts import column, row
from bokeh.models import ColumnDataSource, HoverTool, Select, Slider
from bokeh.io import output_file, save, show
from bokeh.palettes import Category10, Bokeh

from plots.speedup import draw_speedup, draw_speedup_static
from plots.timing import draw_timing_grid, draw_timing_processes, draw_timing_processes_static
from plots.efficency import draw_efficiency, draw_efficiency_static


# import data
file_path = "./output_clean.csv"
data = pd.read_csv(file_path)


def save_as_html(p):
    output_file("num_training.html")
    save(p)
    show(p)


def speedup():
    global speedup_p
    speedup_p = draw_speedup(data, grid, input)

def speedup_mult(solver, input_sel, title):
    grid_input = [20, 25, 30, 35, 40, 50, 60]
    colors = Bokeh[len(grid_input)] 
    return draw_speedup_static(data, grid_input, input_sel, solver, colors, title)

def grid_time():
    global grid_time_p
    grid_time_p = draw_timing_grid(data, nr_proc, input)

def nr_proc_time():
    global nr_proc_time_p
    nr_proc_time_p = draw_timing_processes(data, grid, input)

def nr_proc_time_mult(solver, input_sel, title):
    grid_input = [20, 25, 30, 35, 40, 50, 60]
    colors = Bokeh[len(grid_input)] 
    return draw_timing_processes_static(data, grid_input, input_sel, solver, colors, title)


def efficiency():
    global efficiency_p
    efficiency_p = draw_efficiency(data, grid, input)

def efficiency_mult(solver, input_sel, title):
    grid_input = [20, 25, 30, 35, 40, 50, 60]
    colors = Bokeh[len(grid_input)] 
    return draw_efficiency_static(data, grid_input, input_sel, solver, colors, title)



# PLOTS
grid = 42
input = "five-point"
nr_proc = 10
slider_grid = Slider(start=2, end=80, value=42, step=1, title="Grid Dimension")
slider_processes = Slider(start=1, end=64, value=10, step=1, title="Parallel Processes")
select_input = Select(
    title="Select Input",
    value=input,
    options=["five-point", "nine-point"],
    width=200,
    # margin= (20,10,10,29),
)


grid_time()
nr_proc_time()
speedup()
efficiency()

p_dj_f_s = speedup_mult("damped-jacobi", "five-point", "Damped-Jacobi Speedup for Five-Point")
p_dj_n_s = speedup_mult("damped-jacobi", "nine-point", "Damped-Jacobi Speedup for L2-Projection")
p_gs_f_s = speedup_mult("gauss-seidel", "five-point",  "Gauss-Seidel Speedup for Five-Point")
p_gs_n_s = speedup_mult("gauss-seidel", "nine-point",  "Gauss-Seidel Speedup for L2-Projection")


p_dj_f_t = nr_proc_time_mult("damped-jacobi", "five-point", "Damped-Jacobi Timing for Five-Point")
p_dj_n_t = nr_proc_time_mult("damped-jacobi", "nine-point", "Damped-Jacobi Timing for L2-Projection")
p_gs_f_t = nr_proc_time_mult("gauss-seidel", "five-point",  "Gauss-Seidel Timing for Five-Point")
p_gs_n_t = nr_proc_time_mult("gauss-seidel", "nine-point",  "Gauss-Seidel Timing for L2-Projection")

p_dj_f_e = efficiency_mult("damped-jacobi", "five-point",  "Damped-Jacobi Efficiency for Five-Point")
p_dj_n_e = efficiency_mult("damped-jacobi", "nine-point",  "Damped-Jacobi Efficiency for L2-Projection")
p_gs_f_e = efficiency_mult("gauss-seidel", "five-point",  "Gauss-Seidel Efficiency for Five-Point")
p_gs_n_e = efficiency_mult("gauss-seidel", "nine-point",  "Gauss-Seidel Efficiency for L2-Projection")





def callback_slider_processes(attr, old, new):
    global nr_proc
    nr_proc = slider_processes.value
    layout.children[1].children[1] = draw_timing_grid(data, nr_proc, input)


def callback_select_input(attr, old, new):
    global input
    input = select_input.value
    layout.children[0].children[0] = draw_speedup(data, grid, input)
    layout.children[0].children[1] = draw_efficiency(data, grid, input)
    layout.children[1].children[0] = draw_timing_processes(data, nr_proc, input)
    layout.children[1].children[1] = draw_timing_grid(data, grid, input)


def callback_slider_grid(attr, old, new):
    global grid
    grid = slider_grid.value
    layout.children[0].children[0] = draw_speedup(data, grid, input)
    layout.children[0].children[1] = draw_efficiency(data, grid, input)
    layout.children[1].children[0] = draw_timing_processes(data, grid, input)


select_input.on_change("value", callback_select_input)
slider_processes.on_change("value", callback_slider_processes)
slider_grid.on_change("value", callback_slider_grid)

# LAYOUT
layout = row(
    column(
        speedup_p,
        efficiency_p,
        # p_dj_f_s,
        # p_dj_n_s,
        # p_dj_f_t,
        # p_dj_n_t,
        # p_dj_f_e,
        # p_dj_n_e,
        width=500,
    ),
    column(
        nr_proc_time_p,
        grid_time_p,
        # p_gs_f_s,
        # p_gs_n_s,
        # p_gs_f_t,
        # p_gs_n_t,
        # p_gs_f_e,
        # p_gs_n_e,
        width=500,
    ),
    column(
        select_input,
        slider_processes,
        slider_grid,
        width=350,
    ),
)

curdoc().add_root(layout)
curdoc().title = "Numerical Training"
