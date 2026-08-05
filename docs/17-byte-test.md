# 17 · 在一个字里测试字节范围

**头文件：** [`byte_test.hpp`](../include/bithacks/byte_test.hpp)

目标：一次检查 4 或 8 个字节是否含 0、等于 `n`、小于 `n` 等——字符串 `strlen`、解析器的老派优化基础。

## 核心：`haszero`

```cpp
#define haszero(v) (((v) - 0x01010101) & ~(v) & 0x80808080)
```

每个字节减 1：若原字节为 0 会借位，使该字节最高位模式可被检测。

## 派生

| 宏/函数 | 含义 |
|---------|------|
| `has_byte_eq` | 是否含字节 `n`（先 `xor` 广播再 haszero） |
| `has_byte_less` | 是否有字节 `< n` |
| `has_byte_more` | 是否有字节 `> n` |
| `has_byte_between` | 是否有字节在 `(m,n)` |
| `count_*` | 统计满足条件的字节数 |

## 注意

- 经典形式对 `n` 的范围有假设（常见 `n < 128`）  
- `likely_has_between` 可能更快但存在假阳性风险，需再确认  
- 今天 SIMD（SSE/NEON）通常更合适；这些技巧仍常见于可移植标量路径  

下一章：[18 · 下一比特排列](18-next-bit-permutation.md)
