#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <inttypes.h>
#include <sched.h>

/* CPU Priority */

#define SCHED_NORMAL 0
#define SCHED_BATCH 3
#define SCHED_ISO 4
#define SCHED_IDLE 5

struct sched_attr {
  uint32_t size;
  uint32_t sched_policy;
  uint64_t sched_flags;
  // SCHED_NORMAL, SCHED_BATCH /
  int32_t sched_nice;
  // SCHED_FIFO, SCHED_RR /
  uint32_t sched_priority;
  // SCHED_DEADLINE /
  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

static int
sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
  return syscall(SYS_sched_setattr, pid, attr, flags);
}

static int
sched_getattr(pid_t pid, const struct sched_attr *attr, unsigned int size,
 unsigned int flags)
{
  return syscall(SYS_sched_getattr, pid, attr, size, flags);
}

/* I/O Priority */

enum {
  IOPRIO_CLASS_NONE,
  IOPRIO_CLASS_RT,
  IOPRIO_CLASS_BE,
  IOPRIO_CLASS_IDLE,
};

enum {
  IOPRIO_WHO_PROCESS = 1,
  IOPRIO_WHO_PGRP,
  IOPRIO_WHO_USER,
};

#define IOPRIO_CLASS_SHIFT (13)
#define IOPRIO_PRIO_MASK ((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask) ((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask) ((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data) (((class) << IOPRIO_CLASS_SHIFT) | data)

static inline int ioprio_set(int which, int who, int ioprio)
{
    return syscall(SYS_ioprio_set, which, who, ioprio);
}

static inline int ioprio_get(int which, int who)
{
    return syscall(SYS_ioprio_get, which, who);
}
