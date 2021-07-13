FROM rikorose/gcc-cmake
MAINTAINER a.kiselev.fw@gmail.com
COPY ./algorithms/ /usr/src/algorithms/
WORKDIR /usr/src/algorithms/
RUN cmake . && make
ENTRYPOINT ./algorithms
