FROM ubuntu:latest
COPY ./docker_entrypoint.sh /docker_entrypoint.sh
RUN apt-get update -y && \
    apt-get install -y build-essential cmake git gcc g++ \
                        libgl1-mesa-dev libsoil-dev libglm-dev \
                        libassimp-dev libglew-dev libglfw3-dev libxinerama-dev \
                        libxcursor-dev libxi-dev mesa-common-dev mesa-utils \
                        libxxf86vm-dev libfreetype6-dev assimp-utils && \
                        mkdir /code && mkdir /output && chmod +x /docker_entrypoint.sh

ENTRYPOINT [ "/docker_entrypoint.sh" ]
