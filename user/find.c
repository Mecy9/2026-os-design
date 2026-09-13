#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"

static char*
base_name(char *path)
{
  char *p = path + strlen(path);
  while(p > path && p[-1] == '/')
    p--;
  while(p > path && p[-1] != '/')
    p--;
  return p;
}

static void
run_command(char *path, int command_argc, char **command_argv)
{
  char *args[MAXARG];

  if(command_argc + 2 > MAXARG){
    fprintf(2, "find: too many arguments for -exec\n");
    return;
  }

  for(int i = 0; i < command_argc; i++)
    args[i] = command_argv[i];
  args[command_argc] = path;
  args[command_argc + 1] = 0;

  int pid = fork();
  if(pid < 0){
    fprintf(2, "find: fork failed\n");
    return;
  }
  if(pid == 0){
    exec(args[0], args);
    fprintf(2, "find: cannot exec %s\n", args[0]);
    exit(1);
  }
  wait(0);
}

static void
find_path(char *path, char *name, int command_argc, char **command_argv)
{
  char child[512];
  struct stat st;
  struct dirent de;
  int fd = open(path, O_RDONLY);

  if(fd < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR){
    if(strcmp(base_name(path), name) == 0){
      if(command_argc)
        run_command(path, command_argc, command_argv);
      else
        printf("%s\n", path);
    }
    close(fd);
    return;
  }

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(child)){
      fprintf(2, "find: path too long\n");
      continue;
    }

    strcpy(child, path);
    char *p = child + strlen(child);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(child, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", child);
      continue;
    }
    if(st.type == T_DIR){
      find_path(child, name, command_argc, command_argv);
    } else if(strcmp(de.name, name) == 0){
      if(command_argc)
        run_command(child, command_argc, command_argv);
      else
        printf("%s\n", child);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  int command_argc = 0;
  char **command_argv = 0;

  if(argc < 3){
    fprintf(2, "usage: find path name [-exec command ...]\n");
    exit(1);
  }
  if(argc > 3){
    if(argc < 5 || strcmp(argv[3], "-exec") != 0){
      fprintf(2, "usage: find path name [-exec command ...]\n");
      exit(1);
    }
    command_argc = argc - 4;
    command_argv = &argv[4];
  }

  find_path(argv[1], argv[2], command_argc, command_argv);
  exit(0);
}
