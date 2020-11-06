#include "policy_net.h"
#include <iostream>
#include <string>
#include <fstream>
#include "board.h"
#include <chrono> 
#include <torch/script.h>

void parseCSV(std::string &buffer, std::string board, std::string ko, int turn, std::string next){
  std::stringstream ss(buffer);
  std::getline(ss, buffer, ','); board = buffer;
  std::getline(ss, buffer, ','); ko = buffer;
  std::getline(ss, buffer, ','); turn = std::stoi(buffer);
  std::getline(ss, buffer, ','); next = buffer;
}

int main(int argc, const char* argv[]) {
  // if (argc != 2) {
  //   std::cerr << "usage: example-app <path-to-exported-script-module>\n";
  //   return -1;
  // }


  // torch::jit::script::Module module;
  // try {
  //   // Deserialize the ScriptModule from a file using torch::jit::load().
  //   module = torch::jit::load(argv[1]);
  // }
  // catch (const c10::Error& e) {
  //   std::cerr << "error loading the model\n";
  //   return -1;
  // }


  //read csv
  std::ifstream rs("../data/boards_aug.csv");
  if(!rs.is_open()){
    throw std::runtime_error("file cannot be open");
  }
  std::string buffer, board, next, ko;
  int turn;
  torch::Tensor sample;

  //remove column names
  std::getline(rs, buffer);

  for(int i = 0; i<100; i++){
    std::getline(rs, buffer);
  }
  PolicyNet model = PolicyNet();
  Board game = Board(9);
  parseCSV(buffer, board, ko, turn, next);
  game.loadState(board, turn, ko);
  sample = model.features(game);

  // std::vector<torch::jit::IValue> inputs;
  // inputs.push_back(sample.unsqueeze(0));
  // at::Tensor output = module.forward(inputs).toTensor();
  // sample = torch::softmax(output.squeeze(0), 0);
  // std::cout << torch::argmax(sample) << '\n';

  return 0;
}