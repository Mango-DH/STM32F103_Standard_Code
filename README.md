# 工程代码风格规范

这个工程是我在厦门大学机器人队期间，为了规范和完善自己的代码风格而设计的。主要包括灵活运用句柄控制函数和代码文件的分层管理。希望这些经验对你有所帮助。

## 代码层次

代码按照分层架构设计，通常分为以下几个层次：

1. **应用层（Application Layer）**：
   - `main.c`：这是程序的入口点，通常包含主循环和应用程序的主要逻辑。

2. **业务逻辑层（Business Logic Layer）**：
   - `system business logic`：这一层处理应用程序的核心逻辑和功能。

3. **驱动层（Driver Layer）**：
   - `driver` 目录下的文件（如 `drive_key.c`, `drive_led.c`, `drive_servo.c`）：这些文件包含与硬件设备直接交互的代码，负责控制和管理硬件资源。

4. **硬件抽象层（HAL - Hardware Abstraction Layer）**：
   - `hal/peripherals` 和 `hal/software` 目录下的文件：这些文件提供对硬件的抽象接口，使得上层代码可以与硬件交互而不需要了解硬件的具体细节。例如，`utility_gpio.c` 提供通用的 GPIO 操作接口。

5. **工具层（Utility Layer）**：
   - `utility` 目录下的文件（如 `utility_delay.c`, `utility_error.c`, `utility_exit.c`）：这些文件提供通用的工具函数和辅助功能，如延时、错误处理、退出处理等。

6. **数据结构层（Data Structures Layer）**：
   - `utility_linked_list.c`, `utility_queue.c`, `utility_stack.c`：这些文件实现常用的数据结构，如链表、队列和栈，供其他层使用。

7. **通信层（Communication Layer）**：
   - `utility_usart.c`：这个文件处理串口通信相关的功能。

8. **标准库层（Standard Library Layer）**：
   - `RCS_ANSI.c`：这个文件包含与标准库或特定标准（如 ANSI）相关的实现。

## 模块化和封装性

- 代码通过函数封装了 GPIO 的初始化、输出、输入、翻转等操作，体现了模块化设计的思想。
- 使用了 `Util_GPIO_Handler` 结构体来管理 GPIO 的配置，增强了代码的可读性和可维护性。

## 注释清晰

- 每个函数都有详细的注释，说明了函数的功能、参数和返回值，符合良好的文档规范。
- 注释风格统一，使用了 `@name`、`@brief`、`@param`、`@reval` 等标签，便于生成文档。

## 命名规范

- 函数和变量命名清晰，使用了驼峰命名法（如 `Util_GPIO_Create`、`Util_GPIO_Output`），符合常见的嵌入式开发命名规范。
- 宏定义和枚举值（如 `Util_GPIO_Mode_PP`）的命名也具有一定的描述性。

## 错误处理

- 函数返回值使用了 `My_Err_Type`，虽然当前代码中没有复杂的错误处理逻辑，但这种设计为后续扩展提供了可能性。

## 静态函数的使用

- `My_GPIO_Get_RCC` 被定义为 `static inline`，限制了其作用域，同时避免了函数调用的开销，适合嵌入式环境。
