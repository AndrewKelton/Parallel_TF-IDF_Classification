import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

threads = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]
# accuracy_mean = [] # thread 1 = [0]...
# accuracy_vals = [[] for _ in range(len(threads))] # thread 1, accuracy 1 = [0][0]...


def accuracy_plt(accuracy_mean, accuracy_vals, i):
    colors = plt.cm.viridis(np.linspace(0, 0.85, len(threads))) 

    plt.figure(figsize=(10, 6))
    bars = plt.bar([str(t) for t in threads], accuracy_mean, color=colors)

    # Add value labels inside the bars
    for bar, acc in zip(bars, accuracy_mean):
        plt.text(
            bar.get_x() + bar.get_width() / 2,  # X position: center of bar
            bar.get_height() / 2,  # Y position: just below the top of the bar
            f"{acc:.2f}",  # Format to 2 decimal places
            ha='center',  # Align center
            va='top',  # Align to top
            fontsize=10,
            color='white' if acc > max(accuracy_mean) * 0.1 else 'black',  # Contrast color
            fontweight='bold'
        )
        
    # Labels and title
    plt.xlabel("Number of Threads")
    plt.ylabel("Mean Accuracy")
    plt.title(f'Mean Accuracy vs. Number of Threads w/ Dataset {i}')
    # plt.xticks(rotation=45, ha="right")  # Rotate x-axis labels for readability

    plt.savefig(f"tests/test-output/graphs/accuracy-dataset-{i}.pdf", format="pdf")
    plt.close()


def precision_plt(accuracy_mean, accuracy_vals, i):
    precisions = []
    for thread_acc in accuracy_vals:
        mean = np.mean(thread_acc)
        std_dev = np.std(thread_acc, ddof=1)
        precisions.append(1 - (std_dev / mean))

    colors = plt.cm.viridis(np.linspace(0, 0.85, len(threads))) 

    plt.figure(figsize=(10, 6))
    bars = plt.bar([str(t) for t in threads], precisions, color=colors)

    # Add value labels inside the bars
    for bar, acc in zip(bars, precisions):
        plt.text(
            bar.get_x() + bar.get_width() / 2,  # X position: center of bar
            bar.get_height() / 2,  # Y position: just below the top of the bar
            f"{acc:.2f}",  # Format to 2 decimal places
            ha='center',  # Align center
            va='top',  # Align to top
            fontsize=10,
            color='white' if acc > max(precisions) * 0.1 else 'black',  # Contrast color
            fontweight='bold'
        )
        
    # Labels and title
    plt.xlabel("Number of Threads")
    plt.ylabel("Precision")
    plt.title(f"Precision vs. Number of Threads w/ Dataset {i}")
    # plt.xticks(rotation=45, ha="right")  # Rotate x-axis labels for readability

    plt.savefig(f"tests/test-output/graphs/precision-plot-d{i}.pdf", format="pdf")
    plt.close()


def main():
    x = 1
    while x <= 3:
        accuracy_mean = [] # thread 1 = [0]...
        accuracy_vals = [[] for _ in range(len(threads))] # thread 1, accuracy 1 = [0][0]...
        i = 0
        for thread in threads:
            file_name = ''
            if thread == 1:
                file_name=f'tests/test-output/processed-data-results/sequential-{x}-processed.csv'
            else:
                file_name = f'tests/test-output/processed-data-results/parallel-{thread}-{x}-processed.csv'
            df = pd.read_csv(file_name)
            accuracy_mean.append(df['Accuracy'].mean())

            for acc in df["Accuracy"]:
                accuracy_vals[i].append(acc)
            i += 1

        accuracy_plt(accuracy_mean, accuracy_vals, x)
        precision_plt(accuracy_mean, accuracy_vals, x)

        x+=1

    

if __name__ == '__main__':
    main()