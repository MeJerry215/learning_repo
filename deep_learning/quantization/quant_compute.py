import numpy as np

def gen_random(shape, low=0, high=1, dtype="float32"):
    if dtype == "float32":
        return np.random.uniform(low=low, high=high, size=shape)
    elif dtype == "int8" or dtype == "int32":
        return np.random.randint(low=low, high=high, size=shape)
    return None


def quant_tensor(x, w, b, a_min=-128, a_max=127):
    return np.clip(np.round((x - b) / w), a_min=a_min, a_max=a_max).astype("int8")


def dequant_tensor(x, w, b):
    return w * x + b


def calculate_quant_param(x, with_bias=True):
    a_min = np.min(x) 
    a_max = np.max(x)
    if not with_bias:
        a_max = max(abs(a_min), abs(a_max))
        a_min = -a_max
    if with_bias:
        w = (a_max - a_min) / 255
        b = a_max - w * 127
    else:
        w = a_max / 127
        b = 0
    print("a_max {} a_min {}".format(a_max, a_min))
    return w, b

shape = (10000)

def _find_scale_by_percentile(arr, percentile=0.99999):
    x = np.abs(arr)
    max_k = int(x.size * percentile)
    return np.partition(x, max_k)[0: max_k]

np.random.seed(42)
x1 = gen_random(shape=shape, low=-1, high=1, dtype="float32")
x2 = gen_random(shape=shape, low=-2, high=1, dtype="float32")
x1[0] = 10

w1, b1 = calculate_quant_param(_find_scale_by_percentile(x1), with_bias=False)
w2, b2 = calculate_quant_param(_find_scale_by_percentile(x2), with_bias=False)

# w1, b1 = calculate_quant_param(x1, with_bias=False)
# w2, b2 = calculate_quant_param(x2, with_bias=False)
print(w1, b1, w2, b2)
    
exp = x1 + x2

q1 = quant_tensor(x1, w1, b1)
q2 = quant_tensor(x2, w2, b2)

f1 = dequant_tensor(q1, w1, b1)
f2 = dequant_tensor(q2, w2, b2)
c = f1 + f2

np.testing.assert_allclose(c, exp, atol=0.01, rtol=0.01)




# print(_find_scale_by_percentile(x1, percentile=0.9))