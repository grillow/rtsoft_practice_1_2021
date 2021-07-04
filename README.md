# RTSoft task | Tenyaev Oleg
### Task
![Task](task.jpg)
### Run
```
mkdir -p build && cd build && cmake .. && make && cd ..
cat test_TEMP.txt | ./bin/task1
```
or
```
docker build . -t grillow1337/rtsoft
docker run grillow1337/rtsoft --rm
```
