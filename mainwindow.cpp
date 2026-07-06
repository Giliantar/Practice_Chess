#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->gridLayoutWidget->layout());
    if (!layout) {
        layout = new QGridLayout(ui->gridLayoutWidget);
        ui->gridLayoutWidget->setLayout(layout);
    }
    layout->setSpacing(0);

    cells.resize(8, std::vector<QPushButton*>(8, nullptr));
    board.resize(8, std::vector<Figure*>(8, nullptr));

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            QPushButton *btn = new QPushButton(this);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn->setMinimumSize(55, 55);
            btn->setMaximumSize(65, 65);

            bool isLight = (r + c) % 2 == 0;
            btn->setStyleSheet(isLight ?
                                   "background-color: #F0D9B5; border: none;" :
                                   "background-color: #B58863; border: none;");

            btn->setFont(QFont("Arial", 24));
            btn->setText("");
            connect(btn, &QPushButton::clicked, this, &MainWindow::onCellClicked);
            layout->addWidget(btn, r, c);
            cells[r][c] = btn;
        }
    }

    animationTimer = new QTimer(this);
    animationTimer->setInterval(1000);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::playNextMove);

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadButtonClicked);
    connect(ui->actionStartPosition, &QAction::triggered, this, &MainWindow::onStartPositionClicked);
    connect(ui->actionLoadGame, &QAction::triggered, this, &MainWindow::onLoadGameClicked);

    loadStartPosition();
}

MainWindow::~MainWindow()
{
    clearBoard();
    delete ui;
}

void MainWindow::clearBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            delete board[r][c];
            board[r][c] = nullptr;
        }
    }
}

Figure* MainWindow::createFigure(char colorChar, char typeChar)
{
    Figure::Color color = (colorChar == 'w' || colorChar == 'W') ?
                              Figure::Color::White : Figure::Color::Black;

    switch (typeChar) {
    case 'p': case 'P': return new Pawn(color);
    case 'r': case 'R': return new Rook(color);
    case 'n': case 'N': return new Knight(color);
    case 'b': case 'B': return new Bishop(color);
    case 'q': case 'Q': return new Queen(color);
    case 'k': case 'K': return new King(color);
    default: return nullptr;
    }
}

void MainWindow::loadStartPosition()
{
    QStringList lines = {
        "BRa8", "BNb8", "BBc8", "BQd8", "BKe8", "BBf8", "BNg8", "BRh8",
        "BPa7", "BPb7", "BPc7", "BPd7", "BPe7", "BPf7", "BPg7", "BPh7",
        "WPa2", "WPb2", "WPc2", "WPd2", "WPe2", "WPf2", "WPg2", "WPh2",
        "WRa1", "WNb1", "WBc1", "WQd1", "WKe1", "WBf1", "WNg1", "WRh1"
    };

    clearBoard();

    for (const QString& line : lines) {
        if (line.length() < 4) continue;

        char colorChar = line[0].toLatin1();
        char typeChar = line[1].toLatin1();
        char colChar = line[2].toLatin1();
        char rowChar = line[3].toLatin1();

        int col = colChar - 'a';
        int row = 8 - (rowChar - '0');

        if (row < 0 || row > 7 || col < 0 || col > 7) continue;

        Figure* fig = createFigure(colorChar, typeChar);
        if (fig) {
            board[row][col] = fig;
        }
    }

    updateBoard();
    ui->statusbar->showMessage("Начальная позиция загружена", 2000);
}

void MainWindow::loadFromFile(const QString& filename)
{
    clearBoard();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Не удалось открыть файл: " + filename, 3000);
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.length() < 4) continue;

        char colorChar = line[0].toLatin1();
        char typeChar = line[1].toLatin1();
        char colChar = line[2].toLatin1();
        char rowChar = line[3].toLatin1();

        int col = colChar - 'a';
        int row = 8 - (rowChar - '0');

        if (row < 0 || row > 7 || col < 0 || col > 7) continue;

        Figure* fig = createFigure(colorChar, typeChar);
        if (fig) {
            board[row][col] = fig;
        }
    }

    file.close();
    updateBoard();
    ui->statusbar->showMessage("Загружено: " + filename, 2000);
}

void MainWindow::loadGameFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->statusbar->showMessage("Не удалось открыть файл: " + filename, 3000);
        return;
    }

    animationTimer->stop();
    loadStartPosition();

    moves.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) continue;
        moves.append(line);
    }
    file.close();

    currentMoveIndex = 0;
    ui->statusbar->showMessage("Загружено " + QString::number(moves.size()) + " ходов", 2000);

    if (!moves.isEmpty()) {
        animationTimer->start();
    }
}

void MainWindow::makeMove(const QString& move)
{
    if (move.length() < 4) return;

    char fromColChar = move[0].toLatin1();
    char fromRowChar = move[1].toLatin1();
    char toColChar = move[2].toLatin1();
    char toRowChar = move[3].toLatin1();

    int fromCol = fromColChar - 'a';
    int fromRow = 8 - (fromRowChar - '0');
    int toCol = toColChar - 'a';
    int toRow = 8 - (toRowChar - '0');

    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7) return;
    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return;

    if (board[fromRow][fromCol] == nullptr) {
        ui->statusbar->showMessage("Ошибка: нет фигуры на " + move.left(2), 2000);
        animationTimer->stop();
        return;
    }

    if (!board[fromRow][fromCol]->canMove(fromRow, fromCol, toRow, toCol, board)) {
        ui->statusbar->showMessage("Невозможный ход: " + move, 2000);
        animationTimer->stop();
        return;
    }

    delete board[toRow][toCol];
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = nullptr;

    updateBoard();
    ui->statusbar->showMessage("Ход: " + move, 500);
}

