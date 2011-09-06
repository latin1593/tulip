#ifndef COLORPROPERTYANIMATION_H_
#define COLORPROPERTYANIMATION_H_

#include <tulip/ColorProperty.h>
#include "tulip/VectorPropertyAnimation.h"

namespace tlp {

typedef VectorPropertyAnimation<tlp::ColorProperty, tlp::Color, unsigned char, 4> ColorPropertyAnimation;

}

#endif /* COLORPROPERTYANIMATION_H_ */
