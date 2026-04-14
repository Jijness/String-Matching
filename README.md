# HƯỚNG DẪN TRIỂN KHAI BÁO CÁO CHUYÊN ĐỀ CNPM - CÁC THUẬT TOÁN ĐỐI SÁNH MẪU

## Mục tiêu
- Cài đặt và đánh giá **24 thuật toán** đối sánh mẫu kinh điển, bỏ qua Brute-Force.
- Phân nhóm chính xác theo chiến lược so sánh:
  - Nhóm 1: Từ trái sang phải
  - Nhóm 2: Từ phải sang trái
  - Nhóm 3: Từ vị trí xác định (specific order)
  - Nhóm 4: Từ vị trí bất kỳ (any order)
- Tự động hóa đo đạc: **thời gian thực thi** và **số phép so sánh ký tự**.
- Xuất kết quả dạng CSV để tổng hợp và so sánh.

---

## 1. Danh sách thuật toán chính xác theo nhóm

| Nhóm | Tên nhóm                   | Thuật toán |
|------|----------------------------|------------|
| 1    | Trái → Phải                | KMP, Morris-Pratt, Karp-Rabin, Shift-Or, DFA, Simon |
| 2    | Phải → Trái                | Boyer-Moore, Turbo-BM, Apostolico-Giancarlo, Reverse Colussi, Reverse Factor, Turbo Reverse Factor |
| 3    | Vị trí xác định            | Colussi, Galil-Giancarlo, Apostolico-Crochemore, Two Way, Galil-Seiferas, Ordered Alphabet |
| 4    | Vị trí bất kỳ             | Not So Naive, Horspool, Quick Search, Tuned BM, Smith, Raita |

## 2. Tổ chức thư mục (giữ như trước)
.
├── Datasets/
│ ├── pattern.txt # Mẫu cần tìm (mỗi dòng một mẫu, dùng mẫu đầu tiên)
│ ├── text_small.txt # Văn bản nhỏ để minh họa
│ ├── text_large.txt # Văn bản lớn để đo hiệu năng
│ └── dna_sequence.txt # (Tùy chọn) Bộ dữ liệu DNA
├── results/
│ └── summary.csv # Kết quả tổng hợp (tự động sinh)
├── scripts/
│ └── run_all.py # Script tự động chạy và thu thập kết quả
├── src/
│ ├── common/
│ │ ├── utils.h
│ │ └── utils.c
│ ├── 01_Left_to_Right/
│ ├── 02_Right_to_Left/
│ ├── 03_Specific_Order/
│ └── 04_Any_Order/
└── GUIDE.md


## 3. Hướng dẫn triển khai nhanh từng thuật toán

### 3.1. Bộ đếm và đo thời gian dùng chung
- Tạo file `utils.h` và `utils.c` trong `src/common/`.
- Cung cấp:
  - Biến toàn cục `comparisons` (đếm số lần gọi `if (a==b)`).
  - Macro `COMPARE(a,b)` để thay thế phép so sánh trực tiếp.
  - Hàm `run_once()` nhận con trỏ hàm tìm kiếm, trả về `(time_ms, comps)`.
  - Hàm `read_file()` để đọc pattern/text.

> **Mẹo:** Bạn không cần viết lại code này trong GUIDE, chỉ cần biết là có một bộ khung dùng chung.

### 3.2. Cấu trúc một file thuật toán (ví dụ `kmp.c`)
1. **Copy code từ tài liệu** vào file.
2. **Thay tất cả phép so sánh ký tự** bằng `COMPARE(x[i], y[j+i])`.
3. **Thêm comment ngắn gọn** giải thích các bước chính.
4. **Viết hàm `main`** nhận tham số dòng lệnh: `./kmp pattern.txt text.txt [số lần lặp]`.
   - Trong `main`, gọi `run_once()` và in ra màn hình dòng: `"time_ms,comparisons"`.
   - Nếu chạy nhiều lần, in ra nhiều dòng (mỗi dòng một lần chạy).

### 3.3. Chạy thủ công và ghi kết quả
- **Chạy từng thuật toán** bằng tay:
  ```bash
  ./src/group1_left_to_right/kmp data/pattern.txt data/text_large.txt 2