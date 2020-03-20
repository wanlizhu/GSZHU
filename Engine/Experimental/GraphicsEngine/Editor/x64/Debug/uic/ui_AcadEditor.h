/********************************************************************************
** Form generated from reading UI file 'AcadEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACADEDITOR_H
#define UI_ACADEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AcadEditorClass
{
public:
    QAction *action_file_save;
    QAction *action_file_exit;
    QAction *action_file_new;
    QAction *action_file_open;
    QAction *action_file_open_recent;
    QAction *action_file_save_as;
    QAction *action_file_export_image;
    QAction *action_edit_copy;
    QAction *action_edit_paste;
    QAction *action_edit_cut;
    QAction *action_view_property_panel;
    QAction *action_view_animation_panel;
    QAction *action_view_grid;
    QAction *action_view_coordination;
    QAction *action_view_zoom_in;
    QAction *action_view_zoom_out;
    QAction *action_view_reset_camera;
    QAction *action_view_shading_pbr;
    QAction *action_view_shading_wireframe;
    QAction *action_view_shading_smooth_shaded;
    QAction *action_view_shading_smooth_shaded_and_wireframe;
    QAction *action_view_shading_albedo;
    QAction *action_view_shading_normal;
    QAction *action_view_shading_specular_metallic;
    QAction *action_view_shading_gloss_roughness;
    QAction *action_view_shading_occlussion;
    QAction *action_view_shading_opacity;
    QAction *actionEnvironment_and_Lighting;
    QAction *actionShading_2;
    QAction *actionGrid_and_Views;
    QAction *actionCube;
    QAction *actionPolyline;
    QAction *actionRectangle;
    QAction *actionCircle;
    QAction *actionEcllipse;
    QAction *actionSpline;
    QAction *actionArc;
    QAction *actionCube_2;
    QAction *actionSphere;
    QAction *actionEllipsoid;
    QAction *actionTorus;
    QAction *actionBunny;
    QAction *actionDragon;
    QAction *actionLucy;
    QAction *actionTeapot;
    QAction *actionWineglass;
    QAction *actionPBR;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuShading;
    QMenu *menuTools;
    QMenu *menuPrimitives;
    QMenu *menuModel;
    QMenu *menuMaterials;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AcadEditorClass)
    {
        if (AcadEditorClass->objectName().isEmpty())
            AcadEditorClass->setObjectName(QString::fromUtf8("AcadEditorClass"));
        AcadEditorClass->resize(1000, 618);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(9);
        AcadEditorClass->setFont(font);
        action_file_save = new QAction(AcadEditorClass);
        action_file_save->setObjectName(QString::fromUtf8("action_file_save"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/RES/ic_save_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_save->setIcon(icon);
        action_file_save->setFont(font);
        action_file_exit = new QAction(AcadEditorClass);
        action_file_exit->setObjectName(QString::fromUtf8("action_file_exit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/RES/ic_exit_to_app_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_exit->setIcon(icon1);
        action_file_exit->setFont(font);
        action_file_new = new QAction(AcadEditorClass);
        action_file_new->setObjectName(QString::fromUtf8("action_file_new"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/RES/ic_create_new_folder_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_new->setIcon(icon2);
        action_file_new->setFont(font);
        action_file_open = new QAction(AcadEditorClass);
        action_file_open->setObjectName(QString::fromUtf8("action_file_open"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/RES/ic_folder_open_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_open->setIcon(icon3);
        action_file_open->setFont(font);
        action_file_open_recent = new QAction(AcadEditorClass);
        action_file_open_recent->setObjectName(QString::fromUtf8("action_file_open_recent"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/RES/ic_history_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_open_recent->setIcon(icon4);
        action_file_save_as = new QAction(AcadEditorClass);
        action_file_save_as->setObjectName(QString::fromUtf8("action_file_save_as"));
        action_file_export_image = new QAction(AcadEditorClass);
        action_file_export_image->setObjectName(QString::fromUtf8("action_file_export_image"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/RES/ic_image_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_file_export_image->setIcon(icon5);
        action_edit_copy = new QAction(AcadEditorClass);
        action_edit_copy->setObjectName(QString::fromUtf8("action_edit_copy"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/RES/ic_content_copy_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_edit_copy->setIcon(icon6);
        action_edit_paste = new QAction(AcadEditorClass);
        action_edit_paste->setObjectName(QString::fromUtf8("action_edit_paste"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/RES/ic_content_paste_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_edit_paste->setIcon(icon7);
        action_edit_cut = new QAction(AcadEditorClass);
        action_edit_cut->setObjectName(QString::fromUtf8("action_edit_cut"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/RES/ic_content_cut_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_edit_cut->setIcon(icon8);
        action_view_property_panel = new QAction(AcadEditorClass);
        action_view_property_panel->setObjectName(QString::fromUtf8("action_view_property_panel"));
        action_view_animation_panel = new QAction(AcadEditorClass);
        action_view_animation_panel->setObjectName(QString::fromUtf8("action_view_animation_panel"));
        action_view_grid = new QAction(AcadEditorClass);
        action_view_grid->setObjectName(QString::fromUtf8("action_view_grid"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/RES/ic_grid_on_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_view_grid->setIcon(icon9);
        action_view_coordination = new QAction(AcadEditorClass);
        action_view_coordination->setObjectName(QString::fromUtf8("action_view_coordination"));
        action_view_zoom_in = new QAction(AcadEditorClass);
        action_view_zoom_in->setObjectName(QString::fromUtf8("action_view_zoom_in"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/RES/ic_zoom_in_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_view_zoom_in->setIcon(icon10);
        action_view_zoom_out = new QAction(AcadEditorClass);
        action_view_zoom_out->setObjectName(QString::fromUtf8("action_view_zoom_out"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/RES/ic_zoom_out_black_48dp.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_view_zoom_out->setIcon(icon11);
        action_view_reset_camera = new QAction(AcadEditorClass);
        action_view_reset_camera->setObjectName(QString::fromUtf8("action_view_reset_camera"));
        action_view_shading_pbr = new QAction(AcadEditorClass);
        action_view_shading_pbr->setObjectName(QString::fromUtf8("action_view_shading_pbr"));
        action_view_shading_pbr->setCheckable(true);
        action_view_shading_wireframe = new QAction(AcadEditorClass);
        action_view_shading_wireframe->setObjectName(QString::fromUtf8("action_view_shading_wireframe"));
        action_view_shading_wireframe->setCheckable(true);
        action_view_shading_smooth_shaded = new QAction(AcadEditorClass);
        action_view_shading_smooth_shaded->setObjectName(QString::fromUtf8("action_view_shading_smooth_shaded"));
        action_view_shading_smooth_shaded->setCheckable(true);
        action_view_shading_smooth_shaded_and_wireframe = new QAction(AcadEditorClass);
        action_view_shading_smooth_shaded_and_wireframe->setObjectName(QString::fromUtf8("action_view_shading_smooth_shaded_and_wireframe"));
        action_view_shading_smooth_shaded_and_wireframe->setCheckable(true);
        action_view_shading_albedo = new QAction(AcadEditorClass);
        action_view_shading_albedo->setObjectName(QString::fromUtf8("action_view_shading_albedo"));
        action_view_shading_albedo->setCheckable(true);
        action_view_shading_normal = new QAction(AcadEditorClass);
        action_view_shading_normal->setObjectName(QString::fromUtf8("action_view_shading_normal"));
        action_view_shading_normal->setCheckable(true);
        action_view_shading_specular_metallic = new QAction(AcadEditorClass);
        action_view_shading_specular_metallic->setObjectName(QString::fromUtf8("action_view_shading_specular_metallic"));
        action_view_shading_specular_metallic->setCheckable(true);
        action_view_shading_gloss_roughness = new QAction(AcadEditorClass);
        action_view_shading_gloss_roughness->setObjectName(QString::fromUtf8("action_view_shading_gloss_roughness"));
        action_view_shading_gloss_roughness->setCheckable(true);
        action_view_shading_occlussion = new QAction(AcadEditorClass);
        action_view_shading_occlussion->setObjectName(QString::fromUtf8("action_view_shading_occlussion"));
        action_view_shading_occlussion->setCheckable(true);
        action_view_shading_opacity = new QAction(AcadEditorClass);
        action_view_shading_opacity->setObjectName(QString::fromUtf8("action_view_shading_opacity"));
        action_view_shading_opacity->setCheckable(true);
        actionEnvironment_and_Lighting = new QAction(AcadEditorClass);
        actionEnvironment_and_Lighting->setObjectName(QString::fromUtf8("actionEnvironment_and_Lighting"));
        actionShading_2 = new QAction(AcadEditorClass);
        actionShading_2->setObjectName(QString::fromUtf8("actionShading_2"));
        actionGrid_and_Views = new QAction(AcadEditorClass);
        actionGrid_and_Views->setObjectName(QString::fromUtf8("actionGrid_and_Views"));
        actionCube = new QAction(AcadEditorClass);
        actionCube->setObjectName(QString::fromUtf8("actionCube"));
        actionPolyline = new QAction(AcadEditorClass);
        actionPolyline->setObjectName(QString::fromUtf8("actionPolyline"));
        actionRectangle = new QAction(AcadEditorClass);
        actionRectangle->setObjectName(QString::fromUtf8("actionRectangle"));
        actionCircle = new QAction(AcadEditorClass);
        actionCircle->setObjectName(QString::fromUtf8("actionCircle"));
        actionEcllipse = new QAction(AcadEditorClass);
        actionEcllipse->setObjectName(QString::fromUtf8("actionEcllipse"));
        actionSpline = new QAction(AcadEditorClass);
        actionSpline->setObjectName(QString::fromUtf8("actionSpline"));
        actionArc = new QAction(AcadEditorClass);
        actionArc->setObjectName(QString::fromUtf8("actionArc"));
        actionCube_2 = new QAction(AcadEditorClass);
        actionCube_2->setObjectName(QString::fromUtf8("actionCube_2"));
        actionSphere = new QAction(AcadEditorClass);
        actionSphere->setObjectName(QString::fromUtf8("actionSphere"));
        actionEllipsoid = new QAction(AcadEditorClass);
        actionEllipsoid->setObjectName(QString::fromUtf8("actionEllipsoid"));
        actionTorus = new QAction(AcadEditorClass);
        actionTorus->setObjectName(QString::fromUtf8("actionTorus"));
        actionBunny = new QAction(AcadEditorClass);
        actionBunny->setObjectName(QString::fromUtf8("actionBunny"));
        actionDragon = new QAction(AcadEditorClass);
        actionDragon->setObjectName(QString::fromUtf8("actionDragon"));
        actionLucy = new QAction(AcadEditorClass);
        actionLucy->setObjectName(QString::fromUtf8("actionLucy"));
        actionTeapot = new QAction(AcadEditorClass);
        actionTeapot->setObjectName(QString::fromUtf8("actionTeapot"));
        actionWineglass = new QAction(AcadEditorClass);
        actionWineglass->setObjectName(QString::fromUtf8("actionWineglass"));
        actionPBR = new QAction(AcadEditorClass);
        actionPBR->setObjectName(QString::fromUtf8("actionPBR"));
        centralWidget = new QWidget(AcadEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        AcadEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AcadEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setFont(font);
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuShading = new QMenu(menuView);
        menuShading->setObjectName(QString::fromUtf8("menuShading"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuPrimitives = new QMenu(menuBar);
        menuPrimitives->setObjectName(QString::fromUtf8("menuPrimitives"));
        menuModel = new QMenu(menuBar);
        menuModel->setObjectName(QString::fromUtf8("menuModel"));
        menuMaterials = new QMenu(menuBar);
        menuMaterials->setObjectName(QString::fromUtf8("menuMaterials"));
        AcadEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AcadEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AcadEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AcadEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AcadEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuPrimitives->menuAction());
        menuBar->addAction(menuModel->menuAction());
        menuBar->addAction(menuMaterials->menuAction());
        menuFile->addAction(action_file_new);
        menuFile->addAction(action_file_open);
        menuFile->addAction(action_file_open_recent);
        menuFile->addSeparator();
        menuFile->addAction(action_file_save);
        menuFile->addAction(action_file_save_as);
        menuFile->addAction(action_file_export_image);
        menuFile->addSeparator();
        menuFile->addAction(action_file_exit);
        menuEdit->addAction(action_edit_copy);
        menuEdit->addAction(action_edit_cut);
        menuEdit->addAction(action_edit_paste);
        menuView->addAction(action_view_property_panel);
        menuView->addAction(action_view_animation_panel);
        menuView->addAction(action_view_grid);
        menuView->addAction(action_view_coordination);
        menuView->addSeparator();
        menuView->addAction(action_view_zoom_in);
        menuView->addAction(action_view_zoom_out);
        menuView->addSeparator();
        menuView->addAction(action_view_reset_camera);
        menuView->addSeparator();
        menuView->addAction(menuShading->menuAction());
        menuShading->addAction(action_view_shading_pbr);
        menuShading->addAction(action_view_shading_wireframe);
        menuShading->addAction(action_view_shading_smooth_shaded);
        menuShading->addAction(action_view_shading_smooth_shaded_and_wireframe);
        menuShading->addAction(action_view_shading_albedo);
        menuShading->addAction(action_view_shading_normal);
        menuShading->addAction(action_view_shading_specular_metallic);
        menuShading->addAction(action_view_shading_gloss_roughness);
        menuShading->addAction(action_view_shading_occlussion);
        menuShading->addAction(action_view_shading_opacity);
        menuTools->addAction(actionEnvironment_and_Lighting);
        menuTools->addAction(actionShading_2);
        menuTools->addAction(actionGrid_and_Views);
        menuPrimitives->addAction(actionCube);
        menuPrimitives->addAction(actionPolyline);
        menuPrimitives->addAction(actionRectangle);
        menuPrimitives->addAction(actionArc);
        menuPrimitives->addAction(actionCircle);
        menuPrimitives->addAction(actionSpline);
        menuPrimitives->addSeparator();
        menuPrimitives->addAction(actionCube_2);
        menuPrimitives->addAction(actionSphere);
        menuPrimitives->addAction(actionEllipsoid);
        menuPrimitives->addAction(actionTorus);
        menuModel->addAction(actionTeapot);
        menuModel->addAction(actionBunny);
        menuModel->addAction(actionDragon);
        menuModel->addAction(actionLucy);
        menuModel->addAction(actionWineglass);
        mainToolBar->addAction(action_file_new);
        mainToolBar->addAction(action_file_open);
        mainToolBar->addAction(action_file_save);
        mainToolBar->addAction(action_file_export_image);
        mainToolBar->addSeparator();

        retranslateUi(AcadEditorClass);

        QMetaObject::connectSlotsByName(AcadEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AcadEditorClass)
    {
        AcadEditorClass->setWindowTitle(QCoreApplication::translate("AcadEditorClass", "AcadEditor", nullptr));
        action_file_save->setText(QCoreApplication::translate("AcadEditorClass", "Save", nullptr));
#if QT_CONFIG(tooltip)
        action_file_save->setToolTip(QCoreApplication::translate("AcadEditorClass", "Save to file", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        action_file_save->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        action_file_exit->setText(QCoreApplication::translate("AcadEditorClass", "Exit", nullptr));
        action_file_new->setText(QCoreApplication::translate("AcadEditorClass", "New", nullptr));
#if QT_CONFIG(tooltip)
        action_file_new->setToolTip(QCoreApplication::translate("AcadEditorClass", "Create new scene", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        action_file_new->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        action_file_open->setText(QCoreApplication::translate("AcadEditorClass", "Open", nullptr));
#if QT_CONFIG(tooltip)
        action_file_open->setToolTip(QCoreApplication::translate("AcadEditorClass", "Open scene from file", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        action_file_open->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        action_file_open_recent->setText(QCoreApplication::translate("AcadEditorClass", "Open Recent", nullptr));
        action_file_save_as->setText(QCoreApplication::translate("AcadEditorClass", "Save as", nullptr));
        action_file_export_image->setText(QCoreApplication::translate("AcadEditorClass", "Export Image", nullptr));
        action_edit_copy->setText(QCoreApplication::translate("AcadEditorClass", "Copy", nullptr));
#if QT_CONFIG(shortcut)
        action_edit_copy->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        action_edit_paste->setText(QCoreApplication::translate("AcadEditorClass", "Paste", nullptr));
#if QT_CONFIG(shortcut)
        action_edit_paste->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        action_edit_cut->setText(QCoreApplication::translate("AcadEditorClass", "Cut", nullptr));
#if QT_CONFIG(shortcut)
        action_edit_cut->setShortcut(QCoreApplication::translate("AcadEditorClass", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_property_panel->setText(QCoreApplication::translate("AcadEditorClass", "Property Panel", nullptr));
        action_view_animation_panel->setText(QCoreApplication::translate("AcadEditorClass", "Animation Panel", nullptr));
        action_view_grid->setText(QCoreApplication::translate("AcadEditorClass", "Grid", nullptr));
#if QT_CONFIG(shortcut)
        action_view_grid->setShortcut(QCoreApplication::translate("AcadEditorClass", "G", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_coordination->setText(QCoreApplication::translate("AcadEditorClass", "Coordinates", nullptr));
#if QT_CONFIG(shortcut)
        action_view_coordination->setShortcut(QCoreApplication::translate("AcadEditorClass", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_zoom_in->setText(QCoreApplication::translate("AcadEditorClass", "Zoom In", nullptr));
#if QT_CONFIG(shortcut)
        action_view_zoom_in->setShortcut(QCoreApplication::translate("AcadEditorClass", "+", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_zoom_out->setText(QCoreApplication::translate("AcadEditorClass", "Zoom Out", nullptr));
#if QT_CONFIG(shortcut)
        action_view_zoom_out->setShortcut(QCoreApplication::translate("AcadEditorClass", "-", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_reset_camera->setText(QCoreApplication::translate("AcadEditorClass", "Reset Camera", nullptr));
#if QT_CONFIG(shortcut)
        action_view_reset_camera->setShortcut(QCoreApplication::translate("AcadEditorClass", "`", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_pbr->setText(QCoreApplication::translate("AcadEditorClass", "Physically Based Rendering", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_pbr->setShortcut(QCoreApplication::translate("AcadEditorClass", "1", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_wireframe->setText(QCoreApplication::translate("AcadEditorClass", "Wireframe", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_wireframe->setShortcut(QCoreApplication::translate("AcadEditorClass", "2", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_smooth_shaded->setText(QCoreApplication::translate("AcadEditorClass", "Smooth Shaded", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_smooth_shaded->setShortcut(QCoreApplication::translate("AcadEditorClass", "3", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_smooth_shaded_and_wireframe->setText(QCoreApplication::translate("AcadEditorClass", "Smooth Shaded and Wireframe", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_smooth_shaded_and_wireframe->setShortcut(QCoreApplication::translate("AcadEditorClass", "4", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_albedo->setText(QCoreApplication::translate("AcadEditorClass", "Albedo", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_albedo->setShortcut(QCoreApplication::translate("AcadEditorClass", "5", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_normal->setText(QCoreApplication::translate("AcadEditorClass", "Normal", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_normal->setShortcut(QCoreApplication::translate("AcadEditorClass", "6", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_specular_metallic->setText(QCoreApplication::translate("AcadEditorClass", "Specular/Metallic", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_specular_metallic->setShortcut(QCoreApplication::translate("AcadEditorClass", "7", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_gloss_roughness->setText(QCoreApplication::translate("AcadEditorClass", "Gloss/Roughness", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_gloss_roughness->setShortcut(QCoreApplication::translate("AcadEditorClass", "8", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_occlussion->setText(QCoreApplication::translate("AcadEditorClass", "Occlusion", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_occlussion->setShortcut(QCoreApplication::translate("AcadEditorClass", "9", nullptr));
#endif // QT_CONFIG(shortcut)
        action_view_shading_opacity->setText(QCoreApplication::translate("AcadEditorClass", "Opacity", nullptr));
#if QT_CONFIG(shortcut)
        action_view_shading_opacity->setShortcut(QCoreApplication::translate("AcadEditorClass", "0", nullptr));
#endif // QT_CONFIG(shortcut)
        actionEnvironment_and_Lighting->setText(QCoreApplication::translate("AcadEditorClass", "Environment and Lighting", nullptr));
        actionShading_2->setText(QCoreApplication::translate("AcadEditorClass", "Shading", nullptr));
        actionGrid_and_Views->setText(QCoreApplication::translate("AcadEditorClass", "Grid and Views", nullptr));
        actionCube->setText(QCoreApplication::translate("AcadEditorClass", "Segment", nullptr));
        actionPolyline->setText(QCoreApplication::translate("AcadEditorClass", "Polyline", nullptr));
        actionRectangle->setText(QCoreApplication::translate("AcadEditorClass", "Rectangle", nullptr));
        actionCircle->setText(QCoreApplication::translate("AcadEditorClass", "Circle", nullptr));
        actionEcllipse->setText(QCoreApplication::translate("AcadEditorClass", "Ellipse", nullptr));
        actionSpline->setText(QCoreApplication::translate("AcadEditorClass", "Spline", nullptr));
        actionArc->setText(QCoreApplication::translate("AcadEditorClass", "Arc", nullptr));
        actionCube_2->setText(QCoreApplication::translate("AcadEditorClass", "Cube", nullptr));
        actionSphere->setText(QCoreApplication::translate("AcadEditorClass", "Sphere", nullptr));
        actionEllipsoid->setText(QCoreApplication::translate("AcadEditorClass", "Cylinder", nullptr));
        actionTorus->setText(QCoreApplication::translate("AcadEditorClass", "Torus", nullptr));
        actionBunny->setText(QCoreApplication::translate("AcadEditorClass", "Bunny", nullptr));
        actionDragon->setText(QCoreApplication::translate("AcadEditorClass", "Dragon", nullptr));
        actionLucy->setText(QCoreApplication::translate("AcadEditorClass", "Lucy", nullptr));
        actionTeapot->setText(QCoreApplication::translate("AcadEditorClass", "Teapot", nullptr));
        actionWineglass->setText(QCoreApplication::translate("AcadEditorClass", "Wineglass", nullptr));
        actionPBR->setText(QCoreApplication::translate("AcadEditorClass", "PBR", nullptr));
        menuFile->setTitle(QCoreApplication::translate("AcadEditorClass", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("AcadEditorClass", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("AcadEditorClass", "View", nullptr));
        menuShading->setTitle(QCoreApplication::translate("AcadEditorClass", "Shading", nullptr));
        menuTools->setTitle(QCoreApplication::translate("AcadEditorClass", "Tools", nullptr));
        menuPrimitives->setTitle(QCoreApplication::translate("AcadEditorClass", "Primitives", nullptr));
        menuModel->setTitle(QCoreApplication::translate("AcadEditorClass", "Models", nullptr));
        menuMaterials->setTitle(QCoreApplication::translate("AcadEditorClass", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AcadEditorClass: public Ui_AcadEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACADEDITOR_H
