import tkinter as tk
from tkinter import ttk
import tkinter.font as tkfont
import os
import sys
import subprocess as subproc
from datetime import datetime as clock


globalFont = ('Neue Haas Grotesk Text Pro', '12', 'bold')

def run(cmd, print=False):
    if print:
        subproc.call(cmd, stdout=sys.stdout, stderr=sys.stderr, shell=False)
    else:
        subproc.call(cmd, shell=False)

class QueryGit:

    def __init__(self):
        pass
    
    def status(self):
        subproc.call('git status', stdout=sys.stdout, stderr=sys.stderr)
        desc = ''
        desc += 'HEAD: ' + subproc.check_output('git rev-parse --short HEAD').decode('utf-8') 
        desc += 'UPSTREAM: ' + subproc.check_output('git rev-parse --short @{u}').decode('utf-8')
        branch = 'Local Branch: '
        remoteName = 'xx'
        proc = subproc.Popen('git branch -vv', shell=False, stdout=subproc.PIPE)
        for lineBytes in proc.stdout.readlines():
            line = lineBytes.decode("utf-8") 
            if (line.startswith('*')):
                branch += line[2:line.find(']') + 1]
                trackedInfo = line[line.rfind('['):]
                remoteName = trackedInfo[1:trackedInfo.find('/')]
        proc.wait()
        branch += '\nRemote(%s): '%(remoteName) + subproc.check_output('git config --get remote.%s.url'%(remoteName)).decode('utf-8') + '\n'
        desc += branch

        numWSChanges = 0
        numStagedChanges = 0
        numUntracked = 0
        proc = subproc.Popen('git status --porcelain', shell=False, stdout=subproc.PIPE)
        for lineBytes in proc.stdout.readlines():
            line = lineBytes.decode('utf-8')
            if line[1] != ' ':
                numWSChanges += 1
            if line[0] != ' ':
                numStagedChanges += 1
            if line[0] == '?' and line[1] == '?':
                numUntracked += 1
        proc.wait()
        
        desc += 'Workspace Changes: [%d] Files\n'\
                'Staged Changes: [%s] Files\n'\
                'Untracked Changes: [%s] Files'\
                % (numWSChanges, numStagedChanges, numUntracked)
        
        return desc


class GitSettingsWindow():

    def __init__(self, parent):
        self._topLevel = tk.Toplevel(parent, width=300)
        self._topLevel.title('Git Settings')
        self._topLevel.grid()

        ttk.Button(self._topLevel, text='Save', command=lambda: self._save).grid(sticky='e')

        labelFrame1 = ttk.LabelFrame(self._topLevel, text='Config Source')
        labelFrame1.grid(pady=5, sticky='ew')

        self._selectedConfigSource = 'global'
        ttk.Radiobutton(labelFrame1, text='Local', variable=self._selectedConfigSource, value='local').grid(row=0, column=0, sticky='w')
        ttk.Radiobutton(labelFrame1, text='Global', variable=self._selectedConfigSource, value='global').grid(row=0, column=1, sticky='w')
        ttk.Radiobutton(labelFrame1, text='System', variable=self._selectedConfigSource, value='system').grid(row=0, column=2, sticky='w')

        labelFrame2 = ttk.LabelFrame(self._topLevel, text='User Info')
        labelFrame2.grid(pady=5, sticky='ew')

        ttk.Label(labelFrame2, text='Name: ', justify=tk.LEFT).grid(row=0, column=0, sticky='w')
        self._userName = ttk.Entry(labelFrame2, font=globalFont, width=20)
        self._userName.grid(row=0, column=1, sticky='w', ipady=3)

        ttk.Label(labelFrame2, text='Email: ', justify=tk.LEFT).grid(row=1, column=0, sticky='w')
        self._userEmail = ttk.Entry(labelFrame2, font=globalFont, width=20)
        self._userEmail.grid(row=1, column=1, sticky='w', ipady=3)

    def _save(self):
        pass



