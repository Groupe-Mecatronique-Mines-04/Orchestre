#include "melody.h"
#include <iostream>
#include <stdlib.h>
using namespace std;


Melody::Melody()
{

}

Melody::~Melody()
{

}

int Melody::notecompare(const void* a, const void* b) {
   Melody& aa = *((Melody*)a);
   Melody& bb = *((Melody*)b);

   if (aa.tick < bb.tick) {
      return -1;
   } else if (aa.tick > bb.tick) {
      return 1;
   } else {
      // highest note comes first
      if (aa.pitch > bb.pitch) {
         return 1;
      } else if (aa.pitch < bb.pitch) {
         return -1;
      } else {
         return 0;
      }
   }
}



