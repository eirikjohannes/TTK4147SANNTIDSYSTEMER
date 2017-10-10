#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data);

struct proc_dir_entry* myproc;

MODULE_LICENSE("GPL");

int init_module(void)
{
	printk(KERN_INFO "Hello world 1.\n");
	myproc = create_proc_entry("myproc",0644,NULL);
	myproc->read_proc = procfile_read;
	return 0;
}


int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
	//printk(KERN_INFO "Entering procfile_read");
	if (offset > 0)
	{
		return 0;
	}
	else
	{
		return sprintf(buffer, "Hello world\n");
	}
}
void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye World 1.\n");
	remove_proc_entry("myproc", NULL);
}