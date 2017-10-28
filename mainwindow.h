#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QStandardItemModel>
#include <QTime>
#include <QKeyEvent>
#include <QStyle>
//#include <QWinThumbnailToolBar>
//#include <QWinThumbnailToolButton>
#include <QtWin>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:

    //Отображение времени воспроизведения
    void playTimeDisplay(qint64 val);

    //Нажатие на пункт меню "Добавить файл"
    void on_add_file_action_triggered();

    //Нажатие на пункт меню "Добавить файлы"
    void on_add_files_action_triggered();

    //Нажатие на клавишу выбора следующего трека
    void on_next_pushButton_clicked();

    //Нажатие на клавишу предидущего трека
    void on_prev_pushButton_clicked();

    //Нажатие на клавишу начала воспроизведения
    void on_play_pushButton_clicked();

    //Наэатие на клавишу "Пауза"
    void on_pause_pushButton_clicked();

    //Двойной щелчок по списку воспроизведения
    void on_play_list_tableView_doubleClicked(const QModelIndex &index);

    //Действия при переходе к другому треку
    void onPlaylistIndexChanged(int index);

    //Нажатие на пункт меню "Выход"
    void on_exit_action_triggered();

    //Регулировка звука
    void on_volume_horizontalSlider_valueChanged(int value);

    //Перемотка с помощью ползунка
    void on_play_time_horizontalSlider_sliderMoved(int position);

    //Отключение звука при нажатии на ползунок перемотки
    void on_play_time_horizontalSlider_sliderPressed();

    //Включение звука при нажатии на ползунок перемотки
    void on_play_time_horizontalSlider_sliderReleased();

    //Нажатие на кнопку стоп
    void on_stop_pushButton_clicked();

    //Обработка нажатия клавиш
    virtual void keyPressEvent(QKeyEvent *event);

    void stylize();

    void on_volume_pushButton_clicked();

    void on_loop_mode_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QMediaPlaylist* playList;
    QStandardItemModel* itemModel;

    int prevVolume;
//    QWinThumbnailToolBar* thumbnailToolBar;
};

#endif // MAINWINDOW_H
