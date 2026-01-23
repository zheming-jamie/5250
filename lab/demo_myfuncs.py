import math

def my_factorial(n):
    """Computes n! using a for loop """
    if n == 0: return 1
    s = 1
    for k in range(1, n):
        s = s * (k + 1)
    return s

def my_exp(x, terms=10):
    """
    Computes e^x using Taylor series about x0
    x0: int(round(x))
    z: x - x0
    """
    E = 2.7182818284590451
    x0 = int(round(x))
    z = x - x0
    
    # Taylor series: 1 + z + z^2/2! + z^3/3! ... 
    series_sum = 0
    for i in range(terms):
        series_sum += (z**i) / my_factorial(i)
        
    return (E**x0) * series_sum

def my_ln(x, iterations=10):
    """
    Computes ln(x) using Newton's method
    Update rule: s = s - 1 + x * exp(-s) 
    Initial guess: s = x
    """
    s = float(x)
    for k in range(iterations):
        # Using math.exp for stability in the update rule
        s = s - 1 + x * math.exp(-s)
    return s

def main():
    test_values = [1.0, 2.0, 0.5, 5.0]
    #print the table header
    print(f"{'x-value':<10} | {'my_exp':<15} | {'math.exp':<15} | {'my_ln':<15} | {'math.log':<15}")
    print("-" * 75)
    #compute my function and math.function
    for val in test_values:
        e_custom = my_exp(val)
        e_math = math.exp(val)
        ln_custom = my_ln(val)
        ln_math = math.log(val)
        # print and compare function values in a table as output
        print(f"{val:<10} | {e_custom:<15.10f} | {e_math:<15.10f} | {ln_custom:<15.10f} | {ln_math:<15.10f}")

if __name__ == "__main__":
    main()