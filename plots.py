import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
df = pd.read_csv('stock_simulations.csv', header=None)

plt.figure(figsize=(12, 8))
df = df.transpose()

for i in range(df.shape[1]):
    plt.plot(df[i], alpha=0.5)
    # print(df[i])

plt.title('Stock Price Simulations')
plt.xlabel('Time Steps')
plt.ylabel('Stock Price')
plt.xlim(0, 110)
plt.xticks([0, 25, 50, 75, 100], ['0', '25', '50', '75', '100'])
plt.grid(True)
plt.savefig('stock_simulations_plot.png', dpi=300, bbox_inches='tight')

print("Plotted 100 sample paths and saved image as stock_simulations_plot.png")
print("*****************")


df = pd.read_csv('optionPrices_simulations.csv', header=None)

plt.figure(figsize=(12, 8))
df = df.transpose()

for i in range(min(df.shape[1],100)):
    plt.plot(df[i], alpha=0.5)
    # print(df[i])

plt.title('Option Price Simulations')
plt.xlabel('Time Steps')
plt.ylabel('Option Price')
plt.xlim(0, 110)
plt.xticks([0, 25, 50, 75, 100], ['0', '25', '50', '75', '100'])
plt.grid(True)
plt.savefig('optionPrices_simulations.png', dpi=300, bbox_inches='tight')
print("Plotted 100 sample paths and saved image as optionPrices_simulations.png")
print("*****************")


df = pd.read_csv('hedging_results.csv')

final_hedging_errors = df['FinalHedgingError']

plt.figure(figsize=(10, 6))
plt.hist(final_hedging_errors, bins=30, edgecolor='black')
plt.title('Distribution of Cumulative Hedging Errors at Time T')
plt.xlabel('Cumulative Hedging Error')
plt.ylabel('Frequency')
plt.grid(True, alpha=0.3)

mean_error = np.mean(final_hedging_errors)
plt.axvline(mean_error, color='r', linestyle='dashed', linewidth=2, label=f'Mean: {mean_error:.4f}')

plt.legend()
plt.savefig('hedging_error_distribution.png', dpi=300, bbox_inches='tight')

print("Plotted distribution of deltaa hedging errors as hedging_error_distribution.png")
print("*****************")

stats = {
    'Mean': np.mean(final_hedging_errors),
    'Median': np.median(final_hedging_errors),
    'Std Dev': np.std(final_hedging_errors),
    '5th Percentile': np.percentile(final_hedging_errors, 5),
    '95th Percentile': np.percentile(final_hedging_errors, 95)
}

print("Summary Statistics of Cumulative Hedging Errors at Time T:")
for stat, value in stats.items():
    print(f"{stat}: {value:.4f}")