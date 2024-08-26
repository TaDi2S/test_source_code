import pyodbc


server = '127.0.0.1'  # 예: 'localhost' 또는 '192.168.1.100'
database = 'CC4HDTV_1310'
username = 'cc4iptv'
password = 'cc4iptv'
query = (
"SELECT "
"c.[content_id] as cId, "
"c.[ctype_exe] as type, "
"F.[filecontent_filename] as name "
"FROM "
"[CC4HDTV_1310].[dbo].[content] c "
"LEFT OUTER JOIN "
"[CC4HDTV_1310].[dbo].[filecontent] F ON c.[content_pk] = F.[content_pk]"
"ORDER BY c.[content_pk] DESC;"
)
    
def read_from_mssql_chunks(server, database, username, password, query, fileNameList):
    chunk_size=10000
    
    conn = pyodbc.connect(
        f'DRIVER={{ODBC Driver 17 for SQL Server}};'
        f'SERVER={server};'
        f'DATABASE={database};'
        f'UID={username};'
        f'PWD={password}'
    )
    cursor = conn.cursor()

    # 쿼리 실행
    cursor.execute(query)
    cnt = 0
    dataCnt = 0
    listLen = len(fileNameList)
    list3 = []
    while True:
        data = cursor.fetchmany(chunk_size)  # 지정한 크기만큼 데이터를 가져옴
        if not data:  # 더 이상 가져올 데이터가 없으면 종료
            break
        
        
        for row in data:
            dataCnt = dataCnt+1
            #type = row.type
            fileName = row.name
            contentID = row.cId
            if fileName in fileNameList:
                result2 = ""+contentID + ","+fileName
                list3.append(result2)
                cnt = cnt+1
        
        print("100000 datas complet! cnt : ", cnt, " dataCnt : ", dataCnt)
        if cnt >= listLen:
            break

    conn.close()
    return list3
    

def read_from_mssql(server, database, username, password, query):
    # MSSQL 데이터베이스 연결
    conn = pyodbc.connect(
        f'DRIVER={{ODBC Driver 17 for SQL Server}};'
        f'SERVER={server};'
        f'DATABASE={database};'
        f'UID={username};'
        f'PWD={password}'
    )
    cursor = conn.cursor()

    # 쿼리 실행
    cursor.execute(query)
    
    # 결과 가져오기
    rows = cursor.fetchall()
    
    # 연결 종료
    conn.close()
    
    return rows
