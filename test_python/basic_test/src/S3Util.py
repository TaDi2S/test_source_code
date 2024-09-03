import boto3

def getbucketList():
    """_summary_
        버킷리스트를 가져와서 리턴하는 함수
        resource 사용
    Returns:
        _type_: list
    """
    s3 = boto3.resource('s3')
    tmpList = []
    print("bucket List")
    for bucket in s3.buckets.all():
        print(bucket.name)
        tmpList.append(bucket.name)
        
    return tmpList

def getObjectInBucket(bucketName):
    """_summary_
        버킷의 오브젝트들을 가져와서 파일명 : 사이즈의 딕셔너리로 리턴하는 함수
        resource 사용
    Args:
        bucketName (_str_): 버킷이름

    Returns:
        _type_: _dict_
    """
    s3 = boto3.resource('s3')
    bucket = s3.Bucket(bucketName)
    tmpDict = {}
    print(f"Read {bucketName}:")
    cnt = 0
    size = 0
    for obj in bucket.objects.all():
        cnt += 1
        size += obj.size
        tmpDict[obj.key] = obj.size
    print(f"cnt : {cnt}     size : {size}")
    return tmpDict

