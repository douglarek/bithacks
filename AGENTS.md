# AGENTS.md — 本仓库协作规范

面向在本仓库中工作的人与编码助手。目标：保持 **Bit Twiddling Hacks C++23 教程** 风格一致、可构建、可测试。

## 项目是什么

- 把 [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html) 做成中文教程 + C++23 header-only 实现 + 单元测试
- 教学优先：每个技巧要有文档推导、可移植说明、与 `std::bit` 对照
- 非性能基准项目；不要为「少一个周期」牺牲可读性，除非文档明确在对比变体

## 语言与工具链

| 项 | 要求 |
|----|------|
| 语言 | **C++23**（`CMAKE_CXX_STANDARD 23`） |
| 构建 | CMake ≥ 3.20 |
| 格式化 | 根目录 [`.clang-format`](.clang-format)（**LLVM + IndentWidth 4 + LF**） |
| 静态检查 | 根目录 [`.clang-tidy`](.clang-tidy)（`bugprone-*` / `performance-*` 为主；见下） |
| 测试 | `ctest`；自包含 `tests/test_framework.hpp`（无第三方测试框架） |

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

改动 C++ 后务必重新构建并跑测试。格式化：

```bash
find include tests examples \( -name '*.hpp' -o -name '*.cpp' \) -print0 \
  | xargs -0 clang-format -i
```

`clang-tidy` 需要 `compile_commands.json`（CMake 已 `CMAKE_EXPORT_COMPILE_COMMANDS ON`）。**不要直接扫裸 header 当主 TU**（缺完整编译上下文）；通过已有 `.cpp` 分析，并限制只报告我们的头文件：

```bash
# 推荐：经 tests/examples 编译单元分析 include/bithacks
clang-tidy -p build tests/*.cpp examples/demo_all.cpp \
  --header-filter='.*/include/bithacks/.*' \
  --system-headers=0 \
  -- -std=c++23 -Iinclude
```

`.clang-tidy` 已关闭对教学项目噪声大的项（如 `misc-include-cleaner` 对 umbrella header、`bugprone-exception-escape` 对 `main`+iostream）。**新增检查项前先在全量 tests 上试跑**，避免把 libc++ 告警灌进 PR。

## 代码布局

```text
include/bithacks/   # 对外 header-only API，命名空间 bithacks
docs/               # 中文教程 00–18，与技巧一一对应
tests/              # 每个模块一个 test_*.cpp
examples/           # demo_all 等可执行演示
```

- 新技巧：`include/bithacks/<name>.hpp` + `docs/NN-<name>.md` + `tests/test_<name>.cpp`，并挂到 `bithacks.hpp` 与 `tests/CMakeLists.txt`
- 不要引入重量级依赖；保持 header-only，除非有强理由

## C++ 风格（与 clang-format 对齐）

- **缩进 4 空格**；行尾 LF；基于 LLVM 风格
- 命名空间：`bithacks`；实现细节放 `bithacks::detail`
- 函数/变量：`snake_case`；模板类型参数：`T` / `U`；概念：`word32` 这类短名可保留
- 头文件：`#pragma once`；包含顺序大致为对应模块 → C++ 标准库
- API 优先：
  - `constexpr` / `noexcept` 能写就写
  - `std::unsigned_integral` / `std::signed_integral` 约束位运算接口
  - 纯位运算优先 `unsigned`；有符号仅在语义需要时使用
- 类型双关只用 `std::bit_cast`，禁止指针类型双关
- 与标准库对照时，在注释或文档写明 `std::popcount` / `std::countr_zero` / `std::bit_ceil` 等，**不要**在库里无谓地再包一层同名转发除非有教学变体

### 可移植性约定

1. 依赖算术右移或实现定义行为的变体，函数名加后缀（如 `*_shift`），并在文档标明
2. `abs` 类接口对 `INT_MIN` 返回 `unsigned`，避免有符号溢出
3. 移位量必须在 `[0, width)`；文档与测试覆盖 0、全 1、`min`/`max`
4. 原站「乘除靠 32 位溢出」一类技巧：在 unsigned 包装下保持 wrap 语义，并加注释

## 文档风格（`docs/`）

每节建议结构：

1. 问题是什么  
2. 朴素写法  
3. 位技巧推导  
4. C++23 实现（指向 header）  
5. 与 `<bit>` 对照  
6. 可移植性与边界  
7. 测试思路  

- 正文**中文**；标识符与代码保持英文  
- 注明出处链接；讲解为原创改写，勿整页粘贴原站 HTML  
- 交叉链接上一章/下一章

## 测试规范

- 使用 `EXPECT` / `EXPECT_EQ`（`test_framework.hpp`）
- 能对照标准库的（`std::popcount`、`std::has_single_bit`…）尽量对照  
- 覆盖边界：`0`、全 1、`INT_MIN`、最大幂等  
- 测试文件顶部包含 `test_framework.hpp` 再包含被测头文件  
- 失败信息要能定位；`main` 以 `test::summary("name")` 返回码结束

## Git / 提交

- 提交信息简洁说明「改了什么、为什么」；可用 conventional commits（`feat:` / `fix:` / `docs:` / `test:` / `style:`）
- 不要提交 `build/`、本地 `compile_commands.json` 副本（已在 `.gitignore`）
- 不要强推、不要改无关历史

## 版权

- 技巧思想来自 Sean Eron Anderson 收集页；片段多为 public domain，集合描述有版权  
- 本仓库中文讲解与 C++ 重写见 `LICENSE`  
- 新增内容保持同样出处说明，勿删除 README/LICENSE 中的归属

## 给编码助手的额外约束

- **先匹配周围代码**：命名、注释密度、`constexpr` 习惯与现有 header 一致  
- 改算法先补/跑测试，再声称完成  
- 格式化以 `.clang-format` 为准（4 空格），不要再引入 2 空格风格  
- 新增 clang-tidy 告警：优先修代码；若属教学刻意写法，用 NOLINT 注释收窄到单行并说明原因  
- 不把教程做成外部网络服务依赖；保持可离线构建  
- 用户若要求「只改文档」或「只改实现」，不要越界大重构  

## 常用检查清单（PR / 会话结束前）

- [ ] `clang-format` 已应用于改动的 `.hpp`/`.cpp`  
- [ ] `cmake --build build` 无错误  
- [ ] `ctest --test-dir build` 全绿  
- [ ] 新技巧具备 docs + header + test + umbrella include  
- [ ] 可移植性/边界在文档中有一句说明  
