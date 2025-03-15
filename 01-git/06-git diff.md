# 使用git log查看日志

使用git log --oneline查看精简日志

```shell
$ git log --oneline
d4208aa (HEAD -> master, github/master, giee/master, giee/HEAD) 完成cmd-parser适配
9dd18fc 添加cmd-parser源码
16d98fe 增加42-cmd-parser初始模版工程
b625708 解决编译时报的warning
f0b133d MultiTimer示例代码增加一些备注
19777e2 延时更加精确
f91e0a2 增加41-SoftwareTask
```

# 使用git difff查看差异

git diff f91e0a2  19777e2
* f91e0a2 
* 19777e2 

查看"19777e2 延时更加精确"这一笔到底修改了什么

```shell
$ git diff f91e0a2  19777e2
diff --git a/04-Delay/main.c b/04-Delay/main.c
index 302409f..e0a9169 100644
--- a/04-Delay/main.c
+++ b/04-Delay/main.c
@@ -74,7 +74,7 @@ int main(void)
 void delay_us(uint32_t nus)
 {
   uint32_t temp;
-  SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000000/8*nus;
+  SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000000/8*nus-1;
   SysTick->VAL=0X00;
   SysTick->CTRL=0X01;
   do
@@ -87,7 +87,7 @@ void delay_us(uint32_t nus)
 void delay_ms(uint16_t nms)
 {
   uint32_t temp;
-  SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000/8*nms;
+  SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000/8*nms-1;
   SysTick->VAL=0X00;
   SysTick->CTRL=0X01;
   do
```
以下是 `git diff` 输出结果的逐行解读：

---

### **1. `diff --git a/04-Delay/main.c b/04-Delay/main.c`**  
• **含义**：表示比较两个版本的文件差异。  
  • `a/04-Delay/main.c` 是旧版本的文件路径（对应提交 `f91e0a2`），`b/04-Delay/main.c` 是新版本的文件路径（对应提交 `19777e2`）。  
  • `a` 和 `b` 是 Git 生成的临时引用，用于标识变动前后的文件。

---

### **2. `index 302409f..e0a9169 100644`**  
• **含义**：表示文件的索引信息。  
  • `302409f` 是旧版本文件的 SHA-1 哈希值，`e0a9169` 是新版本文件的 SHA-1 哈希值，用于唯一标识文件内容。  
  • `100644` 是文件权限模式：  
    ◦ `100` 表示普通文件（非可执行文件或目录）。  
    ◦ `644` 对应权限 `rw-r--r--`（用户可读写，组和其他用户只读）。

---

### **3. `--- a/04-Delay/main.c`**  
• **含义**：旧版本文件的路径和标识，即变动前的文件内容。

---

### **4. `+++ b/04-Delay/main.c`**  
• **含义**：新版本文件的路径和标识，即变动后的文件内容。

---

### **5. `@@ -74,7 +74,7 @@ int main(void)`**  
• **含义**：差异块的上下文范围标识。  
  • `-74,7`：旧版本中从第 74 行开始的连续 7 行（包含上下文）。  
  • `+74,7`：新版本中从第 74 行开始的连续 7 行（包含上下文）。  
  • `int main(void)` 是差异块附近的代码上下文，帮助定位修改位置。

---

### **6. 代码变更部分**  
• **减号 `-` 开头的行**：表示旧版本中被删除的内容。  
  • 例如：`- SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000000/8*nus;`  
• **加号 `+` 开头的行**：表示新版本中新增的内容。  
  • 例如：`+ SysTick->LOAD = RCC_Clocks.HCLK_Frequency/1000000/8*nus-1;`  
• **未标记的行**：上下文代码，帮助理解变更的周围逻辑。

---




