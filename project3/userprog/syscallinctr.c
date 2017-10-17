#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
typedef uint32_t pid_t;

typedef uint32_t (*syscall)(uint32_t, uint32_t, uint32_t);
/* syscall is typedef'ed to a function pointers that takes three uint32_t
   and returns a unint32_t */

syscall syscall_tab[20];  //for all syscalls possible

uint32_t syscall_nArgs[20]; //

void sys_halt(void) {
}
void sys_exit(int status) {
  printf("%s: exit(%d)\n", thread_name(), status);
}
pid_t sys_exec (const char * *cmd_line) {
  return -1;
}
int sys_wait(pid_t pid) {
  return -1;

}
bool sys_create(const char* file, unsigned initial_size) {
  return 1;
}
bool sys_remove(const char* file) {
  return 1;
}
int sys_open(const char * file) {
  return 1;

}
int sys_filesize(int fd) {
  return 1;
}
int sys_read(int fd, void * buffer, unsigned size) {
  return 1;
}
int sys_write(int fd, const void *buffer, unsigned size) {
  if(fd ==1) {
    putbuf(buffer, size);
  }
}
void sys_seek (int fd, unsigned position) {
}
unsigned sys_tell(int fd) {
  return 1;
}
void sys_close (int fd) {
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  // AMY_R
  // SYS_HALT
  syscall_tab[SYS_HALT] = (syscall)&sys_halt;
  syscall_nArgs[SYS_HALT] = 0;

  // SYS_EXIT
  syscall_tab[SYS_EXIT] = (syscall)&sys_exit;
  syscall_nArgs[SYS_EXIT] = 1;

  // SYS_EXEC
  syscall_tab[SYS_EXEC] = (syscall)&sys_exec;
  syscall_nArgs[SYS_EXEC] = 1;

  // SYS_WAIT
  syscall_tab[SYS_WAIT] = (syscall)&sys_wait;
  syscall_nArgs[SYS_WAIT] = 1;

  // SYS_CREATE 
  syscall_tab[SYS_CREATE] = (syscall)&sys_create;
  syscall_nArgs[SYS_CREATE] =2 ;

  // SYS_REMOVE
  syscall_tab[SYS_REMOVE] = (syscall)&sys_remove;
  syscall_nArgs[SYS_REMOVE] =1 ;

  // SYS_OPEN
  syscall_tab[SYS_OPEN] = (syscall)&sys_open;
  syscall_nArgs[SYS_OPEN] =1 ;

  // SYS_FILESIZE
  syscall_tab[SYS_FILESIZE] = (syscall)&sys_filesize;
  syscall_nArgs[SYS_FILESIZE] =1 ;
 
  // SYS_READ
  syscall_tab[SYS_READ] = (syscall)&sys_read;
  syscall_nArgs[SYS_READ] =3 ;

  // SYS_WRITE
  syscall_tab[SYS_WRITE] = (syscall)&sys_write;
  syscall_nArgs[SYS_WRITE] =1;

//  SYS_SEEK
  syscall_tab[SYS_SEEK] = (syscall)&sys_seek;
  syscall_nArgs[SYS_SEEK] =2;

// SYS_TELL
  syscall_tab[SYS_TELL] = (syscall)&sys_tell;
  syscall_nArgs[SYS_TELL] =1;

//  SYS_CLOSE
  syscall_tab[SYS_CLOSE] = (syscall)&sys_close;
  syscall_nArgs[SYS_CLOSE] =1;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
//  thread_exit ();
  uint32_t callno, args[3], *usp= f->esp;
  


  callno = (uint32_t)(usp); // Get 32 bit number at the top of the stack
  args[0] =(uint32_t)(usp+1);
  args[1] =(uint32_t)(usp+2);
  args[2] =(uint32_t)(usp+3);

  f->eax = syscall_tab[callno](args[0],args[1],args[2]);
}
