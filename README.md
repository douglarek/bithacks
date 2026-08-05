# Bit Twiddling Hacks

把经典页面 [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)（Sean Eron Anderson）做成**中文讲解 + 可运行 C++ 代码 + 单元测试**的完整教程。

原站每个技巧在本仓库中都有：

1. 中文文档（直觉、推导、边界、可移植性）
2. `include/bithacks/*.hpp` 中的 `constexpr` 实现
3. 对应测试（与朴素实现 / `std::bit` 对照）

## 快速开始

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/demo_all
```

需要支持 **C++23** 的编译器（Apple Clang / GCC 13+ / Clang 17+ 等）。

协作与代码规范见 [AGENTS.md](AGENTS.md)。格式化 / 静态检查使用仓库根目录的 [`.clang-format`](.clang-format)、[`.clang-tidy`](.clang-tidy)：

```bash
# 格式化全部 C++ 源文件
find include tests examples \( -name '*.hpp' -o -name '*.cpp' \) -print0 \
  | xargs -0 clang-format -i

# clang-tidy（需 compile_commands.json，CMake 默认导出到 build/）
clang-tidy -p build include/bithacks/*.hpp -- -std=c++23 -Iinclude
```

## 学习路径

| 顺序 | 文档 | 主题 | 头文件 |
|------|------|------|--------|
| 0 | [docs/00-intro.md](docs/00-intro.md) | 导论、操作计数、`std::bit` 对照 | `common.hpp` |
| 1 | [docs/01-sign.md](docs/01-sign.md) | 整数符号 | `sign.hpp` |
| 2 | [docs/02-opposite-signs.md](docs/02-opposite-signs.md) | 异号检测 | `sign.hpp` |
| 3 | [docs/03-abs.md](docs/03-abs.md) | 无分支绝对值 | `abs.hpp` |
| 4 | [docs/04-minmax.md](docs/04-minmax.md) | 无分支 min/max | `minmax.hpp` |
| 5 | [docs/05-power-of-two.md](docs/05-power-of-two.md) | 是否为 2 的幂 | `power_of_two.hpp` |
| 6 | [docs/06-sign-extend.md](docs/06-sign-extend.md) | 符号扩展 | `sign_extend.hpp` |
| 7 | [docs/07-conditional.md](docs/07-conditional.md) | 条件置位/取反/合并 | `conditional.hpp` |
| 8 | [docs/08-popcount.md](docs/08-popcount.md) | 统计置位 | `popcount.hpp` |
| 9 | [docs/09-parity.md](docs/09-parity.md) | 奇偶校验 | `parity.hpp` |
| 10 | [docs/10-swap.md](docs/10-swap.md) | 交换 | `swap.hpp` |
| 11 | [docs/11-reverse.md](docs/11-reverse.md) | 位反转 | `reverse.hpp` |
| 12 | [docs/12-modulus.md](docs/12-modulus.md) | 取模技巧 | `modulus.hpp` |
| 13 | [docs/13-log2.md](docs/13-log2.md) | 整数 log2 / log10 | `log2.hpp` |
| 14 | [docs/14-trailing-zeros.md](docs/14-trailing-zeros.md) | 尾随零 / 最低置位 | `trailing_zeros.hpp` |
| 15 | [docs/15-round-power2.md](docs/15-round-power2.md) | 上取整到 2 的幂 | `round_power2.hpp` |
| 16 | [docs/16-interleave.md](docs/16-interleave.md) | 位交错 / Morton | `interleave.hpp` |
| 17 | [docs/17-byte-test.md](docs/17-byte-test.md) | 字中的字节范围测试 | `byte_test.hpp` |
| 18 | [docs/18-next-bit-permutation.md](docs/18-next-bit-permutation.md) | 下一比特排列 | `next_permutation.hpp` |

## 目录结构

```text
include/bithacks/   # header-only C++23 实现
docs/               # 中文教程
tests/              # 单元测试
examples/demo_all.cpp
```

## 设计约定

- 命名空间：`bithacks`
- 优先 `constexpr` + `std::unsigned_integral` / `std::signed_integral`
- 对「算术右移」等实现定义行为：提供可移植版本，并保留原站高速写法供对照
- 现代 C++ 对照：`std::popcount`、`std::countr_zero`、`std::bit_ceil`、`std::has_single_bit`、`std::bit_width` 等
- 教学优先于极致微优化；真实性能请在目标 CPU 上实测

## 版权与出处

- 技巧思想来自 Sean Eron Anderson 的收集页；原站注明**各代码片段多为 public domain**，**集合与描述文字 © 1997–2005 Sean Eron Anderson**
- 本仓库的中文讲解与 C++23 重写见 [LICENSE](LICENSE)

## 贡献方式

修 bug、补测试、改进讲解均可。发现与原站语义不一致时，请同时说明预期输入边界（尤其是 `0`、`INT_MIN`、有符号移位）。
