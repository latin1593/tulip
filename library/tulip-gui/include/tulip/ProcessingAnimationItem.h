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
#ifndef PROCESSINGANIMATIONITEM_H
#define PROCESSINGANIMATIONITEM_H

#include <QtGui/QGraphicsPixmapItem>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtGui/QBrush>

class ProcessingAnimationItem : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT

  QVector<QPixmap> _pixmaps;
  QTimer _animationTimer;
  unsigned int _currentFrame;
  QBrush _brush;

public:
  explicit ProcessingAnimationItem(const QPixmap& pixmap, const QSize& size, QGraphicsItem* parent=0, QGraphicsScene* scene=0);
  virtual ~ProcessingAnimationItem();

  QBrush brush() const { return _brush; }
  void setBrush(const QBrush& brush) { _brush = brush; }

protected slots:
  void animationTimeout();
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PROCESSINGANIMATIONITEM_H
