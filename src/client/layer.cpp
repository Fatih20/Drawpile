/*
   DrawPile - a collaborative drawing program.

   Copyright (C) 2006 Calle Laakkonen

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <cmath>

#include "layer.h"
#include "brush.h"

namespace drawingboard {

Layer::Layer(QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsItem(parent,scene)
{
}

Layer::Layer(const QImage& image, QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsItem(parent,scene), image_(image)
{
}

void Layer::setImage(const QImage& image)
{
	image_ = image;
}

QImage Layer::image() const
{
	return image_;
}

/**
 * The brush pixmap is drawn at each point between point1 and point2.
 * Pressure values are interpolated between the points.
 * First pixel is not drawn. This is done on purpose, as drawLine is usually
 * used to draw multiple joined lines.
 */
void Layer::drawLine(const QPoint& point1, qreal pressure1,
		const QPoint& point2, qreal pressure2, const Brush& brush)
{
	int rad = qMax(brush.radius(pressure1),brush.radius(pressure2));
	qreal pressure = pressure1;
#if 0 // TODO
	qreal deltapressure;
	if(qAbs(pressure2-pressure1) < 1.0/255.0)
		deltapressure = 0;
	else
		deltapressure = (pressure2-pressure1) / hypot(point1.x()-point2.x(), point1.y()-point2.y());
#endif

	// Based on interpolatePoints() in kolourpaint
	const int x1 = point1.x ()-rad,
		y1 = point1.y ()-rad,
		x2 = point2.x ()-rad,
		y2 = point2.y ()-rad;

	// Difference of x and y values
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	// Absolute values of differences
	const int ix = qAbs (dx);
	const int iy = qAbs (dy);

	// Larger of the x and y differences
	const int inc = ix > iy ? ix : iy;

	// Plot location
	int plotx = x1;
	int ploty = y1;

	int x = 0;
	int y = 0;

	for (int i = 0; i <= inc; i++) {
		int plot = 0;

		x += ix;
		y += iy;

		if (x > inc) {
			plot++;
			x -= inc;

			if (dx < 0)
				plotx--;
			else
				plotx++;
		}

		if (y > inc) {
			plot++;
			y -= inc;

			if (dy < 0)
				ploty--;
			else
				ploty++;
		}

		if (plot)
			brush.draw(image_, QPoint(plotx,ploty), pressure);
	}

	// Update screen
	const int left = qMin(point1.x(), point2.x());
	const int right = qMax(point1.x(), point2.x());
	const int top = qMin(point1.y(), point2.y());
	const int bottom = qMax(point1.y(), point2.y());
	if(rad==0) rad=1;
	update(left-rad,top-rad,right-left+rad*2,bottom-top+rad*2);
}

/**
 * Draw a single point
 * @param point coordinates
 * @param pressure pen pressure
 * @param brush brush to use
 */
void Layer::drawPoint(const QPoint& point, qreal pressure, const Brush& brush)
{
	const int r = brush.radius(pressure);
	const int x = point.x()-r;
	const int y = point.y()-r;
	brush.draw(image_, QPoint(x,y), pressure);
	update(point.x()-r,point.y()-r,point.x()+r,point.y()+r);
}

QRectF Layer::boundingRect() const
{
	return QRectF(0,0, image_.width(), image_.height());
}

void Layer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	 QWidget *)
{
	QRectF exposed = option->exposedRect.adjusted(-1, -1, 1, 1);
    painter->drawImage(exposed, image_, exposed);
}

}

