#ifndef SOLUTIONMESSENGER_H_
#define SOLUTIONMESSENGER_H_

#include <vector>
using std::vector;
#include <mpi.h>

const int PACK_SIZE = sizeof(int) * 8;

class SolutionMessenger {
 public:
  SolutionMessenger(size_t _length);
  ~SolutionMessenger() {
    delete [] buffer;
  }
  void send(const vector<bool> & message, int tag);
  vector<vector<bool>> recv(int tag);
 private:
  void bit_pack(const vector<bool>& bits);
  vector<bool> bit_unpack();
  size_t length;
  size_t buffer_length;
  int * buffer;
  int comm_size;
  int comm_rank;
};


#endif /* SOLUTIONMESSENGER_H_ */
