import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

threads = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

def time_plt(time_mean, time_vals, i):
    colors = plt.cm.viridis(np.linspace(0, 0.85, len(threads))) 

    plt.figure(figsize=(10, 6))
    bars = plt.bar([str(t) for t in threads], time_mean, color=colors)

    # Add value labels inside the bars
    for bar, acc in zip(bars, time_mean):
        plt.text(
            bar.get_x() + bar.get_width() / 2,  # X position: center of bar
            bar.get_height() / 2,  # Y position: just below the top of the bar
            f"{acc:.2f}",  # Format to 2 decimal places
            ha='center',  # Align center
            va='top',  # Align to top
            fontsize=10,
            color='white' if acc > max(time_mean) * 0.1 else 'black',  # Contrast color
            fontweight='bold'
        )
        
    # Labels and title
    plt.xlabel("Number of Threads")
    plt.ylabel("Mean Time (minutes)")
    plt.title(f'Mean Time vs. Number of Threads w/ Dataset {i}')
    # plt.xticks(rotation=45, ha="right")  # Rotate x-axis labels for readability
    
    plt.savefig(f"tests/output/graphs/time-{i}.pdf", format="pdf")
    plt.close()


def main():
    print('starting graphing')
    x = 1
    time_mean = [] # thread 1 = [0]...
    time_vals = [[] for _ in range(len(threads))] # thread 1, accuracy 1 = [0][0]...
    i = 0
    for thread in threads:
        file_name = ''
        if thread == 1:
            file_name=f'tests/output/processed-data-results/sequential-{x}-processed.csv'
        else:
            file_name = f'tests/output/processed-data-results/parallel-{thread}-{x}-processed.csv'
        df = pd.read_csv(file_name)
        
        time_mean.append((df['Vectorization'].mean() + df['TF-IDF'].mean() + df['Categories'].mean() + df['Unknown Classification'].mean()) / 60000) 
        # print(time_mean[-1])

        for _, row in df.iterrows():
            vec = row['Vectorization']
            tf = row['TF-IDF']
            cat = row['Categories']
            unkn = row['Unknown Classification']
            time_vals[i].append((vec + tf + cat + unkn) / 60000)
        i += 1
        print(df.head())

    time_plt(time_mean, time_vals, x)


if __name__ == "__main__":
    main()