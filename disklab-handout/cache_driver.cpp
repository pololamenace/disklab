//------------------------------------------------------------------------------
/// @file
/// @brief test driver for the cache module
/// @author Bernhard Egger <bernhard@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2017/05/16 Bernhard Egger created
///
/// @section license_section License
/// Copyright (c) 2017, Computer Systems and Platforms Laboratory,
/// Seoul National University. All rights reserved.
///
/// Redistribution and use in source and binary forms,  with or without modifi-
/// cation, are permitted provided that the following conditions are met:
///
/// - Redistributions of source code must retain the above copyright notice,
///   this list of conditions and the following disclaimer.
/// - Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY  AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER  OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT,  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSE-
/// QUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE
/// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
/// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT
/// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY
/// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
/// DAMAGE.
//------------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string.h>

#include "cache.h"
using namespace std;

void test(int size, bool debug)
{
  cout << "-----------------------------------------------------------" << endl;

  int i, j;
  BlockCache bc(size, true);

  if (debug) bc.dump();

  //
  // access block 0
  //
  bc.get(0);
  if (debug) bc.dump();

  //
  // stride-1 0-9 twice
  //
  for (j=0; j<2; j++) {
    for (i=0; i<10; i++) {
      bc.get(i);
      if (debug) bc.dump();
    }
  }

  //
  // stride-4 0,4,...,16
  //
  for (i=0; i<20; i+=4) {
    bc.get(i);
    if (debug) bc.dump();
  }

  //
  // 3x stride-1 0-1
  //
  for (j=0; j<3; j++) {
    for (i=0; i<2; i++) {
      bc.get(0);
      if (debug) bc.dump();
    }
  }

  if (!debug) bc.dump();

  cout << endl << endl;
}

/// @brief program entry point
int main(int argc, char *argv[])
{
  //
  // set to true to see contents of cache after each
  // operation
  //
  bool debug = true;

  test(2, debug);
  test(7, debug);
  test(16, debug);

  return EXIT_SUCCESS;
}

