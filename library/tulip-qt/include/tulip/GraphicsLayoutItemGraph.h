#ifndef GRAPHICSLAYOUTITEMGRAPH_H
#define GRAPHICSLAYOUTITEMGRAPH_H

#include "tulip/GraphicsGraphMapper.h"
#include <QtGui/QGraphicsLayoutItem>

namespace tlp {
class GraphicsLayoutItemGraphMapper: public GraphicsGraphMapper<QGraphicsLayoutItem> {
public:
  GraphicsLayoutItemGraphMapper(const std::vector<QGraphicsLayoutItem *> &items);
  virtual ~GraphicsLayoutItemGraphMapper();

protected:
  virtual void setItemPosition(QGraphicsLayoutItem *item, const QPointF &pos);
  virtual QRectF itemGeometry(QGraphicsLayoutItem *item);
};
}

#endif // GRAPHICSLAYOUTITEMGRAPH_H
