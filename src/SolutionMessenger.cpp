/*
 * SolutionMessenger.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: goldman
 */

#include "SolutionMessenger.h"

SolutionMessenger::SolutionMessenger(size_t _length) : length(_length) {
  // always assume you need to round up
  buffer_length = length / PACK_SIZE + 1;
  buffer = new int[buffer_length];
  comm_rank = MPI::COMM_WORLD.Get_rank();
  comm_size = MPI::COMM_WORLD.Get_size();
}

void SolutionMessenger::bit_pack(const vector<bool>& bits) {
  int buffer_index = 0;
  buffer[buffer_index] = 0;
  int pack_index = 0;
  for (const bool & bit : bits) {
    buffer[buffer_index] |= (bit << pack_index);
    pack_index++;
    if (pack_index == PACK_SIZE) {
      pack_index = 0;
      buffer_index ++;
      buffer[buffer_index] = 0;
    }
  }
}

vector<bool> SolutionMessenger::bit_unpack() {
  vector<bool> result;
  int buffer_index = 0;
  int unpacked = 0;
  for (size_t i = 0; i < length; i++) {
    result.push_back(buffer[buffer_index] & 1);
    buffer[buffer_index] >>= 1;
    unpacked ++;
    if (unpacked == PACK_SIZE) {
      unpacked = 0;
      buffer_index ++;
    }
  }
  return result;
}

void SolutionMessenger::send(const vector<bool> & solution, int tag) {
  bit_pack(solution);
  for (int node=0; node < comm_size; node++) {
    MPI::COMM_WORLD.Isend(buffer, buffer_length, MPI_INT, node, tag);
  }
}

vector<vector<bool>> SolutionMessenger::recv(int tag) {
  vector<vector<bool>> result;
  for (int node=0; node < comm_size; node++) {
    while (MPI::COMM_WORLD.Iprobe(node, tag)) {
      MPI::COMM_WORLD.Irecv(buffer, buffer_length, MPI_INT, node, tag);
      result.push_back(bit_unpack());
    }
  }
  return result;
}
