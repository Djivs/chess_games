#include "tournamentswidget.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>

TournamentsWidget::TournamentsWidget(FormWidget *parent):
    FormWidget{parent} {
    formHeader = new FormHeader;
    formHeader->setTitle("Tournaments");

    layout = new QGridLayout(this);
    id = new QSpinBox;
    name = new QLineEdit;
    ratingRestriction = new QSpinBox;
    winner = new QLineEdit;
    city = new QLineEdit;
    country = new QLineEdit;
    judge = new QLineEdit;
    playedGames = new QTableView;
    save = new QPushButton("Save");

    layout->addWidget(formHeader, 0, 0, 1, 1);

    layout->addWidget(new QLabel("ID"), 1, 0, 1, 1);
    layout->addWidget(id, 1, 1, 1, 1);
    layout->addWidget(new QLabel("Tournaments name"), 2, 0, 1, 1);
    layout->addWidget(name, 2, 1, 1, 1);
    layout->addWidget(new QLabel("Rating restriction"), 3, 0, 1, 1);
    layout->addWidget(ratingRestriction, 3, 1, 1, 1);
    layout->addWidget(new QLabel("Winner"), 4, 0, 1, 1);
    layout->addWidget(winner, 4, 1, 1, 1);
    layout->addWidget(new QLabel("City"), 5, 0, 1, 1);
    layout->addWidget(city, 5, 1, 1, 1);
    layout->addWidget(new QLabel("Country"), 5, 2, 1, 1);
    layout->addWidget(country, 5, 3, 1, 1);
    layout->addWidget(new QLabel("Judge"), 6, 0, 1, 1);
    layout->addWidget(judge, 6, 1, 1, 1);
    layout->addWidget(new QLabel("Games, played at a tournament:"), 7, 0, 5, 1);
    layout->addWidget(playedGames, 7, 1, 5, 3);
    layout->addWidget(save, 14, 0, 1, 1);

    loadPage();

    connectFormHeader();
    connect(save, &QPushButton::clicked, this, [this] {saveChanges();});
}

TournamentsWidget::~TournamentsWidget() {
    
}

void TournamentsWidget::loadPage() {
    loadBasics();
    loadTable();
}

void TournamentsWidget::loadBasics() {
    id->setValue(curInd);
    QSqlQuery query("SELECT tournaments.name, rating_restriction, winner.name, city, country, judges.name"
    " FROM tournaments"
    " INNER JOIN chessplayers AS winner ON winner_id = winner.chessplayer_id"
    " INNER JOIN places ON tournaments.place_id = places.place_id"
    " INNER JOIN judges ON judges.judge_id = tournaments.judge_id"
    " WHERE tournament_id = " + QString::number(curInd));

    while (query.next()) {
        name->setText(query.value(0).toString().simplified());
        ratingRestriction->setValue(query.value(1).toInt());
        winner->setText(query.value(2).toString().simplified());
        city->setText(query.value(3).toString().simplified());
        country->setText(query.value(4).toString().simplified());
        judge->setText(query.value(5).toString().simplified());
    }
}

void TournamentsWidget::loadTable() {
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT game_date, format, time_control, result, white.name, black.name, moves"
        " FROM chess_games"
        " INNER JOIN chessplayers AS white ON white.chessplayer_id = white_id"
        " INNER JOIN chessplayers AS black ON black.chessplayer_id = black_id"
        " WHERE tournament_id = " + QString::number(curInd));

    playedGames->setModel(model);
    playedGames->show();

}

quint32 TournamentsWidget::getWinnerID(){
    QSqlQuery query("SELECT chessplayer_id FROM chessplayers WHERE name = \'" + winner->text() + "\'");
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return -1;
    }
}
quint32 TournamentsWidget::getJudgeID() {
    QSqlQuery query("SELECT judge_id FROM judges WHERE name = \'" + judge->text() + "\'");
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return -1;
    }
}
quint32 TournamentsWidget::getPlaceID() {
    QSqlQuery query("SELECT place_id FROM places WHERE city = \'" + city->text() + "\'"
            "AND country = \'" + country->text() + "\'");
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return -1;
    }
}
void TournamentsWidget::saveChanges() {
    bool exists = QSqlQuery("SELECT * FROM tournaments WHERE tournament_id = " + QString::number(curInd)).next();
    if (exists) {
        QSqlQuery query;
        query.prepare("UPDATE tournaments SET"
                      " name = :name,"
                      " rating_restriction = :rating_restriction,"
                      " place_id = :place_id,"
                      " winner_id = :winner_id,"
                      " judge_id = :judge_id"
                      " WHERE tournament_id = :tournament_id");

        query.bindValue(":tournament_id", curInd);
        query.bindValue(":name", name->text());
        query.bindValue(":rating_restriction", ratingRestriction->value());
        query.bindValue(":place_id", getPlaceID());
        query.bindValue(":winner_id", getWinnerID());
        query.bindValue(":judge_id", getJudgeID());

        query.exec();
    } else {
        QSqlQuery query;

        query.prepare("INSERT INTO tournaments VALUES("
                      ":tournament_id,"
                      " :name,"
                      " :rating_restriction,"
                      " :winner_id,"
                      " :place_id,"
                      " :judge_id)");

        query.bindValue(":tournament_id", curInd);
        query.bindValue(":name", name->text());
        query.bindValue(":rating_restriction", ratingRestriction->value());
        query.bindValue(":place_id", getPlaceID());
        query.bindValue(":winner_id", getWinnerID());
        query.bindValue(":judge_id", getJudgeID());


        query.exec();
    }
}
