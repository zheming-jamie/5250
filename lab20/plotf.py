import argparse
from pathlib import Path
import subprocess

import matplotlib.pyplot as plt
import numpy as np


def parse_stdout(text: str):
    value = None
    wall_time = None
    for line in text.splitlines():
        if line.startswith("V(x0) =") or line.startswith("J0(x) ="):
            value = float(line.split("=", 1)[1].strip())
        elif line.startswith("wall_time ="):
            wall_time = float(line.split("=", 1)[1].strip())
    if value is None or wall_time is None:
        raise RuntimeError(f"Could not parse solver output:\n{text}")
    return value, wall_time


def run_solver(exe: Path, case_id: int, threads: int, tol: float, param: float, log_file: Path):
    cmd = [str(exe), str(case_id), str(threads), f"{tol:.12e}", f"{param:.16f}", str(log_file)]
    result = subprocess.run(cmd, check=True, capture_output=True, text=True)
    return parse_stdout(result.stdout), result.stdout


def read_intervals(log_file: Path):
    data = np.loadtxt(log_file)
    if data.ndim == 1:
        data = data.reshape(1, -1)
    thread_ids = data[:, 0].astype(int)
    a = data[:, 1]
    b = data[:, 2]
    return thread_ids, a, b


def plot_subintervals(log_file: Path, output_png: Path, title: str):
    thread_ids, a, b = read_intervals(log_file)
    unique_threads = sorted(set(thread_ids.tolist()))
    cmap = plt.get_cmap("tab10", max(10, len(unique_threads)))
    thread_to_color = {tid: cmap(i % 10) for i, tid in enumerate(unique_threads)}

    plt.figure(figsize=(10, 6))
    for k, (tid, ak, bk) in enumerate(zip(thread_ids, a, b)):
        plt.plot([ak, bk], [-k, -k], color=thread_to_color[tid], linewidth=1.8)
    handles = [plt.Line2D([0], [0], color=thread_to_color[tid], lw=2, label=f"Thread {tid}") for tid in unique_threads]
    plt.legend(handles=handles, loc="best", fontsize=9)
    plt.title(title)
    plt.xlabel("Interval")
    plt.ylabel("Recorded subdivision order")
    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close()


def plot_case1_curve(x0_vals, v_vals, output_png: Path):
    plt.figure(figsize=(9, 5))
    plt.plot(x0_vals, v_vals, linewidth=2)
    plt.xlabel(r"$x_0$")
    plt.ylabel(r"$V(x_0)$")
    plt.title(r"Case 1: 1D electrostatic potential for $\rho(x)=e^{-x^2}$ on $[-1,1]$")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close()


def plot_case2_curve(x_vals, j_vals, output_png: Path):
    plt.figure(figsize=(9, 5))
    plt.plot(x_vals, j_vals, linewidth=2)
    plt.xlabel(r"$x$")
    plt.ylabel(r"$J_0(x)$")
    plt.title(r"Case 2: Bessel function $J_0(x)$ from adaptive quadrature")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close()


def plot_wall_time(params, wall_times, xlabel: str, title: str, output_png: Path):
    plt.figure(figsize=(9, 5))
    plt.plot(params, wall_times, linewidth=1.8)
    plt.xlabel(xlabel)
    plt.ylabel("Wall time (s)")
    plt.title(title)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(output_png, dpi=300)
    plt.close()


def save_csv(path: Path, header: str, cols):
    arr = np.column_stack(cols)
    np.savetxt(path, arr, delimiter=",", header=header, comments="")


def benchmark_threads(exe: Path, case_id: int, tol: float, param: float, threads_list, outdir: Path, stem: str):
    rows = []
    for threads in threads_list:
        log_file = outdir / f"{stem}_threads_{threads}.data"
        (value, wall_time), _ = run_solver(exe, case_id, threads, tol, param, log_file)
        rows.append((threads, value, wall_time, log_file))
    csv_path = outdir / f"{stem}_benchmark.csv"
    save_csv(csv_path, "threads,value,wall_time_s", [[r[0] for r in rows], [r[1] for r in rows], [r[2] for r in rows]])
    return rows, csv_path


