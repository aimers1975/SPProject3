#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
struct child_process* get_child_process(pid_t);
struct opened_file* get_opened_file(int fd);

#endif /* userprog/syscall.h */
