# Thread-Pool

採用 C++ 語言，設計執行緒池，並符合以下功能：

+ 指定、清除要處理的函數物件
    - 若清除則執行緒應皆停止運作並清除
+ 指定執行緒數，min、max
+ 操作執行中的執行緒池
    - 開啟、 暫停、關閉
+ 處理訊息柱列
		- threshold 柱列閥值，超過閥值應逐次增加執行緒，直到執行緒最大值
		- alert 柱列告警，超過告警值應記錄執行緒不足處理

## 指令

+ 操作專案的開發、編譯、封裝指令

```
dockerw.bat [dev | build | run | package]
```

+ 開發環境

依據 Docker 啟動開發環境，測試開發環境指令，結束請使用 "Ctrl + Z"

```
dockerw dev
```

+ 編譯專案

依據 Docker 啟動虛擬環境，運用 CMake 編譯專案，其編譯內容會放在 ```cache/publish``` 中

```
dockerw build
```

+ 執行專案

依據 Docker 啟動虛擬環境，編譯後執行 ```cache/publish/bin``` 的輸出檔案

```
dockerw run
```

## 參考
