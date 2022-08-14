# Python

## Decorator

python装饰器，@调用将标记的func作为参数传入

```python
import functools

def log(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print('call %s():' % func.__name__)
        print('args = {}'.format(*args))
        return func(*args, **kwargs)

    return wrapper

@log
def test(p):
    print(test.__name__ + " param: " + p)
    
test("I'm a param")

def log_with_param(text):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            print('call %s():' % func.__name__)
            print('args = {}'.format(*args))
            print('log_param = {}'.format(text))
            return func(*args, **kwargs)

        return wrapper

    return decorator
    
@log_with_param("param") # 如果要带参数，则使用param的call之后返回了decorator才是真正的装饰器，log_with_param("param")已经是一个新的decorator了
def test_with_param(p):
    print(test_with_param.__name__)

test_with_param("aa",)
```

