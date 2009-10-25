#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 1024

char *get_line_buf(int fd, char *buf, unsigned int *remain, char **save)
{
#define READ_SIZE  8 //(BUF_SIZE - 8)
  //  static char save;
  unsigned int rem = *remain;
  char *p = *save;
  if (p == NULL) {
    rem = 0;
  }
  if (rem == 0) {
    printf("***initial read***\n");
    rem = read(fd, buf, READ_SIZE); /* read more */
    if (rem <= 0) {
      *save = NULL;
      return (char*) NULL;   
    }
    p = buf;
    buf[rem] = '\0';
  }
  
  char *end = strchr(p, '\n');
  if (!end) {			/* no more \n in current buffer */
    memcpy(buf, p, rem);
    p = buf;
    int new_read = read(fd, buf+rem, READ_SIZE-rem); /* need more data */
    printf("*** read more data *** %d\n", new_read);
    if (new_read <= 0) {		/* no more input data */
      printf("*** no more data\n");
      end = buf + rem;
    } else {
      rem += new_read;
      printf("rem = %d\n", rem);
      end = strchr(buf, '\n');
      if (!end) {
	printf("*** line end not found\n");
	printf(buf);
	end = buf + rem;
      }
    }     
  }
  
  int len;
  if (*end == '\0')
    len = end - p;
  else
    len = end - p + 1;
  *end = '\0';
  rem -= len;
  printf("rem = %d, len = %d\n", rem, len);
  if (rem < 0) {
    printf("**** rem < 0 **** \n");
  }
  
  *remain = rem;
  *save = end + 1;
  return p;
}

void test2()
{
  char buffer[BUF_SIZE];
  int rlen, wlen;
  int fd1, fd2;

  fd1 = open("./text.in", O_RDONLY);
  fd2 = open("./text.out", O_WRONLY|O_TRUNC|O_CREAT, 0666);
  if (fd1 < 0 || fd2 < 0) {
    printf("open file error %d %d\n", fd1, fd2);
    return ;
  }
  
  unsigned int remain = 0;
  char *next = NULL, *line;
  while ((line = get_line_buf(fd1, buffer, &remain, &next)) != NULL) {
    printf("%s\n", line);
    printf("remain %d, next %p\n", remain, next);
    write(fd2, line, strlen(line));
    write(fd2, "\n", 1);
  }

  close(fd1);
  close(fd2);
}


char *rfind_char(char *start, char *end, char c)
{
  while ((end >= start) && (*end != c))
    end--;

  return end<start?NULL:end;
}

#define start_tag "--START--"
#define  end_tag "--END--"
void test1()
{
  char buffer[1024];
  int rlen, wlen;
  int fd1, fd2;

  fd1 = open("./text.in", O_RDONLY);
  fd2 = open("./text.out", O_WRONLY|O_TRUNC|O_CREAT, 0666);
  if (fd1 < 0 || fd2 < 0) {
    printf("open file error %d %d\n", fd1, fd2);
    return ;
  }
  
  int remain = 0;
  int in_section = 0;		/* 0 not in; 2 in; 1 in, the last one */
  while ((rlen = read(fd1, buffer+remain, 1000-remain)) > 0) {
    char *token, *dec_end;
    char *end = buffer + remain + rlen - 1;

    *(end + 1) = '\0';
    if ((token = strstr(buffer, start_tag)) != NULL) {
      token += strlen(start_tag);
      memcpy(buffer, token, end - token + 1);
      end = buffer + (end - token);
      *(end + 1) = '\0';
      in_section = 2;
    }
    if ((token = strstr(buffer, end_tag)) != NULL) {
      *token = '\0';
      in_section = 1;
    }

    if (in_section) {
      char save;
      dec_end = rfind_char(buffer, end, '\n');
      printf("dec_end %p\n", dec_end);
      if (!dec_end)
	dec_end = end;

      dec_end++;
      save = *dec_end;
      *dec_end = '\0';

      wlen = strlen(buffer);
      printf("rlen %d, wlen %d\n", rlen, wlen);
      printf("%s", buffer);
      write(fd2, buffer, wlen);

      remain = end - dec_end + 1; 
      *dec_end = save;
      printf("save %d %c\n", save, save);
      printf("(%s)\n", dec_end);
      if (remain > 0)
	memcpy(buffer, dec_end, remain);
    
      printf("remian %d, next read %d\n", remain, 1000 - remain);
      printf("-------------------\n\n");
      if (in_section == 1)
	break;
    }
  }
  
}

int main(int argc, char **argv)
{
  //  test1();
  test2();
  return 0;
}
