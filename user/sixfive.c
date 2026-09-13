#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static int
separator(char c)
{
  return strchr(" -\r\t\n./,", c) != 0;
}

static void
print_if_match(int number, int have_number)
{
  if(have_number && (number % 5 == 0 || number % 6 == 0))
    printf("%d\n", number);
}

static void
scan(int fd)
{
  char c;
  int number = 0;
  int have_number = 0;
  int valid_number = 1;

  while(read(fd, &c, 1) == 1){
    if(separator(c)){
      if(valid_number)
        print_if_match(number, have_number);
      number = 0;
      have_number = 0;
      valid_number = 1;
    } else if(c >= '0' && c <= '9'){
      if(valid_number){
        have_number = 1;
        number = number * 10 + c - '0';
      }
    } else {
      valid_number = 0;
    }
  }

  if(valid_number)
    print_if_match(number, have_number);
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "usage: sixfive file ...\n");
    exit(1);
  }

  for(int i = 1; i < argc; i++){
    int fd = open(argv[i], O_RDONLY);
    if(fd < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    scan(fd);
    close(fd);
  }

  exit(0);
}
