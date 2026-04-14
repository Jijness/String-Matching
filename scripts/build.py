import os
import subprocess
import sys

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(ROOT_DIR, "src")
COMMON_DIR = os.path.join(SRC_DIR, "common")
UTILS_C = os.path.join(COMMON_DIR, "utils.c")

GROUPS = ["01_Left_to_Right", "02_Right_to_Left", "03_Specific_Order", "04_Any_Order"]

def compile_file(c_file, exe_file):
    cmd = ["gcc", "-O2", "-o", exe_file, c_file, UTILS_C]
    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.returncode == 0, result.stderr

def main():
    success_count = 0
    fail_count = 0

    for group in GROUPS:
        group_path = os.path.join(SRC_DIR, group)
        if not os.path.isdir(group_path):
            continue
        for file in os.listdir(group_path):
            if not file.endswith(".c"):
                continue
            c_file = os.path.join(group_path, file)
            exe_name = os.path.splitext(file)[0] + ".exe"
            exe_file = os.path.join(group_path, exe_name)

            print(f"Compiling {group}/{file}...", end=" ", flush=True)
            ok, err = compile_file(c_file, exe_file)
            if ok:
                print("OK")
                success_count += 1
            else:
                print("FAILED")
                print(err)
                fail_count += 1

    print(f"\nBuild finished: {success_count} succeeded, {fail_count} failed.")

if __name__ == "__main__":
    main()