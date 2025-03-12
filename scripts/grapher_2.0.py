import matplotlib.pyplot as plt
import pandas as pd
import os
import numpy as np

MAIN_DIR = "test-output/processed-data-results/"
RES_PAR = MAIN_DIR + "parallel-processed.csv"
RES_SEQ = MAIN_DIR + "sequential-processed.csv"

def main():
    df_par = pd.read_csv(RES_PAR)
    df_seq = pd.read_csv(RES_SEQ)

    # Convert "Time (ms)" column to numeric (handle errors)
    df_par["Time (ms)"] = pd.to_numeric(df_par["Time (ms)"], errors="coerce")
    df_seq["Time (ms)"] = pd.to_numeric(df_seq["Time (ms)"], errors="coerce")

    # Compute average for each section
    df_par = df_par.groupby("Section", as_index=False)["Time (ms)"].mean().rename(columns={"Time (ms)": "Time_par"})
    df_seq = df_seq.groupby("Section", as_index=False)["Time (ms)"].mean().rename(columns={"Time (ms)": "Time_seq"})

    # Merge dataframes
    df_merge = pd.merge(df_seq, df_par, on="Section")

    # Rename sections for better readability
    df_merge["Section"] = df_merge["Section"].replace({
        "Vectorization": "Vectorization",
        "TF-IDF": "TF-IDF",
        "Categories": "Categories",
        "Accuracy": "Accuracy"
    })
    df_merge.loc[df_merge["Section"] == "Accuracy", ["Time_seq", "Time_par"]] /= 100

    # Apply log scale transformation to time data (excluding "Accuracy")
    df_merge["Log_Time_seq"] = np.log1p(df_merge["Time_seq"])  # Log scale (log(x+1) to handle zero values)
    df_merge["Log_Time_par"] = np.log1p(df_merge["Time_par"])

    # Plot side-by-side bars for log-scaled values for all sections
    sections = df_merge["Section"]
    time_seq = df_merge["Log_Time_seq"]
    time_par = df_merge["Log_Time_par"]

    x = np.arange(len(sections))  # X-axis positions
    width = 0.35  # Bar width

    plt.figure(figsize=(10, 6))
    plt.bar(x - width/2, time_seq, width, label="Sequential", color="lightcoral")
    plt.bar(x + width/2, time_par, width, label="Parallel", color="skyblue")

    # Labels and formatting for all sections
    plt.xlabel("Sections")
    plt.ylabel("Log-Transformed Average Time (ms)")
    plt.title("Comparison of Sequential and Parallel Processing Times (Log Scale)")
    plt.xticks(x, sections, rotation=45, ha="right")
    plt.legend()
    plt.tight_layout()

    # Save the plot for all sections
    plt.savefig("test-output/graphs/time_comparison_plot.pdf", format="pdf")
    plt.close()

    # ---- Separate Plot for "Accuracy" ----
    # Filter the data for the "Accuracy" section (no log transformation)
    df_accuracy = df_merge[df_merge["Section"] == "Accuracy"]

    # Extract relevant data for accuracy plot (using original times, no log transformation)
    time_seq_accuracy = df_accuracy["Time_seq"]
    time_par_accuracy = df_accuracy["Time_par"]

    # Plot the accuracy comparison without log transformation
    plt.figure(figsize=(8, 6))
    plt.bar([0], time_seq_accuracy, width, label="Sequential", color="lightcoral")
    plt.bar([1], time_par_accuracy, width, label="Parallel", color="skyblue")

    # Labels and formatting for accuracy section
    plt.xlabel("Accuracy")
    plt.ylabel("Average Accuracy")
    plt.title("Comparison of Sequential and Parallel Categorization Accuracy (Unknown Section)")
    plt.xticks([0, 1], ["Sequential", "Parallel"])
    plt.legend()
    plt.tight_layout()

    # Save the plot for accuracy
    plt.savefig("test-output/graphs/accuracy_comparison_plot.pdf", format="pdf")
    plt.close()

if __name__ == '__main__':
    main()
