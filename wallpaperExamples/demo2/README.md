# Demo 2

这是第二个 Demo，展示了一个时间倒计时的效果，具体效果：

![demo2 效果](../../ScreenShots/demo2.gif)

其中，计算时间的部分使用了 **wasm** （无他，只是因为写这个 demo 时恰好对  wasm 比较感兴趣，对 C++ 中的 `ctime` 库又相对比较熟悉，实则没有必要，这一点计算也不会明显更快。）。

为了项目的简洁性，这一部分需要手动使用 **Emscripten** 将 `time.cpp` 编译为 `time.js` 文件，具体来说：

首先请确保安装了 Emscripten 环境，然后键入

```powershell
emcc time.cpp -o time.js -s WASM=1 -s SINGLE_FILE -s EXPORTED_FUNCTIONS="['_getNowTime', '_modifyTime', '_freeBuf', '_diffTime', '_free']"
```

以编译。

并且，其中使用了 **Bootstrap** 和 **jQuery** 的 **CDN**（同样只是为了方便，对于运行效率并没有好处。），试用该 demo 时请确保网络连接。