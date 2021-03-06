#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

typedef uint32_t (*syscall)(uint32_t, uint32_t, uint32_t);
/* syscal is typedef'ed to a function pointers that
   takes three uint32_t's and returns a unit32_t */
syscall syscall_tab[20]; // for all syscalls possible
uint32_t syscall_nArgs[20];

typedef uint32_t pid_t; 

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
    //  printf ("system call!\n");
    //thread_exit ();
    uint32_t callno, args[3], *usp = f->esp;
    

    callno = (uint32_t)(*usp);
    args[0] = (uint32_t)(*(usp+1));
    args[1] = (uint32_t)(*(usp+2));
    args[2] = (uint32_t)(*(usp+3));
    f->eax = syscall_tab[callno](args[0],args[1],args[2]);
}

/*    Terminates Pintos by calling shutdown_power_off() (declared in
      "threads/init.h"). This should be seldom used, because you lose
      some information about possible deadlock situations, etc. */
void sys_halt (void){

}

/*    Terminates the current user program, returning status to the
      kernel. If the process's parent waits for it (see below), this
      is the status that will be returned. Conventionally, a status of
      0 indicates success and nonzero values indicate errors.
 */
void sys_exit (int status){

    printf("%s: exit(%d)\n",thread_name(),status);
    thread_exit();

}

/*    Runs the executable whose name is given in cmd_line, passing any
      given arguments, and returns the new process's program id
      (pid). Must return pid -1, which otherwise should not be a valid
      pid, if the program cannot load or run for any reason. Thus, the
      parent process cannot return from the exec until it knows
      whether the child process successfully loaded its
      executable. You must use appropriate synchronization to ensure
      this.
 */

pid_t sys_exec (const char *cmd_line){

}

/*
    Waits for a child process pid and retrieves the child's exit
    status.

    If pid is still alive, waits until it terminates. Then, returns
    the status that pid passed to exit. If pid did not call exit(),
    but was terminated by the kernel (e.g. killed due to an
    exception), wait(pid) must return -1. It is perfectly legal for a
    parent process to wait for child processes that have already
    terminated by the time the parent calls wait, but the kernel must
    still allow the parent to retrieve its child's exit status, or
    learn that the child was terminated by the kernel.

    wait must fail and return -1 immediately if any of the following
    conditions is true:

        pid does not refer to a direct child of the calling
        process. pid is a direct child of the calling process if and
        only if the calling process received pid as a return value
        from a successful call to exec.

        Note that children are not inherited: if A spawns child B and
        B spawns child process C, then A cannot wait for C, even if B
        is dead. A call to wait(C) by process A must fail. Similarly,
        orphaned processes are not assigned to a new parent if their
        parent process exits before they do.

        The process that calls wait has already called wait on
        pid. That is, a process may wait for any given child at most
        once.

    Processes may spawn any number of children, wait for them in any
    order, and may even exit without having waited for some or all of
    their children. Your design should consider all the ways in which
    waits can occur. All of a process's resources, including its
    struct thread, must be freed whether its parent ever waits for it
    or not, and regardless of whether the child exits before or after
    its parent.

    You must ensure that Pintos does not terminate until the initial
    process exits. The supplied Pintos code tries to do this by
    calling process_wait() (in "userprog/process.c") from main() (in
    "threads/init.c"). We suggest that you implement process_wait()
    according to the comment at the top of the function and then
    implement the wait system call in terms of process_wait().

    Implementing this system call requires considerably more work than
    any of the rest.
*/
int sys_wait (pid_t pid){
}

/*    Creates a new file called file initially initial_size bytes in
      size. Returns true if successful, false otherwise. Creating a
      new file does not open it: opening the new file is a separate
      operation which would require a open system call.
*/
bool sys_create (const char *file, unsigned initial_size){
}

/*
    Deletes the file called file. Returns true if successful, false
    otherwise. A file may be removed regardless of whether it is open
    or closed, and removing an open file does not close it. See
    Removing an Open File, for details.
*/
bool sys_remove (const char *file){
}

/*    Opens the file called file. Returns a nonnegative integer handle
      called a "file descriptor" (fd), or -1 if the file could not be
      opened.

    File descriptors numbered 0 and 1 are reserved for the console: fd
    0 (STDIN_FILENO) is standard input, fd 1 (STDOUT_FILENO) is
    standard output. The open system call will never return either of
    these file descriptors, which are valid as system call arguments
    only as explicitly described below.

    Each process has an independent set of file descriptors. File
    descriptors are not inherited by child processes.

    When a single file is opened more than once, whether by a single
    process or different processes, each open returns a new file
    descriptor. Different file descriptors for a single file are
    closed independently in separate calls to close and they do not
    share a file position.
    */
int sys_open (const char *file){
}

