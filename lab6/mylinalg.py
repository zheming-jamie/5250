# this file include the function GaussElimination made in lab5. Function has built already with pivoting.
# following is LeastSquareApprox(x,f,n)

import numpy as np
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

def polyval_ascending(c, x):
    """
    Evaluate polynomial p(x) = c[0] + c[1] x + ... + c[n] x^n.
    c is in ascending-power order.
    """
    x = np.array(x, dtype=float)
    y = np.zeros_like(x, dtype=float)
    for coeff in c[::-1]:  # Horner (descending pass)
        y = y * x + coeff
    return y

def LeastSquareApprox(x, f, n):
    """
    Least-squares polynomial approximation of degree <= n.
    Here f is a FUNCTION (callable): y_i = f(x_i).

    Returns coefficients c in ascending powers:
    p(x) = c[0] + c[1] x + ... + c[n] x^n
    """
    if n+1 > len(x):
        raise ValueError("Degree+1 can't be greater than dim of X.")
    
    x = np.array(x, dtype=float).reshape(-1)

    if not callable(f):
        raise TypeError("LeastSquareApprox expects f to be a function (callable).")

    # Evaluate f on nodes (prefer vectorized; fallback to scalar calls)
    try:
        y = np.asarray(f(x), dtype=float).reshape(-1)
    except Exception:
        y = np.array([f(xi) for xi in x], dtype=float)

    #if y.shape[0] != x.shape[0]:
    #    raise ValueError("f(x) must return an array of the same length as x.")

    # Vandermonde matrix with powers 0..n
    V = np.vstack([x**k for k in range(n + 1)]).T  # (m, n+1)

    # Normal equations: (V^T V) c = V^T y
    ATA = V.T @ V
    ATy = V.T @ y

    c = GaussElimination(ATA, ATy, pivoting=True)
    return c

# now here starts the code if run as script.
if __name__ == "__main__":

    import matplotlib.pyplot as plt
    from numpy import cos

    degree = 5
    xgrid = np.linspace(-np.pi, np.pi, 51)

    # fit coefficients (ascending powers)
    coef = LeastSquareApprox(xgrid, cos, degree)

    # evaluate on nodes + fine grid
    y_nodes = cos(xgrid)
    y_nodes_fit = polyval_ascending(coef, xgrid)

    xfine = np.linspace(-np.pi, np.pi, 2001)
    yfine = cos(xfine)
    yfine_fit = polyval_ascending(coef, xfine)

    # errors
    err_nodes = y_nodes_fit - y_nodes
    err_fine = yfine_fit - yfine
    max_err_nodes = np.max(np.abs(err_nodes))
    max_err_fine = np.max(np.abs(err_fine))

    # print results
    print("\nLeast-squares polynomial approximation to cos(x)")
    print(f"Degree = {degree}")
    print("Coefficients (ascending powers):")
    for k, ck in enumerate(coef):
        print(f"  c[{k}] = {ck:.16e}")
    print(f"Max |error| on 51 nodes      = {max_err_nodes:.16e}")
    print(f"Max |error| on fine grid     = {max_err_fine:.16e}")

    # plot: function + fit
    fig1 = plt.figure()
    plt.plot(xfine, yfine, label="cos(x)")
    fit_line = plt.plot(xfine, yfine_fit, label=f"LS poly degree {degree}")
    plt.scatter(xgrid, y_nodes_fit, s=12,color=fit_line[0].get_color(), label="poly nodes")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Least-squares fit of cos(x)")
    plt.legend()
    plt.tight_layout()
    plt.savefig("ls_cos_degree5.png", dpi=200)

    # plot: error curve
    fig2 = plt.figure()
    plt.plot(xfine, err_fine)
    plt.xlabel("x")
    plt.ylabel("p(x) - cos(x)")
    plt.title("Error of least-squares approximation")
    plt.tight_layout()
    plt.savefig("ls_cos_degree5_error.png", dpi=200)

    plt.show()