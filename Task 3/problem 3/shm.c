#include <stdbool.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "shm.h"

/* Generate a human-readable timestamp */
char * getTimeStamp() {
    time_t ltime = time(NULL);
    return strtok(ctime(&ltime), "\n");
}

/* Create Shared Memory Segment
 *
 * Function creates a named SHM file descriptor on the filesystem.
 *
 * @param shname Name of SHM
 * @return file descriptor
 */
int createSHM(char * shname)
{
int fd = shm_open(shname, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);

if(fd != -1)
  ftruncate(fd, sizeof(SHMstruct));
else{
  perror("shm open error");
  exit(1);
}
  return fd;
}
/* Load Shared Memory
 *
 * Function loads an existing named SHM, or gracefully fails
 * when the SHM does not exist.
 *
 * @param shname Name of SHM region
 * @return file descriptor
 */
int loadSHM(char * shname)
{
  return shm_open(shname, O_RDWR,  S_IRWXU);
}

/* Access Existing SHM
 *
 * From an existing SHM file descriptor, allocate the SHMstruct and
 * return its pointer.
 *
 * @param fd File descriptor of existing SHM
 * @return Pointer to SHMstruct
 */
SHMstruct * accessSHM(int fd)
{

return mmap (NULL, sizeof(SHMstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

/* Initialise SHM
 *
 * From an existing SHM file descriptor, initilise the SHMstruct variable.
 *
 * HINT: use accessSHM()
 *
 * @param fd File descriptor of SHM
 * @return Pointer to SHMstruct
 */
SHMstruct * initSHM(int fd, SHMstruct *data)
{

SHMstruct * shm = accessSHM(fd);


  if (shm == MAP_FAILED){
    perror("mmap failed");
    exit(1);
  }

  *shm = *data;

  return shm;
}

/* De-allocate SHMstruct
 *
 * Function de-allocates an already existing SHMstruct.
 *
 * @param shm Pointer to SHMstruct
 */
void clearSHM(SHMstruct * shm)
{
munmap(shm, sizeof(SHMstruct));
}

/* Close SHM file descriptor
 *
 * Function closes an existing SHM file descriptor.
 *
 * @param fd File descriptor of SHM
 */
void closeSHM(int fd)
{
close(fd);
}

/* Unlink SHM
 *
 * Function destroys an existing SHM assuming that its
 * allocated memory has been de-allocated.
 *
 * @param shname Name of SHM
 */
void destroySHM(char * shname)
{
  int fd = shm_unlink(shname);

  if(fd==-1)
    printf("faild to destroy the shared Memory segment");
}
