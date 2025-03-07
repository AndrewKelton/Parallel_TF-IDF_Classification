# grapher.py

'''

       || ===== * ==== * ==== * !!!!!!!!!! * ==== * ==== * ===== ||
       || ===== * ==== * ==== * ! NOTICE ! * ==== * ==== * ===== ||
       || ===== * ==== * ==== * !!!!!!!!!! * ==== * ==== * ===== ||
       || ===== + ==== + ==== + ========== + ==== + ==== + ===== ||
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
import os

MAIN_DIR="test-output/processed-data-results/"
RES_PAR=MAIN_DIR + "parallel-processed.csv"
RES_SEQ=MAIN_DIR + "sequential-processed.csv"


def main():
    df_par = pd.read_csv(RES_PAR)
    df_seq = pd.read_csv(RES_SEQ)

    # Convert "Time (ms)" column to numeric (strip spaces, handle errors)
    df_par["Time (ms)"] = pd.to_numeric(df_par["Time (ms)"], errors="coerce")
    df_seq["Time (ms)"] = pd.to_numeric(df_seq["Time (ms)"], errors="coerce")

    # compute average for each section
    df_par = df_par.groupby("Section", as_index=False)["Time (ms)"].mean().rename(columns={"Time (ms)": "Time_par"})
    df_seq = df_seq.groupby("Section", as_index=False)["Time (ms)"].mean().rename(columns={"Time (ms)": "Time_seq"})

    df_merge = pd.merge(df_seq, df_par, on="Section")
    df_merge["Time_rat"] = df_merge["Time_seq"] / df_merge["Time_par"]

    df_merge["Section"] = df_merge["Section"].replace({
        "Vectorization": "Vectorization Time Ratio",
        "TF-IDF": "TF-IDF Time Ratio",
        "Categories": "Categories Time Ratio",
        "Accuracy": "Accuracy Ratio"
    })

    # plot
    plt.figure(figsize=(10, 6))
    plt.bar(df_merge["Section"], df_merge["Time_rat"], color="skyblue")
    plt.xlabel("Sections")
    plt.ylabel("Ratio (Sequential / Parallel)")
    plt.title("Ratio of Sequential to Parallel Processing Time of TF-IDF Vectorization Over 100 Iterations")
    plt.xticks(rotation=45, ha="right")
    plt.tight_layout()

    # save
    plt.savefig("test-output/graphs/time_differences_plot.pdf", format="pdf")
    plt.close()


if __name__ == '__main__':
    main()