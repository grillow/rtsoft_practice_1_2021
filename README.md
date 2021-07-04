# RTSoft task | Tenyaev Oleg
![Task](task.jpg)
### Run
```
mkdir -p build && cd build && cmake .. && make && cd ..
cat test_TEMP.txt | ./bin/task1
```
### OR
```
docker build . -t grillow1337/rtsoft
docker run grillow1337/rtsoft --rm
```
