---
title: Docker  
date: 2024-03-01 21:15:20 +0900
categories: [DevOps, Docker]
tags: [Docker]		# tags MUST be small letters, like tag NOT TAG 
---
Your blog post provides a concise tutorial on using Docker for managing containers and images, including running a web server using the `httpd` image. I'll help you reorganize and refine your post for clarity and flow.

---

In this blog post, we'll explore the basics of Docker, focusing on image management, container operations, and networking concepts. Docker simplifies the deployment of applications inside software containers, ensuring that they work seamlessly in any environment.

## Getting Started with Docker

To begin, let's check if Docker is installed successfully on your system. You can do this by running the following command:

```
docker images
```

If Docker is installed correctly, you'll see a message indicating success.

### Pulling an Apache Server Image

To pull the Apache server image, use:

```
docker pull httpd
```

After pulling the image, you can verify it's listed among your Docker images with:

```
docker images
```

### Running an Apache Server Container

To run a container based on the `httpd` image, execute:

```
docker run httpd
```

This command creates and runs a container based on the Apache server image. You can view the running container by using:

```
docker ps
```

### Naming Containers and Checking Logs

For more control, you can name your containers during creation:

```
docker run --name ws2 httpd
```

To view logs of your container in real-time:

```
docker logs -f ws2
```

## Managing Containers

Stopping a running container:

```
docker stop ws2
```

To see all containers, including stopped ones:

```
docker ps -a
```

To remove a container:

```
docker rm ws2
```

Note: You cannot remove a running container. First, stop it or use `--force`.

## Removing Docker Images

To remove an image:

```
docker rmi httpd
```

## Docker Networking: Port Forwarding

Docker containers and the host are independent environments with their own ports and file systems. To connect a host port to a container port (port forwarding), use:

```
docker run -p host_port:container_port httpd
```

For example, to connect host port 80 to container port 80:

```
docker run -p 80:80 httpd
```

### Changing Port Access

If you need to change the port access, for instance, from `http://example.com:80/index` to `http://example.com:8000/index`, run:

```
docker run --name ws3 -p 8081:80 httpd
```

### Serving Custom Web Content

To serve custom content from your host machine through Docker, you can use the volume option to bind a directory on your host to the container:

```
docker run -p 8888:80 -v ~/Desktop/htdocs:/usr/local/apache2/htdocs/ httpd
```

This command maps a directory from your host (`~/Desktop/htdocs`) to the directory inside the container where `httpd` expects web content (`/usr/local/apache2/htdocs/`).

---

By following these steps, you can leverage Docker to simplify the deployment and management of your applications and services. Docker's containerization technology ensures that your application runs smoothly in any environment, from development to production.