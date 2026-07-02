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
                                   "background-color: #F0D9B5;" :
                                   "background-color: #B58863;");

            btn->setFont(QFont("Arial", 22));
            layout->addWidget(btn, r, c);
            cells[r][c] = btn;
        }
    }

    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoadButtonClicked);

    loadFromFile(":/position.txt");
    updateBoard();
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


void MainWindow::loadFromFile(const QString& filename)
{
    clearBoard();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
}


void MainWindow::updateBoard()
{
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board[r][c] != nullptr) {
                cells[r][c]->setText(board[r][c]->getSymbol());
            } else {
                cells[r][c]->setText("");
            }
        }
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