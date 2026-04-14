import os
import subprocess
import csv
import sys

# Đường dẫn tương đối
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(BASE_DIR)
PATTERN_FILE = os.path.join(ROOT_DIR, "Datasets", "pattern.txt")
TEXT_FILE = os.path.join(ROOT_DIR, "Datasets", "text_large.txt")
RESULT_FILE = os.path.join(ROOT_DIR, "results", "summary.csv")

GROUPS = {
    "1": "01_Left_to_Right",
    "2": "02_Right_to_Left",
    "3": "03_Specific_Order",
    "4": "04_Any_Order"
}

ALGO_NAMES = {
    "kmp": "Knuth-Morris-Pratt",
    "morris_pratt": "Morris-Pratt",
    "karp_rabin": "Karp-Rabin",
    "shift_or": "Shift-Or",
    "dfa": "DFA Automaton",
    "simon": "Simon",
    "boyer_moore": "Boyer-Moore",
    "turbo_bm": "Turbo-BM",
    "apostolico_giancarlo": "Apostolico-Giancarlo",
    "reverse_colussi": "Reverse Colussi",
    "reverse_factor": "Reverse Factor",
    "turbo_reverse_factor": "Turbo Reverse Factor",
    "colussi": "Colussi",
    "galil_giancarlo": "Galil-Giancarlo",
    "apostolico_crochemore": "Apostolico-Crochemore",
    "two_way": "Two Way",
    "galil_seiferas": "Galil-Seiferas",
    "ordered_alphabet": "Ordered Alphabet",
    "not_so_naive": "Not So Naive",
    "horspool": "Horspool",
    "quick_search": "Quick Search",
    "tuned_bm": "Tuned Boyer-Moore",
    "smith": "Smith",
    "raita": "Raita"
}

def run_executable(exe_path, repeat=2):
    results = []
    for _ in range(repeat):
        proc = subprocess.run([exe_path, PATTERN_FILE, TEXT_FILE, "1"],
                              capture_output=True, text=True)
        if proc.returncode != 0:
            print(f"Lỗi khi chạy {exe_path}: {proc.stderr}")
            return None
        line = proc.stdout.strip().split('\n')[-1]
        try:
            t, c = line.split(',')
            results.append((float(t), int(c)))
        except:
            return None
    return results

def main():
    repeat = int(sys.argv[1]) if len(sys.argv) > 1 else 2
    print(f"Số lần lặp mỗi thuật toán: {repeat}")
    summary = []
    for group_id, folder in GROUPS.items():
        group_path = os.path.join(ROOT_DIR, "src", folder)
        if not os.path.isdir(group_path):
            continue
        for exe_name in os.listdir(group_path):
            exe_path = os.path.join(group_path, exe_name)
            if not os.access(exe_path, os.X_OK):
                continue
            base = os.path.splitext(exe_name)[0]
            algo_display = ALGO_NAMES.get(base, base)
            print(f"Đang chạy {algo_display}...", end=" ", flush=True)
            res = run_executable(exe_path, repeat)
            if res is None:
                print("LỖI")
                continue
            avg_time = sum(r[0] for r in res) / len(res)
            avg_comp = sum(r[1] for r in res) / len(res)
            summary.append([group_id, algo_display, f"{avg_time:.3f}", f"{avg_comp:.0f}"])
            print(f"OK ({avg_time:.3f} ms, {avg_comp:.0f} comps)")

    with open(RESULT_FILE, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["Nhóm", "Thuật toán", "Thời gian (ms)", "Số phép so sánh"])
        writer.writerows(summary)
    print(f"\nKết quả đã được ghi vào {RESULT_FILE}")

if __name__ == "__main__":
    main()