#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "pvroof.h"
#include "pvsqlite.h"
#include "pvdrawing.h"


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
    void load_modules_from_database(const PVSQLite& db);
    void populate_ModuleComboBox();

    void on_ResetButton_clicked();

    void on_ModuleSaveButton_clicked();

    void on_ModuleComboBox_currentIndexChanged(int index);

    void on_ModuleComboBox_textActivated(const QString &arg1);

    void on_ModuleLoadButton_clicked();

    void on_PortraitButton_clicked();

    void on_LandscapeButton_clicked();

private:
    Ui::MainWindow *ui;
    std::string db_name;
    std::map<int, std::vector<QString>> ModuleMap;
    PVDrawing Drawing;
        qreal scale;

};
#endif // MAINWINDOW_H
