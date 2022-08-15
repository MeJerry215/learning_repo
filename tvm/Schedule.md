# Schedule

考虑以下op生成如下

```python
import tvm
from tvm import te

n = te.var("n")
m = te.var("m")

A = te.placeholder((m), name="A")
B = te.placeholder((m), name="B")
C = te.compute((m, n), lambda i, j: A[i] + B[i], name="C")

s = te.create_schedule([C.op])

print(tvm.lower(s, [A, B, C], simple_mode=True))
```

