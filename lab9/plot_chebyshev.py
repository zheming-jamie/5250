# plot_chebyshev.py (Lab 9)
# Reads lab9/poly.data and saves lab9/chebyshev.png

from pathlib import Path

def PlotChebyshev():
    here = Path(__file__).resolve().parent
    data_path = here / "poly.data"
    fig_path  = here / "chebyshev.png"

    # First, figure out how many lines there are
    NumPts = 0
    with open(data_path, "r") as fid:
        while True:
            line = fid.readline()
            if not line:
                break
            if line.strip() == "":
                continue
            NumPts += 1

    import numpy as np
    import matplotlib.pyplot as plt

    # Second, read-in all the floats
    x = np.zeros(NumPts, dtype=float)
    y = np.zeros(NumPts, dtype=float)

    with open(data_path, "r") as fid:
        for k in range(0, NumPts):
            linestring = fid.readline()
            linelist = linestring.split()
            x[k] = float(linelist[0])
            y[k] = float(linelist[1])

    # Third, plot the result (same style as PlotPoly.py)
    plt.rc("font", size=16)
    plt.figure(1)
    plt.plot(x, y, linestyle="dashed", linewidth=2, marker="o",
             color="red", markersize=12)
    plt.xlim(-1.0, 1.0)
    plt.xticks([-1.0, -0.5, 0.0, 0.5, 1.0])
    ax = plt.gca()
    ax.grid(True)
    plt.xlabel("x-axis", size=20)
    plt.ylabel("y-axis", size=20)
    plt.title("Chebyshev Polynomial Plot", size=20)
    plt.savefig(fig_path, dpi=400, bbox_inches="tight")
    plt.show()

if __name__ == "__main__":
    PlotChebyshev()
