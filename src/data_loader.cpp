#include "data_loader.h"

sample makeSample(std::string line){
    std::string buffer;
    std::stringstream ss(line);
    std::string arr[4];
    int i = 0;
    while(getline(ss, buffer, ',')){
        arr[i] = buffer;
        i++;
    }
    return std::make_tuple(arr[0], arr[1], std::stoi(arr[2]), arr[3]);
}

DataLoader::DataLoader(){
    this->isMounted = false;
    this->file = "";
}

void DataLoader::mount(std::string file){
    rs.open(file);
    if(!this->rs.is_open()){
        throw std::runtime_error("file cannot be open");
    }
    this->file = file;
    this->isMounted = true;
}

void DataLoader::process(int min, int max){
    std::string line, buffer;
    if(!this->isMounted){
        throw std::runtime_error("file is not mounted");
    }
    //remove column names
    getline(this->rs, line);

    for(int i = 0; i<min; i++){
        getline(this->rs, line);
    }

    auto start = std::chrono::high_resolution_clock::now();
    double count = 0, total = 0;
    while(getline(this->rs, line) && total != max){
        if(line != ""){
            if(count == 500000){
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << "num of samples/s: " << count/(double)(duration.count()*0.000001) << std::endl;
                count = 0;
                start = end;
            }else{
                count++;
            }
            this->samples.push_back(line);
            total++;
        }
    }
}

std::vector<std::string>* DataLoader::getSamples(){
    return &this->samples;
}

int DataLoader::size(){
    return this->samples.size();
}