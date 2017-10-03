#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

struct suojattuX {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int val;
};

suojattuX x = { PTHREAD_MUTEX_INITIALIZER, 
                PTHREAD_COND_INITIALIZER, 0 };

int main(int argc, char** argv) 
{
 	printf("Mennaan mutexin sisälle\n");
    if( pthread_mutex_lock( &x.mutex ) != 0 ) {
        perror("mutex_lock");
        exit(1);
    }
	printf("mutexin sisälla. arvo %d\n", x.val);
    x.val--;
   while( x.val < 0 )
      if( pthread_cond_wait( &x.cond, &x.mutex ) != 0 ) {
        perror("cond_wait");
        exit(1);
      }
	else printf("paastiin suoritukseen waitin jalkee\n");
    // ...
}

void foo()
{
    if( pthread_mutex_lock( &x.mutex ) != 0 ) {
        perror("mutex_lock");
        exit(1);
    }
    x.val++;
      if( pthread_cond_signal( &x.cond ) != 0 ) {
        perror("cond_signal");
        exit(1);
      }
	printf("Lahetettiin signaali foo:sta\n");
}
