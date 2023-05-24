import numpy as np
import pandas as pd

from bokeh.io import curdoc
from bokeh.plotting import figure
from bokeh.layouts import column, row
from bokeh.models import ColumnDataSource, HoverTool, Select, Slider
from bokeh.io import output_file, save, show


def draw_timing_grid(df, nr_processes, linear_system):
    filtered_data = df.loc[df["nr_processes"] == nr_processes]
    filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

    gauss_seidel = filtered_data.loc[filtered_data["solver"] == "gauss-seidel"]
    damped_jacobi = filtered_data.loc[filtered_data["solver"] == "damped-jacobi"]

    source_gs = ColumnDataSource(gauss_seidel)
    source_dj = ColumnDataSource(damped_jacobi)

    p = figure(
        title=f"Timing with {nr_processes} of parallel processes",
        tools="pan, wheel_zoom, reset, save",
        toolbar_location="above",
        x_axis_label="Grid Dim",
        y_axis_label="Time [s]",
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


def draw_timing_processes(df, grid_dim, linear_system):
    filtered_data = df.loc[df["grid_dim"] == grid_dim]
    filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

    gauss_seidel = filtered_data.loc[filtered_data["solver"] == "gauss-seidel"]
    damped_jacobi = filtered_data.loc[filtered_data["solver"] == "damped-jacobi"]

    source_gs = ColumnDataSource(gauss_seidel)
    source_dj = ColumnDataSource(damped_jacobi)

    p = figure(
        title=f"Timing with grid dimension {grid_dim}",
        tools="pan, wheel_zoom, reset, save",
        toolbar_location="above",
        x_axis_label="Number of processes",
        y_axis_label="Time [s]",
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

def draw_timing_processes_static(df, grid_dim, linear_system, solver, colors, title):
    p = figure(
            title=title,
            tools="pan, wheel_zoom, reset, save",
            toolbar_location="above",
            x_axis_label="Number of processes",
            y_axis_label="Time [s]",
            width=500,
            height=450,
        )

    for gd, c in zip(grid_dim, colors):
        filtered_data = df.loc[df["grid_dim"] == gd]
        filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

        solver_data = filtered_data.loc[filtered_data["solver"] == solver]

        source_solver = ColumnDataSource(solver_data)

      
        pl_gs = p.line(
            source=source_solver,
            x="nr_processes",
            y="time",
            legend_label=f"{gd}",
            line_color=c,
        )

        c_gs = p.circle(
            source=source_solver,
            x="nr_processes",
            y="time",
            legend_label=f"{gd}",
            line_color=c,
            fill_color=c,
            fill_alpha=0.2,
        )

        

    p.legend.click_policy = "mute" 
    p.legend.title = 'Grid dimension'
    p.legend.location = "top_right"
    p.legend.orientation = "vertical"
    p.legend.background_fill_alpha = 0
    p.legend.border_line_alpha = 0
    p.legend.label_text_font_size = "13px"
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
    hover.renderers = [c_gs]

    p.add_tools(hover)

    return p
