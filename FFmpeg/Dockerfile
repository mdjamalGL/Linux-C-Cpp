FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /FFmpeg
COPY . .
SHELL ["/bin/bash", "-c"]
RUN  apt-get update && apt-get install -y pipx \
		cmake \
		gcc \
		build-essential \
		pkg-config && \
		rm -rf /var/lib/apt/lists/* &&\
		export PATH=/root/.local/bin:$PATH && \
		pipx install conan && \
		pipx ensurepath && \
		conan profile detect && \
		conan install . --build=missing --output-folder=build -c tools.system.package_manager:mode=install

CMD ["conan", "-v"]
 