def main():
    parser = argparse.ArgumentParser(description="Run Lab 20 OpenMP version #2 for both assignment cases and plot results.")
    parser.add_argument("--exe", default="./main.exe")
    parser.add_argument("--sweep-threads", type=int, default=1, help="Threads used for curve sweeps")
    parser.add_argument("--rep-threads", type=int, default=4, help="Threads used for representative subinterval plots")
    parser.add_argument("--bench-threads", default="1,2,4", help="Comma-separated thread counts for benchmark table")
    parser.add_argument("--tol", type=float, default=1.0e-6)
    parser.add_argument("--n1", type=int, default=201, help="Number of x0 points for case 1")
    parser.add_argument("--n2", type=int, default=301, help="Number of x points for case 2")
    parser.add_argument("--outdir", default="results")
    args = parser.parse_args()

    exe = Path(args.exe).resolve()
    outdir = Path(args.outdir).resolve()
    outdir.mkdir(parents=True, exist_ok=True)
    bench_threads = [int(x.strip()) for x in args.bench_threads.split(",") if x.strip()]

    # Case 1 sweep on x0 in [-1, 1]
    x0_vals = np.linspace(-1, 1, args.n1)
    v_vals = np.zeros_like(x0_vals)
    wt1 = np.zeros_like(x0_vals)
    for i, x0 in enumerate(x0_vals):
        log_file = outdir / "tmp_case1.data"
        (value, wall_time), _ = run_solver(exe, 1, args.sweep_threads, args.tol, float(x0), log_file)
        v_vals[i] = value
        wt1[i] = wall_time
    tmp_case1 = outdir / "tmp_case1.data"
    if tmp_case1.exists():
        tmp_case1.unlink()

    save_csv(outdir / "case1_results.csv", "x0,V_x0,wall_time_s", [x0_vals, v_vals, wt1])
    plot_case1_curve(x0_vals, v_vals, outdir / "case1_potential.png")
    plot_wall_time(x0_vals, wt1, r"$x_0$", f"Case 1 wall time per evaluation ({args.sweep_threads} thread)", outdir / "case1_wall_time.png")

    # Case 2 sweep on x in [0,50]
    x_vals = np.linspace(0.0, 50.0, args.n2)
    j_vals = np.zeros_like(x_vals)
    wt2 = np.zeros_like(x_vals)
    for i, x in enumerate(x_vals):
        log_file = outdir / "tmp_case2.data"
        (value, wall_time), _ = run_solver(exe, 2, args.sweep_threads, args.tol, float(x), log_file)
        j_vals[i] = value
        wt2[i] = wall_time
    tmp_case2 = outdir / "tmp_case2.data"
    if tmp_case2.exists():
        tmp_case2.unlink()

    save_csv(outdir / "case2_results.csv", "x,J0_x,wall_time_s", [x_vals, j_vals, wt2])
    plot_case2_curve(x_vals, j_vals, outdir / "case2_j0.png")
    plot_wall_time(x_vals, wt2, r"$x$", f"Case 2 wall time per evaluation ({args.sweep_threads} thread)", outdir / "case2_wall_time.png")

    # Representative subinterval plots from parallel execution
    case1_rep_x0 = 0.0
    case2_rep_x = 50.0
    case1_log = outdir / "case1_subintervals.data"
    case2_log = outdir / "case2_subintervals.data"
    run_solver(exe, 1, args.rep_threads, args.tol, case1_rep_x0, case1_log)
    run_solver(exe, 2, args.rep_threads, args.tol, case2_rep_x, case2_log)
    plot_subintervals(case1_log, outdir / "case1_subintervals.png", f"Case 1 representative subintervals (x0 = {case1_rep_x0}, {args.rep_threads} threads)")
    plot_subintervals(case2_log, outdir / "case2_subintervals.png", f"Case 2 representative subintervals (x = {case2_rep_x}, {args.rep_threads} threads)")

    # Benchmarks
    case1_rows, case1_bench_csv = benchmark_threads(exe, 1, args.tol, case1_rep_x0, bench_threads, outdir, "case1")
    case2_rows, case2_bench_csv = benchmark_threads(exe, 2, args.tol, case2_rep_x, bench_threads, outdir, "case2")

    summary_lines = [
        f"sweep_threads = {args.sweep_threads}",
        f"rep_threads = {args.rep_threads}",
        f"tol = {args.tol:.3e}",
        f"case 1 sweep points = {args.n1}",
        f"case 2 sweep points = {args.n2}",
        f"case 1 sweep total wall time = {wt1.sum():.6f} s",
        f"case 1 sweep average wall time = {wt1.mean():.6e} s",
        f"case 1 sweep max wall time = {wt1.max():.6e} s at x0 = {x0_vals[np.argmax(wt1)]:.6f}",
        f"case 2 sweep total wall time = {wt2.sum():.6f} s",
        f"case 2 sweep average wall time = {wt2.mean():.6e} s",
        f"case 2 sweep max wall time = {wt2.max():.6e} s at x = {x_vals[np.argmax(wt2)]:.6f}",
        "",
        f"Case 1 benchmark at x0 = {case1_rep_x0}:",
    ]
    for threads, value, wall_time, _ in case1_rows:
        summary_lines.append(f"  threads = {threads:2d}, V(x0) = {value:.12e}, wall_time = {wall_time:.6e} s")
    summary_lines.extend(["", f"Case 2 benchmark at x = {case2_rep_x}:"])
    for threads, value, wall_time, _ in case2_rows:
        summary_lines.append(f"  threads = {threads:2d}, J0(x) = {value:.12e}, wall_time = {wall_time:.6e} s")
    summary_lines.extend(["", f"Benchmark CSV files: {case1_bench_csv.name}, {case2_bench_csv.name}"])
    summary = "\n".join(summary_lines) + "\n"
    (outdir / "timing_summary.txt").write_text(summary, encoding="utf-8")
    print(summary)


if __name__ == "__main__":
    main()
