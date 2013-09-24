#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char ** argv){
    
    
    int f = open(argv[1],O_WRONLY);
    int32_t pos = 0;
    write(f,&pos,sizeof(int32_t));
    close(f);
    return 0;

}
