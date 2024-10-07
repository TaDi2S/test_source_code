#import firstWeekAssign
import requests
import pandas

def main():
    query="서울 카페"
    total_data = []
    total_cnt = 0
    for i in range (1, 45, 1):    
        url = f"https://dapi.kakao.com/v2/local/search/address.json?query={query}&page={i}&size=30"
        
        headers = {
            'Content-type': 'application/json;charset=UTF-8 '
        }
        
        # API 요청 보내기
        response = requests.get(url, headers=headers)
        # 결과 출력
        if response.status_code == 200:
            data = response.json()
            print("len(data['items'] : {}".format(len(data['items'])))
            total_data.extend(data['items'])
            # for item in data['items']:
            #     print(f"카페 이름: {item['title']}")
            #     print(f"주소: {item['address']}")
            #     print(f"연락처: {item['telephone']}")
            #     print(f"카페 정보 URL: {item['link']}")
            #     print('-' * 40)
        else:
            print(f"Error Code: {response.status_code}")
        
    print("len(total_data) : {}".format(len(total_data)))
    return





# 프로그램의 진입점
if __name__ == '__main__':
    main()