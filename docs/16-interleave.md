# 16 · 位交错（Morton / Z-order）

**头文件：** [`interleave.hpp`](../include/bithacks/interleave.hpp)

把 `x`、`y` 的位交错：`... y1 x1 y0 x0`。用于空间索引、纹理 swizzle、四叉树线性化。

## 方法

1. **循环**：按位提取  
2. **表**：每字节扩展成「隔位」形式  
3. **64 位乘法**（8×8→16）  
4. **Binary Magic Numbers**：反复 `x | (x<<s)` 再掩码  

```cpp
x = (x | (x << 8)) & 0x00FF00FF;
x = (x | (x << 4)) & 0x0F0F0F0F;
x = (x | (x << 2)) & 0x33333333;
x = (x | (x << 1)) & 0x55555555;
z = x | (y_expanded << 1);
```

下一章：[17 · 字中的字节测试](17-byte-test.md)
