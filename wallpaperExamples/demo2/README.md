# Demo 2

���ǵڶ��� Demo��չʾ��һ��ʱ�䵹��ʱ��Ч��������Ч����

![demo2 Ч��](../../ScreenShots/demo2.gif)

���У�����ʱ��Ĳ���ʹ���� **wasm** ��������ֻ����Ϊд��� demo ʱǡ�ö�  wasm �Ƚϸ���Ȥ���� C++ �е� `ctime` ������ԱȽ���Ϥ��ʵ��û�б�Ҫ����һ�����Ҳ�������Ը��졣����

Ϊ����Ŀ�ļ���ԣ���һ������Ҫ�ֶ�ʹ�� **Emscripten** �� `time.cpp` ����Ϊ `time.js` �ļ���������˵��

������ȷ����װ�� Emscripten ������Ȼ�����

```powershell
emcc time.cpp -o time.js -s WASM=1 -s SINGLE_FILE -s EXPORTED_FUNCTIONS="['_getNowTime', '_modifyTime', '_freeBuf', '_diffTime', '_free']"
```

�Ա��롣

���ң�����ʹ���� **Bootstrap** �� **jQuery** �� **CDN**��ͬ��ֻ��Ϊ�˷��㣬��������Ч�ʲ�û�кô����������ø� demo ʱ��ȷ���������ӡ