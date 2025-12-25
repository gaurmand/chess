#include "mainwindow.h"

namespace chess
{

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
   setWindowTitle(tr("chess"));
   setMinimumSize(QSize(600, 400));
}

} // namespace chess
