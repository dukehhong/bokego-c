# bokego-c 

Based off Google's DeepMind AlphaGo<sup>[1](#1)</sup>, BokeGo is a 9x9 Go bot trained by deep learning. 

## Development
- [x] parse SGF game files
- [x] policy network + features
- [ ] train policy net through GCP
- [ ] monte carlo tree search


## Build
```
mkdir build
../c_build
make
```
Run application:
```
./app
```

## Dependencies
<ul>
<li>libtorch: https://pytorch.org/cppdocs</li>
<li>C++11</li>
<li>Python3</li>
</ul>

## References
<div><a name="1">1</a>: https://www.nature.com/articles/nature16961</div>
