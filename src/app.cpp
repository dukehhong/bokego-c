#include <iostream>
#include <string>
#include <chrono> 
#include "board.h"
#include "policy_net.h"
#include "torch/script.h"
#include "data_loader.h"

#include<fstream>
typedef std::tuple<std::string, std::string, int, std::string> sample;

void makeFeatues(DataLoader &dl, PolicyNet &model, std::vector<std::string>* lines, int i, int j){
  Board game = Board(9);
  for(int k=i; k<j; k++){
    sample attr = dl.makeSample((*lines)[k]);
    game.loadState(std::get<0>(attr), std::get<2>(attr), std::get<1>(attr));
    model.features(game);
  }
}

int main(int argc, const char* argv[]) {
  DataLoader dl = DataLoader();
  dl.mount("../data/boards_aug.csv");
  std::cout << "Loading data..." << std::endl;
  dl.process(0, 10000);
  std::cout << "Finished loading data..." << std::endl;
  auto s = dl.getSamples();

  Board game = Board(9);
  PolicyNet model = PolicyNet();

  int num_workers = 10;
  int partition = (*s).size()/num_workers;
  std::vector<std::thread> threads;

  for(int i = 0; i<num_workers; i++){
    threads.push_back(std::thread(makeFeatues, std::ref(dl), std::ref(model), s, i*partition, (i+1)*partition));
  }

  std::cout << "Making features..." << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  for(auto &thread : threads){
    thread.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Finished making features..." << std::endl;

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Time: " << duration.count()*0.000001 << " seconds" << std::endl;
  
  return 0;
}