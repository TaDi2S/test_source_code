import torch
import torch.nn as nn

import numpy as np
import pandas as pd

import graphviz
import pydotplus

import joblib

from sklearn import tree, model_selection
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, ConfusionMatrixDisplay, confusion_matrix

import matplotlib.pyplot as plt

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

data_path = '../data/Loan_Default.csv'
# 제거 항목 'ID', 'year', 'Gender'
"""
ID: 데이터 또는 대출 신청의 고유 식별자.
year: 대출 신청이 이루어진 연도.
loan_limit: 대출 한도에 대한 정보. 대출 한도가 있는지 없는지?
Gender: 성별. 값은 'Male', 'Female', 'Sex Not Available' 'Joint'
approv_in_adv: 대출이 사전 승인되었는지 여부
loan_type: 대출의 유형. 'type1', 'type2' 등으로 대출 상품 종류를 구분.
loan_purpose: 대출 목적
Credit_Worthiness: 신용할 수 있는 상태? 신용 등급, 대출자가 채무 불이행 가능성을 판단하는 방법 또는 신규 신용을 받을 자격이 있는지 여부
open_credit: 신청자가 다른 개설된 신용 계좌를 가지고 있는지 여부.
business_or_commercial: 대출이 비즈니스 또는 상업용인지 여부. 대출 금액의 사용 목적
loan_amount: 정확한 대출 금액
rate_of_interest: 대출자가 차용인에게 부과하는 금액으로, 대출 원금의 백분율. 이자율?
Interest_rate_spread: 금융 기관이 예금자에게 지불하는 이자율과 대출에서 받는 이자율의 차이. 기준 이자율 대비 가산 이자율. 기준 지수 대비 이자율 차이.
Upfront_charges: 신규 대출에 대한 대가로 차용인이 대출자에게 지불하는 수수료. 선불 수수료. 대출 초기 수수료.
term: 대출 기간. 대출 상환 기간.
Neg_ammortization: 부정 상환(역상각, 초기 이자를 적게하고 이를 후에 추가해 원금이 증가하는 상환 방식) 여부. 대출 차용인이 은행이 정한 표준 할부금보다 적은 금액을 지불하는 상황 
interest_only: 거치 기간(이자만 상환하는 기간) 여부. 'not_int'는 이자만 상환하는 기간이 없음을 의미.
lump_sum_payment: 일시불 지불 옵션이 있는지 여부. 만기 일시상환 여부. 'not_lpsm'은 만기 일시상환이 아님.
property_value: 담보의 가치. 대출과 연계된 자산의 가치.
construction_type: 자산의 건축 유형. 건설 유형.
occupancy_type: 자산의 점유 형태. 'pr'은 거주용(primary residence)을 의미.
Secured_by: 대출보증, 담보 유형입니다. 'home'은 주택 담보를 의미.
total_units: 대출과 관련된 유닛 수. 부동산의 총 단위 수입니다. '1U'는 단독 주택 또는 1가구 주택을 의미.
income: 대출 신청자의 소득.
credit_type: 사용된 신용 조회 유형. 신용 평가 기관.
Credit_Score: 신용 점수.
co-applicant_credit_type: 공동 신청자의 신용 조회 유형. 공동 신청자의 신용 평가 기관.
age: 신청자의 연령대.
submission_of_application: 신청서 제출 방식. 'to_inst'는 기관을 통해 제출했음을 의미.
LTV: 대출 금액 대비 자산 가치의 비율. 담보인정비율(Loan to Value)로, 대출 금액을 담보 가치로 나눈 백분율.
Region: 대출 신청 지역.
Security_Type: 담보의 유형.
Status: 대출 상태. (1: 대출 승인, 0: 대출 거절).
dtir1:총 부채 상환 비율(Debt to Income Ratio). 이는 소득 대비 부채의 비율을 나타냄.

참고 : https://www.kaggle.com/code/claudiojnior/98-accuracy-eda-ml-algorithms-model-fine-tune
"""


feature_name = ['loan_limit', 'approv_in_adv', 'loan_type', 'loan_purpose', 'Credit_Worthiness', 'open_credit', 'business_or_commercial', 
                'loan_amount', 'rate_of_interest', 'Interest_rate_spread', 'Upfront_charges', 'term', 'Neg_ammortization', 'interest_only', 
                'lump_sum_payment', 'property_value', 'construction_type', 'occupancy_type', 'Secured_by', 'total_units', 'income', 'credit_type', 
                'Credit_Score', 'co-applicant_credit_type', 'age', 'submission_of_application', 'LTV', 'Region', 'Security_Type', 'Status', 'dtir1']