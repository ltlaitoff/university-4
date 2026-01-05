import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

GENERATE_EXCEL = False

data = pd.read_csv("data.csv", sep=";", decimal=',')
A_LIMIT = 0.28
N = len(data)

print("Min:", data['U'].min(), "Max:", data['U'].max())

thresholds = np.linspace(data['U'].min(), data['U'].max(), 100)
results = []

for u_kl in thresholds:
    k1_k1 = len(data[(data['U'] <= u_kl) & (data['a'] <= A_LIMIT)])
    k2_k2 = len(data[(data['U'] > u_kl) & (data['a'] > A_LIMIT)])
    k1_k2 = len(data[(data['U'] > u_kl) & (data['a'] <= A_LIMIT)]) # Producer risk count
    k2_k1 = len(data[(data['U'] <= u_kl) & (data['a'] > A_LIMIT)]) # Consumer risk count

    n_res_k1 = k1_k1 + k2_k1
    n_res_k2 = k2_k2 + k1_k2

    p_correct = (k1_k1 + k2_k2) / N
    p_error = (k1_k2 + k2_k1) / N

    p_k2_res_k1  = k2_k1 / n_res_k1 if n_res_k1 > 0 else 0
    p_k1_res_k2 = k1_k2 / n_res_k2 if n_res_k2 > 0 else 0
    p_res_k1_k2 = k2_k1 / len(data[data['a'] > A_LIMIT])
    p_res_k2_k1 = k1_k2 / len(data[data['a'] <= A_LIMIT])

    p_res_k1 = n_res_k1 / N
    p_res_k2 = n_res_k2 / N

    results.append({
        'U_kl': u_kl,
        'n(K1/K1)': k1_k1, 'n(K2/K2)': k2_k2, 'n(K1/K2)': k1_k2, 'n(K2/K1)': k2_k1,
        'n(resK1)': n_res_k1, 'n(resK2)': n_res_k2,
        'P_correct': p_correct, 'P_error': p_error,
        'P(K2/resK1)': p_k2_res_k1, 'P(K1/resK2)': p_k1_res_k2,
        'P(resK1/K2)': p_res_k1_k2, 'P(resK2/K1)': p_res_k2_k1,
        'P(resK1)': p_res_k1, 'P(resK2)': p_res_k2
    })

res_df = pd.DataFrame(results)


# Table
pd.set_option('display.max_columns', None)
pd.set_option('display.width', 1000)
table_transposed = res_df.set_index('U_kl').T.round(2)
print(table_transposed)

if GENERATE_EXCEL:
    table_transposed.to_excel("lb1_table.xlsx")


# Optimal
best = res_df.loc[res_df['P_correct'].idxmax()]
print(f"\nOptimal threshold: {best['U_kl']:.2f}, Max Accuracy: {best['P_correct']:.4f}")


# Visualization
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 12))

ax1.plot(res_df['U_kl'], res_df['P_correct'], label='P correct', color='green', lw=2)
ax1.plot(res_df['U_kl'], res_df['P_error'], label='P error', color='red', lw=2)
ax1.axvline(x=best['U_kl'], color='blue', ls='--', label=f'Optimum: {best["U_kl"]:.2f}')
ax1.set_title('Main Decision Probabilities')
ax1.legend()
ax1.grid(True)

ax2.plot(res_df['U_kl'], res_df['P(K2/resK1)'], label='Consumer Risk P(K2/resK1)', color='orange')
ax2.plot(res_df['U_kl'], res_df['P(K1/resK2)'], label='Producer Risk P(K1/resK2)', color='purple')
ax2.set_title('Risk Probabilities')
ax2.set_xlabel('U_threshold')
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.show()
