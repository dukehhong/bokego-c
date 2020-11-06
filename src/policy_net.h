#include <torch/nn.h>
#include "board.h"

#ifndef POLICY_NET_H
#define POLICY_NET_H

#define BORDER -1
#define EMPTY 0
#define BLACK 1
#define WHITE 2

struct PolicyNet : torch::nn::Module{
    PolicyNet(){
        conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(10, 10, 5).padding(2)));
        conv2 = register_module("conv2", torch::nn::Conv2d(torch::nn::Conv2dOptions(10, 12, 3).padding(1)));
        conv3 = register_module("conv3", torch::nn::Conv2d(torch::nn::Conv2dOptions(12, 128, 3).padding(1)));
        conv4 = register_module("conv4", torch::nn::Conv2d(torch::nn::Conv2dOptions(128, 128, 3).padding(1)));
        conv5 = register_module("conv5", torch::nn::Conv2d(torch::nn::Conv2dOptions(128, 128, 5).padding(2)));
        conv6 = register_module("conv6", torch::nn::Conv2d(torch::nn::Conv2dOptions(128, 1, 3).padding(1)));
        
        scale = 100;
    }

    torch::Tensor forward(torch::Tensor x){
        x = torch::relu(conv1->forward(x));
        x = torch::relu(conv2->forward(x));
        x = torch::relu(conv3->forward(x));
        x = torch::relu(conv4->forward(x));
        x = torch::relu(conv5->forward(x));
        x = torch::log_softmax(conv6->forward(x), 0);
        return x;
    }

    torch::Tensor features(Board &board){
        torch::Tensor input = torch::zeros({19,9,9});
        int lib, libAfterPlay, gsize, color, player, valid, turn;
        turn = (board.getTurn() + 1) % 2;
        player = turn == 1 ? BLACK : WHITE;
        for(int i = 0; i<9; i++){
            for(int j = 0; j<9; j++){
                color = board.getPos(i+1,j+1);
                valid = board.isValidMove(player, i+1 , j+1); 

                //position color, 0 - 2
                if(color == 0){
                    input[2][i][j] = 2;
                }else{
                    input[ (color+2)%3 ][i][j] = 1;
                }
                //turns, 3
                input[3][i][j] = turn;
                
                //lib, 5-11
                if(color != EMPTY){
                    lib = board.getLibAndSize(color, i+1, j+1).first;
                    lib = lib > 7 ? 7 : lib;
                    input[4 + lib][i][j] = lib;
                }
                
                if(valid){
                    //valid, 4
                    input[4][i][j] = 1;
                    //lib after play, 12-18
                    libAfterPlay = board.getLibsAfterPlay(player, i+1, j+1);
                    libAfterPlay = libAfterPlay > 7 ? 7 : libAfterPlay;
                    input[11+ libAfterPlay][i][j] = libAfterPlay;
                }
            }
        }
        return input;
    }

    int scale;
    torch::nn::Conv2d conv1{nullptr}, conv2{nullptr}, conv3{nullptr}, conv4{nullptr}, conv5{nullptr}, conv6{nullptr};
};

#endif