void MainWindow::playNextMove()
{
    if (currentMoveIndex >= moves.size()) {
        animationTimer->stop();
        ui->statusbar->showMessage("Партия завершена!", 3000);
        return;
    }

    QString move = moves[currentMoveIndex];
    makeMove(move);
    currentMoveIndex++;

    ui->statusbar->showMessage("Ход " + QString::number(currentMoveIndex) + "/" +
                                   QString::number(moves.size()) + ": " + move, 500);
}

void MainWindow::updateBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c] != nullptr) {
                cells[r][c]->setText(board[r][c]->getSymbol());

                QString colorStyle = (board[r][c]->getColor() == Figure::Color::White) ?
                                         "color: white;" : "color: black;";

                bool isLight = (r + c) % 2 == 0;
                QString bgStyle = isLight ?
                                      "background-color: #F0D9B5;" :
                                      "background-color: #B58863;";

                cells[r][c]->setStyleSheet(bgStyle + colorStyle + " border: none;");
            } else {
                cells[r][c]->setText("");
                bool isLight = (r + c) % 2 == 0;
                cells[r][c]->setStyleSheet(isLight ?
                                               "background-color: #F0D9B5; border: none;" :
                                               "background-color: #B58863; border: none;");
            }
        }
    }
}

void MainWindow::highlightMoves(int row, int col)
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            bool isLight = (r + c) % 2 == 0;
            cells[r][c]->setStyleSheet(isLight ?
                                           "background-color: #F0D9B5; border: none;" :
                                           "background-color: #B58863; border: none;");
            cells[r][c]->setText("");
        }
    }

    if (row < 0 || row > 7 || col < 0 || col > 7) return;
    if (board[row][col] == nullptr) return;

    cells[row][col]->setText(board[row][col]->getSymbol());

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[row][col]->canMove(row, col, r, c, board)) {
                cells[r][c]->setStyleSheet("background-color: #7FC97F; border: none;");
                if (board[r][c] != nullptr) {
                    cells[r][c]->setText(board[r][c]->getSymbol());
                    if (board[r][c]->getColor() == Figure::Color::White) {
                        cells[r][c]->setStyleSheet(cells[r][c]->styleSheet() + " color: white;");
                    } else {
                        cells[r][c]->setStyleSheet(cells[r][c]->styleSheet() + " color: black;");
                    }
                }
            }
        }
    }

    if (board[row][col]->getColor() == Figure::Color::White) {
        cells[row][col]->setStyleSheet(cells[row][col]->styleSheet() + " color: white;");
    } else {
        cells[row][col]->setStyleSheet(cells[row][col]->styleSheet() + " color: black;");
    }
}

void MainWindow::onCellClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = -1, col = -1;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (cells[r][c] == btn) {
                row = r; col = c;
                break;
            }
        }
        if (row != -1) break;
    }

    if (selectedRow != -1 && selectedCol != -1) {
        if (board[selectedRow][selectedCol] != nullptr &&
            board[selectedRow][selectedCol]->canMove(selectedRow, selectedCol, row, col, board)) {

            delete board[row][col];
            board[row][col] = board[selectedRow][selectedCol];
            board[selectedRow][selectedCol] = nullptr;

            selectedRow = -1;
            selectedCol = -1;

            updateBoard();
            highlightMoves(-1, -1);
            ui->statusbar->showMessage("Ход сделан", 1000);
            return;
        }

        if (board[row][col] != nullptr && board[row][col]->getColor() == board[selectedRow][selectedCol]->getColor()) {
            selectedRow = row;
            selectedCol = col;
            highlightMoves(row, col);
            ui->statusbar->showMessage("Выбрана " + board[row][col]->getName(), 1000);
            return;
        }
    }

    if (board[row][col] != nullptr) {
        selectedRow = row;
        selectedCol = col;
        highlightMoves(row, col);
        ui->statusbar->showMessage("Выбрана " + board[row][col]->getName(), 1000);
    } else {
        selectedRow = -1;
        selectedCol = -1;
        highlightMoves(-1, -1);
        ui->statusbar->showMessage("Выделение сброшено", 500);
    }
}

void MainWindow::onLoadButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Выберите файл с позицией", "",
                                                    "Text files (*.txt);;All files (*.*)");

    if (!filename.isEmpty()) {
        loadFromFile(filename);
    }
}

void MainWindow::onStartPositionClicked()
{
    animationTimer->stop();
    loadStartPosition();
}

void MainWindow::onLoadGameClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Выберите файл с партией", "",
                                                    "Text files (*.txt);;All files (*.*)");

    if (!filename.isEmpty()) {
        loadGameFromFile(filename);
    }
}