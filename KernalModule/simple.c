

/*Timothy Lee
  04/07/15
  CS4540

Part III
—
Implement Simple Kernel Module Using Illustrative Program
Use the code from Section I.1 to create the kernel module and to load and unload the module (proceed
through the steps described above).
Be sure to display the contents of the kernel log buffer (using dmesg) in your report for this assignment. */



#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

//Module info
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG"); 

/* This function is called when the module is loaded. */
int __init simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	return 0;
}


int __exit simple_exit(void)          // exit point function
{
	printk(KERN_INFO "Removing Module\n");
	return 0;
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);
