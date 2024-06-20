#ifndef PYRAMIDWIDGET_H
#define PYRAMIDWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class PyramidWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit PyramidWidget(QWidget *parent = nullptr); // Конструктор
    ~PyramidWidget(); // Деструктор

protected:
    void initializeGL() override; // Инициализация OpenGL
    void resizeGL(int w, int h) override; // Обработка изменения размера окна
    void paintGL() override; // Рисование сцены

private slots:
    void updateView(); // Слот для обновления точки зрения

private:
    QTimer timer; // Таймер для обновления вида
    float x0; // Координата x для точки зрения
};

#endif // PYRAMIDWIDGET_H
