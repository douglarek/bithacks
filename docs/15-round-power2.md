# 15 · 上取整到 2 的幂

**头文件：** [`round_power2.hpp`](../include/bithacks/round_power2.hpp)

## 位填充法

```cpp
--v;
v |= v >> 1;
v |= v >> 2;
v |= v >> 4;
v |= v >> 8;
v |= v >> 16;
++v;
```

先减 1，使已是 2 的幂时保持不变；再把最高 1 以下全部填 1，最后加 1 进位成下一个幂。

## float 法

用浮点指数快速估，范围/精度有限，见 `round_up_pow2_float`。

## 标准库

```cpp
std::bit_ceil(v);  // C++20
```

注意：若结果无法用该无符号类型表示，`std::bit_ceil` 的行为有约束——调用前检查值域。

下一章：[16 · Morton 码](16-interleave.md)
