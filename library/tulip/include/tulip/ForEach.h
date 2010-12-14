/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <assert.h>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include "tulip/memorypool.h"

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {
  template<typename TYPE>
    struct _TLP_IT {
    _TLP_IT(Iterator<TYPE> *_it) : _it(_it) {
    }
    ~_TLP_IT() {
      delete _it;
    }
    Iterator<TYPE> *_it;
  };

  template<typename TYPE>
  inline bool _tlp_if_test(TYPE &n, _TLP_IT<TYPE> &_it) {
    assert(_it._it !=0);
    if(_it._it->hasNext()) {
      n = _it._it->next();
      return true;
    } 
    else {
      return false;
    }
  }
}
#endif //DOXYGEN_NOTFOR_DEVEL


/**
 * Warning, do not use break or return inside a for each block;
 * it causes a memory leak; use breakForEach pr returnForEachInstead
 */
#define forEach(A, B) \
for(_TLP_IT<typeof(A)> _it_foreach(B); tlp::_tlp_if_test(A, _it_foreach);)

#define stableForEach(A, B)  \
  for(_TLP_IT<typeof(A)> _it_foreach(new StableIterator<typeof(A)>(B));  tlp::_tlp_if_test(A, _it_foreach);)

#define breakForEach {break;}

#define returnForEach(VAL) {return VAL;}

#endif
