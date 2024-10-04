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
# ���� �׸� 'ID', 'year', 'Gender'
"""
ID: ������ �Ǵ� ���� ��û�� ���� �ĺ���.
year: ���� ��û�� �̷���� ����.
loan_limit: ���� �ѵ��� ���� ����. ���� �ѵ��� �ִ��� ������?
Gender: ����. ���� 'Male', 'Female', 'Sex Not Available' 'Joint'
approv_in_adv: ������ ���� ���εǾ����� ����
loan_type: ������ ����. 'type1', 'type2' ������ ���� ��ǰ ������ ����.
loan_purpose: ���� ����
Credit_Worthiness: �ſ��� �� �ִ� ����? �ſ� ���, �����ڰ� ä�� ������ ���ɼ��� �Ǵ��ϴ� ��� �Ǵ� �ű� �ſ��� ���� �ڰ��� �ִ��� ����
open_credit: ��û�ڰ� �ٸ� ������ �ſ� ���¸� ������ �ִ��� ����.
business_or_commercial: ������ ����Ͻ� �Ǵ� ��������� ����. ���� �ݾ��� ��� ����
loan_amount: ��Ȯ�� ���� �ݾ�
rate_of_interest: �����ڰ� �����ο��� �ΰ��ϴ� �ݾ�����, ���� ������ �����. ������?
Interest_rate_spread: ���� ����� �����ڿ��� �����ϴ� �������� ���⿡�� �޴� �������� ����. ���� ������ ��� ���� ������. ���� ���� ��� ������ ����.
Upfront_charges: �ű� ���⿡ ���� �밡�� �������� �����ڿ��� �����ϴ� ������. ���� ������. ���� �ʱ� ������.
term: ���� �Ⱓ. ���� ��ȯ �Ⱓ.
Neg_ammortization: ���� ��ȯ(����, �ʱ� ���ڸ� �����ϰ� �̸� �Ŀ� �߰��� ������ �����ϴ� ��ȯ ���) ����. ���� �������� ������ ���� ǥ�� �Һαݺ��� ���� �ݾ��� �����ϴ� ��Ȳ 
interest_only: ��ġ �Ⱓ(���ڸ� ��ȯ�ϴ� �Ⱓ) ����. 'not_int'�� ���ڸ� ��ȯ�ϴ� �Ⱓ�� ������ �ǹ�.
lump_sum_payment: �Ͻú� ���� �ɼ��� �ִ��� ����. ���� �Ͻû�ȯ ����. 'not_lpsm'�� ���� �Ͻû�ȯ�� �ƴ�.
property_value: �㺸�� ��ġ. ����� ����� �ڻ��� ��ġ.
construction_type: �ڻ��� ���� ����. �Ǽ� ����.
occupancy_type: �ڻ��� ���� ����. 'pr'�� ���ֿ�(primary residence)�� �ǹ�.
Secured_by: ���⺸��, �㺸 �����Դϴ�. 'home'�� ���� �㺸�� �ǹ�.
total_units: ����� ���õ� ���� ��. �ε����� �� ���� ���Դϴ�. '1U'�� �ܵ� ���� �Ǵ� 1���� ������ �ǹ�.
income: ���� ��û���� �ҵ�.
credit_type: ���� �ſ� ��ȸ ����. �ſ� �� ���.
Credit_Score: �ſ� ����.
co-applicant_credit_type: ���� ��û���� �ſ� ��ȸ ����. ���� ��û���� �ſ� �� ���.
age: ��û���� ���ɴ�.
submission_of_application: ��û�� ���� ���. 'to_inst'�� ����� ���� ���������� �ǹ�.
LTV: ���� �ݾ� ��� �ڻ� ��ġ�� ����. �㺸��������(Loan to Value)��, ���� �ݾ��� �㺸 ��ġ�� ���� �����.
Region: ���� ��û ����.
Security_Type: �㺸�� ����.
Status: ���� ����. (1: ���� ����, 0: ���� ����).
dtir1:�� ��ä ��ȯ ����(Debt to Income Ratio). �̴� �ҵ� ��� ��ä�� ������ ��Ÿ��.

���� : https://www.kaggle.com/code/claudiojnior/98-accuracy-eda-ml-algorithms-model-fine-tune
"""


feature_name = ['loan_limit', 'approv_in_adv', 'loan_type', 'loan_purpose', 'Credit_Worthiness', 'open_credit', 'business_or_commercial', 
                'loan_amount', 'rate_of_interest', 'Interest_rate_spread', 'Upfront_charges', 'term', 'Neg_ammortization', 'interest_only', 
                'lump_sum_payment', 'property_value', 'construction_type', 'occupancy_type', 'Secured_by', 'total_units', 'income', 'credit_type', 
                'Credit_Score', 'co-applicant_credit_type', 'age', 'submission_of_application', 'LTV', 'Region', 'Security_Type', 'Status', 'dtir1']