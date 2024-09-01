#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pvroof.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_CalculateButton_clicked();
    bool check_numeric_values(PVRoof& PVArray);
    void fill_result(PVRoof& Array);

    void on_ResetButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
