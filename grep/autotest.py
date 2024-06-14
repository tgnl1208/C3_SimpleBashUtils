import subprocess
from itertools import chain, combinations

def all_combinations(any_list):
    return chain(*map(lambda x: combinations(any_list, x), range(0, len(any_list)+1)))

files = [f"grep_tests/test{i + 1}.txt" for i in range(3)]
flags = ["e", "i", "v", "c", "l", "n", "h", "s", "o"]
patterns = ["lorem", "Lorem", "LOREM", "Hello world!", "hello World!", "Hello World!", "[a-z]", "[upper]", "[lower]", "'.'"]

# генерация комбинаций флагов
flag_combinations = []
for l in range(1, 3):
    flag_combinations = flag_combinations + list(combinations(flags, l))

# генерация комбинаций до 3 файлов
files_combinations = []
for l in range(1, 4):
    files_combinations = files_combinations + list(combinations(files, l))

pass_counter = 0
fail_counter = 0
total_test = 0

# тест без флагов
for files_comb in files_combinations:
    for pattern in patterns:
        grep_command = ["grep"] + [f"{pattern}"] + list(files_comb)
        s21_grep_command = ["./s21_grep"] + [f"{pattern}"] + list(files_comb)
        res_grep = subprocess.run(grep_command, capture_output=True, text=True)
        res_s21_grep = subprocess.run(s21_grep_command, capture_output=True, text=True)

        total_test += 1
        if res_grep.stdout == res_s21_grep.stdout:
            print(f"{' '.join(s21_grep_command)} - PASS")
            pass_counter += 1
        else:
            print(f"{' '.join(s21_grep_command)} - FAIL")
            fail_counter += 1

# тест с флагами
for flags_comb in flag_combinations:
    for files_comb in files_combinations:
        for pattern in patterns:
            grep_command = ["grep"] + [f"-{''.join(flags_comb)}"] + [f"{pattern}"] + list(files_comb)
            s21_grep_command = ["./s21_grep"] + [f"-{''.join(flags_comb)}"] + [f"{pattern}"] + list(files_comb)
            res_grep = subprocess.run(grep_command, capture_output=True, text=True)
            res_s21_grep = subprocess.run(s21_grep_command, capture_output=True, text=True)

            total_test += 1
            if res_grep.stdout == res_s21_grep.stdout:
                print(f"{' '.join(s21_grep_command)} - PASS")
                pass_counter += 1
            else:
                print(f"{' '.join(s21_grep_command)} - FAIL")
                fail_counter += 1

# тест флага -f
flags = ["i", "v", "c", "l", "n", "h", "s", "o"]
pattern_files = ["grep_tests/patterns.txt"]
for flags_comb in flags:
    for files_comb in files_combinations:
        for pattern in pattern_files:
            grep_command = ["grep"] + [f"-{' -'.join(flags_comb)}"] + ["-f"] + [f"{pattern}"] + list(files_comb)
            s21_grep_command = ["./s21_grep"] + [f"-{' -'.join(flags_comb)}"] + ["-f"] + [f"{pattern}"] + list(files_comb)
            res_grep = subprocess.run(grep_command, capture_output=True, text=True)
            res_s21_grep = subprocess.run(s21_grep_command, capture_output=True, text=True)

            total_test += 1
            if res_grep.stdout == res_s21_grep.stdout:
                print(f"{' '.join(s21_grep_command)} - PASS")
                pass_counter += 1
            else:
                print(f"{' '.join(s21_grep_command)} - FAIL")
                fail_counter += 1


print(f"total tests - {total_test}\npass - {pass_counter}\nfail - {fail_counter}")
