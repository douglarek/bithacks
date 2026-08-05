# 10 · 交换

**头文件：** [`swap.hpp`](../include/bithacks/swap.hpp)

## 整值交换

```cpp
// XOR（a、b 不得是同一对象，否则清零）
a ^= b; b ^= a; a ^= b;
// 加减（注意有符号溢出；用 unsigned）
a -= b; b += a; a = b - a;
```

现代 C++ 请直接 `std::swap`。XOR swap 主要是面试题与历史代码。

## 交换两组位

把位置 `i` 起的 `n` 位与位置 `j` 起的 `n` 位对调：

```cpp
x = ((b >> i) ^ (b >> j)) & ((1u << n) - 1);
r = b ^ ((x << i) | (x << j));
```

本仓库 API 把 `i/j/n` 收进 `BitSwap`，避免三个 `unsigned` 相邻易传反：

```cpp
auto r = bithacks::swap_bits(b, bithacks::BitSwap{i, j, n});
```

区间重叠时结果未定义/错误——调用方需保证不重叠。

下一章：[11 · 位反转](11-reverse.md)
