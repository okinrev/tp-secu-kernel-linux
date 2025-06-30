int kexpita_init(void);
void kexpita_exit(void);

int kexpita_open(struct inode *inode, struct file *file);
int kexpita_release(struct inode *inode, struct file *file);
static ssize_t kexpita_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos);
static ssize_t kexpita_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos);