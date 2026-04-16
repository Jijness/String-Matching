# HƯỚNG DẪN TRIỂN KHAI BÁO CÁO CHUYÊN ĐỀ CNPM - CÁC THUẬT TOÁN ĐỐI SÁNH MẪU

## Mục tiêu
- Cài đặt và đánh giá **24 thuật toán** đối sánh mẫu kinh điển.
- Phân nhóm chính xác theo chiến lược so sánh:
  - Nhóm 1: Từ trái sang phải (Left-to-Right)
  - Nhóm 2: Từ phải sang trái (Right-to-Left)
  - Nhóm 3: Từ vị trí xác định (Specific Order)
  - Nhóm 4: Từ vị trí bất kỳ (Any Order)
- Tự động hóa đo đạc: **thời gian thực thi** và **số phép so sánh ký tự**.
- Xuất kết quả dạng CSV để tổng hợp và so sánh.

---

## 1. Danh sách 24 thuật toán (đã cài đặt)

### Nhóm 1: Tìm kiếm từ trái sang phải (Left to Right)
| STT | Thuật toán             | File nguồn                   |
|-----|------------------------|------------------------------|
| 1   | Knuth-Morris-Pratt     | `01_Left_to_Right/kmp.c`     |
| 2   | Morris-Pratt           | `01_Left_to_Right/morris_pratt.c` |
| 3   | Karp-Rabin             | `01_Left_to_Right/karp_rabin.c` |
| 4   | Shift-Or               | `01_Left_to_Right/shift_or.c` |
| 5   | DFA Automaton          | `01_Left_to_Right/dfa.c`     |
| 6   | Simon                  | `01_Left_to_Right/simon.c`   |

### Nhóm 2: Tìm kiếm từ phải sang trái (Right to Left)
| STT | Thuật toán             | File nguồn                   |
|-----|------------------------|------------------------------|
| 7   | Boyer-Moore            | `02_Right_to_Left/boyer_moore.c` |
| 8   | Horspool               | `02_Right_to_Left/horspool.c` |
| 9   | Quick Search           | `02_Right_to_Left/quick_search.c` |
| 10  | Tuned Boyer-Moore      | `02_Right_to_Left/tuned_bm.c` |
| 11  | Zhu-Takaoka            | `02_Right_to_Left/zhu_takaoka.c` |
| 12  | Reverse Factor         | `02_Right_to_Left/reverse_factor.c` |

### Nhóm 3: Tìm kiếm từ vị trí xác định (Specific Order)
| STT | Thuật toán                     | File nguồn                         |
|-----|--------------------------------|------------------------------------|
| 13  | Colussi                        | `03_Specific_Order/colussi.c`      |
| 14  | Galil-Giancarlo                | `03_Specific_Order/galil_giancarlo.c` |
| 15  | Apostolico-Crochemore          | `03_Specific_Order/apostolico_crochemore.c` |
| 16  | Two Way                        | `03_Specific_Order/two_way.c`      |
| 17  | Galil-Seiferas                 | `03_Specific_Order/galil_seiferas.c` |
| 18  | String Matching on Ordered Alphabet | `03_Specific_Order/ordered_alphabet.c` |

### Nhóm 4: Tìm kiếm từ vị trí bất kỳ (Any Order)
| STT | Thuật toán             | File nguồn                   |
|-----|------------------------|------------------------------|
| 19  | Not So Naive           | `04_Any_Order/not_so_naive.c` |
| 20  | Smith                  | `04_Any_Order/smith.c`       |
| 21  | Raita                  | `04_Any_Order/raita.c`       |
| 22  | Optimal Mismatch       | `04_Any_Order/optimal_mismatch.c` |
| 23  | Maximal Shift          | `04_Any_Order/maximal_shift.c` |
| 24  | Skip Search            | `04_Any_Order/skip_search.c` |

---

## 2. Cấu trúc thư mục dự án
```text
.
├── Datasets/
│ ├── pattern.txt # Mẫu GCAGAGAG
│ ├── text_small.txt # Văn bản minh họa 24 ký tự
│ └── text_large.txt # Bộ gen E. coli ~5.7 MB
├── results/
│ └── summary.csv # Kết quả tổng hợp (tự động sinh)
├── scripts/
│ ├── build.py # Biên dịch tất cả thuật toán
│ └── run_all.py # Chạy đo lường và xuất CSV
├── src/
│ ├── common/
│ │ ├── utils.h # Khung đo lường dùng chung
│ │ └── utils.c
│ ├── 01_Left_to_Right/ # Mã nguồn nhóm 1
│ ├── 02_Right_to_Left/ # Mã nguồn nhóm 2
│ ├── 03_Specific_Order/ # Mã nguồn nhóm 3
│ └── 04_Any_Order/ # Mã nguồn nhóm 4
└── README.md
```
---

## 3. Quy trình chạy thực nghiệm

### 3.1. Biên dịch tất cả thuật toán
```bash
cd D:\...\String-Matching
python scripts/build.py
```

Script sẽ duyệt các thư mục nhóm, biên dịch từng file .c với gcc -O2 và tạo file .exe tương ứng.

### 3.2. Chạy đo lường và xuất CSV
```bash
python scripts/run_all.py 2
```
* Tham số 2 là số lần lặp mỗi thuật toán (có thể thay đổi).
* Kết quả thời gian (ms) và số phép so sánh được lưu vào results/summary.csv (UTF-8 BOM).

### 3.3. Phân tích kết quả trong Excel
1. Mở summary.csv bằng Excel (nếu lỗi font, dùng Data > From Text/CSV, chọn UTF-8).
2. Tạo PivotTable để nhóm theo Nhóm, xem Min/Max của Thời gian và Số phép so sánh.
3. Dùng Conditional Formatting (Data Bars) để trực quan hóa.
4. Lọc Top/Bottom để xác định thuật toán nhanh nhất/chậm nhất toàn bộ và theo nhóm.

## 4. Ghi chú
* Mã nguồn mỗi thuật toán đều sử dụng macro COMPARE(a,b) để đếm số phép so sánh ký tự.
* Thời gian đo bằng clock() (CPU time), không bao gồm I/O đọc file.
* Với các thuật toán không dùng so sánh ký tự trực tiếp (DFA, Shift-Or, Reverse Factor), số phép so sánh bằng 0.
* Toàn bộ kết quả đã được tổng hợp trong báo cáo chính.