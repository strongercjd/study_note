输出log的完整信息和精简信息
```shell
$ git log
commit d4208aaa6066a2ddcba919f2bf62482403c0afb3 (HEAD -> master, github/master, giee/master, giee/HEAD)
Author: strongercjd <strongercjd@163.com>
Date:   Mon Feb 17 19:09:47 2025 +0800

    完成cmd-parser适配

$ git log --oneline
d4208aa (HEAD -> master, github/master, giee/master, giee/HEAD) 完成cmd-parser适配
9dd18fc 添加cmd-parser源码
```

在 Git 中，`d4208aaa6066a2ddcba919f2bf62482403c0afb3` 和 `9dd18fc` 都是提交的哈希值，但它们的表现形式不同，原因如下：

---

### 1. **长哈希与短哈希的区别**
• **长哈希**（如 `d4208aaa6066...`）  
  这是 Git 生成的完整 SHA-1 哈希值，由 40 位十六进制字符组成。它唯一标识了一次提交的所有内容（包括代码变动、作者、时间等元数据），确保全局唯一性。
  
• **短哈希**（如 `9dd18fc`）  
  是长哈希的前 7 个字符。Git 允许通过短哈希快速引用提交，因为前 7 位在大多数情况下足以唯一标识一个提交。这简化了日常操作（如 `git checkout` 或 `git show`）的输入。

---

### 2. **为什么两种哈希都能指向同一次提交？**
• **哈希生成的机制**  
  Git 的哈希值基于提交内容（包括代码快照、作者、时间等）通过 SHA-1 算法生成。只要内容不变，哈希值就固定不变。
  
• **短哈希的唯一性**  
  虽然短哈希只有 7 位，但 Git 会自动检测当前仓库中是否存在哈希冲突。如果前 7 位足够唯一，Git 会将其解析为完整哈希。例如，你的仓库中 `9dd18fc` 可能对应唯一的提交，因此可以直接使用。

---

### 3. **命令输出的差异**
• **`git log` 默认显示长哈希**  
  完整的哈希值用于精确标识提交，适合需要绝对唯一性的场景（如跨仓库操作或长期追溯）。
  
• **`git log --oneline` 显示短哈希**  
  该选项优化了可读性，将每条提交压缩为一行，仅显示短哈希和提交信息，适合快速浏览历史记录。

---
