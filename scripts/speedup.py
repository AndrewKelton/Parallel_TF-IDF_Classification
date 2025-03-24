import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# File paths to your processed data
MAIN_DIR = "tests/test-output/processed-data-results/"
RES_PAR = MAIN_DIR + "parallel-processed.csv"
RES_SEQ = MAIN_DIR + "sequential-processed.csv"

# Load the CSV data
df_par = pd.read_csv(RES_PAR)
df_seq = pd.read_csv(RES_SEQ)

# Reshape the data into long format where each section is a row
df_par_long = pd.melt(df_par, var_name='Section', value_name='Time_par')
df_seq_long = pd.melt(df_seq, var_name='Section', value_name='Time_seq')

# Merge the dataframes on 'Section'
df_merge = pd.merge(df_seq_long, df_par_long, on="Section")

# Calculate speedup ratio (Sequential Time / Parallel Time)
df_merge["Speedup"] = df_merge["Time_seq"] / df_merge["Time_par"]

# Group by 'Section' and calculate the average speedup
df_speedup = df_merge.groupby("Section")["Speedup"].mean().reset_index()

# Sort sections to display in a meaningful order
df_speedup = df_speedup.sort_values("Section")

# Plotting the Speedup Graph as a line plot
plt.figure(figsize=(10, 6))

# Plot the speedup as a line
plt.plot(df_speedup["Section"], df_speedup["Speedup"], marker='o', color='b', linestyle='-', label="Speedup")

# Add labels for each point
for i, txt in enumerate(df_speedup["Speedup"]):
    plt.text(df_speedup["Section"].iloc[i], txt, f'{txt:.2f}', ha='center', va='bottom', fontsize=10)

# Labels and formatting
plt.xlabel("Sections")
plt.ylabel("Speedup (Sequential / Parallel Time)")
plt.title("Speedup Comparison: Sequential vs Parallel Processing")
plt.xticks(rotation=45, ha="right")
plt.legend()

# Adjust layout
plt.tight_layout()

# Save the plot as a PDF
plt.savefig("tests/test-output/graphs/speedup_comparison_line_plot.pdf", format="pdf")
plt.show()
