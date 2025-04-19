import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
from datetime import datetime
import seaborn as sns
from sklearn.preprocessing import StandardScaler

class DistanceAnalyzer:
    def __init__(self, excel_file):
        print(f"Đang đọc dữ liệu từ file {excel_file}...")
        self.df = pd.read_excel(excel_file)
        self.df['Thời gian'] = pd.to_datetime(self.df['Thời gian'])
        print(f"Đã đọc {len(self.df)} mẫu dữ liệu")
        
        # Tính toán các thông số thống kê cơ bản
        self.calculate_statistics()
        
        # Phát hiện bất thường
        self.detect_anomalies()
        
        # Tạo thư mục để lưu kết quả phân tích
        self.timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        
    def calculate_statistics(self):
        """Tính toán các thông số thống kê"""
        self.stats = {
            'Số lượng mẫu': len(self.df),
            'Giá trị nhỏ nhất': self.df['Khoảng cách (mm)'].min(),
            'Giá trị lớn nhất': self.df['Khoảng cách (mm)'].max(),
            'Giá trị trung bình': self.df['Khoảng cách (mm)'].mean(),
            'Độ lệch chuẩn': self.df['Khoảng cách (mm)'].std(),
            'Trung vị': self.df['Khoảng cách (mm)'].median()
        }
        
        # Tính độ biến thiên
        self.df['Độ biến thiên'] = self.df['Khoảng cách (mm)'].diff()
        
    def detect_anomalies(self):
        """Phát hiện các điểm bất thường bằng nhiều phương pháp"""
        # 1. Phát hiện bằng Z-score
        z_scores = np.abs(stats.zscore(self.df['Khoảng cách (mm)']))
        self.df['Z-score anomaly'] = z_scores > 3
        
        # 2. Phát hiện bằng IQR
        Q1 = self.df['Khoảng cách (mm)'].quantile(0.25)
        Q3 = self.df['Khoảng cách (mm)'].quantile(0.75)
        IQR = Q3 - Q1
        self.df['IQR anomaly'] = (self.df['Khoảng cách (mm)'] < (Q1 - 1.5 * IQR)) | (self.df['Khoảng cách (mm)'] > (Q3 + 1.5 * IQR))
        
        # 3. Phát hiện bằng độ biến thiên đột ngột
        threshold = self.df['Độ biến thiên'].std() * 3
        self.df['Variation anomaly'] = abs(self.df['Độ biến thiên']) > threshold
        
        # Tổng hợp kết quả
        self.df['Is Anomaly'] = self.df['Z-score anomaly'] | self.df['IQR anomaly'] | self.df['Variation anomaly']
        
    def plot_analysis(self):
        """Vẽ các biểu đồ phân tích"""
        # Thiết lập font hỗ trợ tiếng Việt
        plt.rcParams['font.family'] = 'Arial Unicode MS'
        
        # Tạo figure với kích thước lớn
        plt.figure(figsize=(15, 25))
        
        # 1. Biểu đồ khoảng cách theo thời gian
        plt.subplot(5, 1, 1)
        plt.plot(self.df['Thời gian'], self.df['Khoảng cách (mm)'], 'b-', label='Distance')
        plt.scatter(self.df[self.df['Is Anomaly']]['Thời gian'], 
                   self.df[self.df['Is Anomaly']]['Khoảng cách (mm)'],
                   color='red', label='Anomaly')
        plt.title('Distance vs Time')
        plt.xlabel('Time')
        plt.ylabel('Distance (mm)')
        plt.legend()
        plt.grid(True)
        
        # 2. Biểu đồ phân phối
        plt.subplot(5, 1, 2)
        sns.histplot(data=self.df, x='Khoảng cách (mm)', bins=50)
        plt.axvline(self.stats['Giá trị trung bình'], color='r', linestyle='--', label='Mean')
        plt.axvline(self.stats['Trung vị'], color='g', linestyle='--', label='Median')
        plt.title('Distance Distribution')
        plt.xlabel('Distance (mm)')
        plt.ylabel('Frequency')
        plt.legend()
        
        # 3. Biểu đồ độ biến thiên
        plt.subplot(5, 1, 3)
        plt.plot(self.df['Thời gian'], self.df['Độ biến thiên'], 'g-')
        plt.title('Distance Variation')
        plt.xlabel('Time')
        plt.ylabel('Variation (mm)')
        plt.grid(True)
        
        # 4. Box plot
        plt.subplot(5, 1, 4)
        sns.boxplot(y=self.df['Khoảng cách (mm)'])
        plt.title('Distance Box Plot')
        plt.ylabel('Distance (mm)')

        # 5. Thêm thông tin thống kê
        plt.subplot(5, 1, 5)
        plt.axis('off')
        
        # Tính toán thông tin thống kê
        total_anomalies = self.df['Is Anomaly'].sum()
        anomaly_percentage = (total_anomalies / len(self.df)) * 100
        
        # Tính xu hướng
        x = np.arange(len(self.df))
        slope, _, _, _, _ = stats.linregress(x, self.df['Khoảng cách (mm)'])
        if abs(slope) < 0.01:
            trend = "stable"
        elif slope > 0:
            trend = "increasing"
        else:
            trend = "decreasing"

        # Tạo text thống kê bằng tiếng Anh
        stats_text = f"""
STATISTICAL ANALYSIS:
===================
Basic Statistics:
---------------
• Number of samples: {self.stats['Số lượng mẫu']:.0f}
• Minimum value: {self.stats['Giá trị nhỏ nhất']:.2f} mm
• Maximum value: {self.stats['Giá trị lớn nhất']:.2f} mm
• Mean value: {self.stats['Giá trị trung bình']:.2f} mm
• Standard deviation: {self.stats['Độ lệch chuẩn']:.2f}
• Median value: {self.stats['Trung vị']:.2f} mm

Anomaly Analysis:
--------------
• Total anomalies: {total_anomalies}
• Anomaly rate: {anomaly_percentage:.2f}%
• Z-score anomalies: {self.df['Z-score anomaly'].sum()}
• IQR anomalies: {self.df['IQR anomaly'].sum()}
• Variation anomalies: {self.df['Variation anomaly'].sum()}

Trend: {trend}
        """
        
        plt.text(0.05, 0.95, stats_text, 
                transform=plt.gca().transAxes,
                fontsize=10,
                verticalalignment='top',
                family='monospace')
        
        # Lưu biểu đồ
        plt.tight_layout()
        plot_filename = f'distance_analysis_{self.timestamp}.png'
        plt.savefig(plot_filename, dpi=300, bbox_inches='tight')
        print(f"Saved plot to {plot_filename}")
        
    def generate_report(self):
        """Tạo báo cáo phân tích"""
        # Thống kê cơ bản
        print("\n=== THỐNG KÊ CƠ BẢN ===")
        for key, value in self.stats.items():
            print(f"{key}: {value:.2f}")
            
        # Phân tích bất thường
        total_anomalies = self.df['Is Anomaly'].sum()
        anomaly_percentage = (total_anomalies / len(self.df)) * 100
        
        print("\n=== PHÂN TÍCH BẤT THƯỜNG ===")
        print(f"Tổng số điểm bất thường: {total_anomalies}")
        print(f"Tỷ lệ bất thường: {anomaly_percentage:.2f}%")
        
        # Phân tích chi tiết từng loại bất thường
        print("\nChi tiết các loại bất thường:")
        print(f"- Bất thường theo Z-score: {self.df['Z-score anomaly'].sum()}")
        print(f"- Bất thường theo IQR: {self.df['IQR anomaly'].sum()}")
        print(f"- Bất thường theo độ biến thiên: {self.df['Variation anomaly'].sum()}")
        
        # Phân tích xu hướng
        print("\n=== PHÂN TÍCH XU HƯỚNG ===")
        # Tính độ dốc của đường trend
        x = np.arange(len(self.df))
        slope, _, _, _, _ = stats.linregress(x, self.df['Khoảng cách (mm)'])
        
        if abs(slope) < 0.01:
            trend = "ổn định"
        elif slope > 0:
            trend = "tăng dần"
        else:
            trend = "giảm dần"
            
        print(f"Xu hướng chung: {trend}")
        
        # Lưu báo cáo vào file
        report_filename = f'analysis_report_{self.timestamp}.txt'
        with open(report_filename, 'w', encoding='utf-8') as f:
            f.write("BÁO CÁO PHÂN TÍCH KHOẢNG CÁCH\n")
            f.write(f"Thời gian phân tích: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
            
            f.write("=== THỐNG KÊ CƠ BẢN ===\n")
            for key, value in self.stats.items():
                f.write(f"{key}: {value:.2f}\n")
            
            f.write("\n=== PHÂN TÍCH BẤT THƯỜNG ===\n")
            f.write(f"Tổng số điểm bất thường: {total_anomalies}\n")
            f.write(f"Tỷ lệ bất thường: {anomaly_percentage:.2f}%\n")
            
            f.write("\nChi tiết các loại bất thường:\n")
            f.write(f"- Bất thường theo Z-score: {self.df['Z-score anomaly'].sum()}\n")
            f.write(f"- Bất thường theo IQR: {self.df['IQR anomaly'].sum()}\n")
            f.write(f"- Bất thường theo độ biến thiên: {self.df['Variation anomaly'].sum()}\n")
            
            f.write("\n=== PHÂN TÍCH XU HƯỚNG ===\n")
            f.write(f"Xu hướng chung: {trend}\n")
            
        print(f"\nĐã lưu báo cáo chi tiết vào file {report_filename}")

def main():
    import glob
    # Tìm file Excel mới nhất trong thư mục
    excel_files = glob.glob('distance_data_20250416_173050.xlsx')
    if not excel_files:
        print("Không tìm thấy file Excel chứa dữ liệu khoảng cách!")
        return
    
    latest_file = max(excel_files, key=os.path.getctime)
    print(f"Đang phân tích file mới nhất: {latest_file}")
    
    # Tạo đối tượng phân tích
    analyzer = DistanceAnalyzer(latest_file)
    
    # Vẽ biểu đồ
    analyzer.plot_analysis()
    
    # Tạo báo cáo
    analyzer.generate_report()

if __name__ == "__main__":
    import os
    main() 