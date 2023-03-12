#include "chessgameslistwidget.h"

#include <QDebug>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QHeaderView>
#include <QDate>

ChessGamesListWidget::ChessGamesListWidget(SQLWorker *w, FormWidget *parent):
    FormWidget{parent} {
    worker = w;

    formHeader = new FormHeader();
    formHeader->setTitle("Chess games list");

    date = new QLineEdit();
    whiteName = new QLineEdit();
    blackName = new QLineEdit();
    format = new QLineEdit();
    timeControl = new QLineEdit();
    opening = new QLineEdit();
    result = new QLineEdit;

    date->setReadOnly(true);
    whiteName->setReadOnly(true);
    blackName->setReadOnly(true);
    format->setReadOnly(true);
    timeControl->setReadOnly(true);
    opening->setReadOnly(true);
    result->setReadOnly(true);

    whiteRating = new QLineEdit;
    blackRating = new QLineEdit;

    whiteRating->setReadOnly(true);
    blackRating->setReadOnly(true);

    moves = new QTextBrowser();

    ratingDifs = new QTableView;
    ratingDifs->setMinimumHeight(300);

    layout = new QVBoxLayout(this);

    layout->addWidget(formHeader);
    layout->addWidget(new QLabel("Date:"));
    layout->addWidget(date);
    layout->addWidget(new QLabel("White:"));
    layout->addWidget(new QLabel("Name:"));
    layout->addWidget(whiteName);
    layout->addWidget(new QLabel("Rating:"));
    layout->addWidget(whiteRating);
    layout->addWidget(new QLabel("Black:"));
    layout->addWidget(new QLabel("Name:"));
    layout->addWidget(blackName);
    layout->addWidget(new QLabel("Rating:"));
    layout->addWidget(blackRating);
    layout->addWidget(new QLabel("Result:"));
    layout->addWidget(result);
    layout->addWidget(new QLabel("Format:"));
    layout->addWidget(format);
    layout->addWidget(new QLabel("Time control:"));
    layout->addWidget(timeControl);
    layout->addWidget(new QLabel("Played opening:"));
    layout->addWidget(opening);
    layout->addWidget(new QLabel("Moves:"));
    layout->addWidget(moves);
    layout->addWidget(new QLabel("Rating differences in games:"));
    layout->addWidget(ratingDifs);

    connectFormHeader();
    connectWorker();

    loadPage();
    
}

ChessGamesListWidget::~ChessGamesListWidget() {
    
}

void ChessGamesListWidget::loadAllChessplayersNames(QStringList names) {
    chessplayersNames = names;
}

void ChessGamesListWidget::loadGame(DMap map) {
    date->setText(map["date"].toString());
    whiteName->setText(map["white_name"].toString());
    whiteRating->setText(map["white_rating"].toString());
    blackName->setText(map["black_name"].toString());
    blackRating->setText(map["black_rating"].toString());
    format->setText(map["format"].toString());
    timeControl->setText(map["time_control"].toString());
    opening->setText(map["opening"].toString());
    moves->setText(map["moves"].toString());
    result->setText(map["result"].toString());
}

void ChessGamesListWidget::loadGamesCrossRequest(DTable table) {

    QStringList header = {"", "Итог"};
    for (auto &i : chessplayersNames) {
        header.push_back(i);
    }

    ratingDifs->setModel(DTableToModel(table, header));
    resizeTableView(ratingDifs);
    ratingDifs->show();
}

void ChessGamesListWidget::connectWorker() {
    connect(this, &ChessGamesListWidget::getGame, worker, &SQLWorker::getGame);
    connect(worker, &SQLWorker::gameReady, this, &ChessGamesListWidget::loadGame);
    connect(this, &ChessGamesListWidget::getGamesCrossRequest, worker, &SQLWorker::getGamesCrossRequest);
    connect(worker, &SQLWorker::gamesCrossRequestReady, this, &ChessGamesListWidget::loadGamesCrossRequest);
    connect(this, &ChessGamesListWidget::getAllChessplayersNames, worker, &SQLWorker::getAllChessplayersNames);
    connect(worker, &SQLWorker::allChessplayersNamesReady, this, &ChessGamesListWidget::loadAllChessplayersNames);

    connect(this, &ChessGamesListWidget::setMaxInd, worker, &SQLWorker::getMaxGameID);
    connect(worker, &SQLWorker::maxGameIDReady, this, &ChessGamesListWidget::loadMaxInd);
}
void ChessGamesListWidget::loadPage() {
    emit getGame(curInd);
    emit getAllChessplayersNames();
    emit getGamesCrossRequest();
}
