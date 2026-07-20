---
title: "CV | Jae Heung Park"
---

# Jae Heung Park (박재흥)

**Email:** stephenjhpark99@gmail.com  
**LinkedIn:** [linkedin.com/in/jaeheungpark](https://www.linkedin.com/in/jaeheung-park-6623ab1b1/)  
**GitHub:** [github.com/renaissancor](https://github.com/renaissancor)  
**Download:** [PDF (English)](cv.pdf) · [PDF (한국어)](cv-ko.pdf)  

---

## Introduction  

Versatile **software engineer** with experience spanning **web development**, **systems programming**, and **ML / data infrastructure**.  
At **i-Scream Media**, built and scaled a production **multi-modal content-understanding platform** — VLM, OCR, and ASR extraction pipelines processing **millions of assets** on on-premise **H200 GPU** infrastructure.  

Background in **C/C++ systems programming** (IOCP, multi-threaded architectures, Linux kernel) with prior internship experience in **React/TypeScript** web platforms at NAVER.  

Trilingual (Korean, English, Mandarin) with international experience across the US, China, and South Korea.

---

## Education  

**Shanghai Jiao Tong University**  
_Bachelor of Engineering (September 2018 – March 2025)_  
**University of Michigan – Shanghai Jiao Tong University Joint Institute**  
- Major: Electrical and Computer Engineering  
- Minor: Computer Science  
- Minor: Entrepreneurship  

**Fort Worth Christian School International**  
_March 2015 – December 2017_  
Pre-University Education  

---

## Programming Languages  

![C](https://img.shields.io/badge/-C-333333?style=flat&logo=c)
![C++](https://img.shields.io/badge/-C++-333333?style=flat&logo=cplusplus)
![Python](https://img.shields.io/badge/-Python-333333?style=flat&logo=python)
![Java](https://img.shields.io/badge/-Java-333333?style=flat&logo=java)
![JavaScript](https://img.shields.io/badge/-JavaScript-333333?style=flat&logo=javascript)
![Rust](https://img.shields.io/badge/-Rust-333333?style=flat&logo=rust)
![SQL](https://img.shields.io/badge/-SQL-333333?style=flat&logo=microsoft%20sql%20server)

---

## Frameworks  

![React](https://img.shields.io/badge/-React-333333?style=flat&logo=react)
![TypeScript](https://img.shields.io/badge/-TypeScript-333333?style=flat&logo=typescript)
![React Native](https://img.shields.io/badge/-React_Native-333333?style=flat&logo=react)
![Next.js](https://img.shields.io/badge/-Next.js-333333?style=flat&logo=nextdotjs)

---

## ML & Data Infrastructure  

![vLLM](https://img.shields.io/badge/-vLLM-333333?style=flat&logo=python)
![Hugging Face](https://img.shields.io/badge/-Hugging_Face-333333?style=flat&logo=huggingface)
![FastAPI](https://img.shields.io/badge/-FastAPI-333333?style=flat&logo=fastapi)
![Docker](https://img.shields.io/badge/-Docker-333333?style=flat&logo=docker)
![NVIDIA](https://img.shields.io/badge/-CUDA_/_H200_/_RTX_4090-333333?style=flat&logo=nvidia)
![DuckDB](https://img.shields.io/badge/-DuckDB-333333?style=flat&logo=duckdb)
![PostgreSQL](https://img.shields.io/badge/-PostgreSQL-333333?style=flat&logo=postgresql)
![MySQL](https://img.shields.io/badge/-MySQL-333333?style=flat&logo=mysql)
![Parquet](https://img.shields.io/badge/-Parquet-333333?style=flat&logo=apacheparquet)
![Streamlit](https://img.shields.io/badge/-Streamlit-333333?style=flat&logo=streamlit)
![FFmpeg](https://img.shields.io/badge/-FFmpeg-333333?style=flat&logo=ffmpeg)

---

## Collaboration & Tools  

![Git](https://img.shields.io/badge/-Git-333333?style=flat&logo=git)
![GitHub](https://img.shields.io/badge/-GitHub-333333?style=flat&logo=github)
![Slack](https://img.shields.io/badge/-Slack-333333?style=flat&logo=slack)
![Jira](https://img.shields.io/badge/-Jira-0052CC?style=flat&logo=jira)
![LaTeX](https://img.shields.io/badge/-LaTeX-333333?style=flat&logo=latex)
![Markdown](https://img.shields.io/badge/-Markdown-333333?style=flat&logo=markdown)
![MkDocs](https://img.shields.io/badge/-MkDocs-333333?style=flat&logo=mkdocs)

---

## Systems & Network Programming  

- Built a production **VLM image classification pipeline** on **H200 GPU** with **FP8/AWQ quantization** for cost-efficient on-premise inference.  
- Proficient in **Windows System Programming**, focusing on **asynchronous I/O** models such as **IOCP** and **Overlapped I/O**.  
- Experienced with **socket programming (TCP/UDP)** using **Winsock2** and **POSIX sockets**.  
- Designed and benchmarked **multi-threaded**, **event-driven**, and **asynchronous** server architectures.  
- Understanding of **packet processing**, **session management**, and **concurrency control** for scalable game server systems.  

---

## Experience  

### **i-Scream Media**  
_February 2026 – Present_  
**Junior Researcher**, Seoul, South Korea

- Built and scaled an on-premise **multi-modal content-understanding platform** that extracts metadata and curriculum tags from a large children's educational media library, spanning **images, video, documents, interactive HTML, and legacy Flash** assets under a unified metadata contract.  
- Shipped the original **VLM image-safety classifier** (**Gemma 4 31B, FP8**) on **H200 GPU** via **vLLM** — a **2-pass pipeline** (VLM visual analysis + deterministic profanity/slur text analysis) producing Korean media ratings (all/12/15/18), served via a **FastAPI** production server.  
- Engineered **high-throughput, resumable extraction pipelines** processing **5M+ images and 1.5M+ curriculum records at 99.9%+ success**, using transactional claim queues, checkpointing, and watchdog-based recovery; batching optimizations cut a projected multi-week run to a few days (**~5× throughput**).  
- Built **video and document AI pipelines** — frame/audio extraction with **Whisper** transcription and **VAD**, plus layout-aware OCR (**PaddleOCR-VL**) over PDF/PPT/HWP formats.  
- Designed the platform's **data layer**: stable content-hash identity, content-specific schemas, and an analytical **DuckDB / Parquet** mart benchmarked **50–180× faster** than the operational **MySQL / PostgreSQL** store.  
- Automated **Mac → H200 deployment** (standalone rsync scripts, systemd services, locked `uv` environments) and shipped **Streamlit / static QA dashboards** for coverage and data-quality review.  
- Managed shared multi-GPU infrastructure (**H200 NVL**, **RTX 4090**) with **FP8/AWQ quantization**, tensor-parallel topologies, and concurrency tuning for cost-efficient inference under children's data-privacy constraints.

---

### **NAVER Start Up Factory – DeepAuto AI**  
_August 2023 – January 2024_  
**Platform Engineer Intern**

- Developed a Diffusion Image Generator web platform, enhancing AI-based image synthesis.  
- Designed and implemented client-side web features using **React** and **TypeScript**.  
- Integrated APIs with backend servers and databases to improve system interoperability.

---

### **Digital Company Inc.**  
_March 2021 – December 2021_  
**Web Development Intern (Remote)**

- Developed marketing websites using **TypeScript** and **React**, ensuring modern UI design.  
- Ensured responsive design and cross-browser compatibility, improving user experience.

---

## Projects  

- **AI Image Generator (GenAIOps)** — *Fall 2023* — ML-model visualization & optimization web platform with real-time performance monitoring — [YouTube](https://www.youtube.com/watch?v=yq9ckGJPQOE) · *Python, FastAPI, React, Next.js*
- **AI Model Optimization Statistics Viewer** — *Winter 2023* — real-time dashboards for AI-model performance metrics — [YouTube](https://www.youtube.com/watch?v=7fE3d_xOX0A) · *TypeScript, React, Redux, ECharts*

**Academic projects** (SJTU · ECE coursework):

- **Multi-Threading Database** — *Fall 2024* — 64-thread-optimized database with semaphore-based read/write locking — [GitHub](https://github.com/renaissancor/ECE482P2) · *C++, PThreads, SQLite*
- **Lottery Scheduling (Linux Kernel)** — *Winter 2024* — eBPF ticket-based scheduler with shared dispatch queues — [GitHub](https://github.com/renaissancor/ECE482P3) · *C, eBPF, Linux Kernel*
- **Big Data Million Song** — *Summer 2024* — distributed recommendation over the Million Song Dataset — [GitHub](https://github.com/renaissancor/ECE472P1) · *Hadoop, Spark, Python*
- **Web App & Database Design** — *Summer 2024* — capstone-tracking web app with OAuth + decentralized DB — [GitHub](https://github.com/renaissancor/ECE450SU24) · *MySQL, React, Node.js*

---

_Last updated: July 2026_
