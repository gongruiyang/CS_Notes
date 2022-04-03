void* srv_master_thread(void* arg) {
	while (Database running normally) {
loop:
		// 数据库启动时，检查是否需要数据库恢复操作
		if (srv_force_recovery >= SRV_FORCE_NO_BACKGROUND) {
			goto suspend_thread;
		}

		sleep(1); 	// master thread sleep 1 second;

		if (服务器是否空闲) {
			srv_master_do_active_tasks();	// 服务器非空闲时
		} else {
			srv_master_do_idle_tasks();	// 服务器空闲时
		}

suspend_thread:
		//挂起 master thread 以在其线程槽 slot 中等待事件。
		srv_main_thread_op_info = "suspending";
		srv_suspend_thread(slot);
		srv_main_thread_op_info = "waiting for server activity";
		os_event_wait(slot->event);

		if (不需要退出所有线程) {
			goto loop;
		}

		my_thread_end();
		os_thread_exit();
		DBUG_RETURN(0);
	}
}

static void srv_master_do_idle_tasks(void) {

	/* MySQL中的 alter table 要求Unix上的表处理程序必须在没有 select 查询后删除表*/
	srv_main_thread_op_info = "doing background drop tables";
	row_drop_tables_for_mysql_in_background();

	/* 确保重做日志文件中有足够的可重用空间：如果不够，就刷新日志缓冲区或创建新的检查点 */
	srv_main_thread_op_info = "checking free log space";
	log_free_check();

	/* Do merge 100% * srv_io_capacity of insert buffer */
	srv_main_thread_op_info = "doing insert buffer merge";
	ibuf_merge_in_background(100);

	/* LRU内存清理：通过逐出 未使用的表 在 LRU表缓存 中腾出空间，最大扫描表长为百分之100 */
	srv_main_thread_op_info = "enforcing dict cache limit";
	srv_master_evict_from_table_cache(100);

	/* 每秒刷新一次日志 */
	srv_sync_log_buffer_in_background();

	/* 设置新的检查点：只向 log file 中写入 LSN，不做脏页刷新 */
	srv_main_thread_op_info = "making checkpoint";
	log_checkpoint(TRUE, FALSE);

}

/*
执行主线程在服务器处于活动状态时应该执行的任务。
有两种类型的任务:
	第一类是在该功能的每次启动时执行的任务。我们假设当服务器处于活动状态时，该函数大约每秒调用一次。
	第二类是按一定间隔执行的任务，例如：清除、dict_LRU清理等。
 */
static void srv_master_do_active_tasks(void)
{
	/* MySQL中的 alter table 要求Unix上的表处理程序可以在没有 select 查询后延迟删除表*/
	srv_main_thread_op_info = "doing background drop tables";
	row_drop_tables_for_mysql_in_background();


	/* 确保重做日志文件中有足够的可重用空间：如果不够，就刷新日志缓冲区或创建新的检查点 */
	srv_main_thread_op_info = "checking free log space";
	log_free_check();

	/* Do merge 5% * srv_io_capacity of insert buffer */
	srv_main_thread_op_info = "doing insert buffer merge";
	ibuf_merge_in_background(5);


	/* 每秒刷新一次日志 */
	srv_main_thread_op_info = "flushing log";
	srv_sync_log_buffer_in_background();

	/* 现在，查看是否需要执行按 定义的时间间隔执行 的各种任务 */
	if (cur_time % 47 == 0) {
		/* LRU内存清理：通过逐出 未使用的表 在 LRU表缓存 中腾出空间，最大扫描表长为百分之50 */
		srv_main_thread_op_info = "enforcing dict cache limit";
		srv_master_evict_from_table_cache(50);
	}

	if (cur_time % 7 == 0) {
		/* Make a new checkpoint ：只向 log file 中写入 LSN，不做脏页刷新 */
		srv_main_thread_op_info = "making checkpoint";
		log_checkpoint(TRUE, FALSE);
	}
}