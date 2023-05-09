import numpy as np
import pandas as pd

from bokeh.io import curdoc
from bokeh.plotting import figure
from bokeh.layouts import column, row, Spacer
from bokeh.models import ColumnDataSource, HoverTool, LassoSelectTool, Select, Slider
from bokeh.palettes import TolRainbow, Turbo256
from bokeh.transform import factor_cmap, linear_cmap, log_cmap
from bokeh.io import output_file, save, show, output_notebook


# import data
file_path = "./output_clean.csv"
data = pd.read_csv(file_path)


def draw_grid_vs_time(df, nr_processes, linear_system):
    filtered_data = df.loc[df["nr_processes"] == nr_processes]
    filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

    gauss_seidel = filtered_data.loc[filtered_data["solver"] == "gauss-seidel"]
    damped_jacobi = filtered_data.loc[filtered_data["solver"] == "damped-jacobi"]

    source_gs = ColumnDataSource(gauss_seidel)
    source_dj = ColumnDataSource(damped_jacobi)

    p = figure(
        title=f"Speed up with {nr_processes} of parallel processes",
        tools="pan, wheel_zoom, reset",
        toolbar_location="below",
        width=500,
        height=450,
    )

    pl_gs = p.line(
        source=source_gs,
        x="grid_dim",
        y="time",
        legend_label="Gauss-Seidel",
        line_color="blue",
    )

    pl_dj = p.line(
        source=source_dj,
        x="grid_dim",
        y="time",
        legend_label="Damped-Jacobi",
        line_color="red",
    )
    c_gs = p.circle(
        source=source_gs,
        x="grid_dim",
        y="time",
        legend_label="Gauss-Seidel",
        line_color="blue",
        fill_color="blue",
        fill_alpha=0.2,
    )

    c_dj = p.circle(
        source=source_dj,
        x="grid_dim",
        y="time",
        legend_label="Damped-Jacobi",
        line_color="red",
        fill_color="red",
        fill_alpha=0.2,
    )

    p.legend.click_policy = "mute"
    p.legend.location = "top_left"
    p.legend.orientation = "horizontal"
    p.legend.background_fill_alpha = 0
    p.legend.border_line_alpha = 0
    p.legend.label_text_font_size = "10px"
    p.legend.glyph_width = 16

    hover = HoverTool(
        tooltips=[
            ("Solver", "@solver"),
            ("Time", "@time"),
            ("Grid Dim", "@grid_dim"),
            ("Matrix Dim", "@matrix_size"),
            ("iterations", "@iterations"),
        ]
    )

    hover.mode = "mouse"
    hover.renderers = [c_gs, c_dj]

    p.add_tools(hover)

    return p


def draw_nr_proc_vs_time(df, grid_dim, linear_system):
    filtered_data = df.loc[df["grid_dim"] == grid_dim]
    filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

    gauss_seidel = filtered_data.loc[filtered_data["solver"] == "gauss-seidel"]
    damped_jacobi = filtered_data.loc[filtered_data["solver"] == "damped-jacobi"]

    source_gs = ColumnDataSource(gauss_seidel)
    source_dj = ColumnDataSource(damped_jacobi)

    p = figure(
        title=f"Speed up with {grid_dim} dimension",
        tools="pan, wheel_zoom, reset",
        toolbar_location="below",
        width=500,
        height=450,
    )

    pl_gs = p.line(
        source=source_gs,
        x="nr_processes",
        y="time",
        legend_label="Gauss-Seidel",
        line_color="blue",
    )

    pl_dj = p.line(
        source=source_dj,
        x="nr_processes",
        y="time",
        legend_label="Damped-Jacobi",
        line_color="red",
    )
    c_gs = p.circle(
        source=source_gs,
        x="nr_processes",
        y="time",
        legend_label="Gauss-Seidel",
        line_color="blue",
        fill_color="blue",
        fill_alpha=0.2,
    )

    c_dj = p.circle(
        source=source_dj,
        x="nr_processes",
        y="time",
        legend_label="Damped-Jacobi",
        line_color="red",
        fill_color="red",
        fill_alpha=0.2,
    )

    p.legend.click_policy = "mute"
    p.legend.location = "top_left"
    p.legend.orientation = "horizontal"
    p.legend.background_fill_alpha = 0
    p.legend.border_line_alpha = 0
    p.legend.label_text_font_size = "10px"
    p.legend.glyph_width = 16

    hover = HoverTool(
        tooltips=[
            ("Solver", "@solver"),
            ("Time", "@time"),
            ("Grid Dim", "@grid_dim"),
            ("Matrix Dim", "@matrix_size"),
            ("iterations", "@iterations"),
        ]
    )

    hover.mode = "mouse"
    hover.renderers = [c_gs, c_dj]

    p.add_tools(hover)

    return p


def save_as_html(p):
    output_file("num_training.html")
    save(p)
    show(p)


def grid_time():
    global grid_time_p, slider_grid_time, input_matrix_grid_time
    input_matrix = "five-point"
    nr_proc = 10
    grid_time_p = draw_grid_vs_time(data, nr_proc, input_matrix)
    slider_grid_time = Slider(
        start=1, end=64, value=10, step=1, title="Parallel Processes"
    )

    input_matrix_grid_time = Select(
        title="Select Input",
        value=input_matrix,
        options=["five-point", "nine-point"],
        width=200,
        # margin= (20,10,10,29),
    )

    def callback_slider_grid_time(attr, old, new):
        nr_proc = slider_grid_time.value
        layout.children[0].children[0] = draw_grid_vs_time(data, nr_proc, input_matrix)

    def callback_input_matrix_grid_time(attr, old, new):
        matrix = input_matrix_grid_time.value
        layout.children[0].children[0] = draw_grid_vs_time(data, nr_proc, matrix)

    slider_grid_time.on_change("value", callback_slider_grid_time)
    input_matrix_grid_time.on_change("value", callback_input_matrix_grid_time)


def nr_proc_time():

    global nr_proc_time_p, slider_nr_proc_time, input_matrix_nr_proc_time
    input_matrix = "five-point"
    grid = 42
    nr_proc_time_p = draw_nr_proc_vs_time(data, grid, input_matrix)
    slider_nr_proc_time = Slider(
        start=2, end=80, value=42, step=1, title="Grid Dimension"
    )

    input_matrix_nr_proc_time = Select(
        title="Select Input",
        value=input_matrix,
        options=["five-point", "nine-point"],
        width=200,
        # margin= (20,10,10,29),
    )

    def callback_slider_nr_proc_time(attr, old, new):
        grid = slider_nr_proc_time.value
        layout.children[0].children[1] = draw_nr_proc_vs_time(data, grid, input_matrix)

    def callback_input_matrix_nr_proc_time(attr, old, new):
        matrix = input_matrix_nr_proc_time.value
        layout.children[0].children[1] = draw_nr_proc_vs_time(data, grid, matrix)

    slider_nr_proc_time.on_change("value", callback_slider_nr_proc_time)
    input_matrix_nr_proc_time.on_change("value", callback_input_matrix_nr_proc_time)


# PLOTS
grid_time()
nr_proc_time()


# LAYOUT
layout = row(
     column(
         grid_time_p,
         nr_proc_time_p,
         width=500,
     ),
     column(
         slider_grid_time,
         input_matrix_grid_time,
         Spacer(height=340),
         slider_nr_proc_time,
         input_matrix_nr_proc_time,
         width=350,
     ),
 )

curdoc().add_root(layout)
curdoc().title = "Numerical Training"
