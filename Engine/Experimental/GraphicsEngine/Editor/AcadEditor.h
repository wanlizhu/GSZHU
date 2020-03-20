#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AcadEditor.h"

class AcadEditor : public QMainWindow
{
    Q_OBJECT
public:
    AcadEditor(QWidget *parent = Q_NULLPTR);

private:
    Ui::AcadEditorClass ui;
};
