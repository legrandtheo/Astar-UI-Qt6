#include "mainwindow.h"
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QListWidget>
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QOpenGLWidget>
#include <QDebug>

#include "MapView.h"
#include "../modules/engine/engine.h"


void MainWindow::drawResultsList(QListWidget *list, vector<pair<int, double>> path_vector, const int& start_vector, const int& end_vector)
{
    QElapsedTimer timer;
    timer.start();

    qint64 elapsedTime=timer.elapsed();

    list->clear();
    list->addItem(QString::fromStdString("Time elapsed in calculation : ") + QString::number(elapsedTime));
    for (const auto&[vectorID, total_distance]:path_vector)
    {
        QString vector_ID_string = QString::number(vectorID);
        list->addItem(vector_ID_string);
    }
}

void MainWindow::drawPointsOnMap(const vector<int>& listID_vector, const vector<pair<int,double>>& actual_path)
{
    const double SCALE = 10000.0;
    const double POINT_SIZE = 5.0;

    QPainterPath pathContainer;

    for (int currentID : listID_vector)
    {
        auto coords = m_engine->getPointCoordinates(currentID);
        double x = coords.first*SCALE - POINT_SIZE/2.0;
        double y = -1*coords.second*SCALE - POINT_SIZE/2.0;



        pathContainer.addEllipse(x,y,POINT_SIZE,POINT_SIZE);

    }

    m_pointsPathItem = m_scene->addPath(pathContainer, Qt::NoPen, QBrush(Qt::yellow));

    QRectF drawingZoneRect = m_pointsPathItem->boundingRect();

    m_scene->setSceneRect(drawingZoneRect);

    m_view->fitInView(drawingZoneRect, Qt::KeepAspectRatio);
    m_view->scale(2,2);

}

void MainWindow::drawNeighborsOnMap(const std::vector<int> &listID_vector)
{
    if (listID_vector.empty()) return;
    const double SCALE = 10000.0;
    QPainterPath roadpath;

    for (const int& vector_ID : listID_vector)
    {
        auto [vector_x,vector_y] = m_engine->getPointCoordinates(vector_ID);
        QPointF vectorPoint(vector_x*SCALE,-1*vector_y*SCALE);
        for (const int& neighbor_ID : m_engine->return_neighbors(vector_ID))
        {
            roadpath.moveTo(vectorPoint);
            auto [neighbor_x,neighbor_y] = m_engine->getPointCoordinates(neighbor_ID);
            QPointF neighborPoint(neighbor_x*SCALE,-1*neighbor_y*SCALE);
            roadpath.lineTo(neighborPoint);
        }
    }

    QPen roadPen(Qt::green);
    roadPen.setWidth(1);
    roadPen.setCosmetic(true);

    m_neighborsPathItem = m_scene->addPath(roadpath, roadPen, Qt::NoBrush);

}

void MainWindow::drawPathOnMap(const std::vector<std::pair<int, double> > &actual_path)
{
    const double SCALE = 10000.0;

    QPainterPath roadpath;
    int first_ID = actual_path.at(0).first;
    auto [first_x,first_y] = m_engine->getPointCoordinates(first_ID);
    QPointF first_point(first_x*SCALE, -1*first_y*SCALE);
    roadpath.moveTo(first_point);

    for (auto& [vector_ID, neighbor_distance] : actual_path)
    {
        auto [x,y] = m_engine->getPointCoordinates(vector_ID);
        QPointF current_point(x*SCALE, -1*y*SCALE);
        roadpath.lineTo(current_point);
    }

    QPen roadPen(Qt::red);
    roadPen.setWidth(5);
    roadPen.setCosmetic(true);

    m_currentPathItem = m_scene->addPath(roadpath,roadPen,Qt::NoBrush);
}

MainWindow::MainWindow(Engine *engine,QWidget *parent) : QMainWindow(parent), m_engine(engine)
{
    vector<int> reference_map_IDs = m_engine->reference_map_IDs_getter();

    resize(700,500);
    setWindowTitle("Algorithme A* en C++ ! Map de Washington");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0,0,400,300);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_view = new MapView();
    m_view->setScene(m_scene);
    m_view->setViewport(new QOpenGLWidget());

    QListWidget *m_listwidget = new QListWidget(this);

    QLineEdit *start_vector_text = new QLineEdit;
    start_vector_text->setPlaceholderText("Vecteur de départ");

    QLineEdit *end_vector_text = new QLineEdit;
    end_vector_text->setPlaceholderText("Vecteur d'arrivée'");

    QPushButton *button = new QPushButton("Valider");

    layout->addWidget(m_view, 7);
    layout->addWidget(m_listwidget, 3);
    layout->addWidget(start_vector_text,1);
    layout->addWidget(end_vector_text,1);
    layout->addWidget(button,1);



    connect(button,&QPushButton::clicked,this, [=]() {
        vector<pair<int, double>> path_vector = m_engine->full_on_pathfinder(start_vector_text->text().toInt(),end_vector_text->text().toInt());

        drawResultsList(m_listwidget,path_vector,start_vector_text->text().toInt(), end_vector_text->text().toInt());

        if (!m_initialized)
        {
            m_scene->clear();
            m_pointsPathItem = nullptr;
            m_neighborsPathItem = nullptr;
            m_currentPathItem = nullptr;

            drawNeighborsOnMap(reference_map_IDs);
            drawPointsOnMap(reference_map_IDs,path_vector);
            m_initialized = true;
        }

        else
        {
            if (m_currentPathItem)
            {
                m_scene->removeItem(m_currentPathItem);
                delete m_currentPathItem;
                m_currentPathItem = nullptr;
            }
        }
        drawPathOnMap(path_vector);
    });
}

MainWindow::~MainWindow() = default;