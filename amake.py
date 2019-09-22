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
import tkinter as tk
from tkinter import ttk
from tkinter import scrolledtext
from tkinter import Menu
from tkinter import messagebox as mbox
from tkinter import filedialog as fd
from tkinter import Spinbox
import CMake as cmk


FONT_TOOLTIP = ('Microsoft YaHei', 9, 'normal')
FONT_UI      = ('Microsft YaHei', 10, 'normal')
FONT_UI_BOLD = ('Microsft YaHei', 10, 'bold')
FONT_MONO    = ('Courier New', 10, 'normal')


class ToolTip(object):
    def __init__(self, widget):
        self.widget = widget
        self.tip_window = None

    def show_tip(self, tip_text):
        "Display text in a tooltip window"
        if self.tip_window or not tip_text:
            return

        x, y, _cx, cy = self.widget.bbox('insert')
        x += self.widget.winfo_rootx() + 25
        y += self.widget.winfo_rooty() + 25 + cy       # below and to the right
        self.tip_window = tk.Toplevel(self.widget)
        self.tip_window.wm_overrideredirect(True)      # remove all Window Manager (wm) decorations
        self.tip_window.wm_geometry("+%d+%d" % (x, y)) # create window size

        label = tk.Label(self.tip_window,
                         text=tip_text,
                         justify=tk.LEFT,
                         background="#ffffe0", 
                         relief=tk.SOLID,
                         borderwidth=1,
                         font=FONT_TOOLTIP)
        label.pack(ipadx=1)

    def hide_tip(self):
        if self.tip_window:
            self.tip_window.destroy()
            self.tip_window = None

class Window():
    def __init__(self):
        self.win = tk.Tk()
        self.win.option_add('*font', FONT_UI)
        self.win.title('CMake Configuration Tool')
        #self.win.geometry('500x309')

        self.config = tk.StringVar()
        self.aget_directory = tk.StringVar()

        self._create_widgets()

    def run(self):
        self.win.mainloop()

    def _quit(self):
        self.win.quit()
        self.win.destroy()
        exit()

    def _ask_aget_directory(self):
        self.aget_directory = fd.askdirectory(parent=self.win, initialdir=os.path.dirname(__file__))
        self.entry_aget.delete(0, tk.END)
        self.entry_aget.insert(0, self.aget_directory)

    def _create_widgets(self):
        # AGet
        ttk.Label(self.win, text='AGET', font=FONT_UI_BOLD).grid(column=0, row=0, columnspan=3)
        
        ttk.Label(self.win, text='Where to store:').grid(column=0, row=1, padx=4)
        self.entry_aget = ttk.Entry(self.win, width=40, textvariable=self.aget_directory)
        self.entry_aget.grid(column=1, row=1)
        ttk.Button(self.win, text='...', width=5, command=self._ask_aget_directory).grid(column=2, row=1)

        """# Config
        ttk.Label(self.win, text='Configuration Type:').grid(column=0, row=2)
        
        config_combo = ttk.Combobox(self.win, width=15, state='readonly', textvariable=self.config)
        config_combo['values'] = ('DEBUG', 'RELEASE')
        config_combo.grid(column=0, row=2)
        config_combo.current(0)"""


if __name__=='__main__':
    win = Window()
    win.run()

