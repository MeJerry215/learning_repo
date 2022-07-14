import numpy as np

def quantize(data_f, scale, zero):
    data_q = (data_f / scale + zero).astype(np.int8)
    return data_q


def dequantize(data_q, scale, zero):
    data_f = (data_q - zero) * scale
    return data_f


# def trad_kl_divergence(data_p, data_q):
#     return np.sum(np.log(data_q.flatten() / data_p.flatten()) * data_q.flatten())

def percentile():
    pass

def cross_entropy():
    pass

np.random.seed(42)
data_f = np.random.uniform(low=-100, high=100, size=(2, 4, 4))

q_max = 127
q_min = -128
r_max = np.max(data_f.flatten())
r_min = np.min(data_f.flatten())

def cal_scale(rmin, rmax, qmin, qmax):
    scale = (rmax - rmin) / (qmax - qmin)
    return scale

def cal_zero(rmax, qmax, scale):
    return round(qmax - rmax / scale)

scale = cal_scale(-r_max, r_max, -q_max, q_max)
zero = cal_zero(r_max, q_max, scale)

print(data_f)
data_q = quantize(data_f, scale, zero)
print(data_q)
data_r = dequantize(data_q, scale, zero)
print(data_r)
print(scale, zero)

data_f2 = np.random.uniform(low=-30, high=40, size=(2, 4, 4))
r_max = np.max(data_f2.flatten())
r_min = np.min(data_f2.flatten())
scale2 = cal_scale(-r_max, r_max, -q_max, q_max)
zero2 = cal_zero(r_max, q_max, scale2)
print(scale2, zero2)
data_q2 = quantize(data_f2, scale2, zero2)
print(data_f2)
print(data_q2)
data_r2 = dequantize(data_q2, scale2, zero2)
print(data_r2)

exp = data_f * data_f2
data_q = quantize(data_f, scale, zero)
rel = data_q.astype(np.int32) * data_q2.astype(np.int32) * scale2 * scale
print(exp.flatten()[0:10], rel.flatten()[0:10])
# np.testing.assert_allclose(exp.flatten(), rel.flatten())
# print(trad_kl_divergence(data_f, data_r))