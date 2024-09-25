import DbConnect
import S3Util
import RePackFileUtil
from datetime import datetime


def readAndSaveBucketDiffData():
    """_summary_
    각 버킷에 있는 오브젝트들 읽어서 비교하고 파일로 저장하는 함수
    """
    rawBucketName = 'coretrust-raw-data'
    rePackBucketName = 'coretrust-repacked-data'
    testBucketName = 'coretrust-test-repacked'
    currentTime = datetime.now()
    formattedTime = currentTime.strftime("%Y_%m_%d %H_%M")
    print(formattedTime)
     
    
    rawDict = S3Util.getObjectInBucket(rawBucketName)
    rePackDict = S3Util.getObjectInBucket(rePackBucketName)
    testDict = S3Util.getObjectInBucket(testBucketName)
    
    # raw 데이터랑 repacked 데이터랑 사이즈 다른거 찾아서 새로운 dict에 넣기
    rawRepackDiffDict = {}
    for key, value in rawDict.items():
        if key in rePackDict:
            if value != rePackDict[key]:
                rawRepackDiffDict[key] = value
    
    # 새로운 dict과 test에 올라가져있는 dict 비교
    keysOnlyInTestDict = testDict.keys() - rawRepackDiffDict.keys()
    keysOnlyInRawRepackDiffDict = rawRepackDiffDict.keys() - testDict.keys()
    
    print(f"test에만 있는 key : {len(keysOnlyInTestDict)}")
    
    for i in keysOnlyInTestDict:
        print(i)
    RePackFileUtil.write_list_to_file(f'E:/S3_test/test에만 있는 파일_{formattedTime}.txt', keysOnlyInTestDict)
    print(f"diff에만 있는 key : {len(keysOnlyInRawRepackDiffDict)}")
    
    for i in keysOnlyInRawRepackDiffDict:
        print(i)
    RePackFileUtil.write_list_to_file(f'E:/S3_test/차이에만 있는 파일{formattedTime}.txt', keysOnlyInRawRepackDiffDict)
    
    
    diffSize = {k: (testDict[k], rawRepackDiffDict[k]) for k in testDict.keys() & rawRepackDiffDict.keys() if testDict[k] != rawRepackDiffDict[k]}
    print(f"값이 다른 키-값 쌍 : {len(diffSize)}")  # 출력: {'b': (2, 4)}

    tmpList = []
    for key, val in diffSize.items():
        print(key)
        tmpList.append(key)
        
    RePackFileUtil.write_list_to_file(f'E:/S3_test/사이즈가 다른 파일_{formattedTime}.txt', tmpList)

# 파일들을 비교해서 content_id를 구해야한다.
def getRepackListFile():
    file1_path = 'E:/S3_test/repack_list.txt'
    file2_path = 'E:/S3_test/사이즈가 다른 파일.txt'
    file3_path = 'E:/S3_test/차이에만 있는 파일.txt'
    dict1 = RePackFileUtil.read_file_to_map(file1_path, ',')
    list1 = RePackFileUtil.read_file_to_list(file2_path)
    list2 = RePackFileUtil.read_file_to_list(file3_path)
    print(f"dict len : {len(dict1)}, list1 len : {len(list1)}, list2 len : {len(list2)}")
    totalList = list1 + list2
    falseFile = []
    resultList = []
    
    for fileName in totalList:
        if fileName in dict1:
            resultList.append(f'{dict1[fileName]},{fileName}')
        else:
            print(f"{fileName} is Not in repack_list.txt!!!!!!")
            falseFile.append(fileName)
    
    RePackFileUtil.write_list_to_file('E:/S3_test/repack_list_0903.txt', resultList)
    RePackFileUtil.write_list_to_file('E:/S3_test/false_list_0903.txt', falseFile)

# 메인 함수
def main():  
    rawBucketName = 'coretrust-raw-data'
    rePackBucketName = 'coretrust-repacked-data'
    rawDict = S3Util.getObjectInBucket(rawBucketName)
    rePackDict = S3Util.getObjectInBucket(rePackBucketName)
    
    listDiff = []
    listRaw = []
    listRepack = []
    
    # raw에는 있지만 repacked에는 없는 것들을 구분하자.->key값으로
    for key in rawDict.keys():
        if key in rePackDict:
            listRaw.append(f'{key},{rawDict[key]}')
            listRepack.append(f'{key},{rePackDict[key]}')
        else:
            listDiff.append(key)
            
    RePackFileUtil.write_list_to_file('E:/S3_test/09051822_list_raw.txt', listRaw)
    RePackFileUtil.write_list_to_file('E:/S3_test/09051822_list_repacked.txt', listRepack)
    RePackFileUtil.write_list_to_file('E:/S3_test/09051822_list_not_in_repacked.txt', listDiff)
   
   
    # fileName = 'E:/S3_test/sdsdsadasd.txt'
    # list1 = RePackFileUtil.read_file_to_list(fileName)
    
    # list2 = DbConnect.read_from_mssql_chunks(list1)
    
    # RePackFileUtil.write_list_to_file('E:/S3_test/repack_list_0905.txt', list2)
    
    return
    
# 프로그램의 진입점
if __name__ == '__main__':
    main()