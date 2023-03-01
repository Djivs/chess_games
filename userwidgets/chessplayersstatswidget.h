#ifndef CHESSPLAYERSSTATSWIDGET_H
#define CHESSPLAYERSSTATSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QSpinBox>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>

#include <QVBoxLayout>
#include <QFormLayout>


class ChessplayersStatsWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChessplayersStatsWidget(QWidget *parent = nullptr);
    ~ChessplayersStatsWidget();
private:
    QLineEdit *search;
    QLineEdit *name;
    QSpinBox *rating;
    QSpinBox *birthYear;
    QSpinBox *amount;
    QSpinBox *wins;
    QSpinBox *loses;
    QSpinBox *draws;
    QTableView *gamesWhite;
    QTableView *gamesBlack;
    QSpinBox *amountWhite;
    QSpinBox *winsWhite;
    QSpinBox *losesWhite;
    QSpinBox *drawsWhite;
    QSpinBox *amountBlack;
    QSpinBox *winsBlack;
    QSpinBox *losesBlack;
    QSpinBox *drawsBlack;
    QTableView *openingsWhite;
    QTableView *openingsBlack;
    QWidget *whiteOpeningsGraph;
    QWidget *blackOpeningsGraph;
    QTableView *strongestOponents;


    QFormLayout *layout;

};
#endif //CHESSPLAYERSSTATSWIDGET_H
