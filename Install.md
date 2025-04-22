### **1. Install Chocolatey**
Chocolatey is a package manager for Windows, making software installation easier.

1. **Open PowerShell as Administrator** (Search for "PowerShell", right-click "Run as administrator").
2. Run this command to install Chocolatey:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
   ```
3. Close and reopen PowerShell to refresh the environment.

---

### **2. Install Python**
Chocolatey helps install Python with `pip` enabled.

1. Run:
   ```powershell
   choco install python -y
   ```
2. Close and reopen PowerShell, then verify the installation:
   ```powershell
   python --version
   pip --version
   ```

If it does not work, restart your computer.

---

### **3. Install MkDocs**
Once Python is installed, use `pip` to install MkDocs.

1. Run:
   ```powershell
   pip install mkdocs
   ```
2. Verify installation:
   ```powershell
   mkdocs --version
   ```

---

### **4. Create an MkDocs Project**
1. Navigate to your desired folder:
   ```powershell
   cd C:\Users\YourUsername\Documents
   ```
2. Create a new MkDocs project:
   ```powershell
   mkdocs new my-blog
   cd my-blog
   ```
3. Serve the documentation locally:
   ```powershell
   mkdocs serve
   ```
   Open `http://127.0.0.1:8000/` in your browser.

---

### **5. Enable Dark Theme**
Edit `mkdocs.yml` in your project folder:

1. Open it with Notepad:
   ```powershell
   notepad mkdocs.yml
   ```
2. Modify the `theme` section:
   ```yaml
   theme:
     name: material
     palette:
       scheme: slate
   ```
3. Install the Material theme:
   ```powershell
   pip install mkdocs-material
   ```
4. Restart the MkDocs server:
   ```powershell
   mkdocs serve
   ```

---

### **6. Manage MkDocs as a GitHub Repo**
1. **Initialize a Git repository**:
   ```powershell
   git init
   git add .
   git commit -m "Initial commit"
   ```
2. **Create a GitHub repository** from GitHub.com.
3. **Link your local folder to GitHub**:
   ```powershell
   git remote add origin https://github.com/yourusername/yourrepo.git
   git branch -M main
   git push -u origin main
   ```

To publish it on GitHub Pages:
```powershell
mkdocs gh-deploy
```

---

This sets up Chocolatey, Python, and MkDocs with a dark theme and GitHub integration. 

Install chocolately 
Install python 
Install git 


Run UTF-8 Conversion of all files 
pip install chardet
python convert_to_utf8.py

