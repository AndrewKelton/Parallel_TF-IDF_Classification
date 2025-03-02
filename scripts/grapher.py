import matplotlib.pyplot as plt
import pandas as pd
import os

MAIN_DIR="test-output/processed-data-results"
RES_PAR=MAIN_DIR + "parallel-processed.csv"
RES_SEQ=MAIN_DIR + "sequential-processed.csv"


def cut_ms(df):
    df['Time'] = df['Time'].str.replace(' ms', '').astype(int)
    return df


def main():
    df_par = pd.read_csv(RES_PAR)
    df_seq = pd.read_csv(RES_SEQ)

    df_par = cut_ms(df_par)
    df_seq = cut_ms(df_seq)

    df_merge = pd.merge(df_seq, df_par, on="Section", suffixes=('_seq', '_par'))
    df_merge['Time_diff'] = df_merge['Time_par'] - df_merge['Time_seq']

    # Plot the time differences
    plt.figure(figsize=(10, 6))
    plt.bar(df_merged['Section'], df_merged['Time_diff'], color='skyblue')
    plt.xlabel('Sections')
    plt.ylabel('Time Difference (ms)')
    plt.title('Time Differences Between Sequential and Parallel Processing')
    plt.xticks(rotation=45, ha="right")
    plt.tight_layout()

    # Show the plot
    plt.show()


if __name__ == '__main__':
    print("THIS DIRECTORY IS ", os.getcwd())

    main()