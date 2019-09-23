'''
Sept. 2019
@author: Wanli Zhu
'''

import os
import sys
import datetime
import time
import shutil
import subprocess as subproc
import CMake as cm
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *


class Model(QObject):
    def __init__(self):
        super(QObject, self).__init__()

        self.file_dir = os.path.dirname(os.path.realpath(__file__))
        self.manifest_filename = os.path.join(self.file_dir, 'MANIFEST.ini')


class MainControl(QObject):
    def __init__(self, model):
        super(QObject, self).__init__()
        self.model = model

    def actionTodo(self):
        pass

    def openManifest(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog()
        filename, _ = QFileDialog.getOpenFileName(self, 
                                                  'Open MANIFEST.ini File', 
                                                  '', 
                                                  'All Files (*);;Setting Files (*.ini)',
                                                  options=options)
        if filename:
            self.model.manifest_filename = filename
        
        pass

    def saveManifest(self):
        pass

    def saveAsManifest(self):
        pass

class MainWindow(QMainWindow):
    def __init__(self, model, controller):
        super(QMainWindow, self).__init__()
        self.model = model
        self.controller = controller
        self.initUI()
        self.resize(742, 1200)

    def initUI(self):
        self.setWindowFlags(self.windowFlags() | Qt.CustomizeWindowHint)
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowMaximizeButtonHint)

        self.file_dir = os.path.dirname(os.path.realpath(__file__))
        self.setWindowIcon(QIcon(os.path.join(self.file_dir, 'logo.ico')))
        self.setWindowTitle('AMake GUI')

        self.__createMenuBar()        

    def __createMenuBar(self):
        menubar    = self.menuBar()
        menu_file  = menubar.addMenu('File')
        menu_view  = menubar.addMenu('View')
        menu_shell = menubar.addMenu('Shell')
        menu_tools = menubar.addMenu('Tools')
        menu_help  = menubar.addMenu('Help')

        # Menu File
        menu_file.addAction(self.__createAction('Open', 
                                                self.controller.openManifest,
                                                shortcut='Ctrl+O',
                                                tip='Load settings from MANIFEST.ini')
        menu_file.addAction(self.__createAction('Save', 
                                                self.controller.saveManifest,
                                                shortcut='Ctrl+S',
                                                tip='Save settings to MANIFEST.ini')
        menu_file.addAction(self.__createAction('Save As', 
                                                self.controller.saveAsManifest,
                                                shortcut='Ctrl+Shift+S',
                                                tip='Save settings to a new file')   
        menu_file.addAction(self.__createAction('Open Recent', 
                                                self.controller.actionTodo, 
                                                shortcut=None, 
                                                tip='Open recent files'))  
        menu_file.addSeparator()           
        menu_file.addAction(self.__createAction('Exit', 
                                                self.close, 
                                                shortcut='Ctrl+Q', 
                                                tip='Exit Application'))    
        
        # Menu View
        menu_view.addAction(self.__createAction('Show Advanced',
                                                self.showAdvanced,
                                                shortcut=None,
                                                tip='Show advanced options',
                                                checkable=True))

        # Menu Shell
        menu_shell.addAction(self.__createAction('Open Shell',))
        # Menu Tools

        # Menu Help

        

    def __createAction(self, 
                       text,
                       slot=None, 
                       shortcut=None,      # is of type QShortcut
                       icon=None, 
                       tip=None, 
                       checkable=False, 
                       signal='triggered'):
        """
        Return a QAction given a number of common QAction attributes
        Just a shortcut function Originally borrowed from
        'Rapid GUI Development with PyQT' by Mark Summerset
        """
        action = QAction(text, self)
        if icon is not None:
            action.setIcon(QIcon.fromTheme(icon))
        
        if shortcut is not None and shortcut:
            tip += ' ({})'.format(shortcut)
            QShortcut(QKeySequence(shortcut), self).activated.connect(self.close)
        
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)

        if slot is not None:
            getattr(action, signal).connect(slot)
        
        if checkable:
            action.setCheckable()

        return action

    def showAdvanced(self):
        pass


class AMakeApp(QApplication):
    def __init__(self, sys_argv):
        super(QApplication, self).__init__(sys_argv)
        self.model = Model()
        self.controller = MainControl(self.model)
        self.view = MainWindow(self.model, self.controller)
    
    def run(self):
        self.view.show()
        return app.exec_()

if __name__ == '__main__':
    app = AMakeApp(sys.argv)
    sys.exit(app.run())

