# C++课程设计报告：12306火车购票模拟系统

**姓名**：[你的名字]  
**学号**：[你的学号]  
**班级**：[你的班级]  
**日期**：2026-01-11

---

## 1. 课题概述

### 1.1 课题背景
随着交通网络的日益发达，火车出行成为人们出行的主要方式之一。为了模拟真实的火车购票流程，理解12306系统的核心业务逻辑，本课程设计旨在利用C++面向对象编程思想和Qt图形界面框架，开发一个功能完善的“火车购票模拟系统”。

### 1.2 课题要求
1.  **图形化界面**：系统需具备美观、友好的图形用户界面（GUI）。
2.  **核心功能**：
    *   用户注册与登录（区分乘客和管理员）。
    *   车次查询（支持按起止站点、日期查询）。
    *   车票预订（检查余票、生成订单）。
    *   退票功能。
    *   管理员功能（添加车次、查看所有车次）。
3.  **技术要求**：
    *   使用C++语言编写，代码量不少于1000行。
    *   体现面向对象编程（OOP）特性：封装、继承、多态。
    *   使用STL容器（vector, map等）进行数据管理。
    *   严禁使用全局变量，保持良好的代码风格。

---

## 2. 系统需求分析

### 2.1 功能需求
系统主要分为两个角色的功能模块：

*   **乘客（Passenger）**：
    *   **注册/登录**：输入用户名、密码、真实姓名、身份证号进行操作。
    *   **车次查询**：输入出发地、目的地、日期，查询符合条件的车次信息（包括时刻、票价）。
    *   **订票**：选择车次进行预订，系统自动扣减库存并生成订单。
    *   **订单管理**：查看历史订单，支持对“已支付”订单进行退票操作。
    *   **注销**：退出当前账号。

*   **管理员（Admin）**：
    *   **登录**：使用管理员账号登录。
    *   **车次管理**：查看系统中所有车次信息。
    *   **添加车次**：录入新车次，支持默认路线、随机路线生成或自定义路线。
    *   **注销**：退出管理系统。

### 2.2 数据需求
系统需要存储以下核心数据：
*   **用户信息**：用户名、密码、角色、关联订单。
*   **车次信息**：车次ID、列车类型、总座位数、经停站点（站点名、到达/出发时间、里程、价格）。
*   **座位库存**：按日期和区间段管理的剩余座位数。
*   **订单信息**：订单号、用户ID、车次ID、起止站、日期、票数、订单状态（已支付、已取消、已完成）。

---

## 3. 系统设计

### 3.1 总体架构
系统采用分层架构设计：
*   **表现层（View）**：基于Qt6实现的图形界面 (`MainWindow` 类)，负责与用户交互。
*   **逻辑层（Controller）**：`SystemManager` 类，负责核心业务逻辑处理（如登录验证、余票计算、订单生成）。
*   **数据模型层（Model）**：`User`, `Train`, `Order` 等实体类，封装数据和基础操作。

### 3.2 类结构设计（Class Design）

#### 3.2.1 用户类体系（继承与多态）
*   `User` (抽象基类)：
    *   成员：`username`, `password`, `realName`。
    *   虚函数：`getRole()` (纯虚函数), `displayMenu()`。
*   `Passenger` (继承自 User)：
    *   特有成员：`orders` (订单列表)。
    *   实现：`getRole()` 返回 "Passenger"。
*   `Admin` (继承自 User)：
    *   实现：`getRole()` 返回 "Admin"。

#### 3.2.2 车次与订单类
*   `Train`：
    *   成员：`trainId`, `type`, `totalSeats`, `stations` (站点列表)。
    *   核心方法：
        *   `getTicketCount(start, end, date)`: 计算指定区间余票。
        *   `bookTickets(...)`: 扣减库存。
        *   `releaseTickets(...)`: 释放库存（退票）。
    *   **数据结构亮点**：使用 `std::map<string, vector<int>> seatInventory` 存储不同日期的分段座位库存，避免了全局变量，且支持无限日期的扩展。

*   `Order`：
    *   成员：`orderId`, `trainId`, `userId`, `startStation`, `endStation`, `status` (枚举)。
    *   功能：记录交易详情。

#### 3.2.3 系统管理类
*   `SystemManager`：
    *   作用：系统的“大脑”，持有所有数据的所有权。
    *   成员：
        *   `users`: `std::map<string, shared_ptr<User>>` (用户名到用户智能指针的映射)。
        *   `trains`: `std::map<string, Train>` (车次ID到车次对象的映射)。
        *   `currentUser`: 指向当前登录用户的智能指针。

### 3.3 关键业务流程
1.  **订票流程**：
    用户点击预订 -> `SystemManager` 检查该车次在该日期的该区间是否有余票 -> 若有，调用 `Train::bookTickets` 更新内存中的库存 -> 创建 `Order` 对象 -> 将订单存入 `Passenger` 的历史记录 -> 返回成功。

