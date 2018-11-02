#include "renderarea.hpp"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
}

QSize RenderArea::minimumSizeHint() const{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const {
    return QSize(400, 200);
}

void RenderArea::setReg(DiskAPI::Registro* R){
    this->Reg = R;
}

void RenderArea::paintEvent(QPaintEvent *event){
    QPainter p(this);
    p.setPen(Qt::green);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.scale(7, 7);
    if(Reg != nullptr){
        auto tipo = Reg->tipo;
        if(tipo == POLIGONO){
            pair<QPointF*, int> vertices = Reg->lista->toQPoint();
            p.drawPolygon(vertices.first, vertices.second);
            delete vertices.first;
        }
        else if(tipo == CIRCULO){
            QPointF centro(10, 10);
            qreal raio = this->Reg->lista->Horario()->GetX();
            p.drawEllipse(centro, raio, raio);
        }
        else if(tipo == LINHA){

        }
    }

}
