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
#ifndef _GLQUAD_H
#define _GLQUAD_H

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include "tulip/GlPolygon.h"

namespace tlp {
/**
 * \addtogroup GlEntities
 */
/*@{*/
/**
 * \brief General class to render quads .
 *
 * This class is a generic class to render quadsl.
 */
class TLP_GL_SCOPE GlQuad : public GlPolygon
{
 
 public:

  /**
   * Constructor
   */
  GlQuad();

  /**
   * Constructor
   *
   * \param positions Array indicating the position of the points. The first point is the top-left one. The next points are winding in clockwise order.
   * \param color A single color indicating the color for every point of the Quad.
   */
  GlQuad(const Coord positions[4], const Color &color);

  /**
   * Constructor
   * 
   * \param positions Array indicating the position of the points. The first point is the top-left one. The next points are winding in clockwise order.
   * \param colors Array indicating the color of the points. The order is the same as for the positions.
   */
  GlQuad(const Coord positions[4], const Color colors[4]);

  /**
   * Destructor
   */
  virtual ~GlQuad();

  /**
   * Accessor in writing to the position.
   * \param idPosition Indicates which point we want to move.
   */
  void setPosition(const int idPosition, const Coord &position);

  /**
   * Accessor in writing to the color.
   * \param idColor Indicates which point we want to colorize.
   */
  void setColor(const int idColor, const Color &color);

  /**
   * Accessor in writing to the basic color of GlAugmentedDisplay
   */
  void setColor(const Color& color);

  /**
   * Accessor in reading to the position.
   * \param idPosition Indicates which point we want to get the position.
   */
  const Coord& getPosition(const int idPosition) const;

  /**
   * Accessor in reading to the color.
   * \param idColor Indicates which point we want to get the color.
   */
  const Color& getColor(const int idColor) const;

  /**
   * Function to export data in XML
   */
  virtual void getXML(xmlNodePtr rootNode);
  
  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);

  
};
/*@}*/
}
#endif
