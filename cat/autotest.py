import subprocess
from itertools import chain, combinations

def all_combinations(any_list):
    return chain(*map(lambda x: combinations(any_list, x), range(0, len(any_list)+1)))

files = [f"cat_tests/test{i + 1}.txt" for i in range(5)]
flags = ["b", "e", "n", "s", "t", "v", "T", "E", "-number-nonblank", "-number", "-squeeze-blank"]

# генерация комбинаций до 3 флагов
flag_combinations = []
for l in range(1, 4):
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
    cat_command = ["cat"] + list(files_comb)
    s21_cat_command = ["./s21_cat"] + list(files_comb)
    res_cat = subprocess.run(cat_command, capture_output=True, text=True)
    res_s21_cat = subprocess.run(s21_cat_command, capture_output=True, text=True)

    total_test += 1
    if res_cat.stdout == res_s21_cat.stdout:
        print(f"{' '.join(s21_cat_command)} - PASS")
        pass_counter += 1
    else:
        print(f"{' '.join(s21_cat_command)} - FAIL")
        fail_counter += 1

# тест с флагами
for flags_comb in flag_combinations:
    for files_comb in files_combinations:
        cat_command = ["cat"] + [f"-{' -'.join(flags_comb)}"] + list(files_comb)
        s21_cat_command = ["./s21_cat"] + [f"-{' -'.join(flags_comb)}"] + list(files_comb)
        res_cat = subprocess.run(cat_command, capture_output=True, text=True)
        res_s21_cat = subprocess.run(s21_cat_command, capture_output=True, text=True)

        total_test += 1
        if res_cat.stdout == res_s21_cat.stdout:
            print(f"{' '.join(s21_cat_command)} - PASS")
            pass_counter += 1
        else:
            print(f"{' '.join(s21_cat_command)} - FAIL")
            fail_counter += 1

print(f"total tests - {total_test}\npass - {pass_counter}\nfail - {fail_counter}")