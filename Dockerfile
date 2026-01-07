FROM gcc:13

# Install CMake and other build dependencies
RUN apt-get update && \
    apt-get install -y cmake git && \
    rm -rf /var/lib/apt/lists/*

COPY ./algorithms/ /usr/src/algorithms/
WORKDIR /usr/src/algorithms/

# Build the project
RUN cmake . && make

ENTRYPOINT ["./algorithms"]