class GitWidget(ttk.Frame):

    def __init__(self, parent=None):
        super(GitWidget, self).__init__(parent)
        self.grid(sticky='ewns')
        self._createWidgets()
        self._loadStatus()

    def _createWidgets(self):
        ttk.Label(self, text='>> WORKSPACE', background='#00ffff', justify=tk.LEFT).grid(ipady = 5, sticky='w')
        self._statusLabel = ttk.Label(self, text='Uninitialized', justify='left')
        self._statusLabel.grid(sticky='w')
        
        ttk.Separator(self, orien='horizontal').grid(pady=10, sticky='ew')

        buttonFrame = ttk.Frame(self)
        buttonFrame.grid(pady=5, sticky='w')

        ttk.Button(buttonFrame, text='Scan', command=self._loadStatus).grid(row=0, column=0, sticky='w')
        ttk.Button(buttonFrame, text='Pull', command=lambda: run('git pull', True)).grid(row=0, column=1, sticky='w')
        ttk.Button(buttonFrame, text='Push', command=lambda: run('git push', True)).grid(row=0, column=2, sticky='w')
        ttk.Button(buttonFrame, text='Fetch', command=lambda: run('git fetch --all', True)).grid(row=0, column=3, sticky='w')
        
        self._commitMessage = ttk.Entry(self, font=globalFont, width=20)
        self._commitMessage.grid(ipady=3, sticky='ew')
        self._commitMessage.insert(0, 'Auto Commit (' +  clock.strftime(clock.now(), "%a, %b %d | %Y-%m-%d %X") + ')')
        commitFrame = ttk.Frame(self)
        commitFrame.grid(sticky='e')
        ttk.Button(commitFrame, text='Add *', command=lambda: run('git add .', True)).grid(row=0, column=0, sticky='e')
        ttk.Button(commitFrame, text='Commit', command=lambda: run('git commit -m \"%s\"' % self._commitMessage.get(), True)).grid(row=0, column=1, sticky='e')

        stashFrame = ttk.LabelFrame(self, text='Stash')
        stashFrame.grid(pady=5, sticky='ew')
        self._stashUntrackedFiles = False
        self._stashIgnoredFiles = False
        
        ttk.Radiobutton(stashFrame, text='Include untracked files', variable=self._stashUntrackedFiles, value=False).grid(row=0, column=0, sticky='w', columnspan=2)
        ttk.Radiobutton(stashFrame, text='Include ignored files', variable=self._stashIgnoredFiles, value=False).grid(row=1, column=0, sticky='w', columnspan=2)
        ttk.Button(stashFrame, text='Add Stash', command=self._addStash).grid(row=2, column=0, sticky='w')
        ttk.Button(stashFrame, text='Restore Stash', command=lambda: run('git stash pop', True)).grid(row=2, column=1, sticky='w', pady=5)

        undoFrame = ttk.LabelFrame(self, text='Undo')
        undoFrame.grid(pady=5, sticky='ew')
        ttk.Button(undoFrame, text='Undo', command=None).grid(row=0, column=0, sticky='w', pady=5)
        ttk.Button(undoFrame, text='Redo', command=None).grid(row=0, column=1, sticky='w')
        ttk.Button(undoFrame, text='Clean', command=None).grid(row=0, column=2, sticky='w')

    def _openSettings(self):
        settingsWindow = GitSettingsWindow(self)

    def _loadStatus(self):
        self._statusLabel.configure(text=QueryGit().status())
        
    def _addStash(self):
        cmd = 'git stash'
        if self._stashUntrackedFiles and self._stashIgnoredFiles:
            cmd += ' --all'
        elif self._stashUntrackedFiles:
            cmd += ' --include-untracked'
        run(cmd, True)



class BuildWidget(ttk.Frame):

    def __init__(self, parent=None):
        super(BuildWidget, self).__init__(parent)
        self.grid(sticky='ewns')
        self._createWidgets()

    def _createWidgets(self):
        pass


class Application:

    def __init__(self):
        self._root = tk.Tk()
        self._root.grid()
        self._root.title('NTGS Builder')

        self._createStyle()
        self._createFrames()

    def _createStyle(self):
        style = ttk.Style()
        style.theme_use('default')
        style.configure('.', font=globalFont)
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
    
    def _createFrames(self):
        ttk.Button(self._root, text='Open Terminal', command=lambda: os.system('start bash')).grid(sticky='e')

        self._notebook = ttk.Notebook(self._root)
        self._notebook.grid(sticky='ewns')
        self._frameGit = GitWidget(self._notebook)
        self._frameBuild = BuildWidget(self._notebook)
        self._notebook.add(self._frameBuild, text='  [ BUILD ]  ')
        self._notebook.add(self._frameGit, text='   [ GIT ]   ')
        
    def mainLoop(self):
        self._root.mainloop()


if __name__=='__main__':
    app = Application()
    app.mainLoop()
