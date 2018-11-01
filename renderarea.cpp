#include "renderarea.hpp"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent),
    BackgroundColor(0, 0, 255),
    ShapeColor(255,255,255)
{
    this->setStyleSheet("background-color: blue;");
}

QSize RenderArea::minimumSizeHint() const{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const {
    return QSize(400, 200);
}
void RenderArea::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setBrush(BackgroundColor);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawRect(this->rect());

}
