import math

CL = float(input("Nhập độ dài CL: "))
BC = float(input("Nhập độ dài BC: "))
CT = float(input("Nhập độ dài CT: "))
KM = float(input("Nhập độ dài KM: "))

def cy(CL, BC, CT, KM):
    MN = 700    
    BN = 310
    KY = KM - CL
    BT = math.sqrt(BC * BC + CT * CT)
    NT = math.sqrt(BT * BT - BN * BN)
    TL = math.sqrt(CT * CT - CL * CL)
    CY = MN - NT - TL
    KC = math.sqrt(CY * CY + KY * KY)
    return KC

print(f"Độ dài chuyền KC là: {cy(CL, BC, CT, KM):.2f}")




















