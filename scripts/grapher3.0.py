#grapher3.0.py

'''

       || ===== * ==== * ==== * !!!!!!!!!! * ==== * ==== * ===== ||
       || ===== * ==== * ==== * ! NOTICE ! * ==== * ==== * ===== ||
       || ===== * ==== * ==== * !!!!!!!!!! * ==== * ==== * ===== ||
       ||                                                        ||
       ||       This python file is only used to graph           ||
       ||        certain metrics being recorded during           ||
       ||           during runs of this project.                 ||
       ||                                                        ||
       ||  To view outputted graphs, visit 'test-output/graphs'  ||
       ||                                                        ||
       || ===== + ==== + ==== + ========== + ==== + ==== + ===== ||

'''
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

# Define file paths
MAIN_DIR = "tests/test-output/processed-data-results/"
RES_PAR = MAIN_DIR + "parallel-processed.csv"
RES_SEQ = MAIN_DIR + "sequential-processed.csv"

def main():
    # Load CSV files
    df_par = pd.read_csv(RES_PAR)
    df_seq = pd.read_csv(RES_SEQ)

    # Convert all columns to numeric (handle errors gracefully)
    df_par = df_par.apply(pd.to_numeric, errors="coerce")
    df_seq = df_seq.apply(pd.to_numeric, errors="coerce")

    # Compute mean for each section
    df_par_mean = df_par.mean().reset_index()
    df_seq_mean = df_seq.mean().reset_index()
    df_par_mean.columns = ["Section", "Time_par"]
    df_seq_mean.columns = ["Section", "Time_seq"]

    # Merge dataframes
    df_merge = pd.merge(df_seq_mean, df_par_mean, on="Section")

    # Rename "Unknown Classification" for readability
    df_merge["Section"] = df_merge["Section"].replace({
        "Unknown Classification": "Unknown Class."
    })

    # Separate accuracy and scale it correctly
    accuracy_seq = df_merge[df_merge["Section"] == "Accuracy"]["Time_seq"].values[0] / 100
    accuracy_par = df_merge[df_merge["Section"] == "Accuracy"]["Time_par"].values[0] / 100
    df_merge = df_merge[df_merge["Section"] != "Accuracy"]

    # Apply log scale transformation to time values
    df_merge["Log_Time_seq"] = np.log1p(df_merge["Time_seq"])
    df_merge["Log_Time_par"] = np.log1p(df_merge["Time_par"])

    # Plot processing time comparison
    sections = df_merge["Section"]
    time_seq = df_merge["Log_Time_seq"]
    time_par = df_merge["Log_Time_par"]

    x = np.arange(len(sections))  # X-axis positions
    width = 0.35  # Bar width

    plt.figure(figsize=(10, 6))
    bars_seq = plt.bar(x - width/2, time_seq, width, label="Sequential", color="lightcoral")
    bars_par = plt.bar(x + width/2, time_par, width, label="Parallel", color="skyblue")

    # Labels and formatting
    plt.xlabel("Sections")
    plt.ylabel("Log-Transformed Average Time (ms)")
    plt.title("Comparison of Sequential and Parallel Processing Times (Log Scale)")
    plt.xticks(x, sections, rotation=45, ha="right")
    plt.legend()
    plt.tight_layout()

    # Print y-values next to bars
    for bar in bars_seq:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width() / 2, yval, round(yval, 2), ha='center', va='bottom', fontsize=10)

    for bar in bars_par:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width() / 2, yval, round(yval, 2), ha='center', va='bottom', fontsize=10)

    # Save the processing time plot
    os.makedirs("tests/test-output/graphs", exist_ok=True)
    plt.savefig("tests/test-output/graphs/time_comparison_plot.pdf", format="pdf")
    plt.close()

    # ---- Accuracy Comparison Plot ----
    plt.figure(figsize=(6, 6))
    bars_seq_acc = plt.bar([0], [accuracy_seq], width, label="Sequential", color="lightcoral")
    bars_par_acc = plt.bar([1], [accuracy_par], width, label="Parallel", color="skyblue")

    # Labels and formatting
    plt.xlabel("Accuracy")
    plt.ylabel("Average Accuracy")
    plt.title("Comparison of Sequential and Parallel Categorization Accuracy")
    plt.xticks([0, 1], ["Sequential", "Parallel"])
    plt.legend()
    plt.ylim(0, 1)  # Scale accuracy from 0 to 1
    plt.tight_layout()

    # Print accuracy values next to bars
    plt.text(0, accuracy_seq, round(accuracy_seq, 2), ha='center', va='bottom', fontsize=10)
    plt.text(1, accuracy_par, round(accuracy_par, 2), ha='center', va='bottom', fontsize=10)

    # Save the accuracy plot
    plt.savefig("tests/test-output/graphs/accuracy_comparison_plot.pdf", format="pdf")
    plt.close()

if __name__ == '__main__':
    main()
