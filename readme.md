# Thread-Pool

本專案基於 [docker-c-application](https://github.com/eastmoon/docker-c-application) 設立，並規劃與設計執行緒池功能，以符合下述內容：

+ 指定、清除要處理的函數物件
    - 若清除則執行緒應皆停止運作並清除
+ 指定執行緒數，min、max
+ 操作執行中的執行緒池
    - 開啟、 暫停、關閉
+ 處理訊息柱列
		- threshold 柱列閥值，超過閥值應逐次增加執行緒，直到執行緒最大值
		- alert 柱列告警，超過告警值應記錄執行緒不足處理

## Algorithm

#### Thread Pool

+ use pool worker to assign data and worker
+ add will push data into queue
    - if queue have more then threshold data, grow up worker array
+ each data object have member variable which for worker calculate, and result object for save calculate result

```
ThreadPool.Data
ThreadPool.Pool
ThreadPool.Worker
```

#### Job Pool

+ assign worker and job number
+ play for start all worker
    - use flag to control worker loop.
+ stop for down all worker
    - use flag to control worker loop.

```
JobPool.Data
JobPool.Pool
JobPool.Worker
```

## 參考
