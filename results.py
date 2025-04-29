import pandas as pd
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import glob


def process_mlfq(filename): #Process MLFQ CSVs
    df = pd.read_csv(filename)
    n_processes = df.shape[0]
    avg_waiting = df['Waiting'].mean()
    avg_turnaround = df['Turnaround'].mean()
    cpu_util = df['CPU Utilization'].iloc[0]

    total_time = df['Turnaround'].max() + df['Arrival'].min()
    throughput = n_processes / total_time if total_time > 0 else 0

    return avg_waiting, avg_turnaround, cpu_util, throughput


def process_srtf(filename): #Process SRTF CSVs
    df = pd.read_csv(filename)
    n_processes = df.shape[0]
    avg_waiting = df['Waiting'].mean()
    avg_turnaround = df['Turnaround'].mean()
    cpu_util = df['CPU Utilization'].iloc[0]

    total_time = df['Turnaround'].max() + df['Arrival'].min()
    throughput = n_processes / total_time if total_time > 0 else 0

    return avg_waiting, avg_turnaround, cpu_util, throughput



mlfq_files = sorted(glob.glob('results_MLFQ_*.csv'))
srtf_files = sorted(glob.glob('results_SRTF_*.csv'))

mlfq_results = []
srtf_results = []

for mlfq, srtf in zip(mlfq_files, srtf_files):
    mlfq_results.append(process_mlfq(mlfq))
    srtf_results.append(process_srtf(srtf))

test_cases = [f'Test {i + 1}' for i in range(len(mlfq_results))]

df_mlfq = pd.DataFrame(mlfq_results, columns=['Avg Waiting', 'Avg Turnaround', 'CPU Utilization', 'Throughput'],
                       index=test_cases)
df_srtf = pd.DataFrame(srtf_results, columns=['Avg Waiting', 'Avg Turnaround', 'CPU Utilization', 'Throughput'],
                       index=test_cases)


df_combined = pd.concat([
    df_mlfq.add_prefix('MLFQ_'),
    df_srtf.add_prefix('SRTF_')
], axis=1)

df_combined.to_csv('combined_results.csv')
print(df_combined)


fig, axes = plt.subplots(4, 1, figsize=(12, 20))

metrics = ['Avg Waiting', 'Avg Turnaround', 'CPU Utilization', 'Throughput']

for idx, metric in enumerate(metrics):
    axes[idx].plot(df_mlfq.index, df_mlfq[metric], marker='o', label='MLFQ')
    axes[idx].plot(df_srtf.index, df_srtf[metric], marker='s', label='SRTF')
    axes[idx].set_title(metric)
    axes[idx].set_ylabel(metric)
    axes[idx].legend()
    axes[idx].grid(True)

plt.tight_layout()
plt.savefig('scheduling_comparison.png')
plt.show()
