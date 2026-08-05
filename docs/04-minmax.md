# 04 · 无分支 min / max

**头文件：** [`minmax.hpp`](../include/bithacks/minmax.hpp)

## 技巧

```cpp
// min
r = y ^ ((x ^ y) & -(x < y));
// max
r = x ^ ((x ^ y) & -(x < y));
```

当 `x < y` 时 `-(x<y)` 为全 1，`(x^y)&mask == x^y`，于是 `y^(x^y) == x`；否则 mask 为 0，结果保持 `y`（min）或 `x`（max）。

## 快速脏写法

在保证 `x - y` 不溢出时：

```cpp
r = y + ((x - y) & ((x - y) >> 31)); // min, 32-bit
```

本教程提供 `min_quick` / `max_quick`，但**默认推荐 XOR 形式**。

## 现代代码

多数情况下直接写 `std::min` / `std::max` 即可；编译器常能生成条件传送（`cmov`）而无昂贵分支。无分支位技巧主要用于：

- 必须纯整数表达式（某些 DSL / GPU / constexpr 约束）
- 教学与逆向阅读

下一章：[05 · 2 的幂](05-power-of-two.md)