/*    Returns the size, in bytes, of the file open as fd. 
 */
int sys_filesize (int fd){

}
/*
    Reads size bytes from the file open as fd into buffer. Returns the number of bytes actually read (0 at end of file), or -1 if the file could not be read (due to a condition other than end of file). Fd 0 reads from the keyboard using input_getc(). 
*/
int sys_read (int fd, void *buffer, unsigned size){

}

/*
    Writes size bytes from buffer to the open file fd. Returns the
    number of bytes actually written, which may be less than size if
    some bytes could not be written.

    Writing past end-of-file would normally extend the file, but file
    growth is not implemented by the basic file system. The expected
    behavior is to write as many bytes as possible up to end-of-file
    and return the actual number written, or 0 if no bytes could be
    written at all.

    Fd 1 writes to the console. Your code to write to the console
    should write all of buffer in one call to putbuf(), at least as
    long as size is not bigger than a few hundred bytes. (It is
    reasonable to break up larger buffers.) Otherwise, lines of text
    output by different processes may end up interleaved on the
    console, confusing both human readers and our grading scripts.
*/
int sys_write (int fd, const void *buffer, unsigned size){

    if (fd ==1){
	putbuf(buffer,size);
    }
}


/*    Changes the next byte to be read or written in open file fd to
      position, expressed in bytes from the beginning of the
      file. (Thus, a position of 0 is the file's start.)

    A seek past the current end of a file is not an error. A later
    read obtains 0 bytes, indicating end of file. A later write
    extends the file, filling any unwritten gap with zeros. (However,
    in Pintos files have a fixed length until project 4 is complete,
    so writes past end of file will return an error.) These semantics
    are implemented in the file system and do not require any special
    effort in system call implementation.
*/

void sys_seek (int fd, unsigned position){

}

/*
    Returns the position of the next byte to be read or written in
    open file fd, expressed in bytes from the beginning of the file.
*/
unsigned sys_tell (int fd){

}
    
/*
    Closes file descriptor fd. Exiting or terminating a process
    implicitly closes all its open file descriptors, as if by calling
    this function for each one.
*/
void sys_close (int fd){
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");

  //  SYS_HALT,                   /* Halt the operating system. */
  syscall_tab[SYS_HALT] = (syscall)&sys_halt;
  syscall_nArgs[SYS_HALT] = 0;
	
  //    SYS_EXIT,                   /* Terminate this process. */
  syscall_tab[SYS_EXIT] = (syscall)&sys_exit;
  syscall_nArgs[SYS_EXIT] = 1;

  // SYS_EXEC,                   /* Start another process. */
  syscall_tab[SYS_EXEC] = (syscall)&sys_exec;
  syscall_nArgs[SYS_EXEC] = 1;

  
  //  SYS_WAIT,                   /* Wait for a child process to die. */
  syscall_tab[SYS_WAIT] = (syscall)&sys_wait;
  syscall_nArgs[SYS_WAIT] = 1;

  //  SYS_CREATE,                 /* Create a file. */
  syscall_tab[SYS_CREATE] = (syscall)&sys_create;
  syscall_nArgs[SYS_CREATE] = 2;

  //  SYS_REMOVE,                 /* Delete a file. */
  syscall_tab[SYS_REMOVE] = (syscall)&sys_remove;
  syscall_nArgs[SYS_REMOVE] = 1;

  //  SYS_OPEN,                   /* Open a file. */
  syscall_tab[SYS_OPEN] = (syscall)&sys_open;
  syscall_nArgs[SYS_OPEN] = 1;

  // SYS_FILESIZE,               /* Obtain a file's size. */
  syscall_tab[SYS_FILESIZE] = (syscall)&sys_filesize;
  syscall_nArgs[SYS_FILESIZE] = 1;

  //  SYS_READ,                   /* Read from a file. */
  syscall_tab[SYS_READ] = (syscall)&sys_read;
  syscall_nArgs[SYS_READ] = 3;

  //  SYS_WRITE,                  /* Write to a file. */
  syscall_tab[SYS_WRITE] = (syscall)&sys_write;
  syscall_nArgs[SYS_WRITE] = 1;

  //  SYS_SEEK,                   /* Change position in a file. */
  syscall_tab[SYS_SEEK] = (syscall)&sys_seek;
  syscall_nArgs[SYS_SEEK] = 2;

  //  SYS_TELL,                   /* Report current position in a file. */
  syscall_tab[SYS_TELL] = (syscall)&sys_tell;
  syscall_nArgs[SYS_TELL] = 1;

  //  SYS_CLOSE,                  /* Close a file. */
  syscall_tab[SYS_CLOSE] = (syscall)&sys_close;
  syscall_nArgs[SYS_CLOSE] = 1;


}
