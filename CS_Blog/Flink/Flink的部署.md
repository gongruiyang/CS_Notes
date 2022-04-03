# Flink目录结构解析

bin目录

* start-cluster.sh：启动集群
* stop-cluster.sh：关闭集群
* jobmanager.sh：作业管理者（Driver）
* taskmanager.sh：任务管理者（Executor）
* yarn-session.sh：yarn模式下部署相关
* flink：启动集群之后，用于 提交\取消\查看 作业的命令

conf目录

* flink-conf.yaml：Flink集群配置核心文件

* masters：jobmanager
* slaves：taskmanager

# Flink conf文件解读

```apl
jobmanager.rpc.address 	# RPC的IP地址
jobmanager.rpc.port 	# RPC的端口号
jobmanager.heap.size	# 为jobmanager的JVM分配的堆内存
taskmanager.memory.process.size		# 为taskmanager分配的总进程内存
taskmanager.numberOfTaskSlots		# 一个taskmanager提供的并行执行流的槽位个数，即最大同时执行的线程个数
parallelism.default		# 运行时真正的线程的个数
```



# 启动Flink集群

1. 启动FLink集群

```she
./bin/start-cluster.sh
```

2. 查看是否启动成功

```shell
jps
```

3. 查看`http://localhost:8081/`网页

# 任务上传与配置

1. 将 maven 程序打包成 jar 包
2. 上传 task
3. 点击任务，进行配置

![image-20211005232345592](https://i.loli.net/2021/10/05/6VLhvQTgeEbUBlG.png)