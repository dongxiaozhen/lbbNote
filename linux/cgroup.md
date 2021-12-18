
#cpu
The bandwidth allowed for a group is specified using a quota and period. Within
each given "period" (microseconds), a group is allowed to consume only up to
"quota" microseconds of CPU time.  

Examples
--------
1. Limit a group to 1 CPU worth of runtime.

	If period is 250ms and quota is also 250ms, the group will get
	1 CPU worth of runtime every 250ms.

	# echo 250000 > cpu.cfs_quota_us /* quota = 250ms */
	# echo 250000 > cpu.cfs_period_us /* period = 250ms */

2. Limit a group to 2 CPUs worth of runtime on a multi-CPU machine.

	With 500ms period and 1000ms quota, the group can get 2 CPUs worth of
	runtime every 500ms.

	# echo 1000000 > cpu.cfs_quota_us /* quota = 1000ms */
	# echo 500000 > cpu.cfs_period_us /* period = 500ms */

	The larger period here allows for increased burst capacity.

3. Limit a group to 20% of 1 CPU.

	With 50ms period, 10ms quota will be equivalent to 20% of 1 CPU.

	# echo 10000 > cpu.cfs_quota_us /* quota = 10ms */
	# echo 50000 > cpu.cfs_period_us /* period = 50ms */

	By using a small period here we are ensuring a consistent latency
	response at the expense of burst capacity.


#cpuset  https://www.kernel.org/doc/Documentation/cgroup-v1/cpusets.txt
Cpusets provide a mechanism for assigning a set of CPUs and Memory Nodes to a set of tasks.  
Cpusets constrain the CPU and Memory placement of tasks to only the resources within a task's current cpuset.  They form a nested hierarchy visible in a virtual file system.  These are the essential hooks, beyond what is already present, required to manage dynamic job placement on large systems.  

Each cpuset is represented by a directory in the cgroup file system
containing (on top of the standard cgroup files) the following
files describing that cpuset:

 - cpuset.cpus: list of CPUs in that cpuset
 - cpuset.mems: list of Memory Nodes in that cpuset
 - cpuset.memory_migrate flag: if set, move pages to cpusets nodes
 - cpuset.cpu_exclusive flag: is cpu placement exclusive?
 - cpuset.mem_exclusive flag: is memory placement exclusive?
 - cpuset.mem_hardwall flag:  is memory allocation hardwalled
 - cpuset.memory_pressure: measure of how much paging pressure in cpuset
 - cpuset.memory_spread_page flag: if set, spread page cache evenly on allowed nodes
 - cpuset.memory_spread_slab flag: if set, spread slab cache evenly on allowed nodes
 - cpuset.sched_load_balance flag: if set, load balance within CPUs on that cpuset
 - cpuset.sched_relax_domain_level: the searching range when migrating tasks

#cpuacct  https://www.kernel.org/doc/Documentation/cgroup-v1/cpuacct.txt
cpuacct controller uses percpu_counter interface to collect user and
system times. This has two side effects:

- It is theoretically possible to see wrong values for user and system times.
  This is because percpu_counter_read() on 32bit systems isn't safe
  against concurrent writes.
- It is possible to see slightly outdated values for user and system times
  due to the batch processing nature of percpu_counter.

