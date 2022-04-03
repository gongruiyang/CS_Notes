# 一、Page Cleaner Thread 主循环

> 主要分为以下几部分：
>
> 1. 给 page cleaner thread 分配线程空间
> 2. 给 page cleaner 计数器加一
> 3. 进入 while 循环，进入阻塞状态等待唤醒，线程被唤醒后进行脏页刷新 pc_flush_slot
> 4. 退出 while 循环后进行线程退出处理 

```c++
void* buf_flush_page_cleaner_worker(void* arg)
{
	// 1. 给 page cleaner thread 分配空间
	my_thread_init();
	// 2. 增加 page cleaner work counter 数量
	mutex_enter(&page_cleaner->mutex);	
	page_cleaner->n_workers++;
	mutex_exit(&page_cleaner->mutex);

	// 3. page cleaner thread loop
	while (true) {
		// 等待激活 page cleaner thread 的事件
		os_event_wait(page_cleaner->is_requested);
		// 判断是否需要退出 page cleaner thread
		if (!page_cleaner->is_running) {
			break;
		}
		// 进行 脏页刷新至磁盘
		pc_flush_slot();
	}

	// 4. page cleanner 线程退出
	mutex_enter(&page_cleaner->mutex);
	page_cleaner->n_workers--;
	mutex_exit(&page_cleaner->mutex);
	my_thread_end();
	os_thread_exit();
	OS_THREAD_DUMMY_RETURN;
}
```

# 二、脏页刷新 pc_flush_slot 函数

> 主要做两件事：
>
> 1. 从 LRU List 中进行脏页刷新
> 2. 从 Flush List 中进行脏页刷新

```c++
static ulint pc_flush_slot(void)
{
	// 1. 将指定的 buffer pool instance 中 LRU List 的末端开始刷新脏页至磁盘
	// 		1.1 将LRU尾部的可替换页面放入空闲列表
	// 		1.2 将LRU尾部的脏页刷新到磁盘
	// 		我们扫描每个缓冲池的深度由动态配置参数innodb_LRU_scan_depth控制
	buf_flush_LRU_list(/* arg... */);

	// 2. 将指定的 buffer pool instance 中 Flush List 中按照 批处理 刷新脏页至磁盘
	buf_flush_do_batch(/* arg... */);
}
```

# 三、总结

* page cleanner thread 是唯一能够在 LRU List 中进行脏页刷新并释放出新的 free page 的线程

  

