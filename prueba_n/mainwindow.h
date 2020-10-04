#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>

#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>

#include "point.h"
#include "octree.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
    float rotation;
public:
    MainWindow();
    ~MainWindow();

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    Octree<double> *qTree ;

    void printQT(Octree<double> **quad);

    vector<Point<double>> pointsW;
    vector<Point<double>*> pSelec;

    Point<double> cCircle;
    double angle;
    bool pressLB;
    Point<double> cSphere;
    double radio;

public slots:
    void UpdateAnimation();

private:

};
#endif // MAINWINDOW_H
