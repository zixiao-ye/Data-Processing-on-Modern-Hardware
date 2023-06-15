# Assignment3 - Hardware optimized hash joins

----

## Performance Analysis

![](/Users/yezixiao/Library/Application%20Support/marktext/images/2023-05-23-22-28-20-image.png)

- As number of tuples increasing, the run time increases linearly for the hash join baseline. Because it's just the naive loop which goes through the whole relation to build the hash table.

- Radix join is almost twice as fast as the hash join baseline.

- Software mananged partition + non-temporal wirtes radix join is a little bit faster than the normal partition radix join and multi-pass partition radix join.



## Partitioning Analysis

![](/Users/yezixiao/Library/Application%20Support/marktext/images/2023-05-23-23-18-07-image.png)

- Too many partitions might thrash the TLB cache, causing there is no more performance improvement.

- Using 9 or 10 radix bits for partition can achieve the best performance.

- Using the same number of radix bits, naive partition radix join is faster than multi-pass partition. Because for the naive partion radix join, we only copy each tuple in the realtions once. But for the multi-pass partition, we copy each tuple once in each pass, there are total two pass in my experiment, thus each tuple in the relations is copied twice, which affects the performance of multi-pass partition radix join.


