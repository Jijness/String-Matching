# clean_text.py
# Cách dùng: python clean_text.py
# Nó sẽ đọc text_large.txt, xóa hết xuống dòng, ghi đè lên chính file đó.

input_file = "text_large.txt"

with open(input_file, "r", encoding="utf-8") as f:
    content = f.read()

# Xóa tất cả ký tự xuống dòng (cả \n và \r)
cleaned = content.replace("\n", "").replace("\r", "")

with open(input_file, "w", encoding="utf-8") as f:
    f.write(cleaned)

print("Đã xóa xuống dòng trong text_large.txt.")