2.  **余票计算算法**：
    由于火车票是分段销售的，全程余票等于该行程所经过的所有区间段中，剩余座位数的**最小值**。
    *   例如：A->B->C->D，查询 A->C 的票。
    *   需检查 A->B 和 B->C 两个区间的座位数，取 `min(seats[A->B], seats[B->C])`。

---

## 4. 详细设计与实现

### 4.1 核心代码片段

#### 4.1.1 智能指针与多态的使用
在 `SystemManager` 中，使用 `std::shared_ptr<User>` 来管理用户，这允许我们在同一个容器中存储 `Passenger` 和 `Admin` 对象，并自动管理内存。

```cpp
// SystemManager.h
std::map<std::string, std::shared_ptr<User>> users;

// 登录逻辑
std::shared_ptr<User> SystemManager::login(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second->checkPassword(password)) {
        currentUser = it->second;
        return currentUser;
    }
    return nullptr;
}
```

#### 4.1.2 动态余票管理
为了支持任意日期的购票，`Train` 类使用 Map 按需创建库存记录。

```cpp
// Train.h
// 日期 -> (区间索引 -> 剩余座位数)
std::map<std::string, std::vector<int>> seatInventory;

// 检查余票逻辑
bool Train::hasTickets(const std::string& startStation, const std::string& endStation, 
                      const std::string& date, int count) {
    // ... 获取起始和结束站点的索引 ...
    auto& seats = seatInventory[date]; 
    if (seats.empty()) {
        // 如果该日期未初始化，则默认为全额座位
        seats.resize(stations.size() - 1, totalSeats);
    }
    
    // 遍历区间，检查最小值
    for (int i = startIdx; i < endIdx; ++i) {
        if (seats[i] < count) return false;
    }
    return true;
}
```

### 4.2 图形界面实现 (Qt)
使用了 `QStackedWidget` 实现页面切换，将登录、注册、乘客首页、管理员首页放在不同的 Widget 中。

*   **信号与槽 (Signals & Slots)**：
    *   连接按钮点击事件与后端逻辑。
    *   例如：`connect(searchBtn, &QPushButton::clicked, this, &MainWindow::handleSearch);`

*   **动态表格展示**：
    *   使用 `QTableWidget` 展示查票结果和订单历史。
    *   在表格单元格中嵌入按钮（如“预订”、“退票”按钮）。

---

## 5. 系统测试

### 5.1 测试环境
*   **操作系统**：macOS / Windows / Linux
*   **编译器**：Clang / GCC / MSVC (支持 C++17)
*   **GUI框架**：Qt 6.x
*   **构建工具**：CMake

### 5.2 功能测试用例

| 测试项目 | 测试操作 | 预期结果 | 实际结果 |
| :--- | :--- | :--- | :--- |
| **用户注册** | 输入新用户名和密码注册 | 提示注册成功，跳转登录页 | 通过 |
| **重复注册** | 输入已存在的用户名 | 提示“用户名已存在” | 通过 |
| **用户登录** | 输入正确账号密码 | 跳转至对应角色的主页 | 通过 |
| **车次查询** | 输入 Beijing 到 Shanghai | 列表显示 G1, G2 等车次及票价 | 通过 |
| **车票预订** | 点击 G1 的“预订”按钮 | 提示预订成功，库存减少 | 通过 |
| **库存不足** | 连续预订直到票罄 | 再次预订提示“余票不足” | 通过 |
| **退票功能** | 在订单页点击“退票” | 订单状态变更为“已取消”，库存恢复 | 通过 |
| **管理员添加** | 添加随机路线新车次 | 列表刷新，新车次可被搜索到 | 通过 |

### 5.3 自动化测试
项目中包含了 `test_main.cpp`，基于断言（Assertion）实现了无界面的逻辑测试，覆盖了登录、购票、退票的核心路径，确保后端逻辑的健壮性。

---

## 6. 总结与展望

### 6.1 设计总结
本次课程设计成功实现了一个基于C++和Qt的火车购票模拟系统。
1.  **面向对象实践**：深入理解了继承（User体系）、封装（Train内部库存管理）和多态（不同角色的视图更新）的应用。
2.  **数据结构应用**：灵活使用了STL容器，特别是 `map` 和 `vector` 的组合嵌套，解决了复杂的库存管理问题。
3.  **GUI开发**：掌握了Qt框架的基本使用，包括布局管理、信号槽机制和多窗口切换。

### 6.2 不足与改进
由于时间和篇幅限制，系统仍有改进空间：
1.  **数据持久化**：目前数据存储在内存中，程序重启后会丢失。未来可引入 SQLite 数据库或文件读写（JSON/CSV）来保存数据。
2.  **并发控制**：在多用户同时抢票场景下，需要引入互斥锁（Mutex）来保证库存数据的线程安全。
3.  **网络功能**：目前是单机版，未来可改造为 C/S 架构，分离客户端和服务器。

---

**致谢**：感谢老师的悉心指导和同学们的帮助。
