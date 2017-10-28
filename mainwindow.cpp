#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Настройка списка воспроизведения
    playList = new QMediaPlaylist(this);
    playList->setPlaybackMode(QMediaPlaylist::Loop);

    //Подготовка модели с данными
    itemModel = new QStandardItemModel(this);
    itemModel->setHorizontalHeaderLabels(QStringList() << tr("Имя Файла") << tr("Путь к файлу"));

    //Настройка вывода
    ui->play_list_tableView->setModel(itemModel);
    ui->play_list_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->play_list_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->play_list_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->play_list_tableView->hideColumn(1);

    //Настройка плеера
    player = new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->setVolume(ui->volume_horizontalSlider->value());
    prevVolume = ui->volume_horizontalSlider->value();

    // при изменении индекса текущего трека в плейлисте, устанавливаем название файла в специальном лейбле
    connect(playList, SIGNAL(currentIndexChanged(int)), this, SLOT(onPlaylistIndexChanged(int)));

    //Изменять индикатор текущего времени воспроизведения
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(playTimeDisplay(qint64)));

    if (QCoreApplication::arguments().size() > 1)
    {
        QString file = QCoreApplication::arguments()[1];
        file.replace(QString("\\"), QString("/"));

        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(file).dirName()));
        items.append(new QStandardItem(file));
        itemModel->appendRow(items);
        playList->addMedia(QUrl(file));
        player->play();
    }

    stylize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

///Отображение времени воспроизведения
void MainWindow::playTimeDisplay(qint64 val)
{
    //Отобразить на метке времени
    QTime allTime = QTime(0, 0, 0).addMSecs(player->duration());
    QTime currentTime = QTime(0 , 0, 0).addMSecs(val);
    ui->current_track_timer->setText(currentTime.toString() + "/" + allTime.toString());

    //Отобразить на ползенке времени
    ui->play_time_horizontalSlider->setMaximum(player->duration());
    ui->play_time_horizontalSlider->setValue(val);

    //Отобразить название текущего трека
    ui->currentTrack->setText(itemModel->item(playList->currentIndex(), 0)->text());
}

///Нажатие на пункт меню "Добавить файл"
void MainWindow::on_add_file_action_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3)"));

    if (file.isEmpty())
        return;

    QList<QStandardItem *> items;
    items.append(new QStandardItem(QDir(file).dirName()));
    items.append(new QStandardItem(file));
    itemModel->appendRow(items);
    playList->addMedia(QUrl(file));
}

///Нажатие на пункт меню "Добавить файлы"
void MainWindow::on_add_files_action_triggered()
{
    // С помощью диалога выбора файлов делаем множественный выбор mp3 файлов
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3)"));

    if (files.isEmpty())
        return;

    // Далее устанавливаем данные по именам и пути к файлам
    // в плейлист и таблицу отображающую плейлист
    foreach (QString filePath, files) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        itemModel->appendRow(items);
        playList->addMedia(QUrl(filePath));
    }

}

///Нажатие на клавишу выбора следующего трека
void MainWindow::on_next_pushButton_clicked()
{
    playList->next();
}

///Нажатие на клавишу предидущего трека
void MainWindow::on_prev_pushButton_clicked()
{
    playList->previous();
}

///Нажатие на клавишу начала воспроизведения
void MainWindow::on_play_pushButton_clicked()
{
    player->play();
}

///Наэатие на клавишу "Пауза"
void MainWindow::on_pause_pushButton_clicked()
{
    player->pause();
}

///Двойной щелчок по списку воспроизведения
void MainWindow::on_play_list_tableView_doubleClicked(const QModelIndex &index)
{
    playList->setCurrentIndex(index.row());
    player->play();
}

///Действия при переходе к другому треку
void MainWindow::onPlaylistIndexChanged(int index)
{
    ui->play_list_tableView->selectRow(index);
    player->play();
}

///Нажатие на пункт меню "Выход"
void MainWindow::on_exit_action_triggered()
{
    qApp->exit();
}

///Регулировка звука
void MainWindow::on_volume_horizontalSlider_valueChanged(int value)
{
    player->setVolume(value);

    if (value == 0)
    {
        ui->volume_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }
    else
    {
        ui->volume_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
}

///Перемотка с помощью ползунка
void MainWindow::on_play_time_horizontalSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

///Отключение звука при нажатии на ползунок перемотки
void MainWindow::on_play_time_horizontalSlider_sliderPressed()
{
    player->setMuted(true);
}

///Включение звука при нажатии на ползунок перемотки
void MainWindow::on_play_time_horizontalSlider_sliderReleased()
{
    player->setMuted(false);
}

///Нажатие на кнопку стоп
void MainWindow::on_stop_pushButton_clicked()
{
    player->stop();
}

///Обработка нажатия клавиш
void MainWindow::keyPressEvent(QKeyEvent *event)
{

 int key=event->key();//event->key() - целочисленный код клавиши

 if (key==Qt::Key_Enter || key==Qt::Key_Return) {
  playList->setCurrentIndex(ui->play_list_tableView->currentIndex().row());
 }
 else if (key == Qt::Key_Delete)
 {
     int row = ui->play_list_tableView->currentIndex().row();
     if (row >= 0)
     {
         itemModel->removeRow(row);
         playList->removeMedia(row);
     }
 }
 else if (key == Qt::Key_Left)
 {
     qDebug() << "Left";
     player->setPosition(player->position() - 5000);
 }
 else if (key == Qt::Key_Right)
 {
     qDebug() << "Right";
     player->setPosition(player->position() + 5000);
 }
}

//! [1]
void MainWindow::stylize()
{
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_NoSystemBackground, false);
        setStyleSheet("MainWindow { background: transparent; }");
    } else {
        QtWin::resetExtendedFrame(this);
        setAttribute(Qt::WA_TranslucentBackground, false);
        setStyleSheet(QString("MainWindow { background: %1; }").arg(QtWin::realColorizationColor().name()));
    }

    //Задать иконки для кнопок
    ui->loop_mode_pushButton->setIcon(QIcon(":/res/all.jpg"));
    ui->play_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stop_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pause_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->next_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->prev_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->volume_pushButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}
//! [1]

void MainWindow::on_volume_pushButton_clicked()
{
    if (player->volume() != 0)
    {
        prevVolume = player->volume();
        ui->volume_horizontalSlider->setValue(0);
    }
    else
    {
        ui->volume_horizontalSlider->setValue(prevVolume);
    }
}

void MainWindow::on_loop_mode_pushButton_clicked()
{
    switch (playList->playbackMode())
    {
    case QMediaPlaylist::Loop:
        ui->loop_mode_pushButton->setIcon(QIcon(":/res/rand.jpg"));
        playList->setPlaybackMode(QMediaPlaylist::Random);
        break;

    case QMediaPlaylist::Random:
        ui->loop_mode_pushButton->setIcon(QIcon(":/res/one.jpg"));
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;

    case QMediaPlaylist::CurrentItemInLoop:
        ui->loop_mode_pushButton->setIcon(QIcon(":/res/all.jpg"));
        playList->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    }
}
