# 09 · 奇偶校验（Parity）

**头文件：** [`parity.hpp`](../include/bithacks/parity.hpp)

Parity = 置位数模 2（奇数个 1 则为 true）。

## 方法

1. **朴素**：每次清最低 1 并翻转布尔  
2. **查表**：折到一字节 + `ParityTable256`  
3. **字节乘模**：`((b*0x0101... & 0x8040...) % 0x1FF) & 1`  
4. **字乘**：折叠后 `* 0x11111111` 取高位  
5. **并行**：折到 4 位，用 `0x6996 >> nibble` 查小表  

## 与 popcount

```cpp
bool p = std::popcount(v) & 1;
```

有硬件 popcount 时这往往最快；无 popcount 时并行折叠很有竞争力。

下一章：[10 · 交换](10-swap.md)
