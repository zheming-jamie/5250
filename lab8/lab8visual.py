#!/usr/bin/env python3
"""
Read exp.data (two whitespace-separated columns: x, my_exp(x)),
plot my_exp(x), show it, and save the plot to ./lab/.
"""

from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt


def main():
    # Auto-detect input file location
    candidates = [Path("lab8/exp.data"), Path("exp.data")]
    inpath = next((p for p in candidates if p.exists()), None)
    if inpath is None:
        raise FileNotFoundError("Could not find exp.data at lab8/exp.data or ./exp.data")

    # Output folder: "./lab" (relative to where you run python)
    outdir = Path("lab")
    outdir.mkdir(parents=True, exist_ok=True)
    outpath = outdir / "my_exp_plot.png"

    # Read whitespace-separated data, no header
    df = pd.read_csv(inpath, sep=r"\s+", header=None, names=["x", "my_exp"], engine="python")

    # Plot
    plt.figure()
    plt.plot(df["x"], df["my_exp"], marker="o", linestyle="-")
    plt.xlabel("x")
    plt.ylabel("my_exp(x)")
    plt.title("my_exp(x) from exp.data")
    plt.tight_layout()

    # Save then show
    plt.savefig(outpath, dpi=200)
    plt.show()

    print(f"Loaded {len(df)} rows from: {inpath}")
    print(f"Saved plot to: {outpath.resolve()}")


if __name__ == "__main__":
    main()
