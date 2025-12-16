import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("./vote-csv.csv", na_values=["?"])
data = data.replace({'y': True, 'n': False})

print(data.dtypes)
print("\n -- Data is:\n")
print(data)

print("\n -- Data info is:\n")
print(data.info())

print("\n -- Data of handicapped-infants is:\n")
print(data["handicapped-infants"])

print("\n -- Data of 1 row is:\n")
print(data.loc[1])

print("\n -- Sum of null fields:\n")
print(data.isnull().sum())

data = data.fillna(data.groupby("Class").transform(lambda x: x.mode()[0]))

for col in data.columns.drop("Class"):
    data[col] = data[col].astype("bool")
data["Class"] = data["Class"].astype("category")

print("\n -- Sum of null fields:\n")
print(data.isnull().sum())
print(data)
print(data.dtypes)

data["Class"].value_counts().plot(kind = "bar", figsize=(5, 5))
plt.show()

fig, axes = plt.subplots(nrows=4, ncols=4, figsize=(20, 20))
xa=0
ya=0

for col in data.columns.drop("Class"):
    plt.title(col)
    data[col].value_counts().plot(
        kind='bar',
        color=['green', 'red'],
        ax=axes[xa, ya],
    )

    for i, count in enumerate(data[col].value_counts()):
        axes[xa, ya].text(
            i,
            count + 0.1,
            str(count),
            ha='center',
            va='bottom',
            fontsize=9,
            color='black'
        )

    if xa < 3:
        xa += 2
    else:
        ya += 1
        xa=0

plt.tight_layout()
plt.show()

save = [
        "adoption-of-the-budget-resolution",
        "physician-fee-freeze",
        "immigration",
        "synfuels-corporation-cutback",
        "duty-free-exports",
    ]

data = data.replace({True: "y", False: "n"})
data.to_csv("./vote-python-raw.csv", index=False)

for col in data.columns.drop("Class"):
    if (col not in save):
        data = data.drop([col], axis=1)

data.to_csv("./vote-python.csv", index=False)
