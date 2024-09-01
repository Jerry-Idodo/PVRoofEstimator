#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "pvroof.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void show_warning(const QString& text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

bool MainWindow::check_numeric_values(PVRoof& Array)
{
    bool ok = true;

    /* PV Module checking */
    Array.module.name = ui->ModuleLineName->text();
    Array.module.manufacturer = ui->ModuleLineMan->text();

    Array.module.length = ui->ModuleLineLength->text().toDouble(&ok) / 1000;
    if (!ok) {
        show_warning("Invalid module length");
        return ok;
    }

    Array.module.width = ui->ModuleLineWidth->text().toDouble(&ok) / 1000;
    if (!ok) {
        show_warning("Invalid module width");
        return ok;
    }

    Array.module.rating = ui->ModuleLineRating->text().toDouble(&ok) / 1000;
    if (!ok) {
        show_warning("Invalid module rating");
        return ok;
    }

    /* Roof checking */
    Array.roof.type = ui->RoofComboBox->currentIndex();

    Array.roof.length = ui->RoofLengthLine->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid roof length");
        return ok;
    }

    Array.roof.width = ui->RoofWidthLine->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid roof width");
        return ok;
    }

    if (ui->RoofLineEdit_11->text().isEmpty()) {
        if (Array.roof.type == Roof::RECTANGULAR) {
            ui->RoofLineEdit_11->setText(ui->RoofLengthLine->text());
            Array.roof.top = Array.roof.length;
        }
    } else {
        Array.roof.top = ui->RoofLineEdit_11->text().toDouble(&ok);
        if (!ok) {
            show_warning("Invalid top/ridge length");
            return ok;
        }
    }

    if (ui->RoofLineEdit_41->text().isEmpty()) {
        if (Array.roof.type == Roof::RECTANGULAR) {
            ui->RoofLineEdit_41->setText(ui->RoofLengthLine->text());
            Array.roof.bottom = Array.roof.length;
        }
    } else {
        Array.roof.bottom = ui->RoofLineEdit_41->text().toDouble(&ok);
        if (!ok) {
            show_warning("Invalid bottom/eave length");
            return ok;
        }
    }

    if (ui->RoofLineEdit_31->text().isEmpty()) {
        if (Array.roof.type == Roof::RECTANGULAR) {
            ui->RoofLineEdit_31->setText(ui->RoofWidthLine->text());
            Array.roof.left = Array.roof.width;
        }
    } else {
        Array.roof.left = ui->RoofLineEdit_31->text().toDouble(&ok);
        if (!ok) {
            show_warning("Invalid left side roof dimension");
            return ok;
        }
    }

    if (ui->RoofLineEdit_21->text().isEmpty()) {
        if (Array.roof.type == Roof::RECTANGULAR) {
            ui->RoofLineEdit_21->setText(ui->RoofWidthLine->text());
            Array.roof.right = Array.roof.width;
        }
    } else {
        Array.roof.right = ui->RoofLineEdit_21->text().toDouble(&ok);
        if (!ok) {
            show_warning("Invalid right side roof dimension");
            return ok;
        }
    }

    Array.roof.top_clearance = ui->RoofLineEdit_12->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid top clearance");
        return ok;
    }

    Array.roof.bottom_clearance = ui->RoofLineEdit_42->text().toDouble(&ok);
    if (!ok) {
        show_warning(("Invalid bottom clearance"));
        return ok;
    }

    Array.roof.left_clearance = ui->RoofLineEdit_32->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid left clearance");
        return ok;
    }

    Array.roof.right_clearance = ui->RoofLineEdit_22->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid right clearance");
        return ok;
    }

    Array.roof.module_spacing = ui->RoofModuleLineEdit->text().toDouble(&ok);
    if (!ok) {
        show_warning("Invalid module spacing");
        return ok;
    }

    return ok;
}

void MainWindow::fill_result(PVRoof& Array)
{
    ui->PortraitSizeLabel->setText(QString::number(Array.portrait.calculate_rating(Array.module.rating), 'f', 2) + " kWp");
    ui->PortraitModuleLabel->setText(QString::number(Array.portrait.rows * Array.portrait.columns) + " modules");
    ui->PortraitLayoutLabel->setText(QString::number(Array.portrait.rows)
                                     + " rows by "
                                     + QString::number(Array.portrait.columns)
                                     + " columns");
    ui->PortraitUtliizationLabel->setText(QString::number(100 * Array.portrait.calculate_area(Array.module, Array.roof.module_spacing) / Array.roof.area(), 'f', 2) + "%");

    ui->LandscapeSizeLabel->setText(QString::number(Array.landscape.calculate_rating(Array.module.rating), 'f', 2) + " kWp");
    ui->LandscapeModuleLabel->setText(QString::number(Array.landscape.rows * Array.landscape.columns) + " modules");
    ui->LandscapeLayoutLabel->setText(QString::number(Array.landscape.rows)
                                     + " rows by "
                                     + QString::number(Array.landscape.columns)
                                     + " columns");
    ui->LandscapeUtliizationLabel->setText(QString::number(100 * Array.landscape.calculate_area(Array.module, Array.roof.module_spacing) / Array.roof.area(), 'f', 2) + "%");
}

void MainWindow::on_CalculateButton_clicked()
{
    PVRoof PVArray;

    if (ui->RoofComboBox->currentIndex() != Roof::RECTANGULAR) {
        show_warning("Rectangular roofs only for now");
        return;
    }

    if (!check_numeric_values(PVArray))
        return;


    int max_array = PVArray.calculate_pv_layout();

    fill_result(PVArray);
}

void MainWindow::on_ResetButton_clicked()
{
    ui->RoofLengthLine->setText("");
    ui->RoofWidthLine->setText("");
    ui->RoofLineEdit_11->setText("");
    ui->RoofLineEdit_12->setText("1");
    ui->RoofLineEdit_21->setText("");
    ui->RoofLineEdit_22->setText("1");
    ui->RoofLineEdit_31->setText("");
    ui->RoofLineEdit_32->setText("1");
    ui->RoofLineEdit_41->setText("");
    ui->RoofLineEdit_42->setText("1");
    ui->RoofModuleLineEdit->setText("0.02");

    ui->PortraitSizeLabel->setText("0 kWp");
    ui->PortraitModuleLabel->setText("0");
    ui->PortraitLayoutLabel->setText("0 rows  by 0 columns");
    ui->PortraitUtliizationLabel->setText("0%");

    ui->LandscapeSizeLabel->setText("0 kWp");
    ui->LandscapeModuleLabel->setText("0");
    ui->LandscapeLayoutLabel->setText("0 rows by 0 columns");
    ui->LandscapeUtliizationLabel->setText("0%");
}

