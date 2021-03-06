#include "mainwindow.h"
#include "ui_mainwindow.h"

//QFileDialog
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <QCoreApplication>
#include <QApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

bool createConnection();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitButton_clicked()
{
    QCoreApplication::quit();

}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
               tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    QString name;

    QTextStream in(stdin);

       if (!fileName.isEmpty()) {
           QFile file(fileName);

           if (!file.open(QIODevice::ReadOnly)) {
               QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
               return;
           }


           QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
           db.setHostName("localhost");
           db.setDatabaseName("test");
           db.setUserName("root");
           db.setPassword("Raistlin");
           //db.exec("drop database test;");
           if (!db.open()) {
               qDebug() << "Database error occurred";
               //return false;
           }
           else{

               QSqlQuery query;
               qDebug() << fileName << "Connected!";
               //query.exec("drop database test;");
                //query.exec("create database test;");
                query.exec("use test;");
                QString temp = "source C:/Users/user/Documents/School/NorthWind/Sample-SQL-File-100-Rows.sql;";
                qDebug() << temp;
                query.exec(temp);
                query.next();

                //return #
                //https://stackoverflow.com/questions/10492164/how-do-i-count-columns-of-a-table

               //correct query to return column #'s, unsure how to extract value

               query.exec("SELECT count(*) FROM information_schema.columns WHERE table_name = 'user_details';");

               //int count = query.value(0).toInt();

               //http://doc.qt.io/qt-5/sql-sqlstatements.html
               //#After the call to exec(), QSqlQuery's internal pointer is located one position before the first record. We must call QSqlQuery::next() once to advance to the first record,
               query.next();
               int count = query.value(0).toInt();
               qDebug() << "count: " << count;

               query.exec(ui->textEditQuery->toPlainText());
                //qDebug() << query.exec("select * from user_details;");

               //dumps text
               while (query.next()) {

                   int x=0;
                    while (x<count)
                    {
                   name = name + query.value(x).toString() + "\t";
                   x++;
                    }
                    name = name + "\n";
                   ui->textEdit->setText(name);

                    //qDebug() << "name:" << query.value(0).toString() <<"\n";
               }
           }

           file.close();
       }

}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
              tr("SQL Files (*.sql)"));

      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              // error message
          } else {
              QTextStream stream(&file);
              stream << ui->textEdit->toPlainText();
              stream.flush();
              file.close();
          }
      }

}
