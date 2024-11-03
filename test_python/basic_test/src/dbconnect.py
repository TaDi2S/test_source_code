import pyodbc
import re

basicServer = '127.0.0.1'  # 예: 'localhost' 또는 '192.168.1.100'
basicDatabase = 'CC4HDTV_1310'
basicUsername = 'cc4iptv'
basicPassword = 'cc4iptv'
basicQuery = (
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
    
def read_from_mssql_chunks(fileNameList: list, server = basicServer, database = basicDatabase, username = basicUsername, 
                           password = basicPassword, query = basicQuery) -> list:
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

def sp_ctsinfo_find():
    dbname = "CC4IPTV_1310"
    # 데이터베이스 연결
    conn = pyodbc.connect('DRIVER={ODBC Driver 17 for SQL Server};'
                        f'SERVER={basicServer};'
                        f'DATABASE={dbname};'
                        f'UID={basicUsername};'
                        f'PWD={basicPassword};')
    # 커서 생성
    cursor = conn.cursor()
    
    table_name = 'ctsinfo'.lower()
    query = f"""
        SELECT ROUTINE_NAME, ROUTINE_DEFINITION
        FROM INFORMATION_SCHEMA.ROUTINES
        WHERE LOWER(ROUTINE_DEFINITION) LIKE '%{table_name}%'
        AND ROUTINE_TYPE = 'PROCEDURE'
        ORDER BY ROUTINE_NAME ASC
    """
    
    cursor.execute(query)
    
    # 결과 가져오기
    procedures = []
    for row in cursor.fetchall():
        routine_name = row.ROUTINE_NAME
        routine_definition = row.ROUTINE_DEFINITION.lower()

        # '_' 뒤에 숫자가 있는 경우 제외
        if re.search(r'_\d+$', routine_name):
            continue

        # 주석 안에 있는 경우 제외 (SQL 주석 방식 -- 또는 /* */)
        comments_pattern = r'(--.*?$|/\*.*?\*/)'  # 주석 패턴 정의
        comments = re.findall(comments_pattern, routine_definition, re.DOTALL | re.MULTILINE)
        is_commented = any(table_name in comment for comment in comments)
        if is_commented:
            continue

        # 조건을 만족하는 프로시저 추가
        procedures.append(routine_name)

    # 결과 출력
    if procedures:
        print(f"테이블 '{table_name}'을 참조하는 프로시저들:")
        for proc in procedures:
            print(proc)
    else:
        print(f"테이블 '{table_name}'을 참조하는 프로시저가 없습니다.")

    # 연결 종료
    cursor.close()
    conn.close()

    
    return