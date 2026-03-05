# Lab 14 — Eigenvalue solvers (Power / Shifted Inverse / Rayleigh Quotient)

## What the last slide asks you to do
Add these 3 methods to `matrix.c`:
- `PowerIteration()`
- `ShiftedInverseIteration()`
- `RayleighQuotientIteration()`

Use the **same A matrix as Lab 13**, and test with **n=5** and **n=10**.

This project already does that.

## Matrix A (same as Lab 13)
`A = (1/4) * tridiag(1, 2, 1)` (symmetric blur operator), i.e.
- diagonal = 0.5
- off-diagonal = 0.25

## Build / Run (matches your MinGW workflow)

From Git Bash (MINGW64):

```bash
mingw32-make clean
mingw32-make
./main.exe
```

Optional:
```bash
./main.exe 10
```

## Output
For each `n`, it prints eigenvalue estimates from:
- Power Iteration (dominant eigenvalue)
- Shifted Inverse Iteration with `mu=0` (targets smallest eigenvalue)
- Shifted Inverse Iteration with `mu=0.55` (targets eigenvalue nearest 0.5)
- Rayleigh Quotient Iteration (cubic local convergence)

It also prints the corresponding **true eigenvalues** for this special tridiagonal matrix for quick verification.

## Output file
The program writes the same output to `lab14_output.txt` in the current folder.
