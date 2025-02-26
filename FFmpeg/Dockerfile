FROM ubuntu:latest AS base_image
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /FFmpeg
COPY . .
RUN  apt-get update && apt-get install -y \
	libavformat-dev\
	libavcodec-dev\
	libswscale-dev\
	libsdl2-dev\
	gcc

RUN gcc -g Play.c -L/usr/lib/x86_64-linux-gnu/ -lavcodec -lavformat -lswresample -lavutil -lswscale -lSDL2 -o dplay
CMD ["ls"]

 
