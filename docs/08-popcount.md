# 08 · 统计置位（Population Count）

**头文件：** [`popcount.hpp`](../include/bithacks/popcount.hpp)

## 方法一览

| 方法 | 复杂度 | 函数 |
|------|--------|------|
| 朴素移位 | O(宽度) | `popcount_naive` |
| Kernighan | O(置位数) | `popcount_kernighan` |
| 256 项表 | O(宽度/8) | `popcount_table` |
| 64 位乘取模 | 固定（窄输入） | `popcount_mul_14/24/32` |
| SWAR 并行 | 固定 ~12 ops | `popcount_parallel` |
| 从 MSB 的 rank | — | `popcount_rank_from_msb` |
| 按 rank 选位置 | — | `select_bit_from_msb` |

## Kernighan

```cpp
for (c = 0; v; ++c) v &= v - 1;
```

每次清掉最低 1，循环次数 = popcount。

## 并行（SWAR）

用魔法常数把相邻位的计数折叠：

```cpp
v = v - ((v >> 1) & 0x55555555);
v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
c = (((v + (v >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
```

## 标准库

```cpp
std::popcount(v);  // 通常映射到 POPCNT / CNT 指令
```

生产代码优先 `std::popcount`；手写版本用于理解与无指令目标。

下一章：[09 · 奇偶性](09-parity.md)
