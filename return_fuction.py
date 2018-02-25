def price_with_vat(amount):
    vat = amount*7/107
    price = amount - vat

    t = price,vat
    # return t
    # return price,vat
    return price,vat
# print(price_with_vat(107))
# a = price_with_vat(200)
# print(type(a))
# print("price =" ,a[0])
# print("vat =",a[1])
#
# p,v = price_with_vat(200)
# print("p =",p)
# print("v =",v)

def lao_area(sqwa):
    rai = sqwa // 400
    ngan = (sqwa - (rai * 400))
    wa = sqwa % 100

    return rai,ngan,wa
a = 956
print(lao_area(a))




