#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QListWidget>

class Engine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    MainWindow(Engine *engine,QWidget *parent = nullptr);
    ~MainWindow() override;
    private:
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    Engine *m_engine;


    QGraphicsPathItem *m_neighborsPathItem = nullptr;
    QGraphicsPathItem *m_pointsPathItem = nullptr;
    QGraphicsPathItem *m_currentPathItem = nullptr;

    bool m_initialized{false};

    void drawResultsList(QListWidget* list, std::vector<std::pair<int, double>> path_vector, const int& start_vector, const int& end_vector);
    void drawPointsOnMap(const std::vector<int>& listID_vector, const std::vector<std::pair<int,double>>& actual_path);
    void drawNeighborsOnMap(const std::vector<int>& listID_vector);
    void drawPathOnMap(const std::vector<std::pair<int,double>>& actual_path);

};
#endif