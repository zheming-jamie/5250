import numpy as np
from numpy import cos,max

# Gaussian Elimination build:

def GaussElimination(A, b, pivoting=True, tol=1e-14):
    """
    Solve Ax = b using Gaussian elimination (with optional partial pivoting).

    Parameters
    ----------
    A : array_like, shape (n, n)
        Coefficient matrix.
    b : array_like, shape (n,) or (n,1)
        Right-hand side vector.
    pivoting : bool
        If True, use partial pivoting for numerical stability.
    tol : float
        Pivot tolerance for detecting near-singularity.

    Returns
    -------
    x : ndarray, shape (n,)
        Solution vector.
    """
    A = np.array(A, dtype=float, copy=True)
    b = np.array(b, dtype=float, copy=True).reshape(-1)

    n = A.shape[0]
    if A.shape != (n, n):
        raise ValueError("A must be square (n x n).")
    if b.shape[0] != n:
        raise ValueError("b must have length n.")

    # Augmented matrix [A | b]
    Ab = np.hstack([A, b.reshape(n, 1)])

    # Forward elimination
    for k in range(n - 1):
        if pivoting:
            p = k + np.argmax(np.abs(Ab[k:, k]))
            if abs(Ab[p, k]) < tol:
                raise ValueError("Matrix is singular or nearly singular (pivot too small).")
            if p != k:
                Ab[[k, p], :] = Ab[[p, k], :]
        else:
            if abs(Ab[k, k]) < tol:
                raise ValueError("Zero/near-zero pivot encountered; enable pivoting.")

        for i in range(k + 1, n):
            m = Ab[i, k] / Ab[k, k]
            Ab[i, k:] -= m * Ab[k, k:]
            Ab[i, k] = 0.0  # clean-up

    if abs(Ab[n - 1, n - 1]) < tol:
        raise ValueError("Matrix is singular or nearly singular (last pivot too small).")

    # Back substitution
    x = np.zeros(n, dtype=float)
    for i in range(n - 1, -1, -1):
        rhs = Ab[i, -1] - np.dot(Ab[i, i + 1:n], x[i + 1:n])
        x[i] = rhs / Ab[i, i]

    return x

# approach polynomial function to cos function at points -0.1, -0.02, 0.02, 0.1
# and measure the maximum error
# print out the maximum error

cospoint = np.array([cos(-0.1), cos(-0.02), cos(0.02), cos(0.1)])

A = np.matrix([[(-0.1)**3, (-0.1)**2, (-0.1), 1],
             [(-0.02)**3, (-0.02)**2, (-0.02), 1],
             [(0.02)**3, (0.02)**2, (0.02), 1],
             [(0.1)**3, (0.1)**2, (0.1), 1]])

x = GaussElimination(A, cospoint)

f = lambda t: x[0]*t**3 + x[1]*t**2 + x[2]*t + x[3]

errors = np.array([abs(f(t)-cos(t)) for t in cospoint])
maxerr = errors.max()
print("Maximium error between f and p is:", maxerr)