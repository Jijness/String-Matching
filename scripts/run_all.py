import os
import subprocess
import csv
import sys

# Đường dẫn tuyệt đối đến thư mục gốc dự án
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(BASE_DIR)
PATTERN_FILE = os.path.join(ROOT_DIR, "Datasets", "pattern.txt")
TEXT_FILE = os.path.join(ROOT_DIR, "Datasets", "text_large.txt")
RESULT_FILE = os.path.join(ROOT_DIR, "results", "summary.csv")

# Ánh xạ thư mục nhóm
GROUPS = {
    "1": "01_Left_to_Right",
    "2": "02_Right_to_Left",
    "3": "03_Specific_Order",
    "4": "04_Any_Order"
}

# Ánh xạ tên file thực thi (không có .exe) sang tên hiển thị đẹp
ALGO_NAMES = {
    "kmp": "Knuth-Morris-Pratt",
    "morris_pratt": "Morris-Pratt",
    "karp_rabin": "Karp-Rabin",
    "shift_or": "Shift-Or",
    "dfa": "DFA Automaton",
    "simon": "Simon",
    # 
    "boyer_moore": "Boyer-Moore",
    "horspool": "Horspool",
    "quick_search": "Quick Search",
    "tuned_bm": "Tuned Boyer-Moore",
    "zhu_takaoka": "Zhu-Takaoka",
    "reverse_factor": "Reverse Factor",
    # 
    "colussi": "Colussi",
    "galil_giancarlo": "Galil-Giancarlo",
    "apostolico_crochemore": "Apostolico-Crochemore",
    "two_way": "Two Way",
    "galil_seiferas": "Galil-Seiferas",
    "ordered_alphabet": "Ordered Alphabet",
    # 
    "not_so_naive": "Not So Naive",
    "smith": "Smith",
    "raita": "Raita",
    "optimal_mismatch": "Optimal Mismatch",
    "maximal_shift": "Maximal Shift",
    "skip_search": "Skip Search"
}

def run_executable(exe_path, repeat=2):
    """Chạy file exe và trả về list các tuple (time_ms, comps)"""
    results = []
    for _ in range(repeat):
        try:
            proc = subprocess.run(
                [exe_path, PATTERN_FILE, TEXT_FILE, "1"],
                capture_output=True,
                text=True,
                timeout=60  # tránh treo nếu thuật toán quá chậm
            )
            if proc.returncode != 0:
                print(f"Lỗi returncode {proc.returncode}: {proc.stderr.strip()}")
                return None
            # Lấy dòng cuối cùng của output (dạng "time,comparisons")
            lines = proc.stdout.strip().splitlines()
            if not lines:
                return None
            last_line = lines[-1].strip()
            t_str, c_str = last_line.split(',')
            results.append((float(t_str), int(c_str)))
        except Exception as e:
            print(f"Lỗi khi chạy {exe_path}: {e}")
            return None
    return results

def main():
    repeat = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    print(f"Số lần lặp mỗi thuật toán: {repeat}")

    summary = []
    stt = 1

    for group_id, folder in GROUPS.items():
        group_path = os.path.join(ROOT_DIR, "src", folder)
        if not os.path.isdir(group_path):
            continue

        # Lấy danh sách file .exe trong thư mục
        exe_files = [f for f in os.listdir(group_path) if f.endswith('.exe')]
        for exe_file in exe_files:
            exe_path = os.path.join(group_path, exe_file)
            base_name = os.path.splitext(exe_file)[0]
            algo_display = ALGO_NAMES.get(base_name, base_name)

            print(f"Đang chạy {algo_display}...", end=" ", flush=True)
            res = run_executable(exe_path, repeat)
            if res is None:
                print("LỖI")
                continue

            avg_time = sum(r[0] for r in res) / len(res)
            avg_comp = sum(r[1] for r in res) / len(res)

            summary.append([
                stt,
                group_id,
                algo_display,
                f"{avg_time:.3f}",
                f"{avg_comp:.0f}"
            ])
            print(f"OK ({avg_time:.3f} ms, {avg_comp:.0f} comps)")
            stt += 1

    # Ghi kết quả ra CSV với encoding utf-8-sig (tương thích Excel)
    with open(RESULT_FILE, "w", newline="", encoding="utf-8-sig") as f:
        writer = csv.writer(f)
        writer.writerow(["STT", "Nhóm", "Thuật toán", "Thời gian (ms)", "Số phép so sánh"])
        writer.writerows(summary)

    print(f"\nĐã ghi kết quả vào {RESULT_FILE}")

if __name__ == "__main__":
    main()