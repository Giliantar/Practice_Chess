#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <arm_fix.h>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
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
    void onStartPositionClicked();
    void onLoadGameClicked();
    void onCellClicked();
    void playNextMove();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<QPushButton*>> cells;
    std::vector<std::vector<Figure*>> board;

    int selectedRow = -1;
    int selectedCol = -1;

    QTimer *animationTimer;
    QStringList moves;
    int currentMoveIndex;

    void initBoard();
    void updateBoard();
    void loadFromFile(const QString& filename);
    void loadStartPosition();
    void loadGameFromFile(const QString& filename);
    Figure* createFigure(char colorChar, char typeChar);
    void clearBoard();
    void highlightMoves(int row, int col);
    void makeMove(const QString& move);
};

#endif // MAINWINDOW_H