# README

本应用使用 Qt 5.15.2 开发。

## 架构

采用如下图所示的两种 Model - View - View Model（MVVM）架构。

![https://mermaid-js.github.io/mermaid-live-editor/edit/#eyJjb2RlIjoiZ3JhcGggVERcblx0dm0oVmlldyBNb2RlbClcblx0dihWaWV3KVxuXHRtKE1vZGVsKVxuXHR2LS0gXCJJbnRlbnRzIChVSSBFdmVudHMpXCIgLS0-dm1cblx0dm0tLSBVcGRhdGUtLT5tXG5cdG0tLlNpZ25hbHMuLT52XG5cblx0dm0xKFZpZXcgTW9kZWwpXG5cdHYxKFZpZXcpXG5cdG0xKE1vZGVsKVxuXHR2MS0tIFwiSW50ZW50cyAoVUkgRXZlbnRzKVwiIC0tLT52bTFcblx0dm0xLS0gVXBkYXRlLS0tPm0xXG5cdG0xLS5TaWduYWxzLi0-dm0xXG4gICAgdm0xIC0uU2lnbmFscy4tPnYxIiwibWVybWFpZCI6IntcbiAgXCJ0aGVtZVwiOiBcImRlZmF1bHRcIlxufSIsInVwZGF0ZUVkaXRvciI6ZmFsc2UsImF1dG9TeW5jIjp0cnVlLCJ1cGRhdGVEaWFncmFtIjpmYWxzZX0](https://mermaid.ink/svg/eyJjb2RlIjoiZ3JhcGggVERcblx0dm0oVmlldyBNb2RlbClcblx0dihWaWV3KVxuXHRtKE1vZGVsKVxuXHR2LS0gXCJJbnRlbnRzIChVSSBFdmVudHMpXCIgLS0-dm1cblx0dm0tLSBVcGRhdGUtLT5tXG5cdG0tLlNpZ25hbHMuLT52XG5cblx0dm0xKFZpZXcgTW9kZWwpXG5cdHYxKFZpZXcpXG5cdG0xKE1vZGVsKVxuXHR2MS0tIFwiSW50ZW50cyAoVUkgRXZlbnRzKVwiIC0tLT52bTFcblx0dm0xLS0gVXBkYXRlLS0tPm0xXG5cdG0xLS5TaWduYWxzLi0-dm0xXG4gICAgdm0xIC0uU2lnbmFscy4tPnYxIiwibWVybWFpZCI6eyJ0aGVtZSI6ImRlZmF1bHQifSwidXBkYXRlRWRpdG9yIjpmYWxzZSwiYXV0b1N5bmMiOnRydWUsInVwZGF0ZURpYWdyYW0iOmZhbHNlfQ)

对于命令式 UI 构建，最重要的是保证数据在不同部分之间的流动是单向的，**杜绝在同一个地方同时更新 Model 和 View**，不然随着 UI 依赖的状态越来越复杂，正确地更新 UI 变得几乎不可能。

如上图所示，View 中点击等 UI 事件触发 intent，由 View Model 进行处理后，更新 Model，Model 在自身更新后发送 signals（或者由 View Model 接受后发送 signals），View 根据信号进行更新。

本应用采取了以下方法严格实现数据的单向流动：

- 只有 View Model 持有 Model 对象
- View 只能通过 View Model 获取指向 Model 的 const 指针，因此不能改变 Model
- View Model 和 Model 不能访问 View 对象

由此，改变 Model 只能通过 View Model，而更新 View 只能通过 Model 或 View Model 发送的信号来间接实现。借助 Qt 的 signal - slot 机制，我们很容易实现信息的流动。

具体而言，应用整体框架如下：

![https://mermaid-js.github.io/mermaid-live-editor/edit#eyJjb2RlIjoiZmxvd2NoYXJ0IFREXG5cbmdhbWV2KENoZXNzR2FtZVZpZXcpXG5jb25maWd2KENoZXNzR2FtZUNvbmZpZ3VyYXRpb25WaWV3KVxuXG5tYW5hZ2VyKENoZXNzR2FtZU1hbmFnZXIpXG5nYW1lKENoZXNzR2FtZSlcblxubmV0W1tOZXR3b3JraW5nXV1cblxuc3ViZ3JhcGggdiBbVmlld11cblx0Z2FtZXZcblx0Y29uZmlndlxuZW5kXG5cbnN1YmdyYXBoIG0gW01vZGVsXVxuXHRnYW1lXG5cdG5ldFxuZW5kXG5cbnN1YmdyYXBoIHZtIFtWaWV3IE1vZGVsXVxuXHRtYW5hZ2VyXG5lbmRcblxuZ2FtZXYgLS0-IG1hbmFnZXJcbm1hbmFnZXIgLS0-IGdhbWVcbmdhbWUgLS4tPiBnYW1ldlxuY29uZmlndiAtLT4gbWFuYWdlclxubWFuYWdlciAtLi0-IGNvbmZpZ3Zcbm1hbmFnZXIgLS0-IG5ldFxubmV0IC0uLT4gbWFuYWdlciIsIm1lcm1haWQiOiJ7XG4gIFwidGhlbWVcIjogXCJkZWZhdWx0XCJcbn0iLCJ1cGRhdGVFZGl0b3IiOmZhbHNlLCJhdXRvU3luYyI6dHJ1ZSwidXBkYXRlRGlhZ3JhbSI6ZmFsc2V9](https://mermaid.ink/svg/eyJjb2RlIjoiZmxvd2NoYXJ0IFREXG5cbmdhbWV2KENoZXNzR2FtZVZpZXcpXG5jb25maWd2KENoZXNzR2FtZUNvbmZpZ3VyYXRpb25WaWV3KVxuXG5tYW5hZ2VyKENoZXNzR2FtZU1hbmFnZXIpXG5nYW1lKENoZXNzR2FtZSlcblxubmV0W1tOZXR3b3JraW5nXV1cblxuc3ViZ3JhcGggdiBbVmlld11cblx0Z2FtZXZcblx0Y29uZmlndlxuZW5kXG5cbnN1YmdyYXBoIG0gW01vZGVsXVxuXHRnYW1lXG5cdG5ldFxuZW5kXG5cbnN1YmdyYXBoIHZtIFtWaWV3IE1vZGVsXVxuXHRtYW5hZ2VyXG5lbmRcblxuZ2FtZXYgLS0-IG1hbmFnZXJcbm1hbmFnZXIgLS0-IGdhbWVcbmdhbWUgLS4tPiBnYW1ldlxuY29uZmlndiAtLT4gbWFuYWdlclxubWFuYWdlciAtLi0-IGNvbmZpZ3Zcbm1hbmFnZXIgLS0-IG5ldFxubmV0IC0uLT4gbWFuYWdlciIsIm1lcm1haWQiOnsidGhlbWUiOiJkZWZhdWx0In0sInVwZGF0ZUVkaXRvciI6dHJ1ZSwiYXV0b1N5bmMiOnRydWUsInVwZGF0ZURpYWdyYW0iOnRydWV9)

其中 View 包括游戏界面 `ChessGameView` 和连接界面 `ChessGameConfigurationView`；View Model 为 `ChessGameManager`，负责接受 View 传送的 actions、进行网络通信、更新 `ChessGame`；Model 为负责游戏逻辑的 `ChessGame` 和负责网络通信的一些类。

## 具体设计

### 游戏逻辑：Model

#### 类设计

`ChessGame` 类负责游戏逻辑的处理。对 `ChessGame` 的职责规定如下：

- 负责游戏状态的维护，private
- 负责所有对游戏的输入，mutating

- 负责所有游戏逻辑的判定，read only

- 负责所有游戏状态更新后的信号发送，signals

总结来说，所有与下棋操作和逻辑有关的工作均由 `ChessGame` 完成，其他类只能通过调用接口或者接受信号来进行操作。在开发过程中，曾经尝试过用 `ChessGameManager` 完成计时等操作，但与设计的架构不符，最后还是将这部分功能放在了 `ChessGame` 中。

#### 多用户同步

由于涉及网络通信，两台机器上的状态同步需要格外注意。为了方便，所有对于游戏逻辑的判定均在**本地**进行，不依赖对方程序上的结果。根据对方发送的数据更新 Game 的方法详见后文。

### 网络通信：Model

#### 类设计

网络通信较为简单，分为 `ChessGameNetworkClient` 和 `ChessGameNetworkServer` 两个类。因 C/S 多数发送、解析数据的方式相同，在 C/S 类之下定义一个网络通信基类 `ChessGameNetworkBase`，实现：

- 发送和解析：游戏开始信号、投降信号、棋子翻转数据、棋子移动数据
- 向调用者发送信号：已连接上、已断开连接、收到某个数据

Client 和 server 类在此基础上，添加了一些与连接、发送和接收棋盘数据有关的方法。

#### 通信协议

为方便，不对黏包情况作处理。

Header 规定如下：

- 4 字节的 `type`：数据类别，包括
  - `Flip = 200` 翻转棋子
  - `Move = 201` 移动棋子
  - `Chessboard = 202` 棋盘数据
  - `Surrender = 203` 投降
  - `Start = 204` 开始游戏
- 4 字节的 `index`：当前操作序号
  - 发送棋盘数据：`index = random_int(0, 20)`
  - 翻转棋子和移动棋子：`index = old_index + 1`
  - 其他：`index = any_int`

其中 `index` 用于双方同步游戏。另外，`index` 的奇偶决定当前哪方可以进行棋子操作。初始化棋盘时，客户端生成一个范围在 `[0, 20)` 的随机数作为 `index` 的初始值，即可随机决定先后手。

Payload 的规定由各自需要传输的数据决定，只需要统一发送和解析的方法即可。

### 游戏界面：View

界面均采用纯代码编写，以方便维护。

游戏界面关系如下：

![https://mermaid-js.github.io/mermaid-live-editor/edit/#eyJjb2RlIjoiZ3JhcGggVERcblxuZyhDaGVzc0dhbWVWaWV3KVxuYihDaGVzc2JvYXJkVmlldylcbnAoQ2hlc3NHYW1lUGFuZWxWaWV3KVxucyhDaGVzc2JvYXJkU2NlbmUpXG5cbmcgLS0-IGJcbmcgLS0-IHBcbmIgLS0-IHMiLCJtZXJtYWlkIjoie1xuICBcInRoZW1lXCI6IFwiZGVmYXVsdFwiXG59IiwidXBkYXRlRWRpdG9yIjpmYWxzZSwiYXV0b1N5bmMiOnRydWUsInVwZGF0ZURpYWdyYW0iOmZhbHNlfQ](https://mermaid.ink/svg/eyJjb2RlIjoiZ3JhcGggVERcblxuZyhDaGVzc0dhbWVWaWV3KVxuYihDaGVzc2JvYXJkVmlldylcbnAoQ2hlc3NHYW1lUGFuZWxWaWV3KVxucyhDaGVzc2JvYXJkU2NlbmUpXG5cbmcgLS0-IGJcbmcgLS0-IHBcbmIgLS0-IHMiLCJtZXJtYWlkIjp7InRoZW1lIjoiZGVmYXVsdCJ9LCJ1cGRhdGVFZGl0b3IiOmZhbHNlLCJhdXRvU3luYyI6dHJ1ZSwidXBkYXRlRGlhZ3JhbSI6ZmFsc2V9)

`ChessGameView` 包括显示状态信息、操作按钮的 `ChessGamePanelView`，和绘制棋盘的 `ChessboardView`。`ChessboardView` 使用 `ChessboardScene` 进行绘制。

#### 棋盘绘制

棋盘的绘制使用 Qt 的 Graphics View 框架，其子组件可独立管理，正好契合我们的要求。

##### 重绘

固定大小的界面十分不友好，为了实现窗口自适应，使用如下方法：

- 固定所有组件的宽高比，以一颗棋子的高度为基准
- 响应 `resizeEvent()`，更新棋子高度，重绘 scene
- 根据总的宽高比设定窗口宽度

##### 更新

另外，毕竟不可能接受所有界面更新都用重绘的办法解决，对于不是窗口大小改变这种必须重绘的情况，我们调用组件的方法来实现移动、删除等操作，这需要我们储存对应的组件（指针）。

#### 事件响应

在我们的架构设计中，理想状态下 UI 由 Model 状态唯一确定，即存在确定的 `f` 使得：

```
View = f(state)
```

简单起见，稍微更改一下上面的表达式：

```
View = f(state_mutations)
```

我们需要做的，是实现这样的函数 `f`，在 Model 某个部分发生改变时，将改变应用到 UI 上。

在 `ChessGame` 中，当其状态变化时会发送信号：

```c++
signals:
    void didUpdateChesses();
    void chessDidFlip(const ChessPoint &pos);
    void chessDidMove(const ChessPoint &source, const ChessPoint &dest);
    void chessDidRemoved(const ChessPoint &point);
    void stateDidChange(State state, State oldState);
    void indexDidChange();
    void remainingTimeDidChange(int seconds);
    void thisPlayerDidTimeout(int times);
    void anotherPlayerDidTimeout(int times);
    void didStarted();
```

对应 View，我们只需响应这些改变。例如，在 `ChessboardScene` 中：

```c++
private slots:
    void chessGameDidUpdateChesses();
    void chessGameDidFlipChess(const ChessPoint &pos);
    void chessGameDidMoveChess(const ChessPoint &source, const ChessPoint &dest);
    void chessGameDidRemoveChess(const ChessPoint &pos);
    void chessGameDidTimeout();
```

这样，就可以在每次游戏更新时，根据 Model 更新正确的 UI。

### 操作的转发：通过 View Model

应用的关键是执行游戏的各种操作。View Model（即 `ChessGameManager`）在这里起到的是一个转发双方操作的作用。

下图说明了这一点是如何达到的：

![https://mermaid-js.github.io/mermaid-live-editor/edit/#eyJjb2RlIjoiZ3JhcGggTFJcblxubShNYW5hZ2VyKVxubihOZXR3b3JrKVxudihWaWV3KVxuZyhHYW1lKVxuXG52IC0tIEFjdGlvbiAtLT4gbVxubSAtLSBGb3J3YXJkIEFjdGlvbiAtLT4gZ1xubSAtLSBGb3J3YXJkIEFjdGlvbiBEYXRhIC0tPm5cbmcgLS4gU2lnbmFscyAuLT4gdlxuXG5tMShNYW5hZ2VyKVxubjEoTmV0d29yaylcbnYxKFZpZXcpXG5nMShHYW1lKVxuXG5uMSAtLSBBY3Rpb24gRGF0YSAtLT4gbTFcbm0xIC0tIEZvcndhcmQgQWN0aW9uIC0tPiBnMVxuZzEgLS4gU2lnbmFscyAuLT4gdjEiLCJtZXJtYWlkIjoie1xuICBcInRoZW1lXCI6IFwiZGVmYXVsdFwiXG59IiwidXBkYXRlRWRpdG9yIjpmYWxzZSwiYXV0b1N5bmMiOnRydWUsInVwZGF0ZURpYWdyYW0iOmZhbHNlfQ](https://mermaid.ink/svg/eyJjb2RlIjoiZ3JhcGggTFJcblxubShNYW5hZ2VyKVxubihOZXR3b3JrKVxudihWaWV3KVxuZyhHYW1lKVxuXG52IC0tIEFjdGlvbiAtLT4gbVxubSAtLSBGb3J3YXJkIEFjdGlvbiAtLT4gZ1xubSAtLSBGb3J3YXJkIEFjdGlvbiBEYXRhIC0tPm5cbmcgLS4gU2lnbmFscyAuLT4gdlxuXG5tMShNYW5hZ2VyKVxubjEoTmV0d29yaylcbnYxKFZpZXcpXG5nMShHYW1lKVxuXG5uMSAtLSBBY3Rpb24gRGF0YSAtLT4gbTFcbm0xIC0tIEZvcndhcmQgQWN0aW9uIC0tPiBnMVxuZzEgLS4gU2lnbmFscyAuLT4gdjEiLCJtZXJtYWlkIjp7InRoZW1lIjoiZGVmYXVsdCJ9LCJ1cGRhdGVFZGl0b3IiOmZhbHNlLCJhdXRvU3luYyI6dHJ1ZSwidXBkYXRlRGlhZ3JhbSI6ZmFsc2V9)

当本用户执行操作时，Manager 将此操作转发给 Game，同时通过网络转发给对方用户；当对方用户执行操作时，Manager 将接收到的操作数据转发给 Game。View 通过 Game 的信号即可更新。

通过这种设计，简化了处理异步事件的过程：View 只需将操作发送给 Manager，同时只需接受 Game 的信号，就可以正确地在两端执行操作并更新 UI。

由此，我们不难看到，View 不与 Game 直接沟通，是非常必要的。
