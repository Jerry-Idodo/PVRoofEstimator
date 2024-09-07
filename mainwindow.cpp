#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    db_name = "pvmodule.db";
    PVSQLite database(db_name);
    database.create_table();

    if (database.fetch_records() == SQLITE_OK) {
        qDebug() << "Database Fetched";

        for (const auto& moduleData : database.Modules) {
            int key = std::stoi(moduleData[0]);
            std::vector<QString> data;

            for (const auto& value : moduleData) {
                data.push_back(QString::fromStdString(value));
            }

            ModuleMap[key] = data;
        }
    }
    qDebug() << "Database Mapped";

    ui->setupUi(this);
    qDebug() << "UI setup";

    if (!ModuleMap.empty()) {
        int i = 1;
        ui->ModuleLineName->setText(ModuleMap.begin()->second[1]);
        ui->ModuleLineMan->setText(ModuleMap.begin()->second[2]);
        ui->ModuleLineLength->setText(ModuleMap.begin()->second[3]);
        ui->ModuleLineWidth->setText(ModuleMap.begin()->second[4]);
        ui->ModuleLineRating->setText(ModuleMap.begin()->second[5]);
        qDebug() << "Module Mapped \n" << ModuleMap;

        for (const auto& [key, value] : ModuleMap) {
            qDebug() << value[1];
            ui->ModuleComboBox->insertItem(i, value[1]);
            qDebug() << key << ": " << value;
            i++;
        }
    }
    qDebug() << "ui constructor finished";

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

QString pick_file()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filename = QFileDialog::getOpenFileName(nullptr, "Open File",
                                                    documentsPath,
                                                    "db (*.db)");
    return filename;
}

void MainWindow::load_modules_from_database(const PVSQLite& db)
{
    for (const auto& moduleData : db.Modules) {
        int key = std::stoi(moduleData[0]);
        std::vector<QString> data;
        for (const auto& value : moduleData) {
            data.push_back(QString::fromStdString(value));
        }
        ModuleMap[key] = data;
    }
}

void MainWindow::populate_ModuleComboBox()
{
    int i = 1;
    for (const auto& [key, value] : ModuleMap) {
        ui->ModuleComboBox->insertItem(i, value[1]);
        i++;
    }
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


void MainWindow::on_ModuleSaveButton_clicked()
{
    PVSQLite db(db_name);

    db.ModuleData.push_back(ui->ModuleLineName->text().toStdString());
    db.ModuleData.push_back(ui->ModuleLineMan->text().toStdString());
    db.ModuleData.push_back(ui->ModuleLineLength->text().toStdString());
    db.ModuleData.push_back(ui->ModuleLineWidth->text().toStdString());
    db.ModuleData.push_back(ui->ModuleLineRating->text().toStdString());

    db.insert_record();
    if (db.rc != SQLITE_OK)
        show_warning(QString::fromStdString(db.Msg));

    db.ModuleData.clear();
}


void MainWindow::on_ModuleComboBox_currentIndexChanged(int index)
{
    return;
}


void MainWindow::on_ModuleComboBox_textActivated(const QString &arg1)
{
    int i = ui->ModuleComboBox->currentIndex() + 1;

    ui->ModuleLineName->setText(ModuleMap[i][1]);
    ui->ModuleLineMan->setText(ModuleMap[i][2]);
    ui->ModuleLineLength->setText(ModuleMap[i][3]);
    ui->ModuleLineWidth->setText(ModuleMap[i][4]);
    ui->ModuleLineRating->setText(ModuleMap[i][5]);
}


void MainWindow::on_ModuleLoadButton_clicked()
{
    // Get database filename
    std::string temp = pick_file().toStdString();
    if (temp.empty())
        return;

    // Clear existing data
    ui->ModuleComboBox->clear();
    ModuleMap.clear();
    db_name = temp;

    // Create or open database
    PVSQLite db(db_name);
    db.create_table();

    // Load modules from database
    if (db.fetch_records() == SQLITE_OK) {
        qDebug() << "Database Fetched";
        load_modules_from_database(db);
    } else {
        show_warning(QString::fromStdString(db.Msg));
    }

    // Update UI with loaded modules
    if (!ModuleMap.empty()) {
        int i = 1;

        ui->ModuleLineName->setText(ModuleMap.begin()->second[1]);
        ui->ModuleLineMan->setText(ModuleMap.begin()->second[2]);
        ui->ModuleLineLength->setText(ModuleMap.begin()->second[3]);
        ui->ModuleLineWidth->setText(ModuleMap.begin()->second[4]);
        ui->ModuleLineRating->setText(ModuleMap.begin()->second[5]);
        qDebug() << "Module Mapped \n" << ModuleMap;

        populate_ModuleComboBox();
    }
}

