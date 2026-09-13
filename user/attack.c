#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  const char marker[] = "This may help.";
  const uint64 bytes = 8 * PGSIZE;
  char *memory = sbrk(bytes);

  if(memory == (char*)-1)
    exit(1);

  for(uint64 i = 0; i + 16 < bytes; i++){
    if(memcmp(memory + i, marker, sizeof(marker) - 1) == 0){
      printf("%s\n", memory + i + 16);
      exit(0);
    }
  }

  exit(1);
}
