# 18 · 字典序的下一比特排列

**头文件：** [`next_permutation.hpp`](../include/bithacks/next_permutation.hpp)

## 问题

给定无符号整数 `v`，生成**相同 popcount** 的下一个更大数（字典序下一位组合）。

例（4 位、k=2）：

```text
0011 → 0101 → 0110 → 1001 → 1010 → 1100
```

## 算法（除法形式）

```cpp
t = (v | (v - 1)) + 1;
w = t | ((((t & -t) / (v & -v)) >> 1) - 1);
```

步骤直觉：

1. `v | (v-1)` 把最低连续 1 段填满  
2. `+1` 把这一段进位到更高的 0，得到「把最低块上移」后的前缀 `t`  
3. 用 `lowbit(t)/lowbit(v)` 算出需要补回的低位 1 的个数，填到 `t` 的低位  

## ctz 形式

原站用 `__builtin_ctz`；本教程用可移植 `countr_zero`：

```cpp
next_bit_permutation_ctz(v);
```

## 应用

组合枚举、棋盘走法生成、某些 DP 子集转移的邻接遍历。

---

## 学完之后

- 回看 [00-intro.md](00-intro.md) 的 `std::bit` 对照表，把生产代码换成标准库  
- 运行 `ctest` 与 `demo_all`  
- 在你关心的 CPU 上对热点路径做基准，再决定手写或内建指令  

原站：<https://graphics.stanford.edu/~seander/bithacks.html>
