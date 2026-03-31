# 📂 WeanteX | WinPE File Deployment System
### Advanced Environment Setup Tool for Pre-Boot Operations

<div align="center">
  <img src="https://img.shields.io/badge/Task-File%20Deployment-F5363F?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Environment-WinPE-0078D6?style=for-the-badge&logo=windows" />
  <img src="https://img.shields.io/badge/Architecture-x64-white?style=for-the-badge" />
</div>

---

## 🔍 Concept & Usage
This tool is designed to automate **file deployment** within the **Windows Preinstallation Environment (WinPE)**. Instead of injecting code, it focuses on preparing the target environment by placing necessary files, drivers, or modules into restricted system directories before the main OS boots.

### Key Advantages:
* **No File Locks:** Deploy files that are normally "in use" or protected by Windows.
* **Pre-Boot Readiness:** Ensure all necessary components are in place before the game or anti-cheat initializes.
* **Custom UI:** Simple and effective graphical interface to manage file paths and deployment status.

---

## 🛠️ Functional Features
- [x] **Automated Path Resolution:** Automatically finds the target OS drive in PE.
- [x] **Integrity Check:** Verifies files before and after deployment.
- [x] **Native GUI:** Lightweight interface for monitoring the copy process.
- [ ] **Cloud-Based Updates:** Fetching the latest files directly from the server (In Progress).

---

## ⚠️ Important Note
This is a **Deployment Tool**, not an injector. It is part of the **WeanteX** ecosystem, intended for developers researching system setup and environment preparation.

<div align="right">
  <sub>Maintained by <b>Header1337</b> @ WeanteX</sub>
</div>
