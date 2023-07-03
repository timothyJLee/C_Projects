
/*Timothy Lee
  04/07/15
  CS4540

Part IV
—
Implement Linked List Kernel Module Entry point.  In the module entry point, create a linked list containing six struct student elements for a single students, that is with the same value of studentNumber (and different values of courseCredit
and grade). Traverse the linked list and output its contents to the kernel log buffer.Invoke the dmesg command to ensure the list is properly constructed once the kernel module has been loaded.
Exit point.
In the module exit point, delete the elements from the linked list and return the free memoryback to the kernel. Again, invoke the dmesg command to check that the list has been removed once the kernel module has been unloaded. */


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/random.h>
#include <linux/ioctl.h>


struct student {
	int studentNumber;
	int courseCredit;
	int grade;

	struct list_head list;   // For holding prev and next
};

static LIST_HEAD(student_list);  // macro for initializing list

int __init ProjectA4_init(void)   //Entry point
{
	int i, r = 0;
	struct student *newstudentptr, *ptr;

	printk(KERN_INFO "Loading Module\n");

	for(i=0;i<5;i++) // For loop initializing 5 student structs
	{
		newstudentptr = kmalloc(sizeof(*newstudentptr), GFP_KERNEL);
		newstudentptr->studentNumber = 760120495;

		get_random_bytes(&r, sizeof(int)); // for generating random # in kernel
		r = abs(r % 4);
		newstudentptr->courseCredit = r;

		get_random_bytes(&r, sizeof(int));
		r = abs(r % 5);
		newstudentptr->grade = r;
		
		INIT_LIST_HEAD(&newstudentptr->list);   // initialize list head
		list_add_tail(&newstudentptr->list, &student_list); // add to tail
	}

	
	list_for_each_entry(ptr, &student_list, list) { // just list no delete
		/* on each iteration ptr points */
		/* to the next student struct */
		printk("Student Number: %d Course Credit %d Grade %d\n", ptr->studentNumber, ptr->courseCredit, ptr->grade);
	}
	return 0;
}




int __exit ProjectA4_exit(void) 
{
	struct student *ptr, *next;
	list_for_each_entry_safe(ptr, next, &student_list, list) {
		/* on each iteration ptr points */
		/* to the next student struct */
		list_del(&ptr->list); // delete node
		kfree(ptr); // free memory
	}	
	printk(KERN_INFO "Removing Module\n");
	return 0;
}




/* Macros for registering module entry and exit points. */
module_init(ProjectA4_init);
module_exit(ProjectA4_exit);

//Module Info
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ProjectA4 Module");
MODULE_AUTHOR("SGG"); 














