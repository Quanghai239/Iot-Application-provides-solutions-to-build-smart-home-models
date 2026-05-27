# 🏡 Ứng Dụng IoT Cung Cấp Giải Pháp Xây Dựng Mô Hình Nhà Thông Minh

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)]()
[![App](https://img.shields.io/badge/App-Android_Studio-green.svg)]()
[![Database](https://img.shields.io/badge/Database-Firebase-orange.svg)]()
[![AI](https://img.shields.io/badge/AI-Machine_Learning-yellow.svg)]()

Dự án nghiên cứu và phát triển mô hình nhà thông minh thu nhỏ, hoạt động theo thời gian thực. Hệ thống tích hợp các giải pháp tự động hóa đa dạng bao gồm điều khiển thiết bị, cảnh báo an toàn, tối ưu năng lượng và đặc biệt tích hợp **Machine Learning** để dự báo thời tiết ngay trên vi điều khiển nhúng.

## 📑 Mục Lục
- [Giới Thiệu Tổng Quan](#-giới-thiệu-tổng-quan)
- [Tính Năng Nổi Bật](#-tính-năng-nổi-bật)
- [Cấu Trúc Hệ Thống](#-cấu-trúc-hệ-thống)
- [Phần Cứng & Phần Mềm](#-phần-cứng--phần-mềm)
- [Ứng Dụng Machine Learning](#-ứng-dụng-machine-learning)
- [Cấu Trúc Thư Mục](#-cấu-trúc-thư-mục)
- [Hướng Dẫn Cài Đặt](#-hướng-dẫn-cài-đặt)
- [Tác Giả](#-tác-giả)

## 📖 Giới Thiệu Tổng Quan
Hệ thống sử dụng **ESP32** làm trung tâm xử lý, kết nối với mạng lưới cảm biến đa dạng (chuyển động, ánh sáng, khí gas, lửa, nhiệt độ, độ ẩm, mực nước). Người dùng có thể giám sát và điều khiển toàn bộ ngôi nhà từ xa thông qua ứng dụng **Android** kết nối với **Firebase Realtime Database**, hoặc điều khiển bằng giọng nói qua **Google Assistant** (thông qua IFTTT).

## ✨ Tính Năng Nổi Bật
1. **💡 Chiếu sáng thông minh & Tiết kiệm năng lượng:** Tự động điều chỉnh độ sáng đèn LED (PWM) theo ánh sáng môi trường và cảm biến chuyển động. Hỗ trợ các ngữ cảnh (ban ngày, ban đêm, vắng nhà).
2. **🔥 Báo động an toàn thời gian thực:** Phát hiện lửa và rò rỉ khí gas (MQ-2), lập tức kích hoạt còi/đèn báo và gửi cảnh báo về điện thoại.
3. **📱 Điều khiển từ xa & Giám sát Camera:** Điều khiển thiết bị qua App Android, giám sát hình ảnh trực tiếp qua ESP32-CAM.
4. **🎙️ Điều khiển bằng giọng nói:** Tích hợp Google Assistant qua Webhook IFTTT.
5. **🤖 AI Dự báo thời tiết:** Tích hợp mô hình Cây quyết định (Decision Tree) trên ESP32 để phân tích dữ liệu nhiệt độ/độ ẩm, từ đó dự đoán và hiển thị thời tiết (Foggy, Rainy, Cloudy, Sunny) lên màn hình LCD.

## ⚙️ Phần Cứng & Phần Mềm

### 🛠 Phần Cứng
- Vi điều khiển trung tâm: **ESP-WROOM-32**
- Camera giám sát: **ESP32-CAM**
- Cảm biến: PIR SR505 (Chuyển động), LDR (Ánh sáng), MQ-2 (Khí Gas), Flame Sensor (Lửa), DHT11 (Nhiệt độ/Độ ẩm), Cảm biến mực nước.
- Hiển thị & Cơ cấu chấp hành: Màn hình LCD I2C 16x2, Relay, LED RGB, Đèn LED.

### 💻 Phần Mềm & Công Nghệ
- **Firmware:** C/C++ lập trình trên **PlatformIO** (VS Code).
- **Mobile App:** Java/XML lập trình trên **Android Studio**.
- **Database:** **Firebase Realtime Database**.
- **AI/ML:** Python (Huấn luyện mô hình), C++ Header (Triển khai trên ESP32).
- **Voice Control:** IFTTT & Google Assistant.

## 🧠 Ứng Dụng Machine Learning
Dự án thu thập bộ dữ liệu thời tiết, tính toán chỉ số **Heat Index** và huấn luyện mô hình phân loại mạng nơ-ron (ANN) cùng Cây quyết định (Decision Tree). 
Mô hình sau khi huấn luyện được trích xuất thành file header C++ (`DecisionTree.h`) và nhúng trực tiếp vào ESP32 để dự báo thời tiết *offline* dựa trên dữ liệu cảm biến DHT11. Độ chính xác dự báo đạt 87.5%.

## 📂 Cấu Trúc Thư Mục (Đề xuất)
```text
├── ESP32_Firmware/         # Mã nguồn C/C++ cho ESP32 (PlatformIO)
│   ├── include/            # Chứa thư viện và file DecisionTree.h
│   ├── src/                # File main.cpp xử lý đa luồng (phòng khách, bếp, ngủ)
│   └── platformio.ini      # Cấu hình môi trường PlatformIO
├── Android_App/            # Mã nguồn ứng dụng di động (Android Studio)
│   ├── app/src/main/java   # Logic điều khiển, kết nối Firebase
│   └── app/src/main/res    # Giao diện XML (Login, Dashboard, RGB, Camera)
├── Machine_Learning/       # Dữ liệu và mã nguồn huấn luyện mô hình AI
│   ├── dataset.csv         # Bộ dữ liệu thời tiết
│   └── train_model.ipynb   # Jupyter Notebook huấn luyện Decision Tree
└── Docs/                   # Sơ đồ khối, lưu đồ thuật toán và hình ảnh mô hình
