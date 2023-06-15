# Report - Synchronization

# Analysis ad Evaluation

![](/Users/yezixiao/Library/Application%20Support/marktext/images/2023-06-07-09-38-09-image.png)![](/Users/yezixiao/Library/Application%20Support/marktext/images/2023-06-07-09-46-40-image.png)

![](/Users/yezixiao/Library/Application%20Support/marktext/images/2023-06-07-09-50-32-image.png)



- With number of threads increasing, optimistic synchronizaiton has lower clock cycles, which has the best performance.

- With number of threads increasing, lock coupling and lock coupling RW have less instructions, but still higher than other methods. I think the reason is that, in those two methods, there are more times to lock and unlock corresponding mutex.

- We can see from the figures, lock sometimes can be expensive. More times calling the lock() and unlock() function, More instructions and cycles will be executed. Other threads have to wait until the current thread unlock() a mutex.

- In general, optimistic can provide the best performance. 
