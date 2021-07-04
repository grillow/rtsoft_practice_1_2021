FROM gcc:11
RUN apt-get update && apt-get install -y cmake
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN mkdir build && cd build && cmake .. && make
CMD cat ./test_TEMP.txt | ./bin/task1
