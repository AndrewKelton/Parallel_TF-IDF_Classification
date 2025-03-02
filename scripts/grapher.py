# grapher.py

'''

        *-------*------*------*!!!!!!!!!!!!*------*------*-------*
        *-------*------*------*!! NOTICE !!*------*------*-------*
        *-------*------*------*!!!!!!!!!!!!*------*------*-------*
        +--------------------------------------------------------+
        +       This python file is only used to graph           +
        +      the ratio of time between sequential and          +
        +     parallel tf-idf versions. It has no impact         +
        +  on the algorithm itself. All code that implements     +
        +       the tf-idf algorithm is written in c++.          +
        +--------------------------------------------------------+

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

    df_merge = pd.merge(df_seq, df_par, on="Section", suffixes=('_seq', '_par'))
    df_merge['Time_rat'] = df_merge['Time_seq'] / df_merge['Time_par']

    df_merge['Section'] = df_merge['Section'].replace({
        "Vectorization": "Vectorization Ratio",
        "TF-IDF": "TF-IDF Ratio",
        "Categories": "Categories Ratio"
    })

    # Plot the time differences
    plt.figure(figsize=(10, 6))
    plt.bar(df_merge['Section'], df_merge['Time_rat'], color='skyblue')
    plt.xlabel('Sections')
    plt.ylabel('Time Ratio (Sequential / Parallel)')
    plt.title('Ratio of Sequential to Parallel Processing Time of TF-IDF Vectorization Over 100 Iterations')
    plt.xticks(rotation=45, ha="right")
    plt.tight_layout()

    # Show the plot
    plt.savefig('test-output/graphs/time_differences_plot.pdf', format='pdf')

    plt.close()


if __name__ == '__main__':
    main()