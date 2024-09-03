from dbconnect import *
from s3UpDown import *
from rePackFileUtil import *




# 메인 함수
def main():    
    file1_path = 'E:/S3_test/result_diff_fileName.txt'
    # list1 = read_file_to_list(file1_path)
    # list3 = read_from_mssql_chunks(server, database, username, password, query, list1)
    # data = read_from_mssql(server, database, username, password, query)
    # list1 = read_file_to_map()
    # list3 = []
    # for row in data:
    #     #type = row.type
    #     fileName = row.name
    #     contentID = row.cId
    #     if fileName in list1:
    #         result2 = ""+contentID + ","+fileName
    #         list3.append(result2)
    # print(len(list3))
    # write_list_to_file("E:/S3_test/repack_diff_list.csv", list3)
    diff_same_file_make()


# 프로그램의 진입점
if __name__ == '__main__':
    main()