
import chardet
from ftfy import fix_text

# �� ������ ������ �о�ͼ� �Ű��������� �и��� �� ����Ʈ�� �ʿ� ��� �Լ�
def read_file_to_map(file_path, sep):
    result_map = {}
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.strip().split(sep)
            if len(parts) > 1:  # �ּ��� �� ���� ��Ұ� �ִ� ��쿡�� ó��
                if '/' in parts[-1]:
                    parts[-1] = parts[-1].strip().split("/")[-1]
                key = parts[-1]  # ������ ��Ҹ� key�� ���
                value = parts[-2]  # �޸𸮸� ����
                result_map[key] = value

    return result_map
# �̰� ����Ʈ��
def read_file_to_list(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = [line.strip() for line in file]
    return lines

# ����Ű¡ ���� ����Ʈ �ΰ� ���ؼ� ũ�� �ٸ����ϰ� �������ϰ� �����ؼ� ����
def diff_same_file_make():
    # ���� ��� ����
    file1_path = 'E:/S3_test/raw_data.txt'
    file2_path = 'E:/S3_test/repacked_0903.txt'

    # �� ������ ������ ����Ʈ�� �о����
    list1 = read_file_to_map(file1_path)
    list2 = read_file_to_map(file2_path)
    
    list3 = []
    list4 = []
    list5 = []
    list6 = []

    for key, value in list1.items():
        if key in list2:
            result = "file name ["+key+"] raw data size : "+value+", repack data size : "+list2[key]
            result2 = key
            if value != list2[key]:
                list3.append(result)
                list5.append(result2)
            else:
                list4.append(result)
                list6.append(result2)
    
    write_list_to_file("E:/S3_test/result_diff.txt", list3)
    write_list_to_file("E:/S3_test/result_same.txt", list4)
    write_list_to_file("E:/S3_test/result_diff_fileName.txt", list5)
    write_list_to_file("E:/S3_test/result_same_fileName.txt", list6)

# ����Ʈ�� ���پ� ���Ͽ� ��
def write_list_to_file(file_path, items):
    with open(file_path, 'w', encoding='utf-8') as file:
        for item in items:
            file.write(f"{item}\n")  # �� �׸��� �� �ٿ� �ϳ��� ��

# ���ڵ� Ȯ��
def fileEncodingCheck(filePath):
    
    with open(filePath, 'rb') as file:
        rawdata = file.read()
        result = chardet.detect(rawdata)
        print(result['encoding'])
    return 0

# ���ڵ� Ȯ�� ftfy ���
def fileEncodingCure(filePath):
    with open(filePath, 'r', encoding='euc-kr', errors='ignore') as file:
        content = file.read()

    fixed_content = fix_text(content)
    print(fixed_content)
    return 0

