#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/capability.h>
#include <krad/system/priority.h>
#include <linux/sched.h>
#include <krad/system/linux_priority.c>
#include <krad/app/debug.h>

typedef enum {
  KR_CAPS_UNKNOWN,
  KR_CAPS_NONE,
  KR_CAPS_REALTIME,
  KR_CAPS_REALTIME_IO
} kr_caps;

typedef struct {
  int policy;
  int nice;
  int rt_priority;
  int io_priority;
  int io_class;
} kr_priority_spec;

static kr_caps caps_level = KR_CAPS_UNKNOWN;

static const kr_priority_spec kr_priority_level[] = {
  [KR_PRIORITY_NORMAL] = {
    .policy = SCHED_OTHER,
    .nice = 0,
    .io_priority = 4,
    .io_class = IOPRIO_CLASS_BE
  },
  [KR_PRIORITY_REALTIME] = {
    .policy = SCHED_FIFO,
    .rt_priority = 20,
    .io_priority = 4,
    .io_class = IOPRIO_CLASS_RT
  },
  [KR_PRIORITY_REALTIME_IO] = {
    .policy = SCHED_FIFO,
    .rt_priority = 30,
    .io_priority = 0,
    .io_class = IOPRIO_CLASS_RT
  },
  [KR_PRIORITY_NONINTERACTIVE] = {
    .policy = SCHED_OTHER,
    .nice = 5,
    .io_priority = 7,
    .io_class = IOPRIO_CLASS_BE
  },
  [KR_PRIORITY_TRANSCODER] = {
    .policy = SCHED_BATCH,
    .nice = 10,
    .io_priority = 7,
    .io_class = IOPRIO_CLASS_BE
  }
};

/*
static const char *kr_ioprio_class[] = {
  [IOPRIO_CLASS_NONE] = "NONE",
  [IOPRIO_CLASS_IDLE] = "IDLE",
  [IOPRIO_CLASS_BE] = "BE",
  [IOPRIO_CLASS_RT] = "RT",
};

static const char *kr_sched_policy[] = {
  [SCHED_OTHER] = "OTHER",
  [SCHED_BATCH] = "BATCH",
  [SCHED_IDLE] = "IDLE",
  [SCHED_ISO] = "ISO",
  [SCHED_FIFO] = "FIFO",
  [SCHED_RR] = "RR"
};
*/

static void kr_caps_debug() {
  switch (caps_level) {
    case KR_CAPS_UNKNOWN:
      printk("Cap: Unknown");
      break;
    case KR_CAPS_NONE:
      printk("Caps: None");
      break;
    case KR_CAPS_REALTIME:
      printk("Caps: Realtime");
      break;
    case KR_CAPS_REALTIME_IO:
      printk("Caps: Realtime and Realtime IO");
      break;
  }
}

static void kr_caps_get() {
  int ret;
  cap_t caps;
  cap_flag_value_t permitted;
  cap_flag_value_t effective;
  caps_level = KR_CAPS_NONE;
  caps = cap_get_proc();
  if (!caps) return;
  ret = cap_get_flag(caps, CAP_SYS_NICE, CAP_EFFECTIVE, &effective);
  if (ret) return;
  ret = cap_get_flag(caps, CAP_SYS_NICE, CAP_PERMITTED, &permitted);
  if (ret) return;
  if (effective && permitted) caps_level = KR_CAPS_REALTIME;
  ret = cap_get_flag(caps, CAP_SYS_ADMIN, CAP_EFFECTIVE, &effective);
  if (ret) return;
  ret = cap_get_flag(caps, CAP_SYS_ADMIN, CAP_PERMITTED, &permitted);
  if (ret) return;
  if (effective && permitted) caps_level = KR_CAPS_REALTIME_IO;
}

void kr_priority_debug(pid_t tid) {
  int ret;
  int which;
  int who;
  //int ioprio;
  int class;
  //const char *class_name;
  struct sched_attr attr;
  //const char *sched_policy_name;
  ret = 0;
  memset(&attr, 0, sizeof(attr));
  //class_name = "";
  //ioprio = 0;
  kr_caps_get();
  kr_caps_debug();
  ret = sched_getattr(tid, &attr, sizeof(struct sched_attr), 0);
  //sched_policy_name = kr_sched_policy[attr.sched_policy];
  if (ret == -1) {
    printke("sched_getattr: %s", strerror(errno));
  }
  which = IOPRIO_WHO_PROCESS;
  who = tid;
  ret = ioprio_get(which, who);
  if (ret == -1) {
    printke("ioprio_get: %s", strerror(errno));
  } else {
    //ioprio = IOPRIO_PRIO_DATA(ret);
    class = IOPRIO_PRIO_CLASS(ret);
    if (class == IOPRIO_CLASS_NONE) {
      if (attr.sched_policy == SCHED_IDLE)
        class = IOPRIO_CLASS_IDLE;
      else if (attr.sched_policy == SCHED_FIFO
       || attr.sched_policy == SCHED_RR)
        class = IOPRIO_CLASS_RT;
      else
        class = IOPRIO_CLASS_BE;
    }
    //class_name = kr_ioprio_class[class];
    /*printk("sched: [ policy: '%s' nice: '%d' priority: '%d' ]"
     " ioprio: [ class: '%s', prio: '%d' ]",  sched_policy_name,
     attr.sched_nice, attr.sched_priority, class_name, ioprio);*/
  }
}

int kr_priority_set(kr_priority priority) {
  int ret;
  int fail;
  int ioprio_val;
  struct sched_attr attr;
  const kr_priority_spec *prio;
  if ((priority < KR_PRIORITY_MIN) || (priority > KR_PRIORITY_MAX)) {
    printke("Priority: Invalid value %d", priority);
    return -1;
  }
  fail = 0;
  prio = &kr_priority_level[priority];
  memset(&attr, 0, sizeof(attr));
  attr.size = sizeof(attr);
  attr.sched_policy = prio->policy;
  attr.sched_nice = prio->nice;
  attr.sched_priority = prio->rt_priority;
  ret = sched_setattr(0, &attr, 0);
  if (ret == -1) {
    fail = errno;
    printke("Priority: sched_setattr %s", strerror(fail));
  }
  if (!((priority == KR_PRIORITY_REALTIME ||
   priority == KR_PRIORITY_REALTIME_IO) &&
   caps_level != KR_CAPS_REALTIME_IO)) {
    ioprio_val = IOPRIO_PRIO_VALUE(prio->io_class, prio->io_priority);
    ret = ioprio_set(IOPRIO_WHO_PROCESS, 0, ioprio_val);
    if (ret == -1) {
      fail = errno;
      printke("Priority: ioprio_set %s", strerror(fail));
    }
  }
  if (fail) {
    if (caps_level == KR_CAPS_UNKNOWN) kr_caps_get();
  }
  return 0;
}
