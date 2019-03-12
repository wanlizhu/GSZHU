import tkinter as tk
from tkinter import ttk
import tkinter.font as tkfont
import os
import subprocess 


class QueryGit:

    def __init__(self, repo):
        self.repo = repo
        os.system('cd ' + self.repo)
    
    def status(self):
        desc = ''
        branch = 'Current Branch: '
        proc = subprocess.Popen('git branch -vv', shell=False, stdout=subprocess.PIPE)
        for lineBytes in proc.stdout.readlines():
            line = lineBytes.decode("utf-8") 
            if (line.startswith('*')):
                branch += line[2:line.find(']') + 1]
        proc.wait()
        desc += branch
        return desc
    
    def info(self):
        desc = ''
        desc += 'User: ' + subprocess.check_output('git config --global user.name').decode('utf8')
        desc += 'Email: ' + subprocess.check_output('git config --global user.email').decode('utf8')
        return desc


class GitSettingsWindow():

    def __init__(self, parent):
        self.topLevel = tk.Toplevel(parent, width=300)
        self.topLevel.title('Git Settings')
        self.topLevel.grid()

        ttk.Button(self.topLevel, text='Save', command=lambda: self.save).grid(pady=5, sticky='w')

        self.labelFrame1 = ttk.LabelFrame(self.topLevel, text='Config Source')
        self.labelFrame1.grid(pady=5)

        self.selectedConfigSource = 'global'
        ttk.Radiobutton(self.labelFrame1, text='Local', variable=self.selectedConfigSource, value='local').grid(row=0, column=0, sticky='w')
        ttk.Radiobutton(self.labelFrame1, text='Global', variable=self.selectedConfigSource, value='global').grid(row=0, column=1, sticky='w')
        ttk.Radiobutton(self.labelFrame1, text='System', variable=self.selectedConfigSource, value='system').grid(row=0, column=2, sticky='w')

        self.labelFrame2 = ttk.LabelFrame(self.topLevel, text='User Info')
        self.labelFrame2.grid(pady=5)

        ttk.Label(self.labelFrame2, text='Name: ', justify=tk.LEFT).grid(row=0, column=0, sticky='w')
        self.userName = ttk.Entry(self.labelFrame2)
        self.userName.grid(row=0, column=1, sticky='w')

        ttk.Label(self.labelFrame2, text='Email: ', justify=tk.LEFT).grid(row=1, column=0, sticky='w')
        self.userEmail = ttk.Entry(self.labelFrame2)
        self.userEmail.grid(row=1, column=1, sticky='w')

    def save(self):
        pass



class GitWidget(ttk.Frame):

    def __init__(self, parent=None):
        super(GitWidget, self).__init__(parent)
        self.grid(sticky='ewns')
        self.createWidgets()
        self.loadUserEmail()
        self.loadStatus()

    def createWidgets(self):
        ttk.Label(self, text='>> CONFIG', background='#00ffff', justify=tk.LEFT).grid(ipady = 5, sticky='w')
        ttk.Button(self, text='Open Settings', command=self.openSettings).grid(sticky='w')

        self.repoLabel = ttk.Label(self, text='Uninitialized', justify=tk.LEFT)
        self.repoLabel.grid(ipady = 5, sticky='w')

        ttk.Separator(self, orien='horizontal').grid(pady = 10, sticky='ew')

        ttk.Label(self, text='>> WORKSPACE', background='#00ffff', justify=tk.LEFT).grid(ipady = 5, sticky='w')
        ttk.Button(self, text='Load Status', command=self.loadStatus).grid(sticky='w')

        self.statusLabel = ttk.Label(self, text='Uninitialized', justify=tk.LEFT)
        self.statusLabel.grid(sticky='w')

        ttk.Separator(self, orien='horizontal').grid(pady = 10, sticky='ew')

    def openSettings(self):
        settingsWindow = GitSettingsWindow(self)

    def loadUserEmail(self):
        self.repoLabel.configure(text=QueryGit(os.getcwd()).info())

    def loadStatus(self):
        self.statusLabel.configure(text=QueryGit(os.getcwd()).status())

    def runPull(self):
        pass

    def runPush(self):
        pass
        
    def runStash(self):
        pass


class Application:

    def __init__(self):
        self.root = tk.Tk()
        self.root.grid()
        self.root.title('NTGS Builder')

        self.createStyle()
        self.createFrames()

    def createStyle(self):
        style = ttk.Style()
        style.theme_use('default')
        style.configure('.', font=('Fixdsys', '10', 'bold'))
        style.configure('TButton', )
        style.configure('TCheckbutton', )
        style.configure('TCombobox', )
        style.configure('TEntry', )
        style.configure('TFrame', )
        style.configure('TLabel', )
        style.configure('TLabelFrame', )
        style.configure('TMenubutton', )
        style.configure('TNotebook', )
        style.configure('TPanedwindow', )
        style.configure('Horizontal.TProgressbar', )
        style.configure('Vertical.TProgressbar', )
        style.configure('TRadiobutton', )
        style.configure('Horizontal.TScale', )
        style.configure('Vertical.TScale', )
        style.configure('Horizontal.TScrollbar', )
        style.configure('Vertical.TScrollbar', )
        style.configure('TSeparator', )
        style.configure('TSizegrip', )
        style.configure('Treeview', )
    
    def createFrames(self):
        ttk.Button(self.root, text='Open Terminal', command=lambda: os.system('start bash')).grid(sticky='e')

        self.notebook = ttk.Notebook(self.root, width=485, height=600)
        self.notebook.grid(sticky='ewns')
        self.frameGit = GitWidget(self.notebook)
        self.frameBuild = ttk.Frame(self.notebook)
        self.notebook.add(self.frameGit, text='[ GIT ]')
        self.notebook.add(self.frameBuild, text='[ BUILD ]')
        
    def mainLoop(self):
        self.root.mainloop()


if __name__=='__main__':
    app = Application()
    app.mainLoop()
