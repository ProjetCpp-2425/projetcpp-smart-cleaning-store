#include "mainwindow.h"
        #include <QApplication>
        #include <QMessageBox>
        #include "connection.h"
        #include <QApplication>
        #include <QLocale>
        #include <QTranslator>

        int main(int argc, char *argv[])
        {
            QApplication a(argc, argv);
            Connection c;
            bool test=c.createconnect();
            QTranslator translator;
            const QStringList uiLanguages = QLocale::system().uiLanguages();
            for (const QString &locale : uiLanguages) {
                const QString baseName = "untitled1_" + QLocale(locale).name();
                if (translator.load(":/i18n/" + baseName)) {
                    a.installTranslator(&translator);
                    break;
                }
            }
            MainWindow w;
            if(test)
            {w.show();
                QMessageBox::information(nullptr, QObject::tr("database is open"),
                            QObject::tr("connection successful.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

        }
            else
                QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                            QObject::tr("connection failed.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);



            return a.exec();
        }
