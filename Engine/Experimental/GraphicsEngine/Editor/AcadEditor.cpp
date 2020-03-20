#include "AcadEditor.h"

AcadEditor::AcadEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/RES/logo64x64.bmp"));
}
