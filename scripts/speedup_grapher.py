import pandas as pd
import matplotlib.pyplot as plt

# Number of threads
threads = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

i = 1
while i <= 3:
    vectorization_mean = []
    tfidf_mean = []
    categories_mean = []
    unknown_classification_mean = []
    # Read data from each CSV file and calculate the mean of the required columns
    for thread in threads:
        file_name = ''
        if thread == 1:
            file_name=f'tests/test-output/processed-data-results/sequential-{i}-processed.csv'
        else:
            file_name = f'tests/test-output/processed-data-results/parallel-{thread}-{i}-processed.csv'
        df = pd.read_csv(file_name)
        vectorization_mean.append(df['Vectorization'].mean())
        tfidf_mean.append(df['TF-IDF'].mean())
        categories_mean.append(df['Categories'].mean())
        unknown_classification_mean.append(df['Unknown Classification'].mean())

    # Calculate speedup for each metric
    base_vectorization = vectorization_mean[0]
    base_tfidf = tfidf_mean[0]
    base_categories = categories_mean[0]
    base_unknown_classification = unknown_classification_mean[0]

    speedup_vectorization = [base_vectorization / v for v in vectorization_mean]
    speedup_tfidf = [base_tfidf / t for t in tfidf_mean]
    speedup_categories = [base_categories / c for c in categories_mean]
    speedup_unknown_classification = [base_unknown_classification / u for u in unknown_classification_mean]

    # Plot the speedup graph
    plt.figure(figsize=(10, 6))
    plt.plot(threads, speedup_vectorization, label='Vectorization')
    plt.plot(threads, speedup_tfidf, label='TF-IDF')
    plt.plot(threads, speedup_categories, label='Categories')
    plt.plot(threads, speedup_unknown_classification, label='Unknown Classification')

    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup')
    plt.title('Speedup Graph for Dataset-3')
    plt.legend()
    plt.xscale('log', base=2)
    plt.xticks(threads, [f'2^{i}' for i in range(11)])
    plt.grid(True)
    # plt.tight_layout()

    # Save the plot as a PDF
    plt.savefig("tests/test-output/graphs/speedup-dataset-" + str(i) + ".pdf", format="pdf")
    plt.show()

    i+=1