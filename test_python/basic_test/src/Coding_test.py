from memory_profiler import profile

tmpDic = {}
@profile
def CalWoobaksooCnt(num:int):
    if num in tmpDic.keys():
        return tmpDic[num]
    cnt = 1
    tmpList = []
    tmpList.append(num)
    while(num > 1):
        if num & 1 == True:
            num = num*3 + 1
        else:
            num = num // 2
        
        if num in tmpDic.keys():    
            cnt += tmpDic[num]
            break
        else:
            tmpList.append(num)
            cnt += 1
                    
    tmpCnt = cnt
    for item  in tmpList:
        tmpDic[item] = tmpCnt
        tmpCnt -= 1
        

    return cnt
    
def Woobaksoo(numStr: str):
    numArr = list(map(int, numStr.split(" ")))
    
    sNum = numArr[0]
    eNum = numArr[1]
    longCnt = 0
    longNum = 0
    for i in range(sNum, eNum+1, 1):
        tmpCnt = CalWoobaksooCnt(i)

        if longCnt< tmpCnt:
            longCnt = tmpCnt
            longNum = i
    print(f'{longNum} {longCnt}')
    return


inputStr = "1 10000000"

Woobaksoo(inputStr)

