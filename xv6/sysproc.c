#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int status;
  //Comprobar los datos de entrada
  if(argint(0,&status) < 0)
  	return -1;
  //lamada a exit con el estado desplazado 	
  exit(status << 8);	

  return 0;  // not reached
}

int
sys_wait(void)
{
  int * status;
  
  //Comprobar los datos de entrada
  if(argptr(0,(void*)&status, sizeof(int*))<0) 
  	return -1;
 //Llamada a wait con el puntero al estado
  return wait(status);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
    
  addr = myproc()->sz;
  
  if(n<0 && growproc(n) < 0)
    return -1;
    
  if(n>=0)  
    myproc()->sz += n;  
    
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// return date
int
sys_date(void)
{
  struct rtcdate* r;
  
  if(argptr(0,(void**)&r, sizeof(struct rtcdate)) < 0)
  	return -1;
  	
  cmostime(r);
  return 0;	   
}


// return prio from pid process
int
sys_getprio(void)
{
  int pid;
  
  if(argint(0, &pid) < 0)
    return -1;
    
  return getprio(pid);
}

// set prio to pid process
int
sys_setprio(void)
{
  int pid;
  int prio;
  
  if(argint(0, &pid) < 0 || argint(1, &prio) < 0 || argint(1, &prio) > 9)
    return -1;
  
    
  return setprio(pid,prio);
}

// return physic memory assigned to process
int
sys_phmem(void)
{
  int pid;
  
  if(argint(0, &pid) < 0)
    return -1;
    
  return phmem(pid);
}




