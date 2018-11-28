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

void RenderArea::setRegistros(vector<Registro*>& v){
    this->regs = v;
}

void RenderArea::paintEvent(QPaintEvent *event){
    desenha();
}

QPainter* RenderArea::defaultPainter(){
    QPainter* p= new QPainter(this);
    p->setRenderHint(QPainter::Antialiasing, true);
    p->translate(this->rect().bottomLeft());
    p->scale(7, -7);
    return p;
}

void RenderArea::desenha(){
    QColor cores[QNT_CORES] = {
        Qt::red, Qt::green, Qt::blue, Qt::darkRed,
        Qt::darkGreen, Qt::darkBlue, Qt::cyan, Qt::magenta,
        Qt::yellow, Qt::gray, Qt::darkCyan, Qt::darkMagenta,
        Qt::darkYellow, Qt::darkGray, Qt::lightGray
    };
    QPainter *p = defaultPainter();
    unsigned i=0, tipo;
    pair<QPointF*, int> vertices;
    double gap = getGap();
    for(auto r: regs){
        if(r != nullptr and r->tam){
            vertices = r->lista->toQPoint(gap);
            p->setPen(QPen(cores[i++%QNT_CORES], 0.3, Qt::SolidLine));
            tipo = r->tipo;
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
            else{
                QPointF centro(10, 10);
                qreal raio = r->lista->Horario()->GetX();
                p->drawEllipse(centro, raio, raio);
            }
        }
    }
    delete p;
}

double RenderArea::getGap(){
    double gap = 0.0;
    if(regs.size() > 1){
        for(auto r: regs)
            gap = min(r->lista->GetMin(), gap);
    }
    return gap;
}

bool RenderArea::listaValida(){
    for(auto r: regs)
        if(r[0].tam == 0) return false;
    return true;
}
