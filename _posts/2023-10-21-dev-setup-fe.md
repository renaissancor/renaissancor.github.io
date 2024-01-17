---
title: Web Front End Development Setup  
date: 2023-10-21 21:15:20 +0900
categories: [Web]
tags: [setup]		# tags MUST be small letters, like tag NOT TAG 
---

## Preparation before Project Setup 

### Package Management Tools : npm or yarn 

If you are comfortable with one of them, use it. 
If both not unfamiliar, npm is recommended 
npm is the largest software registry in the world. 

```bash 
npm init 
npm install 
npm install package_name@version
npm install "http://address"
npm install package_name -g 
npm uninstall 
npm update 
npm dedupe   
```

Install **nvm** [Node Version Manager](https://github.com/nvm-sh/nvm) to use different types of node versions for each different project. 
```bash
$ nvm use 16 # If nvm is installed and npm version 16 is installed in nvm you can switch your npm version like this 
Now using node v16.9.1 (npm v7.21.1)
$ node -v
v16.9.1
$ nvm use 14
Now using node v14.18.0 (npm v6.14.15)
$ node -v
v14.18.0
$ nvm install 12
Now using node v12.22.6 (npm v6.14.5)
$ node -v
v12.22.6
```

The **package.json** file of the npm project manages project information and **dependencies** 

yarn is made by Meta (Facebook). Compared to npm, yarn has beter performance, stability, security. 
However, npm has longer history and their continuous update reduced performance gap between yarn and npm. 
Both npm and yarn are great package managers to use, but I have used npm. 

## Start React Project 

- [Create a new React App](https://legacy.reactjs.org/docs/create-a-new-react-app.html)
- [Create a new Next.js App](https://nextjs.org/learn-pages-router/basics/create-nextjs-app/setup)

Example codes to setup 
```bash
npx create-next-app@latest client --typescript --tailwind --eslint
```

## Git GitHub gitignore Setup 

Start git 

### CSS or Tailwind CSS 

### UI Component Collection Tools 

- [Radix UI](https://www.radix-ui.com/)  
- [Shadcn UI](https://ui.shadcn.com/) based on Radix UI, automatically installs UI 
- [Tailwind UI](https://tailwindui.com/)
- [Segment](https://segment.com/)

### Eslint : Static Code Analysis Tool 

[Eslint](https://eslint.org/)

### Prettier : Opinionated Code Formatter 

[Prettier](https://prettier.io/)

# Deployment 