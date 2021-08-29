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

+ worker is a thread object to package method and variable.
    - thread set is a thread handler
    - thread guard is make sure thread has run join or detach
+ pool is a worker runner and controller
    - play to start all thread
    - stop to delete all thread
    - size to change current execute thread

```
ThreadPool.Worker
ThreadPool.ThreadSet
ThreadPool.ThreadGuard
ThreadPool.Pool
```

#### Job Pool

+ job is a thread which run at death mode and infinite loop.
    - job is a Worker
    - job is a Worker action ( feature )
    - job is a closure object  ( feature )
+ pool is a job controller and container.
+ pool need assign worker and job number
    - worker using memory pool to new object.
+ pool can play for start all worker
    - use flag to control worker loop.
+ pool can stop for down all worker
    - use flag to control worker loop.
+ pool can resize


```
JobPool.Worker
JobPool.Pool
```

## 參考
