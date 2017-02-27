#include "filegenerator.h"

FileGenerator::FileGenerator(MainModel* model, QObject *parent) : QObject(parent),
                                                                  modelHandler(model)
{
    connect(WindowsManager::getQmlRootObjects().first(), SIGNAL(onCreateButtonClicked()), this, SLOT(slot_create_file()));
    connect(WindowsManager::getQmlRootObjects().first(), SIGNAL(onAddSubsClicked()), this, SLOT(slot_on_add_subs_clicked()));
    connect(WindowsManager::getQmlRootObjects().first(), SIGNAL(onClearTableClicked()), this, SLOT(slot_on_clear_table_clicked()));
    connect(WindowsManager::getQmlRootObjects().first(), SIGNAL(onAddSubClicked()), this, SLOT(slot_on_add_sub_clicked()));
    connect(WindowsManager::getQmlRootObjects().first(), SIGNAL(onRemSubClicked()), this, SLOT(slot_on_rem_sub_clicked()));
}

void FileGenerator::busy(const QVariant& state)
{
    QObject* moduleRootObject = WindowsManager::getQmlRootObjects().first();
    QMetaObject::invokeMethod(moduleRootObject, "change_progressbar_state",
                              Q_ARG(QVariant, state));
}

void FileGenerator::slot_create_file()
{
    busy(true);
    QFile file("D:\\DESKTOPDEV\\workspace\\settings.txt");
       if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
           return;

    QTextStream out(&file);
    out << modelHandler->getSubscribersCount();
    out << "\n";
    Subscriber sub;
    for (int i = 0; i < modelHandler->getSubscribersCount(); i++)
    {
        QGuiApplication::processEvents();
        sub = modelHandler->getSubscriber(i);
        out << sub.flatNumber() / 100 % 10 << sub.flatNumber() / 10 % 10 << sub.flatNumber() % 10
            << sub.isEnabled();
        QString telNumber1 = "00000000000000000000";
        telNumber1.replace(0, sub.telNumber1().count(), sub.telNumber1());
        QString telNumber2 = "00000000000000000000";
        telNumber2.replace(0, sub.telNumber2().count(), sub.telNumber2());
        QString telNumber3 = "00000000000000000000";
        telNumber3.replace(0, sub.telNumber3().count(), sub.telNumber3());
        QString telNumber4 = "00000000000000000000";
        telNumber4.replace(0, sub.telNumber4().count(), sub.telNumber4());
        QString telNumber5 = "00000000000000000000";
        telNumber5.replace(0, sub.telNumber5().count(), sub.telNumber5());
        out << sub.telNumber1().count() /10 % 10 << sub.telNumber1().count() % 10 << telNumber1
            << sub.telNumber2().count() /10 % 10 << sub.telNumber2().count() % 10 << telNumber2
            << sub.telNumber3().count() /10 % 10 << sub.telNumber3().count() % 10 << telNumber3
            << sub.telNumber4().count() /10 % 10 << sub.telNumber4().count() % 10 << telNumber4
            << sub.telNumber5().count() /10 % 10 << sub.telNumber5().count() % 10 << telNumber5 << "\n";
    }
    file.close();
    busy(false);
}

void FileGenerator::slot_on_add_subs_clicked()
{
    WindowsManager::show_add_subs_window(this);
}

void FileGenerator::slot_add_subs(const QString& number)
{
    busy(true);
    int startNumOfSubs = modelHandler->getSubscribersCount();
    int currentNumOfSubs = modelHandler->getSubscribersCount();
    for (int i = 0; (i < number.toInt()) && (currentNumOfSubs < 300); i++)
    {
        QGuiApplication::processEvents();
        modelHandler->add_new_subscriber(Subscriber(startNumOfSubs + i + 1, false, "", "", "", "", ""));
        currentNumOfSubs = modelHandler->getSubscribersCount();
    }
    busy(false);
}

void FileGenerator::slot_on_clear_table_clicked()
{
    modelHandler->clear_subscribers_list();
}

void FileGenerator::slot_on_add_sub_clicked()
{
    int numOfSubs = modelHandler->getSubscribersCount();
    if (numOfSubs < 300)
        modelHandler->add_new_subscriber(Subscriber(numOfSubs + 1, false, "", "", "", "", ""));
}

void FileGenerator::slot_on_rem_sub_clicked()
{
    int numOfSubs = modelHandler->getSubscribersCount();
    if (numOfSubs > 0)
        modelHandler->remove_last_subscriber();
}