# 05 · 判断是否为 2 的幂

**头文件：** [`power_of_two.hpp`](../include/bithacks/power_of_two.hpp)

## 核心观察

若 `v` 是 2 的幂，其二进制只有一个 1，则 `v - 1` 会把该位以下全部变成 1，与 `v` 与运算得 0：

```cpp
f = (v & (v - 1)) == 0;   // 错误地把 0 也当成 true
f = v && !(v & (v - 1));  // 正确
```

## 为什么 `v & (v-1)` 有用

`v & (v-1)` 会**清除最低的置位**。这是 Kernighan popcount、下一排列等技巧的积木。

## 标准库

```cpp
std::has_single_bit(v);  // C++20
```

下一章：[06 · 符号扩展](06-sign-extend.md)
