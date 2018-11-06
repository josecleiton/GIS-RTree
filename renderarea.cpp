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

void RenderArea::setReg2(DiskAPI::Registro* R){
    this->Reg2 = R;
}

void RenderArea::paintEvent(QPaintEvent *event){
    QPainter p(this);
    /*
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    p.drawLine(0 ,0, 400, 0);
    */
    p.setPen(QPen(Qt::green, 0.3, Qt::SolidLine));
    p.setRenderHint(QPainter::Antialiasing, true);
    p.translate(200, 30);
    p.rotate(90);
    p.scale(7, 7);
    if(Reg != nullptr){
        auto tipo = Reg->tipo;
        if(tipo != CIRCULO){
            pair<QPointF*, int> vertices = Reg->lista->toQPoint();
            if(tipo == POLIGONO)
                p.drawConvexPolygon(vertices.first, vertices.second);
            else if(tipo == LINHA)
                p.drawLine(vertices.first[0], vertices.first[1]);
            else if(tipo == POLIGONAL)
                p.drawPolyline(vertices.first, vertices.second);
            else if(tipo == PONTO)
                p.drawPoint(vertices.first[0]);
            delete[] vertices.first;
        }
        else if(tipo == CIRCULO){
            QPointF centro(10, 10);
            qreal raio = this->Reg->lista->Horario()->GetX();
            p.drawEllipse(centro, raio, raio);
        }
        else
            p.drawText(10, 10, "Forma indefinida, impossível mostrá-la.");
    }
    if(Reg2 != nullptr){
        p.setPen(QPen(Qt::blue, 0.3, Qt::SolidLine));
        auto tipo = Reg2->tipo;
        if(tipo != CIRCULO){
            pair<QPointF*, int> vertices = Reg2->lista->toQPoint();
            if(tipo == POLIGONO)
                p.drawConvexPolygon(vertices.first, vertices.second);
            else if(tipo == LINHA)
                p.drawLine(vertices.first[0], vertices.first[1]);
            else if(tipo == POLIGONAL)
                p.drawPolyline(vertices.first, vertices.second);
            else if(tipo == PONTO)
                p.drawPoint(vertices.first[0]);
            delete[] vertices.first;
        }
        else if(tipo == CIRCULO){
            QPointF centro(10, 10);
            qreal raio = this->Reg2->lista->Horario()->GetX();
            p.drawEllipse(centro, raio, raio);
        }
        else
            p.drawText(10, 10, "Forma indefinida, impossível mostrá-la.");
    }
}
