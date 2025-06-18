import math

def tinh_dc(DB, BC, goc_cbd):
    g7_rad = math.radians(goc_cbd)
    return round(math.sqrt(DB**2 + BC**2 - 2 * DB * BC * math.cos(g7_rad)), 2)

def tinh_db(EB, ED):
    return round(math.sqrt(EB**2 + ED**2), 2)

def tinh_bt(BC, CT):
    return round(math.sqrt(BC**2 + CT**2), 2)

def goc_cbd(goc_NBT, goc_CBT, goc_EBD):
    return round(180 - (goc_NBT + goc_CBT + goc_EBD), 2)

def goc_nbt(BN, BT):
    ratio = BN / BT
    return math.degrees(math.cos(ratio))

def goc_cbt(BC, BT):
    ratio = BC / BT
    return math.degrees(math.cos(ratio))

def goc_ebd(EB, DB):
    ratio = EB / DB
    return math.degrees(math.cos(ratio))



# ====== Nhập giá trị mẫu từ hình ======
EB = 55      # giữ
ED = 35      # giữ
BN = 310     # giữ
BC = int(input("Nhập giá trị BC: "))     # điền 473
CT = int(input("Nhập giá trị CT: "))     # điền 154

# ====== Tính toán ======
DB = tinh_db(EB, ED)
BT = tinh_bt(BC, CT)

goc_nbt_val = goc_nbt(BN, BT)
goc_cbt_val = goc_cbt(BC, BT)
goc_ebd_val = goc_ebd(EB, DB)

goc_CBD = goc_cbd(goc_nbt_val, goc_cbt_val, goc_ebd_val)
DC = tinh_dc(DB, BC, goc_CBD)

# ====== In kết quả ======
print("DB:", DB)
print("BT:", BT)
print("Góc NBT:", goc_nbt_val)
print("Góc CBT:", goc_cbt_val)
print("Góc EBD:", goc_ebd_val)
print("Góc CBD:", goc_CBD)
print("DC:", DC)
