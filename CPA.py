import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import pearsonr

# Load your oscilloscope data (power traces)
power_traces = np.loadtxt('power_traces.csv', delimiter=',')

# Load the corresponding plaintext values
plaintexts = np.loadtxt('plaintexts.csv', delimiter=',')

# Key hypothesis (0x00 to 0xFF for one byte)
key_guesses = range(256)

# Function to compute Hamming weight
def hamming_weight(n):
    return bin(n).count('1')

# Perform correlation analysis
best_guess = None
best_corr = -1

for guess in key_guesses:
    hypothetical_power = []
    for p in plaintexts:
        intermediate_value = sbox[guess ^ p]  # XOR with key guess, then SubBytes (sbox lookup)
        hypothetical_power.append(hamming_weight(intermediate_value))
    
    # Correlate hypothetical power with real traces
    for trace in power_traces:
        corr, _ = pearsonr(hypothetical_power, trace)
        if corr > best_corr:
            best_corr = corr
            best_guess = guess

print(f"Best Key Guess: {hex(best_guess)}")
