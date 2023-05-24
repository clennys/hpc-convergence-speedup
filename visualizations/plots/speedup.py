import numpy as np
import pandas as pd

from bokeh.io import curdoc
from bokeh.plotting import figure
from bokeh.layouts import column, row
from bokeh.models import ColumnDataSource, HoverTool, Select, Slider
from bokeh.io import output_file, save, show


def draw_speedup(df, grid_dim, linear_system):
    filtered_data = df.loc[df["grid_dim"] == grid_dim]
    filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

    gauss_seidel = filtered_data.loc[filtered_data["solver"] == "gauss-seidel"]
    damped_jacobi = filtered_data.loc[filtered_data["solver"] == "damped-jacobi"]

    max_proc_gs, max_proc_dj = (
        gauss_seidel["nr_processes"].min(),
        damped_jacobi["nr_processes"].min(),
    )
    time_proc_N_gs = gauss_seidel.loc[
        gauss_seidel["nr_processes"] == max_proc_gs, "time"
    ].values[0]
    time_proc_N_dj = damped_jacobi.loc[
        damped_jacobi["nr_processes"] == max_proc_dj, "time"
    ].values[0]

    gs_np = time_proc_N_gs / gauss_seidel["time"].values
    dj_np = time_proc_N_dj / damped_jacobi["time"].values

    gauss_seidel["speedup"] = gs_np
    damped_jacobi["speedup"] = dj_np

    source_gs = ColumnDataSource(gauss_seidel)
    source_dj = ColumnDataSource(damped_jacobi)

    p = figure(
        title=f"Speed up with {grid_dim} dimension",
        tools="pan, wheel_zoom, reset, save",
        x_axis_label="Number of processes",
        y_axis_label="Speedup",
        toolbar_location="above",
        width=500,
        height=450,
    )

    pl_gs = p.line(
        source=source_gs,
        x="nr_processes",
        y="speedup",
        legend_label="Gauss-Seidel",
        line_color="blue",
    )

    pl_dj = p.line(
        source=source_dj,
        x="nr_processes",
        y="speedup",
        legend_label="Damped-Jacobi",
        line_color="red",
    )

    pl_optimal = p.line(
        x=np.arange(1, 65),
        y=np.arange(1, 65),
        legend_label="optimal speedup",
        line_color="green",
    )

    c_gs = p.circle(
        source=source_gs,
        x="nr_processes",
        y="speedup",
        legend_label="Gauss-Seidel",
        line_color="blue",
        fill_color="blue",
        fill_alpha=0.2,
    )

    c_dj = p.circle(
        source=source_dj,
        x="nr_processes",
        y="speedup",
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
            ("Iterations", "@iterations"),
            ("Speedup", "@speedup"),
            ("Nr Proc", "@nr_processes"),
        ]
    )

    hover.mode = "mouse"
    hover.renderers = [c_gs, c_dj]

    p.add_tools(hover)

    return p

def draw_speedup_static(df, grid_dim, linear_system, solver, colors, title):
    p = figure(
            title=title,
            tools="pan, wheel_zoom, reset, save",
            x_axis_label="Number of processes",
            y_axis_label="Speedup",
            toolbar_location="above",
            width=500,
            height=450,
        )

    for gd, c in zip(grid_dim, colors):

        filtered_data = df.loc[df["grid_dim"] == gd]
        filtered_data = filtered_data.loc[filtered_data["linear_system"] == linear_system]

        solver_data = filtered_data.loc[filtered_data["solver"] == solver]

        max_proc = (
            solver_data["nr_processes"].min(),
        )
        time_proc_N_solver = solver_data.loc[
            solver_data["nr_processes"] == max_proc, "time"
        ].values[0]

        solver_np = time_proc_N_solver / solver_data["time"].values

        solver_data["speedup"] = solver_np

        solver_src = ColumnDataSource(solver_data)

        pl_optimal = p.line(
            x=np.arange(1, 65),
            y=np.arange(1, 65),
            legend_label="optimal speedup",
            line_color="green",
        )



        pl_gs = p.line(
            source=solver_src,
            x="nr_processes",
            y="speedup",
            legend_label=f"{gd}",
            line_color=c,
        )

        c_gs = p.circle(
            source=solver_src,
            x="nr_processes",
            y="speedup",
            legend_label=f"{gd}",
            line_color=c,
            fill_color=c,
            fill_alpha=0.2,
        )

    p.legend.click_policy = "mute"
    p.legend.location = "top_left"
    p.legend.orientation = "vertical"
    p.legend.title = 'Grid dimension'
    # p.legend.background_fill_alpha = 0
    # p.legend.border_line_alpha = 0
    p.legend.label_text_font_size = "10px"
    p.legend.glyph_width = 16

    hover = HoverTool(
        tooltips=[
            ("Solver", "@solver"),
            ("Time", "@time"),
            ("Grid Dim", "@grid_dim"),
            ("Matrix Dim", "@matrix_size"),
            ("Iterations", "@iterations"),
            ("Speedup", "@speedup"),
            ("Nr Proc", "@nr_processes"),
        ]
    )

    hover.mode = "mouse"
    hover.renderers = [c_gs]

    p.add_tools(hover)

    return p
