import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
df.columns = df.columns.str.strip()  # <-- IMPORTANT FIX


# Remove duplicate serial entries (keep minimum time for each thread group)
serial = df[df["mode"] == "serial"].groupby("threads")["time"].mean()
parallel = df[df["mode"] == "parallel"].groupby("threads")["time"].mean()

# --- EXECUTION TIME GRAPH ---
plt.figure()
plt.plot(serial.index, serial.values, marker='o', label="Serial")
plt.plot(parallel.index, parallel.values, marker='o', label="Parallel")
plt.xlabel("Threads")
plt.ylabel("Execution Time (sec)")
plt.title("Execution Time vs Threads")
plt.legend()
plt.grid(True)
plt.savefig("execution_time.png")

# --- SPEEDUP GRAPH ---
speedup = serial.values / parallel.values

plt.figure()
plt.plot(parallel.index, speedup, marker='o')
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs Threads")
plt.grid(True)
plt.savefig("speedup.png")

print("Graphs generated:")
print(" - execution_time.png")
print(" - speedup.png")
