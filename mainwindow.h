#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <arm_fix.h>

#include <QMainWindow>
#include <QPushButton>
#include <vector>
#include "Figure.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadButtonClicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<QPushButton*>> cells;
    std::vector<std::vector<Figure*>> board;

    void initBoard();
    void updateBoard();
    void loadFromFile(const QString& filename);
    Figure* createFigure(char colorChar, char typeChar);
    void clearBoard();
};

#endif // MAINWINDOW_H