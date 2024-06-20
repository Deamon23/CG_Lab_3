#include "PyramidWidget.h"
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

// Конструктор класса PyramidWidget
PyramidWidget::PyramidWidget(QWidget *parent)
    : QOpenGLWidget(parent), x0(50) // Инициализация начального значения x0
{
    // Подключение таймера к слоту обновления вида
    connect(&timer, &QTimer::timeout, this, &PyramidWidget::updateView);
    timer.start(1000); // Запуск таймера с интервалом 1 секунда
}

// Деструктор класса PyramidWidget
PyramidWidget::~PyramidWidget()
{
}

// Инициализация OpenGL
void PyramidWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Инициализация функций OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Установка черного фона
    glEnable(GL_DEPTH_TEST); // Включение теста глубины для правильного отображения 3D объектов
}

// Обработка изменения размера окна
void PyramidWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // Установка области просмотра OpenGL
}

// Рисование сцены
void PyramidWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов цвета и глубины

    // Настройка проекционной матрицы
    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f); // Установка перспективной проекции

    // Настройка вида (камера)
    // Камера находится в точке (x0, 10, 50), смотрит на точку (0, 0, 0), верхний вектор направлен вдоль оси z
    projection.lookAt(QVector3D(x0, 10, 50), QVector3D(0, 0, 0), QVector3D(0, 0, 1));

    // Настройка модели
    QMatrix4x4 modelView;
    modelView.translate(0, 0, -5); // Перемещение модели по оси z

    // Вычисление матрицы модели-проекции
    QMatrix4x4 mvp = projection * modelView;

    // Настройка шейдерной программы
    QOpenGLShaderProgram program;
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        attribute vec3 vertexPosition;
        attribute vec3 vertexColor;
        varying vec3 fragColor;
        uniform mat4 mvpMatrix;
        void main() {
            fragColor = vertexColor;
            gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
        }
    )");
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        varying vec3 fragColor;
        void main() {
            gl_FragColor = vec4(fragColor, 1.0);
        }
    )");
    program.link(); // Компиляция и линковка шейдеров
    program.bind(); // Активация шейдерной программы

    program.setUniformValue("mvpMatrix", mvp); // Передача матрицы модели-проекции в шейдер

    // Определение вершин пирамиды и их цветов
    GLfloat vertices[] = {
        // Координаты вершин и цвета
        -10.0f, -5.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Нижняя левая вершина основания (красный)
        10.0f, -5.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Нижняя правая вершина основания (зеленый)
        10.0f,  5.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Верхняя правая вершина основания (синий)
        -10.0f,  5.0f, 0.0f,  1.0f, 1.0f, 0.0f, // Верхняя левая вершина основания (желтый)
        0.0f,  0.0f, 20.0f,  1.0f, 0.0f, 1.0f  // Вершина пирамиды (фиолетовый)
    };

    // Определение индексов вершин для отрисовки граней пирамиды линиями
    GLuint indices[] = {
        0, 1, // Основание
        1, 2,
        2, 3,
        3, 0,
        0, 4, // Боковые грани
        1, 4,
        2, 4,
        3, 4
    };

    // Активация и настройка атрибутов вершин
    program.enableAttributeArray("vertexPosition");
    program.setAttributeArray("vertexPosition", GL_FLOAT, vertices, 3, 6 * sizeof(GLfloat));

    program.enableAttributeArray("vertexColor");
    program.setAttributeArray("vertexColor", GL_FLOAT, vertices + 3, 3, 6 * sizeof(GLfloat));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Установка режима wireframe (отрисовка только линий)
    glDrawElements(GL_LINES, 16, GL_UNSIGNED_INT, indices); // Отрисовка пирамиды линиями

    // Деактивация атрибутов вершин
    program.disableAttributeArray("vertexPosition");
    program.disableAttributeArray("vertexColor");
    program.release(); // Деактивация шейдерной программы
}

// Обновление точки зрения
void PyramidWidget::updateView()
{
    x0 -= 10; // Уменьшение координаты x0
    if (x0 < -50) x0 = 50; // Возврат к начальному значению при достижении предела
    update(); // Перерисовка сцены
}
