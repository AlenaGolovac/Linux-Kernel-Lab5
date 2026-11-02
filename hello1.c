

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Linux kernel module hello1 - Lab 5");

struct hello_event {
    struct list_head list;
    ktime_t duration;
};

static LIST_HEAD(event_list);

void print_hello(void)
{
    struct hello_event *event;
    ktime_t start, end;

    event = kmalloc(sizeof(*event), GFP_KERNEL);
    if (!event) {
        pr_err("Failed to allocate memory\n");
        return;
    }

    start = ktime_get();
    pr_info("Hello, world!\n");
    end = ktime_get();

    event->duration = ktime_sub(end, start);
    list_add_tail(&event->list, &event_list);
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("hello1 module loaded\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct hello_event *item, *tmp;

    list_for_each_entry_safe(item, tmp,
                             &event_list, list) {
        pr_info("Print duration: %lld ns\n",
                (long long)ktime_to_ns(item->duration));

        list_del(&item->list);
        kfree(item);
    }

    pr_info("hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);



