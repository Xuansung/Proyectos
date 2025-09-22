void factor(int n, int& f, int& q) {
    f = 2;         // Initialize the factor to the smallest prime
    q = 0;         // Initialize the count of the most frequent factor
    int auxf = 0;  // Auxiliary variable to track the current factor
    int auxq = 0;  // Auxiliary variable to track the frequency of the current factor

    for (int i = 2; i * i <= n; ++i) { // Iterate up to the square root of n
        auxq = 0;                      // Reset the count for the current factor
        while (n % i == 0) {           // While i divides n
            n /= i;                   // Divide n by i
            auxq++;                   // Increment the count for i
        }
        if (auxq > q || (auxq == q && i < auxf)) {
            q = auxq;  // Update the maximum frequency
            auxf = i;  // Update the most frequent factor
        }
    }

    // If n is still greater than 1, it must be a prime factor
    if (n > 1) {
        if (1 > q || n < auxf) {
            auxf = n;
            q = 1;
        }
    }

    f = auxf; // Store the most frequent factor in f
}
