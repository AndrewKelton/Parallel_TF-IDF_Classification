import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import scipy.stats as stats
import os

# Define file paths
MAIN_DIR = "tests/output/processed-data-results/"
RES_PAR = MAIN_DIR + "parallel-processed.csv"
RES_SEQ = MAIN_DIR + "sequential-processed.csv"

def plot_bell_curve(data, title, filename):
    """Generates a bell curve (normal distribution) from data"""
    plt.figure(figsize=(8, 6))

    # Fit a normal distribution to the data
    mu, sigma = np.mean(data), np.std(data)

    # Generate x values for the curve
    x = np.linspace(min(data), max(data), 100)
    y = stats.norm.pdf(x, mu, sigma)

    # Plot histogram
    plt.hist(data, bins=10, density=True, alpha=0.6, color='b', edgecolor="black")

    # Plot normal distribution curve
    plt.plot(x, y, 'r-', linewidth=2, label=f"Normal Dist.\nμ={mu:.2f}, σ={sigma:.2f}")

    # Labels and title
    plt.xlabel("Time (ms)")
    plt.ylabel("Density")
    plt.title(title)
    plt.legend()
    plt.grid()

    # Save the figure
    os.makedirs("tests/output/graphs", exist_ok=True)
    plt.savefig(f"tests/output/graphs/{filename}", format="pdf")
    plt.close()

def main():
    # Load CSV files
    df_par = pd.read_csv(RES_PAR)
    df_seq = pd.read_csv(RES_SEQ)

    # Convert all columns to numeric
    df_par = df_par.apply(pd.to_numeric, errors="coerce")
    df_seq = df_seq.apply(pd.to_numeric, errors="coerce")

    # Iterate through each section (excluding Accuracy)
    for section in df_seq.columns[:-1]:  # Skip last column (Accuracy)
        plot_bell_curve(df_seq[section], f"Bell Curve - Sequential: {section}", f"bell_curve_seq_{section}.pdf")
        plot_bell_curve(df_par[section], f"Bell Curve - Parallel: {section}", f"bell_curve_par_{section}.pdf")

if __name__ == '__main__':
    main()
