#include "map.h"
#include "read_com.h"
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

// create map and buffer
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;
int map[MAP_SIZE][MAP_SIZE] = {0};
struct Coordinate map_ring_buffer[180];
int buffer_head;
int buffer_tail;

pthread_mutex_t daq_fd_mutex = PTHREAD_MUTEX_INITIALIZER;
int daq_fd; // File Descriptor: used to read the com port. In unix, everything
            // is a file.
char daq_buffer[15]; // Data Aquisition Buffer: used to store the info sent
                     // from arduino via com port.

int main(void) {
  // make station linked list circular
  H.next = &A;
  // init threads;
  pthread_t
      daq_thread_id; // Thread responsible for Data Aquisition from sensors
  pthread_t del_thread_id; // Thread responsible for deleting old points in map
  // config to read com port
  const char *serial_port_path = "/dev/cu.usbmodem14101";
  struct termios tio;

  // Open port
  daq_fd = open(serial_port_path, O_RDWR | O_NOCTTY);
  if (daq_fd < 0) {
    perror("Error opening serial port");
    return -1;
  }

  // Configure port
  memset(&tio, 0, sizeof(tio));
  cfsetospeed(&tio, B115200);
  cfsetispeed(&tio, B115200);
  tio.c_cflag &= ~PARENB;
  tio.c_cflag &= ~CSTOPB;
  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag |= CREAD | CLOCAL;
  tcsetattr(daq_fd, TCSANOW, &tio);
  tcflush(daq_fd, TCIFLUSH);

  // init reading data from com port
  if (pthread_create(&daq_thread_id, NULL, daq_thread_read_com, NULL) != 0) {
    perror("Failed to create DAQ thread");
    close(daq_fd);
    return -1;
  } else {
    printf("daq thread created \n");
  }

  // init deleting old points in map
  if (pthread_create(&del_thread_id, NULL, del_thread_buffer_del, NULL) != 0) {
    perror("Failed to create delete thread \n");
    close(daq_fd);
    return -1;
  } else {
    printf("delete thread created");
  }

  // Set starting position, this should be set via bluetooth or something. Hard
  // coded for now.
  current_location.x = 45;
  current_location.y = 45;
  int dir = 0;

  // Main loop: print the map
  while (1) {
    pthread_mutex_lock(&map_mutex);
    system("clear");
    print_map();
    pthread_mutex_unlock(&map_mutex);
    sleep(1);
  }

  // if thread ends early close the fd
  pthread_mutex_lock(&daq_fd_mutex);
  close(daq_fd);
  pthread_mutex_unlock(&daq_fd_mutex);
  return 0;
}
