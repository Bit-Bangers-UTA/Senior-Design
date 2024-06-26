#include "map.h"
#include "threads.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B115200

// create map and buffer
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER; // The map mutex is used to ensure that only one thread can access the map at a time.
int map[MAP_SIZE][MAP_SIZE] = {0}; // The map is used to store the points that are mapped.
struct Coordinate map_ring_buffer[BUFFER_SIZE]; // The ring buffer is used to store and manage points on the map. It enables the efficient
struct Coordinate local_maxes[4];
int local_max_index = 0;
int local_dir = 0;
struct Coordinate  current_location = {0, 0, 0, 0, 0};
struct Coordinate prev_coordinate = {0, 0, 0, 0, 0};;
int buffer_head = 0; // The head of the buffer is used to add new points to the buffer.
int buffer_tail = 0; // The tail of the buffer is used to delete old points from the buffer.

pthread_mutex_t daq_fd_mutex = PTHREAD_MUTEX_INITIALIZER; // The daq_fd_mutex is used to ensure that only one thread can access the com port at a time.
int daq_fd; // File Descriptor: used to read the com port. In unix, everything is a file.
char daq_buffer[15]; // Data Aquisition Buffer: used to store the info sent from arduino via com port.

int main(void) {
  // make station linked list circular
  H.next = &A;
  // config to read com port
  const char *serial_port_path = "/dev/cu.usbmodem14101";
  struct termios tio; // Terminal I/O

  // Open port
  daq_fd = open(serial_port_path, O_RDWR | O_NOCTTY);
  if (daq_fd < 0) {
    perror("Error opening serial port");
    return -1;
  }

  // Configure port
  memset(&tio, 0, sizeof(tio));
  cfsetospeed(&tio, BAUDRATE);
  cfsetispeed(&tio, BAUDRATE);
  tio.c_cflag &= ~PARENB;
  tio.c_cflag &= ~CSTOPB;
  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag |= CREAD | CLOCAL;
  tcsetattr(daq_fd, TCSANOW, &tio);
  tcflush(daq_fd, TCIFLUSH);


  // Set starting position, this should be set via bluetooth or something. Hard coded for now.
  current_location.x = 70;
  current_location.y = 70;
  printf("test\n");
// Main loop: print the map
  while(1){

    // update_thread_buffer();
    daq_thread_read();
    // system("clear");
    // print_map();
    // if(local_max_index == 4){
    //     for(int i = 0; i < local_max_index; i++) {
    //         printf("Local max %d: x: %d, y: %d, distance: %lf, angle: %lf)\n",
    //                i,
    //                local_maxes[i].x,
    //                local_maxes[i].y,
    //                local_maxes[i].h,
    //                local_maxes[i].angle);
    //     }
    //     local_max_index = 0;
    //     // sleep(10);
    // }
    // usleep(250000);
  }
 return 0;
}
