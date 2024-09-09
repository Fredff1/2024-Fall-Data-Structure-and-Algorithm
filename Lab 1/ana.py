import pandas as pd
import matplotlib.pyplot as plt

# Python 代码有AI辅助，因为我确实不是很会写python

# 切换到非交互式的 'Agg' 后端
plt.switch_backend('Agg')

# 读取CSV文件
filename = 'output_mixed.csv'
data = pd.read_csv(filename)
test_index="1"

# 定义绘图函数
def plot_performance(subset, n_value, output_dir='.', filename_pattern='performance_N_{n}.png'):
    plt.figure(figsize=(10, 6))
    plt.plot(subset['i'], subset['time'], marker='o', label=f'N={n_value}')
    
    plt.xlabel('i (Parameter in testEfficiency)')
    plt.ylabel('Time (seconds)')
    plt.title(f'Performance of testEfficiency function for N={n_value}')
    plt.legend()
    plt.grid(True)
    
    # 保存图像
    output_path = f'{output_dir}/{filename_pattern.format(n=n_value)+test_index}'
    plt.savefig(output_path)
    plt.close()  # 关闭当前图，防止重叠

# 为每个N值绘制图并保存
for n_value, subset in data.groupby('N'):
    plot_performance(subset, n_value)

print("All plots have been saved successfully.")
