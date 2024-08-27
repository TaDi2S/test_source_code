from dbconnect import *
from s3UpDown import *
import chardet

# 두 파일의 내용을 읽어와서 공백으로 분리한 후 리스트에 담는 함수
def read_file_to_map(file_path):
    result_map = {}
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.strip().split(" ")
            if len(parts) > 1:  # 최소한 두 개의 요소가 있는 경우에만 처리
                if '/' in parts[-1]:
                    parts[-1] = parts[-1].strip().split("/")[-1]
                key = parts[-1]  # 마지막 요소를 key로 사용
                value = parts[-2]  # 메모리만 넣자
                result_map[key] = value

    return result_map

def read_file_to_list(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = [line.strip() for line in file]
    print("=============================================================================")
    print(len(lines))
    return lines

def diff_same_file_make():
    # 파일 경로 설정
    file1_path = 'E:/S3_test/raw_data.txt'
    file2_path = 'E:/S3_test/test_repacked_data.txt'

    # 두 파일의 내용을 리스트로 읽어오기
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

# 인코딩 확인
def fileEncodingCheck():
    filePath = 'D:/source/work/git/CARMPackager_LGU_repack/Content Packaging Server/Content Packaging Server 0.01_vs2008/H265MPEG2TSPackagerHashParse.cpp'
    with open(filePath, 'rb') as file:
        rawdata = file.read()
        result = chardet.detect(rawdata)
        print(result['encoding'])
    return 0

# 메인 함수
def main():
    

    return 0
    
    file1_path = 'E:/S3_test/result_diff_fileName.txt'
    list1 = read_file_to_list(file1_path)
    list3 = read_from_mssql_chunks(server, database, username, password, query, list1)
    # data = read_from_mssql(server, database, username, password, query)
    
    # list3 = []
    # for row in data:
    #     #type = row.type
    #     fileName = row.name
    #     contentID = row.cId
    #     if fileName in list1:
    #         result2 = ""+contentID + ","+fileName
    #         list3.append(result2)
    print(len(list3))
    write_list_to_file("E:/S3_test/repack_diff_list.csv", list3)
            
   
    

def write_list_to_file(file_path, items):
    with open(file_path, 'w', encoding='utf-8') as file:
        for item in items:
            file.write(f"{item}\n")  # 각 항목을 한 줄에 하나씩 씀

# 프로그램의 진입점
if __name__ == '__main__':
    main()