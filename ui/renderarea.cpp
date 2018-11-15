#include "renderarea.hpp"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
}

QSize RenderArea::minimumSizeHint() const{
    return QSize(400, 200);
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

void RenderArea::setRegistros(DiskAPI::Registro** R, size_t tam){
    vector<Registro*> v(R, R+tam);
    /*
    for(unsigned i=0; i<tam; i++)
        v.push_back(R[i]);
    */
    this->regs = v;
}

void RenderArea::paintEvent(QPaintEvent *event){
    if(!regs.size()) desenhaUm();
    else desenhaN();
}

QPainter* RenderArea::defaultPainter(){
    QPainter* p = new QPainter(this);
    p->setRenderHint(QPainter::Antialiasing, true);
    p->translate(this->rect().bottomLeft());
    p->scale(7, -7);
    return p;
}

void RenderArea::desenhaUm(){
    QPainter* p = defaultPainter();

    p->setPen(QPen(Qt::green, 0.3, Qt::SolidLine));
    p->setRenderHint(QPainter::Antialiasing, true);
    //p.translate(width()/2, height()/2);
    p->translate(this->rect().bottomLeft());
    p->scale(7, -7);
    //p.rotate(-45);
    //p.scale(7, 7);
    if(Reg != nullptr){
        auto tipo = Reg->tipo;
        if(tipo != CIRCULO){
            pair<QPointF*, int> vertices = Reg->lista->toQPoint();
            if(tipo == POLIGONO)
                p->drawConvexPolygon(vertices.first, vertices.second);
            else if(tipo == LINHA)
                p->drawLine(vertices.first[0], vertices.first[1]);
            else if(tipo == POLIGONAL)
                p->drawPolyline(vertices.first, vertices.second);
            else if(tipo == PONTO)
                p->drawPoint(vertices.first[0]);
            else{
                for(int i=0; i<vertices.second; i++)
                    p->drawPoint(vertices.first[i]);
            }
            delete[] vertices.first;
        }
        else{
            QPointF centro(10, 10);
            qreal raio = this->Reg->lista->Horario()->GetX();
            p->drawEllipse(centro, raio, raio);
        }
    }
    if(Reg2 != nullptr){
        p->setPen(QPen(Qt::blue, 0.3, Qt::SolidLine));
        auto tipo = Reg2->tipo;
        if(tipo != CIRCULO){
            pair<QPointF*, int> vertices = Reg2->lista->toQPoint();
            if(tipo == POLIGONO)
                p->drawConvexPolygon(vertices.first, vertices.second);
            else if(tipo == LINHA)
                p->drawLine(vertices.first[0], vertices.first[1]);
            else if(tipo == POLIGONAL)
                p->drawPolyline(vertices.first, vertices.second);
            else if(tipo == PONTO)
                p->drawPoint(vertices.first[0]);
            else{
                for(int i=0; i<vertices.second; i++)
                    p->drawPoint(vertices.first[i]);
            }
            delete[] vertices.first;
        }
        else{
            QPointF centro(10, 10);
            qreal raio = this->Reg2->lista->Horario()->GetX();
            p->drawEllipse(centro, raio, raio);
        }
    }
    delete p;
}

void RenderArea::desenhaN(){
    QColor cores[QNT_CORES] = {
        Qt::red, Qt::green, Qt::blue, Qt::darkRed,
        Qt::darkGreen, Qt::darkBlue, Qt::cyan, Qt::magenta,
        Qt::yellow, Qt::gray, Qt::darkCyan, Qt::darkMagenta,
        Qt::darkYellow, Qt::darkGray, Qt::lightGray
    };
    QPainter *p = defaultPainter();
    unsigned tipo = regs[0]->tipo, i=0;
    pair<QPointF*, int> vertices;
    for(auto r: regs){
        vertices = r->lista->toQPoint();
        p->setPen(QPen(cores[i++%QNT_CORES], 0.3, Qt::SolidLine));
        if(tipo != CIRCULO){
            if(tipo == POLIGONO)
                p->drawConvexPolygon(vertices.first, vertices.second);
            else if(tipo == LINHA)
                p->drawLine(vertices.first[0], vertices.first[1]);
            else if(tipo == POLIGONAL)
                p->drawPolyline(vertices.first, vertices.second);
            else if(tipo == PONTO)
                p->drawPoint(vertices.first[0]);
            else{
                for(int i=0; i<vertices.second; i++)
                    p->drawPoint(vertices.first[i]);
            }
            delete[] vertices.first;
        }
    }
    delete p;
}
