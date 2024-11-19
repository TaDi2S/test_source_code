from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import binascii

key = "1EC0E2CE70276A5AC8166D8E824D8702"

key = bytes.fromhex(key)

print(f"{type(key)}, {" ".join(f"0x{b:02X}" for b in key)}")

hex_string = "a9d2cd72b46042b96acb9215042ca5e4"

# HEX 문자열을 바이트로 변환
data = bytes.fromhex(hex_string)

# AES 암호화 객체 생성 (ECB 모드)
cipher = AES.new(key, AES.MODE_ECB)

# 데이터 암호화
encrypted_data = cipher.decrypt(data)

# 암호화된 데이터 출력 (HEX 형식)
print("암호화된 데이터:", binascii.hexlify(encrypted_data).decode())

file1_path = 'D:/temp.ts'
file2_path = 'D:/temp.ts.dec.ts'

with open(file1_path, 'rb') as f1, open(file2_path, 'rb') as f2:
    data1 = f1.read()
    data2 = f2.read()
    
min_length = min(len(data1), len(data2))
data4 = {}
for i in range(0, min_length-1):
    if i == min_length:
        break
    data4[i] = data1[i]
    if data1[i] != data2[i]:
        if data1[i+1] != data2[i+1]:
            data3 = data1[i:i+15]
            data3 = cipher.decrypt(data3)
            data4[i:i+15] = data3
            i += 15
        else:
            continue
        
save_path2 = file2_path + '_copy2.ts'

with open(save_path2, 'wb') as f:
        f.write(data4)