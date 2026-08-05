# 07 · 条件置位 / 条件取反 / 按掩码合并

**头文件：** [`conditional.hpp`](../include/bithacks/conditional.hpp)

## 条件置位或清位

语义：`if (f) w |= m; else w &= ~m;`

```cpp
w = (w & ~m) | (-f & m);           // 超标量友好
w ^= (-f ^ w) & m;                 // 异或形式
```

`bool_mask(f)` 把 `true` 变成全 1。

## 条件取反

```cpp
// flag ? -v : v
r = (v ^ -f) + f;
// keep ? v : -v
r = (k ^ (k - 1)) * v;
```

## 按掩码合并

```cpp
// 经典: (a & ~mask) | (b & mask)
r = a ^ ((a ^ b) & mask);  // 少一次运算
```

`mask` 为 1 的位取自 `b`，为 0 的位取自 `a`。这是 SIMD blend 的标量原型。

下一章：[08 · 统计置位](08-popcount.md)
