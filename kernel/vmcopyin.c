#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

//
// This file contains copyin_new() and copyinstr_new(), the
// replacements for copyin and coyinstr in vm.c.
//
void u2kpgtblcopy1(pagetable_t upagetable, pagetable_t kpagetable, uint64 oldsz, uint64 newsz) {
  oldsz = PGROUNDUP(oldsz);
  for (uint64 i = oldsz; i < newsz; i += PGSIZE) {
    pte_t* pte_from = walk(upagetable, i, 0);
    pte_t* pte_to = walk(kpagetable, i, 1);
    if(pte_from == 0) panic("u2kvmcopy: src pte do not exist");
    if(pte_to == 0) panic("u2kvmcopy: dest pte walk fail");
    uint64 pa = PTE2PA(*pte_from);
    uint flag = (PTE_FLAGS(*pte_from)) & (~PTE_U);
    *pte_to = PA2PTE(pa) | flag;
  }
}
void u2kpgtblcopy(pagetable_t upgtbl, pagetable_t kpgtbl, uint64 start, uint64 end){
  start = PGROUNDUP(start);
  for (uint64 i = start; i < end; i+=PGSIZE)
  {
    pte_t *upte = walk(upgtbl, i, 0);
    pte_t *kpte = walk(kpgtbl, i, 1);
    if (upte == 0 || kpte == 0)
    {
      panic("panic:u2kpgtblcopy");
    }
    uint flag = (PTE_FLAGS(*upte)) & (~PTE_U);
    *kpte = PTE2PA(*upte) | flag;
  }
  
}
static struct stats {
  int ncopyin;
  int ncopyinstr;
} stats;

int
statscopyin(char *buf, int sz) {
  int n;
  n = snprintf(buf, sz, "copyin: %d\n", stats.ncopyin);
  n += snprintf(buf+n, sz, "copyinstr: %d\n", stats.ncopyinstr);
  return n;
}

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
int
copyin_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  struct proc *p = myproc();

  if (srcva >= p->sz || srcva+len >= p->sz || srcva+len < srcva)
    return -1;
  memmove((void *) dst, (void *)srcva, len);
  stats.ncopyin++;   // XXX lock
  return 0;
}

// Copy a null-terminated string from user to kernel.
// Copy bytes to dst from virtual address srcva in a given page table,
// until a '\0', or max.
// Return 0 on success, -1 on error.
int
copyinstr_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
  struct proc *p = myproc();
  char *s = (char *) srcva;
  
  stats.ncopyinstr++;   // XXX lock
  for(int i = 0; i < max && srcva + i < p->sz; i++){
    dst[i] = s[i];
    if(s[i] == '\0')
      return 0;
  }
  return -1;
}
