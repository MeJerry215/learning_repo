import numpy as np
'''
X (100, 6)
W (6)
B (1)
Y (100)
'''
batch_size = 100

def forward(x, w, b):
    return np.dot(x, w) + b

def backward(x, w, b, y, y_exp, lr=1e-3):
    b_gradient = (y - y_exp)
    w_gradient = (y - y_exp) * x
    b = b - b_gradient * lr
    w = w - w_gradient * lr
    return w.reshape(6,), b

np.random.seed(99)
x = np.random.random((batch_size, 6)) - 0.5
w = np.zeros((6,))
b = np.zeros((1,))

w_expect = np.array([0.3, 0.2, 0.5, -0.1, 1.3, -2.3])
b_expect = np.array([-3.5])
y_expect = forward(x, w_expect, b_expect)
loss = 1.0
i = 0
while loss > 1e-15:
    batch = x[i % 100].reshape(1, -1)
    y = forward(batch, w, b)
    l1_loss = y - y_expect[i % 100]
    w, b = backward(batch, w, b, y, y_expect[i % 100])
    loss = 2 * l1_loss * l1_loss
    i = i+1

print(loss, w, b, i)
np.testing.assert_allclose(y_expect, forward(x, w, b), 1e-3, 1e-3)
# def backward_batched(x, w, b, y, y_exp, lr=1e-3):
#     pass
#     w_gradient = np.sum(np.dot(y - y_exp, ), 0)
#     b_gradient = np.sum(y - y_exp) / batch_size * 2

# for i in range(n_iter):
#     y = forward(x, w, b)
#     l1_loss = y - y_expect
#     loss = np.sum((l1_loss ** 2) * 2) / batch_size
#     w, b = backward_batched(x, w, b, y, y_expect, lr =1e-3)
